#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include "hacking.h"
#define file "/tmp/notes"

void usage(char *program, char *fname)
{
	printf("Usage:\n");
	printf("       %s <text>: add notes to %s\n", program, fname);
	printf("       %s -d: delete %s\n", program, fname);
	exit(0);
}

int main(int argc, char **argv)
{
	int fd, userid;
	char *buffer = (char *) ec_malloc(100);
	
	if (argc < 2)
		usage(argv[0], file);

	userid = getuid();

	if (!strcmp(argv[1], "-d")) {
		if (userid == 1000 || userid == 0) 
			system("rm /tmp/notes");
		else 
			printf("Operation not permitted\n");
		exit(0);
	} else {
		strcpy(buffer, argv[1]);
		fd = open(file, O_WRONLY|O_CREAT|O_APPEND, S_IRUSR|S_IWUSR);
	}

	strncat(buffer, "\n\0", 2);
	if (fd == -1)
		fatal("in main() while opening file\n");

	if (write(fd, &userid, 4) == -1)
		fatal("in main() while writing userid to file\n");
	write(fd, "\n", 1);

	if (write(fd, buffer, strlen(buffer)+1) == -1)
		fatal("in main() while writing data to file\n");
	if (close(fd) == -1)
		fatal("in main() while closing file\n");

	free(buffer);

	return 0;
}
