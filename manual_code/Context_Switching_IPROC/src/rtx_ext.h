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
 * @file        rtx_ext.h 
 * @brief       RTX extended user API
 *              
 * @version     V1.2022.01
 * @authors     Yiqing Huang
 * @date        2022 JAN
 * 
 * @note        self-defined new RTX user API are declared in this file
 * @see         rtx_ext.h
 * @see         common.h
 *****************************************************************************/
 #ifndef RTX_EXT_H_
 #define RTX_EXT_H_
 
 #include "common.h"
 
 /*
 *===========================================================================
 *                             MACROS
 *===========================================================================
 */

#define __SVC_0  __svc_indirect(0)
 /*
 *===========================================================================
 *                            FUNCTION PROTOTYPES
 *===========================================================================
 */
 /* Memeory Management */
extern void *k_request_memory_block_nb(void);
#define request_memory_block_nb() _request_memory_block_nb((U32)k_request_memory_block_nb)
extern void *_request_memory_block_nb(U32 p_func) __SVC_0;

#endif // !RTX_EXT_H_

/*
 *===========================================================================
 *                             END OF FILE
 *===========================================================================
 */

