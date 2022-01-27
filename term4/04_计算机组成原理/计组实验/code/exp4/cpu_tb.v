`timescale 1ns/100ps
`include "cpu.v"

module cpu_tb;
    reg clk; // clock signal
    reg rst; // reset signal, low level enable
    parameter clk_period = 10;

    // generate clock signal
    always begin
        clk = 0; #(clk_period / 2);
        clk = 1; #(clk_period / 2);
    end

    initial begin
        rst = 1;     // 7 period
        #70 $stop;
    end

    cpu u_cpu (
        .clk(clk),
        .rst(rst)
    );
endmodule