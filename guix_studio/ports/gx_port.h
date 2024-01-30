/***************************************************************************
 * Copyright (c) 2024 Microsoft Corporation 
 * 
 * This program and the accompanying materials are made available under the
 * terms of the MIT License which is available at
 * https://opensource.org/licenses/MIT.
 * 
 * SPDX-License-Identifier: MIT
 **************************************************************************/


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
/*    gx_api.h                                           Win32/Visual     */
/*                                                           6.0          */
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
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
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


typedef INT           GX_BOOL;

#if defined(GUIX_STUDIO)

/* Define Win32 event thread create. */
#define GX_WIN32_EVENT_THREAD_CREATE(driver_data, thread_name)

/* Define Win32 event thread exit. */
#define GX_WIN32_EVENT_THREAD_EXIT(code)                       ExitThread(code)

#define GX_WIN32_DISPLAY_DRIVER_EXTRA_MEMBERS_DECLEARE \
    GX_DISPLAY     *win32_display;                     \
    GX_CANVAS      *win32_canvas;                      \
    GX_WINDOW_ROOT *win32_root_win;                    \
    GX_WIDGET     **win32_screens;                     \
    INT             win32_screen_count;                \
    INT             win32_display_index;               \
    LONG            win32_window_xpos;                 \
    LONG            win32_window_ypos;                 \
    LONG            win32_guix_ThreadId;               \
    LONG            win32_window_ThreadId;

                             
typedef SHORT  GX_VALUE;
#define GX_UTF8_SUPPORT
#define GX_EXTENDED_UNICODE_SUPPORT
#define GX_SYNERGY_FONT_FORMAT_SUPPORT
#define GX_DISABLE_DEPRECATED_STRING_API

#ifndef GX_FONT_KERNING_SUPPORT
#define GX_FONT_KERNING_SUPPORT
#endif

#ifndef GX_DYNAMIC_BIDI_TEXT_SUPPORT
#define GX_DYNAMIC_BIDI_TEXT_SUPPORT
#endif

#ifndef GX_DYNAMIC_ARABIC_SHAPING_SUPPORT
#define GX_DYNAMIC_ARABIC_SHAPING_SUPPORT
#endif

#ifndef GX_THAI_GLYPH_SHAPING_SUPPORT
#define GX_THAI_GLYPH_SHAPING_SUPPORT
#endif

#else
typedef SHORT  GX_VALUE;
#endif

#define GX_VALUE_MAX   0x7FFF


/* For the win32 port, the entry point is WinMain, which is defined 
   in the win32 driver file.  The entry point for GUIX demo is gx_main(). */
#define main(a, b)                          gx_main(a, b)

#define GX_WIDGET_USER_DATA

/* Define the basic system parameters.  */

#ifndef GX_THREAD_STACK_SIZE
#define GX_THREAD_STACK_SIZE                4096
#endif

#ifndef GX_TICKS_SECOND
#define GX_TICKS_SECOND                     50
#endif


#define GX_CONST                            const

#define GX_MAX_ACTIVE_TIMERS                32

#define GX_MAX_VIEWS                        32

#define GX_MAX_DISPLAY_HEIGHT               800

/* Define several macros for the error checking shell in GUIX.  */

#ifndef GX_DISABLE_THREADX_BINDING

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

#else
#define GX_CALLER_CHECKING_EXTERNS
#define GX_THREADS_ONLY_CALLER_CHECKING
#define GX_INIT_AND_THREADS_CALLER_CHECKING
#define GX_NOT_ISR_CALLER_CHECKING
#define GX_THREAD_WAIT_CALLER_CHECKING
#endif

/* Define the version ID of GUIX.  This may be utilized by the application.  */

#ifdef GX_SYSTEM_INIT
CHAR _gx_version_id[] =
    "Copyright (c) 1996-2015 Express Logic Inc. * GUIX Win32/Visual C/C++ Version G5.2.6.5.4 SN: GUIX_Studio_Distribution *";
#else
extern  CHAR _gx_version_id[];
#endif

#endif


