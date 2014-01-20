/* @brief: HAL.c Hardware Abstraction Layer
 * @author: Yiqing Huang
 * @date: 2014/01/17
 * NOTE: This file contains embedded assembly. 
 *       The code borrowed some ideas from ARM RL-RTX source code
 */
 
/* pop off exception stack frame from the stack */
__asm void __rte(void)
{
  PRESERVE8            ; 8 bytes alignement of the stack
  MVN  LR, #:NOT:0xFFFFFFF9  ; set EXC_RETURN value, Thread mode, MSP
  BX   LR
}

/* NOTE: assuming MSP is used. Ideally, PSP should be used */
__asm void SVC_Handler (void) 
{
  PRESERVE8            ; 8 bytes alignement of the stack
  MRS  R0, MSP         ; Read MSP
	
  
  LDR  R1, [R0, #24]   ; Read Saved PC from SP
                       ; Loads R1 from a word 24 bytes above the address in R0
                       ; Note that R0 now contains the the SP value after the
                       ; exception stack frame is pushed onto the stack.
             
  LDRH R1, [R1, #-2]   ; Load halfword because SVC number is encoded there
  BICS R1, R1, #0xFF00 ; Extract SVC Number and save it in R1.  
                       ; R1 <= R1 & ~(0xFF00), update flags
                   
  BNE  SVC_EXIT        ; if SVC Number !=0, exit
 
  LDM  R0, {R0-R3, R12}; Read R0-R3, R12 from stack. 
                       ; NOTE R0 contains the sp before this instruction

  PUSH {R4-R11, LR}    ; Save other registers for preemption caused by i-procs
  BLX  R12             ; Call SVC C Function, 
                       ; R12 contains the corresponding 
                       ; C kernel functions entry point
                       ; R0-R3 contains the kernel function input parameter (See AAPCS)
  POP {R4-R11, LR}     ; Restore other registers for preemption caused by i-procs
  MRS  R12, MSP        ; Read MSP
  STR  R0, [R12]       ; store C kernel function return value in R0
                       ; to R0 on the exception stack frame  
SVC_EXIT  
	
  MVN  LR, #:NOT:0xFFFFFFF9  ; set EXC_RETURN value, Thread mode, MSP
  BX   LR
}
