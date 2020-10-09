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
/**   Multi Line Text View Management (Multi Line Text View)              */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_multi_line_text_view.h"
#include "gx_utility.h"
#include "gx_widget.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_multi_line_text_view_display_info_get           PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function gets the length and pixel width of one word or        */
/*      some characters can be displayed in one line.                     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text_view                             Multi line text view          */
/*                                            control block               */
/*    start_index                           Start character position in   */
/*                                            the input buffer            */
/*    end_index                             End character position in the */
/*                                            input buffer                */
/*    text_info                             Text and word information to  */
/*                                            be returned                 */
/*    available_width                       Width available to display    */
/*                                            text in current line        */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_font_get                   Retrieve font                 */
/*    _gx_system_string_width_get           Get the width of a string     */
/*    _gx_system_private_string_get         Get string pointer in a       */
/*                                            dynamically copied string   */
/*                                            buffer                      */
/*    _gx_utility_utf8_string_character_get Parse utf8 string to          */
/*                                            multi-byte glyph            */
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
/*                                            support line break status,  */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
VOID _gx_multi_line_text_view_display_info_get(GX_MULTI_LINE_TEXT_VIEW *text_view, UINT start_index,
                                               UINT end_index, GX_MULTI_LINE_TEXT_INFO *text_info, GX_VALUE available_width)
{
#ifdef GX_UTF8_SUPPORT
UINT      current_index = start_index;
#endif
GX_STRING string;
GX_STRING ch;
GX_VALUE  char_width;
USHORT    display_width;
USHORT    display_number;
UINT      glyph_len = 1;
GX_FONT  *font;

    text_info -> gx_text_display_width = 0;
    text_info -> gx_text_display_number = 0;

    if (text_view -> gx_multi_line_text_view_text_id)
    {
        _gx_widget_string_get_ext((GX_WIDGET *)text_view, text_view -> gx_multi_line_text_view_text_id, &string);
    }
    else
    {
        _gx_system_private_string_get(&text_view -> gx_multi_line_text_view_text, &string, text_view -> gx_widget_style);
    }


    string.gx_string_ptr += start_index;
    string.gx_string_length = end_index - start_index;

    if (text_view -> gx_widget_status & GX_STATUS_LINE_BREAK_PROCESSED)
    {
        /* Line breaking alreay preprocessed, only need to search for line break characters to do line break. */
        while (string.gx_string_length > 0)
        {
            if (string.gx_string_ptr[0] == GX_KEY_CARRIAGE_RETURN)
            {
                text_info -> gx_text_display_number++;

                if ((string.gx_string_length > 1) && string.gx_string_ptr[1] == GX_KEY_LINE_FEED)
                {
                    text_info -> gx_text_display_number++;
                }
                break;
            }
            else if (string.gx_string_ptr[0] == GX_KEY_LINE_FEED)
            {
                text_info -> gx_text_display_number++;
                break;
            }
            else
            {
                text_info -> gx_text_display_number++;
            }

            string.gx_string_length--;
            string.gx_string_ptr++;
        }
    }
    else
    {
        _gx_widget_font_get((GX_WIDGET *)text_view, text_view -> gx_multi_line_text_view_font_id, &font);

        if (!font)
        {
            return;
        }

        display_width = 0;
        display_number = 0;

        while (string.gx_string_length > 0)
        {
            ch = string;

#ifdef GX_UTF8_SUPPORT
            _gx_utility_utf8_string_character_get(&string, GX_NULL, &glyph_len);
            current_index += glyph_len;
#else
            string.gx_string_ptr++;
            string.gx_string_length--;
#endif /* GX_UTF8_SUPPORT */

            ch.gx_string_length = glyph_len;
            _gx_system_string_width_get_ext(font, &ch, &char_width);

            if (ch.gx_string_ptr[0] == GX_KEY_CARRIAGE_RETURN)
            {
                if (ch.gx_string_ptr[1] == GX_KEY_LINE_FEED)
                {
                    text_info -> gx_text_display_number = (USHORT)(text_info -> gx_text_display_number + 2);
                }
                else
                {
                    text_info -> gx_text_display_number++;
                }
                break;
            }
            else if (ch.gx_string_ptr[0] == GX_KEY_LINE_FEED)
            {
                text_info -> gx_text_display_number++;
                break;
            }
            else if (((text_info -> gx_text_display_width + char_width) > available_width - 1) &&
                     (text_info -> gx_text_display_number > 0) &&
                     (ch.gx_string_ptr[0] != ' '))
            {
                if (display_number == 0)
                {
                    break;
                }
                text_info -> gx_text_display_width = display_width;
                text_info -> gx_text_display_number = display_number;
                break;
            }
            text_info -> gx_text_display_width = (USHORT)(text_info -> gx_text_display_width + char_width);
            text_info -> gx_text_display_number = (USHORT)(text_info -> gx_text_display_number + glyph_len);

            if ((ch.gx_string_ptr[0] == ' ') || (ch.gx_string_ptr[0] == ',') || (ch.gx_string_ptr[0] == ';'))
            {
                display_width = text_info -> gx_text_display_width;
                display_number = text_info -> gx_text_display_number;
            }
        }
    }
}

