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
 * @file        ae_proc_111.c
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
abcde
fghij
01234
56789
01234
56789
proc2 end of testing
proc1 end of testing
G99-TS111-LOG : real exec order: 1 -> 1 -> 2 -> 2 -> 2 -> 2 -> 2 -> 1 -> NIL
G99-TS111-LOG : expt exec order: 1 -> 1 -> 2 -> 2 -> 2 -> 2 -> 2 -> 1 -> NIL
G99-TS111-LOG : Test[0]-[0]: new task priority scheduling.
G99-TS111-LOG : Test[0]-[0]: result = 1.
G99-TS111-LOG : Test[0]-[1]: release_proc when current running proc. ready queue is empty.
G99-TS111-LOG : Test[0]-[1]: result = 1.
G99-TS111-LOG : Test[0]-[2]: set_proc_prio preemption, raising other lower prio proc's prio higher than the current running proc's prio .
G99-TS111-LOG : Test[0]-[2]: result = 1.
G99-TS111-LOG : Test[0]-[3]: release_proc when current running proc. ready queue is empty.
G99-TS111-LOG : Test[0]-[3]: result = 1.
G99-TS111-LOG : Test[0]-[4]: release_proc when current running proc. ready queue is empty.
G99-TS111-LOG : Test[0]-[4]: result = 1.
G99-TS111-LOG : Test[0]-[5]: release_proc when current running proc. ready queue is empty.
G99-TS111-LOG : Test[0]-[5]: result = 1.
G99-TS111-LOG : Test[0]-[6]: set_proc_prio no preemption, raising other lower prio proc's prio to the same as the current running proc's.
G99-TS111-LOG : Test[0]-[6]: result = 1.
G99-TS111-LOG : Test[0]-[7]: release_proc when current running proc. ready queue is not empty .
G99-TS111-LOG : Test[0]-[7]: result = 1.
G99-TS111: FINISHED
G99-TS111-LOG : Test 0: 8/8 PASSED. Expected = 0xFF, result = 0xFF
G99-TS111: 1/1 test function(s) COMPLETED.
G99-TS111: 8/8 tests PASSED.
G99-TS111: 0/8 tests FAILED.
G99-TS111: END
-----------------------------------------------------------------------------*/
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


const char   PREFIX[]      = "G99-TS111";
const char   PREFIX_LOG[]  = "G99-TS111-LOG ";
const char   PREFIX_LOG2[] = "G99-TS111-LOG2";

AE_XTEST     g_ae_xtest;                // test data, re-use for each test
AE_CASE      g_ae_cases[NUM_TESTS];
AE_CASE_TSK  g_tsk_cases[NUM_TESTS];

#endif // AE_ENABLE

/*
 *===========================================================================
 *                             ADDED FUNCS PROTOTYPES
 *===========================================================================
 */

void gen_req0           (int test_id);
int  test0_start        (int test_id);
void test0_end          (int test_id);
void init_ae_tsk_test   (void);
int  update_exec_seq    (int test_id, task_t tid);



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
	procs[1].m_priority   = LOW;
	
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


/**
 * @brief test0 iniitialization
 * @details 8 sub-tests
      bit[0]: correct intial priority scheduling test
      bit[1]: release_proc when the current running proc's ready queue is empty
      bit[2]: set_prio preempts, raise another ready lower prio proc's to higher prio
      bit[5:3]: release_proc when the current running proc's ready queue is empty
      bit[6]: set_prio do not preempt when raising target pro to the same prio as the current running proc
      bit[7]: release_proc when the current running proc's ready queue is not empty
 */

void gen_req0(int test_id)
{
    g_tsk_cases[test_id].p_ae_case->num_bits = 8;  
    g_tsk_cases[test_id].p_ae_case->results = 0;
    g_tsk_cases[test_id].p_ae_case->test_id = test_id;
    g_tsk_cases[test_id].len = 16; // assign a value no greater than MAX_LEN_SEQ
    g_tsk_cases[test_id].pos_expt = 8;
       
    update_ae_xtest(test_id);
    
    // expected execution sequence setup
    
    task_t *p_seq_expt = g_tsk_cases[test_id].seq_expt;
    
    for (int i = 0; i < 2; i++) {
        p_seq_expt[i]  = PID_P1;
    }
    
    for (int i = 2; i < 7; i++) {
        p_seq_expt[i]  = PID_P2;
    }
    
    p_seq_expt[7] = PID_P1;
}


