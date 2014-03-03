/**
 * @brief: UART defines
 * @file: uart_def.h 
 * @author: Yiqing Huang
 * @date: 2014/02/08
 */

#ifndef UART_DEF_H_
#define UART_DEF_H_

/* The following macros are from NXP uart.h */
#define IER_RBR		0x01
#define IER_THRE	0x02
#define IER_RLS		0x04

#define IIR_PEND	0x01
#define IIR_RLS		0x03
#define IIR_RDA		0x02
#define IIR_CTI		0x06
#define IIR_THRE	0x01

#define LSR_RDR		0x01
#define LSR_OE		0x02
#define LSR_PE		0x04
#define LSR_FE		0x08
#define LSR_BI		0x10
#define LSR_THRE	0x20
#define LSR_TEMT	0x40
#define LSR_RXFE	0x80

#define BUFSIZE		0x40
/* end of NXP uart.h file reference */


/* convenient macro for bit operation */
#define BIT(X)    ( 1 << (X) )

/* 
   8 bits, no Parity, 1 Stop bit
   
   0x83 = 1000 0011 = 1 0 00 0 0 11
   LCR[7]  =1  enable Divisor Latch Access Bit DLAB
   LCR[6]  =0  disable break transmission
   LCR[5:4]=00 odd parity
   LCR[3]  =0  no parity
   LCR[2]  =0  1 stop bit
   LCR[1:0]=11 8-bit char len
   See table 279, pg306 LPC17xx_UM
*/
#define UART_8N1  0x83

#ifndef NULL
#define NULL 0
#endif


#endif /* !UART_DEF_H_ */
