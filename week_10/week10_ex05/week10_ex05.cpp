#include <stdio.h>

int main(void)
{
	char* pc;
	int* pi;
	double* pd;

	pc = (char*)10000;
	pi = (int*)10000;
	pd = (double*)10000;
	printf(" pc=%u,  pc+1=%u,  pc+2= %u\n", pc, pc + 1, pc + 2);
	printf(" pi=%u,  pi+1=%u,  pi+2= %u\n", pi, pi + 1, pi + 2);
	printf(" pd=%u,  pd+1=%u,  pd+2= %u\n", pd, pd + 1, pd + 2);

	return 0;
}