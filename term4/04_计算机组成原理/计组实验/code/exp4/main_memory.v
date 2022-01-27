`timescale 1ns/100ps

module main_memory (
    input wire m_wr_en, // write enable, high level valid
    input wire [8:0] addr,
    input wire [15:0] write_data,
    output reg [15:0] read_data
);
    // 256 * 2B
    reg [15:0] mem[0:255];

    // memory initialization
    reg [8:0] cnt = 0;
    initial begin
        repeat(256) begin
            mem[cnt] = 16'b0;
            cnt = cnt + 1;
        end
    end

    // read
    always @ addr read_data = mem[addr];

    // write
    always @ (posedge m_wr_en or addr)
        if(m_wr_en) mem[addr] = write_data;
endmodule