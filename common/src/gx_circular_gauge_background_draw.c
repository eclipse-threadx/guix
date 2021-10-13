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
/**   Circular Gauge Management (Circular Gauge)                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_icon.h"
#include "gx_system.h"
#include "gx_context.h"
#include "gx_canvas.h"
#include "gx_circular_gauge.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_circular_gauge_background_draw                  PORTABLE C      */
/*                                                           6.1.9        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function draws background of the specified circular gauge.     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    gauge                                 Pointer to circular gauge     */
/*                                            control block               */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_icon_background_draw              Draw icon background          */
/*    _gx_context_pixelmap_get              Gets the pixelmap associated  */
/*                                            the supplied resource ID.   */
/*    _gx_circular_gauge_needle_rotate      Needle rotate function        */
/*    _gx_canvas_pixelmap_draw              Draw the specified pixelmap.  */
/*    _gx_system_memory_free                Free memory.                  */
/*    [gx_display_driver_callback_assign]   Assigns widget callback       */
/*                                            function                    */
/*    _gx_circular_gauge_needle_rotate_callback                           */
/*                                          Needle rotate callback        */
/*                                            function                    */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    GUIX Internal Code                                                  */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*  10-15-2021     Kenneth Maxwell          Modified comment(s),          */
/*                                            fixed the order of setting  */
/*                                            the display driver callback */
/*                                            and drawing the background, */ 
/*                                            resulting in version 6.1.9  */
/*                                                                        */
/**************************************************************************/
VOID  _gx_circular_gauge_background_draw(GX_CIRCULAR_GAUGE *gauge)
{
GX_PIXELMAP            *map;
GX_CIRCULAR_GAUGE_INFO *info;
GX_DISPLAY             *display;
GX_BOOL                 free_mem;

/* pickup pointer to current context */
GX_DRAW_CONTEXT *context = _gx_system_current_draw_context;

    display = context -> gx_draw_context_display;


    /* Pick up gauge information.  */
    info = &gauge -> gx_circular_gauge_info;
    free_mem = GX_FALSE;

    if (gauge -> gx_circular_gauge_info.gx_circular_gauge_info_needle_pixelmap)
    {
        /* get pointer to needle source image */
        _gx_context_pixelmap_get(gauge -> gx_circular_gauge_info.gx_circular_gauge_info_needle_pixelmap,
                                 &gauge -> gx_circular_gauge_needle_source);
    }

    if (gauge -> gx_circular_gauge_current_angle != 0)
    {
        if (display -> gx_display_driver_callback_assign)
        {
            /* rotate the needle while gauge background is being drawn */
            display -> gx_display_driver_callback_assign(_gx_circular_gauge_needle_rotate_callback, gauge);
        }
        else
        {
            /* rotate the needle inline: */
            _gx_circular_gauge_needle_rotate(gauge);
        }
        map = &gauge -> gx_circular_gauge_needle_rotated;
        free_mem = GX_TRUE;
    }
    else
    {
        map = gauge -> gx_circular_gauge_needle_source;
        gauge -> gx_circular_gauge_current_needle_x = gauge -> gx_widget_size.gx_rectangle_left +
            info -> gx_circular_gauge_info_needle_xpos -
            info -> gx_circular_gauge_info_needle_xcor;
        gauge -> gx_circular_gauge_current_needle_y = gauge -> gx_widget_size.gx_rectangle_top +
            info -> gx_circular_gauge_info_needle_ypos -
            info -> gx_circular_gauge_info_needle_ycor;
    }
    
    /* Call icon draw function to draw the background.  */
    _gx_icon_background_draw((GX_ICON *)gauge);

    if ((map != GX_NULL) && (map -> gx_pixelmap_data != GX_NULL))
    {
        /* Draw needle .  */
        _gx_canvas_pixelmap_draw((GX_VALUE)gauge -> gx_circular_gauge_current_needle_x,
                                 (GX_VALUE)gauge -> gx_circular_gauge_current_needle_y,
                                 map);

        /* Free rotate needle pixelmap memory.  */
        if (_gx_system_memory_free && free_mem)
        {
            if (map -> gx_pixelmap_aux_data)
            {
                _gx_system_memory_free((VOID *)map -> gx_pixelmap_aux_data);
                map -> gx_pixelmap_aux_data = GX_NULL;
            }
            _gx_system_memory_free((VOID *)map -> gx_pixelmap_data);
            map -> gx_pixelmap_data = GX_NULL;
        }
    }
}

