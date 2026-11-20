#include <stdio.h>

int main(void)
{
	char name[100];
	char address[100];

	printf("이름을 입력하시오: ");
	scanf_s("%s", name, (unsigned int)sizeof(name));
	printf("현재 거주하는 주소를 입력하시오: ");
	scanf_s("%s", address, (unsigned int)sizeof(address)); // 두 scanf 사용 시 오류 발생하여 AI에게 질문하고 scanf_s 사용하는 코드로 변경했습니다.

	printf("안녕하세요, %s에 사는 %s씨.\n", address, name);
	return 0;
}