module alu_tb;
    reg [15:0] in0, in1;
    reg [2:0] operate;
    wire [15:0] out;

    initial begin
        in0 = 16'h000f;
        in1 = 16'h0002;
        operate = 3'b000;
        #10 operate = 3'b001;
        #10 operate = 3'b010;
        #10 operate = 3'b011;
        #10 operate = 3'b100;
        #10 operate = 3'b101;
        #10 operate = 3'b110;
        #10 operate = 3'b111;
        #10 $stop;
    end

    alu uut(
        .in0(in0), .in1(in1),
        .operate(operate),
        .out(out)
    );
endmodule