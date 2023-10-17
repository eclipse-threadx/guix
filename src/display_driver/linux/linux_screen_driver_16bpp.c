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
/**   STM324XG Screen Driver                                              */
/**                                                                       */
/**************************************************************************/


#include   "tx_api.h"
#include   "gx_api.h"
#include   "gx_system.h"
#include   "gx_display.h"

void linux_565rgb_buffer_toggle(GX_CANVAS *canvas, GX_RECTANGLE *dirty)
{

#if 0
GX_RECTANGLE    Limit;
GX_RECTANGLE    Copy;
INT             x;
INT             y;
USHORT*        pixel;


    USHORT *memptr = (USHORT*)canvas -> gx_canvas_memory;

    gx_utility_rectangle_define(&Limit, 0, 0,
                                STM324XG_SCREEN_WIDTH - 1, STM324XG_SCREEN_HEIGHT - 1);

    if (gx_utility_rectangle_overlap_detect(&Limit, &canvas->gx_canvas_dirty_area, &Copy))
    {

        for(y = Copy.gx_rectangle_top;  y <= Copy.gx_rectangle_bottom; y++)
        {

            LCD_SetCursor(y, STM324XG_SCREEN_WIDTH - 1 - Copy.gx_rectangle_left);
            LCD_WriteRAM_Prepare();
            pixel = memptr + y * STM324XG_SCREEN_WIDTH;
            for(x = Copy.gx_rectangle_left; x <= Copy.gx_rectangle_right; x++)
            {
                LCD_WriteRAM(*(pixel + x));
            }
        }
    }
#endif
}


UINT stm324xg_graphics_driver_16bpp(GX_DISPLAY *display)
{
    /* initialize the low-level drawing function pointers

       for windows, these are always just the generic funcions,
       but for some hardware, these will be customized,
       optimized functions specific to that hardware
    */
    
    display -> gx_display_driver_canvas_copy                   = _gx_display_driver_16bpp_canvas_copy;
    display -> gx_display_driver_canvas_blend                  = _gx_display_driver_565rgb_canvas_blend;

    display -> gx_display_driver_simple_line_draw              = _gx_display_driver_16bpp_simple_line_draw;
    display -> gx_display_driver_simple_wide_line_draw         = _gx_display_driver_generic_simple_wide_line_draw;
    display -> gx_display_driver_anti_aliased_line_draw        = _gx_display_driver_generic_aliased_line_draw;
    display -> gx_display_driver_anti_aliased_wide_line_draw   = _gx_display_driver_generic_aliased_wide_line_draw;

    display -> gx_display_driver_horizontal_line_draw          = _gx_display_driver_16bpp_horizontal_line_draw;
    display -> gx_display_driver_vertical_line_draw            = _gx_display_driver_16bpp_vertical_line_draw;
    display -> gx_display_driver_pixelmap_blend                = _gx_display_driver_565rgb_pixelmap_blend;
    display -> gx_display_driver_pixelmap_draw                 = _gx_display_driver_565rgb_pixelmap_draw;
    display -> gx_display_driver_horizontal_pattern_line_draw  = _gx_display_driver_16bpp_horizontal_pattern_line_draw;
    display -> gx_display_driver_vertical_pattern_line_draw    = _gx_display_driver_16bpp_vertical_pattern_line_draw;
    display -> gx_display_driver_pixel_write                   = _gx_display_driver_16bpp_pixel_write;
    display -> gx_display_driver_pixel_blend                   = _gx_display_driver_565rgb_pixel_blend;
    display -> gx_display_driver_block_move                    = _gx_display_driver_16bpp_block_move;
    display -> gx_display_driver_native_color_get              = _gx_display_driver_565rgb_native_color_get;
    display -> gx_display_driver_row_pitch_get                 = _gx_display_driver_16bpp_row_pitch_get;
    display -> gx_display_driver_polygon_draw                  = _gx_display_driver_generic_polygon_draw;
    display -> gx_display_driver_polygon_fill                  = _gx_display_driver_generic_polygon_fill;

    #if defined(GX_ARC_API_SUPPORT)
    display -> gx_display_driver_anti_aliased_circle_draw      = _gx_display_driver_generic_aliased_circle_draw;
    display -> gx_display_driver_circle_draw                   = _gx_display_driver_generic_circle_draw;
    display -> gx_display_driver_circle_fill                   = _gx_display_driver_generic_circle_fill;
    display -> gx_display_driver_pie_fill                      = _gx_display_driver_generic_pie_fill;
    display -> gx_display_driver_anti_aliased_arc_draw         = _gx_display_driver_generic_aliased_arc_draw;
    display -> gx_display_driver_arc_draw                      = _gx_display_driver_generic_arc_draw;
    display -> gx_display_driver_arc_fill                      = _gx_display_driver_generic_arc_fill;        
    #endif

    display -> gx_display_driver_buffer_toggle                 = linux_565rgb_buffer_toggle;
    

    /* Return successful status.  */
    return(GX_SUCCESS);
}

