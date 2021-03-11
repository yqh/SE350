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
 * @file        ae.h
 * @brief       automated testing framework header file
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
 
#include "rtx.h"

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
extern void set_test_procs(PROC_INIT *procs, int num);

/*
 *===========================================================================
 *                             END OF FILE
 *===========================================================================
 */
