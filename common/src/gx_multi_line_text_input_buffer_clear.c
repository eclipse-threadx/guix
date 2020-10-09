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
#include "gx_scrollbar.h"
#include "gx_multi_line_text_view.h"
#include "gx_multi_line_text_input.h"
#include "gx_window.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_multi_line_text_input_buffer_clear              PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function deletes all characters from the text input buffer.    */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text_input                            Multi-line text input widget  */
/*                                            control blcok               */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*   _gx_multi_line_text_input_char_remove  Remove characters from input  */
/*                                            buffer                      */
/*   _gx_multi_line_text_view_string_total_rows_compute                   */
/*                                          Calculate total rows          */
/*   _gx_multi_line_text_input_cursor_pos_update                          */
/*                                          Update cursor position        */
/*   _gx_window_scrollbar_find              Find scrollbar for a window   */
/*   _gx_scrollbar_reset                    Reset scroll bar information  */
/*   _gx_system_dirty_mark                  Mark the widget area as dirty */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
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
UINT _gx_multi_line_text_input_buffer_clear(GX_MULTI_LINE_TEXT_INPUT *text_input)
{
GX_SCROLLBAR *scroll;

    if (text_input -> gx_multi_line_text_view_text.gx_string_length != 0)
    {
        _gx_multi_line_text_input_char_remove(text_input, 0, text_input -> gx_multi_line_text_view_text.gx_string_length);

        /* Initiate line cache information. */
        text_input -> gx_multi_line_text_view_first_cache_line = 0;
        memset(text_input -> gx_multi_line_text_view_line_index, 0, sizeof(UINT) * GX_MULTI_LINE_INDEX_CACHE_SIZE);

        /* Calculate new text rows. */
        _gx_multi_line_text_view_string_total_rows_compute((GX_MULTI_LINE_TEXT_VIEW *)text_input);

        text_input -> gx_multi_line_text_input_text_insert_position = 0;

        _gx_window_scrollbar_find((GX_WINDOW *)text_input, GX_TYPE_VERTICAL_SCROLL, &scroll);
        if (scroll)
        {
            /* Reset scrollbar.  */
            _gx_scrollbar_reset(scroll, GX_NULL);
        }

        _gx_multi_line_text_input_cursor_pos_update(text_input, GX_TRUE);

        /* Mark the widget area dirty. */
        _gx_system_dirty_mark((GX_WIDGET *)text_input);
    }

    return GX_SUCCESS;
}

