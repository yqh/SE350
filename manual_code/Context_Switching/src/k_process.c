/*
 ****************************************************************************
 *
 *                  UNIVERSITY OF WATERLOO SE 350 RTX LAB  
 *
 *           Copyright 2020-2022 Yiqing Huang and Thomas Reidemeister
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
 * @file        k_process.c 
 * @brief       kernel process management C file 
 *              
 * @version     V1.2022.01
 * @authors     Yiqing Huang, Thomas Reidemeister
 * @date        2022 JAN
 * @note        The example code shows one way of implementing context switching.
 *              The code only has minimal sanity check. 
 *              There is no stack overflow check.
 *              The implementation assumes only two simple user processes and 
 *              NO HARDWARE INTERRUPTS. 
 *              The purpose is to show one way of doing context switch
 *              under stated assumptions. 
 *              These assumptions are not true in the required RTX Project!!!
 *              If you decide to use this piece of code, 
 *              you need to understand the assumptions and the limitations.
 *
 *****************************************************************************/

#include "k_process.h"
#include "k_rtx.h"
/*
 *==========================================================================
 *                            GLOBAL VARIABLES
 *==========================================================================
 */
 
PCB **gp_pcbs;                  /* array of pcbs */
PCB *gp_current_process = NULL; /* always point to the current RUN process */

/* process initialization table */
PROC_INIT g_proc_table[NUM_TEST_PROCS];

/**************************************************************************//**
 * @biref initialize all processes in the system
 * @note  We assume there are only two user processes in the system in 
 *        this example starter code.
 *        The preprocessor ECE350_DEMO should be defined
 *****************************************************************************/
void process_init(PROC_INIT *proc_info, int num) 
{
	int i;
	U32 *sp;
  
    /* fill out the initialization table */
#ifdef SE350_DEMO
	for ( i = 0; i < 2; i++ ) {
#else
    for ( i = 0; i < num; i++ ) {
#endif /* SE350_DEMO */
		g_proc_table[i].m_pid        = proc_info[i].m_pid;
		g_proc_table[i].m_stack_size = proc_info[i].m_stack_size;
		g_proc_table[i].mpf_start_pc = proc_info[i].mpf_start_pc;
	}
  
	/* initilize exception stack frame (i.e. initial context) for each process */
	for ( i = 0; i < num; i++ ) {
		int j;
		(gp_pcbs[i])->m_pid = (g_proc_table[i]).m_pid;
		(gp_pcbs[i])->m_state = NEW;
		
		sp = alloc_stack((g_proc_table[i]).m_stack_size);
		*(--sp)  = INITIAL_xPSR;      // user process initial xPSR  
		*(--sp)  = (U32)((g_proc_table[i]).mpf_start_pc); // PC contains the entry point of the process
		for ( j = 0; j < 6; j++ ) { // R0-R3, R12 are cleared with 0
			*(--sp) = 0x0;
		}
		(gp_pcbs[i])->mp_sp = sp;
	}
}

/**************************************************************************//**
 * @brief   scheduler, pick the pid of the next to run process
 * @return  PCB pointer of the next to run process
 *          NULL if error happens
 * @post    if gp_current_process was NULL, then it gets set to pcbs[0].
 *          No other effect on other global variables.
 *****************************************************************************/

PCB *scheduler(void)
{
	if (gp_current_process == NULL) {
		gp_current_process = gp_pcbs[0]; 
		return gp_pcbs[0];
	}

	if ( gp_current_process == gp_pcbs[0] ) {
		return gp_pcbs[1];
	} else if ( gp_current_process == gp_pcbs[1] ) {
		return gp_pcbs[0];
	} else {
		return NULL;
	}
}

/**************************************************************************//**
 * @brief   switch out old pcb (p_pcb_old), run the new pcb (gp_current_process)
 * @param   p_pcb_old, the old pcb that was in RUN
 * @return  RTX_OK upon success
 *          RTX_ERR upon failure
 * @pre     p_pcb_old and gp_current_process are pointing to valid PCBs.
 * @post    if gp_current_process was NULL, then it gets set to pcbs[0].
 *          No other effect on other global variables.
 *****************************************************************************/

int process_switch(PCB *p_pcb_old) 
{
	PROC_STATE_E state;
	
	state = gp_current_process->m_state;

	if (state == NEW) {
		if (gp_current_process != p_pcb_old && p_pcb_old->m_state != NEW) {
			p_pcb_old->m_state = RDY;
			p_pcb_old->mp_sp = (U32 *) __get_MSP();
		}
		gp_current_process->m_state = RUN;
        
        U32 ctrl = __get_CONTROL();
        ctrl &= ~BIT(0);    // clear bit 0, want to be at priv. level when exit from the kernel
        __set_CONTROL(ctrl);
		__set_MSP((U32) gp_current_process->mp_sp);
		__rte();  // pop exception stack frame from the stack for a new processes
	} 
	
	/* The following will only execute if the if block above is FALSE */

	if (gp_current_process != p_pcb_old) {
		if (state == RDY){ 		
			p_pcb_old->m_state = RDY; 
			p_pcb_old->mp_sp = (U32 *) __get_MSP(); // save the old process's sp
			gp_current_process->m_state = RUN;
			__set_MSP((U32) gp_current_process->mp_sp); //switch to the new proc's stack    
		} else {
			gp_current_process = p_pcb_old; // revert back to the old proc on error
			return RTX_ERR;
		} 
	}
	return RTX_OK;
}

/**************************************************************************//**
 * @brief   release_processor(). 
 * @return  RTX_ERR on error and zero on success
 * @post    gp_current_process gets updated to next to run process
 *****************************************************************************/

int k_release_processor(void)
{
	PCB *p_pcb_old = NULL;
	
	p_pcb_old = gp_current_process;
	gp_current_process = scheduler();
	
	if ( gp_current_process == NULL  ) {
		gp_current_process = p_pcb_old; // revert back to the old process
		return RTX_ERR;
	}
        if ( p_pcb_old == NULL ) {
		p_pcb_old = gp_current_process;
	}
	process_switch(p_pcb_old);
	return RTX_OK;
}

int k_get_process_priority(int pid)
{
    return RTX_ERR;
}

int k_set_process_priority(int pid, int prio_new) 
{
    return RTX_ERR;
}

/*
 *===========================================================================
 *                             END OF FILE
 *===========================================================================
 */
