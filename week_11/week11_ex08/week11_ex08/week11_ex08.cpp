#include <string.h>
#include <stdio.h>

int main(void)
{
	char string[80];

	strcpy_s(string, sizeof(string), "Hello world from ");
	strcat_s(string, sizeof(string), "strcpy ");
	strcat_s(string, sizeof(string), "and ");
	strcat_s(string, sizeof(string), "strcat!");
	printf("string = %s\n", string);
	return 0;
}