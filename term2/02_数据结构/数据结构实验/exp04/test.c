#include"head.h"

int main(void)
{
	while(1)
	{
		int key;
		printf("-------请选择功能-------\n");
		printf("--1.初始化队列\n");
		printf("--2.判断是否队空\n");
		printf("--3.判断是否队满\n");
		printf("--4.入队\n");
		printf("--5.出队\n");
		printf("--6.取队头元素\n");
		printf("--7.求当前队列中元素的个数\n");
		printf("--8.自定义的函数\n");
		printf("--\n");
		printf("------------------------\n");
		printf("请输入序号：");
		scanf("%d",&key);
		seqQueue Q; 
		int x;
		switch(key)
		{
			case 1:
				initQueue(Q);
				printf("初始化成功！"); 
				printf("\n");
				system("pause");
				system("cls");
				break;
			case 2:
				queueEmpty(Q);
				printf("\n");
				system("pause");
				system("cls");
				break;
			case 3:
				queueFull(Q);
				printf("\n");
				system("pause");
				system("cls");
				break;
			case 4:
				printf("请输入入队元素：");
				scanf("%d",&x);
				enQueue(Q,x);
				printf("入队成功！");
				printf("\n");
				system("pause");
				system("cls");
				break;
			case 5:
				outQueue(Q,x);
				printf("%d",x);
				printf("\n");
				system("pause");
				system("cls");
				break;
			case 6:
				queueFront(Q,x);
				printf("队头元素为：%d",&x);
				printf("\n");
				system("pause");
				system("cls");	
				break;
			case 7:
				x = numberOfQueue(Q);
				printf("元素的个数为：%d",&x);
				printf("\n");
				system("pause");
				system("cls");
				break;
			case 8:
				myQueue();
				printf("\n");
				system("pause");
				system("cls");
				break;							
			default:
				printf("输入错误！"); 
				system("pause");
				system("cls");
		} 		
	}

	return 0;
}
