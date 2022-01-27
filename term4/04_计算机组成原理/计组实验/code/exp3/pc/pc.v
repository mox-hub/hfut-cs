`timescale 1ns/100ps

module pc (
    output reg [7:0] address,
    input wire clock,
    input wire reset // low level enable
);
    // 8 bits program counter
    reg [7:0] register = 8'b0;

    always @ (posedge clock or negedge reset) begin
        if (!reset) register <= 8'b0;
        else begin
            register = register + 1;
            assign address = register;
        end
    end
endmodule