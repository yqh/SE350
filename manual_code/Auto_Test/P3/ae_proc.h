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
 * @file        ae_proc.h
 * @brief       six user test processes header file 
 *              
 * @version     V1.2021.01
 * @authors     Yiqing Huang
 * @date        2021 JAN
 *****************************************************************************/
 
#ifndef AE_PROC_H_
#define AE_PROC_H_

#include "ae.h"

/*
 *===========================================================================
 *                             MACROS
 *===========================================================================
 */
#ifdef SIM_TARGET       /* using the simulator is slow */
#define DELAY 100000
#else
#define DELAY 10000000
#endif /* SIM_TARGET */

/*
 *===========================================================================
 *                             MACROS
 *===========================================================================
 */
#define AE_REQ_100          200
#define AE_REQ_101          201

#define AE_RST_100          (AE_REQ_100 + 100)
#define AE_RST_101          (AE_REQ_101 + 100)  

#define AE_1M_NSEC          1000000     /* 10^6 nsec = 1   ms  */  
#define AE_10M_NSEC         10000000    /* 10^7 nsec = 10  ms  */
#define AE_100M_NSEC        100000000   /* 10^8 nsec = 100 ms  */
#define AE_1B_NSEC          1000000000  /* 10^9 nsec = 1   sec */

/*
 *===========================================================================
 *                             STRUCTURES
 *===========================================================================
 */
  
typedef  struct ae_msg_req {
    U32     req_no;             /*> req number              */
    struct  ae_tick tk;         /*> timer tick              */
    int     delay;              /*> delay in milliseconds   */ 
    char    bytes[1];           /*> bytes                   */
} AE_MSG_REQ;

/*
 *===========================================================================
 *                            FUNCTION PROTOTYPES
 *===========================================================================
 */
extern void set_test_procs(PROC_INIT *procs, int num);
extern void proc1(void);
extern void proc2(void);
extern void proc3(void);
extern void proc4(void);
extern void proc5(void);
extern void proc6(void);

#endif /* AE_PROC_H_ */
