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
 * @brief   P2 public testing code TS211
 * @author  Yiqing Huang
 * @date    2022/02/04
 * @note    Each process is in an infinite loop. Processes never terminate.
 *****************************************************************************/

/* expected output at COM1 (polled terminal) 
proc2: receiving messages ...
proc1: requesting a mem_blk...
proc1: send messages to proc2...
proc2: got a message - A
abcde
ABCDE
proc1: send a message to proc2...
proc2: got a message - 0
fghij
FGHIJ
proc1: send a message to proc2...
proc2: got a message - 1
klmno
KLMNO
proc1: send a message to proc2...
proc2: got a message - 2
pqrst
PQRST
proc1: send a message to proc2...
proc2: got a message - 3
uvwxy
UVWXY
proc1: send a message to proc2...
proc2: got a message - 4
proc3: entering..., starting delay_send
proc3: done with delay_send
proc4: entering...
proc4: received e
proc4: received d
proc4: received c
proc4: received b
proc4: received a
*/


#include "rtx.h"
#include "ae_proc.h"
#include "ae_proc_ext.h"
#include "ae_util.h"
#include "uart_polling.h"
#include "printf.h"

/*
 *===========================================================================
 *                             MACROS
 *===========================================================================
 */

#ifdef AE_ENABLE
    
#define NUM_TESTS       2       // number of tests

#ifdef AE_ECE350
#define NUM_INIT_TASKS  2       // number of tasks during initialization
#endif // AE_ECE350

#endif // AE_ENABLE
/*
 *===========================================================================
 *                             GLOBAL VARIABLES 
 *===========================================================================
 */
 
#ifdef AE_ENABLE

#ifdef AE_ECE350
TASK_INIT    g_init_tasks[NUM_INIT_TASKS];
#endif


const char   PREFIX[]      = "G99-TS211";
const char   PREFIX_LOG[]  = "G99-TS211-LOG ";
const char   PREFIX_LOG2[] = "G99-TS211-LOG2";

AE_XTEST     g_ae_xtest;                // test data, re-use for each test
AE_CASE      g_ae_cases[NUM_TESTS];
AE_CASE_TSK  g_tsk_cases[NUM_TESTS];

#endif // AE_ENABLE

int g_iterations;
/* initialization table item */
void set_test_procs(PROC_INIT *procs, int num)
{
    int i;
    for( i = 0; i < num; i++ ) {
        procs[i].m_pid        = (U32)(i+1);
        procs[i].m_stack_size = 0x200;
    }
  
    procs[0].mpf_start_pc = &proc1;
    procs[0].m_priority   = MEDIUM;
    
    procs[1].mpf_start_pc = &proc2;
    procs[1].m_priority   = HIGH;
    
    procs[2].mpf_start_pc = &proc3;
    procs[2].m_priority   = LOW;
    
    procs[3].mpf_start_pc = &proc4;
    procs[3].m_priority   = LOW;
    
    procs[4].mpf_start_pc = &proc5;
    procs[4].m_priority   = LOWEST;
    
    procs[5].mpf_start_pc = &proc6;
    procs[5].m_priority   = LOWEST;
    
    g_iterations = 5;
    init_ae_tsk_test();
}

void init_ae_tsk_test(void)
{
    g_ae_xtest.test_id = 0;
    g_ae_xtest.index = 0xFF;
    g_ae_xtest.num_tests = NUM_TESTS;
    g_ae_xtest.num_tests_run = 0;
    
    for ( int i = 0; i< NUM_TESTS; i++ ) {
        g_tsk_cases[i].p_ae_case = &g_ae_cases[i];
        g_tsk_cases[i].p_ae_case->results  = 0x0;
        g_tsk_cases[i].p_ae_case->test_id  = i;
        g_tsk_cases[i].p_ae_case->num_bits = 0;
        g_tsk_cases[i].pos = 0;  // first avaiable slot to write exec seq tid
        // *_expt fields are case specific, deligate to specific test case to initialize
    }
    printf("%s: START\r\n", PREFIX);
    
    int test_id = 0;
    test0_start(test_id);
}

void update_ae_xtest(int test_id)
{
    g_ae_xtest.test_id = test_id;
    g_ae_xtest.index = 0xFF;
    g_ae_xtest.num_tests_run++;
}

int update_exec_seq(int test_id, task_t tid) 
{

    U8 len = g_tsk_cases[test_id].len;
    U8 *p_pos = &g_tsk_cases[test_id].pos;
    task_t *p_seq = g_tsk_cases[test_id].seq;
    p_seq[*p_pos] = tid;
    (*p_pos)++;
    (*p_pos) = (*p_pos)%len;  // preventing out of array bound
    return RTX_OK;
}


/**
 * @brief test0 iniitialization
 * @details 8 sub-tests
      bit[0]: new process initial run test
      bit[1]: an already run process can be resumed properly
      bit[2]: local variables are preserved properly
 */

