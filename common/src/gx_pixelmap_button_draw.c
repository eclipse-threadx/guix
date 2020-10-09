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
/*    _gx_pixelmap_button_draw                            PORTABLE C      */
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
/*    _gx_context_pixelmap_get              Retrieve pixelmap image       */
/*    _gx_canvas_pixelmap_draw              Draw the pixelmap             */
/*    _gx_button_draw                       Draw the button               */
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
/*                                                                        */
/**************************************************************************/
VOID  _gx_pixelmap_button_draw(GX_PIXELMAP_BUTTON *button)
{
GX_RESOURCE_ID pixelmap;
GX_PIXELMAP   *map;
GX_WIDGET     *widget = (GX_WIDGET *)button;
INT            xpos;
INT            ypos;


    /* default to using the normal pixlemap */
    pixelmap = button -> gx_pixelmap_button_normal_id;

    /* If the button is not enabled and the user provided
       a disabled pixelmap, use that */
    if (button -> gx_pixelmap_button_disabled_id &&
        !(button -> gx_widget_style & GX_STYLE_ENABLED))
    {
        pixelmap = button -> gx_pixelmap_button_disabled_id;
    }
    else
    {
        /* otherwise, if the button is pushed used the selected pixelmap */
        if (button -> gx_widget_style & GX_STYLE_BUTTON_PUSHED)
        {
            if (button -> gx_pixelmap_button_selected_id)
            {
                pixelmap = button -> gx_pixelmap_button_selected_id;
            }
        }
    }

    if (!(button -> gx_widget_style & GX_STYLE_ENABLED))
    {
        if (button -> gx_pixelmap_button_disabled_id)
        {
            pixelmap = button -> gx_pixelmap_button_disabled_id;
        }
    }

    _gx_context_pixelmap_get(pixelmap, &map);

    if (map)
    {
        switch (button -> gx_widget_style & GX_PIXELMAP_HALIGN_MASK)
        {
        case GX_STYLE_HALIGN_CENTER:
            xpos = button -> gx_widget_size.gx_rectangle_right -
                button -> gx_widget_size.gx_rectangle_left -
                map -> gx_pixelmap_width + 1;

            xpos /= 2;
            xpos += button -> gx_widget_size.gx_rectangle_left;
            break;

        case GX_STYLE_HALIGN_RIGHT:
            xpos = button -> gx_widget_size.gx_rectangle_right - map -> gx_pixelmap_width + 1;
            break;

        default:
            xpos = button -> gx_widget_size.gx_rectangle_left;
            break;
        }

        switch (button -> gx_widget_style & GX_PIXELMAP_VALIGN_MASK)
        {
        case GX_STYLE_VALIGN_CENTER:
            ypos = button -> gx_widget_size.gx_rectangle_bottom -
                button -> gx_widget_size.gx_rectangle_top -
                map -> gx_pixelmap_height + 1;

            ypos /= 2;
            ypos += button -> gx_widget_size.gx_rectangle_top;
            break;

        case GX_STYLE_VALIGN_BOTTOM:
            ypos = button -> gx_widget_size.gx_rectangle_bottom - map -> gx_pixelmap_height + 1;
            break;

        default:
            ypos = button -> gx_widget_size.gx_rectangle_top;
            break;
        }
        _gx_widget_context_fill_set(widget);
        _gx_canvas_pixelmap_draw((GX_VALUE)xpos, (GX_VALUE)ypos, map);
    }
    else
    {
        /* If I don't have a pixelmap and not transparent,
           draw something
         */
        if (!(widget -> gx_widget_style & GX_STYLE_TRANSPARENT))
        {
            _gx_button_draw((GX_BUTTON *)button);
        }
    }

    /* Draw button's children.  */
    _gx_widget_children_draw(widget);
}

