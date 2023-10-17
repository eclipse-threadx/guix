/**************************************************************************/ 
/*                                                                        */ 
/*            Copyright (c) 1996-2011 by Express Logic Inc.               */ 
/*                                                                        */ 
/*  This software is copyrighted by and is the sole property of Express   */ 
/*  Logic, Inc.  All rights, title, ownership, or other interests         */ 
/*  in the software remain the property of Express Logic, Inc.  This      */ 
/*  software may only be used in accordance with the corresponding        */ 
/*  license agreement.  Any unauthorized use, duplication, transmission,  */ 
/*  distribution, or disclosure of this software is expressly forbidden.  */ 
/*                                                                        */
/*  This Copyright notice may not be removed or modified without prior    */ 
/*  written consent of Express Logic, Inc.                                */ 
/*                                                                        */ 
/*  Express Logic, Inc. reserves the right to modify this software        */ 
/*  without notice.                                                       */ 
/*                                                                        */ 
/*  Express Logic, Inc.                     info@expresslogic.com         */
/*  11423 West Bernardo Court               http://www.expresslogic.com   */
/*  San Diego, CA  92127                                                  */
/*                                                                        */
/**************************************************************************/


/**************************************************************************/
/**************************************************************************/
/**                                                                       */ 
/** FileX Component                                                       */
/**                                                                       */
/**   Port Specific                                                       */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/


/**************************************************************************/ 
/*                                                                        */ 
/*  PORT SPECIFIC C INFORMATION                            RELEASE        */ 
/*                                                                        */ 
/*    fx_port.h                                         Cortex-M4/IAR     */ 
/*                                                           5.0          */ 
/*                                                                        */
/*  AUTHOR                                                                */ 
/*                                                                        */ 
/*    William E. Lamie, Express Logic, Inc.                               */ 
/*                                                                        */ 
/*  DESCRIPTION                                                           */ 
/*                                                                        */ 
/*    This file contains data type definitions that make the FileX FAT    */ 
/*    compatible file system function identically on a variety of         */ 
/*    different processor architectures.  For example, the byte offset of */ 
/*    various entries in the boot record, and directory entries are       */ 
/*    defined in this file.                                               */ 
/*                                                                        */ 
/*  RELEASE HISTORY                                                       */ 
/*                                                                        */ 
/*    DATE              NAME                      DESCRIPTION             */ 
/*                                                                        */ 
/*  12-12-2011     William E. Lamie         Initial Cortex-M4/IAR         */
/*                                            Support Version 5.0         */
/*                                                                        */ 
/**************************************************************************/ 

#ifndef FX_PORT_H
#define FX_PORT_H


/* Determine if the optional FileX user define file should be used.  */

#ifdef FX_INCLUDE_USER_DEFINE_FILE


/* Yes, include the user defines in fx_user.h. The defines in this file may 
   alternately be defined on the command line.  */

#include "fx_user.h"
#endif


/* Include the ThreadX api file.  */

#include "tx_api.h"


/* Define FileX internal protection macros.  If FX_SINGLE_THREAD is defined,
   these protection macros are effectively disabled.  However, for multi-thread
   uses, the macros are setup to utilize a ThreadX mutex for multiple thread 
   access control into an open media.  */

#ifdef FX_SINGLE_THREAD
#define FX_PROTECT                   
#define FX_UNPROTECT
#else
#define FX_PROTECT                      if (media_ptr -> fx_media_id != FX_MEDIA_ID) return(FX_MEDIA_NOT_OPEN); \
                                        else if (tx_mutex_get(&(media_ptr -> fx_media_protect), TX_WAIT_FOREVER) != TX_SUCCESS) return(FX_MEDIA_NOT_OPEN);
#define FX_UNPROTECT                    tx_mutex_put(&(media_ptr -> fx_media_protect));
#endif


/* Define interrupt lockout constructs to protect the system date/time from being updated
   while they are being read.  */

#define FX_INT_SAVE_AREA                unsigned int  old_interrupt_posture;
#define FX_DISABLE_INTS                 old_interrupt_posture =  tx_interrupt_control(TX_INT_DISABLE);
#define FX_RESTORE_INTS                 tx_interrupt_control(old_interrupt_posture);


/* Define the error checking logic to determine if there is a caller error in the FileX API.  
   The default definitions assume ThreadX is being used.  This code can be completely turned 
   off by just defining these macros to white space.  */

#ifndef TX_TIMER_PROCESS_IN_ISR

#define FX_CALLER_CHECKING_EXTERNS      extern  TX_THREAD      *_tx_thread_current_ptr; \
                                        extern  TX_THREAD       _tx_timer_thread; \
                                        extern  volatile ULONG  _tx_thread_system_state;

#define FX_CALLER_CHECKING_CODE         if ((_tx_thread_system_state) || \
                                            (_tx_thread_current_ptr == TX_NULL) || \
                                            (_tx_thread_current_ptr == &_tx_timer_thread)) \
                                            return(FX_CALLER_ERROR);

#else
#define FX_CALLER_CHECKING_EXTERNS      extern  TX_THREAD      *_tx_thread_current_ptr; \
                                        extern  volatile ULONG  _tx_thread_system_state;

#define FX_CALLER_CHECKING_CODE         if ((_tx_thread_system_state) || \
                                            (_tx_thread_current_ptr == TX_NULL)) \
                                            return(FX_CALLER_ERROR);
#endif


/* Define the update rate of the system timer.  These values may also be defined at the command
   line when compiling the fx_system_initialize.c module in the FileX library build.  Alternatively, they can
   be modified in this file.  Note: the update rate must be an even number of seconds greater
   than or equal to 2, which is the minimal update rate for FAT time. */

#ifndef FX_UPDATE_RATE_IN_SECONDS
#define FX_UPDATE_RATE_IN_SECONDS       10     /* Update time at 10 second intervals */
#endif

#ifndef FX_UPDATE_RATE_IN_TICKS
#define FX_UPDATE_RATE_IN_TICKS         1000   /* Same update rate, but in ticks  */
#endif

typedef unsigned long long              UINT64;

/* Define the version ID of FileX.  This may be utilized by the application.  */

#ifdef FX_SYSTEM_INIT
CHAR                            _fx_version_id[] = 
                                    "Copyright (c) 1996-2013 Express Logic Inc. * FileX Cortex-M4/IAR Version G5.2.5.0 SN: INTERNAL_TEST *";
#else
extern  CHAR                    _fx_version_id[];
#endif

#endif

