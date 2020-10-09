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
/**   Multi Line Text Input Management(Multi Line Text Input)             */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_utility.h"
#include "gx_window.h"
#include "gx_scrollbar.h"
#include "gx_multi_line_text_input.h"
#include "gx_multi_line_text_view.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_multi_line_text_input_char_remove               PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function removes a substring from a specified position         */
/*      of input buffer.                                                  */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text_input                            Multi line text input control */
/*                                            block                       */
/*    start_pos                             Specified position in input   */
/*                                            buffer                      */
/*    del_bytes                             The number of bytes that to   */
/*                                            be removed                  */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    memmove                               Move the rest of the string   */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_multi_line_text_input_backspace                                 */
/*    _gx_multi_line_text_input_delete                                    */
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
UINT _gx_multi_line_text_input_char_remove(GX_MULTI_LINE_TEXT_INPUT *text_input, UINT start_pos, UINT del_bytes)
{
UINT       string_size;
GX_CHAR   *input_buffer;

    input_buffer = (GX_CHAR *) text_input -> gx_multi_line_text_view_text.gx_string_ptr;
    string_size = text_input -> gx_multi_line_text_view_text.gx_string_length;
    memmove(input_buffer + start_pos, input_buffer + start_pos + del_bytes, string_size - start_pos - del_bytes);
    input_buffer[string_size - del_bytes] = '\0';

    /* Update the input string size and cursor position. */
    text_input -> gx_multi_line_text_view_text.gx_string_length -= del_bytes;

    /* Set the text modified flag to GX_TRUE. */
    text_input -> gx_multi_line_text_input_text_was_modified = GX_TRUE;

    return GX_SUCCESS;
}

