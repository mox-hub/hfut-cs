/*-----------------------------------------------
  Filename: LifeGame.C
  Purpose:
  Date:   2019.12.02
  Author: Li Haoran
 ----------------------------------------------*/
#include<stdio.h>
#include<windows.h>
#include<time.h>
#include<conio.h>

#define High 25                     //游戏画面大小
#define Width 25
//定义全局变量
int cells[High][Width];         //细胞的生1死0

void HideCursor(void);        //隐藏光标
void goto_xy(int x,int y);     //光标移动到(x,y)位置
void start(void);             //数据初始化
void show(void);              //显示游戏的界面
void update(void);            //更新控制函数


int main(void)
{
    int i;
    //开始界面
    the:
        printf("   生命游戏\t\n");
        printf("  1.开始游戏\n");
        printf("  0.退出游戏\n");
        printf("请输入(1/0):\t");
        scanf("%d", &i);
        if(i != 1 && i != 0)
        {
            printf("输入错误，请在输入一遍！\n");
            system("CLS");
            goto the;
        }

    //系统判断
    switch(i)
    {
    case 1:
        start();
        while(1)
        {
            show();
            update();

        }
    case 0: exit(0);
    }
    return 0;
}

void HideCursor(void)        //隐藏光标
{
 CONSOLE_CURSOR_INFO cursor_info = {1, 0};
 SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}
void goto_xy(int x,int y) //光标移动到(x,y)位置
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(handle,pos);
}
void start(void)  //数据初始化
{

    srand(time(NULL));
    int i,j;
    for(i=0;i<High;i++)
    {
        for(j=0;j<Width;j++)
        {
            cells[i][j]=(rand() % 2);//用rand()函数使细胞的初始状态随机
        }
    }
    HideCursor();
}
void show(void)     //显示游戏的界面
{
    goto_xy(0,0);
    int i,j;
    for(i=1;i<High;i++)
    {
        for(j=1;j<Width;j++)
        {
            if(cells[i][j]==1)
                printf("■");
            else
                printf("□");
        }
        printf("\n");
    }
    Sleep(250);            //控制刷新速度
}
void update(void)       //更新控制函数
{
    int i,j;
    int NeighborCells;
    int temp[High][Width];
    //判断细胞生死
    for(i=1;i<High;i++)
    {
        for(j=1;j<Width;j++)
        {   //相邻细胞个数是NeighborCells
            NeighborCells=(cells[i-1][j-1]+cells[i-1][j]+cells[i-1][j+1]+cells[i][j-1]+cells[i][j+1]+cells[i+1][j-1]+cells[i+1][j]+cells[i+1][j+1]);
            //判断状态
            if(NeighborCells==3)            //周围有3个活细胞时，该细胞生
                temp[i][j]=1;
            else if(NeighborCells==2)       //周围有2个活细胞时，该细胞状态不变
                temp[i][j]=cells[i][j];
            else
                temp[i][j]=0;
        }
    }
    //转换为静态的情况
    for(i=1;i<High;i++)
        for(j=1;j<Width;j++)
            cells[i][j]=temp[i][j];
}

