//****************************************
//File Name:0606
//Author:Li Haoran
//Date:20199.11.14
//Student ID:2019218211
//****************************************
#include<stdio.h>

int main(void)
{
    char a[4], *p;
    p = a;

    printf("请输入四位十六进制数:\n");
    for(p = a;p < a + 4;p++)
    {
        scanf("%c", p);
    }

    for(p = a;p < a + 4;p++)
    {
        switch(*p)
        {
        case '0':printf("0000");break;
        case '1':printf("0001");break;
        case '2':printf("0010");break;
        case '3':printf("0011");break;
        case '4':printf("0100");break;
        case '5':printf("0101");break;
        case '6':printf("0110");break;
        case '7':printf("0111");break;
        case '8':printf("1000");break;
        case '9':printf("1001");break;
        case 'a':printf("1010");break;
        case 'b':printf("1011");break;
        case 'c':printf("1100");break;
        case 'd':printf("1101");break;
        case 'e':printf("1110");break;
        case 'f':printf("1111");break;
        }
    }
}
