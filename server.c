/*
** server.c -- a stream socket server demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <netdb.h>
#include <time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT "1234"
#define BACKLOG 10
#define MAXDATARECV 100
#define LOGFILE "server.log"

int openlog()
{
	int fd;

	fd = open(LOGFILE, O_WRONLY|O_CREAT|O_APPEND, S_IRUSR|S_IWUSR);
	if (fd == -1) {	
		close(fd);
		perror("open log file");
		return -1;
	}

	return fd;
}

char *logging(char *ip, char *buf, int fd)
{
	char *tmp = (char *) malloc(200);
	char *t = (char *) malloc(100);
	time_t lt;
	int len;

	lt = time(NULL);
	strcpy(t, ctime(&lt));
	len = strlen(t) - 6;
	t[len] = '\0';

	strcpy(tmp, ip); strcat(tmp, " - - ");
	strcat(tmp, "["); strcat(tmp, t); strcat(tmp, "]  \"");
	
	strcat(tmp, buf); 
	if (!strcmp(buf, "GET") || !strcmp(buf, "POST")) 
		strcat(tmp, " 200 0\"");
	else strcat(tmp, " 404 1\"");

	strcat(tmp, "  \"-\"  \"-\"");
	strcat(tmp, "\n\0");

	if (write(fd, tmp, strlen(tmp)) == -1)
		perror("write to log file");

	free(tmp), free(t);

	return tmp;
}

int main(int argc, char **argv)
{
	struct addrinfo hints, *res, *p;
	struct sockaddr_storage client_addr;
	socklen_t addr_size;
	char s[INET6_ADDRSTRLEN];
	pid_t pid;
	int sockfd, new_fd, status, fd;
	int yes=1;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if ((fd = openlog()) == -1) {
		printf("Error while opening log file");
		exit(1);
	}

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
	addr_size = sizeof(client_addr);

	printf("Waiting for connection...\n");
	
	while (1) {
		new_fd = accept(sockfd, (struct sockaddr *)&client_addr, &addr_size);
		if (new_fd == -1) {
			perror("accept");
			continue;
		}

		inet_ntop(client_addr.ss_family, &(((struct sockaddr_in*)&client_addr)->sin_addr), s, INET_ADDRSTRLEN);
		int len = strlen(s); s[len+1] = '\0';
		printf("server: got connection from %s\n", s);

		pid = fork();
	
		if (pid < 0) {
			perror("fork");
			continue;
		} else if (pid == 0) {
			char *buf = (char *) malloc(100);
			char *mes = (char *) malloc(20);

			if (recv(new_fd, buf, MAXDATARECV-1, 0) == -1)
				perror("recv");
		
			len = strlen(buf);
			if (buf[len-1] == '\n') buf[len-1] = '\0';

			if (!strcmp(buf, "GET")) 
				strcpy(mes, "GET successfully!\n");
			else if (!strcmp(buf, "POST"))
				strcpy(mes, "POST successfully!\n");
			else strcpy(mes, "Unknown request!\n");
			
			if (send(new_fd, mes, strlen(mes), 0) == -1) 
				perror("send");
				
			close(new_fd);
			printf("server: connection from %s closed\n", s);
			logging(s, buf, fd);

			free(buf); free(mes);

			_exit(0);
		} else close(new_fd);
	}
	
	close(sockfd);
	close(fd);

	return 0;
}
