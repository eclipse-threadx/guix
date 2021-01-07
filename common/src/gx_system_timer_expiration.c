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

#include "gx_api.h"
#include "gx_system.h"
#include "gx_animation.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_system_timer_expiration                         PORTABLE C      */
/*                                                           6.1.3        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This is the callback function for the gx_system_timer. This         */
/*    function updates all of the GUIX application timers, and sends      */
/*    events as needed                                                    */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*   _gx_system_event_send                   Send GUI event               */
/*   tx_timer_deactivate                     Deactivate a timer           */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    tx_timer                                                            */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*  12-31-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            added GX_DISABLE_THREADX_   */
/*                                            TIMER_SOURCE configuration, */
/*                                            resulting in version 6.1.3  */
/*                                                                        */
/**************************************************************************/
VOID  _gx_system_timer_expiration(ULONG val)
{
GX_EVENT timer_event;

    GX_PARAMETER_NOT_USED(val);

    /* if there are no active timers just stop the timer from running */
    if (_gx_system_active_timer_list == GX_NULL &&
        _gx_system_animation_list == GX_NULL)
    {
#ifdef GX_THREADX_BINDING
#ifndef GX_DISABLE_THREADX_TIMER_SOURCE
        tx_timer_deactivate(&_gx_system_timer);
#endif
#else
        GX_TIMER_STOP;
#endif
        return;
    }

    timer_event.gx_event_type = GX_EVENT_TIMER;
    timer_event.gx_event_sender = 0;
    timer_event.gx_event_target = GX_NULL;
    timer_event.gx_event_payload.gx_event_ulongdata = 1;
    _gx_system_event_fold(&timer_event);
}

