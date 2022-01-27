#include "stdio.h"
#define GPNCON (*(volatile unsigned long *)0x7F008830)
#define GPNDAT (*(volatile unsigned long *)0x7F008834)
#define GPKCON     		(*((volatile unsigned long *)0x7F008800))
#define GPKDAT     			(*((volatile unsigned long *)0x7F008808))
#define GPFCON          (*((volatile unsigned long *)0x7F0080A0))
#define GPFDAT          (*((volatile unsigned long *)0x7F0080A4))
#define		TCON      	( *((volatile unsigned long *)(PWMTIMER_BASE+0x08)) )
#define		PWMTIMER_BASE			(0x7F006000)

void timer_init(unsigned long utimer,unsigned long uprescaler,unsigned long udivider,unsigned long utcntb,unsigned long utcmpb);

int i = 0;    //控制三短一长
int j = 0;
int ishalf = 0;
int flag1 = 0;

int main()
{
		
	//配置GPK为输出，同时熄灭所有灯
	GPKCON=0x11110000;
	GPKDAT=0x000000f0;
	// 配置GPN为输入功能
	GPNCON = 0;
	while(1){
		int dat=0;
		int count=0;
		int flag=0;
		int times=2500000;
		
		while (1){	
				dat=GPNDAT;		
				if(!(dat & (1<<0))){//K1连续按下，开始计数
					count++;
					times=2500000;
					while(!(dat & (1<<0))){
						dat=GPNDAT;
						}		
				}	
				if(times<0)break;
				
				if(!(dat & (1<<1))){//K2按下
					flag1 = 2;	
					timer_init(0,65,4,31250,0);
					while(!(dat & (1<<1))){
						dat=GPNDAT;
						}		
				}
				if(!(dat & (1<<2))){//K3按下
					flag1 = 3;	
					timer_init(0,65,4,15625,0);
					while(!(dat & (1<<2))){
						dat=GPNDAT;
						}		
				}
				if((!(dat & (1<<3)))){//key4被按下，复位
					dat=GPNDAT;
					flag=1;
					flag1=0;
					i = 0;
					j = 0;
				GPKDAT=0x000000f0;
					break;

				}
				times--;
				
		}
	
		if(flag)continue;

		if(count%2&&count>0){//按下奇数次
			flag1=1;
			ishalf = 0;
			timer_init(0,65,4,15625,0);//计时器工作
		}

		if(!(count%2)&&count>0){ //按下偶数次
			flag1=1;
			ishalf = 1; 
			timer_init(0,65,4,31250,0);
		}
		

		int num = 0;
		while(dat&(1<<3))
		{//key4被按下，循环退出
		
			dat=GPNDAT;				
			if(GPKDAT & (1<<4))
			{
				num++;		
				while (GPKDAT & (1 << 4)) {}
			}
			if(num==(count))
			{
				if(count%2&&count>0) GPKDAT=0x000000c0;;
				if(!(count%2)&&count>0) GPKDAT=0x00000030;;
				break;
			}
			//灯亮了指定次数后，退出
		}	
		
		TCON &=  ~(1<<0);
	}
	return 0;
}

