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
 * @file        ae_inc.h
 * @brief       ae testing data structures and global var externs
 *
 * @version     V1.2021.05
 * @authors     Yiqing Huang
 * @date        2021 May
 *
 *****************************************************************************/
#ifndef AE_INC_H_
#define AE_INC_H_
#include "ae.h"


/*
 *===========================================================================
 *                             MACROS
 *===========================================================================
 */

#define MAX_LEN_MSG         128     // test message in bytes including '\0'
#define MAX_LEN_SEQ         32      // max test case execution sequence record length
/*
 *===========================================================================
 *                             TYPEDEFS AND STRUCTURES
 *===========================================================================
 */
#ifndef AE_ECE350
typedef unsigned char task_t;
#endif
/**
 * @brief   test case data object
 * @note    each test suite contains num_tests cases, where each test case
 *          contains some sub cases.
 * @details this structure is used by all test case objects
 *          only one test case object can use at a time
 */
typedef struct ae_xtest
{
    U8    test_id;                  /**< test case ID */
    U8    index;                    /**< sub test index in a specific test case*/
    U8    num_tests;                /**< number of test cases/functions in a test suite*/
    U8    num_tests_run;            /**< number of completed test cases/functions */
    char  msg[MAX_LEN_MSG];         /**< testing message buffer */
} AE_XTEST;

/**
 * @brief   generic test case object
 */

typedef struct ae_case 
{
    U32         results;            /**< test results, each test has sub test for each bit */
    U8          test_id;            /**< test ID **/
    U8          num_bits;           /**< results are saved in bits[0:num-1] */
} AE_CASE;  

/**
 * @briekf  task management test case object
 */

typedef struct ae_case_tsk
{
    AE_CASE     *p_ae_case;                 /**< points to generic case object */    
    U8          len;                        /**< length of seq_exec array */
    U8          pos;                        /**< next free spot position in the seq array*/
    U8          pos_expt;                   /**< expected next free spot position in seq_expt when test finishes */    
    task_t      seq[MAX_LEN_SEQ];           /**< actual task execution order   */
    task_t      seq_expt[MAX_LEN_SEQ];      /**< expected task exectuion order */
} AE_CASE_TSK;

/*
 *===========================================================================
 *                             GLOBAL VARIABLES 
 *===========================================================================
 */



// The following globals are defined in the Test Suite .c file
extern const char   PREFIX[];       // test output file prefix
extern const char   PREFIX_LOG[];   // test output file log prefix
extern AE_XTEST     g_ae_xtest;     // test data, re-use by each test case
extern AE_CASE      g_ae_cases[];   // points to the array of testing cases
extern AE_CASE_TSK  g_tsk_cases[];  // array of testing cases

/*
 *===========================================================================
 *                            FUNCTION PROTOTYPES
 *===========================================================================
 */

#endif // ! AE_INC_H_

