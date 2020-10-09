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
#include "gx_system.h"
#include "gx_window.h"
#include "gx_widget.h"
#include "gx_scrollbar.h"
#include "gx_multi_line_text_input.h"
#include "gx_multi_line_text_view.h"
#include "gx_text_input_cursor.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_multi_line_text_input_up_arrow                  PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function moves highlight text end mark one line up.            */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text_input                            Multi line text input         */
/*                                            control block               */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_font_get                   Retrieve font                 */
/*    _gx_text_input_cursor_dirty_rectangle_get                           */
/*                                          Get cursor rectangle          */
/*    _gx_multi_line_text_input_cursor_pos_calculate                      */
/*                                          Calculate cursor position     */
/*                                            according to click index    */
/*    _gx_system_dirty_partial_add          Add one dirty area to         */
/*                                            dirty list                  */
/*    _gx_system_dirty_mark                 Mark widget area dirty        */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_multi_line_text_input_keydown_process                           */
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
UINT _gx_multi_line_text_input_mark_up(GX_MULTI_LINE_TEXT_INPUT *text_input)
{
GX_TEXT_INPUT_CURSOR *cursor_ptr = &text_input -> gx_multi_line_text_input_cursor_instance;
GX_VALUE              line_height;
GX_VALUE              half_line_height;
GX_POINT              cursor_pos;
INT                   shift;
GX_RECTANGLE          cursor_rect;
GX_FONT              *font;
UINT                  start_mark = text_input -> gx_multi_line_text_input_start_mark;
UINT                  end_mark = text_input -> gx_multi_line_text_input_end_mark;
GX_VALUE              width;

    if (start_mark == end_mark)
    {
        start_mark = text_input -> gx_multi_line_text_input_text_insert_position;
        end_mark = text_input -> gx_multi_line_text_input_text_insert_position;

        text_input -> gx_multi_line_text_input_start_mark = start_mark;
        text_input -> gx_multi_line_text_input_end_mark = end_mark;
    }

    shift = text_input -> gx_multi_line_text_view_text_scroll_shift;

    _gx_widget_font_get((GX_WIDGET *)text_input, text_input -> gx_multi_line_text_view_font_id, &font);

    if (!font)
    {
        return GX_FAILURE;
    }

    line_height = (GX_VALUE)(font -> gx_font_line_height + text_input -> gx_multi_line_text_view_line_space);

    if (!line_height)
    {
        return GX_FAILURE;
    }

    cursor_pos = cursor_ptr -> gx_text_input_cursor_pos;
    cursor_pos.gx_point_y = (GX_VALUE)(cursor_pos.gx_point_y - line_height);
    _gx_multi_line_text_input_cursor_pos_calculate(text_input, cursor_pos);
    text_input -> gx_multi_line_text_input_end_mark = text_input -> gx_multi_line_text_input_text_insert_position;

    if (shift == text_input -> gx_multi_line_text_view_text_scroll_shift)
    {
        _gx_widget_border_width_get((GX_WIDGET *)text_input, &width);
        _gx_widget_client_get((GX_WIDGET *)text_input, width, &cursor_rect);

        half_line_height = (GX_VALUE)((line_height + 1) >> 1);
        cursor_rect.gx_rectangle_top = (GX_VALUE)(cursor_pos.gx_point_y - half_line_height);
        cursor_rect.gx_rectangle_bottom = (GX_VALUE)(cursor_pos.gx_point_y + line_height + half_line_height);

        /* Mark highlight area as dirty. */
        _gx_system_dirty_partial_add((GX_WIDGET *)text_input, &cursor_rect);
    }
    else
    {
        /* Mark widget area dirty. */
        _gx_system_dirty_mark((GX_WIDGET *)text_input);
    }

    return GX_SUCCESS;
}

