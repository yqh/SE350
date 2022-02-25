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
 * @brief       echoes keyboard input and outputs a string to UART0 by interrupt.
 *              UART1 is by polling. It is used as a debugging terminal. 
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

uint8_t g_buffer[]= "You Typed a Q\r\n";    // g_buffer[12] = 'Q'
extern uint8_t g_send_char;
extern uint8_t g_char_in;
extern uint8_t g_tx_irq;

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
        if (g_send_char == 1 && !g_tx_irq) {   // these flags are also written by the IRQ handler
            pUart->THR = g_char_in;     // the THR must be empty at this moment
            g_tx_irq = 1;               // tx irq is to be ON
            g_send_char = 0;            // clear the flag
            g_buffer[12] = g_char_in;   // replace Q by the g_char_in in the buffer.
            pUart->IER |= IER_THRE;     // turn on the TX interrupt to output g_buffer contents  
        }
    }
}

/*
 *===========================================================================
 *                             END OF FILE
 *===========================================================================
 */
