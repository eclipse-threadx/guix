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
#include "gx_prompt.h"
#include "gx_single_line_text_input.h"
#include "gx_utility.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_single_line_text_input_create                   PORTABLE C      */
/*                                                           6.1.3        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service creates a single-line text input widget.               */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text_input                        Single-line text input widget     */
/*                                        control block                   */
/*    name                              Name of text input widget         */
/*    parent                            Pointer to parent widget          */
/*    input_buffer                      Pointer to text input buffer      */
/*    buffer_size                       Size of text input buffer         */
/*    style                             Style of text input widget.       */
/*    text_input_id                     Application-defined ID for text   */
/*                                        input                           */
/*    size                              Dimensions of text input widget   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                            Completion status                 */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_prompt_create                 Create a prompt                   */
/*    _gx_widget_border_width_get       Get widget border width           */
/*    _gx_widget_client_get             Get widget client rectangle       */
/*    _gx_widget_link                   Link the widget to its parent     */
/*    _gx_utility_string_length_check   Test string length                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*  12-31-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            supported dynamic buffer,   */
/*                                            resulting in version 6.1.3  */
/*                                                                        */
/**************************************************************************/
UINT  _gx_single_line_text_input_create(GX_SINGLE_LINE_TEXT_INPUT *text_input,
                                        GX_CONST GX_CHAR *name,
                                        GX_WIDGET *parent,
                                        GX_CHAR *input_buffer,
                                        UINT buffer_size,
                                        UINT style,
                                        USHORT text_input_id,
                                        GX_CONST GX_RECTANGLE *size)
{
GX_TEXT_INPUT_CURSOR *cursor_ptr;
GX_VALUE              width;
GX_RECTANGLE          client;

    /* Don't allow TEXT_COPY style, the text input widget always uses a dynamic buffer */
    style &= (ULONG) ~GX_STYLE_TEXT_COPY;

    /* Call the prompt widget create function.  */
    _gx_prompt_create((GX_PROMPT *)text_input, name, GX_NULL, 0, style, text_input_id, size);


    if ((!input_buffer) && buffer_size)
    {
        if (!_gx_system_memory_allocator)
        {
            return GX_SYSTEM_MEMORY_ERROR;
        }

        input_buffer = _gx_system_memory_allocator(buffer_size);

        if (!input_buffer)
        {
            return GX_SYSTEM_MEMORY_ERROR;
        }

        text_input -> gx_widget_status |= GX_STATUS_DYNAMIC_BUFFER;
    }

    cursor_ptr = &text_input -> gx_single_line_text_input_cursor_instance;

    /* Finish the rest of the single line text input widget creation process. */
    text_input -> gx_widget_type = GX_TYPE_SINGLE_LINE_TEXT_INPUT;
    text_input -> gx_prompt_font_id = GX_FONT_ID_TEXT_INPUT;
    text_input -> gx_prompt_normal_text_color = GX_COLOR_ID_TEXT_INPUT_TEXT;
    text_input -> gx_widget_normal_fill_color = GX_COLOR_ID_TEXT_INPUT_FILL;
    text_input -> gx_single_line_text_input_readonly_text_color = GX_COLOR_ID_READONLY_TEXT;
    text_input -> gx_single_line_text_input_readonly_fill_color = GX_COLOR_ID_READONLY_FILL;
    text_input -> gx_widget_draw_function = (VOID (*)(GX_WIDGET *))_gx_single_line_text_input_draw;
    text_input -> gx_widget_event_process_function = (UINT (*)(GX_WIDGET *, GX_EVENT *))_gx_single_line_text_input_event_process;
    text_input -> gx_single_line_text_input_buffer = input_buffer;
    text_input -> gx_single_line_text_input_buffer_size = buffer_size;
    text_input -> gx_widget_style |= GX_STYLE_CURSOR_BLINK;
    text_input -> gx_single_line_text_input_was_modified = GX_FALSE;
    text_input -> gx_single_line_text_input_yoffset = 0;
    text_input -> gx_widget_selected_fill_color = GX_COLOR_ID_SELECTED_FILL;
    text_input -> gx_prompt_selected_text_color = GX_COLOR_ID_SELECTED_TEXT;
    text_input -> gx_single_line_text_input_start_mark = 0;
    text_input -> gx_single_line_text_input_end_mark = 0;

    cursor_ptr -> gx_text_input_cursor_blink_interval = GX_CURSOR_BLINK_INTERVAL;
    cursor_ptr -> gx_text_input_cursor_height = 0;
    cursor_ptr -> gx_text_input_cursor_width = 1;
    cursor_ptr -> gx_text_input_cursor_pos.gx_point_x = 0;
    cursor_ptr -> gx_text_input_cursor_pos.gx_point_y = 0;
    cursor_ptr -> gx_text_input_cursor_flags = 0;

    /* Pickup widget width. */
    _gx_widget_border_width_get((GX_WIDGET *)text_input, &width);

    /* Pickup client rectangle. */
    _gx_widget_client_get((GX_WIDGET *)text_input, width, &client);

    /* Calculate client width.  */
    width = (GX_VALUE)(client.gx_rectangle_right - client.gx_rectangle_left + 1);

    switch (text_input -> gx_widget_style & GX_STYLE_TEXT_ALIGNMENT_MASK)
    {
    case GX_STYLE_TEXT_RIGHT:
        /* Right aliagnment. */
        text_input -> gx_single_line_text_input_xoffset = (GX_VALUE)(3 - width);
        break;

    case GX_STYLE_TEXT_CENTER:
        /* Center aligned. */
        text_input -> gx_single_line_text_input_xoffset = (GX_VALUE)(width >> 1);
        break;

    case GX_STYLE_TEXT_LEFT:
    default:
        /* Default: Left aliagnment. */
        text_input -> gx_single_line_text_input_xoffset = 0;
        break;
    }

    if (buffer_size)
    {
        if (_gx_utility_string_length_check(input_buffer, &text_input -> gx_single_line_text_input_string_size, buffer_size - 1) != GX_SUCCESS)
        {
            text_input -> gx_single_line_text_input_string_size = 0;
        }
    }
    else
    {
        text_input -> gx_single_line_text_input_string_size = 0;
    }

    text_input -> gx_single_line_text_input_insert_pos = text_input -> gx_single_line_text_input_string_size;

    if (text_input -> gx_widget_style & GX_STYLE_CURSOR_ALWAYS)
    {
        text_input -> gx_widget_status |= (GX_STATUS_CURSOR_SHOW | GX_STATUS_CURSOR_DRAW);

        /* this widget wants to be notified if it is moved or re-sized */
        text_input -> gx_widget_status |= GX_STATUS_RESIZE_NOTIFY;
    }

    /* Determine if a parent widget was provided.  */
    if (parent)
    {
        _gx_widget_link(parent, (GX_WIDGET *)text_input);
    }

    /* Return the complete status */
    return(GX_SUCCESS);
}

