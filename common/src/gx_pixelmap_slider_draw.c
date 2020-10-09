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
/**   Slider Management (Slider)                                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE

/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_canvas.h"
#include "gx_context.h"
#include "gx_widget.h"
#include "gx_slider.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_pixelmap_slider_draw                            PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function draws the specified slider, which is a type of        */
/*    of widget designed to allow adjusting or display a value            */
/*    graphically                                                         */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    slider                                Slider control block          */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_slider_needle_position_get        Get the position of the       */
/*                                            slider needle               */
/*    _gx_canvas_pixelmap_tile              Tile a pixelmap               */
/*    _gx_context_pixelmap_get              Retrieve pixelmap image       */
/*    _gx_sidget_width_get                  Obtain the width of a widget  */
/*    _gx_wdiget_background_draw            Draw widget background        */
/*    _gx_canvas_pixelmap_draw              Draw pixelmap on canvas       */
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
VOID  _gx_pixelmap_slider_draw(GX_PIXELMAP_SLIDER *slider)
{
GX_PIXELMAP             *map;
GX_PIXELMAP             *uppermap;
GX_RECTANGLE             fillrect;
GX_SLIDER_INFO          *info;
GX_PIXELMAP_SLIDER_INFO *pixinfo;
GX_VALUE                 space;
GX_RECTANGLE             needle_size;

    info = &slider -> gx_slider_info;
    pixinfo = &slider -> gx_pixelmap_slider_pixelmap_info;

    _gx_context_pixelmap_get(pixinfo -> gx_pixelmap_slider_info_lower_background_pixelmap, &map);
    _gx_context_pixelmap_get(pixinfo -> gx_pixelmap_slider_info_upper_background_pixelmap, &uppermap);
    _gx_slider_needle_position_get((GX_SLIDER *)slider, info, &needle_size);
    _gx_widget_context_fill_set((GX_WIDGET *)slider);

    if ((slider -> gx_widget_style & GX_STYLE_TILE_BACKGROUND) && map && uppermap)
    {
        if (slider -> gx_widget_style & GX_STYLE_SLIDER_VERTICAL)
        {
            _gx_widget_width_get((GX_WIDGET *)slider, &space);
            space = (GX_VALUE)(space - map -> gx_pixelmap_width);
            space /= 2;
            fillrect.gx_rectangle_top = slider -> gx_widget_size.gx_rectangle_top;
            fillrect.gx_rectangle_left = (GX_VALUE)(slider -> gx_widget_size.gx_rectangle_left + space);
            fillrect.gx_rectangle_right = (GX_VALUE)(fillrect.gx_rectangle_left + map -> gx_pixelmap_width - 1);
            fillrect.gx_rectangle_bottom = (GX_VALUE)(needle_size.gx_rectangle_top + info -> gx_slider_info_needle_hotspot_offset);
            _gx_canvas_pixelmap_tile(&fillrect, uppermap);

            fillrect.gx_rectangle_top = (GX_VALUE)(fillrect.gx_rectangle_bottom + 1);
            fillrect.gx_rectangle_bottom = slider -> gx_widget_size.gx_rectangle_bottom;
            _gx_canvas_pixelmap_tile(&fillrect, map);
        }
        else
        {
            _gx_widget_height_get((GX_WIDGET *)slider, &space);
            space = (GX_VALUE)(space - map -> gx_pixelmap_height);
            space /= 2;
            fillrect.gx_rectangle_top = (GX_VALUE)(slider -> gx_widget_size.gx_rectangle_top + space);
            fillrect.gx_rectangle_left = slider -> gx_widget_size.gx_rectangle_left;
            fillrect.gx_rectangle_right = (GX_VALUE)(needle_size.gx_rectangle_left + info -> gx_slider_info_needle_hotspot_offset);
            fillrect.gx_rectangle_bottom = (GX_VALUE)(fillrect.gx_rectangle_top + map -> gx_pixelmap_height - 1);
            _gx_canvas_pixelmap_tile(&fillrect, map);

            fillrect.gx_rectangle_left = (GX_VALUE)(fillrect.gx_rectangle_right + 1);
            fillrect.gx_rectangle_right = slider -> gx_widget_size.gx_rectangle_right;
            _gx_canvas_pixelmap_tile(&fillrect, uppermap);
        }
    }
    else
    {
        if (map)
        {
            if (slider -> gx_widget_style & GX_STYLE_TILE_BACKGROUND)
            {
                _gx_canvas_pixelmap_tile(&slider -> gx_widget_size, map);
            }
            else
            {
                _gx_canvas_pixelmap_draw(slider -> gx_widget_size.gx_rectangle_left,
                                         slider -> gx_widget_size.gx_rectangle_top, map);
            }
        }
        else
        {
            if (!(slider -> gx_widget_style & GX_STYLE_TRANSPARENT))
            {
                _gx_widget_background_draw((GX_WIDGET *)slider);
            }
        }
    }

    /* Pick up needle pixelmap. */
    _gx_context_pixelmap_get(pixinfo -> gx_pixelmap_slider_info_needle_pixelmap, &map);

    if (map)
    {
        /* Draw needle pixelmap. */
        _gx_canvas_pixelmap_draw(needle_size.gx_rectangle_left, needle_size.gx_rectangle_top, map);
    }

    _gx_widget_children_draw((GX_WIDGET *)slider);
}

