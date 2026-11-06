#include <stdio.h>

int main(void)
{
	int* pi = NULL;
	char* pc = NULL;
	double* pd = NULL;

	int num = 10;
	char val = 'A';
	double dnum = 10.5;

	pi = &num;
	pc = &val;
	pd = &dnum;

	printf("*pi = %d, num=%d \n", *pi, num);
	printf("*pc = %c, val=%c \n", *pc, val);
	printf("*pd = %lf, dnum=%lf \n", *pd, dnum);

	return 0;
}