int test0_start(int test_id)
{
    gen_req0(test_id);
    
    // one can add some extra work to be performed here
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
    
    // test 1-[0]
    *p_index = 0;
    strcpy(g_ae_xtest.msg, "new task priority scheduling");
    sub_result = (p_seq[0] == p_seq_expt[0]) ? 1 : 0;
    process_sub_result(test_id, *p_index, sub_result);
    
    //test 1-[1]
    (*p_index)++;
    strcpy(g_ae_xtest.msg, "release_proc when current running proc. ready queue is empty");
    sub_result = (p_seq[1] == p_seq_expt[1]) ? 1 : 0;
    process_sub_result(test_id, *p_index, sub_result);
    
    //test 1-[2]
    (*p_index)++;
    strcpy(g_ae_xtest.msg, "set_proc_prio preemption, raising other lower prio proc's prio higher than the current running proc's prio ");
    sub_result = (p_seq[2] == p_seq_expt[2]) ? 1 : 0;
    process_sub_result(test_id, *p_index, sub_result);
    
    //test 1-[5:3]
    for (int i = 3; i < 6; i++) {
        (*p_index)++;
        strcpy(g_ae_xtest.msg, "release_proc when current running proc. ready queue is empty");
        sub_result = (p_seq[i] == p_seq_expt[i]) ? 1 : 0;
        process_sub_result(test_id, *p_index, sub_result);
    }
    
    //test 1-[6]
    (*p_index)++;
    strcpy(g_ae_xtest.msg, "set_proc_prio no preemption, raising other lower prio proc's prio to the same as the current running proc's");
    sub_result = (p_seq[6] == p_seq_expt[6]) ? 1 : 0;
    process_sub_result(test_id, *p_index, sub_result);
    
    //test 1-[7]
    (*p_index)++;
    strcpy(g_ae_xtest.msg, "release_proc when current running proc. ready queue is not empty ");
    sub_result = (p_seq[7] == p_seq_expt[7]) ? 1 : 0;
    process_sub_result(test_id, *p_index, sub_result);
        
    test_exit();
}

/**************************************************************************//**
 * @brief: a process that prints two lines of five lowercase letters
 *         and then changes P2's priority to HIGH
 *         and then yields the processor.
 *****************************************************************************/
void proc1(void)
{
	int i = 0;
	int counter = 0;
	int ret_val = 100;
    
    int test_id = 0;
    
    test0_start(test_id);
    update_exec_seq(test_id, PID_P1);
    
	while ( 1 ) {
		
		if ( i != 0 && i%5 == 0 ) {
			uart1_put_string("\n\r");
			counter++;
			if ( counter == 2 ) {
				ret_val = set_process_priority(PID_P2, HIGH);
                update_exec_seq(test_id, PID_P1); 
				break;
			} else {
				ret_val = release_processor();
                update_exec_seq(test_id, PID_P1); 
			}
#ifdef DEBUG_0
			printf("proc1: ret_val = %d \n", ret_val);
#endif /* DEBUG_0 */
		}
		uart1_put_char('a' + i%10);
		i++;
	}
	uart1_put_string("proc1 end of testing\n\r");
    release_processor();
    //test_exit();
    check_exec_seq(test_id, test_id);
    
	while ( 1 ) {
		release_processor();
	}
}

/**************************************************************************//**
 * @brief  a process that prints five numbers, change P1's priority to HIGH
 *         and then yields the cpu.
 *****************************************************************************/
void proc2(void)
{
	int i = 0;
	int ret_val = 20;
	int counter = 0;
    int test_id = 0;
	
	while ( 1) {
		if ( i != 0 && i%5 == 0 ) {
			uart1_put_string("\n\r");
			counter++;
			if ( counter == 4 ) {
				ret_val = set_process_priority(PID_P1, HIGH);
				update_exec_seq(test_id, PID_P2); 
                break;
			} else {
				ret_val = release_processor();
                update_exec_seq(test_id, PID_P2); 
			}
#ifdef DEBUG_0
			printf("proc2: ret_val=%d\n", ret_val);
#endif /* DEBUG_0 */
		}
		uart1_put_char('0' + i%10);
		i++;
	}
	uart1_put_string("proc2 end of testing\n\r");
    update_exec_seq(test_id, PID_P2);
    release_processor();
    check_exec_seq(test_id, test_id);

	while ( 1 ) {
		release_processor();
	}
}

void proc3(void)
{
    int test_id = 0;
    while(1) {
        uart1_put_string("proc3: \n\r");
        release_processor();
        check_exec_seq(test_id, test_id);
    }
}

void proc4(void)
{
    int test_id = 0;
    while(1) {
        uart1_put_string("proc4: \n\r");
        release_processor();
        check_exec_seq(test_id, test_id);
    }
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

