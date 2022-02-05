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
 * @brief   P2 public testing code TS202
 * @author  Yiqing Huang
 * @date    2022/02/04
 * @note    Each process is in an infinite loop. Processes never terminate.
 *****************************************************************************/

/*------------------------------------------------------------------------*
expected output at COM1 (polled terminal) 
---------------------------------------------------------------------------
proc2: entering...
proc1: entering...
proc3:
proc2:
proc1: tm.sec = 0, tm.nsec = 9220540
proc1: public test 4: dealyed_send with 10ms PASSED.
--------------------------------------------------------------------------
NOTE: the tm.sec and tm.nsec most likely will not match your output.
      the time absolute error tolerance is 1 ms, 
      which is the granularity of the timer0 interrupt frequency.
-------------------------------------------------------------------------*/

#include "ae_proc.h"
#include "ae_proc_ext.h"
#include "ae.h"
#include "ae_util.h"

/*
 *===========================================================================
 *                             MACROS
 *===========================================================================
 */

#ifdef AE_ENABLE
    
#define NUM_TESTS       1       // number of tests

#ifdef AE_ECE350
#define NUM_INIT_TASKS  1       // number of tasks during initialization
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


const char   PREFIX[]      = "G99-TS202";
const char   PREFIX_LOG[]  = "G99-TS202-LOG ";
const char   PREFIX_LOG2[] = "G99-TS202-LOG2";

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
    
    p_msg = receive_message(&sender_id);
    p_ae_msg_req = (struct ae_msg_req *) (p_msg->mtext);
    req_no = p_ae_msg_req->req_no;
    if (req_no == AE_RST_100) {        
        tk2.pc = p_ae_msg_req->tk.pc;
        tk2.tc = p_ae_msg_req->tk.tc;
        ae_get_tick_diff(&tm, &tk1, &tk2);
        //ae_get_tick_diff(&tm, &tk2, &tk1);
        printf("proc1: tm.sec = %d, tm.nsec = %d\r\n", tm.sec, tm.nsec);
        if ( tm.nsec > (AE_10M_NSEC - AE_1M_NSEC) && tm.nsec < (AE_10M_NSEC + AE_1M_NSEC) ) {
            uart1_put_string("proc1: public test TS202: dealyed_send with 10ms PASSED.\r\n");
        } else {
            uart1_put_string("proc1: public test TS202: dealyed_send with 10ms FAILED.\r\n");
        }
    } else {
        printf("proc1: ERR - got RST_NO %d from sender PID %d, dropping \r\n", req_no, sender_id );
    }
    release_memory_block(p_msg);
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
    
    uart1_put_string("proc2: entering...\r\n");
    p_msg = receive_message(&sender_id);
    ae_get_tick(&tk_recv, AE_TIMER2);
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
    
    while(1) {
        //uart1_put_string("proc2:\r\n");
        release_processor();
    }
}

void proc3(void)
{
    uart1_put_string("proc3:\r\n");
    while(1) {
        release_processor();
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
