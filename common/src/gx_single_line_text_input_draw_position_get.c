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

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_single_line_text_input_draw_position_get        PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service calculates draw start position of text input text.     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text_input                            Single-line text input widget */
/*                                            control block.              */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_context_brush_get                 Get brush of current context  */
/*    _gx_widget_border_width_get           Get widget border width       */
/*    _gx_widget_client_get                 Get widget client rectangle   */
/*    _gx_system_string_width_get           Get the width of a string     */
/*    _gx_single_line_text_input_position_update                          */
/*                                          Update cursor position by     */
/*                                            insert index                */
/*                                                                        */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_single_line_text_input_draw                                     */
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
UINT _gx_single_line_text_input_draw_position_get(GX_SINGLE_LINE_TEXT_INPUT *text_input, GX_VALUE *xpos, GX_VALUE *ypos)
{
GX_VALUE     border_width;
UINT         text_height;
GX_VALUE     text_width;
GX_VALUE     client_height;
GX_VALUE     client_width;
GX_VALUE     x_pos;
GX_VALUE     y_pos;
GX_RECTANGLE client;
GX_BRUSH    *brush;
GX_BOOL      update_cursor_pos;
GX_STRING    string;

    _gx_context_brush_get(&brush);

    if (brush -> gx_brush_font == GX_NULL)
    {
        return(GX_FAILURE);
    }

    if (text_input -> gx_widget_style & GX_STYLE_CURSOR_ALWAYS)
    {
        update_cursor_pos = GX_TRUE;
    }
    else
    {
        update_cursor_pos = GX_FALSE;
    }

    /* Pickup widget width. */
    _gx_widget_border_width_get((GX_WIDGET *)text_input, &border_width);

    _gx_widget_client_get((GX_WIDGET *)text_input, border_width, &client);

    client_width = (GX_VALUE)(client.gx_rectangle_right - client.gx_rectangle_left + 1);
    client_height = (GX_VALUE)(client.gx_rectangle_bottom - client.gx_rectangle_top + 1);


    text_height = brush -> gx_brush_font -> gx_font_line_height;
    string.gx_string_ptr = text_input -> gx_single_line_text_input_buffer;
    string.gx_string_length = text_input -> gx_single_line_text_input_string_size;
    _gx_system_string_width_get_ext(brush -> gx_brush_font, &string, &text_width);

    switch (text_input -> gx_widget_style & GX_STYLE_TEXT_ALIGNMENT_MASK)
    {
    case GX_STYLE_TEXT_RIGHT:
        x_pos = (GX_VALUE)(client.gx_rectangle_right - 1);

        x_pos = (GX_VALUE)(x_pos - text_input -> gx_single_line_text_input_xoffset);

        if (text_width < (client_width - 3))
        {
            /* Text width is shorter than client width. */
            if (text_input -> gx_single_line_text_input_xoffset != text_width)
            {
                /* If xoffset is not equal to text width,
                   reset xoffset and update cursor position. */
                text_input -> gx_single_line_text_input_xoffset = text_width;
                update_cursor_pos = GX_TRUE;
            }
        }
        else if (x_pos > client.gx_rectangle_left + 1)
        {
            /* If text start/end position is inside client area,
               reset xoffset value and update cursor position. */
            text_input -> gx_single_line_text_input_xoffset = text_width;
            update_cursor_pos = GX_TRUE;
        }

        x_pos = (GX_VALUE)(client.gx_rectangle_right - 1);
        break;

    case GX_STYLE_TEXT_CENTER:
        x_pos = (GX_VALUE)(client.gx_rectangle_left + 1 + (client_width >> 1));
        x_pos = (GX_VALUE)(x_pos - text_input -> gx_single_line_text_input_xoffset);

        if (text_width < (client_width - 3))
        {
            /* Text width is shorter than client width. */
            if (text_input -> gx_single_line_text_input_xoffset != ((text_width + 1) >> 1))
            {
                /* If xoffset is not equal to 0,
                   reset xoffset value and update cursor position. */
                text_input -> gx_single_line_text_input_xoffset = (GX_VALUE)((text_width + 1) >> 1);
                update_cursor_pos = GX_TRUE;
            }
        }
        else if ((x_pos > client.gx_rectangle_left + 1) ||
                 (x_pos + text_width < client.gx_rectangle_right - 1))
        {
            /* If text start/end position is inside client area,
               reset xoffset value and update cursor position. */
            text_input -> gx_single_line_text_input_xoffset = (GX_VALUE)((text_width + 1) >> 1);
            update_cursor_pos = GX_TRUE;
        }
        x_pos = (GX_VALUE)(client.gx_rectangle_left + 1 + (client_width >> 1));
        break;

    case GX_STYLE_TEXT_LEFT:
    default:
        x_pos = (GX_VALUE)(client.gx_rectangle_left + 1);

        x_pos = (GX_VALUE)(x_pos - text_input -> gx_single_line_text_input_xoffset);

        if (text_width < (client_width - 3))
        {
            /* Text width is shorter than client width. */
            if (text_input -> gx_single_line_text_input_xoffset != 0)
            {
                /* If xoffset is not equal to 0,
                   reset xoffset value and update cursor position. */
                text_input -> gx_single_line_text_input_xoffset = 0;
                update_cursor_pos = GX_TRUE;
            }
        }
        else if (x_pos + text_width < client.gx_rectangle_right - 1)
        {
            /* If text start/end position is inside client area,
               reset xoffset value and update cursor position. */
            text_input -> gx_single_line_text_input_xoffset = 0;
            update_cursor_pos = GX_TRUE;
        }

        x_pos = (GX_VALUE)(client.gx_rectangle_left + 1);
        break;
    }

    if (update_cursor_pos)
    {
        _gx_single_line_text_input_position_update(text_input);
    }

    x_pos = (GX_VALUE)(x_pos - text_input -> gx_single_line_text_input_xoffset);

    y_pos = (GX_VALUE)(text_input -> gx_widget_size.gx_rectangle_top + border_width);
    y_pos = (GX_VALUE)(y_pos - text_input -> gx_single_line_text_input_yoffset);
    y_pos = (GX_VALUE)(y_pos + (GX_VALUE)(((INT)client_height - (INT)text_height) >> 1));

    *xpos = x_pos;
    *ypos = y_pos;

    return GX_SUCCESS;
}

