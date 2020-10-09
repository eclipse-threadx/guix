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
UINT _gx_multi_line_text_input_text_rectangle_get(GX_MULTI_LINE_TEXT_INPUT *input, GX_POINT start_cursor_pos, GX_POINT end_cursor_pos, GX_RECTANGLE *rect)
{
GX_FONT *font;
GX_VALUE border_width;
GX_VALUE half_line_height;
GX_VALUE cursor_width;


    /* Pick up font. */
    _gx_widget_font_get((GX_WIDGET *)input, input -> gx_multi_line_text_view_font_id, &font);

    if (!font)
    {
        return GX_FAILURE;
    }

    /* Pickup widget border width. */
    _gx_widget_border_width_get((GX_WIDGET *)input, &border_width);

    /* Get client rectangle. */
    _gx_widget_client_get((GX_WIDGET *)input, border_width, rect);

    half_line_height = (GX_VALUE)(font -> gx_font_line_height + input -> gx_multi_line_text_view_line_space);
    half_line_height = (GX_VALUE)(half_line_height >> 1);

    if (start_cursor_pos.gx_point_y == end_cursor_pos.gx_point_y)
    {
        if (start_cursor_pos.gx_point_x > end_cursor_pos.gx_point_x)
        {
            GX_SWAP_VALS(start_cursor_pos.gx_point_x, end_cursor_pos.gx_point_x);
        }

        cursor_width = input -> gx_multi_line_text_input_cursor_instance.gx_text_input_cursor_width;
        rect -> gx_rectangle_top = (GX_VALUE)(start_cursor_pos.gx_point_y - half_line_height);
        rect -> gx_rectangle_bottom = (GX_VALUE)(start_cursor_pos.gx_point_y + half_line_height);
        rect -> gx_rectangle_left = (GX_VALUE)(start_cursor_pos.gx_point_x - (cursor_width >> 1));
        rect -> gx_rectangle_right = (GX_VALUE)(end_cursor_pos.gx_point_x + ((cursor_width + 1) >> 1) - 1);
    }
    else
    {
        if (start_cursor_pos.gx_point_y > end_cursor_pos.gx_point_y)
        {
            GX_SWAP_VALS(start_cursor_pos.gx_point_y, end_cursor_pos.gx_point_y);
        }
        rect -> gx_rectangle_top = (GX_VALUE)(start_cursor_pos.gx_point_y - half_line_height);
        rect -> gx_rectangle_bottom = (GX_VALUE)(end_cursor_pos.gx_point_y + half_line_height);
    }

    return GX_SUCCESS;
}

