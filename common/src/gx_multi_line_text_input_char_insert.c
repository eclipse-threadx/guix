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
#include "gx_utility.h"
#include "gx_scrollbar.h"
#include "gx_multi_line_text_input.h"
#include "gx_multi_line_text_view.h"
#include "gx_window.h"
#include "gx_widget.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_multi_line_text_input_char_insert               PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION (deprecated)                                              */
/*                                                                        */
/*    This function inserts a character into the input buffer.            */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text_input                            Multi line text input widget  */
/*                                            control block               */
/*    str                                   Utf8 string to be inserted    */
/*    str_size                              Inserted string size in bytes */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_multi_line_text_input_char_insert_ext                           */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_multi_line_text_input_keydown_process                           */
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
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gx_multi_line_text_input_char_insert(GX_MULTI_LINE_TEXT_INPUT *text_input, GX_UBYTE *str, UINT str_size)
{
UINT      status;
GX_STRING string;

    string.gx_string_ptr = (GX_CHAR *)str;
    string.gx_string_length = str_size;
    status = _gx_multi_line_text_input_char_insert_ext(text_input, &string);

    return status;
}
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_multi_line_text_input_char_insert_ext           PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function inserts a character into the input buffer.            */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text_input                            Multi line text input widget  */
/*                                            control block               */
/*    str                                   Utf8 string to be inserted    */
/*    str_size                              Inserted string size in bytes */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    memmove                               Move block of memory          */
/*    _gx_widget_font_get                   Retrieve font                 */
/*    _gx_window_scrollbar_find             Find scrollbar for a window   */
/*    _gx_scrollbar_reset                   Reset scrollbar information   */
/*    _gx_multi_line_text_view_string_total_rows_compute                  */
/*                                          Calculate total rows          */
/*    _gx_multi_line_text_view_line_cache_update                          */
/*                                          Update line cache             */
/*    _gx_multi_line_text_input_cursor_pos_update                         */
/*                                          Update cursor position        */
/*                                            according to insert index   */
/*    _gx_utility_rectangle_resize          Increase/Shrink rectangle by  */
/*                                            specified value             */
/*    _gx_utility_string_length_check       Test string length            */
/*    _gx_system_dirty_partial_add          Add one dirty area to dirty   */
/*                                            list                        */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_multi_line_text_input_keydown_process                           */
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
UINT _gx_multi_line_text_input_char_insert_ext(GX_MULTI_LINE_TEXT_INPUT *text_input, GX_CONST GX_STRING *str)
{
GX_TEXT_INPUT_CURSOR *cursor_ptr = &text_input -> gx_multi_line_text_input_cursor_instance;
UINT                  string_size;
UINT                  buffer_size;
GX_CHAR              *input_buffer;
GX_UBYTE             *insert_char;
UINT                  index;
INT                   shift;
INT                   last_visible_line;
GX_RECTANGLE          client;
GX_RECTANGLE          dirty_rect;
GX_FONT              *font;
GX_VALUE              line_height;
GX_VALUE              old_cur_y;
GX_SCROLLBAR         *scroll;
UINT                  text_rows;
UINT                  glyph_len;
UINT                  handled_size;
GX_STRING             insert_str;

    if (text_input -> gx_multi_line_text_input_start_mark != text_input -> gx_multi_line_text_input_end_mark)
    {
        if (text_input -> gx_multi_line_text_input_end_mark < text_input -> gx_multi_line_text_input_start_mark)
        {
            _gx_multi_line_text_input_delete(text_input);
        }
        else
        {
            _gx_multi_line_text_input_backspace(text_input);
        }
    }

    string_size = text_input -> gx_multi_line_text_view_text.gx_string_length;
    buffer_size = text_input -> gx_multi_line_text_input_buffer_size;
    insert_str = *str;

    _gx_widget_font_get((GX_WIDGET *)text_input, text_input -> gx_multi_line_text_view_font_id, &font);

    if (!font)
    {
        return GX_FAILURE;
    }

    /* Check if the insert string size is bigger than the remaining buffer size.  */
    if (buffer_size < string_size + insert_str.gx_string_length + 1)
    {
        return GX_FAILURE;
    }

    /* Insert a character. */

    input_buffer = (GX_CHAR *)text_input -> gx_multi_line_text_view_text.gx_string_ptr;
    glyph_len = 1;

    while (insert_str.gx_string_length)
    {
        if (insert_str.gx_string_ptr[0] == GX_KEY_CARRIAGE_RETURN || insert_str.gx_string_ptr[0] == GX_KEY_LINE_FEED)
        {
            /* If the inserted character is a new line character,
               exchange it with current used new line character. */
            insert_char = text_input -> gx_multi_line_text_input_new_line_character;
            glyph_len = text_input -> gx_multi_line_text_input_new_line_character_size;

            if (insert_str.gx_string_ptr[0] == GX_KEY_CARRIAGE_RETURN)
            {
                if ((insert_str.gx_string_length > 1) && insert_str.gx_string_ptr[1] == GX_KEY_LINE_FEED)
                {
                    handled_size = 2;
                }
                else
                {
                    handled_size = 1;
                }
            }
            else
            {
                handled_size = 1;
            }

            insert_str.gx_string_ptr += handled_size;
            insert_str.gx_string_length -= handled_size;
        }
        else
        {
            insert_char = (GX_UBYTE *)insert_str.gx_string_ptr;
#if defined GX_UTF8_SUPPORT
            _gx_utility_utf8_string_character_get(&insert_str, GX_NULL, &glyph_len);
#else
            glyph_len = 1;
            insert_str.gx_string_ptr++;
            insert_str.gx_string_length--;
#endif
        }

        string_size = text_input -> gx_multi_line_text_view_text.gx_string_length;

        if (buffer_size < string_size + glyph_len + 1)
        {
            break;
        }
        index = text_input -> gx_multi_line_text_input_text_insert_position;
        memmove(input_buffer + index + glyph_len, input_buffer + index, string_size - index + 1);
        memmove(input_buffer + index, insert_char, glyph_len);

        /* Update insert position and string size. */
        text_input -> gx_multi_line_text_input_text_insert_position += glyph_len;
        text_input -> gx_multi_line_text_view_text.gx_string_length += glyph_len;
    }

    /* Save old text rows.  */
    text_rows = text_input -> gx_multi_line_text_view_text_total_rows;

    /* Calculate new text rows. */
    _gx_multi_line_text_view_string_total_rows_compute((GX_MULTI_LINE_TEXT_VIEW *)text_input);

    if (text_rows != text_input -> gx_multi_line_text_view_text_total_rows)
    {
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

    /* Save old shift value. */
    shift = text_input -> gx_multi_line_text_view_text_scroll_shift;

    /* Record old y coordinate of cursor position. */
    old_cur_y = cursor_ptr -> gx_text_input_cursor_pos.gx_point_y;

    /* Update cursor position. */
    _gx_multi_line_text_input_cursor_pos_update(text_input, GX_TRUE);

    /* Set the text modified flag to GX_TRUE. */
    text_input -> gx_multi_line_text_input_text_was_modified = GX_TRUE;

    if (shift == text_input -> gx_multi_line_text_view_text_scroll_shift)
    {
        line_height = (GX_VALUE)(font -> gx_font_line_height +
                                 text_input -> gx_multi_line_text_view_line_space);

        if (line_height)
        {
            /* Calculate dirty area. */
            client = text_input -> gx_window_client;

            /* Offset client area by length of whitespace.  */
            _gx_utility_rectangle_resize(&client, (GX_VALUE)(-text_input -> gx_multi_line_text_view_whitespace));

            dirty_rect = client;

            /* Calculate dirty area. */
            dirty_rect.gx_rectangle_top = (GX_VALUE)(old_cur_y - line_height - (line_height >> 1));

            /* Calculate last visible line. */
            last_visible_line = (-text_input -> gx_multi_line_text_view_text_scroll_shift) / line_height;
            last_visible_line = last_visible_line + (INT)text_input -> gx_multi_line_text_view_text_visible_rows + 1;

            if (last_visible_line > (INT)text_input -> gx_multi_line_text_view_text_total_rows)
            {
                last_visible_line = (INT)text_input -> gx_multi_line_text_view_text_total_rows;
            }

            shift = last_visible_line * line_height + shift;
            shift += client.gx_rectangle_top;

            if (shift < dirty_rect.gx_rectangle_bottom)
            {
                dirty_rect.gx_rectangle_bottom = (GX_VALUE)shift;
            }

            _gx_system_dirty_partial_add((GX_WIDGET *)text_input, &dirty_rect);
        }
    }
    else
    {
        _gx_system_dirty_mark((GX_WIDGET *)text_input);
    }

    return GX_SUCCESS;
}

