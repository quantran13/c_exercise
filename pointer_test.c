#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) 
{
	char *buffer;
	
	buffer = (char *) malloc(4096);
	char *next = buffer + 4096;	
	char *next_2 = buffer + 2048;
	
	strcpy(next, argv[1]); printf("Not the first one.\n");
	strcpy(next_2, argv[1]); printf("Not the second one???\n");

	free(buffer);

	return 0;
}
