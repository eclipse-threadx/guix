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
/**   Radial Slider Management (Slider)                                   */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_widget.h"
#include "gx_radial_slider.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_radial_slider_animation_start                   PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service starts an animation to move the slider needle from     */
/*    current postion to the new position.                                */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    slider                                Radial slider control block   */
/*    target_angle                          Target angle value            */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_timer_start                Start a timer                 */
/*    _gx_widget_event_generate             Generate child signal         */
/*    _gx_radial_slider_angle_set           Set new slider value          */
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
UINT _gx_radial_slider_animation_start(GX_RADIAL_SLIDER *slider, GX_VALUE target_angle)
{
GX_RADIAL_SLIDER_INFO *info = &slider -> gx_radial_slider_info;
USHORT                 delay;

    if (target_angle != info->gx_radial_slider_info_current_angle)
    {
        if ((slider -> gx_radial_slider_animation_total_steps > 1) && slider -> gx_radial_slider_animation_delay)
        {
            slider -> gx_radial_slider_start_angle = info -> gx_radial_slider_info_current_angle;
            slider -> gx_radial_slider_target_angle = target_angle;
            slider -> gx_radial_slider_animation_step = slider -> gx_radial_slider_animation_total_steps;
            delay = slider -> gx_radial_slider_animation_delay;

            /* Start animation to move needle from current position to target position. */
            _gx_system_timer_start((GX_WIDGET *)slider, GX_RADIAL_SLIDER_TIMER, delay, delay);

            _gx_widget_event_generate((GX_WIDGET *)slider, GX_EVENT_ANIMATION_START, slider -> gx_radial_slider_target_angle);
        }
        else
        {
            _gx_radial_slider_angle_set(slider, target_angle);
        }
    }

    return(GX_SUCCESS);
}

