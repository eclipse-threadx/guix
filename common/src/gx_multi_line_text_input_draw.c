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
/**   Multi Line Text Input Management (Multi Line Text Input)            */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_multi_line_text_view.h"
#include "gx_text_input_cursor.h"
#include "gx_multi_line_text_input.h"
#include "gx_context.h"
#include "gx_window.h"
#include "gx_widget.h"
#include "gx_utility.h"
#include "gx_canvas.h"
#include "gx_scrollbar.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_multi_line_text_input_draw                      PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function draws the multi-line text input widget, which is a    */
/*    special type of widget.                                             */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text_input                            Multi line text input         */
/*                                            control block               */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_context_font_get                  Retrieve font                 */
/*    _gx_multi_line_text_view_draw         Draw the text view area       */
/*    _gx_text_input_cursor_draw            Draw the cursor               */
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
VOID  _gx_multi_line_text_input_draw(GX_MULTI_LINE_TEXT_INPUT *input)
{
GX_TEXT_INPUT_CURSOR *cursor_ptr = &input -> gx_multi_line_text_input_cursor_instance;
GX_VALUE              line_height = 0;
GX_FONT              *font;
GX_RESOURCE_ID        fill_color;
GX_RESOURCE_ID        text_color;
UINT                  line_start_mark;
UINT                  line_end_mark;
UINT                  end_mark;

UINT                  index;
GX_STRING             private_string;
GX_STRING             draw_string;
INT                   x_pos;
INT                   y_pos;
GX_RECTANGLE          client;
GX_RECTANGLE          draw_area;
GX_CANVAS            *canvas;
UINT                  first_visible_line;
UINT                  last_visible_line;
UINT                  line_start_index;
UINT                  line_end_index;
UINT                  line_cache_start;
GX_VALUE              text_width;
GX_VALUE              client_width;
GX_SCROLLBAR         *scroll;
GX_VALUE              space_width;

    if (input -> gx_widget_style & GX_STYLE_ENABLED)
    {
        if (input -> gx_widget_style & GX_STYLE_TEXT_INPUT_READONLY)
        {
            fill_color = input -> gx_multi_line_text_input_readonly_fill_color;
            text_color = input -> gx_multi_line_text_input_readonly_text_color;
        }
        else
        {
            fill_color = input -> gx_widget_normal_fill_color;
            text_color = input -> gx_multi_line_text_view_normal_text_color;
        }
    }
    else
    {
        fill_color = input -> gx_widget_disabled_fill_color;
        text_color = input -> gx_multi_line_text_view_disabled_text_color;
    }

    /* Draw background. */
    _gx_window_border_draw((GX_WINDOW *)input, fill_color);

    if (input -> gx_multi_line_text_view_line_index_old)
    {
        /* Get visible rows. */
        _gx_multi_line_text_view_visible_rows_compute((GX_MULTI_LINE_TEXT_VIEW *)input);

        /* Calculate text total rows. */
        _gx_multi_line_text_view_string_total_rows_compute((GX_MULTI_LINE_TEXT_VIEW *)input);

        _gx_window_scrollbar_find((GX_WINDOW *)input, GX_TYPE_VERTICAL_SCROLL, &scroll);
        if (scroll)
        {
            /* Reset scrollbar.  */
            _gx_scrollbar_reset(scroll, GX_NULL);
        }
        else
        {

            if (input -> gx_multi_line_text_view_text_total_rows >
                input -> gx_multi_line_text_view_cache_size)
            {
                /* Update line cache. */
                _gx_multi_line_text_view_line_cache_update((GX_MULTI_LINE_TEXT_VIEW *)input);
            }
        }

        _gx_multi_line_text_input_cursor_pos_update(input, GX_TRUE);
    }

    if ((input -> gx_multi_line_text_input_start_mark == input -> gx_multi_line_text_input_end_mark) ||
        !(input -> gx_widget_style & GX_STYLE_ENABLED))
    {
        /* Draw text. */
        _gx_multi_line_text_view_text_draw((GX_MULTI_LINE_TEXT_VIEW *)input, text_color);

        /* Draw the cursor. */
        if ((input -> gx_widget_status & GX_STATUS_CURSOR_SHOW) &&
            (input -> gx_widget_status & GX_STATUS_CURSOR_DRAW))
        {
            _gx_context_font_get(input -> gx_multi_line_text_view_font_id, &font);

            if (font)
            {
                line_height = (GX_VALUE)(font -> gx_font_line_height +
                                         input -> gx_multi_line_text_view_line_space);
            }

            if (!(cursor_ptr -> gx_text_input_cursor_flags & GX_CURSOR_USE_CUSTOM_HEIGHT))
            {
                cursor_ptr -> gx_text_input_cursor_height = line_height;
            }

            _gx_text_input_cursor_draw(cursor_ptr);
        }
    }
    else
    {
        line_start_mark = input -> gx_multi_line_text_input_start_mark;
        end_mark = input -> gx_multi_line_text_input_end_mark;

        if (line_start_mark > end_mark)
        {
            GX_SWAP_VALS(line_start_mark, end_mark);
        }

        _gx_context_font_get(input -> gx_multi_line_text_view_font_id, &font);
        _gx_context_font_set(input -> gx_multi_line_text_view_font_id);
        _gx_context_brush_width_set(0);

        /* Is there a string and font?  */
        if ((input -> gx_multi_line_text_view_text.gx_string_length) && (font))
        {
            /* pick up current canvas */
            canvas = _gx_system_current_draw_context -> gx_draw_context_canvas;

            /* Pick up client area.  */
            client = input -> gx_window_client;

            /* Offset client area by the size of whitespace.  */
            _gx_utility_rectangle_resize(&client, (GX_VALUE)(-input -> gx_multi_line_text_view_whitespace));

            _gx_utility_rectangle_overlap_detect(&_gx_system_current_draw_context -> gx_draw_context_dirty, &client, &draw_area);
            _gx_canvas_drawing_initiate(canvas, (GX_WIDGET *)input, &draw_area);

            /* Pickup text height. */
            line_height = (GX_VALUE)(font -> gx_font_line_height + input -> gx_multi_line_text_view_line_space);

            if (line_height)
            {

                first_visible_line = ((UINT)(-input -> gx_multi_line_text_view_text_scroll_shift)) / (UINT)line_height;
                last_visible_line = first_visible_line + input -> gx_multi_line_text_view_text_visible_rows;

                if (last_visible_line > input -> gx_multi_line_text_view_text_total_rows - 1)
                {
                    last_visible_line = input -> gx_multi_line_text_view_text_total_rows - 1;
                }

                /* Compute the start displaying position of pixels in x direction and y direction. */
                y_pos = client.gx_rectangle_top;
                y_pos += input -> gx_multi_line_text_view_text_scroll_shift;
                y_pos += (input -> gx_multi_line_text_view_line_space >> 1);
                y_pos += (INT)first_visible_line * line_height;

                _gx_system_private_string_get(&input -> gx_multi_line_text_view_text, &private_string, input -> gx_widget_style);

                draw_string.gx_string_ptr = " ";
                draw_string.gx_string_length = 1;
                _gx_system_string_width_get_ext(font, &draw_string, &space_width);

                for (index = first_visible_line; index <= last_visible_line; index++)
                {
                    line_cache_start = input -> gx_multi_line_text_view_first_cache_line;

                    line_start_index = input -> gx_multi_line_text_view_line_index[index - line_cache_start];

                    if ((INT)(index - line_cache_start) >= (INT)(input -> gx_multi_line_text_view_cache_size - 1))
                    {
                        line_end_index = input -> gx_multi_line_text_view_text.gx_string_length;
                    }
                    else
                    {
                        line_end_index = input -> gx_multi_line_text_view_line_index[index - line_cache_start + 1];
                    }

                    switch (input -> gx_widget_style & GX_STYLE_TEXT_ALIGNMENT_MASK)
                    {
                    case GX_STYLE_TEXT_RIGHT:
                        draw_string.gx_string_ptr = private_string.gx_string_ptr + line_start_index;
                        draw_string.gx_string_length = line_end_index - line_start_index;
                        _gx_system_string_width_get_ext(font, &draw_string, &text_width);
                        while (text_width > (client.gx_rectangle_right - client.gx_rectangle_left - 2))
                        {
                            text_width = (GX_VALUE)(text_width - space_width);
                        }
                        x_pos = client.gx_rectangle_right - 1;
                        x_pos = (GX_VALUE)(x_pos - text_width);
                        break;
                    case GX_STYLE_TEXT_LEFT:
                        x_pos = client.gx_rectangle_left + 1;
                        break;
                    case GX_STYLE_TEXT_CENTER:
                    default:
                        draw_string.gx_string_ptr = private_string.gx_string_ptr + line_start_index;
                        draw_string.gx_string_length = line_end_index - line_start_index;
                        _gx_system_string_width_get_ext(font, &draw_string, &text_width);
                        client_width = (GX_VALUE)(client.gx_rectangle_right - client.gx_rectangle_left + 1);

                        while (text_width > (client_width - 3))
                        {
                            text_width = (GX_VALUE)(text_width - space_width);
                        }

                        x_pos = (GX_VALUE)(client.gx_rectangle_left + ((client_width - text_width) / 2));
                        break;
                    }

                    if ((line_start_mark < line_end_index) && (end_mark > line_start_index))
                    {
                        if (line_start_mark < line_start_index)
                        {
                            line_start_mark = line_start_index;
                        }

                        if (line_start_mark > line_start_index)
                        {
                            /* Draw text[line_start_index : start_mark - 1] with normal text color. */
                            _gx_context_line_color_set(text_color);
                            draw_string.gx_string_ptr = private_string.gx_string_ptr + line_start_index;
                            draw_string.gx_string_length = line_start_mark - line_start_index;
                            _gx_system_string_width_get_ext(font, &draw_string, &text_width);
                            _gx_canvas_text_draw_ext((GX_VALUE)x_pos, (GX_VALUE)y_pos, &draw_string);
                            x_pos += text_width;
                        }

                        if (end_mark < line_end_index)
                        {
                            line_end_mark = end_mark;
                        }
                        else
                        {
                            line_end_mark = line_end_index;
                        }

                        /* Draw text[start_mark: end_mark - 1] with hightlight text color. */
                        _gx_context_line_color_set(input -> gx_multi_line_text_view_selected_text_color);
                        _gx_context_fill_color_set(input -> gx_widget_selected_fill_color);

                        draw_string.gx_string_ptr = private_string.gx_string_ptr + line_start_mark;
                        draw_string.gx_string_length = line_end_mark - line_start_mark;
                        _gx_system_string_width_get_ext(font, &draw_string, &text_width);

                        draw_area.gx_rectangle_left = (GX_VALUE)x_pos;
                        draw_area.gx_rectangle_right = (GX_VALUE)(x_pos + text_width - 1);
                        draw_area.gx_rectangle_top = (GX_VALUE)(y_pos - (input -> gx_multi_line_text_view_line_space >> 1));
                        draw_area.gx_rectangle_bottom = (GX_VALUE)(draw_area.gx_rectangle_top + line_height - 1);

                        while (draw_area.gx_rectangle_right > client.gx_rectangle_right - 1)
                        {
                            draw_area.gx_rectangle_right = (GX_VALUE)(draw_area.gx_rectangle_right - space_width);
                        }

                        _gx_canvas_rectangle_draw(&draw_area);

                        draw_string.gx_string_ptr = private_string.gx_string_ptr + line_start_mark;
                        draw_string.gx_string_length = line_end_mark - line_start_mark;
                        _gx_canvas_text_draw_ext((GX_VALUE)x_pos, (GX_VALUE)y_pos, &draw_string);

                        x_pos += text_width;

                        /* Draw text[end_mark : line_end_index] width normal text color. */
                        if (line_end_mark < line_end_index)
                        {
                            _gx_context_line_color_set(text_color);
                            draw_string.gx_string_ptr = private_string.gx_string_ptr + line_end_mark;
                            draw_string.gx_string_length = line_end_index - line_end_mark;
                            _gx_canvas_text_draw_ext((GX_VALUE)x_pos, (GX_VALUE)y_pos, &draw_string);
                        }

                        if (end_mark > line_end_index)
                        {
                            line_start_mark = line_end_index;
                        }
                    }
                    else
                    {
                        _gx_context_line_color_set(text_color);

                        /* Draw the text. */
                        draw_string.gx_string_ptr = private_string.gx_string_ptr + line_start_index;
                        draw_string.gx_string_length = line_end_index - line_start_index;
                        _gx_canvas_text_draw_ext((GX_VALUE)x_pos, (GX_VALUE)y_pos, &draw_string);
                    }

                    y_pos += line_height;
                }
            }

            _gx_canvas_drawing_complete(canvas, GX_FALSE);
        }
    }

    /* Draw widget's children.  */
    _gx_widget_children_draw((GX_WIDGET *)input);
}

