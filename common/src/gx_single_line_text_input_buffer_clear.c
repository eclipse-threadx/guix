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
/*    _gx_single_line_text_input_buffer_clear             PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service deletes all characters from the text input buffer.     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text_input                            Single-line text input widget */
/*                                            control block               */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_border_width_get           Get the widget border width   */
/*    _gx_widget_client_get                 Get the client rectangle      */
/*    _gx_system_dirty_mark                 Mark the widget area as dirty */
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
UINT _gx_single_line_text_input_buffer_clear(GX_SINGLE_LINE_TEXT_INPUT *text_input)
{
GX_TEXT_INPUT_CURSOR *cursor_ptr;
GX_RECTANGLE          client;
GX_VALUE              client_width;
GX_VALUE              border_width;

    cursor_ptr = &text_input -> gx_single_line_text_input_cursor_instance;

    /* Calculate the widget width for showing text. */
    _gx_widget_border_width_get((GX_WIDGET *)text_input, &border_width);
    _gx_widget_client_get((GX_WIDGET *)text_input, border_width, &client);

    client_width = (GX_VALUE)(client.gx_rectangle_right - client.gx_rectangle_left + 1);

    /* Update the value of cursor position and xoffset. */
    switch (text_input -> gx_widget_style & GX_STYLE_TEXT_ALIGNMENT_MASK)
    {
    case GX_STYLE_TEXT_RIGHT:
        text_input -> gx_single_line_text_input_xoffset = (GX_VALUE)(3 - client_width);
        cursor_ptr -> gx_text_input_cursor_pos.gx_point_x = (GX_VALUE)(client.gx_rectangle_right - 1);
        break;

    case GX_STYLE_TEXT_CENTER:
        text_input -> gx_single_line_text_input_xoffset = (client_width >> 1);
        cursor_ptr -> gx_text_input_cursor_pos.gx_point_x = (GX_VALUE)(client.gx_rectangle_left + (client_width >> 1) + 1);
        break;

    case GX_STYLE_TEXT_LEFT:
    default:
        text_input -> gx_single_line_text_input_xoffset = 0;
        cursor_ptr -> gx_text_input_cursor_pos.gx_point_x = (GX_VALUE)(client.gx_rectangle_left + 1);
        break;
    }

    /* Reset the value of cursor position. */
    text_input -> gx_single_line_text_input_buffer[0] = '\0';
    text_input -> gx_single_line_text_input_string_size = 0;
    text_input -> gx_single_line_text_input_insert_pos = 0;

    /* Mark the widget area dirty. */
    _gx_system_dirty_mark((GX_WIDGET *)text_input);

    return GX_SUCCESS;
}

