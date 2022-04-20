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
#include "gx_animation.h"
#include "gx_canvas.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_animation_drag_tracking                         PORTABLE C      */
/*                                                           6.1.11       */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function moves the animation screens according to current pen  */
/*      position.                                                         */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    animation                             Pointer to animation control  */
/*                                            block                       */
/*    penpos                                Current pen position          */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_detach                     Detach a widget from its      */
/*                                            parent                      */
/*    _gx_widget_shift                      Change widget's position      */
/*    _gx_animation_drag_tracking_start     Prepare for screen draw       */
/*                                            animation                   */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_animation_drag_event_check                                      */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*  04-25-2022     Ting Zhu                 Modified comment(s),          */
/*                                            added canvas and block move */
/*                                            support,                    */
/*                                            resulting in version 6.1.11 */
/*                                                                        */
/**************************************************************************/
UINT  _gx_animation_drag_tracking(GX_ANIMATION *animation, GX_POINT penpos)
{
GX_VALUE     delta_x = 0;
GX_VALUE     delta_y = 0;
GX_VALUE     shift_x = 0;
GX_VALUE     shift_y = 0;
GX_VALUE     last_pos;
GX_VALUE     start_pos;
GX_VALUE     pen_pos;
GX_WIDGET   *target_1 = GX_NULL;
GX_WIDGET   *target_2 = GX_NULL;
GX_WIDGET   *parent;
GX_RECTANGLE block;
GX_VALUE     border_width;

    if (animation -> gx_animation_slide_target_index_1 >= 0)
    {
        target_1 = animation -> gx_animation_info.gx_animation_slide_screen_list[animation -> gx_animation_slide_target_index_1];
    }

    if (target_1 == GX_NULL)
    {
        return GX_SUCCESS;
    }

    if (animation -> gx_animation_slide_target_index_2 >= 0)
    {
        target_2 = animation -> gx_animation_info.gx_animation_slide_screen_list[animation -> gx_animation_slide_target_index_2];
    }

    last_pos = animation -> gx_animation_slide_tracking_current_pos;
    if (animation -> gx_animation_info.gx_animation_style & GX_ANIMATION_VERTICAL)
    {
        pen_pos = penpos.gx_point_y;
        delta_y = (GX_VALUE)(pen_pos - last_pos);
    }
    else
    {
        pen_pos = penpos.gx_point_x;
        delta_x = (GX_VALUE)(pen_pos - last_pos);
    }

    if (delta_x || delta_y)
    {
        /* Calculate sliding distance. */
        start_pos = animation -> gx_animation_slide_tracking_start_pos;

        if (((animation -> gx_animation_slide_direction == GX_ANIMATION_SLIDE_LEFT) && (pen_pos > start_pos)) ||
            ((animation -> gx_animation_slide_direction == GX_ANIMATION_SLIDE_RIGHT) && (pen_pos < start_pos)) ||
            ((animation -> gx_animation_slide_direction == GX_ANIMATION_SLIDE_UP) && (pen_pos > start_pos)) ||
            ((animation -> gx_animation_slide_direction == GX_ANIMATION_SLIDE_DOWN) && (pen_pos < start_pos)))
        {
            /* Sliding direction changed, detach the second animation target and
               call tracking start again.  */
            if (target_2)
            {
                _gx_widget_detach(target_2);
            }

            if (animation -> gx_animation_canvas)
            {
                _gx_widget_shift(target_1,
                                 animation -> gx_animation_canvas -> gx_canvas_display_offset_x,
                                 animation -> gx_animation_canvas -> gx_canvas_display_offset_y, GX_TRUE);
            }

            _gx_animation_drag_tracking_start(animation, penpos);

            if (animation -> gx_animation_slide_target_index_2 >= 0)
            {
                target_2 = animation -> gx_animation_info.gx_animation_slide_screen_list[animation -> gx_animation_slide_target_index_2];
            }
            else
            {
                target_2 = GX_NULL;
            }

            if (animation -> gx_animation_info.gx_animation_style & GX_ANIMATION_VERTICAL)
            {
                shift_y = (GX_VALUE)(start_pos - last_pos);
                delta_y = (GX_VALUE)(pen_pos - start_pos);
            }
            else
            {
                shift_x = (GX_VALUE)(start_pos - last_pos);
                delta_x = (GX_VALUE)(pen_pos - start_pos);
            }
        }

        if (!target_2)
        {
            if (animation -> gx_animation_info.gx_animation_style & GX_ANIMATION_VERTICAL)
            {
                animation -> gx_animation_slide_tracking_start_pos = (GX_VALUE)(animation -> gx_animation_slide_tracking_start_pos + ((delta_y + 1) >> 1));
                delta_y >>= 1;
            }
            else
            {
                animation -> gx_animation_slide_tracking_start_pos = (GX_VALUE)(animation -> gx_animation_slide_tracking_start_pos + ((delta_x + 1) >> 1));
                delta_x >>= 1;
            }
        }

        if (animation -> gx_animation_canvas)
        {
            /* adjust canvas offset */
            _gx_canvas_offset_set(animation -> gx_animation_canvas,
                                  (GX_VALUE)(animation -> gx_animation_canvas -> gx_canvas_display_offset_x + shift_x + delta_x),
                                  (GX_VALUE)(animation -> gx_animation_canvas -> gx_canvas_display_offset_y + shift_y + delta_y));
        }
        else
        {
            if (animation -> gx_animation_info.gx_animation_style & GX_ANIMATION_BLOCK_MOVE)
            {
                if (target_2)
                {
                    _gx_widget_scroll_shift(target_2, (GX_VALUE)(shift_x + delta_x), (GX_VALUE)(shift_y + delta_y), GX_TRUE);
                }

                _gx_widget_scroll_shift(target_1, (GX_VALUE)(shift_x + delta_x), (GX_VALUE)(shift_y + delta_y), GX_TRUE);

                parent = animation -> gx_animation_info.gx_animation_parent;
                _gx_widget_border_width_get(parent, &border_width);
                _gx_widget_client_get(parent, border_width, &block);
                _gx_widget_block_move(parent, &block, (GX_VALUE)(shift_x + delta_x), (GX_VALUE)(shift_y + delta_y));
            }
            else
            {
                if (target_2)
                {
                    _gx_widget_shift(target_2, (GX_VALUE)(shift_x + delta_x), (GX_VALUE)(shift_y + delta_y), GX_TRUE);
                }

                _gx_widget_shift(target_1, (GX_VALUE)(shift_x + delta_x), (GX_VALUE)(shift_y + delta_y), GX_TRUE);
            }
        }

        animation -> gx_animation_slide_tracking_current_pos = pen_pos;
    }

    /* Return completion status code. */
    return(GX_SUCCESS);
}

