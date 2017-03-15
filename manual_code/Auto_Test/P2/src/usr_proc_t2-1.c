/**
 * @file:   usr_proc_t2-1.c
 * @brief:  proc6 registers %G command and process it.
 * @author: Yiqing Huang
 * @date:   2017/03/10
 * NOTE: Each process is in an infinite loop. Processes never terminate.
 */

/* expected output at UART#1 (polled terminal) 
proc6: registering %G command
proc6: type %G in RTX console now.
proc6: received: %G
proc6: asking CRT to display OK!
proc6: received: %G
proc6: asking CRT to display OK!
.....

Expected output at UART#0 (interrupt driven terminal
%G
OK!
%G
OK!
......
*/


#include "rtx.h"
#include "uart_polling.h"
#include "usr_proc_test.h"

/* initialization table item */
PROC_INIT g_test_procs[NUM_TEST_PROCS];

void set_test_procs() {
	int i;
	for( i = 0; i < NUM_TEST_PROCS; i++ ) {
		g_test_procs[i].m_pid=(U32)(i+1);
		g_test_procs[i].m_stack_size=0x100;
	}
  
	g_test_procs[0].mpf_start_pc = &proc1;
	g_test_procs[0].m_priority   = LOW;
	
	g_test_procs[1].mpf_start_pc = &proc2;
	g_test_procs[1].m_priority   = LOW;
	
	g_test_procs[2].mpf_start_pc = &proc3;
	g_test_procs[2].m_priority   = LOW;
	
	g_test_procs[3].mpf_start_pc = &proc4;
	g_test_procs[3].m_priority   = LOW;
	
	g_test_procs[4].mpf_start_pc = &proc5;
	g_test_procs[4].m_priority   = MEDIUM;
	
	g_test_procs[5].mpf_start_pc = &proc6;
	g_test_procs[5].m_priority   = HIGH;
	
	uart1_init();
}


/**
 * @brief: a dummy process
 */
void proc1(void)
{
	while(1) {
		release_processor();
	}
}

/**
 * @brief: a dummy process
 */
void proc2(void)
{
	while(1) {
		release_processor();
	}
	
}

/**
 * @brief: a dummy process
 */
void proc3(void)
{
	while(1) {
		release_processor();
	}
	
}

/**
 * @brief: a dummy process
 */
void proc4(void)
{
	while(1) {
		release_processor();
	}
	
}

/**
 * @brief: a dummy process
 */
void proc5(void)
{
	while(1) {
		release_processor();
	}
}

void proc6(void)
{
	MSG_BUF *p;
	int send_id;
	
	p = request_memory_block();
	p->mtype = KCD_REG;
	p->mtext[0] = '%';
	p->mtext[1] = 'G';
	p->mtext[2] = '\0';
	uart1_put_string("proc6: registering %G command\n\r");
	if ( send_message(PID_KCD, p) == 0 ) {
		uart1_put_string("proc6: type %G in RTX console now.\n\r");
	} else {
		uart1_put_string("proc6: send_message to KCD failed.\n\r");
		while(1) {
			release_processor();
		}
	}
	
	while (1) {
		p = receive_message(&send_id);
		uart1_put_string("proc6: received: ");
		uart1_put_string((U8 *)p->mtext);
		uart1_put_string("\n\r");
		if ( p->mtype == KCD_CMD || p->mtype == DEFAULT ) {
			if ( p->mtext[0] == '%' && p->mtext[1] == 'G') {
				uart1_put_string("proc6: asking CRT to display OK!\n\r");
			
				p->mtype = CRT_DISPLAY;
				p->mtext[0] = 'O';
				p->mtext[1] = 'K';
				p->mtext[2] = '!';
				p->mtext[3] = '\n';
				p->mtext[4] = '\r';
				p->mtext[5] = '\0';
				send_message(PID_CRT, p);
			} else {
				uart1_put_string("proc6: received a non-command.\n\r");
				release_memory_block(p);
			}
		}
	}
}
