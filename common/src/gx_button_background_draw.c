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
/**   Button Management (Button)                                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_button.h"
#include "gx_system.h"
#include "gx_widget.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_button_background_draw                          PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function draws the specified button, which is a special type   */
/*    of widget.                                                          */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    button                                Button control block          */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_border_style_set           Set the border style          */
/*    _gx_widget_border_draw                Draw the border               */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*    _gx_button_draw                                                     */
/*    _gx_icon_button_draw                                                */
/*    _gx_text_button_draw                                                */
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
VOID  _gx_button_background_draw(GX_BUTTON *button)
{
GX_RESOURCE_ID fill1;
GX_RESOURCE_ID fill2;
GX_WIDGET     *widget = (GX_WIDGET *)button;

    /* Draw button background.  */
    if (button -> gx_widget_style & GX_STYLE_ENABLED)
    {
        if (button -> gx_widget_style & GX_STYLE_BUTTON_PUSHED)
        {
            if ((button -> gx_widget_style & GX_STYLE_BORDER_MASK) == GX_STYLE_BORDER_RAISED)
            {
                _gx_widget_border_style_set(widget, GX_STYLE_BORDER_RECESSED);
            }
            fill1 = widget -> gx_widget_normal_fill_color;
            fill2 = widget -> gx_widget_selected_fill_color;
        }
        else
        {
            if ((button -> gx_widget_style & GX_STYLE_BORDER_MASK) == GX_STYLE_BORDER_RECESSED)
            {
                _gx_widget_border_style_set(widget, GX_STYLE_BORDER_RAISED);
            }
            fill1 = widget -> gx_widget_selected_fill_color;
            fill2 = widget -> gx_widget_normal_fill_color;
        }
    }
    else
    {
        fill1 = widget -> gx_widget_disabled_fill_color;
        fill2 = widget -> gx_widget_disabled_fill_color;
    }

    _gx_widget_border_draw(widget, GX_COLOR_ID_BUTTON_BORDER, fill1, fill2, GX_TRUE);
}

