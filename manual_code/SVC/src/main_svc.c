/**
 * @brief:  main_svc.c, demonstrate svc as a gateway to os functions
 * @author: Yiqing Huang
 * @date:   2014/01/09
 * NOTE: Standard C library is not allowed in the final kernel code.
 *       A tiny printf function for embedded application development
 *       taken from http://www.sparetimelabs.com/tinyprintf/tinyprintf.php
 *       and is configured to use UART0 to output.
 *       
 */

#include <LPC17xx.h>
#include "printf.h"
#include "uart_polling.h"
#include "rtx.h"

int main()
{
   
  volatile unsigned int ret_val = 1234;

  SystemInit();  /* initialize the system */
  __disable_irq();
  uart0_init();
  init_printf(NULL, putc);
  __enable_irq();
  
  // transit to unprivileged level, default MSP is used
  __set_CONTROL(__get_CONTROL() | BIT(0));  

  ret_val = release_processor();
  ret_val = (unsigned int) request_memory_block();
	ret_val = release_memory_block(NULL);
  /* printf has been retargeted to use the UART0,
     check putc function in uart0_polling.c.
  */
  printf("The ret_val=%d\n",ret_val); 
  return 0;  
}
