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
#include "gx_utility.h"
#include "gx_multi_line_text_input.h"
#include "gx_multi_line_text_view.h"
#include "gx_utility.h"
#include "gx_scrollbar.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_multi_line_text_input_delete                    PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function deletes a character after the cursor.                 */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text_input                        Multi line text input             */
/*                                        control block                   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_font_get                   Retrieve font                 */
/*    _gx_window_client_width_get           Get the client width          */
/*    _gx_window_scrollbar_find             Find scrollbar for a window   */
/*    _gx_scrollbar_reset                   Reset scrollbar information   */
/*    _gx_multi_line_text_input_char_remove Remove characters from input  */
/*                                            string                      */
/*    _gx_multi_line_text_view_display_info_get                           */
/*                                          Get the number of characters  */
/*                                            that a line can display     */
/*    _gx_multi_line_text_view_string_total_rows_compute                  */
/*                                          Calculate total rows of input */
/*                                            string                      */
/*    _gx_multi_line_text_view_line_cache_update                          */
/*                                          Update line cache             */
/*    _gx_multi_line_text_input_cursor_pos_update                         */
/*                                          Update cursor position        */
/*                                            according to insert position*/
/*    _gx_system_dirty_partial_add          Add one dirty area to dirty   */
/*                                            list                        */
/*    _gx_utility_utf8_string_character_get Parses utf8 string to         */
/*                                            multi-byte glyph            */
/*    _gx_utility_string_length_check       Test string length            */
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
UINT _gx_multi_line_text_input_delete(GX_MULTI_LINE_TEXT_INPUT *text_input)
{
GX_TEXT_INPUT_CURSOR    *cursor_ptr = &text_input -> gx_multi_line_text_input_cursor_instance;
GX_MULTI_LINE_TEXT_VIEW *view = (GX_MULTI_LINE_TEXT_VIEW *)text_input;
UINT                     insert_pos;
UINT                     string_size;
GX_RECTANGLE             client;
GX_RECTANGLE             dirty_rect;
GX_FONT                 *font;
GX_VALUE                 line_height;
UINT                     old_text_rows;
INT                      shift;
INT                      last_visible_line;
UINT                     glyph_len = 1;
GX_SCROLLBAR            *scroll;
GX_STRING                string;
UINT                     start_mark = text_input -> gx_multi_line_text_input_start_mark;
UINT                     end_mark = text_input -> gx_multi_line_text_input_end_mark;

    if (end_mark > start_mark)
    {
        return _gx_multi_line_text_input_backspace(text_input);
    }

    insert_pos =  text_input -> gx_multi_line_text_input_text_insert_position;
    string_size = text_input -> gx_multi_line_text_view_text.gx_string_length;
    if (insert_pos < string_size)
    {
        /* Pickup text font.  */
        _gx_widget_font_get((GX_WIDGET *)text_input, text_input -> gx_multi_line_text_view_font_id, &font);

        if (!font)
        {
            return GX_FAILURE;
        }

        if (start_mark != end_mark)
        {
            glyph_len = (UINT)GX_ABS((INT)start_mark - (INT)end_mark);
            text_input -> gx_multi_line_text_input_start_mark = 0;
            text_input -> gx_multi_line_text_input_end_mark = 0;
        }
        else
        {
#ifdef GX_UTF8_SUPPORT
            string.gx_string_ptr = text_input -> gx_multi_line_text_view_text.gx_string_ptr + insert_pos;
            string.gx_string_length = text_input -> gx_multi_line_text_view_text.gx_string_length - insert_pos;
            _gx_utility_utf8_string_character_get(&string, GX_NULL, &glyph_len);
#endif

            string.gx_string_ptr = text_input -> gx_multi_line_text_view_text.gx_string_ptr + insert_pos;
            if (string.gx_string_ptr[0] == GX_KEY_CARRIAGE_RETURN || string.gx_string_ptr[0] == GX_KEY_LINE_FEED)
            {
                glyph_len = text_input -> gx_multi_line_text_input_new_line_character_size;
            }
        }

        /* Get scroll shift. */
        shift = (GX_VALUE)(text_input -> gx_multi_line_text_view_text_scroll_shift);

        /* Remove character after cursor.  */
        _gx_multi_line_text_input_char_remove(text_input, insert_pos, glyph_len);

        /* Save old text rows.  */
        old_text_rows = text_input -> gx_multi_line_text_view_text_total_rows;

        /* Calculate new text rows.  */
        _gx_multi_line_text_view_string_total_rows_compute((GX_MULTI_LINE_TEXT_VIEW *)text_input);

        if (old_text_rows != text_input -> gx_multi_line_text_view_text_total_rows)
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

        /* Recalculate cursor position. */
        _gx_multi_line_text_input_cursor_pos_update(text_input, GX_TRUE);

        if (shift == text_input -> gx_multi_line_text_view_text_scroll_shift)
        {
            /* Get text height. */
            line_height = (GX_VALUE)(font -> gx_font_line_height +
                                     text_input -> gx_multi_line_text_view_line_space);

            if (line_height)
            {
                client = text_input -> gx_window_client;

                /* Offset client area by length of whitespace.  */
                _gx_utility_rectangle_resize(&client, (GX_VALUE)(-text_input -> gx_multi_line_text_view_whitespace));

                dirty_rect = client;

                /* Calculate dirty area. */
                dirty_rect.gx_rectangle_top = (GX_VALUE)(cursor_ptr -> gx_text_input_cursor_pos.gx_point_y - line_height - (line_height >> 1));

                /* Calculate last visible line. */
                last_visible_line = (-text_input -> gx_multi_line_text_view_text_scroll_shift) / line_height;
                last_visible_line = last_visible_line + (INT)view -> gx_multi_line_text_view_text_visible_rows + 1;

                if (last_visible_line > (INT)text_input -> gx_multi_line_text_view_text_total_rows)
                {
                    last_visible_line = (INT)text_input -> gx_multi_line_text_view_text_total_rows;
                }

                shift = (last_visible_line + (INT)old_text_rows - (INT)text_input -> gx_multi_line_text_view_text_total_rows) * line_height + shift;
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
    }

    return GX_SUCCESS;
}

