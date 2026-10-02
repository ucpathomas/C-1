#include<stdio.h>
int returnMaxValue(int num1, int num2, int num3); //함수 정의
int returnMinValue(int num1, int num2, int num3);

int main()
{
	//3개의 정수 입력
	int value1 = 0;
	int value2 = 0;
	int value3 = 0;
	int result_MaxValue = 0;
	int result_MinValue = 0;

	printf("정수 3개를 입력하세요: ");
	scanf_s("%d %d %d", &value1, &value2, &value3);
	result_MaxValue = returnMaxValue(value1, value2, value3);

	printf("%d %d %d 중에 최대값은 %d입니다.\n", value1, value2, value3, result_MaxValue);

	result_MinValue = returnMinValue(value1, value2, value3);

	printf("%d %d %d 중에 최소값은 %d입니다.\n", value1, value2, value3, result_MinValue);
	return 0;
}

int returnMaxValue(int num1, int num2, int num3) //함수 구현
{
	int Max = num1;
		if (num2 > Max)
		{
			Max = num2;
		}
		if (num3 > Max)
		{
			Max = num3;
		}
	return Max;
}

int returnMinValue(int num1, int num2, int num3) //함수 구현
{
	int Min = num1;
		if (num2 < Min)
		{
			Min = num2;
		}
		if (num3 < Min)
		{
			Min = num3;
		}
	return Min;
}
