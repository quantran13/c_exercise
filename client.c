/*
** client.c -- a stream socket client demo
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

void check(int status, char *mes)
{
	if (status == -1) {
		fprintf(stderr, "Error in %s\n", mes);
		exit(2);
	}
}
int main(int argc, char **argv)
{
	struct addrinfo hints, *res, *p;
	int sockfd, bytes_sent;
	char *mes = (char *) malloc(100);
	char *buf = (char *) malloc(4096);

	if (argc != 3) {
		printf("Usage: %s <hostname> <port>\n", argv[0]);
		exit(1);
	}

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	check(getaddrinfo(argv[1], argv[2], &hints, &res), "getaddrinfo()");

	for (p = res; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			perror("client: socket()");
			continue;
		}

		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("client: connect()");
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "client: failed to connect\n");
		return 2;
	}

	freeaddrinfo(res);

	scanf("%99[^\n]", mes);
	check(bytes_sent = send(sockfd, mes, strlen(mes), 0), "send()");
	check(recv(sockfd, buf, 4095, 0), "recv()");
	printf("%s", buf);
	
	close(sockfd);
	free(mes); free(buf);

	return 0;
}
