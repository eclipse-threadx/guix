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

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_system_pen_flick_test                           PORTABLE C      */
/*                                                           6.1.11       */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service tests for pen flick event.                             */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    event                                 Pointer to event              */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    tx_time_get                           Get current timer tick        */
/*    GX_ABS                                Compute the absolute value    */
/*    tx_queue_send                         Send message through ThreadX  */
/*                                            queue                       */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_system_event_send                 Send GUIX event               */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*  04-25-2022     Ting Zhu                 Modified comment(s),          */
/*                                            added a new parameter,      */
/*                                            resulting in version 6.1.11 */
/*                                                                        */
/**************************************************************************/
VOID _gx_system_pen_flick_test(GX_WIDGET *target)
{
GX_EVENT flick_event;
INT      abs_speed_x;
INT      abs_speed_y;
ULONG    current_clock;

    flick_event.gx_event_type = 0;

#ifdef GX_THREADX_BINDING
    current_clock = tx_time_get();
#else
    current_clock = GX_SYSTEM_TIME_GET;
#endif

    _gx_system_clock_delta = (LONG)(current_clock - _gx_system_last_clock);

    if (_gx_system_clock_delta < (LONG)_gx_system_pen_configuration.gx_pen_configuration_max_pen_speed_ticks)
    {
        abs_speed_x = GX_ABS(_gx_system_pen_speed_x);
        abs_speed_y = GX_ABS(_gx_system_pen_speed_y);

        if (abs_speed_y > abs_speed_x)
        {
            if (abs_speed_y > (INT)_gx_system_pen_configuration.gx_pen_configuration_min_drag_dist)
            {
                flick_event.gx_event_type = GX_EVENT_VERTICAL_FLICK;
                flick_event.gx_event_payload.gx_event_intdata[0] = _gx_system_pen_speed_y;
            }
        }
        else
        {
            if (abs_speed_x > (INT)_gx_system_pen_configuration.gx_pen_configuration_min_drag_dist)
            {
                flick_event.gx_event_type = GX_EVENT_HORIZONTAL_FLICK;
                flick_event.gx_event_payload.gx_event_intdata[0] = _gx_system_pen_speed_x;
            }
        }
    }
    if (flick_event.gx_event_type)
    {
        flick_event.gx_event_target = target;
        flick_event.gx_event_sender = 0;

#ifdef GX_THREADX_BINDING
        tx_queue_send(&_gx_system_event_queue, &flick_event, TX_NO_WAIT);
#else
        GX_EVENT_PUSH(&flick_event);
#endif
    }
}

