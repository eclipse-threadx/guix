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


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gxe_animation_drag_enable                          PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks for error in animation slide enable call.      */
/*                                                                        */
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
/*    _gx_animation_slide_enable            Actual animation slide enable */
/*                                            call.                       */
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
UINT _gxe_animation_drag_enable(GX_ANIMATION *animation, GX_WIDGET *widget, GX_ANIMATION_INFO *info)
{
    /* Check for invalid pointer. */
    if ((animation == GX_NULL) ||
        (widget == GX_NULL) ||
        (info == GX_NULL))
    {
        return GX_PTR_ERROR;
    }

    /* animation drag is already enabled ? */
    if (animation -> gx_animation_original_event_process_function != GX_NULL)
    {
        return GX_INVALID_ANIMATION;
    }

    if ((info -> gx_animation_style & GX_ANIMATION_EASING_FUNC_MASK) &&
        (info -> gx_animation_steps < 2))
    {
        return GX_INVALID_VALUE;
    }

    if (info -> gx_animation_frame_interval < 1)
    {
        return GX_INVALID_VALUE;
    }

    if (info -> gx_animation_slide_screen_list == GX_NULL)
    {
        /* Slide screen list has not been created */
        return GX_INVALID_WIDGET;
    }

    return(_gx_animation_drag_enable(animation, widget, info));
}

