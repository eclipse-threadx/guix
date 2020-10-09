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
/**   Drop List (List)                                                    */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */
#include "gx_api.h"
#include "gx_widget.h"
#include "gx_system.h"
#include "gx_drop_list.h"
#include "gx_context.h"
#include "gx_canvas.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_drop_list_background_draw                       PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function draws the background of drop list, which is a special */
/*    type of widget.                                                     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    drop list                             Drop List control block       */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_border_draw                Draw the border               */
/*    _gx_context_pixelmap_get              Retrieve pixelmap image       */
/*    _gx_widget_border_width_get           Get the border width          */
/*    _gx_widget_client_get                 Find the client area of a     */
/*                                            widget                      */
/*    _gx_canvas_pixelmap_tile              Tile a pixelmap               */
/*    _gx_canvas_pixelmap_draw              Draw a pixelmap               */
/*    _gx_widget_background_draw            Draw widget background        */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_drop_list_draw                                                  */
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
VOID  _gx_drop_list_background_draw(GX_DROP_LIST *drop_list)
{
GX_RESOURCE_ID      pixelmap_id;
GX_PIXELMAP        *map;
GX_VALUE            xpos;
GX_VALUE            ypos;
GX_WIDGET          *widget;
GX_VALUE            border_width;
GX_COLOR            color;
GX_RECTANGLE        client;

    widget = (GX_WIDGET *)drop_list;
    map = GX_NULL;
    pixelmap_id = drop_list -> gx_drop_list_pixelmap;

    if (pixelmap_id)
    {
        _gx_context_pixelmap_get(pixelmap_id, &map);
    }

    if (map)
    {
        _gx_widget_border_width_get(widget, &border_width);
        color = widget -> gx_widget_normal_fill_color;
        _gx_widget_border_draw(widget, GX_COLOR_ID_DEFAULT_BORDER, color, color, GX_TRUE);

        if (drop_list -> gx_widget_style & GX_STYLE_TILE_BACKGROUND)
        {
            _gx_widget_client_get(widget, border_width, &client);
            _gx_canvas_pixelmap_tile(&client, map);
        }
        else
        {
            xpos = (GX_VALUE)(widget->gx_widget_clip.gx_rectangle_left + border_width + 1);
            ypos = (GX_VALUE)(widget->gx_widget_clip.gx_rectangle_top + border_width + 1);
            _gx_canvas_pixelmap_draw(xpos, ypos, map);
        }
    }
    else
    {
        _gx_widget_background_draw((GX_WIDGET *)drop_list);
    }
}

