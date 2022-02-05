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
 * @file        HAL.c
 * @brief       Hardware Abstraction Layer 
 *              
 * @version     V1.2021.02
 * @authors     Yiqing Huang
 * @date        2021 FEB
 * @note        This file contains embedded assembly. 
 *              The code borrowed some ideas from ARM RL-RTX source code
 *
 *****************************************************************************/


/* pop off exception stack frame from the stack */
__asm void __rte(void)
{
    PRESERVE8                       // 8 bytes alignement of the stack
    MVN     LR, #:NOT:0xFFFFFFF9    // set EXC_RETURN value, Thread mode, MSP
    CPSIE   I                       // enable interrupt
    BX      LR
}


/**************************************************************************//**
 * @brief   	SVC Handler
 * @note        assuming MSP is used in thread mode
 *              requires modification if PSP is used in thread mode
 *****************************************************************************/
__asm void SVC_Handler (void) 
{
    PRESERVE8                       // 8 bytes alignement of the stack
    CPSID   I                       // disable interrupt
    MRS     R0, MSP                 // Read MSP
	
  
    LDR     R1, [R0, #24]           // Read Saved PC from SP
                                    // Loads R1 from a word 24 bytes above the address in R0
                                    // Note that R0 now contains the the SP value after the
                                    // exception stack frame is pushed onto the stack.
             
    LDRH    R1, [R1, #-2]           // Load halfword because SVC number is encoded there
    BICS    R1, R1, #0xFF00         // Extract SVC Number and save it in R1.  
                                    // R1 <= R1 & ~(0xFF00), update flags
                   
    BNE     SVC_EXIT                // if SVC Number !=0, exit
 
    LDM     R0, {R0-R3, R12}        // Read R0-R3, R12 from stack. 
                                    // NOTE R0 contains the sp before this instruction

    PUSH    {R4-R11, LR}            // Save other registers for preemption caused by i-procs
    BLX     R12                     // Call SVC C Function, 
                                    // R12 contains the corresponding 
                                    // C kernel functions entry point
                                    // R0-R3 contains the kernel function input parameter (See AAPCS)
    POP     {R4-R11, LR}            // Restore other registers for preemption caused by i-procs
    MRS     R12, MSP                // Read MSP
    STR     R0, [R12]               // store C kernel function return value in R0
                                    // to R0 on the exception stack frame  
SVC_EXIT  
	
    //MVN     LR, #:NOT:0xFFFFFFF9    // set EXC_RETURN value, Thread mode, MSP
                                    // The above line can be commented out.
                                    // Because we back up LR, we do not need to explicitly set the mode and stack
                                    // we can just use the interrupted process's original mode and stack settting
                                    // which is saved in LR by the processor
    CPSIE   I                       // enable interrupt
    BX      LR
}
