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
/*    gx_api.h                                           Win32/Visual     */
/*                                                           6.1.8        */
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

#define GX_VALUE_MAX                        0x7FFF

/* For the win32 port, the entry point is WinMain, which is defined
   in the win32 driver file.  The entry point for GUIX demo is gx_main(). */

#define main(a, b)                          gx_main(a, b)

#define GX_WIDGET_USER_DATA

/* Define the basic system parameters.  */


#define GX_CONST                            const

#define GX_INCLUDE_DEFAULT_COLORS

#define GX_MAX_ACTIVE_TIMERS                32

#define GX_MAX_VIEWS                        32

#define GX_MAX_DISPLAY_HEIGHT               800


#define GX_CALLER_CHECKING_EXTERNS
#define GX_THREADS_ONLY_CALLER_CHECKING
#define GX_INIT_AND_THREADS_CALLER_CHECKING
#define GX_NOT_ISR_CALLER_CHECKING
#define GX_THREAD_WAIT_CALLER_CHECKING


/* Define the version ID of GUIX.  This may be utilized by the application.  */

#ifdef GX_SYSTEM_INIT
CHAR _gx_version_id[] =
    "Copyright (c) Microsoft Corporation. All rights reserved.  *  GUIX Win32/Standalone/Visual Version 6.1.11 *";
#else
extern  CHAR _gx_version_id[];
#endif

#endif

