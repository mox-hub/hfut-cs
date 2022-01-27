module register_file_tb;
    reg we, oe; // low level enable
    reg [2:0] write_address, read_address0, read_address1;
    reg [15:0] write;
    wire [15:0] read0, read1;

    initial begin
        we = 1'b0;
        oe = 1'b1;
        write_address = 3'b011;
        read_address0 = 3'b011;
        read_address1 = 3'b101;
        write = 16'hf000;

        #10 
        we = 1'b1; 
        oe = 1'b0;
        write_address = 3'b101;
        write = 16'h0f00;
        #10 $stop;
    end

    register_file uut(
        .we(we), .oe(oe),
        .write_address(write_address), 
        .read_address0(read_address0), .read_address1(read_address1),
        .write(write),
        .read0(read0), .read1(read1)
    );
endmodule