//****************************************
//File Name:0801
//Author:Li Haoran
//Date:2019.11.30
//Student ID:2019218211
//****************************************
#include<stdio.h>
#include<stdlib.h>

struct stu_node
{
    char Name[10];
    int StudentID;
    float Score[4];
    float S;
    float ave;
};

void List(struct stu_node *a,int n);
void sort(struct stu_node *a,int n);

int main(void)
{
    int n, i, j;
    struct stu_node *a;

    printf("请输入学生数量n:\n");
    scanf("%d", &n);
    system("CLS");

    a = (struct stu_node*)malloc(n * sizeof(struct stu_node));
    for(i = 0;i < n;i++)
    {
        printf("请输入第%d名学生的姓名：", i+1);
        scanf("%s", a[i].Name);
        printf("请输入学号：");
        scanf("%d", &a[i].StudentID);
        printf("请输入四门课的成绩:\n");
        a[i].S = 0;
        for(j = 0;j < 4;j++)
        {
            scanf("%f", &a[i].Score[j]);
            a[i].S += a[i].Score[j];
        }
        a[i].ave = a[i].S / 4.0f;
        system("CLS");
    }
    printf("原序列为:\n");
    List(a,n);
    sort(a,n);
    printf("按总分排序后的序列为:\n");
    List(a,n);

    return 0;
}

void List(struct stu_node *a,int n)
{
    int i, j;
    printf("姓名\t学号\t成绩1\t成绩2\t成绩3\t成绩4\t总分\t平均分\n");
    for(i = 0;i < n;i++)
    {
        printf("%s\t",a[i].Name);
        printf("%d\t",a[i].StudentID);
        for(j = 0;j < 4;j++)
            printf("%.1f\t",a[i].Score[j]);
        printf("%.1f\t",a[i].S);
        printf("%.1f\n",a[i].ave);
    }
}

void sort(struct stu_node *a,int n)
{
    int i, j;
    struct stu_node t;
    for(i = 0;i < n-1;i++)
    {
        for(j = i;j < n;j++)
        {
            if(a[i].S < a[j].S)
            {
                t = a[i];
                a[i] = a[j];
                a[j] = t;
            }
        }
    }
}

