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
 * @file        ae.h
 * @brief       automated testing framework header file
 *              
 * @version     V1.2022.01
 * @authors     Yiqing Huang
 * @date        2022 JAN
 * 
 * @attention
 * @note
 * @details
 *
 *****************************************************************************/
 
#ifndef _AE_H_
#define _AE_H_

#include "uart_polling.h"
#include "printf.h"
#include "rtx.h"
#include "ae_timer.h"

/*
 *===========================================================================
 *                             MACROS
 *===========================================================================
 */
#ifdef RAM_TARGET
#define IROM_BASE  0x10000000
#else
#define IROM_BASE  0x0
#endif

/*
 *===========================================================================
 *                            FUNCTION PROTOTYPES
 *===========================================================================
 */
extern int  ae_init(PROC_INIT *proc_info, int num, int (*cb_func) (void * arg), void *arg);
extern void ae_set_proc_info(PROC_INIT *tasks, int num);
extern void ae_exit(void); 
extern void set_test_procs(PROC_INIT *procs, int num);

#endif  // ! _AE_H_

/*
 *===========================================================================
 *                             END OF FILE
 *===========================================================================
 */
