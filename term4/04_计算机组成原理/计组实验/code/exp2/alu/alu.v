module alu (
    input wire [15:0] in0, in1,
    input wire [2:0] operate,
    output reg [15:0] out
);
    always @ * begin
        case(operate)
            default: out = 16'bx;
            // add
            3'b000: out = in0 + in1;
            // minus
            3'b001: out = in0 - in1;
            // and
            3'b010: out = in0 & in1;
            // or
            3'b011: out = in0 | in1;
            // logic shift left
            3'b100: out = in0 << in1;
            // logic shift right
            3'b101: out = in0 >> in1;
        endcase
    end
endmodule