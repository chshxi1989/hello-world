#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>

#define PORT_NO (1111)
#define BUFFER_LEN (128)

static int socketfd = -1;

int sig_handle(int signum)
{
    if (signum == SIGINT)
    {
        if (socketfd != -1)
        {
            close(socketfd);
        }
        exit(0);
        return 0;
    }
}
int get_line(int connectfd, char* buffer, int length)
{
    int ret = -1;
    char c;
    int i = 0;
    while(1)
    {
        ret = recv(connectfd, &c, 1, 0);
        printf("char: %c\n", c);
        if (ret == -1)
        {
            printf("can not read data from client socket\n");
            return -1;
        }
        if (i < length)
        {
            buffer[i++] = c;
        }
        else
        {
            printf("buffer overflow\n");
            return -1;
        }
        // check if \r\n
        if (c == '\r')
        {
            // check next char is \n
            ret = recv(connectfd, &c, 1, MSG_PEEK);
            if (ret != -1 && c == '\n')
            {
                recv(connectfd, &c, 1, 0);
                buffer[i++] = c;
                buffer[i++] = '\0'; // null-terminated
                return 0;
            }
        }
    }
    return -1;
}

int handle_request(int* pconnectfd)
{
    int ret = -1;
    // set thread to detach status
    pthread_detach(pthread_self());
    int connectfd = (int)pconnectfd;
    if (connectfd <= 0)
    {
        printf("connectfd can not small than 0\n");
        return -1;
    }
    //get a line from client
    char buffer[BUFFER_LEN] = "\0";
    ret = get_line(connectfd, buffer, BUFFER_LEN);
    if (ret != 0)
    {
        printf("get data from client failed\n");
        close(connectfd);
        return -1;
    }
    printf("get line from client data :\n");
    printf("%s\n", buffer);
    // GET method
    char resPath[BUFFER_LEN] = "\0";
    int i = 0;
    if (strncmp(buffer, "GET", strlen("GET")) == 0)
    {
        // parse resource path
        char* p = resPath;
        char * q = buffer + strlen("GET") + 1;
        while (*q != ' ')
        {
            *p++ = *q++;
        }
        *p = '\0';
        printf("resource path: %s\n", resPath);
        // read resource data and send to client
        int fd = open(resPath, O_RDONLY);
        if (fd == -1)
        {
            printf("can not open file %s, %s\n", resPath, strerror(errno));
            return -1;
        }
        int len;
        char buffer_read[1024];
        while(len = read(fd, buffer, 1024))
        {
            send(connectfd, buffer, len, 0);
        }
    }
    close(connectfd);
    return 0;
}

int main(int argc, char** argv)
{
    int connectfd = -1;
    int ret = -1;
    signal(SIGINT, sig_handle);
    // create socket
    socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd == -1)
    {
        printf("can not allocate socket fd, %s\n", strerror(errno));
        return -1;
    }
    // bind socket fd and port
    struct sockaddr_in sockargs;
    memset(&sockargs, 0, sizeof(sockargs));
    sockargs.sin_family = AF_INET;
    sockargs.sin_port = htons(PORT_NO);
    sockargs.sin_addr.s_addr = htonl(INADDR_ANY);
    ret = bind(socketfd, &sockargs, sizeof(sockargs));
    if (ret != 0)
    {
        printf("can not bind socket and port %d, %s\n", PORT_NO, strerror(errno));
        return -1;
    }
    
    // set socket to listen status
    ret = listen(socketfd, 5);
    if (ret != 0)
    {
        printf("can not listen socket and port %d, %s\n", socketfd, strerror(errno));
        return -1;
    }
    
    // wait for message for client
    struct sockaddr_in clientargs;
    memset(&clientargs, 0, sizeof(clientargs));
    int clientargs_len = sizeof(clientargs);
    pthread_t tid = -1;
    while(1)
    {
        connectfd = accept(socketfd, &clientargs, &clientargs_len);
        printf("get client connect request, connectfd: %d\n", connectfd);
        if (connectfd == -1)
        {
            printf("accept client request failed, %s\n", strerror(errno));
            continue;
        }
        ret = pthread_create(&tid , NULL, handle_request, (void*)connectfd);
        if (ret != 0)
        {
            printf("pthread_create failed, %s\n", strerror(errno));
        }
    }
    close(socketfd);
    return 0;
}