#include <stdio.h>

int main(void)
{
	const char* p = "HelloWorld";
	printf("%s\n", p);
	
	p = "Welcome to C World!"; // 가능
	printf("%s\n", p);

	p = "Goodbye!"; // 가능
	printf("%s\n", p);
	// p[0] = 'a'; // 오류 발생
	
	return 0;
}