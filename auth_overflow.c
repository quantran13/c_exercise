#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int check_auth(char *password)
{
	char password_buffer[16];
	int auth_flag = 0;

	strcpy(password_buffer, password);

	if (strcmp(password_buffer, "quantran") == 0)
		auth_flag = 1;

	printf("password_buffer is at: %p, %p\n", password_buffer, &password_buffer);
	printf("auth_flag is at %p and contains %d\n", &auth_flag, auth_flag);

	return auth_flag;
}

int main(int argc, char **argv)
{
	if (argc != 2) {
		printf("Usage: %s <password>.\n", argv[0]);
		exit(0);
	}

	if (check_auth(argv[1])) {
		printf("\n-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
		printf("      Access Granted.\n");
		printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-\n\n");
	} else 
		printf("\nACCESS DENIED.\n\n");

	return 0;
}
