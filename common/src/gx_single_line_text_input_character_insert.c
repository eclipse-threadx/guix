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


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_single_line_text_input_character_insert         PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This services inserts a character into the text input string at the */
/*    current cursor position.                                            */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text_input                            Single-line text input widget */
/*                                            control block               */
/*    insert_str                            Byte string of insert         */
/*                                            character                   */
/*    insert_size                           String size of insert         */
/*                                            character                   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    memmove                               Move a block of memory        */
/*    _gx_widget_font_get                   Get font by specified ID      */
/*    _gx_widget_border_width_get           Get the widget border width   */
/*    _gx_widget_client_get                 Get widget client rectangle   */
/*    _gx_system_string_width_get           Get the width of a string     */
/*    _gx_system_dirty_partial_add          Mark the partial area of a    */
/*                                            widget as dirty             */
/*    _gx_single_line_text_input_backspace  Remove text before the cursor */
/*    _gx_single_line_text_input_character_delete                         */
/*                                          Remove text after the cursor  */
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
UINT _gx_single_line_text_input_character_insert(GX_SINGLE_LINE_TEXT_INPUT *text_input, GX_UBYTE *insert_str, UINT insert_size)
{
GX_TEXT_INPUT_CURSOR *cursor_ptr;
UINT                  buffer_size;
GX_CHAR              *string_buffer;
UINT                  string_size;
GX_VALUE              new_cursor_pos;
UINT                  insert_pos;
GX_VALUE              border_width;
GX_VALUE              client_right;
GX_VALUE              client_width;
GX_VALUE              insert_width;
GX_VALUE              text_width;
GX_FONT              *gx_font;
GX_RECTANGLE          dirty_area;
GX_STRING             string;

    /* Delete hightlight text. */
    if (text_input -> gx_single_line_text_input_start_mark != text_input -> gx_single_line_text_input_end_mark)
    {
        if (text_input -> gx_single_line_text_input_end_mark > text_input -> gx_single_line_text_input_start_mark)
        {
            _gx_single_line_text_input_backspace(text_input);
        }
        else
        {
            _gx_single_line_text_input_character_delete(text_input);
        }
    }

    cursor_ptr = &text_input -> gx_single_line_text_input_cursor_instance;

    buffer_size = text_input -> gx_single_line_text_input_buffer_size;
    string_size = text_input -> gx_single_line_text_input_string_size;

    /* Judge whether there have remaining buffer size.  */
    if (buffer_size < string_size + insert_size + 1)
    {
        return GX_FAILURE;
    }

    string_buffer = text_input -> gx_single_line_text_input_buffer;

    _gx_widget_font_get((GX_WIDGET *)text_input, text_input -> gx_prompt_font_id, &gx_font);

    /* Pick up text witth. */
    string.gx_string_ptr = string_buffer;
    string.gx_string_length = text_input -> gx_single_line_text_input_string_size;
    _gx_system_string_width_get_ext(gx_font, &string, &text_width);

    /* Insert a char to the string buffer. */
    insert_pos = text_input -> gx_single_line_text_input_insert_pos;
    memmove(string_buffer + insert_pos + insert_size, string_buffer + insert_pos, string_size - insert_pos);
    memmove(string_buffer + insert_pos, insert_str, insert_size);
    string_buffer[string_size + insert_size] = '\0';

    /* Update the input string size and cursor position. */
    text_input -> gx_single_line_text_input_string_size += insert_size;
    text_input -> gx_single_line_text_input_insert_pos += insert_size;

    /* Calculate inside widget right. */
    _gx_widget_border_width_get((GX_WIDGET *)text_input, &border_width);
    _gx_widget_client_get((GX_WIDGET *)text_input, border_width, &dirty_area);
    client_right = dirty_area.gx_rectangle_right;

    /* Get inset character width. */
    string.gx_string_ptr = (GX_CONST GX_CHAR *)insert_str;
    string.gx_string_length = insert_size;
    _gx_system_string_width_get_ext(gx_font, &string, &insert_width);

    switch (text_input -> gx_widget_style & GX_STYLE_TEXT_ALIGNMENT_MASK)
    {
    case GX_STYLE_TEXT_RIGHT:
        text_input -> gx_single_line_text_input_xoffset = (GX_VALUE)(text_input -> gx_single_line_text_input_xoffset + insert_width);

        dirty_area.gx_rectangle_right = cursor_ptr -> gx_text_input_cursor_pos.gx_point_x;
        break;

    case GX_STYLE_TEXT_CENTER:
        /* Calculate the cursor position. */
        client_width = (GX_VALUE)(dirty_area.gx_rectangle_right - dirty_area.gx_rectangle_left + 1);
        text_width = (GX_VALUE)(text_width + insert_width);
        new_cursor_pos = (GX_VALUE)(cursor_ptr -> gx_text_input_cursor_pos.gx_point_x -
                                    (((text_width + 1) >> 1) - text_input -> gx_single_line_text_input_xoffset) +
                                    insert_width);
        /* Update offset in x axis. */
        text_input -> gx_single_line_text_input_xoffset = (GX_VALUE)((text_width + 1) >> 1);

        /* Update offset in x axis. */
        if (new_cursor_pos < client_right - 1)
        {
            dirty_area.gx_rectangle_left = (GX_VALUE)(dirty_area.gx_rectangle_left + 1 + (client_width >> 1) - text_input -> gx_single_line_text_input_xoffset);
            dirty_area.gx_rectangle_right = (GX_VALUE)(dirty_area.gx_rectangle_left + text_width);
            dirty_area.gx_rectangle_left = (GX_VALUE)(dirty_area.gx_rectangle_left - (cursor_ptr -> gx_text_input_cursor_width >> 1));
            dirty_area.gx_rectangle_right = (GX_VALUE)(dirty_area.gx_rectangle_right + ((cursor_ptr -> gx_text_input_cursor_width + 1) >> 1) - 1);
        }

        cursor_ptr -> gx_text_input_cursor_pos.gx_point_x = new_cursor_pos;
        break;

    case GX_STYLE_TEXT_LEFT:
    default:
        /* Calculate cursor position. */
        new_cursor_pos = (GX_VALUE)(cursor_ptr -> gx_text_input_cursor_pos.gx_point_x + insert_width);

        /* Update offset in x axis. */
        if (new_cursor_pos > client_right - 1)
        {
            text_input -> gx_single_line_text_input_xoffset = (GX_VALUE)(text_input -> gx_single_line_text_input_xoffset + new_cursor_pos - client_right + 1);

            new_cursor_pos = (GX_VALUE)(client_right - 1);
        }
        else
        {
            dirty_area.gx_rectangle_left = (GX_VALUE)(cursor_ptr -> gx_text_input_cursor_pos.gx_point_x - (cursor_ptr -> gx_text_input_cursor_width >> 1));
            dirty_area.gx_rectangle_right = (GX_VALUE)(new_cursor_pos + text_width + cursor_ptr -> gx_text_input_cursor_width - 1);
        }

        /* Update x coordinate of cursor position. */
        cursor_ptr -> gx_text_input_cursor_pos.gx_point_x = new_cursor_pos;
        break;
    }

    /* Mark the text input widget be changed. */
    text_input -> gx_single_line_text_input_was_modified = GX_TRUE;

    _gx_system_dirty_partial_add((GX_WIDGET *)text_input, &dirty_area);

    return GX_SUCCESS;
}

