/*
 ****************************************************************************
 *
 *                  UNIVERSITY OF WATERLOO ECE 350 RTOS LAB
 *
 *                     Copyright 2020-2022 Yiqing Huang
 *                          All rights reserved.
 *
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
 ****************************************************************************
 */

/**************************************************************************//**
 * @file        ae_util.h
 * @brief       auto-tester utility function header file
 *
 * @version     V1.2021.06
 * @authors     Yiqing Huang
 * @date        2021 JUN
 *
 *****************************************************************************/
#ifndef AE_UTIL_H_
#define AE_UTIL_H_
#include "ae.h"
#include "ae_inc.h"



/*
 *===========================================================================
 *                            FUNCTION PROTOTYPES
 *===========================================================================
 */

void test_abort(int test_id, int index);
void test_exit(void);

void print_results(AE_CASE *p_ae_cases);
int  print_log(int test_id, int index, int result, char *msg); 
int  process_sub_result(int test_id, int index, int result);
void print_summary(void);

char *strcpy(char* dest, const char* source);



#endif // ! AE_UTIL_H_

