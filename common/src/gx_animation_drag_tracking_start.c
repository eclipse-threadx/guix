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


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_animation_drag_tracking_start                   PORTABLE C      */
/*                                                           6.1          */
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
/*                                                                        */
/**************************************************************************/
UINT  _gx_animation_drag_tracking_start(GX_ANIMATION *animation, GX_POINT penpos)
{
GX_ANIMATION_INFO *info;
GX_RECTANGLE       size;
INT                width;
GX_WIDGET         *target_1;
GX_WIDGET         *target_2;
INT                index;
INT                current_pos;

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

    if ((index >= 0) && (index < animation -> gx_animation_slide_screen_list_size))
    {
        animation -> gx_animation_slide_target_index_2 = (GX_VALUE)(index);

        target_1 = info -> gx_animation_slide_screen_list[animation -> gx_animation_slide_target_index_1];
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

        /* Attach the second target to animation parent.  */
        _gx_widget_attach(info -> gx_animation_parent, target_2);
    }
    else
    {
        animation -> gx_animation_slide_target_index_2 = -1;
    }

    /* Return completion status code. */
    return(GX_SUCCESS);
}

