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
#include "gx_canvas.h"
#include "gx_animation.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gxe_animation_start                                PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks for error in animation start call.             */
/*                                                                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    animation                             Pointer to animation control  */
/*                                            block                       */
/*    info                                  Animation information         */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_animation_start                   Actual animation start call.  */
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
UINT _gxe_animation_start(GX_ANIMATION *animation, GX_ANIMATION_INFO *info)
{

    if (animation == GX_NULL ||
        info == GX_NULL)
    {
        return GX_PTR_ERROR;
    }

    if (info->gx_animation_steps < 2 || info->gx_animation_frame_interval < 1)
    {
        return GX_INVALID_VALUE;
    }

    if (info -> gx_animation_end_alpha == 0 &&
        info -> gx_animation_start_alpha == 0)
    {
        return GX_INVALID_VALUE;
    }

    if (info -> gx_animation_target == GX_NULL)
    {
        return GX_INVALID_WIDGET;
    }
    if (info -> gx_animation_target -> gx_widget_type == 0)
    {
        /* target widget has not been created */
        return GX_INVALID_WIDGET;
    }

    /* Check for invalid animation status. */
    if (animation -> gx_animation_status != GX_ANIMATION_IDLE)
    {
        return(GX_INVALID_STATUS);
    }

#ifndef GX_BRUSH_ALPHA_SUPPORT
    if (info -> gx_animation_end_alpha != info -> gx_animation_start_alpha)
    {
        /* fade animations must have a canvas */
        if (animation -> gx_animation_canvas.gx_canvas_memory == GX_NULL)
        {
            return GX_INVALID_CANVAS;
        }
    }
#endif

    return(_gx_animation_start(animation, info));
}

