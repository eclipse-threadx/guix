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
/**   Multi Line Text Input                                               */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_multi_line_text_input.h"
#include "gx_multi_line_text_view.h"
#include "gx_text_input_cursor.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_multi_line_text_input_mark_home                 PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function moves the end mark of highligh text to the start      */
/*    of the line.                                                        */
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
/*    _gx_text_input_cursor_dirty_rectangle_get                           */
/*                                          Retrieve cursor rectangle     */
/*    _gx_multi_line_text_input_cursor_pos_calculate                      */
/*                                          Calculate cursor position     */
/*                                            according to click point    */
/*    _gx_multi_line_text_input_text_rectangle_get                        */
/*                                          Retrieve text rectangle       */
/*                                            between specified positions */
/*    _gx_system_dirty_partial_add          Mark specified area dirty     */
/*    _gx_system_dirty_mark                 Mark specified widget dirty   */
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
UINT _gx_multi_line_text_input_mark_home(GX_MULTI_LINE_TEXT_INPUT *text_input)
{
GX_TEXT_INPUT_CURSOR *cursor_ptr = &text_input -> gx_multi_line_text_input_cursor_instance;
GX_POINT              old_cursor_pos;
GX_POINT              cursor_pos;
GX_RECTANGLE          cursor_rect;
INT                   shift;
UINT                  start_mark = text_input -> gx_multi_line_text_input_start_mark;
UINT                  end_mark = text_input -> gx_multi_line_text_input_end_mark;

    old_cursor_pos = cursor_ptr -> gx_text_input_cursor_pos;
    if (start_mark == end_mark)
    {
        start_mark = text_input -> gx_multi_line_text_input_text_insert_position;
        end_mark = text_input -> gx_multi_line_text_input_text_insert_position;

        text_input -> gx_multi_line_text_input_start_mark = start_mark;
        text_input -> gx_multi_line_text_input_end_mark = end_mark;
    }

    cursor_pos = old_cursor_pos;
    cursor_pos.gx_point_x = (GX_VALUE)(text_input -> gx_window_client.gx_rectangle_left +
                                       text_input -> gx_multi_line_text_view_whitespace);

    shift = text_input -> gx_multi_line_text_view_text_scroll_shift;

    /* Calculate new cursor position. */
    _gx_multi_line_text_input_cursor_pos_calculate(text_input, cursor_pos);

    text_input -> gx_multi_line_text_input_end_mark = text_input -> gx_multi_line_text_input_text_insert_position;

    if (shift == text_input -> gx_multi_line_text_view_text_scroll_shift)
    {
        _gx_multi_line_text_input_text_rectangle_get(text_input, old_cursor_pos, cursor_ptr -> gx_text_input_cursor_pos, &cursor_rect);

        /* Mark highlight area as dirty. */
        _gx_system_dirty_partial_add((GX_WIDGET *)text_input, &cursor_rect);
    }
    else
    {
        _gx_system_dirty_mark((GX_WIDGET *)text_input);
    }

    return GX_SUCCESS;
}

