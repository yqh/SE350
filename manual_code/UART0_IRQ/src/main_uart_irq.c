/*
 ****************************************************************************
 *
 *                  UNIVERSITY OF WATERLOO SE 350 RTX LAB  
 *
 *                     Copyright 2020-2022 Yiqing Huang
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
 * @file        main_uart_irq.c
 * @brief       main routine for echoing user input through UART0 by interrupt.
 *              uart1 is by polling. It is used as a debugging terminal. 
 *              
 * @version     V1.2022.02
 * @authors     Yiqing Huang
 * @date        2022 Feb
 * @note        The RLS and RBR interrupts are always on, the THRE interrupt
 *              is only on when we need to transmit data stream.
 *****************************************************************************/

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
        if (g_send_char == 1) {        // This flag is set by the IRQ handler upon an RX interrupt
            //pUart->THR &= ~IER_THRE;// turn off TX interrupt if it is on. But in this example, TX interrupt is off by the time we reach here
            pUart->THR = g_char_in; // the THR must be empty at this moment
            pUart->IER |= IER_THRE; // turn on the TX interrupt
            g_send_char = 0;        // clear the flag
        }
    }
}

/*
 *===========================================================================
 *                             END OF FILE
 *===========================================================================
 */
