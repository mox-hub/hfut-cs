`timescale 1ns/100ps
`include "alu.v"

module alu_tb;
    reg [15:0] in1, in2; // input data
    reg op;              // select operation
    wire [15:0] out;     // output data

    initial begin
        in1 = 16'd836;
        in2 = 16'd112;
        op = 'b1;
        #5;
        in1 = 16'd1427;
        in2 = 16'd201;
        #5;
        in1 = 16'd17;
        in2 = 16'd19;
        #5;
        in1 = 16'd114;
        in2 = 16'd514;
        op = 'b0;
        #5 $stop;
    end

    alu u_alu (
        .in_data1(in1),
        .in_data2(in2),
        .alu_op(op),
        .out_data(out)
    );
endmodule