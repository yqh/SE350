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
 * @file        ae.c
 * @brief       automated testing framework source code 
 *              
 * @version     V1.2021.01
 * @authors     Yiqing Huang
 * @date        2021 JAN
 * 
 * @attention
 * @note
 * @details
 *
 *****************************************************************************/
 
#include "ae.h"
//#include "ae_proc.h"

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
    ae_timer_init_100MHZ(AE_TIMER2);
    ae_set_proc_info(proc_info, num);
    return RTX_OK;
}


/**************************************************************************//**
 * @brief   	 debugger .ini can reference this one to exit 
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
}

/*
 *===========================================================================
 *                             END OF FILE
 *===========================================================================
 */
