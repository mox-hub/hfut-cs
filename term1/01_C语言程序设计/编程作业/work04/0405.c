#include<stdio.h>

int main(void)
{
    char str[80];
    int i, iNum = 0;

    printf("请输入字符串:\n");
    gets(str);
    for(i = 0;str[i] != '\0';i++)
    {
        if((int)str[i] % 10 == 3)
        {
            iNum++;
        }

    }
    printf("ASCII码值个位是三的字符有%d个", iNum);

    return 0;
}
