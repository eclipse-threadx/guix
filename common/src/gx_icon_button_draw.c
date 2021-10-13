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
#include "gx_system.h"
#include "gx_canvas.h"
#include "gx_context.h"
#include "gx_widget.h"
#include "gx_button.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_icon_button_draw                                PORTABLE C      */
/*                                                           6.1.9        */
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
/*    button                                Icon button control block     */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_button_background_draw            Draw the button background    */
/*    _gx_context_pixelmap_get              Retrieve pixelmap image       */
/*    _gx_widget_border_width_get           Get the border width          */
/*    _gx_canvas_pixelmap_draw              Draw the pixelmap             */
/*    _gx_widget_children_draw              Draw children widgets         */
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
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*  10-15-2021     Ting Zhu                 Modified comment(s),          */
/*                                            fixed pixelmap draw offset, */
/*                                            resulting in version 6.1.9  */
/*                                                                        */
/**************************************************************************/
VOID  _gx_icon_button_draw(GX_ICON_BUTTON *button)
{
GX_WIDGET   *widget;
GX_PIXELMAP *map;
GX_VALUE     xpos;
GX_VALUE     ypos;
GX_VALUE     border;
INT          shift = 0;

    /* Setup the widget.  */
    widget =  (GX_WIDGET *)button;

    /* draw the background button */
    _gx_button_background_draw((GX_BUTTON *)button);

    /* draw the button icon */
    _gx_context_pixelmap_get(button -> gx_icon_button_icon_id, &map);

    if (map)
    {
        _gx_widget_border_width_get(widget, &border);

        if (widget -> gx_widget_style & GX_STYLE_BUTTON_PUSHED)
        {
            shift = 1;
        }

        switch (widget -> gx_widget_style & GX_PIXELMAP_HALIGN_MASK)
        {
        case GX_STYLE_HALIGN_CENTER:
            xpos = (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_right -
                              widget -> gx_widget_size.gx_rectangle_left -
                              map -> gx_pixelmap_width + 1);

            xpos /= 2;
            xpos = (GX_VALUE)(xpos + widget -> gx_widget_size.gx_rectangle_left + shift);
            break;

        case GX_STYLE_HALIGN_RIGHT:
            xpos = (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_right - map -> gx_pixelmap_width - border + shift + 1);
            break;

        default:
            xpos = (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_left + border + shift);
            break;
        }

        switch (widget -> gx_widget_style & GX_PIXELMAP_VALIGN_MASK)
        {
        case GX_STYLE_VALIGN_CENTER:
            ypos = (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_bottom -
                              widget -> gx_widget_size.gx_rectangle_top -
                              map -> gx_pixelmap_height + 1);

            ypos /= 2;
            ypos = (GX_VALUE)(ypos + widget -> gx_widget_size.gx_rectangle_top + shift);
            break;

        case GX_STYLE_VALIGN_BOTTOM:
            ypos = (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_bottom - map -> gx_pixelmap_height - border + shift + 1);
            break;

        default:
            ypos = (GX_VALUE)(widget -> gx_widget_size.gx_rectangle_top + border + shift);
            break;
        }
        _gx_widget_context_fill_set(widget);
        _gx_canvas_pixelmap_draw(xpos, ypos, map);
    }


    /* Draw button's children.  */
    _gx_widget_children_draw(widget);
}

