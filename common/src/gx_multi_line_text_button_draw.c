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
#include "gx_canvas.h"
#include "gx_system.h"
#include "gx_context.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_multi_line_text_button_draw                     PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function draws the specified multi-line text button, which is  */
/*    special type of button (widget).                                    */
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
/*   _gx_button_background_draw             Draw button background        */
/*   _gx_multi_line_text_button_text_draw   Draw multi-line button text   */
/*   _gx_widget_children_draw               Draw children widgets         */
/*   _gx_monochrome_driver_disabled_button_line_draw                      */
/*                                          Draw a line for disabled      */
/*                                            monochrome button           */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    System and Application Code                                         */
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
VOID  _gx_multi_line_text_button_draw(GX_MULTI_LINE_TEXT_BUTTON *button)
{
GX_WIDGET *widget = (GX_WIDGET *)button;

    /* draw the background button */
    _gx_button_background_draw((GX_BUTTON *)button);

    /* draw the text lines */
    _gx_multi_line_text_button_text_draw(button);

    /* Draw button's children.  */
    _gx_widget_children_draw(widget);

    if (!(button -> gx_widget_style & GX_STYLE_ENABLED))
    {
        /* Draw a line on the button to show it's disable for mono driver. */
        _gx_monochrome_driver_disabled_button_line_draw((GX_BUTTON *)button);
    }
}

