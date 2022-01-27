module mux4to1(
    input wire[31:0] in0, in1, in2, in3,
    input wire[1:0] select,
    output reg[31:0] out
);
    always @ * begin
        case (select)
            2'b00: out = in0;
            2'b01: out = in1;
            2'b10: out = in2;
            2'b11: out = in3;
            default: out = 4'bx;
        endcase
    end
endmodule
