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
#include "gx_context.h"
#include "gx_system.h"
#include "gx_widget.h"
#include "gx_single_line_text_input.h"
#include "gx_text_input_cursor.h"
#include "gx_utility.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_single_line_text_input_left_arrow               PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service moves the text input cursor one character position to  */
/*      the left.                                                         */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text_input                            Single-line text input widget */
/*                                            control block               */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_utf8_string_character_get Parse utf8 string to          */
/*                                            multi-byte glyph            */
/*    _gx_widget_border_width_get           Get the widget border width   */
/*    _gx_widget_client_get                 Get widget client rectangle   */
/*    _gx_widget_font_get                   Get font by specified ID      */
/*    _gx_system_string_width_get           Get the width of a string     */
/*    _gx_system_dirty_mark                 Mark the area of the widget   */
/*                                            as dirty                    */
/*    _gx_system_dirty_partial_add          Mark the partial area of a    */
/*                                            widget as dirty             */
/*    _gx_text_input_cursor_dirty_rectangle_get                           */
/*                                          Get cursor rectangle          */
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
UINT _gx_single_line_text_input_left_arrow(GX_SINGLE_LINE_TEXT_INPUT *text_input)
{
GX_TEXT_INPUT_CURSOR *cursor_ptr = &text_input -> gx_single_line_text_input_cursor_instance;
UINT                  insert_pos;
GX_RECTANGLE          client;
GX_RECTANGLE          cursor_rect;
GX_VALUE              old_cursor_pos;
GX_VALUE              cursor_pos;
GX_VALUE              width;
GX_FONT              *gx_font;
UINT                  glyph_len = 1;
UINT                  start_mark = text_input -> gx_single_line_text_input_start_mark;
UINT                  end_mark = text_input -> gx_single_line_text_input_end_mark;
GX_BOOL               mark_all = GX_FALSE;
GX_STRING             string;

    string.gx_string_ptr = text_input -> gx_single_line_text_input_buffer;
    string.gx_string_length = text_input -> gx_single_line_text_input_string_size;


    /* Move the cursor to the left by a character width.  */

    /* Pick up widget border width. */
    _gx_widget_border_width_get((GX_WIDGET *)text_input, &width);

    /* Get widget client rectangle. */
    _gx_widget_client_get((GX_WIDGET *)text_input, width, &client);

    cursor_pos = cursor_ptr -> gx_text_input_cursor_pos.gx_point_x;
    old_cursor_pos = cursor_pos;

    if (start_mark != end_mark)
    {
        text_input -> gx_single_line_text_input_start_mark = 0;
        text_input -> gx_single_line_text_input_end_mark = 0;

        if (end_mark < start_mark)
        {
            if (((cursor_pos > client.gx_rectangle_left) &&
                 (cursor_pos < client.gx_rectangle_right)) ||
                ((text_input -> gx_widget_style & GX_STYLE_TEXT_ALIGNMENT_MASK) == GX_STYLE_TEXT_CENTER))
            {
                /* No need to update cursor position, just need mark
                    cursor and highlight area as dirty. */
                _gx_text_input_cursor_dirty_rectangle_get(cursor_ptr, &cursor_rect);

                _gx_single_line_text_input_text_rectangle_get(text_input, (INT)(start_mark - end_mark), &client);

                /* Combine cursor rect and highlight area. */
                _gx_utility_rectangle_combine(&client, &cursor_rect);

                return _gx_system_dirty_partial_add((GX_WIDGET *)text_input, &client);
            }
        }
    }

    insert_pos = text_input -> gx_single_line_text_input_insert_pos;

    if ((insert_pos > 0) || (start_mark > end_mark))
    {
        if (end_mark >= start_mark)
        {
            /* Calculate new cursor position. */
            if (end_mark != start_mark)
            {
                glyph_len = end_mark - start_mark;
            }
#ifdef GX_UTF8_SUPPORT
            else
            {
                /* Get the glyph length of the cursor left character. */
                _gx_utility_utf8_string_backward_character_length_get(&string, (INT)(insert_pos - 1), &glyph_len);
            }
#endif
            /* Reset the value of cursor positin. */
            text_input -> gx_single_line_text_input_insert_pos -= glyph_len;

            /* Pick up text font. */
            _gx_widget_font_get((GX_WIDGET *)text_input, text_input -> gx_prompt_font_id, &gx_font);

            /* Get text length before current cursor position to new cursor position. */
            string.gx_string_ptr += (insert_pos - glyph_len);
            string.gx_string_length = glyph_len;
            _gx_system_string_width_get_ext(gx_font, &string, &width);

            cursor_pos = (GX_VALUE)(cursor_ptr -> gx_text_input_cursor_pos.gx_point_x - width);
        }

        switch (text_input -> gx_widget_style & GX_STYLE_TEXT_ALIGNMENT_MASK)
        {
        case GX_STYLE_TEXT_CENTER:
            if (start_mark == end_mark)
            {
                /* Mark old cursor area dirty. */
                _gx_text_input_cursor_dirty_rectangle_get(cursor_ptr, &client);
                _gx_system_dirty_partial_add((GX_WIDGET *)text_input, &client);
            }

            cursor_ptr -> gx_text_input_cursor_pos.gx_point_x = cursor_pos;

            /* Mark new cursor area dirty. */
            _gx_text_input_cursor_dirty_rectangle_get(cursor_ptr, &client);
            _gx_system_dirty_partial_add((GX_WIDGET *)text_input, &client);
            break;

        case GX_STYLE_TEXT_RIGHT:
        case GX_STYLE_TEXT_LEFT:
        default:
            if (cursor_pos < client.gx_rectangle_left + 1)
            {
                /* Update text input x offset. */
                text_input -> gx_single_line_text_input_xoffset = (GX_VALUE)(text_input -> gx_single_line_text_input_xoffset -
                                                                             client.gx_rectangle_left - 1 + cursor_pos);
                cursor_ptr -> gx_text_input_cursor_pos.gx_point_x = (GX_VALUE)(client.gx_rectangle_left + 1);

                mark_all = GX_TRUE;
            }
            else if (cursor_pos > client.gx_rectangle_right - 1)
            {
                /* Cursor position is out of client rectangle, update x offset. */
                text_input -> gx_single_line_text_input_xoffset = (GX_VALUE)(text_input -> gx_single_line_text_input_xoffset +
                                                                             cursor_pos - client.gx_rectangle_right + 1);
                cursor_ptr -> gx_text_input_cursor_pos.gx_point_x = (GX_VALUE)(client.gx_rectangle_right - 1);

                /* Should mark all widget as dirty.  */
                mark_all = GX_TRUE;
            }
            else
            {
                if (start_mark == end_mark)
                {
                    /* Mark old cursor area dirty. */
                    _gx_text_input_cursor_dirty_rectangle_get(cursor_ptr, &client);
                    _gx_system_dirty_partial_add((GX_WIDGET *)text_input, &client);
                }
                cursor_ptr -> gx_text_input_cursor_pos.gx_point_x = cursor_pos;

                /* Mark new cursor area dirty. */
                _gx_text_input_cursor_dirty_rectangle_get(cursor_ptr, &client);
                _gx_system_dirty_partial_add((GX_WIDGET *)text_input, &client);
            }
            break;
        }

        if ((!mark_all) && (start_mark != end_mark))
        {
            /* Mark highlight area as dirty. */
            client.gx_rectangle_left = cursor_pos;
            client.gx_rectangle_right = (GX_VALUE)(old_cursor_pos - 1);
        }

        _gx_system_dirty_partial_add((GX_WIDGET *)text_input, &client);
    }

    return GX_SUCCESS;
}

