//////////////////////////////////////////////////////////////////////
// Module:  pc_reg
// File:    pc_reg.v
// Author:  @Haoran Li @Shaozhi Zhang
// E-mail:  life_mox@163.com
// Description: 指令指针寄存器PC
// Revision: 1.2
//////////////////////////////////////////////////////////////////////

`include "defines.v"

module pc_reg(

	input	wire										clk,
	input wire										rst,

	//来自译码阶段的信息
	input wire                            branch_flag_i,
	input wire[`RegBus]            branch_target_address_i,
	
	output reg[`InstAddrBus]			pc,
	output reg                                 ce
	
);

	always @ (posedge clk) begin
		if (ce == `ChipDisable) begin
			pc <= 32'h00000000;
		end else  begin
		  	if(branch_flag_i == `Branch) begin
					pc <= branch_target_address_i;
				end else begin
		  		pc <= pc + 4'h4;
		  	end
		end
	end
	
	always @ (posedge clk) begin
		if (rst == `RstEnable) begin
			ce <= `ChipDisable;
		end else begin
			ce <= `ChipEnable;
		end
	end

endmodule