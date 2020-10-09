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
#include "gx_multi_line_text_input.h"
#include "gx_text_input_cursor.h"
#include "gx_widget.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_multi_line_text_input_down_arrow                PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function move the cursor to down by one line.                  */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text_input                        Multi line text input             */
/*                                        control block                   */
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
/*                                          Update cursor position        */
/*                                            according to click position */
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
UINT _gx_multi_line_text_input_down_arrow(GX_MULTI_LINE_TEXT_INPUT *text_input)
{
GX_TEXT_INPUT_CURSOR *cursor_ptr = &text_input -> gx_multi_line_text_input_cursor_instance;
GX_VALUE              line_height;
GX_POINT              cursor_pos;
INT                   shift;
GX_RECTANGLE          cursor_rect;
GX_FONT              *font;

    _gx_widget_font_get((GX_WIDGET *)text_input, text_input -> gx_multi_line_text_view_font_id, &font);

    if (!font)
    {
        return GX_FAILURE;
    }

    if (text_input -> gx_multi_line_text_input_start_mark != text_input -> gx_multi_line_text_input_end_mark)
    {
        _gx_multi_line_text_input_right_arrow(text_input);
    }

    /* Record cursor rectangle and scroll shift value before recalculate cursor position. */
    _gx_text_input_cursor_dirty_rectangle_get(&text_input -> gx_multi_line_text_input_cursor_instance, &cursor_rect);

    shift = text_input -> gx_multi_line_text_view_text_scroll_shift;

    line_height = (GX_VALUE)(font -> gx_font_line_height +
                             text_input -> gx_multi_line_text_view_line_space);

    if (!line_height)
    {
        return GX_FAILURE;
    }

    cursor_pos = cursor_ptr -> gx_text_input_cursor_pos;
    cursor_pos.gx_point_y = (GX_VALUE)(cursor_pos.gx_point_y + line_height);
    _gx_multi_line_text_input_cursor_pos_calculate(text_input, cursor_pos);

    if (shift == text_input -> gx_multi_line_text_view_text_scroll_shift)
    {
        /* Mark old cursor area dirty. */
        _gx_system_dirty_partial_add((GX_WIDGET *)text_input, &cursor_rect);

        /* Mark new cursor area dirty. */
        _gx_text_input_cursor_dirty_rectangle_get(&text_input -> gx_multi_line_text_input_cursor_instance, &cursor_rect);
        _gx_system_dirty_partial_add((GX_WIDGET *)text_input, &cursor_rect);
    }
    else
    {
        /* Mark widget area dirty. */
        _gx_system_dirty_mark((GX_WIDGET *)text_input);
    }

    return GX_SUCCESS;
}

