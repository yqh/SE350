/********************************************************************//**
 * @file        main_uart_irq.c
 * @brief       Echoing user input through UART0 by interrupt.
 *              uart1, a debugging terminal, is by polling. 
 * @note        The RLS and RBR interrupts are always on. 
 *              The THRE interrupt is only on when we need to 
 *              transmit data stream.
 ***********************************************************************/

#include <LPC17xx.h>
#include "uart_irq.h"
#include "uart_polling.h"
#include "printf.h"


extern uint8_t g_send_char;
extern uint8_t g_char_in;

int main()
{
	LPC_UART_TypeDef *pUart;
	
	SystemInit();	
	__disable_irq();
	
	uart_irq_init(0); // uart0 interrupt driven, for RTX console 
	uart_init(1);     // uart1 polling, for debugging
	init_printf(NULL, putc);    // printf uses the polling terminal

	__enable_irq();

	uart1_put_string("COM1> Type a character at COM0 terminal\n\r");

	pUart = (LPC_UART_TypeDef *) LPC_UART0;	
	while( 1 ) {	
		if (g_send_char == 1) {	    // This flag is set by the IRQ handler upon an RX interrupt
            //pUart->THR &= ~IER_THRE;// turn off TX interrupt if it is on. But in this example, TX interrupt is off by the time we reach here
            pUart->THR = g_char_in; // the THR must be empty at this moment
            pUart->IER |= IER_THRE; // turn on the TX interrupt
            g_send_char = 0;        // clear the flag
		}
    }
}
