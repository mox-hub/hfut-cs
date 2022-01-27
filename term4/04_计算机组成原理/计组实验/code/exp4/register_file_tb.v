`timescale 1ns/100ps
`include "register_file.v"

module register_file_tb;
    reg wr; // high level write enable
    reg [2:0] write_address, read_address1, read_address2;
    reg [15:0] write;
    wire [15:0] read1, read2;
    reg clk;
    parameter clk_period = 10;

    // generate clock
    always begin
        clk = 0; #(clk_period / 2);
        clk = 1; #(clk_period / 2);
    end

    initial begin
        wr = 1;
        write_address = 3'b011;
        write = 16'hf000;
        #10;
        wr = 0;
        read_address1 = 3'b011;
        read_address2 = 3'b010;
        #5;
        wr = 1;
        write_address = 3'b010;
        write = 16'h0f00;
        #5 wr = 0;
        #10 $stop;
    end

    register_file u_regs(
        .wr_en(wr),
        .write_reg(write_address),
        .read_reg1(read_address1),
        .read_reg2(read_address2),
        .write_data(write),
        .read_data1(read1),
        .read_data2(read2),
        .clk(clk)
    );
endmodule