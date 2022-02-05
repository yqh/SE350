 /*
 ****************************************************************************
 *
 *                  UNIVERSITY OF WATERLOO SE 350 RTX LAB  
 *
 *         Copyright 2020-2022 Yiqing Huang and NXP Semiconductors
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
 * @brief       ae_timer.c - Timer2 as a free running counter                     
 * @version     V1.2021.03
 * @date        2021 MAR
 *****************************************************************************/

#include "ae_timer.h"
#include "common.h"
 

/**************************************************************************//**
 * @brief       Setting up Timer2 as a free-run counter. No interrupt is fired.
 *              The timer peripheral clock speed is set to the cpu clock speed.
 *              The prescale counter increments every 10 nano seconds.
 * @param       uint8_t n_timer: 2 for timer2, other timers are not supported.
 * @return      0 on success and non-zero on failure
 *              The timer couner increments every 1 second.
 * @details     The timer has two counters. One is PC, the prescale counter;
 *              the other is TC, the timer counter.
 *              The PC incrementes every PCLCK (peripheral clock) cycle.
 *              When PC reaches the pre-set value in PR (prescale register),
 *              the TC increments by one and the PC gets re-set to zero.
 *              When TC reaches the pre-set value in MCR0, the match register, 
 *              it resets to zero.
 *              The Timer2 PCLCK is set to CCLCK (100 MHZ cpu clock).
 *              The PR is set to (100000000 - 1) so that every 1 second 
 *              the TC is incremetned by one.
 *****************************************************************************/

uint32_t ae_timer_init_100MHZ(uint8_t n_timer) 
{
    LPC_TIM_TypeDef *pTimer;
    if (n_timer == 2) {
        pTimer = (LPC_TIM_TypeDef *) LPC_TIM2;
    } else { /* other timer not supported yet */
        return 1;
    }

   /*------------------------------------------------------------------------*
    * Power setting of TIMER2 
    * See Table 46 on page 63 of LPC17xx user's manual
    *------------------------------------------------------------------------*/
    LPC_SC->PCONP |= BIT(22);
    
   /*------------------------------------------------------------------------*
    * PCLK = CCLK, can also be set in system_LPC17xx.c by using configuratio wizard 
    * We set explicitly here to overwrite the system_LPC17xx.c configuration so that
    * we do not need to modify student's system_LPC17xx.c file
    * See Tables 41 and 42 on page 57 of LPC17xx user's manual
    *------------------------------------------------------------------------*/
    
    LPC_SC->PCLKSEL1 |= BIT(12);
    LPC_SC->PCLKSEL1 &= ~BIT(13);
    
   /*------------------------------------------------------------------------*
    * Step 1. Prescale Register PR setting 
    * CCLK = 100 MHZ, PCLK = CCLK
    * Prescale counter increments every PCLK tick: (1/100)* 10^(-6) s = 10 ns
    * TC increments every (PR + 1) PCLK cycles
    * TC increments every (MR0 + 1) * (PR + 1) PCLK cycles 
    *------------------------------------------------------------------------*/
     
    pTimer->PR = 100000000 - 1; /* increment timer counter every 1*10^9 PCLK ticks, which is 1 sec */ 

   /*------------------------------------------------------------------------* 
    * Step 2: MR setting, see section 21.6.7 on pg496 of LPC17xx_UM.
    * Effectively, using timer2 as a counter to measure time, 
    * there is no overflow in TC in about 100 years 
    *------------------------------------------------------------------------*/
    pTimer->MR0 = 0xFFFFFFFF;  

   /*------------------------------------------------------------------------*  
    * Step 3: MCR setting, see table 429 on pg496 of LPC17xx_UM.
    * Reset on MR0: Reset TC if MR0 mathches it. No interrupt triggered on match.
    *------------------------------------------------------------------------*/
    pTimer->MCR = BIT(1);
   /*------------------------------------------------------------------------* 
    * Step 4: Enable the counter. See table 427 on pg494 of LPC17xx_UM.
    *------------------------------------------------------------------------*/
    
    pTimer->TCR = 1;

    return 0;
}

/**************************************************************************//**
 * @brief   	obtain the current PC and TC readings
 *          
 * @return      0 on success and non-zero on failure
 * @param[out]  struct ae_tick *tk, structure to return PC and TC readings
 * @param[in]   uinit8_t n_timer, the timer index, 2 for timer2
 * @attention   tk memory should be allocated by the caller.
 *****************************************************************************/

int ae_get_tick(struct ae_tick *tk, uint8_t n_timer) 
{
    LPC_TIM_TypeDef *pTimer;
    if (n_timer == 2) {
        pTimer = (LPC_TIM_TypeDef *) LPC_TIM2;
    } else { /* other timer not supported yet */
        return 1;
    }
    
    tk->tc = pTimer->TC;
    tk->pc = pTimer->PC;
    
    return 0;
}

/**************************************************************************//**
 * @brief   	compute difference of two time stamps
 *          
 * @return      0 on success and non-zero on failure
 * @param[out]  struct ae_time *tm, structure to return time in seconds + nano seconds
 * @param[in]   struct ae_tick *tk1, the first time stamp
 * @param[in]   struct ae_tick *tk1, the second time stamp
 * @attention   tm, tk1, and tk2 memory should be allocated by the caller.
 *****************************************************************************/
int ae_get_tick_diff(struct ae_time *tm, struct ae_tick *tk1, struct ae_tick *tk2)
{   
    if (tk1 == NULL || tk2 == NULL || tm == NULL) {
        return -1;
    }
    
    int diff_pc = tk2->pc - tk1->pc;
    int diff_tc = tk2->tc - tk1->tc;
   
    
    if ( diff_tc < 0 ) {    
        diff_pc = - diff_pc;
        diff_tc = - diff_tc;
    } else if ( diff_tc == 0  && diff_pc < 0 ) {
        diff_pc = - diff_pc;
    }
   
    
    if ( diff_pc < 0 ) {
        tm->sec  = diff_tc - 1;
        tm->nsec = 10 * (diff_pc + 100000000);         
    } else {
        tm->sec  = diff_tc;
        tm->nsec = 10 * diff_pc;
    }
    return 0;
}
/*
 *===========================================================================
 *                             END OF FILE
 *===========================================================================
 */
