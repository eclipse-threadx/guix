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
/**   System Management (System)                                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_canvas.h"
#include "gx_widget.h"
#include "gx_utility.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_system_canvas_draw_partial_canvas               PORTABLE C      */
/*                                                           6.3.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Ting Zhu, Microsoft Corporation                                     */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function draws the dirty area of the specified canvas with     */
/*    partial canvas buffer and toggle the dirty area to the display or   */
/*    a composite canvas.                                                 */
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
/*    _gx_system_dirty_list_trim            Trim dirty area list          */
/*    _gx_canvas_drawing_initiate           Initiate drawing on canvas    */
/*    _gx_widget_children_draw              Draw widgets children         */
/*    [gx_widget_draw_function]             Call widget's drawing function*/
/*    _gx_canvas_drawing_complete           Complete drawing on canvas    */
/*    _gx_system_canvas_toggle              Toggle the frame buffer       */
/*    _gx_utility_rectangle_shift           Shift a rectangle             */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_system_canvas_refresh                                           */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  10-31-2023     Ting Zhu                 Initial Version 6.3.0         */
/*                                                                        */
/**************************************************************************/
#ifdef GX_ENABLE_CANVAS_PARTIAL_FRAME_BUFFER
static UINT _gx_system_canvas_draw_partial_canvas(GX_WINDOW_ROOT *root)
{
UINT           status = GX_SUCCESS;
GX_RECTANGLE   dirty_sum;
GX_RECTANGLE   dirty_frame;
UINT           index;
GX_WIDGET     *drawit;
GX_DIRTY_AREA *dirty_list_entry;
GX_CANVAS     *canvas = root -> gx_window_root_canvas;
GX_VALUE       dirty_width;
GX_VALUE       dirty_height;

#if defined(GX_CANVAS_REFRESH_DIRECTION_HORIZONTAL) || defined(GX_CANVAS_REFRESH_DIRECTION_VERTICAL)
GX_RECTANGLE dirty_mask;

    if (_gx_system_dirty_list_trim(&dirty_sum, root) == GX_FALSE)
    {
        return GX_SUCCESS;
    }

    dirty_mask = dirty_sum;
#endif

    /* Refresh canvas in the dirty area.  */
    if (canvas -> gx_canvas_draw_count > 0)
    {
        _gx_system_dirty_partial_add((GX_WIDGET *)root, &canvas -> gx_canvas_dirty_area);
        canvas -> gx_canvas_draw_count = 0;
    }

#if defined(GX_CANVAS_REFRESH_DIRECTION_HORIZONTAL)
    dirty_height = (GX_VALUE)(dirty_sum.gx_rectangle_bottom - dirty_sum.gx_rectangle_top + 1);
    dirty_width = (GX_VALUE)(canvas -> gx_canvas_memory_size / canvas -> gx_canvas_display -> gx_display_driver_row_pitch_get((USHORT)dirty_height));
    dirty_width = (GX_VALUE)(dirty_width & 0xFFFC);
    dirty_mask.gx_rectangle_right = (GX_VALUE)(dirty_mask.gx_rectangle_left + dirty_width - 1);

    while (dirty_mask.gx_rectangle_left <= dirty_sum.gx_rectangle_right)
    {
        if (dirty_mask.gx_rectangle_right > dirty_sum.gx_rectangle_right)
        {
            dirty_mask.gx_rectangle_right = dirty_sum.gx_rectangle_right;
        }
#elif defined(GX_CANVAS_REFRESH_DIRECTION_VERTICAL)
    dirty_width = (GX_VALUE)(dirty_sum.gx_rectangle_right - dirty_sum.gx_rectangle_left + 1);
    dirty_height = (GX_VALUE)(canvas -> gx_canvas_memory_size / canvas -> gx_canvas_display -> gx_display_driver_row_pitch_get((USHORT)((dirty_width + 3) & 0xFFFC)));
    dirty_mask.gx_rectangle_bottom = (GX_VALUE)(dirty_mask.gx_rectangle_top + dirty_height - 1);

    while (dirty_mask.gx_rectangle_top <= dirty_sum.gx_rectangle_bottom)
    {
        if (dirty_mask.gx_rectangle_bottom > dirty_sum.gx_rectangle_bottom)
        {
            dirty_mask.gx_rectangle_bottom = dirty_sum.gx_rectangle_bottom;
        }
#endif

        /* Initialize dirty area pointers.  */
        dirty_list_entry = canvas -> gx_canvas_dirty_list;

        /* Loop through dirty areas to redraw as needed.  */
        for (index = 0; index < canvas -> gx_canvas_dirty_count; index++)
        {
            /* Pickup widget associated with dirty area.  */
            drawit = dirty_list_entry -> gx_dirty_area_widget;

            /* Is the widget pointer valid?  */

            if (drawit && (drawit -> gx_widget_status & GX_STATUS_VISIBLE))
            {
                /* if the object is transparent, we need to draw the parent.
                    This should not happen, because dircty_partial_add checks
                    for transparency, but just for safety we test again here  */

                if (drawit -> gx_widget_status & GX_STATUS_TRANSPARENT ||
                    drawit -> gx_widget_style & (GX_STYLE_BORDER_RAISED | GX_STYLE_BORDER_RECESSED))
                {
                    while (drawit -> gx_widget_parent)
                    {
                        drawit = drawit -> gx_widget_parent;

                        if (!(drawit -> gx_widget_status & GX_STATUS_TRANSPARENT))
                        {
                            /* we need to start drawing at this non-transparent
                                background widget */

                            drawit -> gx_widget_status |= GX_STATUS_DIRTY;
                            break;
                        }
                    }
                }

#if defined(GX_CANVAS_REFRESH_DIRECTION_HORIZONTAL) || defined(GX_CANVAS_REFRESH_DIRECTION_VERTICAL)
                if (_gx_utility_rectangle_overlap_detect(&dirty_list_entry -> gx_dirty_area_rectangle, &dirty_mask, &dirty_frame) == GX_TRUE &&
                    _gx_utility_rectangle_overlap_detect(&dirty_frame, &drawit -> gx_widget_clip, &dirty_frame) == GX_TRUE)
                {
#else
                dirty_sum = dirty_list_entry -> gx_dirty_area_rectangle;
                if (_gx_utility_rectangle_overlap_detect(&dirty_sum, &drawit -> gx_widget_clip, &dirty_sum) == GX_TRUE)
                {
                    dirty_frame = dirty_sum;

                    /* Split dirty area into small pieces.  */
                    dirty_width = (GX_VALUE)(dirty_frame.gx_rectangle_right - dirty_frame.gx_rectangle_left + 1);
                    dirty_height = (GX_VALUE)(canvas -> gx_canvas_memory_size / canvas -> gx_canvas_display -> gx_display_driver_row_pitch_get((USHORT)((dirty_width + 3) & 0xFFFC)));
                    dirty_frame.gx_rectangle_bottom = (GX_VALUE)(dirty_frame.gx_rectangle_top + dirty_height - 1);

                    while (dirty_frame.gx_rectangle_top <= dirty_sum.gx_rectangle_bottom)
                    {
                        if (dirty_frame.gx_rectangle_bottom > dirty_sum.gx_rectangle_bottom)
                        {
                            dirty_frame.gx_rectangle_bottom = dirty_sum.gx_rectangle_bottom;
                        }
#endif

                    /* Initiate drawing on this canvas.  */
                    status = _gx_canvas_drawing_initiate(canvas, drawit, &dirty_frame);

                    if (status == GX_NO_VIEWS)
                    {
                        /* If we are attempting to draw the root window and it has no views,
                            just draw the children of the root */

                        if (drawit -> gx_widget_type == GX_TYPE_ROOT_WINDOW)
                        {
                            _gx_widget_children_draw(drawit);
                        }
                    }
                    else
                    {
                        drawit -> gx_widget_draw_function(drawit);
                    }

                    /* Indicate that drawing on this canvas is complete.  */
                    _gx_canvas_drawing_complete(canvas, GX_TRUE);

#if !defined(GX_CANVAS_REFRESH_DIRECTION_HORIZONTAL) && !defined(GX_CANVAS_REFRESH_DIRECTION_VERTICAL)
                    _gx_utility_rectangle_shift(&dirty_frame, 0, dirty_height);
                }
#endif
            }
        }

        /* Move to the next dirty area.  */
        dirty_list_entry++;
    }

#ifdef GX_CANVAS_REFRESH_DIRECTION_HORIZONTAL
    _gx_utility_rectangle_shift(&dirty_mask, dirty_width, 0);
}
#elif defined(GX_CANVAS_REFRESH_DIRECTION_VERTICAL)
    _gx_utility_rectangle_shift(&dirty_mask, 0, dirty_height);
}
#endif

canvas -> gx_canvas_dirty_count = 0;

return GX_SUCCESS;
}
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_system_canvas_refresh                           PORTABLE C      */
/*                                                           6.3.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function refreshes the screen(s) of GUIX.                      */
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
/*    GX_ENTER_CRITICAL                     Enter GUIX critical section   */
/*    _gx_system_views_update               Update views                  */
/*    _gx_system_dirty_list_trim            Trim dirty area list          */
/*    _gx_canvas_drawing_initiate           Initiate drawing on canvas    */
/*    _gx_widget_children_draw              Draw widgets children         */
/*    [gx_widget_draw_function]             Call widget's drawing function*/
/*    _gx_canvas_drawing_complete           Complete drawing on canvas    */
/*    _gx_canvas_composite_create           Create a canvas composite     */
/*    [gx_display_driver_buffer_toggle]     Toggle the frame buffer       */
/*    GX_EXIT_CRITICAL                      Exit GUIX critical section    */
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
/*  10-31-2023     Ting Zhu                 Modified comment(s),          */
/*                                            added partial canvas buffer */
/*                                            support,                    */
/*                                            resulting in version 6.3.0  */
/*                                                                        */
/**************************************************************************/
UINT  _gx_system_canvas_refresh(VOID)
{
UINT            status = GX_SUCCESS;
UINT            index;
GX_DIRTY_AREA  *dirty;
GX_WIDGET      *drawit;
GX_CANVAS      *canvas;
GX_RECTANGLE    dirty_sum;
GX_WINDOW_ROOT *root;

    /* Determine if there are no canvas or screens created.  */
    if (!_gx_system_canvas_created_list)
    {
        return GX_INVALID_CANVAS;
    }

    /* lock access to GUIX */
    GX_ENTER_CRITICAL

    canvas = GX_NULL;

    /* check to see if viewports need to be recalculated */
    root = _gx_system_root_window_created_list;

    while (root)
    {
        if (!(root -> gx_widget_status & GX_STATUS_VISIBLE))
        {
            root = (GX_WINDOW_ROOT *)root -> gx_widget_next;
            continue;
        }

        if (root -> gx_window_root_views_changed)
        {
            _gx_system_views_update(root);
        }

        /* pick up the canvas pointer */
        canvas = root -> gx_window_root_canvas;

#ifdef GX_ENABLE_CANVAS_PARTIAL_FRAME_BUFFER
        if (canvas -> gx_canvas_status & GX_CANVAS_PARTIAL_FRAME_BUFFER)
        {
            _gx_system_canvas_draw_partial_canvas(root);
            root = (GX_WINDOW_ROOT *)root -> gx_widget_next;
            continue;
        }
#endif

        /* Trim any redundant dirty areas prior to doing the update.  */
        if (_gx_system_dirty_list_trim(&dirty_sum, root))
        {
            /* Initiate drawing on this canvas.  */
            _gx_canvas_drawing_initiate(canvas, (GX_WIDGET *)root, &dirty_sum);

            /* Initialize dirty area pointers.  */
            dirty = canvas -> gx_canvas_dirty_list;

            /* Loop through dirty areas to redraw as needed.  */
            for (index = 0; index < canvas -> gx_canvas_dirty_count; index++)
            {
                /* Pickup widget associated with dirty area.  */
                drawit = dirty -> gx_dirty_area_widget;

                /* Is the widget pointer valid?  */

                if (drawit)
                {
                    /* if the object is transparent, we need to draw the parent.
                       This should not happen, because dircty_partial_add checks
                       for transparency, but just for safety we test again here  */

                    if (drawit -> gx_widget_status & GX_STATUS_TRANSPARENT)
                    {
                        while (drawit -> gx_widget_parent)
                        {
                            drawit = drawit -> gx_widget_parent;

                            if (!(drawit -> gx_widget_status & GX_STATUS_TRANSPARENT))
                            {
                                /* we need to start drawing at this non-transparent
                                   background widget */

                                drawit -> gx_widget_status |= GX_STATUS_DIRTY;
                                break;
                            }
                        }
                    }

                    /* Initiate drawing on this canvas.  */

                    status = _gx_canvas_drawing_initiate(canvas, drawit,
                                                         &dirty -> gx_dirty_area_rectangle);

                    if (status == GX_NO_VIEWS)
                    {
                        /* If we are attempting to draw the root window and it has no views,
                           just draw the children of the root */

                        if (drawit -> gx_widget_type == GX_TYPE_ROOT_WINDOW)
                        {
                            _gx_widget_children_draw(drawit);
                        }
                    }
                    else
                    {
                        drawit -> gx_widget_draw_function(drawit);
                    }

                    /* Indicate that drawing on this canvas is complete.  */
                    _gx_canvas_drawing_complete(canvas, GX_FALSE);
                }
                /* Move to the next dirty area.  */
                dirty++;
            }
            /* Indicate that drawing on this canvas is complete.  */
            _gx_canvas_drawing_complete(canvas, GX_FALSE);
        }
        root = (GX_WINDOW_ROOT *)root -> gx_widget_next;
    }

    /* if compositing is enabled, create the composite here, rather than
       in the driver. Most of the code is common */

    if (_gx_canvas_composite_create(&canvas))
    {
        if (canvas -> gx_canvas_draw_count > 0)
        {
            canvas -> gx_canvas_display -> gx_display_driver_buffer_toggle(canvas, &canvas -> gx_canvas_dirty_area);
            canvas -> gx_canvas_draw_count = 0;
        }
    }
    else
    {
        /* Reset the dirty area counts  */
        root = _gx_system_root_window_created_list;

        while (root)
        {
            canvas = root -> gx_window_root_canvas;

            /* reset the canvas dirty counter */
            canvas -> gx_canvas_dirty_count = 0;

            if (root -> gx_widget_status & GX_STATUS_VISIBLE)
            {
                if (canvas -> gx_canvas_draw_count > 0)
                {
                    /* Call the driver buffer toggle function for this canvas.  */

                    if ((canvas -> gx_canvas_status & GX_CANVAS_MANAGED_VISIBLE) == GX_CANVAS_MANAGED_VISIBLE)
                    {
                        canvas -> gx_canvas_display -> gx_display_driver_buffer_toggle(canvas, &canvas -> gx_canvas_dirty_area);
                    }
                }
            }
            /* reset the canvas draw counter */
            canvas -> gx_canvas_draw_count = 0;
            root = (GX_WINDOW_ROOT *)root -> gx_widget_next;
        }
    }

    /* unlock access to GUIX */
    GX_EXIT_CRITICAL

    return status;
}

