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
#include "gx_widget.h"
#include "gx_system.h"
#include "gx_window.h"
#include "gx_multi_line_text_view.h"
#include "gx_utility.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_multi_line_text_view_string_total_rows_compute  PORTABLE C      */
/*                                                           6.1.10       */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function gets the total rows of the text view text.            */
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
/*    _gx_widget_font_get                   Retrieve font                 */
/*    _gx_window_client_width_get           Get the client width          */
/*    _gx_multi_line_text_view_display_info_get                           */
/*                                          Obtain the information on     */
/*                                            a string                    */
/*    _gx_system_string_get                 Get string by resource id     */
/*    _gx_system_private_string_get         Get string pointer in         */
/*                                            dynamically copied string   */
/*                                            buffer                      */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_multi_line_text_input_cursor_pos_calculate                      */
/*    _gx_multi_line_text_input_cursor_pos_update                         */
/*    _gx_multi_line_text_view_text_id_set                                */
/*    _gx_multi_line_text_view_text_set                                   */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            added logic to calculate    */
/*                                            total lines for dynamic     */
/*                                            bidi text,                  */
/*                                            resulting in version 6.1    */
/*  01-31-2022     Ting Zhu                 Modified comment(s),          */
/*                                            updated with new bidi text  */
/*                                            reorder function call,      */
/*                                            resulting in version 6.1.10 */
/*                                                                        */
/**************************************************************************/
UINT _gx_multi_line_text_view_string_total_rows_compute(GX_MULTI_LINE_TEXT_VIEW *text_view)
{
UINT                    total_rows = 0;
GX_VALUE                client_width;
GX_MULTI_LINE_TEXT_INFO text_info;
UINT                    index = 0;
INT                     cache_index = -1;
GX_STRING               string;
UINT                    first_cache_line = text_view -> gx_multi_line_text_view_first_cache_line;
GX_FONT                *font;

#if defined(GX_DYNAMIC_BIDI_TEXT_SUPPORT)
GX_BIDI_TEXT_INFO           bidi_text_info;
GX_BIDI_RESOLVED_TEXT_INFO *next;
GX_CANVAS                  *canvas;
GX_DISPLAY                 *display;
#endif

    _gx_widget_font_get((GX_WIDGET *)text_view, text_view -> gx_multi_line_text_view_font_id, &font);

    if (!font)
    {
        return GX_FAILURE;
    }

    if (!text_view -> gx_multi_line_text_view_text.gx_string_length)
    {
        text_view -> gx_multi_line_text_view_first_cache_line = 0;
        text_view -> gx_multi_line_text_view_cache_size = 0;
        text_view -> gx_multi_line_text_view_text_total_rows = 0;
        text_view -> gx_multi_line_text_view_line_index[0] = '\0';
        text_view -> gx_multi_line_text_view_line_index_old = GX_FALSE;
        return GX_SUCCESS;
    }

    _gx_window_client_width_get((GX_WINDOW *)text_view, &client_width);

    client_width = (GX_VALUE)(client_width - (text_view -> gx_multi_line_text_view_whitespace << 1));

    if (text_view -> gx_multi_line_text_view_text_id)
    {
        _gx_widget_string_get_ext((GX_WIDGET *)text_view, text_view -> gx_multi_line_text_view_text_id, &string);
    }
    else
    {
        _gx_system_private_string_get(&text_view -> gx_multi_line_text_view_text, &string, text_view -> gx_widget_style);
    }

#if defined(GX_DYNAMIC_BIDI_TEXT_SUPPORT)
    if (_gx_system_bidi_text_enabled && (text_view -> gx_widget_type == GX_TYPE_MULTI_LINE_TEXT_VIEW))
    {
        if (!text_view -> gx_multi_line_text_view_bidi_resolved_text_info)
        {
            bidi_text_info.gx_bidi_text_info_display_width = (GX_VALUE)(client_width - 3);
            bidi_text_info.gx_bidi_text_info_font = font;
            bidi_text_info.gx_bidi_text_info_text = string;
            GX_UTILITY_TEXT_DIRECTION_GET(bidi_text_info.gx_bidi_text_info_direction, text_view, canvas, display);

            _gx_utility_bidi_paragraph_reorder_ext(&bidi_text_info, &text_view -> gx_multi_line_text_view_bidi_resolved_text_info);
        }

        next = text_view -> gx_multi_line_text_view_bidi_resolved_text_info;

        while (next)
        {
            total_rows += next -> gx_bidi_resolved_text_info_total_lines;
            next = next -> gx_bidi_resolved_text_info_next;
        }
    }
    else
    {
#endif
        /* Calculate the total rows of text view string. */
        while (index < text_view -> gx_multi_line_text_view_text.gx_string_length)
        {
            /* Save line index. */
            if ((total_rows >= first_cache_line) &&
                (total_rows < first_cache_line + GX_MULTI_LINE_INDEX_CACHE_SIZE))
            {
                cache_index = (GX_UBYTE)(total_rows - first_cache_line);
                text_view -> gx_multi_line_text_view_line_index[cache_index] = index;
            }

            /* Calculate some information used to draw the text. */
            _gx_multi_line_text_view_display_info_get(text_view, index, text_view -> gx_multi_line_text_view_text.gx_string_length, &text_info, (GX_VALUE)(client_width - 2));

            total_rows++;
            index += text_info.gx_text_display_number;
        }

        if ((string.gx_string_ptr[index - 1] == GX_KEY_CARRIAGE_RETURN) ||
            (string.gx_string_ptr[index - 1] == GX_KEY_LINE_FEED))
        {
            /* Save line index. */
            if (total_rows < first_cache_line + GX_MULTI_LINE_INDEX_CACHE_SIZE)
            {
                cache_index = (GX_UBYTE)(total_rows - first_cache_line);
                text_view -> gx_multi_line_text_view_line_index[cache_index] = index;
            }

            total_rows++;
        }

        text_view -> gx_multi_line_text_view_cache_size = (GX_UBYTE)(cache_index + 1);
#if defined(GX_DYNAMIC_BIDI_TEXT_SUPPORT)
    }
#endif

    text_view -> gx_multi_line_text_view_text_total_rows = total_rows;
    text_view -> gx_multi_line_text_view_line_index_old = GX_FALSE;

    return GX_SUCCESS;
}

