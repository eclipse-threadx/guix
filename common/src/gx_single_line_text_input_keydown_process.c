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
/**   Text Input Management (Single Line Text Input)                      */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_widget.h"
#include "gx_single_line_text_input.h"
#include "gx_utility.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_single_line_text_input_keydown_process          PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function processes key-down events for the single line text    */
/*      input widget.                                                     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text_input                            Single-line text input widget */
/*                                            control block               */
/*    event_ptr                             Pointer to GX_EVENT structure */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_event_process             Default widget event process   */
/*    _gx_widget_event_generate            Create a event and send it to  */
/*                                           parent                       */
/*    _gx_single_line_text_input_home      Process the home key           */
/*    _gx_single_line_text_input_end       Process the end key            */
/*    _gx_single_line_text_input_backspace Process the backspace key      */
/*    _gx_single_line_text_input_character_delete                         */
/*                                         Delete a character             */
/*    _gx_single_line_text_input_left_arrow                               */
/*                                         Process the left arrow key     */
/*    _gx_single_line_text_input_right_arrow                              */
/*                                         Process the right arrow key    */
/*    _gx_single_line_text_input_character_insert                         */
/*                                         Insert a charcter              */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_single_line_text_input_event_process                            */
/*                                         Single line text input widget  */
/*                                           event process routine        */
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
VOID  _gx_single_line_text_input_keydown_process(GX_SINGLE_LINE_TEXT_INPUT *text_input, GX_EVENT *event_ptr)
{
GX_WIDGET *widget = (GX_WIDGET *)text_input;
USHORT     key_value;
GX_UBYTE   utf8_str[10];
UINT       utf8_size;

    /* Process relative to the value of key.  */
    switch (event_ptr -> gx_event_payload.gx_event_ushortdata[0])
    {
    case GX_KEY_HOME:
        _gx_single_line_text_input_home(text_input);
        break;

    case GX_KEY_END:
        _gx_single_line_text_input_end(text_input);
        break;

    case GX_KEY_BACKSPACE:
        _gx_single_line_text_input_backspace(text_input);
        break;

    case GX_KEY_DELETE:
        _gx_single_line_text_input_character_delete(text_input);
        break;

    case GX_KEY_LEFT_ARROW:
        _gx_single_line_text_input_left_arrow(text_input);
        break;

    case GX_KEY_RIGHT_ARROW:
        _gx_single_line_text_input_right_arrow(text_input);
        break;

    case GX_KEY_SELECT:
        if (text_input -> gx_single_line_text_input_was_modified)
        {
            _gx_widget_event_generate(widget, GX_EVENT_TEXT_EDITED, 0);
            text_input -> gx_single_line_text_input_was_modified = GX_FALSE;
        }
        break;
    case GX_KEY_SPACE:
        utf8_str[0] = ' ';
        utf8_size = 1;
        _gx_single_line_text_input_character_insert(text_input, utf8_str, utf8_size);
        break;

    default:
        key_value = event_ptr -> gx_event_payload.gx_event_ushortdata[0];

        if ((key_value <= 0x1f) ||
            ((key_value >= 0x1b01) && key_value <= 0x1b14))
        {
            _gx_widget_event_process((GX_WIDGET *)text_input, event_ptr);
            return;
        }

        /* If the key value is displayable, insert the value to the text input.  */
        if (key_value <= 0x7e)
        {
            utf8_str[0] = (GX_UBYTE)key_value;
            utf8_size = 1;
            _gx_single_line_text_input_character_insert(text_input, utf8_str, utf8_size);
        }
#ifdef GX_UTF8_SUPPORT
        else
        {
            _gx_utility_unicode_to_utf8(key_value, utf8_str, &utf8_size);

            /* Insert a utf8 string to input buffer. */
            _gx_single_line_text_input_character_insert(text_input, utf8_str, utf8_size);
        }
#endif
        break;
    }
}

