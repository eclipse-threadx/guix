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
/**   Rich Text View Management (Rich Text View)                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_window.h"
#include "gx_widget.h"
#include "gx_utility.h"
#include "gx_rich_text_view.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_rich_text_view_scroll_info_get                  PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function gets the rich text view scroll information.           */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text_view                            Rich text view control block   */
/*    style                                Reserved                       */
/*    info                                 Pointer to destination for     */
/*                                           scroll info                  */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_font_get                   Obtain the font               */
/*    _gx_multi_line_text_view_string_total_rows_computer                 */
/*                                          Compute the number of rows    */
/*                                            for the text view text      */
/*    _gx_utility_rectangle_resize          Offset rectangle              */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    GUIX Internal Code                                                  */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  09-30-2020     Kenneth Maxwell          Initial Version 6.1           */
/*                                                                        */
/**************************************************************************/
UINT _gx_rich_text_view_scroll_info_get(GX_RICH_TEXT_VIEW *text_view, ULONG style, GX_SCROLL_INFO *info)
{
GX_RECTANGLE client;
GX_VALUE     line_height;
INT          text_height;
INT          value;
INT          shift;
GX_FONT     *font;

    GX_PARAMETER_NOT_USED(style);

    /* Get font. */
    _gx_widget_font_get((GX_WIDGET *)text_view, text_view -> gx_rich_text_view_fonts.gx_rich_text_fonts_normal_id, &font);

    if (!font)
    {
        return GX_FAILURE;
    }

    if (text_view -> gx_multi_line_text_view_line_index_old)
    {
        /* Calculate text total height. */
        _gx_rich_text_view_text_total_height_calculate(text_view);
    }

    /* Pickup text height. */
    line_height = (GX_VALUE)(font -> gx_font_line_height + text_view -> gx_multi_line_text_view_line_space);
    text_height = (INT)text_view -> gx_rich_text_view_text_total_height;

    client = text_view -> gx_window_client;

    if (text_view -> gx_multi_line_text_view_whitespace)
    {
        /* Offset client bounding box.  */
        _gx_utility_rectangle_resize(&client, (GX_VALUE)(-text_view -> gx_multi_line_text_view_whitespace));
    }

    info -> gx_scroll_minimum = client.gx_rectangle_top;
    info -> gx_scroll_maximum = info -> gx_scroll_minimum + text_height - 1;
    info -> gx_scroll_visible = (GX_VALUE)(client.gx_rectangle_bottom - client.gx_rectangle_top + 1);

    if (text_height < info -> gx_scroll_visible)
    {
        info -> gx_scroll_maximum = info -> gx_scroll_minimum + info -> gx_scroll_visible - 1;
    }

    shift = text_view -> gx_multi_line_text_view_text_scroll_shift;
    value = client.gx_rectangle_top - shift;

    if (value < info -> gx_scroll_minimum)
    {
        value = info -> gx_scroll_minimum;
    }
    else if (value > info -> gx_scroll_maximum - info -> gx_scroll_visible + 1)
    {
        value = info -> gx_scroll_maximum - info -> gx_scroll_visible + 1;
    }

    shift = client.gx_rectangle_top - value;

    if (shift != text_view -> gx_multi_line_text_view_text_scroll_shift)
    {
        /* Update shift value.  */
        text_view -> gx_multi_line_text_view_text_scroll_shift = shift;
    }

    info -> gx_scroll_value = value;
    info -> gx_scroll_increment = line_height;

    return GX_SUCCESS;
}

