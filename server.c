/*
** server.c -- a stream socket server demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT "1234"
#define BACKLOG 10
#define MAXDATARECV 100

int main(int argc, char **argv)
{
	struct addrinfo hints, *res, *p;
	struct sockaddr_storage client_addr;
	socklen_t addr_size;
	char s[INET6_ADDRSTRLEN];
	char *buf, *mes;
	int sockfd, new_fd, status;
	int yes=1;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if ((status = getaddrinfo(NULL, PORT, &hints, &res)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
		return 1;
	}

	for(p = res; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			perror("server: socket");
			continue;
		}

		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
			perror("setsockopt");
			exit(1);
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("server: bind");
			continue;
		}

		break;
	}

	if (p == NULL)  {
		fprintf(stderr, "server: failed to bind\n");
		exit(2);
	}

	freeaddrinfo(res);

	if (listen(sockfd, BACKLOG) == -1) {
		perror("listen");
		exit(1);
	}

	printf("Waiting for connection...\n");
	
	while (1) {
		buf = (char *) malloc(100);
		mes = (char *) malloc(20);

		addr_size = sizeof(client_addr);
		new_fd = accept(sockfd, (struct sockaddr *)&client_addr, &addr_size);
		if (new_fd == -1) {
			//perror("accept");
			continue;
		}

		inet_ntop(client_addr.ss_family, &(((struct sockaddr_in*)&client_addr)->sin_addr), s, sizeof(s));
		printf("server: got connection from %s\n", s);

		if (!fork()) {
			if (recv(new_fd, buf, MAXDATARECV-1, 0) == -1)
				perror("recv");

			if (!strcmp(buf, "GET")) 
				strcpy(mes, "GET successfully!\n");
			else if (!strcmp(buf, "POST"))
				strcpy(mes, "POST successfully!\n");
			else strcpy(mes, "Unknown request!\n");
			
			if (send(new_fd, mes, strlen(mes), 0) == -1) 
				perror("send");
				
			close(new_fd);
			printf("server: connection from %s closed\n", s);

			exit(0);
		}

		free(buf); free(mes);
	}
	
	close(sockfd);

	return 0;
}
