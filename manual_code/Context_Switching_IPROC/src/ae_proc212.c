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
 * @brief   P2 public testing code TS212
 * @author  Yiqing Huang
 * @date    2022/02/04
 * @note    Each process is in an infinite loop. Processes never terminate.
 *****************************************************************************/

/*------------------------------------------------------------------------*
expected output at COM1 (polled terminal) 
---------------------------------------------------------------------------
G99-TS212: START
proc2: entering...
proc1: entering...
proc3:
proc2:
proc1: tm.sec = 0, tm.nsec = 9121060
proc1: public test TS212: dealyed_send with 10ms PASSED.
G99-TS212-LOG : Test[0]-[0]: PID = 1: received a message by dealyed_send with 10ms.
G99-TS212-LOG : Test[0]-[0]: result = 1.
G99-TS212-LOG : real exec order: 2 -> 1 -> 1 -> 3 -> 2 -> 1 -> NIL
G99-TS212-LOG : expt exec order: 2 -> 1 -> 1 -> 3 -> 2 -> 1 -> NIL
G99-TS212-LOG : Test[1]-[0]: message passing preemption test.
G99-TS212-LOG : Test[1]-[0]: result = 1.
G99-TS212-LOG : Test[1]-[1]: message passing preemption test.
G99-TS212-LOG : Test[1]-[1]: result = 1.
G99-TS212-LOG : Test[1]-[2]: message passing preemption test.
G99-TS212-LOG : Test[1]-[2]: result = 1.
G99-TS212-LOG : Test[1]-[3]: message passing preemption test.
G99-TS212-LOG : Test[1]-[3]: result = 1.
G99-TS212-LOG : Test[1]-[4]: message passing preemption test.
G99-TS212-LOG : Test[1]-[4]: result = 1.
G99-TS212-LOG : Test[1]-[5]: message passing preemption test.
G99-TS212-LOG : Test[1]-[5]: result = 1.
G99-TS212: FINISHED
G99-TS212-LOG : Test 0: 1/1 PASSED. Expected = 0x1, result = 0x1
G99-TS212-LOG : Test 1: 6/6 PASSED. Expected = 0x3F, result = 0x3F
G99-TS212: 2/2 test function(s) COMPLETED.
G99-TS212: 7/7 tests PASSED.
G99-TS212: 0/7 tests FAILED.
G99-TS212: END
--------------------------------------------------------------------------
NOTE: the tm.sec and tm.nsec most likely will not match your output.
      the time absolute error tolerance is 1 ms, 
      which is the granularity of the timer0 interrupt frequency.
-------------------------------------------------------------------------*/

#include "ae_proc.h"
//#include "ae.h"
#include "ae_proc_ext.h"
#include "ae_util.h"

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


const char   PREFIX[]      = "G99-TS212";
const char   PREFIX_LOG[]  = "G99-TS212-LOG ";
const char   PREFIX_LOG2[] = "G99-TS121-LOG2";

AE_XTEST     g_ae_xtest;                // test data, re-use for each test
AE_CASE      g_ae_cases[NUM_TESTS];
AE_CASE_TSK  g_tsk_cases[NUM_TESTS];

#endif // AE_ENABLE


