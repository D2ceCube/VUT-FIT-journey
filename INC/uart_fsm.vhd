-- uart_fsm.vhd: UART controller - finite state machine
-- Author(s): Dias Assatulla (xassat00)
--
library ieee;
use ieee.std_logic_1164.all;

-------------------------------------------------
entity UART_FSM is
port(
   CLK : in std_logic;
   RST : in std_logic;
   DATA : in std_logic;
   DATA_READ : in std_logic;
   CNT_CLK : in std_logic_vector(4 downto 0);
	
	
   READ_EN : out std_logic;
   CNT_EN : out std_logic;
   VALID : out std_logic
   );
end entity UART_FSM;

-------------------------------------------------
architecture behavioral of UART_FSM is
  type state is (BEGIN_S, WAIT_FOR_MID_BIT, GET_DATA, WAIT_FOR_END_CYCLE, VALID_DATA);
    signal currentState : state := BEGIN_S;
begin
  READ_EN <= '1' when currentState = GET_DATA 
							else '0';
  VALID <= '1' when currentState = VALID_DATA 
							else '0';
  CNT_EN <= '0' when currentState = VALID_DATA 
							or currentState = BEGIN_S 
							else '1';
  process(CLK) begin
    if rising_edge(CLK) then
      if RST = '1' then
        currentState <= BEGIN_S;
      else
        case currentState is
        when BEGIN_S =>
          if DATA = '0' then
            currentState <= WAIT_FOR_MID_BIT;
          end if;
        when WAIT_FOR_MID_BIT =>
          if CNT_CLK = "10110" then
            currentState <= GET_DATA;
          end if;
        when GET_DATA =>
          if DATA_READ = '1' then
            currentState <= WAIT_FOR_END_CYCLE;
          end if;
        when WAIT_FOR_END_CYCLE =>
          if CNT_CLK = "10000" then
            currentState <= VALID_DATA;
          end if;
        when VALID_DATA => currentState <= BEGIN_S;
        end case;
      end if;
    end if;
  end process;
end behavioral;
