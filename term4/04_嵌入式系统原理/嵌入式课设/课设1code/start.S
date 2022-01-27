.globl _start
.global asm_timer_irq
.extern do_irq
_start:

reset:
	// 外设地址告诉cpu
    ldr r0, =0x70000000
    orr r0, r0, #0x13
    mcr p15,0,r0,c15,c2,4
    
	// 关看门狗
	ldr r0, =0x7E004000
	mov r1, #0
	str r1, [r0]
	
	//Enabel VIC
	mrc p15,0,r0,c1,c0,0
	orr r0,r0,#(1<<24)
	mcr p15,0,r0,c1,c0,0	

	// 设置栈
	ldr sp, =8*1024

	// 初始化时钟
	bl clock_init

	// 初始化ddr
	bl sdram_init

	// 初始化nandflash
	bl nand_init

	// 初始化中断
	bl irq_init
	
	// 开中断
	//mov r0, #0x53
	//msr CPSR_cxsf, r0
	mrs r0,cpsr
	bic r0,r0,#0x80
	msr cpsr_c,r0

	// 重定位，把程序的代码段、数据段复制到它的链接地址去
	adr r0, _start   		
	ldr r1, =_start  		
	ldr r2, =bss_start      	
	sub r2, r2, r1
	cmp r0,r1
	beq clean_bss
	bl copy2ddr					
	cmp r0, #0
	bne halt

	// 清BSS，把BSS段对应的内存清零
clean_bss:
	ldr r0, =bss_start
	ldr r1, =bss_end
	mov r3, #0
	cmp r0, r1
	beq on_ddr
clean_loop:
	str r3, [r0], #4
	cmp r0, r1
	bne clean_loop

on_ddr:
	// 跳转
	ldr pc, =main

	// 中断异常
asm_timer_irq:
	.word irq
irq:
	/* 1. 保存现场 */
	ldr sp, =0x54000000
	sub lr, lr, #4
	stmfd sp!, {r0-r12, lr}  /* lr就是swi的下一条指令地址 */
	/* 2. 处理异常 */
	bl do_irq
	/* 3. 恢复现场 */
	ldmfd sp!, {r0-r12, pc}^  /* ^表示把spsr恢复到cpsr */

halt:
	b halt
