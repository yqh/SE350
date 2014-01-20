/**
 * @file:   main_svc.c
 * @brief:  main routine to start up the RTX and processes
 * @author: Yiqing Huang
 * @date:   2014/01/17
 * NOTE: standard C library is not allowed in the final kernel code.
 *       A tiny printf function for embedded application development
 *       taken from http://www.sparetimelabs.com/tinyprintf/tinyprintf.php
 *       is configured to use UART0 to output when DEBUG_0 is defined.
 *       Check target option->C/C++ to see the DEBUG_0 definition.
 *       Note that init_printf(NULL, putc) must be called to initialize 
 *       the printf function.
 */

#include <LPC17xx.h>
#include <system_LPC17xx.h>
#include "rtx.h"
#ifdef DEBUG_0
#include "uart_polling.h"
#include "printf.h"
#endif /* DEBUG_0 */

int main() 
{	
	/* CMSIS system initialization */
	SystemInit(); 
#ifdef DEBUG_0
	init_printf(NULL, putc);
#endif /* DEBUG_0 */
	
	/* start the RTX and built-in processes */
	rtx_init();  
  
	/* We should never reach here!!! */
	return RTX_ERR;  
}
