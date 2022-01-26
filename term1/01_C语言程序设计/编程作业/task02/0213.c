//***********************************************
//File name:0213
//Author:李浩然
//Date:2019.10.20
//Student ID:2019218211
//***********************************************
#include<stdio.h>

int main(void)
{
    int a;
    printf("请输入a的值为:");
    scanf("%d", &a);

    if(a % 2 == 0)
        printf("%d为偶数", a);
    else
        printf("%d为奇数", a);

    return 0;

}
