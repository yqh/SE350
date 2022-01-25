/*
 ****************************************************************************
 *
 *                  UNIVERSITY OF WATERLOO SE 350 RTOS LAB  
 *
 *                     Copyright 2020-2022 Yiqing Huang
 *
 *          This software is subject to an open source license and 
 *          may be freely redistributed under the terms of MIT License.
 ****************************************************************************
 */

/**************************************************************************//**
 * @file        ae_proc*.c
 * @brief       Two auto test processes: proc1 and proc2
 *              
 * @version     V1.2022.01
 * @authors     Yiqing Huang
 * @date        2022 JAN
 * @note        Each process is in an infinite loop. Processes never terminate.
 * @details
 *
 *****************************************************************************/
 /*---------------------------------------------------------------------------- 
 * Expected COM1 Output 
 * Assume we only have MEM_NUM_BLKS = 32 memory blocks in the system.
 * Expected UART output: (assuming memory block has ownership.):
 *------------------------------------------------------------------------------- 
G99-TS112: START
ABCDE
FGHIJ
KLMNO
PQRST
UVWXY
ZABCD
EFGHI
JKLMN
OPQRS
TUVWX
YZABC
DEFGH
IJKLM
NOPQR
STUVW
XYZAB
CDEFG
HIJKL
MNOPQ
RSTUV
WXYZA
BCDEF
GHIJK
LMNOP
QRSTU
VWXYZ
ABCDE
FGHIJ
KLMNO
PQRST
UVWXY
ZABCD
01234
G99-TS112-LOG2: PID = 2, proc2 releasing memory block owned by itself
EFGHI
G99-TS112-LOG : Test[0]-[0]: .
G99-TS112-LOG : Test[0]-[0]: result = 1.
56789
G99-TS112-LOG2: PID = 2, proc2 releasing memory block owned by proc1
G99-TS112-LOG : Test[0]-[0]: .
G99-TS112-LOG : Test[0]-[0]: result = 1.
proc2: end of testing
G99-TS112-LOG2: PID = 2, proc2 changes its own priority to MEDIUM
G99-TS112-LOG : Test[0]-[1]: set_prio of PID = 2  to MEDIUM, check return value.
G99-TS112-LOG : Test[0]-[1]: result = 1.
proc3: 
proc4: 
proc3: 
proc4: 
proc3: 
proc4: 
proc3: 
proc4: 
proc3: 
proc4: 
G99-TS112-LOG : real exec order: 2 -> 1 -> 1 -> 2 -> 1 -> 2 -> 2 -> 2 -> 2 -> 2 -> 3 -> 4 -> 3 -> 4 -> 3 -> 4 -> 3 -> 4 -> 3 -> 4 -> NIL
G99-TS112-LOG : expt exec order: 2 -> 1 -> 1 -> 2 -> 1 -> 2 -> 2 -> 2 -> 2 -> 2 -> 3 -> 4 -> 3 -> 4 -> 3 -> 4 -> 3 -> 4 -> 3 -> 4 -> NIL
G99-TS112-LOG : Test[1]-[0]: new task priority scheduling.
G99-TS112-LOG : Test[1]-[0]: result = 1.
G99-TS112-LOG : Test[1]-[1]: set_prio preemption, lower down current running proc's prio.
G99-TS112-LOG : Test[1]-[1]: result = 1.
G99-TS112-LOG : Test[1]-[2]: memory depletion.
G99-TS112-LOG : Test[1]-[2]: result = 1.
G99-TS112-LOG : Test[1]-[3]: blocked on memory.
G99-TS112-LOG : Test[1]-[3]: result = 1.
G99-TS112-LOG : Test[1]-[4]: release memory preemption.
G99-TS112-LOG : Test[1]-[4]: result = 1.
G99-TS112-LOG : Test[1]-[5]: blocked on memory.
G99-TS112-LOG : Test[1]-[5]: result = 1.
G99-TS112-LOG : Test[1]-[6]: set prio no preemption.
G99-TS112-LOG : Test[1]-[6]: result = 1.
G99-TS112-LOG : Test[1]-[7]: release_proc with blocked on memory proc existing.
G99-TS112-LOG : Test[1]-[7]: result = 1.
G99-TS112: FINISHED
G99-TS112-LOG : Test 0: 2/2 PASSED. Expected = 0x3, result = 0x3
G99-TS112-LOG : Test 1: 8/8 PASSED. Expected = 0xFF, result = 0xFF
G99-TS112: 2/2 test function(s) COMPLETED.
G99-TS112: 10/10 tests PASSED.
G99-TS112: 0/10 tests FAILED.
G99-TS112: END
*-------------------------------------------------------------------------------*/ 

#include "rtx.h"
#include "uart_polling.h"
#include "ae_proc.h"
#include "ae_util.h"
#include "ae_proc_ext.h"

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


