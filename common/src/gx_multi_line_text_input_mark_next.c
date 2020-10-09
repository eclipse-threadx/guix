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
#include "gx_scrollbar.h"
#include "gx_multi_line_text_input.h"
#include "gx_multi_line_text_view.h"
#include "gx_text_input_cursor.h"
#include "gx_utility.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_multi_line_text_input_mark_next                 PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function moves highlight text end mark one character right.    */
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
/*    _gx_text_input_cursor_dirty_rectangle_get                           */
/*                                          Get cursor rectangle          */
/*    _gx_multi_line_text_input_cursor_pos_update                         */
/*                                          Calculate cursor position     */
/*                                            according to insert index   */
/*    _gx_system_dirty_partial_add          Add one dirty area to         */
/*                                            dirty list                  */
/*    _gx_system_dirty_mark                 Mark widget area dirty        */
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
UINT _gx_multi_line_text_input_mark_next(GX_MULTI_LINE_TEXT_INPUT *text_input)
{
GX_RECTANGLE cur_rect;
INT          old_shift;
UINT         glyph_len = 1;
GX_STRING    string;
GX_POINT     start_pos;
GX_POINT     end_pos;
UINT         start_mark = text_input -> gx_multi_line_text_input_start_mark;
UINT         end_mark = text_input -> gx_multi_line_text_input_end_mark;

    if (start_mark == end_mark)
    {
        start_mark = text_input -> gx_multi_line_text_input_text_insert_position;
        end_mark = text_input -> gx_multi_line_text_input_text_insert_position;

        text_input -> gx_multi_line_text_input_start_mark = start_mark;
        text_input -> gx_multi_line_text_input_end_mark = end_mark;
    }

    if (end_mark < text_input -> gx_multi_line_text_view_text.gx_string_length)
    {
#ifdef GX_UTF8_SUPPORT
        string.gx_string_ptr = text_input -> gx_multi_line_text_view_text.gx_string_ptr + end_mark;
        string.gx_string_length = text_input -> gx_multi_line_text_view_text.gx_string_length - end_mark;
        _gx_utility_utf8_string_character_get(&string, GX_NULL, &glyph_len);
#endif
        string.gx_string_ptr = text_input -> gx_multi_line_text_view_text.gx_string_ptr + end_mark;

        if (string.gx_string_ptr[0] == GX_KEY_CARRIAGE_RETURN || string.gx_string_ptr[0] == GX_KEY_LINE_FEED)
        {
            glyph_len = text_input -> gx_multi_line_text_input_new_line_character_size;
        }

        text_input -> gx_multi_line_text_input_end_mark += glyph_len;

        old_shift = text_input -> gx_multi_line_text_view_text_scroll_shift;

        start_pos = text_input -> gx_multi_line_text_input_cursor_instance.gx_text_input_cursor_pos;

        text_input -> gx_multi_line_text_input_text_insert_position = text_input -> gx_multi_line_text_input_end_mark;

        /* Recalculate cursor position according to cursor insert position. */
        _gx_multi_line_text_input_cursor_pos_update(text_input, GX_TRUE);

        end_pos = text_input -> gx_multi_line_text_input_cursor_instance.gx_text_input_cursor_pos;

        /* Dirty mark. */
        if (old_shift == text_input -> gx_multi_line_text_view_text_scroll_shift)
        {
            _gx_multi_line_text_input_text_rectangle_get(text_input, start_pos, end_pos, &cur_rect);

            /* Mark highlight area as dirty. */
            _gx_system_dirty_partial_add((GX_WIDGET *)text_input, &cur_rect);
        }
        else
        {
            _gx_system_dirty_mark((GX_WIDGET *)text_input);
        }
    }
    return GX_SUCCESS;
}

