/*
 ****************************************************************************
 *
 *                  UNIVERSITY OF WATERLOO SE 350 RTX LAB  
 *
 *  Copyright 2020-2022 NXP Semiconductors, Thomas Reidemeister and Yiqing Huang
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
 * @file        timer.c
 * @brief       timer.c - Timer example code. Tiemr IRQ is invoked every 1ms 
 *              
 * @version     V1.2022.01
 * @authors     NXP Semiconductors, Thomas Reidemeister and Yiqing Huang
 * @date        2022 JAN
 *****************************************************************************/

#include <LPC17xx.h>
#include "timer.h"

#define BIT(X) (1<<X)

volatile uint32_t g_timer_count = 0; // increment every 1 ms

/**
 * @brief: initialize timer. Only timer 0 is supported
 */
uint32_t timer_init(uint8_t n_timer) 
{
    LPC_TIM_TypeDef *pTimer;
    if (n_timer == 0) {
        /*
        Steps 1 & 2: system control configuration.
        Under CMSIS, system_LPC17xx.c does these two steps
        
        ----------------------------------------------------- 
        Step 1: Power control configuration.
                See table 46 pg63 in LPC17xx_UM
        -----------------------------------------------------
        Enable UART0 power, this is the default setting
        done in system_LPC17xx.c under CMSIS.
        Enclose the code for your refrence
        //LPC_SC->PCONP |= BIT(1);
    
        -----------------------------------------------------
        Step2: Select the clock source, 
               default PCLK=CCLK/4 , where CCLK = 100MHZ.
               See tables 40 & 42 on pg56-57 in LPC17xx_UM.
        -----------------------------------------------------
        Check the PLL0 configuration to see how XTAL=12.0MHZ 
        gets to CCLK=100MHZ in system_LPC17xx.c file.
        PCLK = CCLK/4, default setting in system_LPC17xx.c.
        Enclose the code for your reference
        //LPC_SC->PCLKSEL0 &= ~(BIT(3)|BIT(2));    

        -----------------------------------------------------
        Step 3: Pin Ctrl Block configuration. 
                Optional, not used in this example
                See Table 82 on pg110 in LPC17xx_UM 
        -----------------------------------------------------
        */
        pTimer = (LPC_TIM_TypeDef *) LPC_TIM0;

    } else { /* other timer not supported yet */
        return 1;
    }

    /*
    -----------------------------------------------------
    Step 4: Interrupts configuration
    -----------------------------------------------------
    */

    /* Step 4.1: Prescale Register PR setting 
       CCLK = 100 MHZ, PCLK = CCLK/4 = 25 MHZ
       2*(12499 + 1)*(1/25) * 10^(-6) s = 10^(-3) s = 1 ms
       TC (Timer Counter) toggles b/w 0 and 1 every 12500 PCLKs
       see MR setting below 
    */
    pTimer->PR = 12499;  

    /* Step 4.2: MR setting, see section 21.6.7 on pg496 of LPC17xx_UM. */
    pTimer->MR0 = 1;

    /* Step 4.3: MCR setting, see table 429 on pg496 of LPC17xx_UM.
       Interrupt on MR0: when MR0 mathches the value in the TC, 
                         generate an interrupt.
       Reset on MR0: Reset TC if MR0 mathches it.
    */
    pTimer->MCR = BIT(0) | BIT(1);

    g_timer_count = 0;

    /* Step 4.4 Timer0 IRQ priority setting */
    NVIC_SetPriority(TIMER0_IRQn, 0x08);
    
    /* Step 4.5: CSMSIS enable timer0 IRQ */
    NVIC_EnableIRQ(TIMER0_IRQn);

    /* Step 4.6: Enable the TCR. See table 427 on pg494 of LPC17xx_UM. */
    pTimer->TCR = 1;

    return 0;
}

/**
 * @brief: use CMSIS ISR for TIMER0 IRQ Handler
 * NOTE: This example shows how to save/restore all registers rather than just
 *       those backed up by the exception stack frame. We add extra
 *       push and pop instructions in the assembly routine. 
 *       The actual c_TIMER0_IRQHandler does the rest of irq handling
 */
__asm void TIMER0_IRQHandler(void)
{
    PRESERVE8
    IMPORT c_TIMER0_IRQHandler
    CPSID I              // disable interrupt
    PUSH{r4-r11, lr}
    BL c_TIMER0_IRQHandler
    CPSIE I              // enable interrupt
    POP{r4-r11, pc}
} 
/**
 * @brief: c TIMER0 IRQ Handler
 */
void c_TIMER0_IRQHandler(void)
{
    /* ack inttrupt, see section  21.6.1 on pg 493 of LPC17XX_UM */
    LPC_TIM0->IR = BIT(0);  
    
    g_timer_count++ ;
}

/*
 *===========================================================================
 *                             END OF FILE
 *===========================================================================
 */
