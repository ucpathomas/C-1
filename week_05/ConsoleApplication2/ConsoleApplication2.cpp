#include <stdio.h>

int main(void)
{

	struct student {
		int number;
		char name[10];
		double grade;
	};
	
	struct student s1 = { 24, "kim", 4.3 };

	int sizeofstudent = sizeof(s1);

	s1.grade = 4.0;
	s1.number = 124;
	s1.name[0] = 'K';


	return 0;
}