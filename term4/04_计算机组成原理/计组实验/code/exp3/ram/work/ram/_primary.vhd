library verilog;
use verilog.vl_types.all;
entity ram is
    port(
        we              : in     vl_logic;
        address         : in     vl_logic_vector(9 downto 0);
        data            : inout  vl_logic_vector(15 downto 0)
    );
end ram;
