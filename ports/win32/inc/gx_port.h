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
/*    gx_port.h                                          Win32/Visual     */
/*                                                           6.1.10       */
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
/*  12-31-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            added new defines,          */
/*                                            resulting in version 6.1.3  */
/*  04-02-2021     Kenneth Maxwell          Modified comment(s),          */
/*                                            added new defines,          */
/*                                            resulting in version 6.1.6  */
/*  06-02-2021     Kenneth Maxwell          Modified comment(s),          */
/*                                            defined GX_DISABLE_THREADX_ */
/*                                            TIMER_SOURCE,               */
/*                                            resulting in version 6.1.7  */
/*  08-02-2021     Kenneth Maxwell          Modified comment(s), removed  */
/*                                            GX_SYSTEM_TIMER_TICKS and   */
/*                                            GX_TICKS_SECOND definitions,*/
/*                                            resulting in version 6.1.8  */
/*  01-31-2022     Ting Zhu                 Modified comment(s), modified */
/*                                            event thread create logic,  */
/*                                            resulting in version 6.1.10 */
/*                                                                        */
/**************************************************************************/

#ifndef GX_PORT_H
#define GX_PORT_H

/* define target for build */
#define GX_TARGET_WIN32

/* Determine if the optional GUIX user define file should be used.  */

#ifdef GX_INCLUDE_USER_DEFINE_FILE

/* Include the user defines in gx_user.h. The defines in this file may
   alternately be defined on the command line.  */

#include "gx_user.h"
#endif

typedef INT    GX_BOOL;

typedef SHORT  GX_VALUE;

/* Disable ThreadX timer.  */
#define GX_DISABLE_THREADX_TIMER_SOURCE

#define GX_VALUE_MAX                        0x7FFF

/* For the win32 port, the entry point is WinMain, which is defined
   in the win32 driver file.  The entry point for GUIX demo is gx_main(). */

#define main(a, b)                          gx_main(a, b)

#define GX_WIDGET_USER_DATA

/* Define Win32 event thread create. */
#define GX_WIN32_EVENT_THREAD_CREATE(driver_data, thread_name) driver_data->win32_driver_thread_handle = CreateThread(NULL, GX_WIN32_STACK_SIZE, (LPTHREAD_START_ROUTINE)gx_win32_driver_thread_entry, driver_data, 0, 0);

/* Define Win32 event thread exit. */
#define GX_WIN32_EVENT_THREAD_EXIT(code)                       exit(code)

#define GX_WIN32_DISPLAY_DRIVER_EXTRA_MEMBERS_DECLEARE

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

#define GX_THREADS_ONLY_CALLER_CHECKING     if ((_tx_thread_system_state) || \
                                                (_tx_thread_current_ptr == TX_NULL) || \
                                                (_tx_thread_current_ptr == &_tx_timer_thread)) \
                                                return(GX_CALLER_ERROR);

#define GX_INIT_AND_THREADS_CALLER_CHECKING if (((_tx_thread_system_state) && (_tx_thread_system_state < ((ULONG) 0xF0F0F0F0))) || \
                                                (_tx_thread_current_ptr == &_tx_timer_thread)) \
                                                return(GX_CALLER_ERROR);


#define GX_NOT_ISR_CALLER_CHECKING          if ((_tx_thread_system_state) && (_tx_thread_system_state < ((ULONG) 0xF0F0F0F0))) \
                                                return(GX_CALLER_ERROR);

#define GX_THREAD_WAIT_CALLER_CHECKING      if ((wait_option) && \
                                               ((_tx_thread_current_ptr == NX_NULL) || (_tx_thread_system_state) || (_tx_thread_current_ptr == &_tx_timer_thread))) \
                                            return(GX_CALLER_ERROR);


#else



#define GX_CALLER_CHECKING_EXTERNS          extern  TX_THREAD           *_tx_thread_current_ptr; \
                                            extern  volatile ULONG      _tx_thread_system_state;

#define GX_THREADS_ONLY_CALLER_CHECKING     if ((_tx_thread_system_state) || \
                                                (_tx_thread_current_ptr == TX_NULL)) \
                                                return(GX_CALLER_ERROR);

#define GX_INIT_AND_THREADS_CALLER_CHECKING if (((_tx_thread_system_state) && (_tx_thread_system_state < ((ULONG) 0xF0F0F0F0)))) \
                                                return(GX_CALLER_ERROR);

#define GX_NOT_ISR_CALLER_CHECKING          if ((_tx_thread_system_state) && (_tx_thread_system_state < ((ULONG) 0xF0F0F0F0))) \
                                                return(GX_CALLER_ERROR);

#define GX_THREAD_WAIT_CALLER_CHECKING      if ((wait_option) && \
                                               ((_tx_thread_current_ptr == NX_NULL) || (_tx_thread_system_state))) \
                                            return(GX_CALLER_ERROR);

#endif


/* Define the version ID of GUIX.  This may be utilized by the application.  */

#ifdef GX_SYSTEM_INIT
CHAR _gx_version_id[] =
    "Copyright (c) Microsoft Corporation. All rights reserved.  *  GUIX Win32/Visual Version 6.1.10 *";
#else
extern  CHAR _gx_version_id[];
#endif

#endif

