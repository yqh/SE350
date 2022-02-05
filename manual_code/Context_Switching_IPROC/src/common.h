/*
 ****************************************************************************
 *
 *                  UNIVERSITY OF WATERLOO SE 350 RTX LAB  
 *
 *                     Copyright 2020-2022 Yiqing Huang
 *                          All rights reserved.
 *---------------------------------------------------------------------------
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *  - Redistributions of source code must retain the above copyright
 *    notice and the following disclaimer.
 *
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *---------------------------------------------------------------------------*/

/**************************************************************************//**
 * @file        common.h
 * @brief       common defines and structs for both kernel and user 
 *              
 * @version     V1.2021.01
 * @authors     Yiqing Huang
 * @date        2021 JAN
 * 
 * @attention   DO NOT MODIFY
 *
 *****************************************************************************/

#ifndef COMMON_H_
#define COMMON_H_

#include "common_ext.h"

/*
 *===========================================================================
 *                             MACROS
 *===========================================================================
 */

#define TRUE                1
#define FALSE               0
#define NULL                0
#define RTX_ERR             -1
#define RTX_OK              0
#define NUM_TEST_PROCS      6

/* Process IDs */
#define PID_NULL            0
#define PID_P1              1
#define PID_P2              2
#define PID_P3              3
#define PID_P4              4
#define PID_P5              5
#define PID_P6              6
#define PID_A               7
#define PID_B               8
#define PID_C               9
#define PID_SET_PRIO        10
#define PID_CLOCK           11
#define PID_KCD             12
#define PID_CRT             13
#define PID_TIMER_IPROC     14
#define PID_UART_IPROC      15


/* Process Priority. The bigger the number is, the lower the priority is*/
#define HIGH                0
#define MEDIUM              1
#define LOW                 2
#define LOWEST              3
#define PRI_NULL            4

/* Process Stack Size */
#ifdef DEBUG_0
#define USR_SZ_STACK 0x200         /* user proc stack size 512B   */
#else
#define USR_SZ_STACK 0x100         /* user proc stack size 256B  */
#endif /* DEBUG_0 */

/* Memory Blocks Configuration */
#define MEM_BLK_SIZE        128 /* memory block size in bytes */
#define MEM_NUM_BLKS        32  /* number of memory blocks    */

/* Message Types */
#define DEFAULT             0
#define KCD_REG             1
#define KCD_CMD             2
#define CRT_DISPLAY         3
#define KEY_IN              4

/*
 *===========================================================================
 *                             TYPEDEFS
 *===========================================================================
 */
typedef unsigned char       U8;
typedef unsigned int        U32;
typedef unsigned char       BOOL;

/*
 *===========================================================================
 *                             TYPEDEFS AND STRUCTURES
 *===========================================================================
 */

/* initialization table item, exposed to user space */
typedef struct proc_init
{    
    int m_pid;                  /**> process id                 */ 
    int m_priority;             /**> initial priority           */ 
    int m_stack_size;           /**> size of stack in bytes     */
    void (*mpf_start_pc) ();    /**> entry point of the process */    
} PROC_INIT;

/* message buffer */
typedef struct msgbuf
{
#ifdef K_MSG_ENV
    void *mp_next;              /**> ptr to next message received*/
    int m_send_pid;             /**> sender pid                  */
    int m_recv_pid;             /**> receiver pid                */
    int m_kdata[5];             /**> extra 20B kernel data place holder */
#endif
    int mtype;                  /**> user defined message type   */
    char mtext[1];              /**> body of the message         */
} MSG_BUF;

#endif // COMMON_H_
