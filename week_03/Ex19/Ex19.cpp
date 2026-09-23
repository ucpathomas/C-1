#define _CRT_SECURE_NO_WARNINGS  // 실행하려고 하면 오류가 계속 나서 AI에게 질문 후 추가한 코드입니다.
#include <stdio.h>

int main(void)
{
	int num1, num2, num3;
	printf("세 개의 정수 입력: ");
	scanf("%d %o %x", &num1, &num2, &num3);

	printf("입력된 정수 10진수 출력: ");
	printf("%d %d %d \n", num1, num2, num3);
	return 0;
}	