/* initialization table item */
void set_test_procs(PROC_INIT *procs, int num)
{
    int i;
    for( i = 0; i < num; i++ ) {
        procs[i].m_pid        = (U32)(i+1);
        procs[i].m_stack_size = 0x300;
    }
  
    procs[0].mpf_start_pc = &proc1;
    procs[0].m_priority   = MEDIUM;
    
    procs[1].mpf_start_pc = &proc2;
    procs[1].m_priority   = HIGH;
    
    procs[2].mpf_start_pc = &proc3;
    procs[2].m_priority   = LOW;
    
    procs[3].mpf_start_pc = &proc4;
    procs[3].m_priority   = LOWEST; 
    
    procs[4].mpf_start_pc = &proc5;
    procs[4].m_priority   = LOWEST;
    
    procs[5].mpf_start_pc = &proc6;
    procs[5].m_priority   = LOWEST;
    
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
    const int num_exec = 6;
    
    g_tsk_cases[test_id].p_ae_case->num_bits = 1;  
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
    const int num_exec = 6;
    g_tsk_cases[test_id].p_ae_case->num_bits = num_exec;  
    g_tsk_cases[test_id].p_ae_case->results = 0;
    g_tsk_cases[test_id].p_ae_case->test_id = test_id;
    g_tsk_cases[test_id].len = 0;       // N/A, use test0 data
    g_tsk_cases[test_id].pos_expt = 0;  // N/A, use test0 data
    
    update_ae_xtest(test_id);
    task_t  *p_seq_expt = g_tsk_cases[test_id_data].seq_expt;
      
    // expected execution sequence setup
    // 2->1->1->3->2->1
    int i = 0;
    p_seq_expt[i]   = PID_P2;
    p_seq_expt[++i] = PID_P1;
    p_seq_expt[++i] = PID_P1;
    p_seq_expt[++i] = PID_P3;
    p_seq_expt[++i] = PID_P2;
    p_seq_expt[++i] = PID_P1;  
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
 * @brief process 1 sends a message X to process 2 with a dealy of 10 ms 
 *        and receives the message X arrival time stamp from process 2
 *        test pass if the time between delay_send and message X arrival time
 *        is within 9 - 11 ms (i.e error tolerance is 1ms).
 */
void proc1(void)
{
    MSG_BUF *p_msg;
    struct  ae_tick tk1, tk2;
    struct  ae_time tm;
    struct  ae_msg_req *p_ae_msg_req;
    int     sender_id;
    U32     req_no;
    
    int     test_id = 0;
    U8      *p_index    = &(g_ae_xtest.index);
    int     sub_result  = 0;
    
    update_exec_seq(test_id, PID_P1);
    
    uart1_put_string("proc1: entering...\r\n");
    p_msg = (MSG_BUF *)request_memory_block();
    p_msg->mtype = DEFAULT;
    p_ae_msg_req = (struct ae_msg_req *) p_msg->mtext;
    p_ae_msg_req->req_no = AE_REQ_100;
    p_ae_msg_req->delay = 10;
    
    *(p_ae_msg_req->bytes) = 'A';
    *(p_ae_msg_req->bytes + 1) = '\0';
    
    ae_get_tick(&tk1, AE_TIMER2);
    delayed_send(PID_P2, (void *) p_msg, 10);
    update_exec_seq(test_id, PID_P1);
    
    p_msg = receive_message(&sender_id);
    update_exec_seq(test_id, PID_P1);
    p_ae_msg_req = (struct ae_msg_req *) (p_msg->mtext);
    req_no = p_ae_msg_req->req_no;
    if (req_no == AE_RST_100) {        
        tk2.pc = p_ae_msg_req->tk.pc;
        tk2.tc = p_ae_msg_req->tk.tc;
        ae_get_tick_diff(&tm, &tk1, &tk2);
        //ae_get_tick_diff(&tm, &tk2, &tk1);
        printf("proc1: tm.sec = %d, tm.nsec = %d\r\n", tm.sec, tm.nsec);
        if ( tm.nsec > (AE_10M_NSEC - AE_1M_NSEC) && tm.nsec < (AE_10M_NSEC + AE_1M_NSEC) ) {
            sub_result = 1;
            uart1_put_string("proc1: public test TS212: dealyed_send with 10ms PASSED.\r\n");
        } else {
            sub_result = 0;
            uart1_put_string("proc1: public test TS212: dealyed_send with 10ms FAILED.\r\n");
        }
        (*p_index)++;
        sprintf(g_ae_xtest.msg, "PID = %d: received a message by dealyed_send with 10ms", PID_P1);
        process_sub_result(test_id, *p_index, sub_result);
    } else {
        printf("proc1: ERR - got RST_NO %d from sender PID %d, dropping \r\n", req_no, sender_id );
        test_abort(test_id, 0);
    }
    release_memory_block(p_msg);
    test1_start(test_id + 1, test_id);
    while (1) {
        //uart1_put_string("proc1:\r\n");
        release_processor();
    }
}

/**
 * @brief process 2 waits a message X from process 1 that was delayed sent with 10ms delay. 
 *        It writes the message X arrival time stamp and send back the message to process 1 immediately.
 *        It then lower down its own priority to LOW and keeps releasing processor.
 */

void proc2(void)
{
    MSG_BUF *p_msg;
    int     sender_id;
    U32     req_no;
    struct  ae_tick tk_recv;
    struct  ae_msg_req *p_ae_msg_req;
    
    int test_id = 0;
    update_exec_seq(test_id, PID_P2);
    
    uart1_put_string("proc2: entering...\r\n");
    p_msg = receive_message(&sender_id);
    ae_get_tick(&tk_recv, AE_TIMER2);
    update_exec_seq(test_id, PID_P2);
    p_ae_msg_req = (struct ae_msg_req *) (p_msg->mtext);
    req_no = p_ae_msg_req->req_no;
    if ( req_no == AE_REQ_100 ) {
        p_ae_msg_req->req_no = AE_RST_100;
        p_ae_msg_req->tk.pc = tk_recv.pc;
        p_ae_msg_req->tk.tc = tk_recv.tc;
        send_message(PID_P1, (void *) p_msg);
    } else {
        printf("Got REQ_NO %d from sender PID %d, dropping \r\n", req_no, sender_id );
        release_memory_block(p_msg);
    }
    uart1_put_string("proc2:\r\n");
    set_process_priority(PID_P2, LOW);
    update_exec_seq(test_id, PID_P2);
    
    while(1) {
        //uart1_put_string("proc2:\r\n");
        release_processor();
    }
}

void proc3(void)
{
    int test_id = 0;
    update_exec_seq(test_id, PID_P3);
    uart1_put_string("proc3:\r\n");
    while(1) {
        //release_processor();
    }
}

void proc4(void)
{
    uart1_put_string("proc4:\r\n");
    while(1) {
        release_processor();
    }
}

void proc5(void)
{
    while(1) {
        uart1_put_string("proc5:\r\n");
        release_processor();
    }
}

void proc6(void)
{
    while(1) {
        uart1_put_string("proc6:\r\n");
        release_processor();
    }
}
