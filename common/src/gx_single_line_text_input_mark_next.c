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
#include "gx_widget.h"
#include "gx_single_line_text_input.h"
#include "gx_text_input_cursor.h"
#include "gx_utility.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_single_line_text_input_mark_next                PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function highlights the character after the end mark position. */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text_input                           Single-line text input widget  */
/*                                           control block                */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_border_width_get           Get the widget border width   */
/*    _gx_widget_client_get                 Get widget client rectangle   */
/*    _gx_widget_font_get                   Retrieve font associate with  */
/*                                            specified font id           */
/*    _gx_system_string_width_get           Get the width of a string     */
/*    _gx_system_dirty_partial_add          Mark the specified area of    */
/*                                            a widget as dirty           */
/*    _gx_text_input_cursor_dirty_rectangle_get                           */
/*                                          Get cursor rectangle          */
/*    _gx_utility_utf8_string_character_get Parse utf8 string to          */
/*                                            multi-byte glyph            */
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
UINT _gx_single_line_text_input_mark_next(GX_SINGLE_LINE_TEXT_INPUT *text_input)
{
GX_TEXT_INPUT_CURSOR *cursor_ptr = &text_input -> gx_single_line_text_input_cursor_instance;
GX_CHAR              *string_buffer = text_input -> gx_single_line_text_input_buffer;
UINT                  string_size = text_input -> gx_single_line_text_input_string_size;
GX_VALUE              width;
GX_FONT              *gx_font;
GX_RECTANGLE          client;
GX_RECTANGLE          cursor_rect;
UINT                  glyph_len = 1;
INT                   new_cursor_pos;
GX_BOOL               mark_all = GX_FALSE;
UINT                  start_mark = text_input -> gx_single_line_text_input_start_mark;
UINT                  end_mark = text_input -> gx_single_line_text_input_end_mark;
GX_STRING             string;


    if (start_mark == end_mark)
    {
        start_mark = text_input -> gx_single_line_text_input_insert_pos;
        end_mark = text_input -> gx_single_line_text_input_insert_pos;

        text_input -> gx_single_line_text_input_start_mark = start_mark;
        text_input -> gx_single_line_text_input_end_mark = end_mark;
    }

    if (end_mark < string_size)
    {
#ifdef GX_UTF8_SUPPORT
        /* Calculate glyph length of the characgter after the end mark. */
        string.gx_string_ptr = string_buffer + end_mark;
        string.gx_string_length = string_size - end_mark;
        _gx_utility_utf8_string_character_get(&string, GX_NULL, &glyph_len);
#endif

        /* Update end mark. */
        text_input -> gx_single_line_text_input_end_mark += glyph_len;

        /* Update insert position. */
        text_input -> gx_single_line_text_input_insert_pos += glyph_len;

        /* Get widget border. */
        _gx_widget_border_width_get((GX_WIDGET *)text_input, &width);

        /* Get client rectangle. */
        _gx_widget_client_get((GX_WIDGET *)text_input, width, &client);

        /* Pickup text font. */
        _gx_widget_font_get((GX_WIDGET *)text_input, text_input -> gx_prompt_font_id, &gx_font);

        /* Get the glyph width after the end mark. */
        string.gx_string_ptr = string_buffer + end_mark;
        string.gx_string_length = glyph_len;
        _gx_system_string_width_get_ext(gx_font, &string, &width);

        /* Calculate new cursor position. */
        new_cursor_pos = (GX_VALUE)(cursor_ptr -> gx_text_input_cursor_pos.gx_point_x + width);

        switch (text_input -> gx_widget_style & GX_STYLE_TEXT_ALIGNMENT_MASK)
        {
        case GX_STYLE_TEXT_CENTER:
            break;

        case GX_STYLE_TEXT_RIGHT:
        case GX_STYLE_TEXT_LEFT:
        default:
            if (new_cursor_pos > client.gx_rectangle_right - 1)
            {
                /* The next marked character is out of client area, update x offset. */
                text_input -> gx_single_line_text_input_xoffset = (GX_VALUE)(text_input -> gx_single_line_text_input_xoffset +
                                                                             new_cursor_pos - client.gx_rectangle_right + 1);

                new_cursor_pos = (GX_VALUE)(client.gx_rectangle_right - 1);

                /* Should mark all widget as dirty.  */
                mark_all = GX_TRUE;
            }
            else if (new_cursor_pos < client.gx_rectangle_left + 1)
            {
                /* Update text input x offset. */
                text_input -> gx_single_line_text_input_xoffset = (GX_VALUE)(text_input -> gx_single_line_text_input_xoffset -
                                                                             client.gx_rectangle_left - 1 + new_cursor_pos);
                new_cursor_pos = (GX_VALUE)(client.gx_rectangle_left + 1);

                mark_all = GX_TRUE;
            }
            break;
        }

        if (!mark_all)
        {
            /* Only need to mark area between old and new end mark as dirty. */
            client.gx_rectangle_left = cursor_ptr -> gx_text_input_cursor_pos.gx_point_x;
            client.gx_rectangle_right = (GX_VALUE)(new_cursor_pos - 1);

            if (start_mark == end_mark)
            {
                /* Get old cursor rectangle. */
                _gx_text_input_cursor_dirty_rectangle_get(cursor_ptr, &cursor_rect);

                /* Combine with old cursor rectangle. */
                _gx_utility_rectangle_combine(&client, &cursor_rect);
            }
        }

        cursor_ptr -> gx_text_input_cursor_pos.gx_point_x = (GX_VALUE)new_cursor_pos;

        _gx_system_dirty_partial_add((GX_WIDGET *)text_input, &client);
    }
    return GX_SUCCESS;
}

