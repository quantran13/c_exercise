/* Number conversion */
/* Coded by Quan Tran */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

int cs(int num) 
{
	int kq;

	for (kq = 0; num != 0; kq++) num = num / 10;

	return kq;
}

long int b_to_d(char *x) 
{
	int bin[10000], binary, m, i, remain, n;
	long int kq;

	kq = 0;
	binary = atoi(x);
	m = cs(binary) - 1;

	for (i = m; i >= 0; i--) {
		remain = binary % 10;
		bin[i] = remain;
		binary = (binary - remain) / 10;
	}
	
	n = m;
	for (i = 0; i <= m; i++, n--) kq = kq + bin[i] * pow(2, n);

	return kq;
}

void d_to_b(char *x) 
{
	int bin[10000], remain, m, i;
	long int dec;
	float dec_2 = 1;
	
	dec = atoi(x);

	for (m = 0; dec_2 >= 1; m++) {
		dec_2 = (float)dec;
		remain = dec % 2;
		dec_2 = dec_2 / 2; 
		dec = (dec - remain) / 2; 
		bin[m] = remain;
	}
	
	for (i = m-1; i >= 0; i--) printf("%d", bin[i]);
	printf("\n");
}

void b_to_h(char *x)
{
	int dec = b_to_d(x);

	printf("%X\n", dec);
}

void d_to_h(char *x)
{
	int dec = atoi(x);

	printf("%X\n", dec);
}

long int h_to_d(char *x)
{
	int kq, i = 0, dem = 0;
	char ch;
	long int dec = 0;

	for (i = strlen(x)-1; i >= 0; i--) {
		kq = 0;
		ch = x[i];
		if (ch >= '0' && ch <= '9') 
			kq = x[i] - '0';
		else if (ch >= 'A' && ch <= 'F') 
			kq = x[i] - 55;
		else {
			printf("Not a hexadecimal number\n");
			exit(1);
		}
		dec += pow(16, dem) * kq;
		dem++;
	}

	return dec;
}

void h_to_b(char *x)
{
	long int dec = h_to_d(x);
	char *dec_2 = (char *) malloc(100);

	sprintf(dec_2, "%ld", dec);
	d_to_b(dec_2);

	free(dec_2);
}

void usage() 
{
	printf("Usage: ./conv <option> <number>\n\n");
	printf("Options:\n");
	printf("        -db: Decimal to binary.\n");
	printf("        -bd: Binary to decimal.\n");
	printf("        -bh: Binary to hexadecimal.\n");
	printf("        -dh: Decimal to hexadecimal.\n");
	printf("        -hd: Hexadecimal to decimal.\n");
	printf("        -hb: Hexadecimal to binary.\n");
}

int main(int argc, char **argv) 
{
	if (argc == 1) {
		usage();
		exit(1);
	}
	
	if (!strcmp("-db", argv[1]) && argc == 3) d_to_b(argv[2]);
	else if (!strcmp("-bd", argv[1]) && argc == 3) printf("%ld\n", b_to_d(argv[2]));
	else if (!strcmp("-bh", argv[1]) && argc == 3) b_to_h(argv[2]);
	else if (!strcmp("-dh", argv[1]) && argc == 3) d_to_h(argv[2]);
	else if (!strcmp("-hd", argv[1]) && argc == 3) printf("%ld\n", h_to_d(argv[2]));
	else if (!strcmp("-hb", argv[1]) && argc == 3) h_to_b(argv[2]);
	else usage();
	
	return 0;
}
