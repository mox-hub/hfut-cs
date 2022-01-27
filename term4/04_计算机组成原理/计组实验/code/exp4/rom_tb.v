`timescale 1ns/100ps
`include "rom.v"

module rom_tb;
    reg [7:0] addr;
    wire [15:0] data;

    initial begin
        addr = 16'h00;
        repeat(3) #5 addr = addr + 1;
        #5 $stop;
    end

    rom u_rom (
        .addr(addr),
        .data_bus(data)
    );
endmodule