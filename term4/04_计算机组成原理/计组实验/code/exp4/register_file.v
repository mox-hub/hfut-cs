`timescale 1ns/100ps

module register_file (
    input wire wr_en, // high level write enable
    input wire clk,   // clock
    input wire [2:0] read_reg1, read_reg2, write_reg,
    input wire [15:0] write_data,
    output reg [15:0] read_data1, read_data2
);
    // 8 * 2B
    reg [15:0] mem[0:7];
    // register initialization
    initial begin
        mem[0] <= 16'h01;
        mem[1] <= 16'h02;
        mem[2] <= 16'h04;
        mem[3] <= 16'h08;
        mem[4] <= 16'h10;
        mem[5] <= 16'h20;
        mem[6] <= 16'h40;
        mem[7] <= 16'h80;
    end

    always @ (read_reg1 or read_reg2) begin
        read_data1 <= mem[read_reg1];
        read_data2 <= mem[read_reg2];
    end

    always @ (negedge clk) 
        // write enable, high level enable
        if (wr_en) mem[write_reg] = write_data;

endmodule