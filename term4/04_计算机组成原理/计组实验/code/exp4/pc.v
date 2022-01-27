`timescale 1ns/100ps

module pc (
    output reg [7:0] addr,
    input wire clk,
    input wire rst // low level enable
);
    // 8 bits program counter
    reg [7:0] register = 8'b0;

    always @ (posedge clk or negedge rst) begin
        if (!rst) register <= 8'b0;
        else begin
            register = register + 1;
            assign addr = register;
        end
    end
endmodule