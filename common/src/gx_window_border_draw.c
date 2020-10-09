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
/**   Window Management (Window)                                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_canvas.h"
#include "gx_context.h"
#include "gx_widget.h"
#include "gx_utility.h"
#include "gx_window.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_window_border_draw                              PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function draws the background of a window widget width         */
/*    specified fill color.                                               */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    win                                   Window control block          */
/*    fill_color                            Specified fill color ID       */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_context_pixelmap_get              Get pixelmap with specified   */
/*                                            resource ID                 */
/*    _gx_widget_border_width_get           Get the border width          */
/*    _gx_widget_client_get                 Get the widget client         */
/*    _gx_widget_border_draw                Draw widget background        */
/*    _gx_canvas_pixelmap_tile              Draw pixelmap in tile style   */
/*    _gx_canvas_pixelmap_draw              Draw the pixelmap             */
/*    _gx_utility_rectangle_overlap_detect  Detect rectangle overlap      */
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
VOID  _gx_window_border_draw(GX_WINDOW *win, GX_RESOURCE_ID fill_color)
{
GX_WIDGET       *widget = (GX_WIDGET *)win;
GX_PIXELMAP     *pixelmap = GX_NULL;
GX_BOOL          fill = GX_TRUE;
GX_DRAW_CONTEXT *context;
GX_VALUE         width;
GX_VALUE         height;
GX_RECTANGLE     client;
GX_RECTANGLE     old_clip;
GX_VALUE         border_width;

    /* test to see if we need to solid fill */

    if (win -> gx_window_wallpaper != 0)
    {
        _gx_context_pixelmap_get(win -> gx_window_wallpaper, &pixelmap);

        /* do we have a wallpaper ? */
        if (pixelmap)
        {
            _gx_widget_border_width_get(widget, &border_width);

            /* get size of client rectangle */
            _gx_widget_client_get(widget, border_width, &client);

            /* is the wallpaper transparent at all ? */
            if ((pixelmap -> gx_pixelmap_flags & (GX_PIXELMAP_TRANSPARENT | GX_PIXELMAP_ALPHA)) == 0)
            {
                /* will the wallpaper fill my client area ? */
                if (widget -> gx_widget_style & GX_STYLE_TILE_WALLPAPER)
                {
                    /* yes, don't need to fill */
                    fill = GX_FALSE;
                }
                else
                {
                    width = (GX_VALUE)(client.gx_rectangle_right - client.gx_rectangle_left + 1);
                    height = (GX_VALUE)(client.gx_rectangle_bottom - client.gx_rectangle_top + 1);

                    if (pixelmap -> gx_pixelmap_width >= width &&
                        pixelmap -> gx_pixelmap_height >= height)
                    {
                        /* yes, don't need to solid fill */
                        fill = GX_FALSE;
                    }
                }
            }
        }
    }

    _gx_widget_border_draw(widget, GX_COLOR_ID_WINDOW_BORDER, fill_color, fill_color, fill);

    if (pixelmap)
    {
        _gx_widget_context_fill_set(widget);
        if (widget -> gx_widget_style & GX_STYLE_TILE_WALLPAPER)
        {
            /* tile the pixelmap into the window client area */
            _gx_canvas_pixelmap_tile(&client, pixelmap);
        }
        else
        {
            /* temporarily set context clipping to my client area */
            context = _gx_system_current_draw_context;
            old_clip = context -> gx_draw_context_dirty;
            fill = _gx_utility_rectangle_overlap_detect(&client, &old_clip, &context -> gx_draw_context_dirty);

            if (fill)
            {
                /* draw the pixelmap */
                _gx_canvas_pixelmap_draw(client.gx_rectangle_left, client.gx_rectangle_top, pixelmap);
                context -> gx_draw_context_dirty = old_clip;
            }
        }
    }
}