void gen_req0(int test_id)
{
    const int num_exec = 16;
    
    g_tsk_cases[test_id].p_ae_case->num_bits = 11;  
    g_tsk_cases[test_id].p_ae_case->results = 0;
    g_tsk_cases[test_id].p_ae_case->test_id = test_id;
    g_tsk_cases[test_id].len = MAX_LEN_MSG; // assign a value no greater than MAX_LEN_SEQ
    g_tsk_cases[test_id].pos_expt = num_exec;
       
    update_ae_xtest(test_id);
}


int test0_start(int test_id)
{
    gen_req0(test_id);
    
    // one can add some extra work to be performed here
    return 0;
}

void gen_req1(int test_id, int test_id_data)
{
    const int num_exec = 16;
    g_tsk_cases[test_id].p_ae_case->num_bits = num_exec;  
    g_tsk_cases[test_id].p_ae_case->results = 0;
    g_tsk_cases[test_id].p_ae_case->test_id = test_id;
    g_tsk_cases[test_id].len = 0;       // N/A, use test0 data
    g_tsk_cases[test_id].pos_expt = 0;  // N/A, use test0 data
    
    update_ae_xtest(test_id);
    task_t  *p_seq_expt = g_tsk_cases[test_id_data].seq_expt;
      
    // expected execution sequence setup
    // 2->1 repeat for 7 times followed by 3->4
    int i = -1;
    for (int j = 0; j < 7; j++) {
        p_seq_expt[++i] = PID_P2;
        p_seq_expt[++i] = PID_P1;
    }
    
    p_seq_expt[++i] = PID_P3;
    p_seq_expt[++i] = PID_P4;
}

int test1_start(int test_id, int test_id_data)
{
    gen_req1(test_id, test_id_data);
    check_exec_seq(test_id, test_id_data);
    return 0;
}

/**
 * @brief   task yield exec order test
 * @param   test_id, the test function ID 
 * @param   ID of the test function that logs the testing data
 * @note    usually test data is logged by the same test function,
 *          but some time, we may have multiple tests to check the same test data
 *          logged by a particular test function
 */
void check_exec_seq(int test_id, int test_id_data)
{  
    
    U8      pos         = g_tsk_cases[test_id_data].pos;
    U8      pos_expt    = g_tsk_cases[test_id_data].pos_expt;
    task_t  *p_seq      = g_tsk_cases[test_id_data].seq;
    task_t  *p_seq_expt = g_tsk_cases[test_id_data].seq_expt;
       
    U8      *p_index    = &(g_ae_xtest.index);
    int     sub_result  = 0;
    
    // output the real execution order
    printf("%s: real exec order: ", PREFIX_LOG);
    int pos_len = (pos > MAX_LEN_SEQ)? MAX_LEN_SEQ : pos;
    for ( int i = 0; i < pos_len; i++) {
        printf("%d -> ", p_seq[i]);
    }
    printf("NIL\r\n");
    
    // output the expected execution order
    printf("%s: expt exec order: ", PREFIX_LOG);
    for ( int i = 0; i < pos_expt; i++ ) {
        printf("%d -> ", p_seq_expt[i]);
    }
    printf("NIL\r\n");
    
    for (int i = 0; i < pos_expt; i++) {
        (*p_index)++;
        strcpy(g_ae_xtest.msg, "message passing preemption test");
        sub_result = (p_seq[i] == p_seq_expt[i]) ? 1 : 0;
        process_sub_result(test_id, *p_index, sub_result);
    }
    
    test_exit();
}

/**
 * @brief: a process that prints five uppercase letters on each line
 *         and sends a message to P2
 */
void proc1(void)
{
    int     i = 0;
    int     j = 0;
    void    *p_blk;
    MSG_BUF *p_msg;
    char    *ptr;
    int test_id = 0;
    
    update_exec_seq(test_id, PID_P1);
    
    uart1_put_string("proc1: requesting a mem_blk...\r\n");
    p_blk = request_memory_block();
    p_msg = p_blk;
    p_msg->mtype = DEFAULT;
    ptr = p_msg->mtext;
    *ptr++ = 'A';
    *ptr++ = '\r';
    *ptr++ = '\n';
    *ptr++ = '\0';
    
    uart1_put_string("proc1: send messages to proc2...\r\n");
    send_message(PID_P2, p_blk);
    update_exec_seq(test_id, PID_P1);
    
    while (1) {
        if ( i != 0 && i%5 == 0 ) {
            uart1_put_string("\r\n");
            //ret_val = release_processor();
            p_blk = request_memory_block();
            p_msg = p_blk;
            p_msg->mtype = DEFAULT;
            ptr = p_msg->mtext;
            *ptr++ = ('0' + (j++)%10);
            *ptr++ = '\r';
            *ptr++ = '\n';
            *ptr++ = '\0';
            uart1_put_string("proc1: send a message to proc2...\r\n");
            send_message(PID_P2, p_blk);
            update_exec_seq(test_id, PID_P1);
        }
        if ( j == g_iterations ) {
            break;
        }
        uart1_put_char('A' + i%26);
        i++;
    }
    
    printf("%s: PID = %d setting its own priority to LOWEST\r\n", PREFIX_LOG2, PID_P1);
    set_process_priority(PID_P1, LOWEST);
    update_exec_seq(test_id, PID_P1);
    test1_start(test_id + 1, test_id);
    while (1) {
        release_processor();
    }
}

