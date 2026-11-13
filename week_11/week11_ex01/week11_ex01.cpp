#include <stdio.h>
#include <string.h>

int main(void)
{
	int i = 0;
	char str[4]{ 0. };
	char* pStr = NULL;
	
	pStr = str;

	const char* Hello_str = "Welcome to Korea";
	char Helllo_str2[100];

	Hello_str2[0] = 'W';

	str[0] = 'a';
	str[1] = 'b';
	str[2] = 'c';
	str[3] = '\0';
	
	i = 0;
	while (str[i] != '\0') {
		printf("%c", str[i]);
		i++;
	}

	*(pStr) = 'A';  // pStr[0]: 
	*(pStr + 1) = 'B';  // pStr[1]:
	*(pStr + 2) = 'C';  // pStr[2]:
	*(pStr + 3) = '\0'; // pStr[3]:

	i = 0;
	while (str[i] != '\0') {
		printf("%c", str[i]);
		i++;
	}
	return 0;
}