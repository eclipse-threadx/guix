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
/*    _gx_system_timer_update                             PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function is called when the GUIX timers need to be updated.    */
/*    It updates all of the GUIX application timers, and sends events     */
/*    as needed.                                                          */
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
/*   _gx_system_lock                        Lock system mutex             */
/*   _gx_system_unlock                      Unlock system mutex           */
/*   [gx_widget_event_process_function]     Event handler of timer owner  */
/*   _gx_system_timer_stop                  Stop the system timer         */
/*   _gx_animation_update                   Update the animation sequence */
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
/*                                                                        */
/**************************************************************************/
VOID  _gx_system_timer_update(ULONG ticks)
{
GX_TIMER  *current_timer;
GX_TIMER  *next_timer;
GX_WIDGET *timer_owner;
UINT       timer_id;
GX_EVENT   timer_event;

GX_ENTER_CRITICAL

    current_timer = _gx_system_active_timer_list;

    timer_event.gx_event_type = GX_EVENT_TIMER;

    while (current_timer)
    {
        next_timer = current_timer -> gx_timer_next;

        /* has this timer expired? */
        if (current_timer -> gx_timer_initial_ticks > ticks)
        {
            /* just decrement the timer */
            current_timer -> gx_timer_initial_ticks -= ticks;
        }
        else
        {
            /* timer has expired, call the timer owner's event handler */
            timer_owner = current_timer -> gx_timer_owner;
            timer_id = current_timer -> gx_timer_id;

            /* check to see if this timer should be removed or restarted */

            if (current_timer -> gx_timer_reschedule_ticks > 0)
            {
                /* reload the timer */
                current_timer -> gx_timer_initial_ticks = current_timer -> gx_timer_reschedule_ticks;
            }
            else
            {
                /* remove the timer */
                _gx_system_timer_stop(current_timer -> gx_timer_owner, current_timer -> gx_timer_id);
            }

            if (timer_owner != GX_NULL && timer_owner -> gx_widget_event_process_function)
            {
                timer_event.gx_event_payload.gx_event_timer_id = timer_id;
                timer_event.gx_event_target = timer_owner;
                /*_gx_system_event_fold(&timer_event);*/
                _gx_system_event_send(&timer_event);
            }
        }
        current_timer = next_timer;
    }

    if (_gx_system_animation_list)
    {
        _gx_animation_update();
    }

    /* release our lock */
    GX_EXIT_CRITICAL
}

