`timescale 1ns/100ps

module alu (
    input wire [15:0] in_data1, in_data2, // input data
    input wire alu_op,                    // select operation
    output reg [15:0] out_data            // output data
);
    always @ * begin
        case (alu_op)
            default: out_data = 16'bx;
            1'b0: out_data = in_data1;
            1'b1: out_data = in_data1 + in_data2;
        endcase
    end
endmodule