/*
 ****************************************************************************
 *
 *                  UNIVERSITY OF WATERLOO SE 350 RTOS LAB  
 *
 *                     Copyright 2020-2021 Yiqing Huang
 *
 *          This software is subject to an open source license and 
 *          may be freely redistributed under the terms of MIT License.
 ****************************************************************************
 */

/**************************************************************************//**
 * @file        ae_proc.c
 * @brief       Two auto test processes: proc1 and proc2
 *              
 * @version     V1.2021.01
 * @authors     Yiqing Huang
 * @date        2021 JAN
 * @note        Each process is in an infinite loop. Processes never terminate.
 * @details
 *
 *****************************************************************************/

#include "rtx.h"
#include "uart_polling.h"
#include "ae_proc.h"


/**************************************************************************//**
 * @brief: a process that prints five uppercase letters in one line
 *         and then releases the processor.
 *****************************************************************************/
void proc1(void)
{
    int i = 0;
    while ( 1 ) {
        if ( i != 0 && i%5 == 0 ) {
            uart1_put_string("\n\r");
            uart1_put_string("proc1: releasing processor\n\r");
            release_processor();
        }
        uart1_put_char('A' + i%26);
        for ( int x = 0; x < DELAY; x++); // some artifical delay
        i++;
    }
}

/**************************************************************************//**
 * @brief  a process that keeps printing five numbers in one line
 *****************************************************************************/
void proc2(void)
{
    int i = 0;
    while ( 1 ) {
        if ( i != 0 && i%5 == 0 ) {
            uart1_put_string("\n\r");
        }
        uart1_put_char('0' + i%10);
        for ( int x = 0; x < DELAY; x++); // some artifical delay
        i++;
    }
}

void proc3(void)
{
    
    while(1) {
        uart1_put_string("proc3: \n\r");
        release_processor();
    }
}

void proc4(void)
{
    while(1) {
        uart1_put_string("proc4: \n\r");
        release_processor();
    }
}

void proc5(void)
{
    while(1) {
        uart1_put_string("proc5: \n\r");
        release_processor();
    }
}

void proc6(void)
{
    while(1) {
        uart1_put_string("proc6: \n\r");
        release_processor();
    }
}
/*
 *===========================================================================
 *                             END OF FILE
 *===========================================================================
 */
