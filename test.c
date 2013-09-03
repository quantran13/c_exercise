// buffer overflow test

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    char *buffer1, *buffer2;

	buffer1 = (char *) malloc(8);
	buffer2 = (char *) malloc(8); 

    strcpy(buffer1, "ab");
    strcpy(buffer2, "456");

    printf("BEFORE:\n");
    printf("           Address            Value\n");
    printf("buffer1    %p         %s\n", buffer1, buffer1);
    printf("buffer2    %p         %s\n\n", buffer2, buffer2);

    strcpy(buffer1, argv[1]);
	
    printf("AFTER COPY argv[1] INTO buffer1:\n");
    printf("           Address            Value\n");
    printf("buffer1    %p         %s\n", buffer1, buffer1);
    printf("buffer2    %p         %s\n\n", buffer2, buffer2);

	free(buffer1); free(buffer2);

    return 0;
}
