#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main(void)
{
	char instring[] = "file 12";
	char name[10];
	int number;

	sscanf(instring, "%s %d", name, &number);

	printf("name = %s \n", name);
	printf("number = %d \n", number);
	return 0;
}