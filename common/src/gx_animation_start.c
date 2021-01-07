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
/*    _gx_animation_start                                 PORTABLE C      */
/*                                                           6.1.3        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function starts an animation sequence.                         */
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
/*    _gx_widget_detach                     Detach a widget from its      */
/*                                            parent                      */
/*    _gx_widget_shift                      Shift a widget                */
/*    _gx_canvas_offset_set                 Set the offset of canvas      */
/*    _gx_canvas_alpha_set                  Set the alpha of canvas       */
/*    _gx_widget_attach                     Attach a widget to its parent */
/*    _gx_widget_show                       Show a widget                 */
/*    tx_timer_info_get                     Get the information of        */
/*                                            ThreadX timer               */
/*    tx_timer_activate                     Start the ThreadX timer       */
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
/*  12-31-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            added GX_DISABLE_THREADX_   */
/*                                            TIMER_SOURCE configuration, */
/*                                            resulting in version 6.1.3  */
/*                                                                        */
/**************************************************************************/
UINT _gx_animation_start(GX_ANIMATION *animation, GX_ANIMATION_INFO *info)
{
UINT            status = GX_SUCCESS;
GX_WINDOW_ROOT *root;

#ifdef GX_THREADX_BINDING
#ifndef GX_DISABLE_THREADX_TIMER_SOURCE
UINT     tx_timer_active;
#endif
#endif
GX_VALUE left;
GX_VALUE top;

    animation -> gx_animation_total_steps = info -> gx_animation_steps;

    if (animation -> gx_animation_canvas)
    {
        /* Make sure the widget is not attached to any parent */
        _gx_widget_detach(info -> gx_animation_target);

        /* position the target widget at 0,0 within the animation canvas */
        left = info -> gx_animation_target -> gx_widget_size.gx_rectangle_left;
        top = info -> gx_animation_target -> gx_widget_size.gx_rectangle_top;

        if (left || top)
        {
            _gx_widget_shift(info -> gx_animation_target, (GX_VALUE)-left, (GX_VALUE)-top, GX_FALSE);
        }

        /* position the canvas at the animation starting position */
        _gx_canvas_offset_set(animation -> gx_animation_canvas,
                              info -> gx_animation_start_position.gx_point_x,
                              info -> gx_animation_start_position.gx_point_y);

        /* link the target to the animation root window */
        root = _gx_system_root_window_created_list;
        while (root && root -> gx_window_root_canvas != animation -> gx_animation_canvas)
        {
            root = (GX_WINDOW_ROOT *)root -> gx_widget_next;
        }
        if (root)
        {
            _gx_widget_attach((GX_WIDGET *)root, info -> gx_animation_target);

            /* and show the animation root window to make everything visible */
            _gx_widget_show((GX_WIDGET *)root);
            _gx_canvas_drawing_initiate(animation -> gx_animation_canvas, (GX_WIDGET *) root, &root -> gx_widget_size);
            _gx_widget_children_draw((GX_WIDGET *)root);
            _gx_canvas_drawing_complete(animation -> gx_animation_canvas, GX_FALSE);

            /* set the initial alpha and make our canvas visible */
            _gx_canvas_alpha_set(animation -> gx_animation_canvas, info -> gx_animation_start_alpha);
            _gx_canvas_show(animation -> gx_animation_canvas);
        }
        else
        {
            status = GX_FAILURE;
        }
    }
    else
    {
        if (info -> gx_animation_start_alpha != info -> gx_animation_end_alpha)
        {
#ifdef GX_BRUSH_ALPHA_SUPPORT
            info -> gx_animation_target -> gx_widget_style |= GX_STYLE_USE_LOCAL_ALPHA;
            info -> gx_animation_target -> gx_widget_alpha = info -> gx_animation_start_alpha;
#else
            status = GX_INVALID_VALUE;
#endif
        }

        /* position the target at the starting position */
        left = (GX_VALUE)(info -> gx_animation_start_position.gx_point_x -
                          info -> gx_animation_target -> gx_widget_size.gx_rectangle_left);
        top =  (GX_VALUE)(info -> gx_animation_start_position.gx_point_y -
                          info -> gx_animation_target -> gx_widget_size.gx_rectangle_top);

        if (left || top)
        {
            _gx_widget_shift(info -> gx_animation_target,
                             left, top, GX_TRUE);
        }

        /* link the target to the animation root window */
        _gx_widget_attach(info -> gx_animation_parent, info -> gx_animation_target);
    }

    /* If we were able to start this animation, link it into the active list */
    if (status == GX_SUCCESS)
    {
        /* save the animation parameters */
        animation -> gx_animation_info = *info;
        if (info -> gx_animation_start_delay)
        {
            animation -> gx_animation_timer = info -> gx_animation_start_delay;
        }
        else
        {
            animation -> gx_animation_timer = animation -> gx_animation_info.gx_animation_frame_interval;
        }
        animation -> gx_animation_status = GX_ANIMATION_ACTIVE;
        animation -> gx_animation_next = _gx_system_animation_list;
        _gx_system_animation_list = animation;

#ifdef GX_THREADX_BINDING
#ifndef GX_DISABLE_THREADX_TIMER_SOURCE
        /* if the low-level timer is not active, start it */
        tx_timer_info_get(&_gx_system_timer, (CHAR **)TX_NULL, &tx_timer_active,
                          (ULONG *)TX_NULL, (ULONG *)TX_NULL, (TX_TIMER **)TX_NULL);

        if (!tx_timer_active)
        {
            tx_timer_activate(&_gx_system_timer);
        }
#endif
#else
        GX_TIMER_START;
#endif
    }

    return(status);
}

