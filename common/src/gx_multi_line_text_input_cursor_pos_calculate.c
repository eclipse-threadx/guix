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
#include "gx_window.h"
#include "gx_widget.h"
#include "gx_scrollbar.h"
#include "gx_multi_line_text_view.h"
#include "gx_multi_line_text_input.h"
#include "gx_utility.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_multi_line_text_input_cursor_pos_calculate      PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function gets the position of the cursor according to click    */
/*    position.                                                           */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text_input                            Multi line text input         */
/*                                            control block               */
/*    click_pos                             Click down position           */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_wiget_font_get                    Retrieve font                 */
/*    _gx_window_client_width_get           Get the client width          */
/*    _gx_window_scrollbar_find             Find scrollbar for a window   */
/*    _gx_scrollbar_reset                   Reset scrollbar information   */
/*    _gx_utility_rectangle_resize          Increase/Shrink rectangle by  */
/*                                            specified value             */
/*    _gx_utility_utf8_string_character_get Parses utf8 string to         */
/*                                            multi-byte glyph            */
/*    _gx_system_string_width_get           Get the width of a string     */
/*    _gx_multi_line_text_view_string_total_rows_compute                  */
/*                                          Calculate total rows of input */
/*                                            string                      */
/*    _gx_multi_line_text_view_line_cache_update                          */
/*                                          Update line cache             */
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
/*                                                                        */
/**************************************************************************/

