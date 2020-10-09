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
/*    _gx_system_pen_speed_update                         PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service sends the specified event for processing.              */
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
/*    GX_ABS                                Compute the absolute value    */
/*    tx_queue_send                         Send message through ThreadX  */
/*                                            queue                       */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_system_event_fold                 Fold GUIX system events       */
/*    _gx_system_event_send                 Send GUIX system event        */
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
VOID _gx_system_pen_speed_update(GX_POINT *current)
{
ULONG current_clock;

#ifdef GX_THREADX_BINDING
    current_clock = tx_time_get();
#else
    current_clock = GX_SYSTEM_TIME_GET;
#endif

    _gx_system_clock_delta = (LONG)(current_clock - _gx_system_last_clock);

    if (_gx_system_clock_delta > 0)
    {
        if (_gx_system_clock_delta < (LONG)_gx_system_pen_configuration.gx_pen_configuration_max_pen_speed_ticks)
        {
            _gx_system_pen_speed_x = GX_FIXED_VAL_MAKE(current -> gx_point_x - _gx_system_last_pen_point.gx_point_x);
            _gx_system_pen_speed_x /= _gx_system_clock_delta;
            _gx_system_pen_speed_y = GX_FIXED_VAL_MAKE(current -> gx_point_y - _gx_system_last_pen_point.gx_point_y);
            _gx_system_pen_speed_y /= _gx_system_clock_delta;
        }
        else
        {
            _gx_system_pen_speed_x = _gx_system_pen_speed_y = 0;
        }
        _gx_system_last_clock = current_clock;
        _gx_system_last_pen_point = *current;
    }
}

