/*
** server.c -- a stream socket server demo
** Usage: use netcat to talk to the server
** Return "GET/POST successfully!" on GET/POST requests
** Return "Unknown request!" on other requests
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

#define MAXDATARECV 100
#define PID "/home/quan/bt/btc/network/server/server.pid"
#define CONFIGURATION_FILE "/home/quan/bt/btc/network/server/server.conf"

char PORT[100] = "1234";
int BACKLOG = 10;
char LOGFILE[1024] = "/home/quan/bt/btc/network/server/server.log";
char ERRORLOG[1024] = "/home/quan/bt/btc/network/server/error.log";

/* Usage */
void usage(char *program)
{
	printf("Usage: %s <start|stop|restart|status>\n", program);
	exit(0);
}

/* Check whether the pid file exists */
int check()
{
	int fd, i;

	fd = open(PID, O_WRONLY);
	i = fd;
	close(fd);

	return i;
}

/* A function to read the current pid of the server program */
int id()
{
	FILE *file;
	char c, *tmp = (char *) malloc(100);
	int i, count = 0;

	file = fopen(PID, "r");
	if (file) {	
		while ((c = getc(file)) != EOF) {
			tmp[count] = c;
			count++;
		}
		tmp[count] = '\0';
		fclose(file);
	}

	i = atoi(tmp);
	free(tmp);
	
	return i;
}

/* A function to strip the newline character */
void strip(char *buf)
{
	int len = strlen(buf);

	if (buf[len-1] == '\n') buf[len-1] = '\0';
}

/* A function to read configurations */
void read_conf(char *file)
{
	FILE *f = fopen(file, "r");
	char *tmp = (char *) malloc(1024);
	int count = 1;

	while (fgets(tmp, 1024, f) != NULL) {
		strip(tmp);

		if (tmp[0] != '#' && strcmp(tmp, "")) {
			if (count == 1) strcpy(PORT, tmp);
			else if (count == 2) BACKLOG = atoi(tmp);
			else if (count == 3) strcpy(LOGFILE, tmp);
			else if (count == 4) strcpy(ERRORLOG, tmp);

			count++;
		}
	}

	fclose(f);
	free(tmp);
}

/* A function to stop the server */
void stop()
{
	char tmp[100], command[100] = "kill ";

	if (check() == -1) {
		printf("server is not running!\n");
		exit(0);
	}
		
	printf("server is stopping...\n");
	sprintf(tmp, "%d", id());
	strcat(command, tmp);

	system(command);
	system("rm server.pid");
}

/* A function to get current time */
void get_time(char *kq)
{
	char *t = (char *) malloc(100);
	time_t lt;
	int len;

	lt = time(NULL);
	strcpy(t, ctime(&lt));
	len = strlen(t) - 6;
	t[len] = '\0';

	strcpy(kq, "["); strcat(kq, t); strcat(kq, "]");
	free(t);
}

/* A function to log errors */
void error_log(char *mes)
{
	char *tmp = (char *) malloc(100);
	char *t = (char *) malloc(50);
	int fd;

	fd = open(ERRORLOG, O_WRONLY|O_CREAT|O_APPEND, S_IRUSR|S_IWUSR);
	if (fd == -1)
		return;

	get_time(t);
	strcpy(tmp, t);
	strcat(tmp, " Error: "); strcat(tmp, mes); strcat(tmp, "\n");

	write(fd, tmp, strlen(tmp));
	free(tmp);

	close(fd);
}

/* A function to log requests to the server */
void logging(char *ip, char *buf)
{
	char *tmp = (char *) malloc(200);
	char *t = (char *) malloc(100);
	int fd;

	fd = open(LOGFILE, O_WRONLY|O_CREAT|O_APPEND, S_IRUSR|S_IWUSR);
	if (fd == -1) 
		error_log("open log file");

	get_time(t);
	strcpy(tmp, ip); strcat(tmp, " - - ");
	strcat(tmp, t); strcat(tmp, "  \"");
	
	strcat(tmp, buf); 
	if (!strcmp(buf, "GET") || !strcmp(buf, "POST")) 
		strcat(tmp, " 200 0\"");
	else strcat(tmp, " 404 1\"");

	strcat(tmp, "  \"-\"  \"-\"");
	strcat(tmp, "\n\0");

	if (write(fd, tmp, strlen(tmp)) == -1)
		error_log("write to log file");

	free(tmp), free(t);
	close(fd);
}

