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
/**   Scroll Management (Scroll)                                          */
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
#include "gx_scrollbar.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_scroll_thumb_draw                               PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service draws a scroll thumb.                                  */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    scroll_thumb                          Scroll thumb widget control   */
/*                                          block                         */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_context_pixelmap_get              Retrieve pixelmap image       */
/*    _gx_widget_width_get                  Gets the width of the widget  */
/*    _gx_widget_height_get                 Gets the height of the widget */
/*    _gx_canvas_pixelmap_draw              Draw pixelmap                 */
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
VOID  _gx_scroll_thumb_draw(GX_SCROLL_THUMB *thumb)
{
GX_WIDGET   *widget = (GX_WIDGET *)thumb;
GX_VALUE     xpos;
GX_VALUE     ypos;
GX_PIXELMAP *map;
GX_VALUE     widget_width;
GX_VALUE     widget_height;

    if (thumb -> gx_scroll_thumb_pixelmap)
    {
        _gx_context_pixelmap_get(thumb -> gx_scroll_thumb_pixelmap, &map);

        if (map)
        {
            xpos = widget -> gx_widget_size.gx_rectangle_left;
            _gx_widget_width_get(widget, &widget_width);
            xpos = (GX_VALUE)(xpos + (widget_width - map -> gx_pixelmap_width) / 2);

            ypos = widget -> gx_widget_size.gx_rectangle_top;
            _gx_widget_height_get(widget, &widget_height);
            ypos = (GX_VALUE)(ypos + (widget_height - map -> gx_pixelmap_height) / 2);

            _gx_canvas_pixelmap_draw(xpos, ypos, map);
        }
    }
    else
    {
        _gx_widget_border_draw(widget, thumb -> gx_scroll_thumb_border_color,
                               widget -> gx_widget_normal_fill_color,
                               widget -> gx_widget_normal_fill_color, GX_TRUE);
    }
}

