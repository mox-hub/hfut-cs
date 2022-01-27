#include "stdio.h"

void timer_init(unsigned long utimer,unsigned long uprescaler,unsigned long udivider,unsigned long utcntb,unsigned long utcmpb);

int main()
{
	timer_init(0,65,4,62500,0);

	while (1)
	{

	}
	
	return 0;
}
