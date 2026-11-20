#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

int main(void)
{
    char filename[100];
    int i;

    for (i = 0; i < 6; i++) {
        sprintf(filename, "image%d.jpg", i);
        printf("%s \n", filename);
    }
    return 0;
}