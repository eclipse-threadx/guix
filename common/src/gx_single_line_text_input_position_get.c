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
#include "gx_utility.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_single_line_text_input_position_get             PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service positions the text input cursor based on the requested */
/*    cursor position. The text input cursor index will be calculated     */
/*    based on the x value of the pixel pisition.                         */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text_input                                Single-line text input    */
/*                                                widget control block    */
/*    pixel_position                            X value of pixel position */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Status                                    Completion status         */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_font_get                   Get widget font               */
/*    _gx_widget_border_width_get           Get the widget border width   */
/*    _gx_widget_client_get                 Get client rectangle          */
/*    _gx_utility_utf8_string_character_get Parse utf8 string to          */
/*                                            multi-byte glyph            */
/*    _gx_system_string_width_get           Get the width of a string     */
/*    _gx_system_dirty_mark                 Mark a widget area dirty      */
/*    _gx_system_dirty_partial_add          Mark the partial area of a    */
/*                                            widget as dirty             */
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
UINT _gx_single_line_text_input_position_get(GX_SINGLE_LINE_TEXT_INPUT *text_input, INT pixel_position)
{
GX_TEXT_INPUT_CURSOR *cursor_ptr;
GX_CHAR              *string_buffer;
GX_FONT              *gx_font;
GX_RECTANGLE          client;
GX_VALUE              width;
GX_VALUE              cursor_pos;
GX_VALUE              x_pos;
GX_VALUE              distance;
UINT                  new_insert_pos;
UINT                  index;
UINT                  glyph_len = 1;
GX_STRING             utf8_string;
GX_STRING             string;

    string.gx_string_ptr = text_input -> gx_single_line_text_input_buffer;
    string.gx_string_length = text_input -> gx_single_line_text_input_string_size;

    cursor_ptr = &text_input -> gx_single_line_text_input_cursor_instance;

    string_buffer = text_input -> gx_single_line_text_input_buffer;

    _gx_widget_font_get((GX_WIDGET *)text_input, text_input -> gx_prompt_font_id, &gx_font);

    _gx_widget_border_width_get((GX_WIDGET *)text_input, &width);
    _gx_widget_client_get((GX_WIDGET *)text_input, width, &client);

    switch (text_input -> gx_widget_style & GX_STYLE_TEXT_ALIGNMENT_MASK)
    {
    case GX_STYLE_TEXT_RIGHT:
        x_pos = (GX_VALUE)(client.gx_rectangle_right - 1);
        break;

    case GX_STYLE_TEXT_CENTER:
        x_pos = (GX_VALUE)(client.gx_rectangle_left + 1 + ((client.gx_rectangle_right - client.gx_rectangle_left + 1) >> 1));
        break;

    case GX_STYLE_TEXT_LEFT:
    default:
        x_pos = (GX_VALUE)(client.gx_rectangle_left + 1);
        break;
    }

    x_pos = (GX_VALUE)(x_pos - text_input -> gx_single_line_text_input_xoffset);

    /* Reset mouse down position when it was out of the text show area. */
    if (pixel_position < client.gx_rectangle_left + 1)
    {
        pixel_position = client.gx_rectangle_left + 1;
    }
    else if (pixel_position > client.gx_rectangle_right + 1)
    {
        pixel_position = client.gx_rectangle_right - 1;
    }

    /*Compute the distance from the first character position to the mouse down position. */
    if (pixel_position > x_pos)
    {
        distance = (GX_VALUE)(pixel_position - x_pos);
    }
    else
    {
        distance = 0;
    }

    new_insert_pos = string.gx_string_length;
    index = 0;
    cursor_pos = 0;

    while (string.gx_string_length > 0)
    {
#ifdef GX_UTF8_SUPPORT
        _gx_utility_utf8_string_character_get(&string, GX_NULL, &glyph_len);
#else
        string.gx_string_ptr++;
        string.gx_string_length--;
#endif
        utf8_string.gx_string_ptr = string_buffer + index;
        utf8_string.gx_string_length = glyph_len;

        _gx_system_string_width_get_ext(gx_font, &utf8_string, &width);
        if ((cursor_pos + width / 2) > distance)
        {
            new_insert_pos = index;
            break;
        }
        cursor_pos = (GX_VALUE)(cursor_pos + width);
        index += glyph_len;
    }

    /* Update insert position. */
    text_input -> gx_single_line_text_input_insert_pos = new_insert_pos;

    cursor_pos = (GX_VALUE)(x_pos + cursor_pos);

    if (cursor_pos < client.gx_rectangle_left + 1)
    {
        /* Cursor is beyond widget left, adjust text offset to show cursor.  */
        cursor_ptr -> gx_text_input_cursor_pos.gx_point_x = (GX_VALUE)(client.gx_rectangle_left + 1);

        distance = (GX_VALUE)(client.gx_rectangle_left + 1 - cursor_pos);
        text_input -> gx_single_line_text_input_xoffset = (GX_VALUE)(text_input -> gx_single_line_text_input_xoffset - distance);

        /* Text offset is modified, mark whole text input area dirty. */
        _gx_system_dirty_mark((GX_WIDGET *)text_input);
    }
    else if (cursor_pos > client.gx_rectangle_right - 1)
    {
        /* Cursor is beyond widget right, adjust text offset to show cursor.  */
        cursor_ptr -> gx_text_input_cursor_pos.gx_point_x = (GX_VALUE)(client.gx_rectangle_right - 1);

        distance = (GX_VALUE)(cursor_pos - (client.gx_rectangle_right - 1));
        text_input -> gx_single_line_text_input_xoffset = (GX_VALUE)(text_input -> gx_single_line_text_input_xoffset + distance);

        /* Text offset is modified, mark whole text input area dirty. */
        _gx_system_dirty_mark((GX_WIDGET *)text_input);
    }
    else if (cursor_ptr -> gx_text_input_cursor_pos.gx_point_x != cursor_pos)
    {
        /* For this situation, we only need to mark old and new cursor position dirty.  */
        client.gx_rectangle_left = (GX_VALUE)(cursor_ptr -> gx_text_input_cursor_pos.gx_point_x - (cursor_ptr -> gx_text_input_cursor_width >> 1));
        client.gx_rectangle_right = (GX_VALUE)(client.gx_rectangle_left + cursor_ptr -> gx_text_input_cursor_width);
        _gx_system_dirty_partial_add((GX_WIDGET *)text_input, &client);

        cursor_ptr -> gx_text_input_cursor_pos.gx_point_x = cursor_pos;

        client.gx_rectangle_left = (GX_VALUE)(cursor_ptr -> gx_text_input_cursor_pos.gx_point_x - (cursor_ptr -> gx_text_input_cursor_width >> 1));
        client.gx_rectangle_right = (GX_VALUE)(client.gx_rectangle_left + cursor_ptr -> gx_text_input_cursor_width);
        _gx_system_dirty_partial_add((GX_WIDGET *)text_input, &client);
    }

    return GX_SUCCESS;
}

