//****************************************
//File Name:0802
//Author:Li Haoran
//Date:2019.12.11
//Student ID:2019218211
//****************************************
#include<stdio.h>
#include<time.h>
#include<stdlib.h>

typedef char BYTEPORT;

void Delay1s(int n);
void printBinary(BYTEPORT a);

int main(void)
{
    int T1, T2, T3, T4;
    BYTEPORT a = 33;//00100001
    printf("请输入:东西向红灯时间:\n");
    scanf("%d", &T1);
    printf("请输入:东西向绿灯时间:\n");
    scanf("%d", &T2);
    printf("请输入:南北向红灯时间:\n");
    scanf("%d", &T3);
    printf("请输入:南北向绿灯时间:\n");
    scanf("%d", &T4);

    printf("交通情况为:\n");
    while(1)
    {
        printBinary(a);
        Delay1s(T1);
        a = a ^ 3;//00100010 //^00000011
        printBinary(a);
        Delay1s(5);
        a = a ^ 46;//00001100 //^00101110
        printBinary(a);
        Delay1s(T2);
        a = a ^ 24;//00010100 //^00011000
        printBinary(a);
        Delay1s(5);
        a = 33;
    }//show
    return 0;
}
//the function of delay (1s)
void Delay1s(int n)
{
    clock_t start;
    int i;

    for(i = 0; i < n; i++)
    {
        start = clock();
        while((clock() - start) < CLK_TCK)
        {
            ;
        }
    }
}
//out put 2
void printBinary(BYTEPORT a)
{
    int t = a,num[8],i;
    int temp;
    for(i = 0;i < 8;i++)
    {
        num[i] = t % 2;
        t = t / 2;
    }
    for(i = 0;i < 4;i++)
    {
        temp = num[i];
        num[i] = num[7-i];
        num[7-i] = temp;
    }
    for(i = 0;i < 8;i++)
    {
        printf("%d", num[i]);
    }
    printf("\n");
}


