`timescale 1ns/100ps
`include "alu.v"
`include "cu.v"
`include "extsign.v"
`include "pc.v"
`include "register_file.v"
`include "rom.v"
`include "main_memory.v"
`include "mux2to1.v"

module cpu (
    input wire clk, // clock signal
    input wire rst  // reset signal, low level enable
);
    // program counter
    pc u_pc (
        .clk(clk),
        .rst(rst)
    );

    // instructions rom
    rom u_rom (
        .addr(u_pc.addr)
    );

    // control unit
    cu u_cu (
        .op_code(u_rom.data_bus[15:12])
    );

    // arithmetic logic unit
    alu u_alu (
        .in_data1(u_regs.read_data1),
        .in_data2(u_mux1.out),
        .alu_op(u_cu.alu_op)
    );

    // register file
    register_file u_regs (
        .clk(clk),
        .wr_en(u_cu.wr_en),
        .read_reg1(u_rom.data_bus[11:9]),
        .read_reg2(u_rom.data_bus[8:6]),
        .write_reg(u_rom.data_bus[11:9]),
        .write_data(u_mux2.out)
    );

    // main memory
    main_memory u_mmem (
        .m_wr_en(u_cu.m_wr_en),
        .addr(u_rom.data_bus[8:0]),
        .write_data(u_alu.out_data)
    );

    mux2to1 u_mux1 (
        .in1(u_regs.read_data2),
        .in2(u_extsign.out_data),
        .select(u_cu.sel1)
    );

    mux2to1 u_mux2 (
        .in1(u_alu.out_data),
        .in2(u_mmem.read_data),
        .select(u_cu.sel2)
    );

    // sign extend unit
    extsign u_extsign (
        .in_data(u_rom.data_bus[8:0]),
        .e_rd_en(u_cu.e_rd_en)
    );
endmodule