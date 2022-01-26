//////////////////////////////////////////////////////////////////////
// Module:  if_id
// File:    if_id.v
// Author:  @Haoran Li @Shaozhi Zhang
// E-mail:  life_mox@163.com
// Description: IF/ID½×¶ÎµÄ¼Ä´æÆ÷
// Revision: 1.0
//////////////////////////////////////////////////////////////////////

`include "defines.v"

module if_id(

	input	wire										clk,
	input wire										rst,

	input wire[`InstAddrBus]			if_pc,
	input wire[`InstBus]          if_inst,
	output reg[`InstAddrBus]      id_pc,
	output reg[`InstBus]          id_inst  
	
);

	always @ (posedge clk) begin
		if (rst == `RstEnable) begin
			id_pc <= `ZeroWord;
			id_inst <= `ZeroWord;
	  end else begin
		  id_pc <= if_pc;
		  id_inst <= if_inst;
		end
	end

endmodule