int main(int argc, char **argv)
{
	struct addrinfo hints, *res, *p;
	struct sockaddr_storage client_addr;
	socklen_t addr_size;
	char s[INET6_ADDRSTRLEN];
	pid_t pid, sid;
	int sockfd, new_fd, status, pid_fd, len;
	int yes=1;

	/*read_conf(CONFIGURATION_FILE);
	printf("%s", PORT);
	printf("%d\n", BACKLOG);
	printf("%s", LOGFILE);
	printf("%s", ERRORLOG);*/
	
	/* Check the arguments */
	if (argc != 2) usage(argv[0]);							/* Check the command's syntax */

	if (!strcmp(argv[1], "stop"))  {						/* Stop the server */
		stop();
		exit(0);
	} else if (!strcmp(argv[1], "status")) {				/* Get the status of the server */
		if (check() == -1) {								/* If the PID file doesn't exist */
			printf("server is not running\n");				
			exit(0);
		} else {											/* If the PID file exists */
			pid = id();										/* print the status and pid */
			printf("server is running, pid: %d\n", pid);
			exit(0);
		}
	} else if (!strcmp(argv[1], "start")) {					/* Start the server */
		if (check() != -1) {								/* If the PID file doesn't exist */
			printf("server is already running!\n");			/* print current status */
			exit(0);
		}

		printf("server is starting...\n");					/* Else start the server */
	} else if (!strcmp(argv[1], "restart")) {				/* Restart the server */
		stop();												/* Stop then start it again */
		printf("server is starting...\n");
	} else usage(argv[0]);
	
	/* Create the daemon process */
	pid = fork();
	if (pid < 0)
		exit(EXIT_FAILURE);
	else if (pid > 0)
		exit(EXIT_SUCCESS);

	if ((sid = setsid()) < 0)
		exit(EXIT_FAILURE);

	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	
	/* Log the process id */
	int parent_pid = getpid();
	char process[10];

	sprintf(process, "%d", parent_pid);
	pid_fd = open(PID, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR);
	if  (pid_fd == -1) error_log("create pid file");
	if (write(pid_fd, process, strlen(process)) == -1)
		error_log("write pid to file");
	close(pid_fd);

	/* Main part of the program */

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
			error_log("in socket()");
			continue;
		}

		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
			error_log("in setsockopt()");
			exit(1);
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			error_log("in bind()");
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
		error_log("in listen()");
		exit(1);
	}
	addr_size = sizeof(client_addr);

	// The big loop
	while (1) {
		new_fd = accept(sockfd, (struct sockaddr *)&client_addr, &addr_size);
		if (new_fd == -1) {
			error_log("in accept()");
			continue;
		}

		inet_ntop(client_addr.ss_family, &(((struct sockaddr_in*)&client_addr)->sin_addr), s, INET_ADDRSTRLEN);

		pid = fork();
	
		if (pid < 0) {
			error_log("in fork()");
			continue;
		} else if (pid == 0) {
			char *buf = (char *) malloc(100);
			char *mes = (char *) malloc(20);

			if (recv(new_fd, buf, MAXDATARECV-1, 0) == -1)
				error_log("in recv()");
		
			len = strlen(buf);
			if (buf[len-1] == '\n') buf[len-1] = '\0';

			if (!strcmp(buf, "GET")) 
				strcpy(mes, "GET successfully!\n");
			else if (!strcmp(buf, "POST"))
				strcpy(mes, "POST successfully!\n");
			else strcpy(mes, "Unknown request!\n");
			
			if (send(new_fd, mes, strlen(mes), 0) == -1) 
				error_log("in send()");
				
			logging(s, buf); 
			close(new_fd);

			free(buf); free(mes);

			_exit(0);
		} else close(new_fd);
	}
	
	close(sockfd);

	return 0;
}
