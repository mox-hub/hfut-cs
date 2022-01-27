// 启动代码
.global _start

_start:

	// 把外设的基地址告诉CPU
    ldr r0, =0x70000000 					
    orr r0, r0, #0x13					
    mcr p15,0,r0,c15,c2,4       		
    
	// 关看门狗
	ldr r0, =0x7E004000
	mov r1, #0
	str r1, [r0] 
	
	// 设置栈
	ldr sp, =0x0C002000
	
	// 开启icaches
#ifdef  CONFIG_SYS_ICACHE_OFF
	bic	r0, r0, #0x00001000					@ clear bit 12 (I) I-cache
#else
	orr	r0, r0, #0x00001000					@ set bit 12 (I) I-cache
#endif
	mcr	p15, 0, r0, c1, c0, 0

	
	// 调用C函数点灯
	bl main
	
halt:
	b halt	
