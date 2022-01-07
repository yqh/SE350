/*
 ****************************************************************************
 *
 *                  UNIVERSITY OF WATERLOO SE 350 RTX LAB  
 *
 *                     Copyright 2020-2021 Yiqing Huang
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
 * @file        k_memory.h
 * @brief       kernel memory management header file            
 * @version     V1.2021.01
 * @authors     Yiqing Huang
 * @date        2021 JAN
 * @attention   Do not modify existing furnction prototypes
 *****************************************************************************/

#ifndef K_MEM_H_
#define K_MEM_H_

#include "k_inc.h"

/*
 *===========================================================================
 *                             MACROS
 *===========================================================================
 */

#define RAM_END_ADDR 0x10008000

/*
 *==========================================================================
 *                            GLOBAL VARIABLES
 *==========================================================================
 */
#if 0
/* This symbol is defined in the scatter file (see RVCT Linker User Guide) */  
extern unsigned int Image$$RW_IRAM1$$ZI$$Limit; 
extern PCB          **gp_pcbs;
extern PROC_INIT    g_proc_table[NUM_TEST_PROCS];
#endif 

/*
 *===========================================================================
 *                            FUNCTION PROTOTYPES
 *===========================================================================
 */
extern void memory_init(void);
extern U32  *alloc_stack(U32 size_b);
extern void *k_request_memory_block(void);
extern int  k_release_memory_block(void *);

#endif /* ! K_MEM_H_ */
/*
 *===========================================================================
 *                             END OF FILE
 *===========================================================================
 */
