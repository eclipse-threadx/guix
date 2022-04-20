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
#include "gx_widget.h"
#include "gx_system.h"
#include "gx_utility.h"
#include "gx_radial_slider.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_radial_slider_animation_update                  PORTABLE C      */
/*                                                           6.1.11       */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function updates slider value for each animation step.         */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    slider                                Radial slider control block   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_easing_function_calculate Calculate next slide angle    */
/*                                            with specified easing       */
/*                                            function type               */
/*    _gx_radial_slider_angle_set           Set radial slider angle       */
/*    _gx_system_dirty_mark                 Mark widget as dirty          */
/*    _gx_system_timer_stop                 Stop specified timer          */
/*    _gx_widget_event_generate             Send event its parent         */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    GUIX Internal Code                                                  */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*  04-25-2022     Ting Zhu                 Modified comment(s),          */
/*                                            added animation step test,  */
/*                                            resulting in version 6.1.11 */
/*                                                                        */
/**************************************************************************/
UINT _gx_radial_slider_animation_update(GX_RADIAL_SLIDER *slider)
{
GX_RADIAL_SLIDER_INFO *info = &slider -> gx_radial_slider_info;
INT                    current_val = info -> gx_radial_slider_info_current_angle;

    if (slider -> gx_radial_slider_animation_step > 0)
    {
        slider -> gx_radial_slider_animation_step--;

        _gx_utility_easing_function_calculate(slider -> gx_radial_slider_animation_style,
                                              slider -> gx_radial_slider_start_angle,
                                              slider -> gx_radial_slider_target_angle,
                                              slider -> gx_radial_slider_animation_total_steps - slider -> gx_radial_slider_animation_step,
                                              slider -> gx_radial_slider_animation_total_steps,
                                              &current_val);

        /* set radial slider value. */
        _gx_radial_slider_angle_set(slider, (GX_VALUE)current_val);

        if (slider -> gx_radial_slider_animation_update_callback)
        {
            /* Call callback function. */
            slider -> gx_radial_slider_animation_update_callback(slider);
        }

        if (slider -> gx_radial_slider_animation_step == 0)
        {
            _gx_system_timer_stop((GX_WIDGET *)slider, GX_RADIAL_SLIDER_TIMER);

            _gx_widget_event_generate((GX_WIDGET *)slider, GX_EVENT_ANIMATION_COMPLETE, current_val);
        }

        if (slider -> gx_widget_status & GX_STATUS_VISIBLE)
        {
            _gx_system_dirty_mark((GX_WIDGET *)slider);
        }
    }

    return GX_SUCCESS;
}

