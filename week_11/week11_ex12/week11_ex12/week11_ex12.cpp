#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include <stdio.h>

int main(void)
{
	char s[] = "Man is immortal, because he has a soul";

	char* t1 = strtok(s, " ");		// 첫 번째 토큰: Man
	char* t2 = strtok(NULL, " ");	// 두 번째 토큰: is
	char* t3 = strtok(NULL, " ");	// 세 번째 토큰: immortal, 
	char* t4 = strtok(NULL, " ");	// 네 번째 토큰: because

	return 0;
}