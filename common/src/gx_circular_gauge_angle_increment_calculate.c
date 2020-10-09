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
/**   Circular Gauge Management (Circular Gauge)                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_circular_gauge.h"
#include "gx_system.h"
#include "gx_utility.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_circular_gauge_angle_increment_calculate        PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function calcualtes needle angle increments for each step.     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    circular_gauge                        Circular gauge control block  */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
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

UINT _gx_circular_gauge_angle_increment_calculate(GX_CIRCULAR_GAUGE *gauge)
{
GX_CIRCULAR_GAUGE_INFO *info;
INT                     delay;

    /* Get gauge info. */
    info = &gauge -> gx_circular_gauge_info;

    /* Calculate total increments.  */
    gauge -> gx_circular_gauge_start_angle = gauge -> gx_circular_gauge_current_angle;

    gauge -> gx_circular_gauge_angle_increment = GX_FIXED_VAL_MAKE(gauge -> gx_circular_gauge_target_angle -
                                                  gauge -> gx_circular_gauge_current_angle);
    gauge -> gx_circular_gauge_animation_step = 0;

    /* Devide total increments by total steps.  */
    if ((gauge ->gx_widget_status & GX_STATUS_VISIBLE) &&
        gauge -> gx_circular_gauge_angle_increment &&
        info -> gx_circular_gauge_info_animation_steps > 0)
    {
        gauge -> gx_circular_gauge_angle_increment /= info -> gx_circular_gauge_info_animation_steps;

        /* Start needle animation timer.  */
        delay = gauge -> gx_circular_gauge_info.gx_circular_gauge_info_animation_delay;

        if (delay <= 0)
        {
            delay = GX_DEFAULT_CIRCULAR_GAUGE_ANIMATION_DELAY;
            gauge -> gx_circular_gauge_info.gx_circular_gauge_info_animation_delay = delay;
        }
        _gx_system_timer_start((GX_WIDGET *)gauge, GX_CIRCULAR_GAUGE_TIMER, (UINT)delay, (UINT)delay);
    }
    else
    {
        gauge -> gx_circular_gauge_current_angle = gauge -> gx_circular_gauge_target_angle;

        /* Mark dirty.  */
        if (gauge -> gx_widget_status & GX_STATUS_VISIBLE)
        {
            _gx_circular_gauge_needle_dirty_mark(gauge);
        }
    }

    return GX_SUCCESS;
}

