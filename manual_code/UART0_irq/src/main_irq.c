/*
 ****************************************************************************
 *
 *                  UNIVERSITY OF WATERLOO SE 350 RTX LAB  
 *
 *                     Copyright 2020-2021 Yiqing Huang
 *                          All rights reserved.
 *---------------------------------------------------------------------------
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *  - Redistributions of source code must retain the above copyright
 *    notice and the following disclaimer.
 *
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *---------------------------------------------------------------------------*/
 

/**************************************************************************//**
 * @file        main_irq.c
 * @brief       main routine for echoing user input through UART0 by interrupt.
 *              uart1 is by polling. It is used as a debugging terminal. 
 *              
 * @version     V1.2021.01
 * @authors     Yiqing Huang
 * @date        2021 JAN
 *****************************************************************************/

#include <LPC17xx.h>
#include "uart_irq.h"
#include "uart_polling.h"
#ifdef DEBUG_0
#include "printf.h"
#endif /* DEBUG_0 */

extern uint8_t g_send_char;

int main()
{
	LPC_UART_TypeDef *pUart;
	
	SystemInit();	
	__disable_irq();
	
	uart_irq_init(0); /* uart0 interrupt driven, for RTX console */
	uart_init(1);     /* uart1 polling, for debugging */
	
#ifdef DEBUG_0
	init_printf(NULL, putc);
#endif // DEBUG_0
	__enable_irq();

	uart1_put_string("COM1> Type a character at COM0 terminal\n\r");

	pUart = (LPC_UART_TypeDef *) LPC_UART0;
	
	while( 1 ) {
		
		if (g_send_char == 0) {
			/* Enable RBR, THRE is disabled */
			pUart->IER = IER_RLS | IER_RBR;
		} else if (g_send_char == 1) {
			/* Enable THRE, RBR left as enabled */
			pUart->IER = IER_THRE | IER_RLS | IER_RBR;
		}
     
	}
}

/*
 *===========================================================================
 *                             END OF FILE
 *===========================================================================
 */
