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
#include "gx_multi_line_text_view.h"
#include "gx_multi_line_text_input.h"
#include "gx_utility.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_multi_line_text_input_cursor_visible            PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function scrolls multi line text input text to make the cursor */
/*    visible.                                                            */
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
/*    _gx_window_scrollbar_find             Find scrollbar for a window   */
/*    _gx_scrollbar_reset                   Reset scrollbar information   */
/*    _gx_multi_line_text_view_line_cache_update                          */
/*                                          Update line index cache       */
/*    _gx_system_dirty_mark                 Mark widget as dirty          */
/*    _gx_utility_rectangle_resize          Increase the size of rectangle*/
/*                                            as specified                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    GUIX Intercal Code                                                  */
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

UINT _gx_multi_line_text_input_cursor_visible(GX_MULTI_LINE_TEXT_INPUT *text_input)
{
GX_TEXT_INPUT_CURSOR *cursor_ptr = &text_input -> gx_multi_line_text_input_cursor_instance;
GX_RECTANGLE          client;
INT                   shift;
GX_SCROLLBAR         *scroll;
GX_FONT              *font;
GX_VALUE              line_height;
GX_VALUE              ypos;

    _gx_widget_font_get((GX_WIDGET *)text_input, text_input -> gx_multi_line_text_view_font_id, &font);

    if (!font)
    {
        return GX_FAILURE;
    }

    client = text_input -> gx_window_client;

    if (text_input -> gx_multi_line_text_view_whitespace)
    {
        /* Offset client bounding box.  */
        _gx_utility_rectangle_resize(&client, (GX_VALUE)(-text_input -> gx_multi_line_text_view_whitespace));
    }

    line_height = (GX_VALUE)(font -> gx_font_line_height + text_input -> gx_multi_line_text_view_line_space);

    ypos = (GX_VALUE)(cursor_ptr -> gx_text_input_cursor_pos.gx_point_y - (line_height >> 1));

    if (ypos < client.gx_rectangle_top)
    {
        shift = client.gx_rectangle_top - ypos;
    }
    else if (ypos + line_height - 1 > client.gx_rectangle_bottom)
    {
        shift = client.gx_rectangle_bottom - (ypos + line_height - 1);
    }
    else
    {
        shift = 0;
    }

    if (shift)
    {
        text_input -> gx_multi_line_text_view_text_scroll_shift += shift;
        cursor_ptr -> gx_text_input_cursor_pos.gx_point_y = (GX_VALUE)(cursor_ptr -> gx_text_input_cursor_pos.gx_point_y + shift);

        _gx_window_scrollbar_find((GX_WINDOW *)text_input, GX_TYPE_VERTICAL_SCROLL, &scroll);
        if (scroll)
        {
            /* Reset scrollbar.  */
            _gx_scrollbar_reset(scroll, GX_NULL);
        }
        else
        {

            if (text_input -> gx_multi_line_text_view_text_total_rows >
                text_input -> gx_multi_line_text_view_cache_size)
            {
                _gx_multi_line_text_view_line_cache_update((GX_MULTI_LINE_TEXT_VIEW *)text_input);
            }
        }

        _gx_system_dirty_mark((GX_WIDGET *)text_input);
    }

    return GX_SUCCESS;
}

