/**************************************************************************/
/*                                                                        */
/*       Copyright (c) Microsoft Corporation. All rights reserved.        */
/*                                                                        */
/*       This software is licensed under the Microsoft Software License   */
/*       Terms for Microsoft Azure RTOS. Full text of the license can be  */
/*       found in the LICENSE file at https://aka.ms/AzureRTOS_EULA       */
/*       and in the root directory of this software.                      */
/*                                                                        */
/**************************************************************************/


/**************************************************************************/
/**************************************************************************/
/**                                                                       */
/** GUIX Component                                                        */
/**                                                                       */
/**   Port Specific                                                       */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/
/**************************************************************************/
/*                                                                        */
/*  APPLICATION INTERFACE DEFINITION                       RELEASE        */
/*                                                                        */
/*    gx_port.h                                         Cortex-M7/AC5     */
/*                                                          6.1.8         */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This file contains data type definitions and constants specific     */
/*    to the implementation of high-performance GUIX UI framework.        */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  09-30-2020     Kenneth Maxwell          Initial Version 6.1           */
/*  08-02-2021     Kenneth Maxwell          Modified comment(s), removed  */
/*                                            GX_SYSTEM_TIMER_TICKS and   */
/*                                            GX_TICKS_SECOND definitions,*/
/*                                            resulting in version 6.1.8  */
/*                                                                        */
/**************************************************************************/

#ifndef GX_PORT_H
#define GX_PORT_H

/* Determine if the optional GUIX user define file should be used.  */

#ifdef GX_INCLUDE_USER_DEFINE_FILE

/* Include the user defines in gx_user.h. The defines in this file may
   alternately be defined on the command line.  */

#include "gx_user.h"
#endif

typedef INT    GX_BOOL;

typedef SHORT  GX_VALUE;

#define GX_VALUE_MAX                        0x7FFF


/* Define the basic system parameters.  */

#ifndef GX_THREAD_STACK_SIZE
#define GX_THREAD_STACK_SIZE                4096
#endif

#define GX_CONST                            const

#define GX_INCLUDE_DEFAULT_COLORS

#define GX_MAX_ACTIVE_TIMERS                32

#define GX_MAX_VIEWS                        32

#define GX_MAX_DISPLAY_HEIGHT               800


/* Define several macros for the error checking shell in GUIX.  */

#ifndef TX_TIMER_PROCESS_IN_ISR

#define GX_CALLER_CHECKING_EXTERNS          extern  TX_THREAD           *_tx_thread_current_ptr; \
                                            extern  TX_THREAD           _tx_timer_thread; \
                                            extern  volatile ULONG      _tx_thread_system_state;

#define GX_THREADS_ONLY_CALLER_CHECKING     if ((TX_THREAD_GET_SYSTEM_STATE()) || \
                                                (_tx_thread_current_ptr == TX_NULL) || \
                                                (_tx_thread_current_ptr == &_tx_timer_thread)) \
                                                return(GX_CALLER_ERROR);

#define GX_INIT_AND_THREADS_CALLER_CHECKING if (((TX_THREAD_GET_SYSTEM_STATE()) && ((TX_THREAD_GET_SYSTEM_STATE()) < ((ULONG) 0xF0F0F0F0))) || \
                                                (_tx_thread_current_ptr == &_tx_timer_thread)) \
                                                return(GX_CALLER_ERROR);


#define GX_NOT_ISR_CALLER_CHECKING          if ((TX_THREAD_GET_SYSTEM_STATE()) && ((TX_THREAD_GET_SYSTEM_STATE()) < ((ULONG) 0xF0F0F0F0))) \
                                                return(GX_CALLER_ERROR);

#define GX_THREAD_WAIT_CALLER_CHECKING      if ((wait_option) && \
                                               ((_tx_thread_current_ptr == NX_NULL) || (TX_THREAD_GET_SYSTEM_STATE()) || (_tx_thread_current_ptr == &_tx_timer_thread))) \
                                            return(GX_CALLER_ERROR);


#else



#define GX_CALLER_CHECKING_EXTERNS          extern  TX_THREAD           *_tx_thread_current_ptr; \
                                            extern  volatile ULONG      _tx_thread_system_state;

#define GX_THREADS_ONLY_CALLER_CHECKING     if ((TX_THREAD_GET_SYSTEM_STATE()) || \
                                                (_tx_thread_current_ptr == TX_NULL)) \
                                                return(GX_CALLER_ERROR);

#define GX_INIT_AND_THREADS_CALLER_CHECKING if (((TX_THREAD_GET_SYSTEM_STATE()) && ((TX_THREAD_GET_SYSTEM_STATE()) < ((ULONG) 0xF0F0F0F0)))) \
                                                return(GX_CALLER_ERROR);

#define GX_NOT_ISR_CALLER_CHECKING          if ((TX_THREAD_GET_SYSTEM_STATE()) && ((TX_THREAD_GET_SYSTEM_STATE()) < ((ULONG) 0xF0F0F0F0))) \
                                                return(GX_CALLER_ERROR);

#define GX_THREAD_WAIT_CALLER_CHECKING      if ((wait_option) && \
                                               ((_tx_thread_current_ptr == NX_NULL) || (TX_THREAD_GET_SYSTEM_STATE()))) \
                                            return(GX_CALLER_ERROR);

#endif


/* Define the version ID of GUIX.  This may be utilized by the application.  */

#ifdef GX_SYSTEM_INIT
CHAR _gx_version_id[] =
    "Copyright (c) Microsoft Corporation. All rights reserved.  *  GUIX Cortex-M7/AC5 Version 6.4.0 *";
#else
extern  CHAR _gx_version_id[];
#endif

#endif

