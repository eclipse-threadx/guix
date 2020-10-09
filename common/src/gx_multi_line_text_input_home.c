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
/*    _gx_multi_line_text_input_home                      PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function move the cursor to the position                       */
/*    before the first character.                                         */
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
/*    _gx_multi_line_text_input_cursor_pos_calculate                      */
/*                                          Calculate cursor position     */
/*                                            according to click point    */
/*    _gx_multi_line_text_input_cursor_rectangle_define                   */
/*                                          Define a rectangle for the    */
/*                                            cursor                      */
/*    _gx_system_dirty_partial_add          Add one dirty area to         */
/*                                            dirty list                  */
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
UINT _gx_multi_line_text_input_home(GX_MULTI_LINE_TEXT_INPUT *text_input)
{
GX_TEXT_INPUT_CURSOR *cursor_ptr = &text_input -> gx_multi_line_text_input_cursor_instance;
GX_POINT              cur_pos;
GX_RECTANGLE          cur_rect;

    if (text_input -> gx_multi_line_text_input_start_mark != text_input -> gx_multi_line_text_input_end_mark)
    {
        _gx_multi_line_text_input_left_arrow(text_input);
    }

    cur_pos = cursor_ptr -> gx_text_input_cursor_pos;

    _gx_text_input_cursor_dirty_rectangle_get(&text_input -> gx_multi_line_text_input_cursor_instance, &cur_rect);

    cur_pos.gx_point_x = (GX_VALUE)(text_input -> gx_window_client.gx_rectangle_left +
                                    text_input -> gx_multi_line_text_view_whitespace);

    /* Calculate new cursor position. */
    _gx_multi_line_text_input_cursor_pos_calculate(text_input, cur_pos);

    if (cur_rect.gx_rectangle_left != cursor_ptr -> gx_text_input_cursor_pos.gx_point_x)
    {
        /* Mark old cursor rectangle dirty. */
        _gx_system_dirty_partial_add((GX_WIDGET *)text_input, &cur_rect);

        /* Mark new cursor rectangle dirty. */
        _gx_text_input_cursor_dirty_rectangle_get(&text_input -> gx_multi_line_text_input_cursor_instance, &cur_rect);
        _gx_system_dirty_partial_add((GX_WIDGET *)text_input, &cur_rect);
    }

    return GX_SUCCESS;
}

