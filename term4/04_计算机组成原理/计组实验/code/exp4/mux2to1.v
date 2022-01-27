`timescale 1ns/100ps

module mux2to1 (
    input wire [15:0] in1, in2,
    input wire select,
    output reg [15:0] out
);
    always @ * begin
        if (select == 0) out = in1;
        else if (select == 1) out = in2;
    end
endmodule