UINT _gx_multi_line_text_input_cursor_pos_calculate(GX_MULTI_LINE_TEXT_INPUT *text_input, GX_POINT click_pos)
{
GX_TEXT_INPUT_CURSOR *cursor_ptr = &text_input -> gx_multi_line_text_input_cursor_instance;
GX_FONT              *font;
INT                   line_height;
GX_RECTANGLE          client;
INT                   y_pos;
GX_POINT              cursor_pos;
UINT                  total_rows;
INT                   shift;
UINT                  cursor_line = 0;
UINT                  index = 0;
UINT                  end_index;
GX_STRING             string;
GX_VALUE              char_width;
GX_SCROLLBAR         *scroll;
UINT                  glyph_len = 1;
GX_VALUE              text_width;
GX_VALUE              client_width;
GX_VALUE              space_width;

    _gx_widget_font_get((GX_WIDGET *)text_input, text_input -> gx_multi_line_text_view_font_id, &font);

    if (!font)
    {
        return GX_FAILURE;
    }

    line_height = font -> gx_font_line_height + text_input -> gx_multi_line_text_view_line_space;

    if (!line_height)
    {
        return GX_FAILURE;
    }

    /* Initialize the x, y position where the text displayed from. */
    client = text_input -> gx_window_client;

    /* Offset client area by length of whitespace.  */
    _gx_utility_rectangle_resize(&client, (GX_VALUE)(-text_input -> gx_multi_line_text_view_whitespace));

    y_pos = (INT)client.gx_rectangle_top + text_input -> gx_multi_line_text_view_text_scroll_shift;

    total_rows = text_input -> gx_multi_line_text_view_text_total_rows;

    if ((click_pos.gx_point_y < y_pos + line_height) || (total_rows == 0))
    {
        cursor_line = 1;
    }
    else
    {
        cursor_line = (UINT)((click_pos.gx_point_y - y_pos) / line_height + 1);

        if (cursor_line > total_rows)
        {
            cursor_line = total_rows;
        }
    }

    /* get y coordinate of cursor postition */
    shift = 0;
    cursor_pos.gx_point_y = (GX_VALUE)(y_pos + (INT)(cursor_line - 1) * line_height);

    if (cursor_pos.gx_point_y + line_height - 1 > client.gx_rectangle_bottom)
    {
        shift = client.gx_rectangle_bottom - (cursor_pos.gx_point_y + line_height - 1);
    }
    else if (cursor_pos.gx_point_y < client.gx_rectangle_top)
    {
        shift = client.gx_rectangle_top - cursor_pos.gx_point_y;
    }

    if (shift)
    {
        text_input -> gx_multi_line_text_view_text_scroll_shift += shift;
        cursor_pos.gx_point_y = (GX_VALUE)(cursor_pos.gx_point_y + shift);

        _gx_window_scrollbar_find((GX_WINDOW *)text_input, GX_TYPE_VERTICAL_SCROLL, &scroll);
        if (scroll)
        {
            /* Reset scrollbar.  */
            _gx_scrollbar_reset(scroll, GX_NULL);
        }
        else
        {

            if (text_input -> gx_multi_line_text_view_text_total_rows >
                text_input -> gx_multi_line_text_view_cache_size)
            {
                _gx_multi_line_text_view_line_cache_update((GX_MULTI_LINE_TEXT_VIEW *)text_input);
            }
        }
    }

    /* get x coordinate of cursor position. */
    cursor_pos.gx_point_x = (GX_VALUE)(client.gx_rectangle_left + 1);
    text_input -> gx_multi_line_text_input_text_cursor_line = cursor_line;

    cursor_line = (UINT)(cursor_line - 1);
    cursor_line = (UINT)(cursor_line - text_input -> gx_multi_line_text_view_first_cache_line);

    index = (text_input -> gx_multi_line_text_view_line_index[cursor_line]);

    if (cursor_line >= (UINT)(text_input -> gx_multi_line_text_view_cache_size - 1))
    {
        end_index = text_input -> gx_multi_line_text_view_text.gx_string_length;
    }
    else
    {
        end_index = text_input -> gx_multi_line_text_view_line_index[cursor_line + 1];
    }

    string.gx_string_ptr = " ";
    string.gx_string_length = 1;

    _gx_system_string_width_get_ext(font, &string, &space_width);

    switch (text_input -> gx_widget_style & GX_STYLE_TEXT_ALIGNMENT_MASK)
    {
    case GX_STYLE_TEXT_RIGHT:
        string.gx_string_ptr = text_input -> gx_multi_line_text_view_text.gx_string_ptr + index;
        string.gx_string_length = end_index - index;
        _gx_system_string_width_get_ext(font, &string, &text_width);
        while (text_width > (client.gx_rectangle_right - client.gx_rectangle_left - 2))
        {
            text_width = (GX_VALUE)(text_width - space_width);
        }
        cursor_pos.gx_point_x = (GX_VALUE)(client.gx_rectangle_right - 1);
        cursor_pos.gx_point_x = (GX_VALUE)(cursor_pos.gx_point_x - text_width);
        break;

    case GX_STYLE_TEXT_LEFT:
        cursor_pos.gx_point_x = (GX_VALUE)(client.gx_rectangle_left + 1);
        break;

    case GX_STYLE_TEXT_CENTER:
    default:
        string.gx_string_ptr = text_input -> gx_multi_line_text_view_text.gx_string_ptr + index;
        string.gx_string_length = end_index - index;
        _gx_system_string_width_get_ext(font, &string, &text_width);
        client_width = (GX_VALUE)(client.gx_rectangle_right - client.gx_rectangle_left + 1);
        while (text_width > (client_width - 3))
        {
            text_width = (GX_VALUE)(text_width - space_width);
        }
        cursor_pos.gx_point_x = (GX_VALUE)(client.gx_rectangle_left + ((client_width - text_width) / 2));
        break;
    }

    while (index < end_index)
    {
        string.gx_string_ptr = text_input -> gx_multi_line_text_view_text.gx_string_ptr + index;
        string.gx_string_length = end_index - index;
        if ((string.gx_string_ptr[0] == GX_KEY_CARRIAGE_RETURN) || (string.gx_string_ptr[0] == GX_KEY_LINE_FEED))
        {
            break;
        }
#ifdef GX_UTF8_SUPPORT
        
        _gx_utility_utf8_string_character_get(&string, GX_NULL, &glyph_len);
#else
        string.gx_string_ptr += glyph_len;
#endif
        string.gx_string_ptr -= glyph_len;
        string.gx_string_length = glyph_len;
        _gx_system_string_width_get_ext(font, &string, &char_width);
        if ((cursor_pos.gx_point_x + char_width / 2) > click_pos.gx_point_x)
        {
            while (cursor_pos.gx_point_x > client.gx_rectangle_right - 1)
            {
                cursor_pos.gx_point_x = (GX_VALUE)(cursor_pos.gx_point_x - space_width);
            }
            break;
        }
        cursor_pos.gx_point_x = (GX_VALUE)(cursor_pos.gx_point_x + char_width);
        index += glyph_len;
    }

    cursor_ptr -> gx_text_input_cursor_pos = cursor_pos;
    cursor_ptr -> gx_text_input_cursor_pos.gx_point_y = (GX_VALUE)(cursor_ptr -> gx_text_input_cursor_pos.gx_point_y + (line_height >> 1));

    /* record text insert position. */
    text_input -> gx_multi_line_text_input_text_insert_position = index;

    return GX_SUCCESS;
}

