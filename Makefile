all: server client

server: server.c
	gcc -W -Wall -pthread -o server server.c

client: client.c
	gcc -W -Wall -pthread -o client client.c
clean:
	rm server
	rm client