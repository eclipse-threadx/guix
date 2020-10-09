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
#include "gx_utility.h"
#include "gx_system.h"
#include "gx_circular_gauge.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_circular_gauge_animation_set                    PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function sets animation steps delay time for a circular gauge. */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    gauge                                 Pointer to circular gauge     */
/*                                            control block.              */
/*    steps                                 Total steps for one rotation  */
/*    delay                                 Delay time for every step     */
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
UINT _gx_circular_gauge_animation_set(GX_CIRCULAR_GAUGE *gauge, INT steps, INT delay)
{
GX_CIRCULAR_GAUGE_INFO *info = &gauge -> gx_circular_gauge_info;

    info -> gx_circular_gauge_info_animation_steps = steps;

    if (delay <= 0)
    {
        delay = GX_DEFAULT_CIRCULAR_GAUGE_ANIMATION_DELAY;
    }
    info -> gx_circular_gauge_info_animation_delay = delay;

    if (steps > 0)
    {

        /* Calculate needle increment angle for each step.  */
        _gx_circular_gauge_angle_increment_calculate(gauge);

        /* Start needle animation timer.  */
        _gx_system_timer_start((GX_WIDGET *)gauge, GX_CIRCULAR_GAUGE_TIMER, (UINT)delay, (UINT)delay);
    }
    else
    {
        /* Calculate needle increment angle to get from current to target in one step.  */
        _gx_circular_gauge_angle_increment_calculate(gauge);
    }
    return GX_SUCCESS;
}

