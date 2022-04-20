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
/**   Animation Management (Animation)                                    */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_widget.h"
#include "gx_system.h"
#include "gx_animation.h"
#include "gx_utility.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_animation_slide_landing_start                   PORTABLE C      */
/*                                                           6.1.11       */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function starts a timer to move the sliding screens to the     */
/*      target position step by step.                                     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    animation                             Pointer to animation control  */
/*                                            block                       */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_timer_start                Start a timer for a widget    */
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
/*  04-25-2022     Ting Zhu                 Modified comment(s),          */
/*                                            added canvas support,       */
/*                                            resulting in version 6.1.11 */
/*                                                                        */
/**************************************************************************/
UINT  _gx_animation_slide_landing_start(GX_ANIMATION *animation)
{
GX_ANIMATION_INFO *info;
GX_WIDGET         *target;
GX_RECTANGLE      *target_size;

    if ((animation -> gx_animation_slide_target_index_1 >= 0) &&
        (animation -> gx_animation_status == GX_ANIMATION_IDLE))
    {
        info = &animation -> gx_animation_info;

        if (animation -> gx_animation_slide_target_index_2 >= 0)
        {
            target = info -> gx_animation_slide_screen_list[animation -> gx_animation_slide_target_index_2];
        }
        else
        {
            target = info -> gx_animation_slide_screen_list[animation -> gx_animation_slide_target_index_1];
        }

        target_size = &target -> gx_widget_size;

        if (animation -> gx_animation_canvas)
        {
            info -> gx_animation_start_position.gx_point_x = (GX_VALUE)(target_size -> gx_rectangle_left + animation -> gx_animation_canvas -> gx_canvas_display_offset_x);
            info -> gx_animation_start_position.gx_point_y = (GX_VALUE)(target_size -> gx_rectangle_top + animation -> gx_animation_canvas -> gx_canvas_display_offset_y);
        }
        else
        {
            info -> gx_animation_start_position.gx_point_x = target_size -> gx_rectangle_left;
            info -> gx_animation_start_position.gx_point_y = target_size -> gx_rectangle_top;
        }

        /* Start a landing timer. */
        _gx_system_timer_start(info -> gx_animation_parent, GX_ANIMATION_SLIDE_TIMER,
                               info -> gx_animation_frame_interval,
                               info -> gx_animation_frame_interval);

        /* Set animation status. */
        animation -> gx_animation_status = GX_ANIMATION_SLIDE_LANDING;
        info -> gx_animation_steps = animation -> gx_animation_total_steps;
    }

    /* Return completion status code. */
    return(GX_SUCCESS);
}

