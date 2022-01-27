`timescale 1ns/100ps

module rom (
    input wire [7:0] addr,
    output reg [15:0] data_bus
);
    // 256 * 2B
    reg [15:0] mem[0:255];
    // operation list
    // ---------------------------------
    // 0001
    // add r1, r2
    //      A15 ~ A12: operation code
    //      A11 ~ A9:  register1
    //      A8 ~ A6 :  register2
    //      A5 ~ A0:   null
    // [r1] + [r2] -> r1
    // ---------------------------------
    // 0010
    // addi r1, imm
    //      A15 ~ A12: operation code
    //      A11 ~ A9:  register1
    //      A8 ~ A0:   immediate value
    // [r1] + imm -> r1
    // ---------------------------------
    // 0011
    // st r1, addr
    //      A15 ~ A12: operation code
    //      A11 ~ A9:  register1
    //      A8 ~ A0:   main memory address
    // [r1] -> mem[addr]
    // ---------------------------------
    // 0100
    // ld r1, addr
    //      A15 ~ A12: operation code
    //      A11 ~ A9:  register1
    //      A8 ~ A0:   main memory address
    // mem[addr] -> r1
    // ---------------------------------
    initial begin
        mem[0] = 16'b0;                   // do nothing
        mem[1] = 16'b0001_100_010_000000; // r[4] + r[2] -> r[4]
        mem[2] = 16'b0010_101_000000111;  // r[5] + #7 -> r[5]
        mem[3] = 16'b0011_100_000000010;  // r[4] -> m[2]
        mem[4] = 16'b0011_101_000000011;  // r[5] -> m[3]
        mem[5] = 16'b0100_001_000000010;  // m[2] -> r[1]
        mem[6] = 16'b0;                   // do nothing
    end

    always @ addr begin
        assign data_bus = mem[addr];
    end
        
endmodule