`timescale 1ns/100ps
`include "cu.v"

module cu_tb;
    reg [6:0] cmd;   // operation code
    wire wr;         // register file write enable
    wire op;         // alu operation selection

    initial begin
        cmd = 16'b000_0001_001_000_000;    // r[1] + r[0] -> r[0]
        #5 cmd = 16'b000_0001_100_010_100; // r[4] + r[2] -> r[4]
        #5 cmd = 16'b000_0001_010_001_001; // r[2] + r[1] -> r[1]
        #5 cmd = 16'b000_0000_000_000_000; // do nothing
        #5 $stop;
    end

    cu u_cu (
        .op_code(cmd),
        .wr_en(en),
        .alu_op(op)
    );
endmodule