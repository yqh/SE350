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
 * @file        ae.c
 * @brief       automated testing framework source code 
 *              
 * @version     V1.2021.02
 * @authors     Yiqing Huang
 * @date        2022 JAN
 * 
 * @attention
 * @note
 * @details
 *
 *****************************************************************************/
 
#include "ae.h"
#include "ae_proc.h"

/*
 *===========================================================================
 *                            FUNCTION
 *===========================================================================
 */

/**************************************************************************//**
 * @brief   	ae_init
 * @return		RTX_OK on success and RTX_ERR on failure
 * @param[out]  proc_info proc initialization struct array AE writes to
 * @param[in]   num       length of proc_info array
 * @param[in]   cb_func   call back function entry point
 * @param[in]   arg       call back function input argument
 * 
 *****************************************************************************/

int ae_init(PROC_INIT *proc_info, int num, int (*cb_func) (void *(arg)), void *arg)
{
    if ( proc_info == NULL || num != NUM_TEST_PROCS ) {
        return RTX_ERR;
    }
    
    cb_func(arg);
    
    ae_set_proc_info(proc_info, num);
    return RTX_OK;
}

/**************************************************************************//**
 * @brief   	 call this function to end testing
 *****************************************************************************/
void ae_exit(void) 
{
    while(1);
}

/**************************************************************************//**
 * @brief   	ae_set_proc_info
 * @param[out]  proc_info proc initialization struct array AE writes to
 * @param[in]   num       length of proc_info array
 *****************************************************************************/

void ae_set_proc_info(PROC_INIT *procs, int num) {
    set_test_procs(procs, num);    
/*
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
*/
}

/*
 *===========================================================================
 *                             END OF FILE
 *===========================================================================
 */
