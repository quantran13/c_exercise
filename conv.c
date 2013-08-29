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

void b_to_d(char *x) 
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
	printf("%d\n", kq);
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

void usage() 
{
	printf("Usage: ./conv [-db] [-bd] number.\n\n");
        printf("Options:\n       -db: Decimal to binary.\n\n       -bd: Binary to decimal.\n\n");
}

int main(int argc, char **argv) 
{
	if (argc == 1) {
		usage();
		return 0;
	}
	
	if (!strcmp("-db", argv[1]) && argc == 3) d_to_b(argv[2]);
	else if (!strcmp("-bd", argv[1]) && argc == 3) b_to_d(argv[2]);
	else usage();
	
	return 0;
}
