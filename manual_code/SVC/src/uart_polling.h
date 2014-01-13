/**
 * @brief:  uart_polling.h 
 * @author: Yiqing Huang
 * @date:   2014/01/12
 */

#ifndef UART_POLLING_H_
#define UART_POLLING_H_

#include <stdint.h>  /* typedefs */

#define NULL 0

/* The following macros are taken from NXP sample UART project uart.h */
#define LSR_RDR   0x01
#define LSR_THRE  0x20
/* end of NXP uart.h file reference */


#define UART_8N1  0x83  /* 8 bits, no Parity, 1 Stop bit, DLAB enabled */
                        /* 0x83 = 1000 0011 = 1 0 00 0 0 11
                           LCR:7 enable Divisor Latch Access Bit DLAB
                           LCR:6 disable break transmission
                           LCR:5-4 odd parity
                           LCR:3 no parity
                           LCR:2-1 8-bit char len
                           Table 279, page 306, LPC17xxum.pdf v2
                         */

#define BIT(X)(1 << (X))/* convenient macro for bit operation */


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
void putc(void *p, char c);     /* call back function for printf */

#endif /* ! UART_POLLING_H_ */
