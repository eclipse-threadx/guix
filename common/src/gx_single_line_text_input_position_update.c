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
/*    _gx_single_line_text_input_position_update          PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function update the cursor position according to character     */
/*      insert index.                                                     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text_input                            Single-line text input widget */
/*                                            control block               */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Complete status                                                     */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_font_get                   Get font by specified ID      */
/*    _gx_widget_border_width_get           Get widget border width       */
/*    _gx_widget_client_get                 Get widget client rectangle   */
/*    _gx_system_string_width_get           Get the width of a string     */
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
UINT _gx_single_line_text_input_position_update(GX_SINGLE_LINE_TEXT_INPUT *text_input)
{
GX_TEXT_INPUT_CURSOR *cursor_ptr;
UINT                  insert_pos;
GX_FONT              *gx_font;
GX_RECTANGLE          client;
GX_VALUE              width;
GX_VALUE              x_pos;
GX_STRING             string;

    cursor_ptr = &text_input -> gx_single_line_text_input_cursor_instance;
    insert_pos = text_input -> gx_single_line_text_input_insert_pos;

    _gx_widget_font_get((GX_WIDGET *)text_input, text_input -> gx_prompt_font_id, &gx_font);
    _gx_widget_border_width_get((GX_WIDGET *)text_input, &width);
    _gx_widget_client_get((GX_WIDGET *)text_input, width, &client);

    /* Calculate distance from first character to character insert position.  */
    string.gx_string_ptr = text_input -> gx_single_line_text_input_buffer;
    string.gx_string_length = insert_pos;
    _gx_system_string_width_get_ext(gx_font, &string, &width);

    switch (text_input -> gx_widget_style & GX_STYLE_TEXT_ALIGNMENT_MASK)
    {
    case GX_STYLE_TEXT_RIGHT:
        x_pos = (GX_VALUE)(client.gx_rectangle_right - 1);
        break;

    case GX_STYLE_TEXT_CENTER:
        x_pos = client.gx_rectangle_left;
        x_pos = (GX_VALUE)(x_pos + 1 + ((client.gx_rectangle_right - client.gx_rectangle_left + 1) >> 1));
        break;

    case GX_STYLE_TEXT_LEFT:
    default:
        x_pos = (GX_VALUE)(client.gx_rectangle_left + 1);
        break;
    }

    /* Pick up draw start position.  */
    x_pos = (GX_VALUE)(x_pos - text_input -> gx_single_line_text_input_xoffset);

    /* Update cursor position.  */
    cursor_ptr -> gx_text_input_cursor_pos.gx_point_x = (GX_VALUE)(x_pos + width);

    return GX_SUCCESS;
}

