-- uart.vhd: UART controller - receiving part
-- Author(s): Dias Assatulla (xassat00)
--
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;

-------------------------------------------------
entity UART_RX is
port(	
    CLK: 	    in std_logic;
	RST: 	    in std_logic;
	DIN: 	    in std_logic;
	DOUT: 	    out std_logic_vector(7 downto 0);
	DOUT_VLD: 	out std_logic
);
end UART_RX;  

-------------------------------------------------
architecture behavioral of UART_RX is
signal READ_EN_SIGNAL : std_logic;
signal CLK_EN_SIGNAL : std_logic;
signal VALID_SIGNAL : std_logic;
signal CNT_CLK_SIGNAL : std_logic_vector(4 downto 0);
signal CNT_BIT : std_logic_vector(3 downto 0);
begin
	FSM: entity work.UART_FSM(behavioral)
	port map(
		CLK => CLK,
		RST => RST,
		DATA => DIN,
		DATA_READ => CNT_BIT(3),
		CNT_CLK => CNT_CLK_SIGNAL,
		READ_EN => READ_EN_SIGNAL,
		VALID => VALID_SIGNAL,
		CNT_EN => CLK_EN_SIGNAL
	);
	
	DOUT_VLD <= VALID_SIGNAL;
	process(CLK) begin
		if rising_edge(CLK) then
			if RST = '1' then
				CNT_CLK_SIGNAL <= "00000";
				CNT_BIT <= "0000";
			else
				if CLK_EN_SIGNAL = '1' then
					CNT_CLK_SIGNAL <= CNT_CLK_SIGNAL + 1;
				else
					CNT_CLK_SIGNAL <= "00000";
				end if;
				if READ_EN_SIGNAL = '1' and CNT_CLK_SIGNAL(4) = '1' then
					DOUT(conv_integer(CNT_BIT)) <= DIN;
					CNT_BIT <= CNT_BIT + 1;
					CNT_CLK_SIGNAL <= "00001";
				end if;
				if READ_EN_SIGNAL = '0' then
					CNT_BIT <= "0000";
				end if;
			end if;
		end if;
	end process;
end behavioral;
