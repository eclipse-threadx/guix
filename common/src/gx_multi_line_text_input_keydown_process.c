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
#include "gx_multi_line_text_input.h"
#include "gx_utility.h"
#include "gx_window.h"
#include "gx_system.h"
#include "gx_multi_line_text_view.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_multi_line_text_input_keydown_process           PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function processes events for the specified text input widget. */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    input                                 Multi line text input         */
/*                                            control block               */
/*    event_ptr                             Incoming event to process     */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_multi_line_text_input_backspace   Process the backspace key     */
/*    _gx_multi_line_text_input_delete      Process the delete key        */
/*    _gx_multi_line_text_input_left_arrow  Process the left arrow key    */
/*    _gx_multi_line_text_input_right_arrow Process the right arrow key   */
/*    _gx_multi_line_text_input_up_arrow    Process the up arrow key      */
/*    _gx_multi_line_text_input_down_arrow  Process the down arrow key    */
/*    _gx_multi_line_text_input_char_insert                               */
/*                                          Insert a character            */
/*    _gx_multi_line_text_input_home        Process the HOME key          */
/*    _gx_multi_line_text_input_end         Process the END key           */
/*    _gx_utility_unicode_to_utf8           Parse a unicode to utf8 string*/
/*    _gx_window_event_process              Default window event process  */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_multi_line_text_input_event_process                             */
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
UINT  _gx_multi_line_text_input_keydown_process(GX_MULTI_LINE_TEXT_INPUT *input, GX_EVENT *event_ptr)
{
UINT      status;
USHORT    key_value;
GX_UBYTE  utf8_str[10];
GX_STRING string;

    switch (event_ptr -> gx_event_payload.gx_event_ushortdata[0])
    {
    case GX_KEY_BACKSPACE:
        status = _gx_multi_line_text_input_backspace(input);
        break;

    case GX_KEY_DELETE:
        status = _gx_multi_line_text_input_delete(input);
        break;

    case GX_KEY_LEFT_ARROW:
        status = _gx_multi_line_text_input_left_arrow(input);
        break;

    case GX_KEY_RIGHT_ARROW:
        status = _gx_multi_line_text_input_right_arrow(input);
        break;

    case GX_KEY_UP_ARROW:
        status = _gx_multi_line_text_input_up_arrow(input);
        break;

    case GX_KEY_DOWN_ARROW:
        status = _gx_multi_line_text_input_down_arrow(input);
        break;

    case GX_KEY_CARRIAGE_RETURN:
    case GX_KEY_LINE_FEED:
        utf8_str[0] = (GX_UBYTE)event_ptr -> gx_event_payload.gx_event_ushortdata[0];
        string.gx_string_ptr = (GX_CHAR *)utf8_str;
        string.gx_string_length = 1;
        status = _gx_multi_line_text_input_char_insert_ext(input, &string);
        break;

    case GX_KEY_SPACE:
        utf8_str[0] = ' ';
        string.gx_string_ptr = (GX_CHAR *)utf8_str;
        string.gx_string_length = 1;
        status = _gx_multi_line_text_input_char_insert_ext(input, &string);
        break;

    case GX_KEY_HOME:
        status = _gx_multi_line_text_input_home(input);
        break;

    case GX_KEY_END:
        status = _gx_multi_line_text_input_end(input);
        break;

    default:
        key_value = event_ptr -> gx_event_payload.gx_event_ushortdata[0];

        if ((key_value <= 0x1f) ||
            ((key_value >= 0x1b01) && key_value <= 0x1b14))
        {
            return _gx_window_event_process((GX_WINDOW *)input, event_ptr);
        }

        if (key_value <= 0x7e)
        {
            utf8_str[0] = (GX_UBYTE)key_value;
            string.gx_string_ptr = (GX_CHAR *)utf8_str;
            string.gx_string_length = 1;
            status = _gx_multi_line_text_input_char_insert_ext(input, &string);
        }
#ifdef GX_UTF8_SUPPORT
        else
        {

            _gx_utility_unicode_to_utf8(key_value, utf8_str, &string.gx_string_length);

            string.gx_string_ptr = (GX_CHAR *)utf8_str;

            /* Insert a utf8 string to input buffer. */
            status = _gx_multi_line_text_input_char_insert_ext(input, &string);
        }
#endif
        break;
    }

    return status;
}

