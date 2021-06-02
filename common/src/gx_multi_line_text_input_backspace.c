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
#include "gx_multi_line_text_input.h"
#include "gx_multi_line_text_view.h"
#include "gx_window.h"
#include "gx_widget.h"
#include "gx_utility.h"
#include "gx_scrollbar.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_multi_line_text_input_backspace                 PORTABLE C      */
/*                                                           6.1.7        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function deletes a character before the cursor.                */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text_input                            Multi line text input widget  */
/*                                            control block               */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_font_get                   Retrieve font                 */
/*    _gx_utility_rectangle_resize          Increase/Shrink rectangle with*/
/*                                            specified value             */
/*    _gx_utility_utf8_string_character_get Parses utf8 string to         */
/*                                            multi-byte glyph            */
/*    _gx_utility_string_length_check       Test string length            */
/*    _gx_window_scrollbar_find             Find scrollbar for a window   */
/*    _gx_window_client_width_get           Get client width              */
/*    _gx_scrollbar_reset                   Reset scrollbar information   */
/*    _gx_multi_line_text_view_line_cache_update                          */
/*                                          Update line cache             */
/*    _gx_multi_line_text_view_display_info_get                           */
/*                                          Get the number of characters  */
/*                                            that a line can display     */
/*    _gx_multi_line_text_input_char_remove Remove a character            */
/*    _gx_multi_line_text_input_cursor_pos_update                         */
/*                                          Update cursor position        */
/*                                            according to insert index   */
/*    _gx_multi_line_text_view_string_total_rows_compute                  */
/*    _gx_system_string_width_get           Get string width              */
/*    _gx_system_dirty_partial_add          Add one dirty area to dirty   */
/*                                            list                        */
/*                                                                        */
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
/*  06-02-2021     Kenneth Maxwell          Modified comment(s),          */
/*                                            removed unused variable     */
/*                                            assignment,                 */
/*                                            resulting in version 6.1.7  */
/*                                                                        */
/**************************************************************************/
UINT _gx_multi_line_text_input_backspace(GX_MULTI_LINE_TEXT_INPUT *text_input)
{
GX_TEXT_INPUT_CURSOR    *cursor_ptr = &text_input -> gx_multi_line_text_input_cursor_instance;
GX_MULTI_LINE_TEXT_VIEW *view = (GX_MULTI_LINE_TEXT_VIEW *)text_input;
GX_FONT                 *font;
GX_RECTANGLE             client;
GX_RECTANGLE             dirty_rect;
UINT                     insert_pos;
INT                      shift;
INT                      last_visible_line;
GX_VALUE                 line_height;
GX_VALUE                 delete_width;
UINT                     old_text_rows;
UINT                     glyph_len = 1;
GX_STRING                string;
GX_SCROLLBAR            *scroll;
UINT                     start_mark = text_input -> gx_multi_line_text_input_start_mark;
UINT                     end_mark = text_input -> gx_multi_line_text_input_end_mark;

    if (end_mark < start_mark)
    {
        return _gx_multi_line_text_input_delete(text_input);
    }

    insert_pos = text_input -> gx_multi_line_text_input_text_insert_position;

    if (insert_pos > 0)
    {
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

            string.gx_string_ptr = &text_input -> gx_multi_line_text_view_text.gx_string_ptr[insert_pos - glyph_len];
        }
        else
        {
            _gx_multi_line_text_input_cursor_visible(text_input);

#ifdef GX_UTF8_SUPPORT
            _gx_utility_utf8_string_backward_character_length_get(&text_input -> gx_multi_line_text_view_text,
                                                                  (INT)(insert_pos - 1), &glyph_len);
#endif

            /* Get the character to be deleted.  */
            string.gx_string_ptr = &text_input -> gx_multi_line_text_view_text.gx_string_ptr[insert_pos - glyph_len];

            if (string.gx_string_ptr[0] == GX_KEY_CARRIAGE_RETURN || string.gx_string_ptr[0] == GX_KEY_LINE_FEED)
            {
                glyph_len = text_input -> gx_multi_line_text_input_new_line_character_size;

                string.gx_string_ptr = &text_input -> gx_multi_line_text_view_text.gx_string_ptr[insert_pos - glyph_len];
            }
        }

        string.gx_string_length = glyph_len;

        /* Get the width of the character to be deleted. */
        _gx_system_string_width_get_ext(font, &string, &delete_width);

        /* Record old shift value.  */
        shift = text_input -> gx_multi_line_text_view_text_scroll_shift;

        /* Remove character before cursor. */
        _gx_multi_line_text_input_char_remove(text_input, insert_pos - glyph_len, glyph_len);

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
                    view -> gx_multi_line_text_view_cache_size)
                {
                    _gx_multi_line_text_view_line_cache_update((GX_MULTI_LINE_TEXT_VIEW *)text_input);
                }
            }
        }

        /* Update text insert position.  */
        text_input -> gx_multi_line_text_input_text_insert_position -= glyph_len;

        client = text_input -> gx_window_client;

        if (text_input -> gx_multi_line_text_view_whitespace)
        {
            /* Offset client bounding box.  */
            _gx_utility_rectangle_resize(&client, (GX_VALUE)(-text_input -> gx_multi_line_text_view_whitespace));
        }

        /* Recalculate cursor position. */
        _gx_multi_line_text_input_cursor_pos_update(text_input, GX_TRUE);

        if (shift == text_input -> gx_multi_line_text_view_text_scroll_shift)
        {

            line_height = (GX_VALUE)(font -> gx_font_line_height + text_input -> gx_multi_line_text_view_line_space);

            if (line_height)
            {
                /* Calculate dirty area. */
                dirty_rect = client;

                dirty_rect.gx_rectangle_top = (GX_VALUE)(cursor_ptr -> gx_text_input_cursor_pos.gx_point_y - (line_height << 1));

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

