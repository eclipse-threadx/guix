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
/**   Icon Management (Icon)                                              */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_canvas.h"
#include "gx_context.h"
#include "gx_widget.h"
#include "gx_icon.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_icon_background_draw                            PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function draws the background of specified icon.               */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    icon                                  Icon widget control block     */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_context_pixelmap_get              Retrieve pixelmap image       */
/*    _gx_canvas_pixelmap_draw              Draw pixelmap                 */
/*    _gx_widget_context_fill_set           Set fill color                */
/*    _gx_widget_background_draw            Retrieve pixelmap image       */
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
VOID  _gx_icon_background_draw(GX_ICON *icon)
{
GX_WIDGET   *widget = (GX_WIDGET *)icon;
GX_PIXELMAP *map;
GX_VALUE    xpos;
GX_VALUE    ypos;
GX_VALUE    size;

    if (icon -> gx_icon_selected_pixelmap &&
        widget -> gx_widget_style & GX_STYLE_DRAW_SELECTED)
    {
        _gx_context_pixelmap_get(icon -> gx_icon_selected_pixelmap, &map);
    }
    else
    {
        _gx_context_pixelmap_get(icon -> gx_icon_normal_pixelmap, &map);
    }

    if (map)
    {
        _gx_widget_context_fill_set(widget);

        switch(widget -> gx_widget_style & GX_PIXELMAP_HALIGN_MASK)
        {
        case GX_STYLE_HALIGN_RIGHT:
            xpos = (GX_VALUE) (widget -> gx_widget_size.gx_rectangle_right - map->gx_pixelmap_width + 1);
            break;

        case GX_STYLE_HALIGN_CENTER:
            size = (GX_VALUE) ((icon -> gx_widget_size.gx_rectangle_right - icon -> gx_widget_size.gx_rectangle_left) + 1);
            size = (GX_VALUE) (size - map -> gx_pixelmap_width);
            xpos = (GX_VALUE) (icon -> gx_widget_size.gx_rectangle_left + (size / 2));
            break;

        default:
            xpos = widget ->gx_widget_size.gx_rectangle_left;
            break;
        }

        switch(widget -> gx_widget_style & GX_PIXELMAP_VALIGN_MASK)
        {
        case GX_STYLE_VALIGN_BOTTOM:
            ypos = (GX_VALUE) (widget -> gx_widget_size.gx_rectangle_bottom - map->gx_pixelmap_height + 1);
            break;

        case GX_STYLE_VALIGN_CENTER:
            size = (GX_VALUE) ((icon -> gx_widget_size.gx_rectangle_bottom - icon -> gx_widget_size.gx_rectangle_top) + 1);
            size = (GX_VALUE) (size - map -> gx_pixelmap_height);
            ypos = (GX_VALUE) (icon -> gx_widget_size.gx_rectangle_top + (size / 2));
            break;

        default:
            ypos = icon -> gx_widget_size.gx_rectangle_top;
            break;
        }
        _gx_canvas_pixelmap_draw(xpos, ypos, map);
    }
    else
    {
        if (!(icon -> gx_widget_style & GX_STYLE_TRANSPARENT))
        {
            _gx_widget_background_draw((GX_WIDGET *)icon);
        }
    }
}

