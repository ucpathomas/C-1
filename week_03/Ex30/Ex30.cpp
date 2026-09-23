#include <stdio.h>

int main(void)
{
	int total = 0, num = 0;
	do
	{
		printf("정수입력(0 to quit): ");
		scanf_s("%d", &num);  // 오류로 인해 AI에게 물어보고 scanf 대신 scanf_s를 사용했습니다.
		total += num;
	} while (num != 0);
	printf("합계: %d \n", total);
	return 0;
}