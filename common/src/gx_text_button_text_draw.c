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
#include "gx_system.h"
#include "gx_display.h"
#include "gx_context.h"
#include "gx_widget.h"
#include "gx_button.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_text_button_text_draw                           PORTABLE C      */
/*                                                           6.0          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function draws the button text                                 */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    button                                Text button control block     */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_text_id_draw               Draw text using resource ID   */
/*    _gx_widget_text_draw                  Draw text string              */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*    GUIX Internal Code                                                  */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*                                                                        */
/**************************************************************************/
VOID  _gx_text_button_text_draw(GX_TEXT_BUTTON *button)
{
GX_WIDGET        *widget;
INT               xtextoffset = 0;
INT               ytextoffset = 0;
GX_RESOURCE_ID    color;
GX_STRING         string;

    /* Setup the button.  */
    widget = (GX_WIDGET *)button;

    /* draw the text */

    if (widget -> gx_widget_style & GX_STYLE_ENABLED)
    {
        if (widget -> gx_widget_style & GX_STYLE_BUTTON_PUSHED)
        {
            xtextoffset = ytextoffset = 1;
            color = button -> gx_text_button_selected_text_color;
        }
        else
        {
            color = button -> gx_text_button_normal_text_color;
        }
    }
    else
    {
        color = button -> gx_text_button_disabled_text_color;
    }

    if (button -> gx_text_button_text_id > 0)
    {
        _gx_widget_text_id_draw(widget, color,
                                button -> gx_text_button_font_id,
                                button -> gx_text_button_text_id,
                                xtextoffset, ytextoffset);
    }
    else
    {
        _gx_system_private_string_get(&button -> gx_text_button_string, &string, button -> gx_widget_style);

        if (string.gx_string_ptr)
        {
            _gx_widget_text_draw_ext(widget, color,
                                 button -> gx_text_button_font_id,
                                 &string, xtextoffset, ytextoffset);
        }
    }
}

