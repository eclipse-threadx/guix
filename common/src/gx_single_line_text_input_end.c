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

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_single_line_text_input_end                      PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service positions the text input widget cursor at the end of   */
/*    the input string.                                                   */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text_input                            Single line text input widget */
/*                                            control blcok               */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_font_get                   Get font by specified ID      */
/*    _gx_widget_border_width_get           Get the widget border width   */
/*    _gx_widget_client_get                 Retrieves client area of the  */
/*                                            widget                      */
/*    _gx_system_string_width_get           Get the width of a string     */
/*    _gx_system_dirty_mark                 Mart the area of the widget   */
/*                                            dirty                       */
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
UINT _gx_single_line_text_input_end(GX_SINGLE_LINE_TEXT_INPUT *text_input)
{
GX_TEXT_INPUT_CURSOR *cursor_ptr;
GX_VALUE              text_width;
GX_RECTANGLE          client;
GX_VALUE              new_xoffset;
GX_VALUE              new_cursor_pos;
GX_VALUE              border_width;
GX_FONT              *gx_font;
UINT                  start_mark = text_input -> gx_single_line_text_input_start_mark;
UINT                  end_mark = text_input -> gx_single_line_text_input_end_mark;
GX_BOOL               mark_new_cursor_dirty = GX_FALSE;
GX_STRING             string;

    if (start_mark != end_mark)
    {
        text_input -> gx_single_line_text_input_start_mark = 0;
        text_input -> gx_single_line_text_input_end_mark = 0;
    }

    cursor_ptr = &text_input -> gx_single_line_text_input_cursor_instance;
    string.gx_string_ptr = text_input -> gx_single_line_text_input_buffer;
    string.gx_string_length = text_input -> gx_single_line_text_input_string_size;

    /* Pick up text width. */
    _gx_widget_font_get((GX_WIDGET *)text_input, text_input -> gx_prompt_font_id, &gx_font);

    /* Calculate the widget width for showing text. */
    _gx_widget_border_width_get((GX_WIDGET *)text_input, &border_width);
    _gx_widget_client_get((GX_WIDGET *)text_input, border_width, &client);

    /* Update the value of cursor position and xoffset. */
    switch (text_input -> gx_widget_style & GX_STYLE_TEXT_ALIGNMENT_MASK)
    {
    case GX_STYLE_TEXT_RIGHT:
        new_xoffset = 0;
        new_cursor_pos = (GX_VALUE)(client.gx_rectangle_right - 1);
        break;

    case GX_STYLE_TEXT_CENTER:
        new_xoffset = 0;
        new_cursor_pos = (GX_VALUE)(client.gx_rectangle_left + 1 + ((client.gx_rectangle_right - client.gx_rectangle_left + 1) >> 1));
        new_cursor_pos = (GX_VALUE)(new_cursor_pos + text_input -> gx_single_line_text_input_xoffset);
        break;

    case GX_STYLE_TEXT_LEFT:
    default:
        /* Calculate new xoffset.  */
        _gx_system_string_width_get_ext(gx_font, &string, &text_width);
        new_xoffset = (GX_VALUE)(client.gx_rectangle_right - client.gx_rectangle_left + 1);
        new_xoffset = (GX_VALUE)(new_xoffset - 3);
        new_xoffset = (GX_VALUE)(text_width - new_xoffset);

        if (new_xoffset < 0)
        {
            new_xoffset = 0;
        }

        /* Calculate new cursor position. */
        new_cursor_pos = (GX_VALUE)(client.gx_rectangle_left + 1 - new_xoffset + text_width);
        break;
    }

    if (text_input -> gx_single_line_text_input_xoffset != new_xoffset)
    {
        /* We need to update text xoffset, mark whole dirty area dirty.  */
        _gx_system_dirty_mark((GX_WIDGET *)text_input);
    }
    else
    {
        if (start_mark != end_mark)
        {
            _gx_single_line_text_input_text_rectangle_get(text_input, (INT)(start_mark - end_mark), &client);

            _gx_system_dirty_partial_add((GX_WIDGET *)text_input, &client);

            mark_new_cursor_dirty = GX_TRUE;
        }
        else if (text_input -> gx_single_line_text_input_insert_pos != string.gx_string_length)
        {

            /* No need to update text xoffset, mark old and new dirty area */
            _gx_text_input_cursor_dirty_rectangle_get(cursor_ptr, &client);
            _gx_system_dirty_partial_add((GX_WIDGET *)text_input, &client);

            mark_new_cursor_dirty = GX_TRUE;
        }
    }

    /* Update the value of cursor position. */
    cursor_ptr -> gx_text_input_cursor_pos.gx_point_x = new_cursor_pos;

    /* Update character insert position.  */
    text_input -> gx_single_line_text_input_insert_pos = string.gx_string_length;

    /* Update text inpu x offset.  */
    text_input -> gx_single_line_text_input_xoffset = new_xoffset;

    if (mark_new_cursor_dirty)
    {
        /* Mark new cursor area as dirty. */
        _gx_text_input_cursor_dirty_rectangle_get(cursor_ptr, &client);
        _gx_system_dirty_partial_add((GX_WIDGET *)text_input, &client);
    }

    return GX_SUCCESS;
}

