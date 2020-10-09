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
/**   System Management (System)                                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE

/* Include necessary system files.  */

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_system_rtos_bind                                PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This file contains a small set of functions that bind GUIX to the   */
/*    underlying RTOS. This is by default the ThreadX RTOS, but can be    */
/*    modified to support other operating systems. Refer to the GUIX      */
/*    User Guide for more information.                                    */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    Refer to GUIX User Guide                                            */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Refer to GUIX User Guide                                            */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    Refer to GUIX User Guide                                            */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    GUIX system serives                                                 */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/

#ifdef GX_DISABLE_THREADX_BINDING

/* If you are porting GUIX to another RTOS, the function shells below serve as prototypes to
   demonstrate what each RTOS specific service should look like. The user will need to complete
   each of these functions using syntax and services available within the chosen RTOS
 */

/* types used by GUIX, normally provided by ThreadX API, but replaced here */



VOID  gx_generic_rtos_initialize(VOID);
UINT  gx_generic_thread_start(VOID (*thread_entry)(ULONG));
UINT  gx_generic_event_post(GX_EVENT *event_ptr);
UINT  gx_generic_event_fold(GX_EVENT *event_ptr);
VOID  gx_generic_event_purge(GX_WIDGET *widget);
UINT  gx_generic_event_pop(GX_EVENT *put_event, GX_BOOL wait);
VOID  gx_generic_timer_start(VOID);
VOID  gx_generic_timer_stop(VOID);
VOID  gx_generic_system_mutex_lock(VOID);
VOID  gx_generic_system_mutex_unlock(VOID);
ULONG gx_generic_system_time_get(VOID);
VOID *gx_generic_thread_identify(VOID);
VOID  gx_generic_time_delay(INT ticks);


#define GX_RTOS_BINDING_INITIALIZE gx_generic_rtos_initialize()
#define GX_SYSTEM_THREAD_START     gx_generic_thread_start
#define GX_EVENT_PUSH              gx_generic_event_post
#define GX_EVENT_POP               gx_generic_event_pop
#define GX_EVENT_FOLD              gx_generic_event_fold
#define GX_EVENT_PURGE             gx_generic_event_purge
#define GX_TIMER_START             gx_generic_timer_start()
#define GX_TIMER_STOP              gx_generic_timer_stop()
#define GX_SYSTEM_MUTEX_LOCK       gx_generic_system_mutex_lock()
#define GX_SYSTEM_MUTEX_UNLOCK     gx_generic_system_mutex_unlock()
#define GX_SYSTEM_TIME_GET         gx_generic_system_time_get()
#define GX_CURRENT_THREAD          gx_generic_thread_identify()
#define GX_GENERIC_TIME_DELAY(a)   gx_generic_time_delay(a)



#endif

