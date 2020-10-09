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
#include "gx_context.h"
#include "gx_widget.h"
#include "gx_single_line_text_input.h"
#include "gx_utility.h"
#include "gx_text_input_cursor.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_single_line_text_input_backspace                PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service processes a backspace character.                       */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text_input                            Single-line text input widget */
/*                                            control blcok               */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    memmove                               Move a block of memory        */
/*    _gx_widget_font_get                   Get font by specified ID      */
/*    _gx_widget_client_get                 Get client rectangle          */
/*    _gx_widget_border_width_get           Get the widget border width   */
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
UINT _gx_single_line_text_input_backspace(GX_SINGLE_LINE_TEXT_INPUT *text_input)
{
GX_TEXT_INPUT_CURSOR *cursor_ptr = &text_input -> gx_single_line_text_input_cursor_instance;
GX_WIDGET            *widget = (GX_WIDGET *)text_input;
UINT                  insert_pos;
GX_VALUE              x_pos;
GX_VALUE              new_x_pos;
GX_RECTANGLE          client;
GX_VALUE              border_width;
GX_VALUE              delete_char_width;
GX_VALUE              text_width;
GX_VALUE              leftoff;
GX_VALUE              rightoff;
GX_FONT              *gx_font;
GX_CHAR              *input_buffer;
UINT                  string_size;
GX_STRING             string;
UINT                  glyph_len = 1;

    if (text_input -> gx_single_line_text_input_start_mark > text_input -> gx_single_line_text_input_end_mark)
    {
        return _gx_single_line_text_input_character_delete(text_input);
    }

    insert_pos = text_input -> gx_single_line_text_input_insert_pos;

    if (insert_pos > 0)
    {
        input_buffer = text_input -> gx_single_line_text_input_buffer;
        string_size = text_input -> gx_single_line_text_input_string_size;

        _gx_widget_border_width_get(widget, &border_width);
        _gx_widget_client_get((GX_WIDGET *)text_input, border_width, &client);

        if (text_input -> gx_single_line_text_input_start_mark != text_input -> gx_single_line_text_input_end_mark)
        {
            glyph_len = text_input -> gx_single_line_text_input_end_mark - text_input -> gx_single_line_text_input_start_mark;

            if (cursor_ptr -> gx_text_input_cursor_pos.gx_point_x <= client.gx_rectangle_left ||
                cursor_ptr -> gx_text_input_cursor_pos.gx_point_x >= client.gx_rectangle_right)
            {
                _gx_single_line_text_input_right_arrow(text_input);
            }

            text_input -> gx_single_line_text_input_start_mark = 0;
            text_input -> gx_single_line_text_input_end_mark = 0;
        }
#ifdef GX_UTF8_SUPPORT
        else
        {
            /* Get the glyph length of the cursor left character. */
            string.gx_string_ptr = input_buffer;
            string.gx_string_length = string_size;
            _gx_utility_utf8_string_backward_character_length_get(&string, (INT)(insert_pos - 1), &glyph_len);
        }
#endif

        /* Pick up delete character width.  */
        _gx_widget_font_get((GX_WIDGET *)text_input, text_input -> gx_prompt_font_id, &gx_font);

        string.gx_string_ptr = input_buffer + insert_pos - glyph_len;
        string.gx_string_length = glyph_len;
        _gx_system_string_width_get_ext(gx_font, &string, &delete_char_width);

        /* Pick up text witth. */
        string.gx_string_ptr = input_buffer;
        string.gx_string_length = string_size;
        _gx_system_string_width_get_ext(gx_font, &string, &text_width);

        /* Delete a character from string buffer.  */
        memmove(input_buffer + insert_pos - glyph_len, input_buffer + insert_pos, string.gx_string_length - insert_pos);
        text_input -> gx_single_line_text_input_buffer[string_size - glyph_len] = '\0';

        /* Update input string size and insert position.  */
        text_input -> gx_single_line_text_input_string_size -= glyph_len;
        text_input -> gx_single_line_text_input_insert_pos -= glyph_len;

        switch (text_input -> gx_widget_style & GX_STYLE_TEXT_ALIGNMENT_MASK)
        {
        case GX_STYLE_TEXT_RIGHT:
            x_pos = (GX_VALUE)(client.gx_rectangle_right - 1);
            x_pos = (GX_VALUE)(x_pos - text_input -> gx_single_line_text_input_xoffset);

            leftoff = (GX_VALUE)(client.gx_rectangle_left + 1 - x_pos);
            rightoff = (GX_VALUE)(x_pos + text_width - (client.gx_rectangle_right - 1));

            if ((leftoff > delete_char_width) || (rightoff == 0))
            {
                /* Decrease text inpit xoffset by the width of the deleted character width.  */
                text_input -> gx_single_line_text_input_xoffset = (GX_VALUE)(text_input -> gx_single_line_text_input_xoffset - delete_char_width);

                /* No need to update cursor position, mark the cursor left area dirty.  */
                client.gx_rectangle_right = cursor_ptr -> gx_text_input_cursor_pos.gx_point_x;
                _gx_system_dirty_partial_add((GX_WIDGET *)text_input, &client);
            }
            else if ((leftoff == 0) && (rightoff > delete_char_width))
            {
                /* Decrease cursor position by the width of the deleted character width.  */
                cursor_ptr -> gx_text_input_cursor_pos.gx_point_x = (GX_VALUE)(cursor_ptr -> gx_text_input_cursor_pos.gx_point_x - delete_char_width);

                /* No need to update x offset, mark the cursor right area dirty.  */
                client.gx_rectangle_left = cursor_ptr -> gx_text_input_cursor_pos.gx_point_x;
                _gx_system_dirty_partial_add((GX_WIDGET *)text_input, &client);
            }
            else
            {
                rightoff = (GX_VALUE)(rightoff - (delete_char_width - leftoff));

                if (rightoff > 0)
                {
                    text_input -> gx_single_line_text_input_xoffset = (GX_VALUE)(text_input -> gx_single_line_text_input_xoffset - leftoff);
                    cursor_ptr -> gx_text_input_cursor_pos.gx_point_x = (GX_VALUE)(cursor_ptr -> gx_text_input_cursor_pos.gx_point_x - delete_char_width + leftoff);
                }
                else
                {
                    text_input -> gx_single_line_text_input_xoffset = (GX_VALUE)(text_input -> gx_single_line_text_input_xoffset - rightoff);
                    cursor_ptr -> gx_text_input_cursor_pos.gx_point_x = (GX_VALUE)(cursor_ptr -> gx_text_input_cursor_pos.gx_point_x - delete_char_width - rightoff);
                }

                _gx_system_dirty_mark((GX_WIDGET *)text_input);
            }
            break;

        case GX_STYLE_TEXT_CENTER:
            x_pos = (GX_VALUE)(client.gx_rectangle_left + 1);
            x_pos = (GX_VALUE)(x_pos + ((client.gx_rectangle_right - client.gx_rectangle_left + 1) >> 1));
            x_pos = (GX_VALUE)(x_pos - text_input -> gx_single_line_text_input_xoffset);
            if (text_width >= (client.gx_rectangle_right - client.gx_rectangle_left + 1 - border_width))
            {
                /* Dirty mark this widget. */
                _gx_system_dirty_mark((GX_WIDGET *)text_input);
            }
            else
            {
                client.gx_rectangle_left = (GX_VALUE)(x_pos - (cursor_ptr -> gx_text_input_cursor_width >> 1));
                client.gx_rectangle_right = (GX_VALUE)(x_pos + text_width + ((cursor_ptr -> gx_text_input_cursor_width + 1) >> 1) - 1);
                _gx_system_dirty_partial_add((GX_WIDGET *)text_input, &client);
            }
            /* Calculate the cursor position. */
            text_input -> gx_single_line_text_input_xoffset = (GX_VALUE)((text_width - delete_char_width + 1) >> 1);
            cursor_ptr -> gx_text_input_cursor_pos.gx_point_x = (GX_VALUE)(cursor_ptr -> gx_text_input_cursor_pos.gx_point_x +
                                                                           (((text_width + 1) >> 1) - text_input -> gx_single_line_text_input_xoffset) -
                                                                           delete_char_width);
            break;

        case GX_STYLE_TEXT_LEFT:
        default:
            x_pos = (GX_VALUE)(client.gx_rectangle_left + 1);
            x_pos = (GX_VALUE)(x_pos - text_input -> gx_single_line_text_input_xoffset);

            new_x_pos = (GX_VALUE)(x_pos + delete_char_width);

            if (new_x_pos < client.gx_rectangle_left + 1)
            {
                /* Decrease x offset value by the width of the deleted character. */
                text_input -> gx_single_line_text_input_xoffset = (GX_VALUE)(text_input -> gx_single_line_text_input_xoffset - delete_char_width);

                /* No need to update cursor position, mark the cursor left area dirty. */
                client.gx_rectangle_right = (GX_VALUE)(cursor_ptr -> gx_text_input_cursor_pos.gx_point_x + ((cursor_ptr -> gx_text_input_cursor_width + 1) >> 1) - 1);
                _gx_system_dirty_partial_add((GX_WIDGET *)text_input, &client);
            }
            else if (x_pos < client.gx_rectangle_left + 1)
            {
                cursor_ptr -> gx_text_input_cursor_pos.gx_point_x = (GX_VALUE)(cursor_ptr -> gx_text_input_cursor_pos.gx_point_x - delete_char_width +
                                                                               text_input -> gx_single_line_text_input_xoffset);

                text_input -> gx_single_line_text_input_xoffset = 0;

                _gx_system_dirty_mark((GX_WIDGET *)text_input);
            }
            else
            {
                /* Decrease cursor position by the width of the deleted character.  */
                cursor_ptr -> gx_text_input_cursor_pos.gx_point_x = (GX_VALUE)(cursor_ptr -> gx_text_input_cursor_pos.gx_point_x - delete_char_width);

                /* No need to update text input x offset, mark th ecursor right area dirty. */
                client.gx_rectangle_left = (GX_VALUE)(cursor_ptr -> gx_text_input_cursor_pos.gx_point_x - (cursor_ptr -> gx_text_input_cursor_width >> 1));
                _gx_system_dirty_partial_add((GX_WIDGET *)text_input, &client);
            }
            break;
        }


        /* Mark text input be changed.  */
        text_input -> gx_single_line_text_input_was_modified = GX_TRUE;
    }

    return GX_SUCCESS;
}

