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
#include "gx_widget.h"
#include "gx_window.h"
#include "gx_scrollbar.h"
#include "gx_utility.h"
#include "gx_multi_line_text_view.h"
#include "gx_multi_line_text_input.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_multi_line_text_input_cursor_pos_update         PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function gets the position of the cursor according to insert   */
/*      index.                                                            */
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
/*    _gx_widget_font_get                   Retrieve font                 */
/*    _gx_window_scrollbar_find             Find scrollbar for a window   */
/*    _gx_scrollbar_reset                   Reset scrollbar information   */
/*    _gx_system_string_width_get           Get the width of a string     */
/*    _gx_utility_rectangle_resize          Increase/Shrink rectangle by  */
/*                                            specified value             */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    GUIX Intercal Code                                                  */
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

UINT _gx_multi_line_text_input_cursor_pos_update(GX_MULTI_LINE_TEXT_INPUT *text_input, GX_BOOL make_cursor_visible)
{
GX_TEXT_INPUT_CURSOR    *cursor_ptr = &text_input -> gx_multi_line_text_input_cursor_instance;
GX_MULTI_LINE_TEXT_VIEW *view = (GX_MULTI_LINE_TEXT_VIEW *)text_input;
GX_FONT                 *font;
GX_VALUE                 line_height;
UINT                     insert_pos;
UINT                     cur_line;
USHORT                   cache_index;
GX_CHAR                 *string_buffer;
GX_POINT                 cur_pos;
GX_RECTANGLE             client;
INT                      y_pos;
GX_VALUE                 text_width;
GX_VALUE                 space_width;
GX_VALUE                 client_width;
UINT                     start_index;
UINT                     end_index;
GX_STRING                string;
GX_MULTI_LINE_TEXT_INFO  text_info;

    _gx_widget_font_get((GX_WIDGET *)text_input, text_input -> gx_multi_line_text_view_font_id, &font);

    if (!font)
    {
        return GX_FAILURE;
    }

    line_height = (GX_VALUE)(font -> gx_font_line_height +
                             text_input -> gx_multi_line_text_view_line_space);
    text_width = 0;

    if (!line_height)
    {
        return GX_FAILURE;
    }

    /* Get the y position where the text displayed from. */
    client = text_input -> gx_window_client;

    if (text_input -> gx_multi_line_text_view_whitespace)
    {
        /* Offset client bounding box.  */
        _gx_utility_rectangle_resize(&client, (GX_VALUE)(-text_input -> gx_multi_line_text_view_whitespace));
    }

    client_width = (GX_VALUE)(client.gx_rectangle_right - client.gx_rectangle_left + 1);

    string_buffer = (GX_CHAR *)text_input -> gx_multi_line_text_view_text.gx_string_ptr;

    insert_pos = text_input -> gx_multi_line_text_input_text_insert_position;

    cache_index = 0;
    cur_line = 0;

    if ((cache_index < view -> gx_multi_line_text_view_cache_size) &&
        (view -> gx_multi_line_text_view_line_index[0] > insert_pos))
    {
        /* Insert position is before cached lines. Search from start. */
        start_index = 0;
        end_index = text_input -> gx_multi_line_text_view_text.gx_string_length;
        cur_line = 1;

        while (1)
        {
            /* Calculate some information used to draw the text. */
            _gx_multi_line_text_view_display_info_get((GX_MULTI_LINE_TEXT_VIEW *)text_input, start_index, end_index, &text_info, (GX_VALUE)(client_width - 2));

            start_index += text_info.gx_text_display_number;

            if (start_index >= insert_pos)
            {
                end_index = start_index;
                start_index -= text_info.gx_text_display_number;
                break;
            }

            cur_line++;
        }

        if (insert_pos)
        {
            if ((string_buffer[insert_pos - 1] == GX_KEY_CARRIAGE_RETURN) ||
                (string_buffer[insert_pos - 1] == GX_KEY_LINE_FEED))
            {
                cur_line++;
            }
        }
    }
    else
    {
        /* Find the cursor line. */
        while (cache_index < view -> gx_multi_line_text_view_cache_size)
        {
            if (view -> gx_multi_line_text_view_line_index[cache_index] >= insert_pos)
            {
                break;
            }
            cache_index++;
        }

        if (cache_index < view -> gx_multi_line_text_view_cache_size)
        {
            /* Found insert position. */
            if (insert_pos &&
                (string_buffer[insert_pos - 1] != GX_KEY_CARRIAGE_RETURN) &&
                (string_buffer[insert_pos - 1] != GX_KEY_LINE_FEED) && cache_index)
            {
                cache_index--;
            }

            /* cursor line is started from 1. */
            cur_line = cache_index + view -> gx_multi_line_text_view_first_cache_line + 1;

            /* get x coordinate of cursor postition */
            start_index = text_input -> gx_multi_line_text_view_line_index[cache_index];

            if (cache_index >= view -> gx_multi_line_text_view_cache_size - 1)
            {
                end_index = text_input -> gx_multi_line_text_view_text.gx_string_length;
            }
            else
            {
                end_index = text_input -> gx_multi_line_text_view_line_index[cache_index + 1];
            }
        }
        else if (view -> gx_multi_line_text_view_cache_size)
        {
            /* Insert position is after cached lines, search from last cached line. */
            cache_index = (USHORT)(view -> gx_multi_line_text_view_cache_size - 1);
            cur_line = cache_index + view -> gx_multi_line_text_view_first_cache_line + 1;
            start_index = text_input -> gx_multi_line_text_view_line_index[cache_index];
            end_index = text_input -> gx_multi_line_text_view_text.gx_string_length;

            while (start_index < end_index)
            {
                /* Calculate some information used to draw the text. */
                _gx_multi_line_text_view_display_info_get((GX_MULTI_LINE_TEXT_VIEW *)text_input, start_index, end_index, &text_info, (GX_VALUE)(client_width - 2));

                start_index += text_info.gx_text_display_number;

                if (start_index >= insert_pos)
                {
                    end_index = start_index;
                    start_index -= text_info.gx_text_display_number;
                    break;
                }

                cur_line++;
            }

            if ((string_buffer[insert_pos - 1] == GX_KEY_CARRIAGE_RETURN) ||
                (string_buffer[insert_pos - 1] == GX_KEY_LINE_FEED))
            {
                cur_line++;
            }
        }
        else
        {
            /* No character. */
            start_index = 0;
            end_index = 0;
            cur_line = 1;
        }
    }

    string.gx_string_ptr = " ";
    string.gx_string_length = 1;
    _gx_system_string_width_get_ext(font, &string, &space_width);

    switch (text_input -> gx_widget_style & GX_STYLE_TEXT_ALIGNMENT_MASK)
    {
    case GX_STYLE_TEXT_RIGHT:
        string.gx_string_ptr = (GX_CHAR *)text_input -> gx_multi_line_text_view_text.gx_string_ptr + start_index;
        string.gx_string_length = end_index - start_index;
        _gx_system_string_width_get_ext(font, &string, &text_width);
        while (text_width > (client.gx_rectangle_right - client.gx_rectangle_left - 2))
        {
            text_width = (GX_VALUE)(text_width - space_width);
        }
        cur_pos.gx_point_x = (GX_VALUE)(client.gx_rectangle_right - 1);
        cur_pos.gx_point_x = (GX_VALUE)(cur_pos.gx_point_x - text_width);
        break;

    case GX_STYLE_TEXT_LEFT:
        cur_pos.gx_point_x = (GX_VALUE)(client.gx_rectangle_left + 1);
        break;

    case GX_STYLE_TEXT_CENTER:
    default:
        string.gx_string_ptr = (GX_CHAR *)text_input -> gx_multi_line_text_view_text.gx_string_ptr + start_index;
        string.gx_string_length = end_index - start_index;
        _gx_system_string_width_get_ext(font, &string, &text_width);
        while (text_width > (client.gx_rectangle_right - client.gx_rectangle_left - 2))
        {
            text_width = (GX_VALUE)(text_width - space_width);
        }

        cur_pos.gx_point_x = (GX_VALUE)(client.gx_rectangle_left + ((client_width - text_width) / 2));
        break;
    }

    if (insert_pos &&
        (string_buffer[insert_pos - 1] != GX_KEY_CARRIAGE_RETURN) &&
        (string_buffer[insert_pos - 1] != GX_KEY_LINE_FEED))
    {
        string.gx_string_ptr = string_buffer + start_index;
        string.gx_string_length = insert_pos - start_index;
        _gx_system_string_width_get_ext(font, &string, &text_width);
        cur_pos.gx_point_x = (GX_VALUE)(cur_pos.gx_point_x + text_width);
    }

    /* make cursor position inside client area. */
    while (cur_pos.gx_point_x > client.gx_rectangle_right - 1)
    {
        cur_pos.gx_point_x = (GX_VALUE)(cur_pos.gx_point_x - space_width);
    }

    /* Update cursor line. */
    text_input -> gx_multi_line_text_input_text_cursor_line = cur_line;

    /* get y coordinate of cursor postition */
    y_pos = (INT)client.gx_rectangle_top + text_input -> gx_multi_line_text_view_text_scroll_shift;

    cur_pos.gx_point_y = (GX_VALUE)(y_pos + (INT)(cur_line - 1) * line_height);
    cur_pos.gx_point_y = (GX_VALUE)(cur_pos.gx_point_y + (line_height >> 1));

    cursor_ptr -> gx_text_input_cursor_pos = cur_pos;

    if (make_cursor_visible)
    {
        _gx_multi_line_text_input_cursor_visible(text_input);
    }
    return GX_SUCCESS;
}

