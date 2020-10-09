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
/**   Multi Line Text Input Managment (Multi Line Text Input)             */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_multi_line_text_view.h"
#include "gx_multi_line_text_input.h"
#include "gx_scrollbar.h"
#include "gx_window.h"
#include "gx_widget.h"
#include "gx_utility.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_multi_line_text_input_new_line_character_get    PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function to retrieve the new line character of the  */
/*    input text.                                                         */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text_view                             Multi-line text input widget  */
/*                                            control block               */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    memcpy                                Copy block of memory          */
/*    _gx_utility_utf8_string_character_get Parse utf8 string to          */
/*                                            multi-byte glyph            */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_multi_line_text_input_text_set                                  */
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
static VOID _gx_multi_line_text_input_new_line_character_get(GX_MULTI_LINE_TEXT_INPUT *text_input)
{
GX_STRING         string;
GX_CONST GX_CHAR *ch;

#ifdef GX_UTF8_SUPPORT
UINT glyph_len;
#endif

    string = text_input -> gx_multi_line_text_view_text;

    while (string.gx_string_length > 0)
    {
        ch = string.gx_string_ptr;

#ifdef GX_UTF8_SUPPORT
        _gx_utility_utf8_string_character_get(&string, GX_NULL, &glyph_len);
#else
        string.gx_string_ptr++;
        string.gx_string_length--;
#endif /* GX_UTF8_SUPPORT */

        if (ch[0] == GX_KEY_CARRIAGE_RETURN)
        {
            if (ch[1] == GX_KEY_LINE_FEED)
            {
                memcpy(text_input -> gx_multi_line_text_input_new_line_character, "\r\n", 3); /* Use case of memcpy is verified. */
                text_input -> gx_multi_line_text_input_new_line_character_size = 2;
            }
            else
            {
                memcpy(text_input -> gx_multi_line_text_input_new_line_character, "\r", 2); /* Use case of memcpy is verified. */
                text_input -> gx_multi_line_text_input_new_line_character_size = 1;
            }

            break;
        }
        else if (ch[0] == GX_KEY_LINE_FEED)
        {
            memcpy(text_input -> gx_multi_line_text_input_new_line_character, "\n", 2); /* Use case of memcpy is verified. */
            text_input -> gx_multi_line_text_input_new_line_character_size = 1;
            break;
        }
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_multi_line_text_input_text_set                  PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION (Deprecated)                                              */
/*                                                                        */
/*    This function assigns a text string to a multi line text input      */
/*    widget.                                                             */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text_view                             Multi-line text input widget  */
/*                                            control block               */
/*    text                                  Null-terminated text string   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_string_length_check       Calculate string length       */
/*    _gx_multi_line_text_input_text_set_ext                              */
/*                                          New text set function         */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
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
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gx_multi_line_text_input_text_set(GX_MULTI_LINE_TEXT_INPUT *text_input, GX_CONST GX_CHAR *text)
{
UINT      status;
UINT      str_len;
GX_STRING new_string;

    new_string.gx_string_ptr = text;
    status = _gx_utility_string_length_check(text, &str_len, GX_MAX_STRING_LENGTH);
    if (status == GX_SUCCESS)
    {
        new_string.gx_string_length = str_len;
        status = _gx_multi_line_text_input_text_set_ext(text_input, &new_string);
    }

    return(status);
}
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_multi_line_text_input_text_set_ext              PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function assigns a text string to a multi line text input      */
/*    widget.                                                             */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text_view                             Multi-line text input widget  */
/*                                            control block               */
/*    text                                  Null-terminated text string   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_multi_line_text_input_new_line_character_get                    */
/*                                          Get the new line character    */
/*    _gx_multi_line_text_view_string_total_rows_compute                  */
/*                                          Compute the total rows of     */
/*                                            the input text              */
/*    _gx_multi_line_text_view_line_cache_update                          */
/*                                          Update line cache             */
/*    _gx_window_scrollbar_find             Find scrollbar for a window   */
/*    _gx_scrollbar_reset                   Reset scrollbar information   */
/*    _gx_system_dirty_mark                 Mark the widget dirty         */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
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
UINT _gx_multi_line_text_input_text_set_ext(GX_MULTI_LINE_TEXT_INPUT *text_input, GX_CONST GX_STRING *text)
{
GX_SCROLLBAR *scroll;
UINT          copy_size;

#if defined GX_UTF8_SUPPORT
GX_STRING string;
UINT      index;
UINT      glyph_len = 0;
#endif

    if (text && text -> gx_string_ptr)
    {
        copy_size = text_input -> gx_multi_line_text_input_buffer_size - 1;

        if (text -> gx_string_length > copy_size - 1)
        {
#if defined GX_UTF8_SUPPORT
            string.gx_string_ptr = text -> gx_string_ptr;
            string.gx_string_length = copy_size;

            index = 0;

            /* Calculate maximum bytes that can be copied. */
            while (_gx_utility_utf8_string_character_get(&string, GX_NULL, &glyph_len) == GX_SUCCESS)
            {
                index += glyph_len;
            }
            copy_size = index;
#endif
        }
        else
        {
            copy_size = text -> gx_string_length;
        }

        memcpy((GX_CHAR *)text_input -> gx_multi_line_text_view_text.gx_string_ptr, text -> gx_string_ptr, copy_size); /* Use case of memcpy is verified. */
        text_input -> gx_multi_line_text_view_text.gx_string_length = copy_size;
    }
    else
    {
        copy_size = 0;
    }

    ((GX_CHAR *)text_input -> gx_multi_line_text_view_text.gx_string_ptr)[copy_size] = '\0';

    text_input -> gx_multi_line_text_view_text.gx_string_length = copy_size;

    /* Get new line character. */
    _gx_multi_line_text_input_new_line_character_get(text_input);

    if (text_input -> gx_widget_status & GX_STATUS_VISIBLE)
    {
        /* Compute the total rows of input string. */
        _gx_multi_line_text_view_string_total_rows_compute((GX_MULTI_LINE_TEXT_VIEW *)text_input);

        text_input -> gx_multi_line_text_input_text_insert_position = copy_size;

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
                /* Update line cache. */
                _gx_multi_line_text_view_line_cache_update((GX_MULTI_LINE_TEXT_VIEW *)text_input);
            }
        }

        /* Update cursor position. */
        _gx_multi_line_text_input_cursor_pos_update(text_input, GX_TRUE);

        _gx_system_dirty_mark((GX_WIDGET *)text_input);
    }
    else
    {
        text_input -> gx_multi_line_text_view_line_index_old = GX_TRUE;
        text_input -> gx_multi_line_text_input_text_insert_position = 0;
    }
    return(GX_SUCCESS);
}

