`timescale 1ns/100ps
`include "ram.v"

module ram_tb (inout wire [15:0] data_bus);
    reg wr; // low level write enable, high level read enable
    reg [9:0] addr;
    reg [15:0] data_reg;

    // low level reg output data to ram
    // high level ram input data to reg
    assign data_bus = (!wr) ? data_reg : 16'bz;
    always @ (wr or data_bus) if (wr) data_reg = data_bus;

    initial begin
        wr = 'b0; // write
        addr = 10'b00_0000_0100;
        data_reg = 16'hffff;
        #5;
        addr = addr - 1;
        data_reg = 16'h0fff;
        #5;
        addr = addr - 1;
        data_reg = 16'h00ff;
        #5;
        wr = 'b1; // read
        addr = 10'b00_0000_0100;
        #5 addr = addr - 1;
        #5 addr = addr - 1;
        #5 $stop;
    end

    ram u_ram (
        .we(wr),
        .address(addr),
        .data(data_bus)
    );
endmodule