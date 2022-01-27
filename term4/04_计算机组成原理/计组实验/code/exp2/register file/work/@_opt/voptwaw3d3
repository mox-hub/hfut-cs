library verilog;
use verilog.vl_types.all;
entity register_file is
    port(
        we              : in     vl_logic;
        oe              : in     vl_logic;
        write_address   : in     vl_logic_vector(2 downto 0);
        read_address0   : in     vl_logic_vector(2 downto 0);
        read_address1   : in     vl_logic_vector(2 downto 0);
        write           : in     vl_logic_vector(15 downto 0);
        read0           : out    vl_logic_vector(15 downto 0);
        read1           : out    vl_logic_vector(15 downto 0)
    );
end register_file;
