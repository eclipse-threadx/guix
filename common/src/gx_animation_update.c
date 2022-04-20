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
#include "gx_utility.h"
#include "gx_animation.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_animation_update                                PORTABLE C      */
/*                                                           6.1.11       */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function executes one step on an animation sequence.           */
/*                                                                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_canvas_alpha_set                  Set the alpha of canvas       */
/*    _gx_animation_complete                Called when an animation      */
/*                                            sequence is finished        */
/*    _gx_canvas_offset_set                 Set the offset of canvas      */
/*    _gx_widget_shift                      Shift a widget                */
/*    _gx_animation_stop                    Stop an animation             */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_system_timer_update               Update active system timers   */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*  12-31-2020     Kenneth Maxwell          Modified comment(s), improve  */
/*                                            linear animation accuracy,  */
/*                                            resulting in version 6.1.3  */
/*  04-25-2022     Ting Zhu                 Modified comment(s),          */
/*                                            added block move support,   */
/*                                            resulting in version 6.1.11 */
/*                                                                        */
/**************************************************************************/
VOID _gx_animation_update(VOID)
{
GX_ANIMATION      *animation;
GX_ANIMATION      *next;
GX_WIDGET         *target;
INT                test_alpha;
INT                x_current;
INT                y_current;
INT                x_trans = 0;
INT                y_trans = 0;
GX_ANIMATION_INFO *info;
GX_WIDGET         *parent;
GX_RECTANGLE       block;

    animation = _gx_system_animation_list;

    while (animation)
    {
        info = &animation -> gx_animation_info;
        next = animation -> gx_animation_next;

        /* has this animation timer expired ? */
        if (animation -> gx_animation_timer > 1)
        {
            /* no, just decrement timer count and continue */
            animation -> gx_animation_timer--;
            animation = next;
            continue;
        }

        if (info -> gx_animation_style & GX_ANIMATION_SCREEN_DRAG)
        {
            animation = next;
            continue;
        }

        /* the animation timer has expired. First reload the
           timer counter in case we are not on last step
         */

        info -> gx_animation_steps = (GX_UBYTE)(info -> gx_animation_steps - 1);

        if (info -> gx_animation_steps <= 0)
        {
            _gx_animation_complete(animation);
            animation = next;
            continue;
        }
        else
        {
            animation -> gx_animation_timer = info -> gx_animation_frame_interval;
        }

        target = info -> gx_animation_target;

        /* check for a fade style animation */
        if (info -> gx_animation_end_alpha !=
            info -> gx_animation_start_alpha)
        {
            _gx_utility_easing_function_calculate(info -> gx_animation_style,
                                                  info -> gx_animation_start_alpha,
                                                  info -> gx_animation_end_alpha,
                                                  animation -> gx_animation_total_steps - info -> gx_animation_steps,
                                                  animation -> gx_animation_total_steps, &test_alpha);

            if (animation -> gx_animation_canvas)
            {
                _gx_canvas_alpha_set(animation -> gx_animation_canvas, (GX_UBYTE)test_alpha);
            }
#if defined(GX_BRUSH_ALPHA_SUPPORT)
            else
            {
                target -> gx_widget_alpha = (GX_UBYTE)test_alpha;
                if (test_alpha == 0xff)
                {
                    _gx_system_dirty_mark(target);
                }
                else
                {
                    _gx_system_dirty_partial_add(target -> gx_widget_parent, &target -> gx_widget_size);
                }
            }
#endif
        }

        /* check for a slide style animation */
        if ((info -> gx_animation_end_position.gx_point_x !=
             info -> gx_animation_start_position.gx_point_x) ||
            (info -> gx_animation_end_position.gx_point_y !=
             info -> gx_animation_start_position.gx_point_y))
        {
            if (animation -> gx_animation_canvas)
            {
                x_current = animation -> gx_animation_canvas -> gx_canvas_display_offset_x;
                y_current = animation -> gx_animation_canvas -> gx_canvas_display_offset_y;
            }
            else
            {
                x_current = target -> gx_widget_size.gx_rectangle_left;
                y_current = target -> gx_widget_size.gx_rectangle_top;
            }

            if (info -> gx_animation_start_position.gx_point_x != info -> gx_animation_end_position.gx_point_x)
            {
                _gx_utility_easing_function_calculate(info -> gx_animation_style,
                                                      info -> gx_animation_start_position.gx_point_x,
                                                      info -> gx_animation_end_position.gx_point_x,
                                                      animation -> gx_animation_total_steps - info -> gx_animation_steps,
                                                      animation -> gx_animation_total_steps, &x_trans);

                x_trans -= x_current;
            }
            else
            {
                x_trans = 0;
            }

            if (info -> gx_animation_start_position.gx_point_y != info -> gx_animation_end_position.gx_point_y)
            {
                _gx_utility_easing_function_calculate(info -> gx_animation_style,
                                                      info -> gx_animation_start_position.gx_point_y,
                                                      info -> gx_animation_end_position.gx_point_y,
                                                      animation -> gx_animation_total_steps - info -> gx_animation_steps,
                                                      animation -> gx_animation_total_steps, &y_trans);

                y_trans -= y_current;
            }
            else
            {
                y_trans = 0;
            }

            /* still moving */
            if (animation -> gx_animation_canvas)
            {
                /* adjust canvas offset */
                _gx_canvas_offset_set(animation -> gx_animation_canvas,
                                      (GX_VALUE)(x_current + x_trans), (GX_VALUE)(y_current + y_trans));
            }
            else
            {
                /* adjust target position */
                if (animation -> gx_animation_info.gx_animation_style & GX_ANIMATION_BLOCK_MOVE)
                {
                    parent = animation -> gx_animation_info.gx_animation_parent;
                    if (_gx_utility_rectangle_overlap_detect(&target -> gx_widget_size, &parent -> gx_widget_size, &block))
                    {
                        _gx_widget_scroll_shift(target, (GX_VALUE)x_trans, (GX_VALUE)y_trans, GX_TRUE);
                        _gx_widget_block_move(parent, &block, (GX_VALUE)x_trans, (GX_VALUE)y_trans);
                    }
                    else
                    {
                        _gx_widget_shift(target, (GX_VALUE)x_trans, (GX_VALUE)y_trans, GX_TRUE);
                    }
                }
                else
                {
                    _gx_widget_shift(target, (GX_VALUE)x_trans, (GX_VALUE)y_trans, GX_TRUE);
                }
            }
        }
        animation = next;
    }
}

