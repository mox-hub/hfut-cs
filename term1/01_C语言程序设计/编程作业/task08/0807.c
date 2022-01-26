//****************************************
//File Name:0807
//Author:Li Haoran
//Date:2019.12.18
//Student ID:2019218211
//****************************************
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int time_num(void);//make the time number

int main(void)
{
    int i;
    double buffer[86400], buf;//buffer for the choose one
    FILE *fp;
    srand(time(NULL));
    //rand for the bufer
    printf("数据正在生成中......\n");
    for(i = 0;i < 86400;i++)
    {
        buffer[i] = rand();
    }
    //open the file

    if((fp = fopen("s.bin","wb")) == NULL)
    {
        printf("Fail to open the file !\n");
        exit(0);
    }
    //input the data
    printf("数据正在写入文件......\n");
    fwrite(buffer,sizeof(double),86400,fp);
    fclose(fp);
    //transpose the time into number
    printf("正在读取当前时间:......\n");
    i = time_num();
    printf("时间序号为:%d\n", i);

    //reopen the file
    if((fp = fopen("s.bin","rb")) == NULL)
    {
        printf("Fail to open the file !\n");
        exit(0);
    }

    fseek(fp,i * 8,SEEK_SET);
    fread(&buf,sizeof(double),1,fp);
    printf("当前时间对应的数据为: %lf\n", buf);
    system("pause");

    return 0;
}

int time_num(void)
{
    int num;
    time_t t;
    struct tm *lt;
    time(&t);
    lt = localtime(&t);

    num = lt->tm_hour * 3600 + lt->tm_min * 60 + lt->tm_sec;

    return num;
}
