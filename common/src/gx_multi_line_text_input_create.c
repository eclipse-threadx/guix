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
/**   Multi Line Text Input Management (Multi Line Text Input)            */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_widget.h"
#include "gx_multi_line_text_input.h"
#include "gx_multi_line_text_view.h"
#include "gx_utility.h"
#include "gx_system.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_multi_line_text_input_create                    PORTABLE C      */
/*                                                           6.1.3        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function creates a multi-line text input widget.               */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text_input                          Multi-line input widget control */
/*                                          block                         */
/*    name                                Name of text input widget       */
/*    parent                              Pointer to parent widget        */
/*    input_buffer                        Pointer to text input buffer    */
/*    buffer_size                         Size of text input buffer in    */
/*                                          bytes                         */
/*    style                               Style of text input widget      */
/*    text_input_id                       Application-defined ID of the   */
/*                                          text input widget             */
/*    Size                                Dimensions of text input widget */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_multi_line_text_view_create     Create the multi line text view */
/*                                          widget                        */
/*    _gx_multi_line_text_view_text_set   Set the multi line text view    */
/*                                          text                          */
/*    _gx_widget_link                     Link the widget to its parent   */
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
UINT  _gx_multi_line_text_input_create(GX_MULTI_LINE_TEXT_INPUT *text_input,
                                       GX_CONST GX_CHAR *name,
                                       GX_WIDGET *parent,
                                       GX_CHAR *input_buffer,
                                       UINT buffer_size,
                                       ULONG style,
                                       USHORT text_input_id,
                                       GX_CONST GX_RECTANGLE *size)
{
GX_MULTI_LINE_TEXT_VIEW *view = (GX_MULTI_LINE_TEXT_VIEW *)text_input;
GX_TEXT_INPUT_CURSOR    *cursor_ptr = &text_input -> gx_multi_line_text_input_cursor_instance;
GX_STRING                string;

    /* Don't allow the GX_STYLE_TEXT_COPY style, the text input always uses a dynamic buffer */
    style &= (ULONG) ~GX_STYLE_TEXT_COPY;

    /* Call the multi-line text view create function. */
    _gx_multi_line_text_view_create(view, name, GX_NULL, 0, style, text_input_id, size);

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

    if (_gx_utility_string_length_check(input_buffer, &string.gx_string_length, buffer_size - 1) == GX_SUCCESS)
    {
        string.gx_string_ptr = input_buffer;
        _gx_multi_line_text_view_text_set_ext(view, &string);
    }
    else
    {
        text_input -> gx_multi_line_text_view_text.gx_string_ptr = input_buffer;
        text_input -> gx_multi_line_text_view_text.gx_string_length = 0;
    }

    text_input -> gx_widget_type = GX_TYPE_MULTI_LINE_TEXT_INPUT;
    text_input -> gx_widget_event_process_function = (UINT (*)(GX_WIDGET *, GX_EVENT *))_gx_multi_line_text_input_event_process;
    text_input -> gx_widget_draw_function = (VOID (*)(GX_WIDGET *))_gx_multi_line_text_input_draw;
    text_input -> gx_multi_line_text_input_text_was_modified = GX_FALSE;
    text_input -> gx_multi_line_text_input_buffer_size = buffer_size;
    text_input -> gx_widget_style |= GX_STYLE_CURSOR_BLINK;
    text_input -> gx_multi_line_text_input_text_cursor_line = 1;
    text_input -> gx_multi_line_text_input_text_insert_position = 0;
    text_input -> gx_multi_line_text_input_new_line_character[0] = GX_KEY_CARRIAGE_RETURN;
    text_input -> gx_multi_line_text_input_new_line_character[1] = '\0';
    text_input -> gx_multi_line_text_input_new_line_character_size = 1;
    text_input -> gx_multi_line_text_input_readonly_text_color = GX_COLOR_ID_READONLY_TEXT;
    text_input -> gx_multi_line_text_input_readonly_fill_color = GX_COLOR_ID_READONLY_FILL;
    text_input -> gx_widget_selected_fill_color = GX_COLOR_ID_SELECTED_FILL;
    text_input -> gx_multi_line_text_view_selected_text_color = GX_COLOR_ID_SELECTED_TEXT;
    text_input -> gx_multi_line_text_input_start_mark = 0;
    text_input -> gx_multi_line_text_input_end_mark = 0;

    cursor_ptr -> gx_text_input_cursor_height = 0;
    cursor_ptr -> gx_text_input_cursor_width = 1;
    cursor_ptr -> gx_text_input_cursor_flags = 0;
    cursor_ptr -> gx_text_input_cursor_blink_interval = GX_CURSOR_BLINK_INTERVAL;
    cursor_ptr -> gx_text_input_cursor_pos.gx_point_y = 0;
    cursor_ptr -> gx_text_input_cursor_pos.gx_point_x = (GX_VALUE)(text_input -> gx_window_client.gx_rectangle_left + 1);

    /* Determine if a parent widget was provided.  */
    if (parent)
    {
        _gx_widget_link((GX_WIDGET *)parent, (GX_WIDGET *)text_input);
    }

    if (text_input -> gx_widget_style & GX_STYLE_CURSOR_ALWAYS)
    {
        text_input -> gx_widget_status |= (GX_STATUS_CURSOR_SHOW | GX_STATUS_CURSOR_DRAW);
    }

    return(GX_SUCCESS);
}

