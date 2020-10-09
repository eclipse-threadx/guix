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

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_single_line_text_input_text_rectangle_get       PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function retrieves the bounding box from the current cursor    */
/*    position to the specified offset position.                          */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text_input                            Single-line text input widget */
/*                                            control block               */
/*    offset_index                          Index offset to the current   */
/*                                            cursor position             */
/*    rect                                  Retrieved bounding rectangle  */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Completion status                                                   */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_dirty_mark                Mark widget as drity           */
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
UINT _gx_single_line_text_input_text_rectangle_get(GX_SINGLE_LINE_TEXT_INPUT *input, INT offset_index, GX_RECTANGLE *rect)
{
GX_TEXT_INPUT_CURSOR *cursor = &input -> gx_single_line_text_input_cursor_instance;
GX_FONT              *font;
GX_VALUE              text_width;
GX_VALUE              border_width;
INT                   start_index;
INT                   end_index;
GX_STRING             string;

    if (offset_index == 0)
    {
        return GX_FAILURE;
    }

    if (offset_index > 0)
    {
        start_index = (INT)input -> gx_single_line_text_input_insert_pos;
        end_index = start_index + offset_index;
    }
    else
    {
        start_index = (INT)input -> gx_single_line_text_input_insert_pos + (INT)offset_index;
        end_index = (INT)(input -> gx_single_line_text_input_insert_pos);
    }

    /* Pick up font. */
    _gx_widget_font_get((GX_WIDGET *)input, input -> gx_prompt_font_id, &font);

    /* Get the width of specified text. */
    string.gx_string_ptr = input -> gx_single_line_text_input_buffer + start_index;
    string.gx_string_length = (UINT)(end_index - start_index);
    _gx_system_string_width_get_ext(font, &string, &text_width);

    /* Pickup widget border width. */
    _gx_widget_border_width_get((GX_WIDGET *)input, &border_width);

    /* Get client rectangle. */
    _gx_widget_client_get((GX_WIDGET *)input, border_width, rect);

    if (offset_index > 0)
    {
        rect -> gx_rectangle_left = cursor -> gx_text_input_cursor_pos.gx_point_x;
        rect -> gx_rectangle_right = (GX_VALUE)(rect -> gx_rectangle_left + text_width - 1);
    }
    else
    {
        rect -> gx_rectangle_right = (GX_VALUE)(cursor -> gx_text_input_cursor_pos.gx_point_x - 1);
        rect -> gx_rectangle_left = (GX_VALUE)(rect -> gx_rectangle_right - text_width);
    }

    return GX_SUCCESS;
}

