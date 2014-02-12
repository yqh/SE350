/**
 * @brief:  uart_polling.h 
 * @author: Yiqing Huang
 * @date:   2014/01/05
 */

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

int uart_init(int n_uart);      /* initialize the n_uart       */
int uart_get_char(int n_uart);  /* read a char from the n_uart */
int uart_put_char(int n_uart, unsigned char c);   /* write a char   to n_uart */
int uart_put_string(int n_uart, unsigned char *s);/* write a string to n_uart */
void putc(void *p, char c);     /* call back function for printf, use uart1 */

#endif /* ! UART_POLLING_H_ */
