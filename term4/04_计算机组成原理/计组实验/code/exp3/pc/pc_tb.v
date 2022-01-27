`timescale 1ns/100ps
`include "pc.v"

module pc_tb;
    parameter clk_period = 5;
    reg clk;
    reg rst;
    wire addr;

    // generate clock
    always begin
        clk = 0; #(clk_period/2);
        clk = 1; #(clk_period/2);
    end

    initial begin
        rst = 1;
        #15 rst = 0;
        #3 rst = 1;
        #10 $stop;
    end

    pc u_pc (
        .clock(clk),
        .reset(rst),
        .address(addr)
    );
endmodule