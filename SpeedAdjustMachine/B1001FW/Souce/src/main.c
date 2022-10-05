#include <reg52.h>
#include "pwm.h"
#include "lcd1602.h"
#include "uart.h"
void main(void)
{
	pwm_init();
	lcd_init();
  lcd_ui_screen();
  uart_init(9600);	
	while(1)
  {
		key_scan();
    key_handle();
		uart_handle();
	}
	
}