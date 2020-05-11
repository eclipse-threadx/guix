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
/**   Text Button Management (Button)                                     */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_widget.h"
#include "gx_button.h"
#include "gx_system.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_multi_line_text_button_line_pointers_set        PORTABLE C      */
/*                                                           6.0          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function creates a multi-line text button, which is a special  */
/*    type of button (widget).                                            */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    button                                Button control block          */
/*    name                                  Name of button                */
/*    parent                                Parent widget control block   */
/*    text_id                               text resource id              */
/*    style                                 Style of button               */
/*    size                                  Button size                   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_string_get                                               */
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
/*                                                                        */
/**************************************************************************/
VOID _gx_multi_line_text_button_line_pointers_set(GX_MULTI_LINE_TEXT_BUTTON *button)
{
INT               line_index;
GX_STRING         string;
GX_CONST GX_CHAR *text;

    button -> gx_multi_line_text_button_line_count = 1;

    for (line_index = 0; line_index < GX_MULTI_LINE_TEXT_BUTTON_MAX_LINES; line_index++)
    {
        button -> gx_multi_line_text_button_lines[line_index].gx_string_ptr = GX_NULL;
        button -> gx_multi_line_text_button_lines[line_index].gx_string_length = 0;
    }
    line_index = 0;

    if (button -> gx_text_button_text_id)
    {
        _gx_widget_string_get_ext((GX_WIDGET *)button, button -> gx_text_button_text_id, &string);
    }
    else
    {
        _gx_system_private_string_get(&button -> gx_text_button_string, &string, button -> gx_widget_style);
    }

    text = string.gx_string_ptr;

    if (!text)
    {
        return;
    }

    while (*text)
    {
        if ((*text == GX_KEY_CARRIAGE_RETURN) || (*text == GX_KEY_LINE_FEED))
        {
            if ((*text == GX_KEY_CARRIAGE_RETURN) && (*(text + 1) == GX_KEY_LINE_FEED))
            {
                text += 2;
            }
            else
            {
                text++;
            }
            line_index++;

            if (line_index >=  GX_MULTI_LINE_TEXT_BUTTON_MAX_LINES)
            {
                break;
            }
            else
            {
                button -> gx_multi_line_text_button_line_count++;
            }
        }
        else
        {
            if (button -> gx_multi_line_text_button_lines[line_index].gx_string_ptr == GX_NULL)
            {
                button -> gx_multi_line_text_button_lines[line_index].gx_string_ptr = text;
            }
            button -> gx_multi_line_text_button_lines[line_index].gx_string_length++;
            text++;
        }
    }
}

