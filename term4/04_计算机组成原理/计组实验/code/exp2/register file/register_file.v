module register_file (
    input wire we, oe, // low level enable
    input wire [2:0] write_address, read_address0, read_address1,
    input wire [15:0] write,
    output reg [15:0] read0, read1
);
    reg [15:0] register[0:7];
    reg [3:0] cnt;
    // register initialization
    initial begin
        cnt = 4'b0;
        repeat(8) begin
            register[cnt] = 16'b0;
            cnt = cnt + 1;
        end
    end

    always @ * begin
        // write enable, low level enable
        if (!we) register[write_address] = write;

        // read enable, low level enable
        if (!oe) begin
            read0 <= register[read_address0];
            read1 <= register[read_address1];
        end
    end
endmodule