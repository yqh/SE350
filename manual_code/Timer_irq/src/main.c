/**
 * @brief Timer interrupt example code. 
          Prints 0-9 every second. Loops around when 9 is reached.
 * @author Yiqing Huang
 * @date 2012/02/12
 */

#include <LPC17xx.h>
#include <system_LPC17xx.h>
#include "timer.h"
#include "uart_polling.h"  

extern volatile uint32_t g_timer_count;

int main () {

	volatile uint8_t sec = 0;

	SystemInit();
	__disable_irq();
	timer_init(0); /* initialize timer 0 */
	
	/* 
	  uart polling is used in this example only to help
	  demonstrate timer interrupt programming.
	  In your final project, polling is NOT ALLOWED in i-processes.
	  Refer to the UART_irq example for interrupt driven UART.
	*/
	uart0_init();
	
	__enable_irq();

	while (1) {
		/* g_timer_count gets updated every 1ms */
		if (g_timer_count == 1000) { 
			uart0_put_char('0'+ sec);
			sec = (++sec)%10;
			g_timer_count = 0; /* reset the counter */
		}     
	}
}
