/*
 ****************************************************************************
 *
 *                  UNIVERSITY OF WATERLOO SE/ECE 350 RTX/RTOS LAB
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
 * @file        ae_util.c
 * @brief       auto-tester utility functions
 *
 * @version     V1.2022.01
 * @authors     Yiqing Huang
 * @date        2022 Jan
 *
 *****************************************************************************/

#include "ae_util.h"

#ifdef AE_ENABLE
char *strcpy(char* dest, const char* source)
{
    if ( dest == NULL || source == NULL ) {
        return NULL;
    }
    
    char *ptr = dest;
    while ((*ptr++ = *source++) != '\0');
    return dest;   
}


/**
 * @brief   Function to get number of set bits in binary
            representation of passed binary no. 
 * @note    Brian Kernighan’s Algorithm
 */
unsigned int count_set_bits(int n)
{
    unsigned int count = 0;
    while (n) {
        n &= (n - 1);
        count++;
    }
    return count;
}




void test_exit(void)
{
    printf("%s: FINISHED\r\n", PREFIX);
    print_summary();
}


void test_abort(int test_id, int index)
{
    
    printf("%s: ERROR: Last Active Test [%d]-[%d] - %s\r\n", PREFIX, test_id, index, g_ae_xtest.msg);
    printf("%s: ABORTED\r\n", PREFIX);
    print_summary();
}

void print_summary(void)
{
    print_results(g_ae_cases);
    printf("%s: END\r\n", PREFIX);
    ae_exit();
    
}

void print_results(AE_CASE *p_ae_cases)
{
    int total = 0;
    int total_pass  = 0;
    
    for ( int i = 0; i < g_ae_xtest.num_tests; i++) {
        int num_bits = p_ae_cases[i].num_bits;
        int pass = count_set_bits(p_ae_cases[i].results);
        total_pass += pass;
        total += num_bits;
        printf("%s: Test %d: %u/%u PASSED. Expected = 0x%X, result = 0x%X\r\n", \
            PREFIX_LOG, i, pass, num_bits, BIT(num_bits) - 1, p_ae_cases[i].results);
    }
    printf("%s: %u/%u test function(s) COMPLETED.\r\n",PREFIX, g_ae_xtest.num_tests_run, g_ae_xtest.num_tests);
    printf("%s: %d/%u tests PASSED.\r\n", PREFIX, total_pass, total);
    printf("%s: %d/%u tests FAILED.\r\n", PREFIX, total - total_pass, total);    
}

int print_log(int test_id, int index, int result, char *msg)
{
    printf("%s: Test[%d]-[%d]: %s.\r\n",  PREFIX_LOG, test_id, index, msg);
    printf("%s: Test[%d]-[%d]: result = %d.\r\n",  PREFIX_LOG, test_id, index, result);
    return 0;
}

int process_sub_result(int test_id, int index, int result)
{
    if ( result == 1 ) {
        g_ae_cases[test_id].results |= BIT(index);
    }
    print_log(test_id, index, result, g_ae_xtest.msg);
    return 0;
}



void HardFault_Handler(void)
{
    printf("%s: HardFault\r\n", PREFIX);
    test_abort(g_ae_xtest.test_id, g_ae_xtest.index);
}

#endif
/*
 *===========================================================================
 *                             END OF FILE
 *===========================================================================
 */
