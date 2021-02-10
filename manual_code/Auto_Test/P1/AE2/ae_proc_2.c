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
 * @file        ae_proc2.c
 * @brief       Two auto test processes to test memory preeption and ownership
 *              
 * @version     V1.2021.01
 * @authors     Yiqing Huang
 * @date        2021 JAN
 * @note        Each process is in an infinite loop. Processes never terminate.
 * @details
 *
 *****************************************************************************/
/*---------------------------------------------------------------------------- 
 * Expected COM1 Output 
 * Assume we only have TWO memory blocks in the system.
 * Expected UART output: (assuming memory block has ownership.):
 * ABCDE
 * FGHIJ
 * 01234
 * KLMNO
 * 56789
 * proc2: end of testing
 * proc3: 
 * proc4: 
 * proc3: 
 * proc4: 
 * proc3: 
 * proc4: 
 * proc3: 
 * proc4:
 * .....
 * you see proc3 and proc4 alternate and keep printing
 *-------------------------------------------------------------------------------*/ 

#include "rtx.h"
#include "uart_polling.h"
#include "ae_proc.h"

#ifdef DEBUG_0
#include "printf.h"
#endif /* DEBUG_0 */


/**************************************************************************//**
 * @brief: a process that keeps allocating memory without freeing
 *****************************************************************************/
void proc1(void)
{
    int i = 0;
    void *p_mem_blk;
    while ( 1 ) {
        if ( i != 0 && i%5 == 0 ) {
            uart1_put_string("\n\r");
            p_mem_blk = request_memory_block();
#ifdef DEBUG_0
            printf("proc1: p_mem_blk=0x%x\n", p_mem_blk);
#endif /* DEBUG_0 */
        }
        uart1_put_char('A' + i%26);
        i++;
    }
}
/**************************************************************************//**
 * @brief  a process that tries to free another process's memory
 *****************************************************************************/
void proc2(void)
{
    int i = 0;
    int ret_val = 20;
    void *p_mem_blk;
    
    p_mem_blk = request_memory_block();
    set_process_priority(PID_P2, LOW);
    while ( 1) {
        if ( i != 0 && i%5 == 0 ) {
            uart1_put_string("\n\r");
            ret_val = release_memory_block(p_mem_blk);            
#ifdef DEBUG_0
            printf("proc2: ret_val=%d\n", ret_val);
#endif /* DEBUG_0 */
            if ( ret_val == -1 ) {
                break;
            }
        }
        uart1_put_char('0' + i%10);
        i++;
    }
    uart1_put_string("proc2: end of testing\n\r");
    set_process_priority(PID_P2, MEDIUM);
    set_process_priority(PID_P3, LOW);
    set_process_priority(PID_P4, LOW);
    set_process_priority(PID_P2, LOWEST);
    while ( 1 ) {
        release_processor();
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
