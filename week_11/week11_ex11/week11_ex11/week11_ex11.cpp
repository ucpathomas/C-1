#include <string.h>
#include <stdio.h>

int main(void)
{
	char s[] = "A bird in hand is worth two in the bush";
	char sub[] = "bird";
	char* p;
	int loc;

	p = strstr(s, sub);
	if (p == NULL)
		printf("%s가 발견되지 않았음\n", sub);
	else {
		loc = (int)(p - s);
		printf("%s에서 첫 번째 %s가 %d에서 발견되었음\n", s, sub, loc);
	}
	return 0;
}