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
/**   Scroll Wheel Management (Generic Scroll Wheel)                      */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_window.h"
#include "gx_widget.h"
#include "gx_scroll_wheel.h"
#include "gx_canvas.h"
#include "gx_context.h"
#include "gx_utility.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_generic_scroll_wheel_draw                       PORTABLE C      */
/*                                                           6.1.7        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Ting Zhu, Microsoft Corporation                                     */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function draws the generic scroll wheel widget.                */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    wheel                                 Generic scroll wheel control  */
/*                                            block                       */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_window_background_draw            Draw window background        */
/*    _gx_context_pixelmap_get              Get pixelmap by resource ID   */
/*    _gx_window_client_height_get          Get window client height      */
/*    _gx_window_client_width_get           Get window client width       */
/*    _gx_canvas_pixelmap_tile              Tile a pixelmap               */
/*    _gx_widget_children_draw              Draw children of the widget   */
/*    _gx_widget_first_visible_client_child_get                           */
/*                                          Get the first visible client  */
/*    _gx_widget_next_visible_client_child_get                            */
/*                                          Get the next visible client   */
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
/*  06-02-2021     Ting Zhu                 Initial Version 6.1.7         */
/*                                                                        */
/**************************************************************************/
VOID _gx_generic_scroll_wheel_draw(GX_GENERIC_SCROLL_WHEEL *wheel)
{
GX_PIXELMAP *map;
GX_RECTANGLE selected_area;
GX_WIDGET   *child;

    /* Draw window background.  */
    _gx_window_background_draw((GX_WINDOW *)wheel);

    /* Pickup selected background.  */
    _gx_context_pixelmap_get(wheel -> gx_scroll_wheel_selected_background, &map);

    /* Define selected area.  */
    selected_area = wheel -> gx_window_client;
    selected_area.gx_rectangle_top = (GX_VALUE)((selected_area.gx_rectangle_top + selected_area.gx_rectangle_bottom) >> 1);
    selected_area.gx_rectangle_top = (GX_VALUE)(selected_area.gx_rectangle_top - (wheel -> gx_scroll_wheel_row_height >> 1));
    selected_area.gx_rectangle_bottom = (GX_VALUE)(selected_area.gx_rectangle_top + wheel -> gx_scroll_wheel_row_height - 1);

    if (map)
    {
        /* Draw selected background.  */
        _gx_canvas_pixelmap_tile(&selected_area, map);
    }


    if (!(wheel -> gx_widget_status & GX_STATUS_TRACKING_PEN) &&
        (wheel -> gx_scroll_wheel_animation_steps == 0))
    {
        child = _gx_widget_first_visible_client_child_get((GX_WIDGET *)wheel);

        /* The scroll wheel is stop scrolling, set GX_STYLE_DRAW_SELECTED style to the selected widget. */
        while (child)
        {
            if (selected_area.gx_rectangle_top == child -> gx_widget_size.gx_rectangle_top)
            {
                child -> gx_widget_style |= GX_STYLE_DRAW_SELECTED;
                break;
            }

            child = _gx_widget_next_visible_client_child_get(child);
        }
    }

    /* Draw all children of the generic scroll wheel.  */
    _gx_widget_children_draw((GX_WIDGET *)wheel);

    /* Draw the overlay pixelmap, if there is one.  */
    if (wheel -> gx_scroll_wheel_gradient.gx_gradient_pixelmap.gx_pixelmap_data)
    {
        _gx_canvas_pixelmap_tile(&wheel -> gx_widget_size, &wheel -> gx_scroll_wheel_gradient.gx_gradient_pixelmap);
    }
}

