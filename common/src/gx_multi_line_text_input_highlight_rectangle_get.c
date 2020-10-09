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
/**   Text Input Management (Multi Line Text Input)                       */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_widget.h"
#include "gx_multi_line_text_input.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_multi_line_text_input_text_rectangle_get        PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function retrieves the bounding box of specified text.         */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    input                                 Multi-line text input widget  */
/*                                            control block               */
/*    start_index                           The start index of the        */
/*                                            specified text              */
/*    end_index                             The end index of the          */
/*                                            specified text              */
/*    rect                                  Retrieved bounding rectangle  */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
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
UINT _gx_multi_line_text_input_highlight_rectangle_get(GX_MULTI_LINE_TEXT_INPUT *input, GX_RECTANGLE *rect)
{
GX_TEXT_INPUT_CURSOR *cursor_ptr = &input -> gx_multi_line_text_input_cursor_instance;
INT                   line_height;
INT                   first_visible_line;
INT                   last_visible_line;
INT                   line_cache_start;
UINT                 *line_index = input -> gx_multi_line_text_view_line_index;
GX_FONT              *font;
GX_POINT              start_pos;
GX_POINT              end_pos;
UINT                  cursor_line;

    _gx_widget_font_get((GX_WIDGET *)input, input -> gx_multi_line_text_view_font_id, &font);

    if (!font)
    {
        return GX_FAILURE;
    }

    /* Pickup text height. */
    line_height = font -> gx_font_line_height + input -> gx_multi_line_text_view_line_space;

    if (!line_height)
    {
        return GX_FAILURE;
    }

    first_visible_line = -input -> gx_multi_line_text_view_text_scroll_shift / line_height;
    last_visible_line = first_visible_line + (INT)input -> gx_multi_line_text_view_text_visible_rows;
    if (last_visible_line > (INT)(input -> gx_multi_line_text_view_text_total_rows - 1))
    {
        last_visible_line = (INT)input -> gx_multi_line_text_view_text_total_rows - 1;
    }

    line_cache_start = (INT)input -> gx_multi_line_text_view_first_cache_line;

    if (input -> gx_multi_line_text_input_start_mark <= line_index[first_visible_line - line_cache_start])
    {
        start_pos.gx_point_x = input -> gx_widget_size.gx_rectangle_left;
        start_pos.gx_point_y = input -> gx_widget_size.gx_rectangle_top;
    }
    else if (input -> gx_multi_line_text_input_start_mark >= line_index[last_visible_line - line_cache_start])
    {
        start_pos.gx_point_x = input -> gx_widget_size.gx_rectangle_left;
        start_pos.gx_point_y = input -> gx_widget_size.gx_rectangle_bottom;
    }
    else
    {
        end_pos = cursor_ptr -> gx_text_input_cursor_pos;
        input -> gx_multi_line_text_input_text_insert_position = input -> gx_multi_line_text_input_start_mark;
        cursor_line = input -> gx_multi_line_text_input_text_cursor_line;

        _gx_multi_line_text_input_cursor_pos_update(input, GX_FALSE);
        start_pos = cursor_ptr -> gx_text_input_cursor_pos;

        input -> gx_multi_line_text_input_text_insert_position = input -> gx_multi_line_text_input_end_mark;
        input -> gx_multi_line_text_input_text_cursor_line = cursor_line;
        cursor_ptr -> gx_text_input_cursor_pos = end_pos;
    }

    _gx_multi_line_text_input_text_rectangle_get(input, start_pos, cursor_ptr -> gx_text_input_cursor_pos, rect);

    return GX_SUCCESS;
}

