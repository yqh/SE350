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
 * @file        main.c
 * @brief       Timer interrupt main driver code
 *              Prints 0-9 every second. Loops around when 9 is reached. 
 *              
 * @version     V1.2021.02
 * @authors     Yiqing Huang
 * @date        2021 FEB
 *****************************************************************************/

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
    uart1_init();   
    __enable_irq();
    uart1_put_string("\r\nTimer0 IRQ fires every one millisecond.\n\r");
   
    while (1) {
        /* g_timer_count gets updated every 1ms */
        if (g_timer_count == 1000) { 
            uart1_put_char('0'+ sec);
            sec = (sec + 1)%10;
            if ( sec == 0) {
                uart1_put_string("\n\r");
            }
            g_timer_count = 0; /* reset the counter */
        }     
    }

}
/*
 *===========================================================================
 *                             END OF FILE
 *===========================================================================
 */
