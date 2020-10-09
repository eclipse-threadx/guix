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
#include "gx_animation.h"
#include "gx_system.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_animation_drag_enable                           PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function sets screen slide animation event process as a hook   */
/*    procedure of the widget's default event process.                    */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    animation                             Pointer to animation control  */
/*                                            block                       */
/*    widget                                Pointer to widget control     */
/*                                            block                       */
/*    info                                  Animation information         */
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
UINT  _gx_animation_drag_enable(GX_ANIMATION *animation, GX_WIDGET *widget, GX_ANIMATION_INFO *info)
{
    /* Test invalid animation status. */
    if (animation -> gx_animation_status != GX_ANIMATION_IDLE)
    {
        return(GX_INVALID_STATUS);
    }

    /* Save the widget's event process function, and replace it with the slide animation event process. */
    animation -> gx_animation_original_event_process_function = widget -> gx_widget_event_process_function;
    widget -> gx_widget_event_process_function = _gx_animation_drag_event_process;

    /* Save the animation parameters */
    animation -> gx_animation_info = *info;
    animation -> gx_animation_timer = animation -> gx_animation_info.gx_animation_frame_interval;
    animation -> gx_animation_next = _gx_system_animation_list;
    animation -> gx_animation_total_steps = info -> gx_animation_steps;
    _gx_system_animation_list = animation;

    /* Return completion status code. */
    return(GX_SUCCESS);
}

