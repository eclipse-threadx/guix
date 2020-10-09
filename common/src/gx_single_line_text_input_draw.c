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
/**   Text Input Management (Single Line Text Input)                      */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_utility.h"
#include "gx_canvas.h"
#include "gx_context.h"
#include "gx_widget.h"
#include "gx_single_line_text_input.h"
#include "gx_text_input_cursor.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_single_line_text_input_draw                     PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service draws a text input widget. This service is normally    */
/*    called internally during canvas refresh, but can also be called from*/
/*    custom text input drawing functions.                                */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text_input                            Single-line text input widget */
/*                                            control block.              */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_background_draw            Draw widget background        */
/*    _gx_widget_border_draw                Draw widget border            */
/*    _gx_widget_border_width_get           Get widget border width       */
/*    _gx_widget_client_get                 Get widget client rectangle   */
/*   _gx_widget_children_draw               Draw widget children          */
/*    _gx_context_line_color_set            Set the line color for the    */
/*                                            context                     */
/*    _gx_context_font_set                  Set the font in the context   */
/*    _gx_context_brush_width_set           Set the width of brush        */
/*    _gx_canvas_text_draw                  Draw the text                 */
/*    _gx_canvas_drawing_initiate           Initiate drawing on specified */
/*                                            canvas                      */
/*    _gx_canvas_drawing_complete           Complete drawing on specified */
/*                                            canvas                      */
/*    _gx_text_input_cursor_draw            Draw a text input cursor      */
/*    _gx_utility_rectangle_overlap_detect  Detect overlap of the         */
/*                                            supplied rectangles         */
/*    _gx_single_line_text_input_draw_position_get                        */
/*                                          Get text draw start position  */
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
VOID  _gx_single_line_text_input_draw(GX_SINGLE_LINE_TEXT_INPUT *text_input)
{
UINT                  status = GX_SUCCESS;
GX_WIDGET            *widget = (GX_WIDGET *)text_input;
GX_TEXT_INPUT_CURSOR *cursor_ptr = &text_input -> gx_single_line_text_input_cursor_instance;
GX_RESOURCE_ID        fill_color;
GX_RESOURCE_ID        text_color;
GX_VALUE              border_width;
GX_VALUE              client_height;
GX_VALUE              x_pos;
GX_VALUE              y_pos;
GX_RECTANGLE          client;
GX_RECTANGLE          overlap;
GX_CANVAS            *canvas;
UINT                  start_mark = text_input -> gx_single_line_text_input_start_mark;
UINT                  end_mark = text_input -> gx_single_line_text_input_end_mark;
GX_VALUE              text_width;
GX_BRUSH             *brush;
GX_CHAR              *input_buffer = text_input -> gx_single_line_text_input_buffer;
GX_STRING             string;

    /* Draw text input background.  */
    if (text_input -> gx_widget_style & GX_STYLE_ENABLED)
    {
        if (text_input -> gx_widget_style & GX_STYLE_TEXT_INPUT_READONLY)
        {
            fill_color = text_input -> gx_single_line_text_input_readonly_fill_color;
            text_color = text_input -> gx_single_line_text_input_readonly_text_color;
        }
        else
        {
            fill_color = text_input -> gx_widget_normal_fill_color;
            text_color = text_input -> gx_prompt_normal_text_color;
        }
    }
    else
    {
        fill_color = text_input -> gx_widget_disabled_fill_color;
        text_color = text_input -> gx_prompt_disabled_text_color;
    }

    _gx_widget_border_draw(widget, GX_COLOR_ID_WINDOW_BORDER, fill_color, fill_color, GX_TRUE);

    _gx_context_font_set(text_input -> gx_prompt_font_id);

    /* Calculate text draw position. */
    status = _gx_single_line_text_input_draw_position_get(text_input, &x_pos, &y_pos);

    if (status != GX_SUCCESS)
    {
        return;
    }

    /* Pickup widget width. */
    _gx_widget_border_width_get(widget, &border_width);

    /* Get client rectangle. */
    _gx_widget_client_get(widget, border_width, &client);

    _gx_context_line_color_set(text_color);

    /* pick up current canvas */
    canvas = _gx_system_current_draw_context -> gx_draw_context_canvas;
    _gx_utility_rectangle_overlap_detect(&_gx_system_current_draw_context -> gx_draw_context_dirty, &client, &overlap);
    _gx_canvas_drawing_initiate(canvas, widget, &overlap);

    /* Draw the cursor. */
    if ((start_mark == end_mark) &&
        (text_input -> gx_widget_status & GX_STATUS_CURSOR_SHOW) &&
        (text_input -> gx_widget_status & GX_STATUS_CURSOR_DRAW))
    {
        client_height = (GX_VALUE)(client.gx_rectangle_bottom - client.gx_rectangle_top + 1);

        if (!(cursor_ptr -> gx_text_input_cursor_flags & GX_CURSOR_USE_CUSTOM_HEIGHT))
        {
            cursor_ptr -> gx_text_input_cursor_height = (GX_VALUE)(client_height - 4);
        }
        cursor_ptr -> gx_text_input_cursor_pos.gx_point_y = (GX_VALUE)(client.gx_rectangle_top + (client_height >> 1));

        _gx_text_input_cursor_draw(cursor_ptr);
    }

    /* Is there a string?  */
    if (input_buffer && (*input_buffer))
    {
        /* Draw the text.  */
        if (start_mark == end_mark)
        {
            string.gx_string_ptr = input_buffer;
            string.gx_string_length = text_input -> gx_single_line_text_input_string_size;
            _gx_canvas_text_draw_ext(x_pos, y_pos, &string);
        }
        else
        {
            _gx_context_brush_get(&brush);

            if (start_mark > end_mark)
            {
                GX_SWAP_VALS(start_mark, end_mark);
            }

            if (start_mark > 0)
            {
                /* Draw text[0:start_mark - 1] with normak text color. */
                string.gx_string_ptr = input_buffer;
                string.gx_string_length = start_mark;
                _gx_system_string_width_get_ext(brush -> gx_brush_font, &string, &text_width);
                _gx_canvas_text_draw_ext(x_pos, y_pos, &string);
                x_pos = (GX_VALUE)(x_pos + text_width);
            }

            /* Draw text[start_mark:end_mark - 1] with highlight text color. */
            _gx_context_line_color_set(text_input -> gx_prompt_selected_text_color);
            _gx_context_fill_color_set(text_input -> gx_widget_selected_fill_color);
            _gx_context_brush_width_set(0);

            string.gx_string_ptr = input_buffer + start_mark;
            string.gx_string_length = (UINT)(end_mark - start_mark);
            _gx_system_string_width_get_ext(brush -> gx_brush_font, &string, &text_width);

            client_height = (GX_VALUE)(client.gx_rectangle_bottom - client.gx_rectangle_top + 1);

            if (!(cursor_ptr -> gx_text_input_cursor_flags & GX_CURSOR_USE_CUSTOM_HEIGHT))
            {
                cursor_ptr -> gx_text_input_cursor_height = (GX_VALUE)(client_height - 4);
            }

            client.gx_rectangle_left = x_pos;
            client.gx_rectangle_right = (GX_VALUE)(x_pos + text_width - 1);
            client.gx_rectangle_top = (GX_VALUE)(client.gx_rectangle_top + ((client_height - cursor_ptr -> gx_text_input_cursor_height) >> 1));
            client.gx_rectangle_bottom = (GX_VALUE)(client.gx_rectangle_top + cursor_ptr -> gx_text_input_cursor_height - 1);

            _gx_canvas_rectangle_draw(&client);
            _gx_canvas_text_draw_ext(x_pos, y_pos, &string);
            x_pos = (GX_VALUE)(x_pos + text_width);

            if (end_mark < text_input -> gx_single_line_text_input_string_size)
            {
                /* Draw text[end_mark:] with normal text color. */
                _gx_context_line_color_set(text_color);

                string.gx_string_ptr = input_buffer + end_mark;
                string.gx_string_length = text_input -> gx_single_line_text_input_string_size - end_mark;
                _gx_canvas_text_draw_ext(x_pos, y_pos, &string);
            }
        }
    }

    _gx_canvas_drawing_complete(canvas, GX_FALSE);

    _gx_widget_children_draw(widget);
}

