`timescale 1ns/100ps

// 9 bits immidiate number extend to 16 bits number
module extsign (
    input wire [8:0] in_data,
    input wire e_rd_en,
    output reg [15:0] out_data
);
    always @ (posedge e_rd_en or in_data) begin
        if (e_rd_en) assign out_data = (in_data[8] << 15) | in_data[7:0];
    end
endmodule