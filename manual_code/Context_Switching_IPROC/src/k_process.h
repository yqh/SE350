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
 * @file        k_process.h
 * @brief       process management hearder file 
 *              
 * @version     V1.2021.01
 * @authors     Yiqing Huang and Thomas Reidemeister
 * @date        2021 JAN
 * @note        Assuming there are only two user processes in the system
 *****************************************************************************/

#ifndef K_PROCESS_H_
#define K_PROCESS_H_

#include "k_inc.h"

/*
 *===========================================================================
 *                             MACROS
 *===========================================================================
 */

#define INITIAL_xPSR 0x01000000        /* user process initial xPSR value */

/*
 *===========================================================================
 *                            FUNCTION PROTOTYPES
 *===========================================================================
 */

extern void process_init(PROC_INIT *proc_info, int num);               
                                       /* initialize all procs in the system */
extern PCB  *scheduler(void);          /* pick the pcb of the next to run process */
extern PCB  *scheduler_tms(void);      /* a time slicing scheduler */
extern int  process_switch(PCB *p_pcb_old);
extern int  k_release_processor(void); /* kernel release_process function */
extern int  k_run_new_process(void);   /* kernel to schedule a process to run */

#endif /* ! K_PROCESS_H_ */
/*
 *===========================================================================
 *                             END OF FILE
 *===========================================================================
 */
