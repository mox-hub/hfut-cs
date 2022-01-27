module mux4to1_tb;
    reg [31:0] in0, in1, in2, in3;
    reg [1:0] select;
    wire [31:0] out;

    initial begin
        in0 = 32'h000000ff;
        in1 = 32'h0000ffff;
        in2 = 32'h00ffffff;
        in3 = 32'hffffffff;
        select = 2'b00;

        #10 select = 2'b01;

        #10 select = 2'b10;

        #10 select = 2'b11;

        #10 $stop;
    end

    mux4to1 uut(
        .in0(in0), .in1(in1), .in2(in2), .in3(in3),
        .select(select),
        .out(out)
    );

endmodule