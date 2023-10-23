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
#include "gx_icon.h"
#include "gx_scrollbar.h"
#include "gx_utility.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_scrollbar_draw                                  PORTABLE C      */
/*                                                           6.3.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function draws the specified scroll bar, which is a            */
/*      special type of widget.                                           */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    scrollbar                             Scrollbar widget to draw      */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_context_brush_define              Define the brush for the      */
/*                                            context                     */
/*    _gx_context_brush_width_set           Set the width of the brush    */
/*    _gx_canvas_rectangle_draw             Draw rectangle                */
/*    _gx_context_pixelmap_get              Retrieve pixelmap image       */
/*    _gx_canvas_pixelmap_tile              Tile the canvas area with     */
/*                                            pixelmap                    */
/*    _gx_canvas_pixelmap_draw              Draw pixelmap                 */
/*    _gx_widget_width_get                  Retrieve the width of the     */
/*                                            widget                      */
/*    _gx_widget_height_get                 Retrieve the height of the    */
/*                                            widget                      */
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
/*  10-31-2023     Ting Zhu                 Modified comment(s),          */
/*                                            improved logic,             */
/*                                            resulting in version 6.3.0  */
/*                                                                        */
/**************************************************************************/
VOID  _gx_scrollbar_draw(GX_SCROLLBAR *scrollbar)
{
GX_PIXELMAP *map;
GX_VALUE     xpos;
GX_VALUE     ypos;
GX_VALUE     widget_width;
GX_VALUE     widget_height;
GX_RECTANGLE size;
GX_RECTANGLE old_dirty;
GX_COLOR     fill_color;

    if (scrollbar -> gx_widget_style & GX_STYLE_ENABLED)
    {
        if (scrollbar -> gx_widget_style & GX_STYLE_DRAW_SELECTED)
        {
            fill_color = scrollbar -> gx_widget_selected_fill_color;
        }
        else
        {
            fill_color = scrollbar -> gx_widget_normal_fill_color;
        }
    }
    else
    {
        fill_color = scrollbar -> gx_widget_disabled_fill_color;
    }

    if ((scrollbar -> gx_widget_style & GX_STYLE_TRANSPARENT) == 0)
    {
        size = scrollbar -> gx_widget_size;

        /* Draw background.  */
        if (scrollbar -> gx_widget_style & GX_STYLE_BORDER_THIN)
        {
            _gx_context_brush_define(GX_COLOR_ID_SHADOW,
                                     fill_color,
                                     GX_BRUSH_SOLID_FILL);
            _gx_context_brush_width_set(1);
        }
        else
        {
            _gx_context_brush_define(fill_color,
                                     fill_color,
                                     GX_BRUSH_SOLID_FILL);
            _gx_context_brush_width_set(0);
            _gx_canvas_rectangle_draw(&size);
        }
        _gx_canvas_rectangle_draw(&size);

        /* Draw pixelmaps.  */
        _gx_context_pixelmap_get(scrollbar -> gx_scrollbar_appearance.gx_scroll_up_pixelmap, &map);

        if (map)
        {
            if (scrollbar -> gx_widget_type == GX_TYPE_VERTICAL_SCROLL)
            {
                /* Draw up pixelmap.  */
                xpos = size.gx_rectangle_left;
                _gx_widget_width_get((GX_WIDGET *)scrollbar, &widget_width);
                xpos = (GX_VALUE)(xpos + (widget_width - map -> gx_pixelmap_width) / 2);

                ypos = size.gx_rectangle_top;

                size.gx_rectangle_top = (GX_VALUE)(size.gx_rectangle_top + map -> gx_pixelmap_height);
            }
            else
            {
                /* Draw left pixelmap.  */
                xpos = size.gx_rectangle_left;

                ypos = size.gx_rectangle_top;
                _gx_widget_height_get((GX_WIDGET *)scrollbar, &widget_height);
                ypos = (GX_VALUE)(ypos + (widget_height - map -> gx_pixelmap_height) / 2);

                size.gx_rectangle_left = (GX_VALUE)(size.gx_rectangle_left + map -> gx_pixelmap_width);
            }

            _gx_canvas_pixelmap_draw(xpos, ypos, map);
        }

        _gx_context_pixelmap_get(scrollbar -> gx_scrollbar_appearance.gx_scroll_down_pixelmap, &map);

        if (map)
        {
            if (scrollbar -> gx_widget_type == GX_TYPE_VERTICAL_SCROLL)
            {
                /* Draw down pixelmap.  */
                size.gx_rectangle_bottom = (GX_VALUE)(size.gx_rectangle_bottom - map -> gx_pixelmap_height);

                xpos = size.gx_rectangle_left;
                _gx_widget_width_get((GX_WIDGET *)scrollbar, &widget_width);
                xpos = (GX_VALUE)(xpos + (widget_width - map -> gx_pixelmap_width) / 2);

                ypos = size.gx_rectangle_bottom;
            }
            else
            {
                /* Draw right pixelmap.  */
                size.gx_rectangle_right = (GX_VALUE)(size.gx_rectangle_right - map -> gx_pixelmap_width);

                xpos = size.gx_rectangle_right;

                ypos = size.gx_rectangle_top;
                _gx_widget_height_get((GX_WIDGET *)scrollbar, &widget_height);
                ypos = (GX_VALUE)(ypos + (widget_height - map -> gx_pixelmap_height) / 2);
            }

            _gx_canvas_pixelmap_draw(xpos, ypos, map);
        }

        _gx_context_pixelmap_get(scrollbar -> gx_scrollbar_appearance.gx_scroll_fill_pixelmap, &map);

        if (map)
        {
            /* Draw background pixelmap.  */
            if (scrollbar -> gx_widget_style & GX_STYLE_TILE_BACKGROUND)
            {
                _gx_canvas_pixelmap_tile(&size, map);
            }
            else
            {
                xpos = size.gx_rectangle_left;
                _gx_widget_width_get((GX_WIDGET *)scrollbar, &widget_width);
                xpos = (GX_VALUE)(xpos + (widget_width - map -> gx_pixelmap_width) / 2);

                ypos = size.gx_rectangle_top;
                _gx_widget_height_get((GX_WIDGET *)scrollbar, &widget_height);
                ypos = (GX_VALUE)(ypos + (widget_height - map -> gx_pixelmap_height) / 2);

                /* Reset dirty area temporarily to avoid cover the end pixelmap area. */
                old_dirty = _gx_system_current_draw_context -> gx_draw_context_dirty;
                if(_gx_utility_rectangle_overlap_detect(&old_dirty, &size, &size) == GX_TRUE)
                {
                    _gx_system_current_draw_context -> gx_draw_context_dirty = size;

                    _gx_canvas_pixelmap_draw(xpos, ypos, map);

                    /* Set dirty area back. */
                    _gx_system_current_draw_context -> gx_draw_context_dirty = old_dirty;
                }
            }
        }
    }

    /* Draw children widgets of prompt widget.  */
    _gx_widget_children_draw((GX_WIDGET *)scrollbar);
}

