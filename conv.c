/* Number conversion */
/* Coded by Quan Tran */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int lt(int num, int so_mu) 
{
	int kq, i;

	kq = 1;
	for (i = 1; i <= so_mu; i++) kq = kq * num;

	return kq;
}

int cs(int num) 
{
	int kq;

	for (kq = 0; num != 0; kq++) num = num / 10;

	return kq;
}

int b_to_d(char *x) 
{
	int bin[100], binary, m, i, remain, n, kq;
	
	kq = 0;
	binary = atoi(x);
	m = cs(binary) - 1;

	for (i = m; i >= 0; i--) {
		remain = binary % 10;
		bin[i] = remain;
		binary = (binary - remain) / 10;
	}
	
	n = m;
	for (i = 0; i <= m; i++, n--) kq = kq + bin[i] * lt(2, n);

	return kq;
}

void d_to_b(char *x) 
{
	int bin[100], remain, dec, m, i;
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

void usage() 
{
	printf("Usage: ./conv [-db] [-bd] [-bh] [-dh] number.\n\n");
	printf("Options:\n");
	printf("        -db: Decimal to binary.\n\n");
	printf("        -bd: Binary to decimal.\n\n");
	printf("        -bh: Binary to hexadecimal.\n\n");
	printf("        -dh: Decimal to hexadecimal.\n");
}

int main(int argc, char **argv) 
{
	if (argc == 1) {
		usage();
		exit(1);
	}
	
	if (!strcmp("-db", argv[1]) && argc == 3) d_to_b(argv[2]);
	else if (!strcmp("-bd", argv[1]) && argc == 3) printf("%d\n", b_to_d(argv[2]));
	else if (!strcmp("-bh", argv[1]) && argc == 3) b_to_h(argv[2]);
	else if (!strcmp("-dh", argv[1]) && argc == 3) d_to_h(argv[2]);
	else usage();
	
	return 0;
}
