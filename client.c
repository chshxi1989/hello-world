#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUF_MAX (128)
int main(int argc, char *argv[])
{
    int sockfd;
    int len;
    struct sockaddr_in target_addr;
    int result;
    char buffer[BUF_MAX] = "\0";

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    target_addr.sin_family = AF_INET;
    target_addr.sin_addr.s_addr = inet_addr("172.21.120.51");
    target_addr.sin_port = htons(1111);
    len = sizeof(target_addr);
    result = connect(sockfd, (struct sockaddr *)&target_addr, len);

    if (result == -1)
    {
        perror("oops: client1");
        exit(1);
    }
    snprintf(buffer, BUF_MAX, "GET test.txt HTTP/1.1\r\n");
    send(sockfd, buffer, strlen(buffer), 0);
    printf("send msg: %s\n", buffer);
    recv(sockfd, buffer, BUF_MAX, 0);
    printf("receive from server = %s\n", buffer);
    close(sockfd);
    exit(0);
}
