//***********************************************
//File name:0208
//Author:李浩然
//Date:2019.10.20
//Student ID:2019218211
//***********************************************
#include<stdio.h>

int main(void)
{
    char a, b;
    printf("请输入大写字母：");
    scanf("%c", &a);

    b = a + 32;

    printf("它的小写字母是：%c\n", b);

    return 0;
}
