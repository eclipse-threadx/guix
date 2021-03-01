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
#include "gx_canvas.h"
#include "gx_rich_text_view.h"
#include "gx_utility.h"
#include "gx_widget.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_rich_text_view_line_info_get                    PORTABLE C      */
/*                                                           6.1.5        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function prepares one line text for drawing.                   */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text_view                              Rich text view control block */
/*    text                                   String to process            */
/*    line_info                              Retrieved line information   */
/*                                             for drawing next line      */
/*    availlable_width                       Availlable width for drawing */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_font_get                   Retireve font by id           */
/*    _gx_rich_text_view_tag_enter          Process rich text tag         */
/*    _gx_utility_utf8_string_character_get Retrieve glyph length         */
/*    _gx_system_string_width_get           Retrieve string width         */
/*    _gx_system_rich_text_format_stack_clear                             */
/*                                          Clear rich text format stack  */
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
/*  03-02-2021     Ting Zhu                 Modified comment(s),          */
/*                                            removed unreachable code,   */
/*                                            resulting in version 6.1.5  */
/*                                                                        */
/**************************************************************************/
UINT  _gx_rich_text_view_line_info_get(GX_RICH_TEXT_VIEW *text_view, GX_STRING text, GX_RICH_TEXT_LINE_INFO *line_info, GX_VALUE availlable_width)
{
GX_STRING              string;
GX_FONT               *font;
UINT                   glyph_len = 1;
GX_VALUE               glyph_width;
GX_RICH_TEXT_LINE_INFO break_info;
GX_RICH_TEXT_FORMAT    text_format;
GX_UBYTE               processed_count;
GX_BOOL                escape = GX_FALSE;
GX_RESOURCE_ID         font_id;
INT                    tail_space_width = 0;

    text_format = line_info -> gx_rich_text_line_info_start_format;
    break_info = *line_info;
    font_id = text_format.gx_rich_text_font_id;

    /* Pickup draw font. */
    _gx_widget_font_get((GX_WIDGET *)text_view, text_format.gx_rich_text_font_id, &font);

    if (!font)
    {
        return GX_INVALID_FONT;
    }

    /* Calculate the total rows of text view string. */
    while (text.gx_string_length > 0)
    {
        if ((!escape) && (text.gx_string_ptr[0] == '\\'))
        {
            escape = GX_TRUE;
            text.gx_string_ptr++;
            text.gx_string_length--;
            line_info -> gx_rich_text_line_info_text.gx_string_length++;
            continue;
        }

        /* Test rich text tags. */
        if ((!escape) &&
            (text.gx_string_ptr[0] == '<') &&
            (_gx_rich_text_view_tag_enter(text_view, &text, &text_format, &processed_count) == GX_SUCCESS))
        {
            if (text_format.gx_rich_text_font_id != font_id)
            {
                /* Font changed. */
                _gx_widget_font_get((GX_WIDGET *)text_view, text_format.gx_rich_text_font_id, &font);

                if (!font)
                {
                    return GX_INVALID_FONT;
                }

                /* Calculate maximum font height for one line text. */
                if (line_info -> gx_rich_text_line_info_line_height < font -> gx_font_line_height)
                {
                    line_info -> gx_rich_text_line_info_line_height = font -> gx_font_line_height;
                    line_info -> gx_rich_text_line_info_baseline = font -> gx_font_baseline;
                }

                font_id = text_format.gx_rich_text_font_id;
            }

            line_info -> gx_rich_text_line_info_text.gx_string_length += processed_count;
            line_info -> gx_rich_text_line_info_end_format = text_format;

            text.gx_string_ptr += processed_count;
            text.gx_string_length -= processed_count;
            continue;
        }

#if defined GX_UTF8_SUPPORT
        string = text;

        /* Pick up glyph length. */
        if (_gx_utility_utf8_string_character_get(&string, GX_NULL, &glyph_len) != GX_SUCCESS)
        {
            /* Invalid UTF8 string. */
            return GX_INVALID_STRING;
        }
#endif
        string.gx_string_ptr = text.gx_string_ptr;
        string.gx_string_length = glyph_len;

        /* Calculate glyph width. */
        _gx_system_string_width_get_ext(font, &string, &glyph_width);

        /* Calculate the successive space width in the end of the line. */
        if ((text.gx_string_ptr[0] == ' '))
        {
            tail_space_width += glyph_width;
        }
        else
        {
            tail_space_width = 0;
        }

        if (text.gx_string_ptr[0] == GX_KEY_CARRIAGE_RETURN)
        {
            /* Line break charater \r. */
            glyph_len = 1;

            if ((text.gx_string_length > 1) &&
                (text.gx_string_ptr[1] == GX_KEY_LINE_FEED))
            {
                /* Line break character \r\n. */
                glyph_len++;
            }

            line_info -> gx_rich_text_line_info_text.gx_string_length += glyph_len;
            break;
        }
        else if (text.gx_string_ptr[0] == GX_KEY_LINE_FEED)
        {
            /* Line break character \n. */
            line_info -> gx_rich_text_line_info_text.gx_string_length += glyph_len;
            break;
        }
        else if (((UINT)(line_info -> gx_rich_text_line_info_text_width + (USHORT)glyph_width) > (UINT)availlable_width) &&
                 line_info -> gx_rich_text_line_info_text_width && (text.gx_string_ptr[0] != ' '))
        {
            if (break_info.gx_rich_text_line_info_text_width == 0)
            {
                break;
            }

            *line_info = break_info;
            break;
        }

        /* Increase text width by glyph width. */
        line_info -> gx_rich_text_line_info_text_width = (UINT)(line_info -> gx_rich_text_line_info_text_width + (USHORT)glyph_width);
        line_info -> gx_rich_text_line_info_text.gx_string_length += glyph_len;

        if ((text.gx_string_ptr[0] == ' ') || (text.gx_string_ptr[0] == ',') || (text.gx_string_ptr[0] == ';'))
        {
            /* Update line info when encounter word break character. */
            break_info = *line_info;

            if (tail_space_width)
            {
                break_info.gx_rich_text_line_info_text_width -= (UINT)tail_space_width;
            }
        }

        /* Increaset next draw text length. */
        text.gx_string_ptr += glyph_len;
        text.gx_string_length -= glyph_len;

        escape = GX_FALSE;
    }

    return GX_SUCCESS;
}

