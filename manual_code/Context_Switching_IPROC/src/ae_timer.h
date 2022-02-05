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
 * @brief       AE Timer header file              
 * @version     V1.2021.03
 * @authors     Yiqing Huang
 * @date        2021 MAR
 *****************************************************************************/

#ifndef _AE_TIMER_H_
#define _AE_TIMER_H_
#include <LPC17xx.h>

/*
 *===========================================================================
 *                             MACROS
 *===========================================================================
 */
 
#define BIT(X) (1U << (X))
#define AE_TIMER2  2

/*
 *===========================================================================
 *                             STRUCTURES
 *===========================================================================
 */
 

/* Timer Counter Struct */
struct ae_tick {
    uint32_t tc;        /**< seconds              */
    uint32_t pc;        /**< tens of nano seconds */
};

/**
 * @brief   data strucure to represent time in seconds and nano seconds
 * @note    Example: to represent 3.004 seconds, set
 *          sec = 3 and nsec = 4*10^6
 */
struct ae_time {
    uint32_t sec;       /**< seconds             */
    uint32_t nsec;      /**< nano seconds        */
};

/*
 *===========================================================================
 *                            FUNCTION PROTOTYPES
 *===========================================================================
 */

extern uint32_t ae_timer_init_100MHZ(uint8_t n_timer);
extern int ae_get_tick(struct ae_tick *tm, uint8_t n_timer);
extern int ae_get_tick_diff(struct ae_time *tm, struct ae_tick *tk1, struct ae_tick *tk2);
#endif /* ! _AE_TIMER_H_ */

/*
 *===========================================================================
 *                             END OF FILE
 *===========================================================================
 */

