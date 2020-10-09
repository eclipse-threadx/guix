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
/**   Progress Bar Management (Progress Bar)                              */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE

/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_context.h"
#include "gx_widget.h"
#include "gx_canvas.h"
#include "gx_utility.h"
#include "gx_progress_bar.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_progress_bar_background_draw                    PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service draws the background of the specified progress bar.    */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    progress_bar                          Progress Bar control block    */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_border_draw                Draw the widget border        */
/*    _gx_context_fill_color_set            Set fill color                */
/*    _gx_context_pixelmap_get              Retrieve pixelmap by ID       */
/*    _gx_widget_client_get                 Get widget client area        */
/*    _gx_widget_width_get                  Get widget width              */
/*    _gx_widget_height_get                 Get widget height             */
/*    _gx_canvas_rectangle_draw             Draw a rectangle on canvas    */
/*    _gx_canvas_pixelmap_tile              Tile a pixelmap on cavnas     */
/*    _gx_utility_rectangle_shift           Shift a rectangle by specified*/
/*                                            x,y values                  */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*    GUIX Internal Code                                                  */
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
VOID  _gx_progress_bar_background_draw(GX_PROGRESS_BAR *progress_bar)
{
GX_PIXELMAP   *fill_map;
GX_RECTANGLE   fill_rect;
INT            fill_width;
INT            fill_height;
GX_VALUE       segment_width;
GX_VALUE       segment_height;
INT            range;
INT            i;
INT            min_value;
INT            max_value;
INT            cur_value;
GX_VALUE       widget_width;
GX_VALUE       widget_height;
INT            segment_number;
INT            total_segment;
GX_RESOURCE_ID fill_color;

    if (progress_bar -> gx_widget_style & GX_STYLE_ENABLED)
    {
        fill_color = progress_bar -> gx_widget_normal_fill_color;
    }
    else
    {
        fill_color = progress_bar -> gx_widget_disabled_fill_color;
    }

    /* Draw progress bar background and border.  */
    _gx_widget_border_draw((GX_WIDGET *)progress_bar, GX_COLOR_ID_DEFAULT_BORDER,
                           fill_color, fill_color, GX_TRUE);

    min_value = progress_bar -> gx_progress_bar_info.gx_progress_bar_info_min_val;
    max_value = progress_bar -> gx_progress_bar_info.gx_progress_bar_info_max_val;
    range = max_value - min_value;

    if (range <= 0)
    {
        return;
    }

    _gx_widget_client_get((GX_WIDGET *)progress_bar, -1, &fill_rect);
    fill_width = fill_rect.gx_rectangle_right - fill_rect.gx_rectangle_left + 1;
    fill_height = fill_rect.gx_rectangle_bottom - fill_rect.gx_rectangle_top + 1;

    cur_value = progress_bar -> gx_progress_bar_info.gx_progress_bar_info_current_val;

    if (progress_bar -> gx_progress_bar_info.gx_progress_bar_fill_pixelmap)
    {
        _gx_context_pixelmap_get(progress_bar -> gx_progress_bar_info.gx_progress_bar_fill_pixelmap, &fill_map);

        if (fill_map)
        {
            if (progress_bar -> gx_widget_style & GX_STYLE_PROGRESS_VERTICAL)
            {
                fill_rect.gx_rectangle_top = (GX_VALUE)(fill_rect.gx_rectangle_bottom -
                                                        ((cur_value - min_value) * fill_height / range) + 1);
            }
            else
            {
                fill_rect.gx_rectangle_right = (GX_VALUE)(fill_rect.gx_rectangle_left +
                                                          ((cur_value - min_value) * fill_width / range) - 1);
            }
            _gx_canvas_pixelmap_tile(&fill_rect, fill_map);
        }
    }
    else
    {
        /* Pickup color to use */
        _gx_context_fill_color_set(progress_bar -> gx_widget_selected_fill_color);

        if (progress_bar -> gx_widget_style & GX_STYLE_PROGRESS_SEGMENTED_FILL)
        {
            if (progress_bar -> gx_widget_style & GX_STYLE_PROGRESS_VERTICAL)
            {
                _gx_widget_width_get((GX_WIDGET *)progress_bar, &widget_width);
                segment_height = widget_width >> 1;
                total_segment = (fill_height - GX_SEGMENT_INTERVAL) / (segment_height + GX_SEGMENT_INTERVAL);
                segment_number = (cur_value - min_value) * total_segment / range;
                fill_rect.gx_rectangle_left = (GX_VALUE)(fill_rect.gx_rectangle_left + GX_SEGMENT_BORDER_INTERVAL);
                fill_rect.gx_rectangle_right = (GX_VALUE)(fill_rect.gx_rectangle_right - GX_SEGMENT_BORDER_INTERVAL);
                fill_rect.gx_rectangle_top = (GX_VALUE)(fill_rect.gx_rectangle_bottom - segment_height + 1);

                for (i = 1; i <= segment_number; i++)
                {
                    _gx_canvas_rectangle_draw(&fill_rect);
                    _gx_utility_rectangle_shift(&fill_rect, 0, (GX_VALUE)(-(segment_height + GX_SEGMENT_INTERVAL)));
                }
            }
            else
            {
                _gx_widget_height_get((GX_WIDGET *)progress_bar, &widget_height);
                segment_width = widget_height >> 1;
                total_segment = (fill_width - GX_SEGMENT_INTERVAL) / (segment_width + GX_SEGMENT_INTERVAL);
                segment_number = (cur_value - min_value) * total_segment / range;

                fill_rect.gx_rectangle_top = (GX_VALUE)(fill_rect.gx_rectangle_top + GX_SEGMENT_BORDER_INTERVAL);
                fill_rect.gx_rectangle_bottom = (GX_VALUE)(fill_rect.gx_rectangle_bottom - GX_SEGMENT_BORDER_INTERVAL);
                fill_rect.gx_rectangle_right = (GX_VALUE)(fill_rect.gx_rectangle_left + segment_width - 1);

                for (i = 1; i <= segment_number; i++)
                {
                    _gx_canvas_rectangle_draw(&fill_rect);
                    _gx_utility_rectangle_shift(&fill_rect, (GX_VALUE)(segment_width + GX_SEGMENT_INTERVAL), 0);
                }
            }
        }
        else
        {
            if (progress_bar -> gx_widget_style & GX_STYLE_PROGRESS_VERTICAL)
            {
                fill_rect.gx_rectangle_top = (GX_VALUE)(fill_rect.gx_rectangle_bottom -
                                                        ((cur_value - min_value) * fill_height / range) + 1);
            }
            else
            {
                fill_rect.gx_rectangle_right = (GX_VALUE)(fill_rect.gx_rectangle_left +
                                                          ((cur_value - min_value) * fill_width / range) - 1);
            }
            _gx_canvas_rectangle_draw(&fill_rect);
        }
    }
}

