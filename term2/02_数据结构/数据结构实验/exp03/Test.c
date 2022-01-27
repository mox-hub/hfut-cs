#include "head.h"
#include<stdlib.h>

int main (void)
{
	while(1)
	{
		int key;
		printf("-------请选择功能-------\n");
		printf("--1.十进制转16进制\n");
		printf("--2.判断算式括号合法性\n");
		printf("------------------------\n");
		printf("请输入序号：");
		scanf("%d",&key);
		int x;
		char *str;
		switch(key)
		{
			case 1:		
				printf("请输入十进制数：");
				scanf("%d",&x);
				numberBaseSwitch(x);
				printf("\n");
				system("pause");
				system("cls");
				break;
			case 2:
				printf("请输入算式：");	
				scanf("%s",str);
				printf("算式：");
				isValid(str);
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
