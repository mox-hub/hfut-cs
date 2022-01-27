`timescale 1ns/100ps

module cu (
    input wire [3:0] op_code,  // operation code
    output reg wr_en,          // register file write enable
    output reg m_wr_en,        // main memory write enable
    output reg e_rd_en,        // imm read enable
    output reg alu_op,         // alu operation selection
    output reg sel1, sel2      // mux select
);
    always @ op_code begin
        case (op_code)
            default: begin
                alu_op = 1'b0;
                wr_en = 1'b0;
                m_wr_en = 1'b0;
                e_rd_en = 1'b0;
                sel1 = 1'b0;
                sel2 = 1'b0;
            end

            // add r1, r2
            4'b0001: begin
                alu_op = 1'b1;
                wr_en = 1'b1;
                m_wr_en = 1'b0;
                e_rd_en = 1'b0;
                sel1 = 1'b0;
                sel2 = 1'b0;
            end

            // addi r1, imm
            4'b0010: begin
                alu_op = 1'b1;
                wr_en = 1'b1;
                m_wr_en = 1'b0;
                e_rd_en = 1'b1;
                sel1 = 1'b1;
                sel2 = 1'b0;
            end

            // st r1, addr
            4'b0011: begin
                alu_op = 1'b0;
                wr_en = 1'b0;
                m_wr_en = 1'b1;
                e_rd_en = 1'b0;
                sel1 = 1'b0;
                sel2 = 1'b0;
            end

            // ld r1, addr
            4'b0100: begin
                alu_op = 1'b0;
                wr_en = 1'b1;
                m_wr_en = 1'b0;
                e_rd_en = 1'b0;
                sel1 = 1'b0;
                sel2 = 1'b1;
            end
        endcase
    end
endmodule