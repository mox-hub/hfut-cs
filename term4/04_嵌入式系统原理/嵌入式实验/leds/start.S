.global _start

_start:
	// 把外设的基地址告诉CPU
    ldr r0, =0x70000000 					//对于6410来说,内存(0x00000000～0x60000000),外设(0x70000000-0x7fffffff)
    orr r0, r0, #0x13						//外设大小:256M
    mcr p15,0,r0,c15,c2,4       			//把r0的值(包括了外设基地址+外设大小)告诉cpu
    
	// 关看门狗
	ldr r0, =0x7E004000
	mov r1, #0
	str r1, [r0] 

	// 设置GPKCON0
	ldr r1, =0x7F008800 					
	ldr r0, =0x11110000																  
	str r0, [r1]

	mov r2, #0x1000
led_blink:
	// 设置GPKDAT，使GPK_4/5/6/7引脚输出低电平，LED亮
	ldr r1, =0x7F008808 					
	mov r0, #0
	str r0, [r1]

	// 延时
	bl delay							

	// 设置GPKDAT，使GPK_4/5/6/7引脚输出高电平，LED灭
	ldr r1, =0x7F008808 					
	mov r0, #0xf0
	str r0, [r1]

	// 延时
	bl delay	

	sub r2, r2, #1
	cmp r2,#0
	bne led_blink

halt:
	b halt


delay:
	mov r0, #0x1000000
delay_loop:
	cmp r0, #0
	sub r0, r0, #1
	bne delay_loop
	mov pc, lr