/**
 * @brief: a process that prints five lower case letters
 *         and receives messages.
 */
void proc2(void)
{
    int     i = 0;
    int     j = 0;
    MSG_BUF *p_msg;
    void    *p_blk;
    
    int test_id = 0;
    U8  *p_index    = &(g_ae_xtest.index);
    int sub_result  = 0;

    update_exec_seq(test_id, PID_P2);
    
    uart1_put_string("proc2: receiving messages ...\r\n");
    p_blk = receive_message(NULL);
    update_exec_seq(test_id, PID_P2);
    
    p_msg = p_blk;

    sub_result = (p_msg->mtext[0] == 'A') ? 1 : 0;
    (*p_index)++;
    sprintf(g_ae_xtest.msg, "PID = %d, receive a message - %c", PID_P2, p_msg->mtext[0]);
    process_sub_result(test_id, *p_index, sub_result);
    
    uart1_put_string("proc2: got a message - ");
    uart1_put_string(p_msg->mtext);
   
    release_memory_block(p_blk);
    
    while (1) {
        if ( i != 0 && i%5 == 0 ) {
            uart1_put_string("\r\n");
            p_blk = receive_message(NULL);
            p_msg = p_blk;
            update_exec_seq(test_id, PID_P2);
            
            sub_result = (p_msg->mtext[0] == '0' + j) ? 1 : 0;
            (*p_index)++;
            sprintf(g_ae_xtest.msg, "PID = %d, receive a message - %c", PID_P2, p_msg->mtext[0]);
            process_sub_result(test_id, *p_index, sub_result);
            
            uart1_put_string("proc2: got a message - ");
            uart1_put_string(p_msg->mtext);
            release_memory_block(p_blk);
            j++;
        }
        if ( j == g_iterations ) {
            break;
        }
        uart1_put_char('a' + i%26);
        i++;
    }
    
    printf("%s: PID = %d setting its own priority to LOW\r\n", PREFIX_LOG2, PID_P2);
    set_process_priority(PID_P2, LOW);

    while(1) {
        release_processor();
    }
}

/**
 * @brief a process that does delayed_send 5 messages in decreasing order of delay
 */

void proc3(void)
{
    char *ptr;
    int delay[] =  {60, 45, 30, 15, 0};
    int i=0;
    int num_msgs = 5;
    MSG_BUF *p_msg;
    void *p_blk;
    
    int test_id = 0;
    
    update_exec_seq(test_id, PID_P3);
    uart1_put_string("proc3: entering..., starting delay_send\r\n");    
    while( i < num_msgs ) {
        p_blk = request_memory_block();
#ifdef DEBUG_0
        printf("=0%x, i =[%d], delay= %d\r\n",p_blk, i, delay[i]);
#endif // DEBUG_0
        
        p_msg = p_blk;
        p_msg->mtype = DEFAULT;
        ptr = (char *)p_msg->mtext;
        *ptr++ = 'a' + i;
        *ptr++ = '\r';
        *ptr++ = '\n';
        *ptr++ = '\0';
#ifdef DEBUG_0
        uart1_put_string("proc3: delayed_send to proc4...\r\n");
#endif // DEBUG_0
        delayed_send(PID_P4, p_blk, delay[i++]); 
    }
    uart1_put_string("proc3: done with delay_send\r\n");
    while ( 1 ) {
        release_processor();
    }
}

/**
 * @brief a process that receives messages
 */

void proc4(void)
{
    MSG_BUF *p_msg;
    void    *p_blk;
    int     send_id;
    int     j = 0;
    int     test_id = 0;
    U8      *p_index    = &(g_ae_xtest.index);
    int     sub_result  = 0;
    
    update_exec_seq(test_id, PID_P4);
    
    uart1_put_string("proc4: entering...\r\n");
    while(1) {
        p_blk = receive_message(&send_id);
        p_msg = p_blk;
        
        sub_result = (p_msg->mtext[0] == 'a' + 4 - j) ? 1 : 0;
        (*p_index)++;
        sprintf(g_ae_xtest.msg, "PID = %d, receive a message - %c", PID_P2, p_msg->mtext[0]);
        process_sub_result(test_id, *p_index, sub_result);
        
        uart1_put_string("proc4: received ");
        uart1_put_string(p_msg->mtext);
        release_memory_block(p_blk);
        j++;
        if (j == 5) {
            test1_start(test_id + 1, test_id);
        }
    }
}

void proc5(void)
{
    int test_id = 0;
    update_exec_seq(test_id, PID_P5);
    while(1) {
        uart1_put_string("proc5:\r\n");
        release_processor();
        test1_start(test_id + 1, test_id);
    }
}

void proc6(void)
{
    int test_id = 0;
    update_exec_seq(test_id, PID_P6);
    while(1) {
        uart1_put_string("proc6:\r\n");
        release_processor();
        test1_start(test_id + 1, test_id);
    }
}
