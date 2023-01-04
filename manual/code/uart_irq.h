/********************************************************************//**
 * @file uart_irq.h
 ***********************************************************************/
#ifndef UART_IRQ_H_
#define UART_IRQ_H_

/* typedefs */
#include <stdint.h>	
#include "uart_def.h"

/* initialize the n_uart to use interrupt */
int uart_irq_init(int n_uart);		

#endif /* ! UART_IRQ_H_ */
