/**************************************************************************/
/*                                                                        */
/*            Copyright (c) 1996-2017 by Express Logic Inc.               */
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
/** GUIX Component                                                        */
/**                                                                       */
/**   User optional settings                                              */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/
/**************************************************************************/
/*                                                                        */
/*  APPLICATION INTERFACE DEFINITION                       RELEASE        */
/*                                                                        */
/*    gx_user.h                                           PORTABLE C      */
/*                                                           5.4          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    William E. Lamie, Express Logic, Inc.                               */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This file contains optional settings. You can enable and disable    */
/*    GUIX features by commenting out or including the definitions below  */
/*    to the implementation of high-performance GUIX UI framework.        */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  11-24-2014     William E. Lamie         Initial Version 5.2           */
/*  01-16-2015     William E. Lamie         Modified comment(s),          */
/*                                            resulting in version 5.2.1  */
/*  01-26-2015     William E. Lamie         Modified comment(s),          */
/*                                            resulting in version 5.2.2  */
/*  03-01-2015     William E. Lamie         Modified comment(s),          */
/*                                            resulting in version 5.2.3  */
/*  04-15-2015     William E. Lamie         Modified comment(s),          */
/*                                            resulting in version 5.2.4  */
/*  08-21-2015     William E. Lamie         Modified comment(s), added    */
/*                                            GX_SYSTEM_TIMER_MS so GUIX  */
/*                                            timer value can be modified,*/
/*                                            resulting in version 5.2.5  */
/*  09-21-2015     William E. Lamie         Modified comment(s),          */
/*                                            resulting in version 5.2.6  */
/*  02-22-2016     William E. Lamie         Modified comment(s),          */
/*                                            resulting in version 5.3    */
/*  04-05-2016     William E. Lamie         Modified comment(s),          */
/*                                            resulting in version 5.3.1  */
/*  06-15-2016     William E. Lamie         Modified comment(s),          */
/*                                            resulting in version 5.3.2  */
/*  03-01-2017     William E. Lamie         Modified comment(s),          */
/*                                            resulting in version 5.3.3  */
/*  11-06-2017     William E. Lamie         Modified comment(s),          */
/*                                            resulting in version 5.4    */
/*                                                                        */
/**************************************************************************/

#ifndef GX_USER_H
#define GX_USER_H

/* Should GUIX support multiple threads using the GUIX API simultaneously
   If your application is organized such that only one thread utilizes the
   GUI API services, comment out the definition below to reduce system
   overhead.
 */
/* #define GUIX_DISABLE_MULTITHREAD_SUPPORT */

/* Defined, GUIX disables UTF8 support.  */
/* #define GX_DISABLE_UTF8_SUPPORT */

/* By default GUIX System Timer runs at 20ms.  Modify the value below to
   change GUIX System Timer value. */
/* #define GX_SYSTEM_TIMER_MS  20 */

/* This can be defined to insert an application specific data 
   field into the GX_WIDGET control block */
/* #define GX_WIDGET_USER_DATA */
#endif

