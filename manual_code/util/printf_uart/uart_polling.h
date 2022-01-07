/*
 ****************************************************************************
 *
 *                  UNIVERSITY OF WATERLOO ECE 350 RTX LAB  
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
 * @file        uart_polling.h 
 * @brief       uart polling header file
 *              
 * @version     V1.2021.01
 * @authors     Yiqing Huang
 * @date        2021 JAN
 *****************************************************************************/

#ifndef UART_POLLING_H_
#define UART_POLLING_H_

#include <stdint.h>  /* typedefs */
#include "uart_def.h"

#define uart0_init()        uart_init(0)  
#define uart0_get_char()    uart_get_char(0)
#define uart0_put_char(c)   uart_put_char(0,c)
#define uart0_put_string(s) uart_put_string(0,s)

#define uart1_init()        uart_init(1)  
#define uart1_get_char()    uart_get_char(1)
#define uart1_put_char(c)   uart_put_char(1,c)
#define uart1_put_string(s) uart_put_string(1,s)

int  uart_init(int n_uart);      /* initialize the n_uart       */
int  uart_get_char(int n_uart);  /* read a char from the n_uart */
int  uart_put_char(int n_uart, char c);   /* write a char   to n_uart */
int  uart_put_string(int n_uart, char *s);/* write a string to n_uart */
void putc(void *p, char c);      /* call back function for printf, use uart1 */

#endif /* ! UART_POLLING_H_ */

/*
 *===========================================================================
 *                             END OF FILE
 *===========================================================================
 */
 
