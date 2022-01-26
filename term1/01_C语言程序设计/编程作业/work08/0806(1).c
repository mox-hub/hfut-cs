//****************************************
//File Name:0806
//Author:Li Haoran
//Date:2019.12.18
//Student ID:2019218211
//****************************************
#include<stdio.h>
#include<stdlib.h>

int main(void)
{
    int key;
    char ch;
    FILE *fp, *fp1;

    printf("请输入秘钥:");
    scanf("%d", &key);

    fp = fopen("hamlet.txt","r");
    fp1 = fopen("Cyb.dat","wb");

    if(fp1 == NULL ||fp == NULL)
    {
        printf("error!\n");
        exit(0);
    }

    while(ch = fgetc(fp) != EOF)
    {
        if(ch >= 'a' &&ch <= 'z')
        {
           ch -= key;
           if(ch < 'a')
                ch += 26;
        }
        if(ch >= 'A' && ch <= 'Z')
        {
            ch -= key;
            if(ch < 'A')
                ch += 26;
        }
        fputc(ch,fp1);
    }
    printf("加密文本已写入Cyb.dat文件中.\n");

    fclose(fp);
    fclose(fp1);
}


