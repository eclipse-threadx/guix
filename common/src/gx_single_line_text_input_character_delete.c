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
#include "gx_utility.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_single_line_text_input_character_delete         PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service deletes the character after the text input cursor      */
/*    position.                                                           */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text_input                             Single line text input       */
/*                                            control blcok               */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    memmove                               Move a block of memory        */
/*    _gx_widget_border_width_get           Get widget border width       */
/*    _gx_widget_client_get                 Get widget client rectangle   */
/*    _gx_widget_font_get                   Get font by specified ID      */
/*    _gx_system_string_width_get           Get the width of a string     */
/*    _gx_system_dirty_partial_add          Mark the partial area of a    */
/*                                            widget as dirty             */
/*    _gx_utility_utf8_string_character_get Parse utf8 string to          */
/*                                            multi-byte glyph            */
/*    _gx_single_line_text_input_position_update                          */
/*                                          Update cursor position        */
/*                                            according to insert position*/
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
UINT _gx_single_line_text_input_character_delete(GX_SINGLE_LINE_TEXT_INPUT *text_input)
{
GX_TEXT_INPUT_CURSOR *cursor_ptr = &text_input -> gx_single_line_text_input_cursor_instance;
UINT                  insert_pos;
GX_CHAR              *string_buffer;
UINT                  string_size;
GX_VALUE              border_width;
GX_VALUE              text_width;
GX_RECTANGLE          client;
GX_RECTANGLE          dirty_area;
GX_FONT              *gx_font;
GX_VALUE              x_pos;
GX_VALUE              offset;
GX_VALUE              del_char_width;
UINT                  glyph_len = 1;

GX_STRING             string;

    if (text_input -> gx_single_line_text_input_end_mark > text_input -> gx_single_line_text_input_start_mark)
    {
        return _gx_single_line_text_input_backspace(text_input);
    }

    insert_pos = text_input -> gx_single_line_text_input_insert_pos;
    string_buffer = text_input -> gx_single_line_text_input_buffer;
    string_size = text_input -> gx_single_line_text_input_string_size;

    if (insert_pos < string_size)
    {
        _gx_widget_border_width_get((GX_WIDGET *)text_input, &border_width);
        _gx_widget_client_get((GX_WIDGET *)text_input, border_width, &client);

        if (text_input -> gx_single_line_text_input_start_mark != text_input -> gx_single_line_text_input_end_mark)
        {
            glyph_len = text_input -> gx_single_line_text_input_start_mark - text_input -> gx_single_line_text_input_end_mark;

            if (cursor_ptr -> gx_text_input_cursor_pos.gx_point_x <= client.gx_rectangle_left ||
                cursor_ptr -> gx_text_input_cursor_pos.gx_point_x >= client.gx_rectangle_right)
            {
                _gx_single_line_text_input_left_arrow(text_input);
            }
            text_input -> gx_single_line_text_input_start_mark = 0;
            text_input -> gx_single_line_text_input_end_mark = 0;
        }
#ifdef GX_UTF8_SUPPORT
        else
        {
            string.gx_string_ptr = string_buffer + insert_pos;
            string.gx_string_length = string_size - insert_pos;

            _gx_utility_utf8_string_character_get(&string, GX_NULL, &glyph_len);
        }
#endif
        dirty_area = client;

        _gx_widget_font_get((GX_WIDGET *)text_input, text_input -> gx_prompt_font_id, &gx_font);

        /* Pick up text witth. */
        string.gx_string_ptr = string_buffer;
        string.gx_string_length = text_input -> gx_single_line_text_input_string_size;

        _gx_system_string_width_get_ext(gx_font, &string, &text_width);

        string.gx_string_ptr = string_buffer + insert_pos;
        string.gx_string_length = glyph_len;

        _gx_system_string_width_get_ext(gx_font, &string, &del_char_width);

        switch (text_input -> gx_widget_style & GX_STYLE_TEXT_ALIGNMENT_MASK)
        {
        case GX_STYLE_TEXT_RIGHT:
            x_pos = (GX_VALUE)(client.gx_rectangle_right - 1);
            x_pos = (GX_VALUE)(x_pos - text_input -> gx_single_line_text_input_xoffset);

            if (x_pos + text_width > client.gx_rectangle_right - 1)
            {
                offset = (GX_VALUE)((x_pos + text_width - del_char_width) - (client.gx_rectangle_right - 1));

                if (offset < 0)
                {
                    text_input -> gx_single_line_text_input_xoffset = (GX_VALUE)(text_input -> gx_single_line_text_input_xoffset + offset);
                    cursor_ptr -> gx_text_input_cursor_pos.gx_point_x = (GX_VALUE)(cursor_ptr -> gx_text_input_cursor_pos.gx_point_x - offset);
                }
            }
            else
            {
                text_input -> gx_single_line_text_input_xoffset = (GX_VALUE)(text_input -> gx_single_line_text_input_xoffset - del_char_width);
                cursor_ptr -> gx_text_input_cursor_pos.gx_point_x = (GX_VALUE)(cursor_ptr -> gx_text_input_cursor_pos.gx_point_x + del_char_width);

                dirty_area.gx_rectangle_right = cursor_ptr -> gx_text_input_cursor_pos.gx_point_x;
            }
            break;

        case GX_STYLE_TEXT_CENTER:
            x_pos = (GX_VALUE)(client.gx_rectangle_left + 1);
            x_pos = (GX_VALUE)(x_pos + ((client.gx_rectangle_right - client.gx_rectangle_left + 1) >> 1));
            x_pos = (GX_VALUE)(x_pos - text_input -> gx_single_line_text_input_xoffset);
            if (text_width <= (client.gx_rectangle_right - client.gx_rectangle_left + 1 - border_width))
            {
                dirty_area.gx_rectangle_left = (GX_VALUE)(x_pos - (cursor_ptr -> gx_text_input_cursor_width >> 1));
                dirty_area.gx_rectangle_right = (GX_VALUE)(x_pos + text_width + ((cursor_ptr -> gx_text_input_cursor_width + 1) >> 1) - 1);
            }
            /* Calculate the cursor position. */
            text_input -> gx_single_line_text_input_xoffset = (GX_VALUE)((text_width - del_char_width + 1) >> 1);
            cursor_ptr -> gx_text_input_cursor_pos.gx_point_x = (GX_VALUE)(cursor_ptr -> gx_text_input_cursor_pos.gx_point_x +
                                                                           (((text_width + 1) >> 1) - text_input -> gx_single_line_text_input_xoffset));
            break;

        case GX_STYLE_TEXT_LEFT:
        default:
            x_pos = (GX_VALUE)(client.gx_rectangle_left + 1);
            x_pos = (GX_VALUE)(x_pos - text_input -> gx_single_line_text_input_xoffset);

            if (x_pos < client.gx_rectangle_left + 1)
            {
                offset = (GX_VALUE)((client.gx_rectangle_right - 1) - (x_pos + text_width - del_char_width));

                if (offset > 0)
                {
                    if (offset > (client.gx_rectangle_left + 1 - x_pos))
                    {
                        offset = (GX_VALUE)(client.gx_rectangle_left + 1 - x_pos);
                    }

                    text_input -> gx_single_line_text_input_xoffset = (GX_VALUE)(text_input -> gx_single_line_text_input_xoffset - offset);
                    cursor_ptr -> gx_text_input_cursor_pos.gx_point_x = (GX_VALUE)(cursor_ptr -> gx_text_input_cursor_pos.gx_point_x + offset);
                }
            }
            else
            {
                dirty_area.gx_rectangle_left = (GX_VALUE)(cursor_ptr -> gx_text_input_cursor_pos.gx_point_x - (cursor_ptr -> gx_text_input_cursor_width >> 1));
                dirty_area.gx_rectangle_right = (GX_VALUE)(x_pos + text_width + cursor_ptr -> gx_text_input_cursor_width - 1);
            }
            break;
        }


        /* Delete a character from the string buffer. */
        memmove(string_buffer + insert_pos, string_buffer + insert_pos + glyph_len, string_size - insert_pos - glyph_len);

        string_buffer[string_size - glyph_len] = '\0';
        text_input -> gx_single_line_text_input_string_size -= glyph_len;

        /* Mark text input be changed.  */
        text_input -> gx_single_line_text_input_was_modified = GX_TRUE;

        _gx_system_dirty_partial_add((GX_WIDGET *)text_input, &dirty_area);
    }

    return GX_SUCCESS;
}

