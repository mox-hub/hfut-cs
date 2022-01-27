`timescale 1ns/100ps

module ram (
    input wire we, // low level write enable, high level read enable
    input wire [9:0] address,
    inout wire [15:0] data
);
    // 1K * 16bits
    reg [15:0] memory[0:1023];
    reg [9:0] cnt;
    // memory initialization
    initial begin
        cnt = 10'b0;
        repeat(1024) begin
            memory[cnt] = 16'b0;
            cnt = cnt + 1;
        end
    end

    // we low level:  set databus in high-impedance state,
    //                now databus can be used as input port
    // we high level: data output
    // read enable
    assign data = (we) ? memory[address] : 16'bz;
    // write enable
    always @ (we or data) if (!we) memory[address] = data;
endmodule