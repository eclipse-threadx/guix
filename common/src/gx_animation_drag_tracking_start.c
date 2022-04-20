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
#include "gx_system.h"
#include "gx_canvas.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_animation_drag_tracking_start                   PORTABLE C      */
/*                                                           6.1.11       */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function prepares for screen slide animation.                  */
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
/*    _gx_widget_resize                     Resize widget                 */
/*    _gx_widget_attach                     Attach a widget to its parent */
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
/*                                            added canvas support,       */
/*                                            resulting in version 6.1.11 */
/*                                                                        */
/**************************************************************************/
UINT  _gx_animation_drag_tracking_start(GX_ANIMATION *animation, GX_POINT penpos)
{
GX_ANIMATION_INFO *info;
GX_RECTANGLE       size;
INT                width;
GX_WIDGET         *target_1;
GX_WIDGET         *target_2 = GX_NULL;
INT                index;
INT                current_pos;
GX_WINDOW_ROOT    *root;
GX_VALUE           left;
GX_VALUE           top;
VOID               (*active_display_area_set)(INT layer, GX_RECTANGLE *size);

    info = &animation -> gx_animation_info;

    target_1 = info -> gx_animation_slide_screen_list[0];
    index = 0;

    /* Search for visible screen, which is the first animation target. */
    while (target_1)
    {
        if (target_1 -> gx_widget_status & GX_STATUS_VISIBLE)
        {
            animation -> gx_animation_slide_target_index_1 = (GX_VALUE)index;
        }
        index++;
        target_1 = info -> gx_animation_slide_screen_list[index];
    }

    animation -> gx_animation_slide_screen_list_size = (USHORT)index;

    if (animation -> gx_animation_slide_target_index_1 == -1)
    {
        return(GX_FAILURE);
    }

    if (info -> gx_animation_style & GX_ANIMATION_VERTICAL)
    {
        current_pos = penpos.gx_point_y;
    }
    else
    {
        current_pos = penpos.gx_point_x;
    }

    /* Find second animation target according to sliding direction. */
    if (current_pos < animation -> gx_animation_slide_tracking_start_pos)
    {
        /* Sliding left/up. */
        index = animation -> gx_animation_slide_target_index_1 + 1;

        if ((index >= animation -> gx_animation_slide_screen_list_size) &&
            (info -> gx_animation_style & GX_ANIMATION_WRAP))
        {
            index = 0;
        }
    }
    else
    {
        /* Sliding right/down. */
        index = animation -> gx_animation_slide_target_index_1 - 1;

        if ((index < 0) && (info -> gx_animation_style & GX_ANIMATION_WRAP))
        {
            index = animation -> gx_animation_slide_screen_list_size - 1;
        }
    }

    if (info -> gx_animation_style & GX_ANIMATION_VERTICAL)
    {
        if (current_pos < animation -> gx_animation_slide_tracking_start_pos)
        {
            animation -> gx_animation_slide_direction = GX_ANIMATION_SLIDE_UP;
        }
        else
        {
            animation -> gx_animation_slide_direction = GX_ANIMATION_SLIDE_DOWN;
        }
    }
    else
    {
        if (current_pos < animation -> gx_animation_slide_tracking_start_pos)
        {
            animation -> gx_animation_slide_direction = GX_ANIMATION_SLIDE_LEFT;
        }
        else
        {
            animation -> gx_animation_slide_direction = GX_ANIMATION_SLIDE_RIGHT;
        }
    }

    target_1 = info -> gx_animation_slide_screen_list[animation -> gx_animation_slide_target_index_1];

    if ((index >= 0) && (index < animation -> gx_animation_slide_screen_list_size))
    {
        animation -> gx_animation_slide_target_index_2 = (GX_VALUE)(index);

        target_2 = info -> gx_animation_slide_screen_list[index];

        size = target_2 -> gx_widget_size;

        if (info -> gx_animation_style & GX_ANIMATION_VERTICAL)
        {
            width = size.gx_rectangle_bottom - size.gx_rectangle_top + 1;

            switch (animation -> gx_animation_slide_direction)
            {
            case GX_ANIMATION_SLIDE_UP:

                /* Slide up. */
                size.gx_rectangle_top = (GX_VALUE)(target_1 -> gx_widget_size.gx_rectangle_bottom + 1);
                size.gx_rectangle_bottom = (GX_VALUE)(size.gx_rectangle_top + width - 1);
                break;

            default:
                /* Slide down. */
                size.gx_rectangle_bottom = (GX_VALUE)(target_1 -> gx_widget_size.gx_rectangle_top - 1);
                size.gx_rectangle_top = (GX_VALUE)(size.gx_rectangle_bottom - width + 1);
                break;
            }
            size.gx_rectangle_left = (GX_VALUE)target_1 -> gx_widget_size.gx_rectangle_left;
            size.gx_rectangle_right = (GX_VALUE)target_1 -> gx_widget_size.gx_rectangle_right;
        }
        else
        {
            width = size.gx_rectangle_right - size.gx_rectangle_left + 1;

            switch (animation -> gx_animation_slide_direction)
            {
            case GX_ANIMATION_SLIDE_LEFT:
                /* Slide left. */
                size.gx_rectangle_left = (GX_VALUE)(target_1 -> gx_widget_size.gx_rectangle_right + 1);
                size.gx_rectangle_right = (GX_VALUE)(size.gx_rectangle_left + width - 1);
                break;
            default:
                /* Slide right. */
                size.gx_rectangle_right = (GX_VALUE)(target_1 -> gx_widget_size.gx_rectangle_left - 1);
                size.gx_rectangle_left = (GX_VALUE)(size.gx_rectangle_right - width + 1);
                break;
            }

            size.gx_rectangle_top = (GX_VALUE)target_1 -> gx_widget_size.gx_rectangle_top;
            size.gx_rectangle_bottom = (GX_VALUE)target_1 -> gx_widget_size.gx_rectangle_bottom;
        }

        /* Resize the second animation target. */
        _gx_widget_resize(target_2, &size);

        if (!animation -> gx_animation_canvas)
        {

            /* Attach the second target to animation parent.  */
            _gx_widget_attach(info -> gx_animation_parent, target_2);
        }
    }
    else
    {
        animation -> gx_animation_slide_target_index_2 = -1;
    }

    if (animation -> gx_animation_canvas)
    {
        /* Find animation root.  */
        root = _gx_system_root_window_created_list;
        while (root && root -> gx_window_root_canvas != animation -> gx_animation_canvas)
        {
            root = (GX_WINDOW_ROOT *)root -> gx_widget_next;
        }

        if (animation -> gx_animation_slide_direction == GX_ANIMATION_SLIDE_UP ||
            animation -> gx_animation_slide_direction == GX_ANIMATION_SLIDE_LEFT ||
            (!target_2))
        {
            left = target_1 -> gx_widget_size.gx_rectangle_left;
            top = target_1 -> gx_widget_size.gx_rectangle_top;
        }
        else
        {
            left = target_2 -> gx_widget_size.gx_rectangle_left;
            top = target_2 -> gx_widget_size.gx_rectangle_top;
        }

        if (left || top)
        {
            _gx_widget_shift(target_1, (GX_VALUE)-left, (GX_VALUE)-top, GX_TRUE);

            if (target_2)
            {
                _gx_widget_shift(target_2, (GX_VALUE)-left, (GX_VALUE)-top, GX_TRUE);
            }
        }

        /* Position the canvas at the animation starting position.  */
        _gx_canvas_offset_set(animation -> gx_animation_canvas, left, top);

        if (animation -> gx_animation_canvas -> gx_canvas_hardware_layer >= 0)
        {
            active_display_area_set = animation -> gx_animation_canvas -> gx_canvas_display -> gx_display_layer_services -> gx_display_layer_active_display_area_set;

            if (active_display_area_set)
            {
                /* Set active display area as the animation parent widget size. */
                active_display_area_set(animation -> gx_animation_canvas -> gx_canvas_hardware_layer, &info -> gx_animation_parent -> gx_widget_size);
            }
        }

        if (root)
        {
            /* Link the target to the animation root window.  */
            _gx_widget_attach((GX_WIDGET *)root, target_1);

            if (target_2)
            {
                _gx_widget_attach((GX_WIDGET *)root, target_2);
            }

            /* and show the animation root window to make everything visible */
            _gx_widget_show((GX_WIDGET *)root);
            _gx_canvas_drawing_initiate(animation -> gx_animation_canvas, (GX_WIDGET *)root, &root -> gx_widget_size);
            _gx_widget_children_draw((GX_WIDGET *)root);
            _gx_canvas_drawing_complete(animation -> gx_animation_canvas, GX_FALSE);

            /* set the initial alpha and make our canvas visible */
            _gx_canvas_alpha_set(animation -> gx_animation_canvas, info -> gx_animation_start_alpha);
            _gx_canvas_show(animation -> gx_animation_canvas);
        }
    }

    /* Return completion status code. */
    return(GX_SUCCESS);
}

