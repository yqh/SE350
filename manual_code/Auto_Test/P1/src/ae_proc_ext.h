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
 * @file        ae_proc_ext.h
 * @brief       six user test processes header file extension
 *              
 * @version     V1.2022.01
 * @authors     Yiqing Huang
 * @date        2022 JAN
 *****************************************************************************/
 
#ifndef AE_PROC_EXT_H_
#define AE_PROC_EXT_H_

#include "ae_util.h"

/*
 *===========================================================================
 *                             MACROS
 *===========================================================================
 */

/*
 *===========================================================================
 *                            FUNCTION PROTOTYPES
 *===========================================================================
 */


extern void init_ae_tsk_test   (void);
extern void update_ae_xtest    (int test_id);
extern int  update_exec_seq    (int test_id, task_t tid);
extern void check_exec_seq     (int test_id, int test_id_data);
extern void gen_req0           (int test_id);
extern int  test0_start        (int test_id);
extern void gen_req1           (int test_id, int test_id_data);
extern int  test1_start        (int test_id, int test_id_data);

#endif // AE_PROC_EXT_H_

/*
 *===========================================================================
 *                             END OF FILE
 *===========================================================================
 */