const char   PREFIX[]      = "G99-TS112";
const char   PREFIX_LOG[]  = "G99-TS112-LOG ";
const char   PREFIX_LOG2[] = "G99-TS112-LOG2";

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
        procs[i].m_stack_size = USR_SZ_STACK;
    }
  
    procs[0].mpf_start_pc = &proc1;
	procs[0].m_priority   = MEDIUM;
	
	procs[1].mpf_start_pc = &proc2;
	procs[1].m_priority   = HIGH;
	
	procs[2].mpf_start_pc = &proc3;
	procs[2].m_priority   = LOWEST;
	
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
    g_ae_xtest.index = 0;
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
}

void update_ae_xtest(int test_id)
{
    g_ae_xtest.test_id = test_id;
    g_ae_xtest.index = 0;
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

void gen_req0(int test_id)
{
    const int num_exec = 20;
    g_tsk_cases[test_id].p_ae_case->num_bits = 2;  
    g_tsk_cases[test_id].p_ae_case->results = 0;
    g_tsk_cases[test_id].p_ae_case->test_id = test_id;
    g_tsk_cases[test_id].len = MAX_LEN_SEQ; // assign a value no greater than MAX_LEN_SEQ
    g_tsk_cases[test_id].pos_expt = num_exec;
       
    update_ae_xtest(test_id);
}

void gen_req1(int test_id, int test_id_data)
{
    //const int num_exec = 20;
    g_tsk_cases[test_id].p_ae_case->num_bits = 8;  
    g_tsk_cases[test_id].p_ae_case->results = 0;
    g_tsk_cases[test_id].p_ae_case->test_id = test_id;
    g_tsk_cases[test_id].len = 0;       // N/A, use test0 data
    g_tsk_cases[test_id].pos_expt = 0;  // N/A, use test0 data
       
    update_ae_xtest(test_id);
    task_t  *p_seq_expt = g_tsk_cases[test_id_data].seq_expt;
      
    // expected execution sequence setup
    
    int i = 0;
    p_seq_expt[i]   = PID_P2;
    p_seq_expt[++i] = PID_P1;
    p_seq_expt[++i] = PID_P1;
    p_seq_expt[++i] = PID_P2;
    p_seq_expt[++i] = PID_P1;
    for (int j = 0; j < 5; j++) {
        p_seq_expt[++i] = PID_P2;
    }    
    
    for (int j = 0; j < 5; j++) {
        p_seq_expt[++i] = PID_P3;
        p_seq_expt[++i] = PID_P4;
    }
}


int test0_start(int test_id)
{
    gen_req0(test_id);
    
    // one can add some extra work to be performed here
    return 0;
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
    
    //int diff = pos - pos_expt;
    
    // test 0-[0]
    *p_index = 0;
    strcpy(g_ae_xtest.msg, "new task priority scheduling");
    sub_result = (p_seq[0] == p_seq_expt[0]) ? 1 : 0;
    process_sub_result(test_id, *p_index, sub_result);
    
    //test 0-[1]
    (*p_index)++;
    strcpy(g_ae_xtest.msg, "set_prio preemption, lower down current running proc's prio");
    sub_result = (p_seq[1] == p_seq_expt[1]) ? 1 : 0;
    process_sub_result(test_id, *p_index, sub_result);
    
    //test 0-[2]
    (*p_index)++;
    strcpy(g_ae_xtest.msg, "memory depletion");
    sub_result = (p_seq[2] == p_seq_expt[2]) ? 1 : 0;
    process_sub_result(test_id, *p_index, sub_result);
    
    //test 0-[3]
    (*p_index)++;
    strcpy(g_ae_xtest.msg, "blocked on memory");
    sub_result = (p_seq[3] == p_seq_expt[3]) ? 1 : 0;
    process_sub_result(test_id, *p_index, sub_result);
    
    //test 0-[4]
    (*p_index)++;
    strcpy(g_ae_xtest.msg, "release memory preemption");
    sub_result = (p_seq[4] == p_seq_expt[4]) ? 1 : 0;
    process_sub_result(test_id, *p_index, sub_result);
    
    //test 0-[5]
    (*p_index)++;
    strcpy(g_ae_xtest.msg, "blocked on memory");
    sub_result = (p_seq[5] == p_seq_expt[5]) ? 1 : 0;
    process_sub_result(test_id, *p_index, sub_result);
    
    // test 0-[6], bits[9:6]
    sub_result = 1;
    for (int i = 6; i < 10; i++) {
        if (p_seq[i] != p_seq_expt[i]) {
            sub_result = 0;
            break;
        }
    }
    (*p_index)++;
    strcpy(g_ae_xtest.msg, "set prio no preemption");
    process_sub_result(test_id, *p_index, sub_result);
    
    // test 0-[7], bits[19:10]
    sub_result = 1;
    for (int i = 10; i < 20; i++) {
        if (p_seq[i] != p_seq_expt[i]) {
            sub_result = 0;
            break;
        }
    }
    (*p_index)++;
    strcpy(g_ae_xtest.msg, "release_proc with blocked on memory proc existing");
    process_sub_result(test_id, *p_index, sub_result);
    
    
        
    test_exit();
}

/**************************************************************************//**
 * @brief: a process that keeps allocating memory without freeing
 *****************************************************************************/
void proc1(void)
{
    int i = 0;
    int j = 0;
    void *p_mem_blk;
    int test_id = 0;
    
    update_exec_seq(test_id, PID_P1);
    
    while ( 1 ) {
        if ( i != 0 && i%5 == 0 ) {
            uart1_put_string("\n\r");
            j++;
            if (j > MEM_NUM_BLKS - 1 ) {
                update_exec_seq(test_id, PID_P1);
            }
            p_mem_blk = request_memory_block();
#ifdef DEBUG_0
            printf("proc1: p_mem_blk=0x%x\n", p_mem_blk);
#endif /* DEBUG_0 */
        }
        uart1_put_char('A' + i%26);
        i++;
    }
}
/**************************************************************************//**
 * @brief  a process that tries to free another process's memory
 *****************************************************************************/
void proc2(void)
{
    int i = 0;
    int ret_val = 20;
    void *p_mem_blk;
    int test_id = 0;
    U8  *p_index    = &(g_ae_xtest.index);
    int sub_result  = 0;
    
    test0_start(test_id);
    
    update_exec_seq(test_id, PID_P2);
    p_mem_blk = request_memory_block();
    set_process_priority(PID_P2, LOW);
    update_exec_seq(test_id, PID_P2);
    while (1) {
        if (i != 0 && i%5 == 0) {
            uart1_put_string("\n\r");
            if  (i > 5) {
                printf("%s: PID = %d, proc2 releasing memory block owned by proc1\r\n", PREFIX_LOG2, PID_P2 );
            } else if (i == 5) {
                printf("%s: PID = %d, proc2 releasing memory block owned by itself\r\n", PREFIX_LOG2, PID_P2 );    
            }
            ret_val = release_memory_block(p_mem_blk);
            update_exec_seq(test_id, PID_P2);            
#ifdef DEBUG_0
            printf("proc2: ret_val=%d\n", ret_val);
#endif /* DEBUG_0 */
            
            if (i == 5) {
                sub_result = (ret_val == RTX_OK) ? 1:0;
            } else if (i > 5) {
                sub_result = (ret_val == RTX_ERR) ? 1:0;
            }
            
            *p_index = 0;
            process_sub_result(test_id, *p_index, sub_result);

            if (ret_val == RTX_ERR) {
                break;
            }
        }
        uart1_put_char('0' + i%10);
        i++;
    }
    uart1_put_string("proc2: end of testing\n\r");
    
    printf("%s: PID = %d, proc2 changes its own priority to MEDIUM\r\n", PREFIX_LOG2, PID_P2 );
    ret_val = set_process_priority(PID_P2, MEDIUM);
    
    sub_result = (ret_val == RTX_OK) ? 1:0;
    (*p_index)++;
    sprintf(g_ae_xtest.msg, "set_prio of PID = %d  to MEDIUM, check return value", PID_P2);
    process_sub_result(test_id, *p_index, sub_result);
    
    update_exec_seq(test_id, PID_P2);
    
    set_process_priority(PID_P3, LOW);
    update_exec_seq(test_id, PID_P2);
    set_process_priority(PID_P4, LOW);
    update_exec_seq(test_id, PID_P2);
    set_process_priority(PID_P2, LOWEST);
    
    while ( 1 ) {
        release_processor();
    }
}

void proc3(void)
{
    int test_id = 0;
    
    for (int i = 0; i < 5; i++) {
        uart1_put_string("proc3: \n\r");
        release_processor();
        update_exec_seq(test_id, PID_P3);
    }
    release_processor();
    test1_start(test_id + 1, test_id);
}

void proc4(void)
{
    int test_id = 0;

    for (int i = 0; i < 5; i++) {
        uart1_put_string("proc4: \n\r");
        release_processor();
        update_exec_seq(test_id, PID_P4);
    }
    
    release_processor();
    test1_start(test_id + 1, test_id);
}

void proc5(void)
{
    int test_id = 0;
    while(1) {
        uart1_put_string("proc5: \n\r");
        release_processor();
        check_exec_seq(test_id, test_id);
    }
}

void proc6(void)
{
    int test_id = 0;
    while(1) {
        uart1_put_string("proc6: \n\r");
        release_processor();
        check_exec_seq(test_id, test_id);
    }
}
/*
 *===========================================================================
 *                             END OF FILE
 *===========================================================================
 */
