/*
 ****************************************************************************
 *
 *                  UNIVERSITY OF WATERLOO SE 350 RTOS LAB  
 *
 *                     Copyright 2020-2022 Yiqing Huang
 *
 *          This software is subject to an open source license and 
 *          may be freely redistributed under the terms of MIT License.
 ****************************************************************************
 */

/**************************************************************************//**
 * @file        ae_proc.c
 * @brief       Two auto test processes: proc1 and proc2
 *              
 * @version     V1.2022.01
 * @authors     Yiqing Huang
 * @date        2021 JAN
 * @note        Each process is in an infinite loop. Processes never terminate.
 * @details
 *
 *****************************************************************************/

#include "rtx.h"
#include "uart_polling.h"
#include "ae_proc.h"
#include "ae_proc_ext.h"
#include "ae_util.h"

/*
 *===========================================================================
 *                             MACROS
 *===========================================================================
 */

#ifdef AE_ENABLE
    
#define NUM_TESTS       2       // number of tests

#ifdef AE_ECE350
#define NUM_INIT_TASKS  2       // number of tasks during initialization
#endif // AE_ECE350

#endif // AE_ENABLE
/*
 *===========================================================================
 *                             GLOBAL VARIABLES 
 *===========================================================================
 */
 
#ifdef AE_ENABLE

#ifdef AE_ECE350
TASK_INIT    g_init_tasks[NUM_INIT_TASKS];
#endif


const char   PREFIX[]      = "G99-TS112";
const char   PREFIX_LOG[]  = "G99-TS112-LOG ";
const char   PREFIX_LOG2[] = "G99-TS112-LOG2";

AE_XTEST     g_ae_xtest;                // test data, re-use for each test
AE_CASE      g_ae_cases[NUM_TESTS];
AE_CASE_TSK  g_tsk_cases[NUM_TESTS];

#endif // AE_ENABLE


/* initialization table item */
void set_test_procs(PROC_INIT *procs, int num) {
    int i;
    for( i = 0; i < num; i++ ) {
        procs[i].m_pid        = (U32)(i+1);
        procs[i].m_priority   = LOWEST;
        procs[i].m_stack_size = USR_SZ_STACK;
    }
  
    procs[0].mpf_start_pc = &proc1;
    procs[1].mpf_start_pc = &proc2;
    procs[2].mpf_start_pc = &proc3;
    procs[3].mpf_start_pc = &proc4;
    procs[4].mpf_start_pc = &proc5;
    procs[5].mpf_start_pc = &proc6;
}

/**************************************************************************//**
 * @brief: a process that prints five uppercase letters in one line
 *         and then releases the processor.
 *****************************************************************************/
void proc1(void)
{
    int i = 0;
    while ( 1 ) {
        if ( i != 0 && i%5 == 0 ) {
            uart1_put_string("\r\n");
            uart1_put_string("proc1: releasing processor\r\n");
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
            uart1_put_string("\r\n");
        }
        uart1_put_char('0' + i%10);
        for ( int x = 0; x < DELAY; x++); // some artifical delay
        i++;
    }
}

void proc3(void)
{
    
    while(1) {
        uart1_put_string("proc3: \r\n");
        release_processor();
    }
}

void proc4(void)
{
    while(1) {
        uart1_put_string("proc4: \r\n");
        release_processor();
    }
}

void proc5(void)
{
    while(1) {
        uart1_put_string("proc5: \r\n");
        release_processor();
    }
}

void proc6(void)
{
    while(1) {
        uart1_put_string("proc6: \r\n");
        release_processor();
    }
}
/*
 *===========================================================================
 *                             END OF FILE
 *===========================================================================
 */
