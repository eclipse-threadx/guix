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
#include "gx_widget.h"
#include "gx_system.h"
#include "gx_window.h"
#include "gx_rich_text_view.h"
#include "gx_utility.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_rich_text_view_text_total_height_calculate      PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function calculates rich text view text total height.          */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text_view                             Multi line text view          */
/*                                            control block               */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_rectangle_resize          Resize rectangle              */
/*    _gx_widget_string_get_ext             Detect and process rich text  */
/*                                            tag                         */
/*    _gx_widget_font_get                   Retrieve font by id           */
/*    _gx_utility_utf8_string_character_get Retrieve glyph length         */
/*    _gx_system_string_width_get           Get string width              */
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
UINT _gx_rich_text_view_text_total_height_calculate(GX_RICH_TEXT_VIEW *text_view)
{
GX_STRING              text;
GX_FONT               *font;
GX_RICH_TEXT_LINE_INFO line_info;
GX_RICH_TEXT_FORMAT    format;
GX_VALUE               client_width;
GX_RECTANGLE           client;
INT                    total_height = 0;

    if (!text_view -> gx_multi_line_text_view_text.gx_string_length)
    {
        text_view -> gx_rich_text_view_text_total_height = 0;
        text_view -> gx_multi_line_text_view_line_index_old = GX_FALSE;
        return GX_SUCCESS;
    }

    /* Pick up client retangle. */
    client = text_view -> gx_window_client;

    /* Offset client area by the size of whitespace.  */
    _gx_utility_rectangle_resize(&client, (GX_VALUE)(-text_view -> gx_multi_line_text_view_whitespace));

    /* Calculate text display width. */
    client_width = (GX_VALUE)(client.gx_rectangle_right - client.gx_rectangle_left - 1);

    /* Set default draw style. */
    format.gx_rich_text_font_id = text_view -> gx_rich_text_view_fonts.gx_rich_text_fonts_normal_id;
    format.gx_rich_text_flags = 0;

    /* Pick up text for drawing. */
    if (text_view -> gx_multi_line_text_view_text_id)
    {
        _gx_widget_string_get_ext((GX_WIDGET *)text_view, text_view -> gx_multi_line_text_view_text_id, &text);
    }
    else
    {
        _gx_system_private_string_get(&text_view -> gx_multi_line_text_view_text, &text, text_view -> gx_widget_style);
    }

    /* Calculate the total rows of text view string. */
    while (text.gx_string_length > 0)
    {
        /* Pickup draw font. */
        _gx_widget_font_get((GX_WIDGET *)text_view, format.gx_rich_text_font_id, &font);

        if (!font)
        {
            return GX_INVALID_FONT;
        }

        line_info.gx_rich_text_line_info_text.gx_string_ptr = text.gx_string_ptr;
        line_info.gx_rich_text_line_info_text.gx_string_length = 0;
        line_info.gx_rich_text_line_info_start_format = format;
        line_info.gx_rich_text_line_info_end_format = format;
        line_info.gx_rich_text_line_info_line_height = font -> gx_font_line_height;
        line_info.gx_rich_text_line_info_text_width = 0;

        _gx_rich_text_view_line_info_get(text_view, text, &line_info, client_width);

        total_height += line_info.gx_rich_text_line_info_line_height;
        total_height += text_view -> gx_multi_line_text_view_line_space;

        format = line_info.gx_rich_text_line_info_end_format;


        text.gx_string_ptr += line_info.gx_rich_text_line_info_text.gx_string_length;
        text.gx_string_length -= line_info.gx_rich_text_line_info_text.gx_string_length;
    }

    _gx_rich_text_view_context_reset();

    text_view -> gx_rich_text_view_text_total_height = (UINT)total_height;
    text_view -> gx_multi_line_text_view_line_index_old = GX_FALSE;
    return GX_SUCCESS;
}

