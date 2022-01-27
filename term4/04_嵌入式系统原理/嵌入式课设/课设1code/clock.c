// 功能:c语言初始化时钟

#define APLL_LOCK (*((volatile unsigned long *)0x7E00F000))
#define MPLL_LOCK (*((volatile unsigned long *)0x7E00F004))
#define EPLL_LOCK (*((volatile unsigned long *)0x7E00F008))

#define OTHERS    (*((volatile unsigned long *)0x7e00f900))

#define CLK_DIV0  (*((volatile unsigned long *)0x7E00F020))

#define ARM_RATIO    0   						// ARMCLK 	= DOUTAPLL / (ARM_RATIO + 1)  	= 532/(0+1) = 532  MHz
#define MPLL_RATIO   0   						// DOUTMPLL = MOUTMPLL / (MPLL_RATIO + 1)   = 532/(0+1) = 532  MHz
#define HCLKX2_RATIO 1   						// HCLKX2 	= HCLKX2IN / (HCLKX2_RATIO + 1) = 532/(1+1) = 266  MHz
#define HCLK_RATIO   1   						// HCLK 	= HCLKX2   / (HCLK_RATIO + 1)   = 266/(1+1) = 133  MHz
#define PCLK_RATIO   3   						// PCLK   	= HCLKX2   / (PCLK_RATIO + 1)   = 266/(3+1) = 66.5 MHz


#define APLL_CON  (*((volatile unsigned long *)0x7E00F00C))
#define APLL_CON_VAL  ((1<<31) | (250 << 16) | (3 << 8) | (1))

#define MPLL_CON  (*((volatile unsigned long *)0x7E00F010))
#define MPLL_CON_VAL  ((1<<31) | (250 << 16) | (3 << 8) | (1))

#define CLK_SRC  (*((volatile unsigned long *)0x7E00F01C))

void clock_init(void)
{	
	/* 1. 设置各PLL的LOCK_TIME,使用默认值 */
	APLL_LOCK = 0xffff;							// APLL_LOCK，供cpu使用 
	MPLL_LOCK = 0xffff;							// MPLL_LOCK，供AHB(存储/中断/lcd等控制器)/APB(看门狗，定时器，SD等)总线上的设备使用
	EPLL_LOCK = 0xffff;							// EPLL_LOCK，供UART,IIS,IIC使用 

	/* 2. 设置为异步模式(Asynchronous mode) */
	OTHERS &= ~0xc0;							//《linux installation for u-boot》3.7中：用MPLL作为HCLK和PCLK的Source是异步(ASYNC)模式,用APLL是同步(SYNC)模式
	while ((OTHERS & 0xf00) != 0);

	/* 3. 设置分频系数 */
	CLK_DIV0 = (ARM_RATIO) | (MPLL_RATIO << 4) | (HCLK_RATIO << 8) | (HCLKX2_RATIO << 9) | (PCLK_RATIO << 12);

	/* 4. 设置PLL,放大时钟 */	
	APLL_CON = APLL_CON_VAL;  
	MPLL_CON = MPLL_CON_VAL;  

	/* 5. 选择PLL的输出作为时钟源 */
	CLK_SRC = 0x03;
}

