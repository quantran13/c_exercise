#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) 
{
	char *buffer;
	
	buffer = (char *) malloc(2048);
	char *next = buffer + 2048;	
	char *next_2 = buffer + 4096;
	
	printf("%p\n", &buffer);
	strcpy(next, argv[1]); printf("Not the first one: %p\n", &next);
	strcpy(next_2, argv[1]); printf("Not the second one: %p???\n", &next_2);

	free(buffer);

	return 0;
}
