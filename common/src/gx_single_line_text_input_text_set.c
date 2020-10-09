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
/**   Single Line Text Input Managment (Single Line Text Input)           */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_single_line_text_input.h"
#include "gx_utility.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_single_line_text_input_text_set                 PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION (deprecated)                                              */
/*                                                                        */
/*    This function assigns a text string to a single line text input     */
/*    widget.                                                             */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text_input                            Single-line text input widget */
/*                                            control block               */
/*    text                                  Null-terminated text string   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_string_length_check       Test string length            */
/*    _gx_single_line_text_input_text_set_ext                             */
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
UINT _gx_single_line_text_input_text_set(GX_SINGLE_LINE_TEXT_INPUT *text_input, GX_CONST GX_CHAR *text)
{
UINT      status = GX_SUCCESS;
GX_STRING string;

    string.gx_string_ptr = text;

    if (text)
    {
        status = _gx_utility_string_length_check(text, &string.gx_string_length, GX_MAX_STRING_LENGTH);
    }
    else
    {
        string.gx_string_length = 0;
    }

    if (status == GX_SUCCESS)
    {
        status = _gx_single_line_text_input_text_set_ext(text_input, &string);
    }

    return(status);
}
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_single_line_text_input_text_set_ext             PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function assigns a text string to a single line text input     */
/*    widget.                                                             */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text_input                            Single-line text input widget */
/*                                            control block               */
/*    text                                  GX_STRING type text string    */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_string_length_check       Test string length            */
/*    memcpy                                Copy block of memory          */
/*    _gx_utility_utf8_string_character_get Retrieve glyph code and length*/
/*    _gx_single_line_text_input_position_update                          */
/*                                          Update cursor position        */
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
UINT _gx_single_line_text_input_text_set_ext(GX_SINGLE_LINE_TEXT_INPUT *text_input, GX_CONST GX_STRING *text)
{
UINT copy_size;

#if defined GX_UTF8_SUPPORT
GX_STRING string;
UINT      index;
UINT      glyph_len = 0;
#endif

    if (text && text -> gx_string_ptr)
    {

        copy_size = text_input -> gx_single_line_text_input_buffer_size - 1;

        if (text -> gx_string_length > copy_size)
        {
#if defined(GX_UTF8_SUPPORT)
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

        memcpy((GX_CHAR *)text_input -> gx_single_line_text_input_buffer, text -> gx_string_ptr, copy_size); /* Use case of memcpy is verified. */
    }
    else
    {
        copy_size = 0;
    }

    ((GX_CHAR *)text_input -> gx_single_line_text_input_buffer)[copy_size] = '\0';

    text_input -> gx_single_line_text_input_string_size = copy_size;
    text_input -> gx_single_line_text_input_insert_pos = copy_size;

    /* Update cursor position. */
    _gx_single_line_text_input_position_update(text_input);

    if (text_input -> gx_widget_status & GX_STATUS_VISIBLE)
    {
        _gx_system_dirty_mark((GX_WIDGET *)text_input);
    }

    return(GX_SUCCESS);
}

