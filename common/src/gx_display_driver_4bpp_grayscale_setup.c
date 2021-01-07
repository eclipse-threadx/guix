
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
/**   Display Management (Display)                                        */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE
/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_display.h"

#if defined(GX_MOUSE_SUPPORT) && !defined(GX_HARDWARE_MOUSE_SUPPORT)
static GX_UBYTE mouse_capture_memory[GX_MOUSE_MAX_RESOLUTION * GX_MOUSE_MAX_RESOLUTION / 4] = { 0 };
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_4bpp_grayscale_setup             PORTABLE C      */
/*                                                           6.1.3        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Generic 4bpp color format display driver setup routine.             */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    display                               The display control block     */
/*    aux_data                              Driver-defined auxiliary data */
/*    toggle_function                       Driver-defined screen toggle  */
/*                                            function                    */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
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
/*  12-31-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            added rotation angle        */
/*                                            initialization,             */
/*                                            resulting in version 6.1.3  */
/*                                                                        */
/**************************************************************************/
VOID _gx_display_driver_4bpp_grayscale_setup(GX_DISPLAY *display, VOID *aux_data,
                                             VOID (*toggle_function)(struct GX_CANVAS_STRUCT *canvas,
                                                                     GX_RECTANGLE *dirty_area))
{
    /* Default initiate and complete function to null for general condition. */
    display -> gx_display_driver_drawing_initiate              = GX_NULL;
    display -> gx_display_driver_drawing_complete              = GX_NULL;

#if defined(GX_MOUSE_SUPPORT)
#if defined(GX_HARDWARE_MOUSE_SUPPORT)
    display -> gx_display_mouse_position_set                   = GX_NULL;
    display -> gx_display_mouse_enable                         = GX_NULL;
#else
    display -> gx_display_mouse.gx_mouse_capture_memory        = (GX_UBYTE *)mouse_capture_memory;
    display -> gx_display_mouse.gx_mouse_status                = 0;
    
    display -> gx_display_mouse.gx_mouse_position.gx_point_x   = display -> gx_display_width / 2;
    display -> gx_display_mouse.gx_mouse_position.gx_point_y   = display -> gx_display_height / 2;
    
    /* these functions are specific to the display color format, and will be NULL for hardware mouse */
    display -> gx_display_mouse_capture                        = _gx_display_driver_4bpp_mouse_capture;
    display -> gx_display_mouse_restore                        = _gx_display_driver_4bpp_mouse_restore;
    display -> gx_display_mouse_draw                           = _gx_display_driver_generic_mouse_draw;
    display -> gx_display_driver_drawing_initiate              = _gx_display_driver_generic_drawing_initiate;
    display -> gx_display_driver_drawing_complete              = _gx_display_driver_generic_drawing_complete;
    display -> gx_display_mouse_position_set                   = _gx_display_driver_generic_mouse_position_set;
    display -> gx_display_mouse_enable                         = _gx_display_driver_generic_mouse_enable;
#endif

    /* these functions are generic, same for every color depth, but will be overridden for hardware mouse */
    display -> gx_display_mouse.gx_mouse_cursor_info           = GX_NULL;
    display -> gx_display_mouse_define                         = _gx_display_driver_generic_mouse_define;
#endif

    display -> gx_display_rotation_angle                       = 0;
    display -> gx_display_driver_data                          = (VOID *)aux_data;
    display -> gx_display_accelerator                          = GX_NULL;
    display -> gx_display_layer_services                       = GX_NULL;
    display -> gx_display_driver_callback_assign               = GX_NULL;

    display -> gx_display_color_format                         = GX_COLOR_FORMAT_4BIT_GRAY;
    display -> gx_display_driver_canvas_copy                   = _gx_display_driver_4bpp_canvas_copy;
    display -> gx_display_driver_simple_line_draw              = _gx_display_driver_4bpp_simple_line_draw;
    display -> gx_display_driver_horizontal_line_draw          = _gx_display_driver_4bpp_horizontal_line_draw;
    display -> gx_display_driver_vertical_line_draw            = _gx_display_driver_4bpp_vertical_line_draw;
    display -> gx_display_driver_horizontal_pattern_line_draw  = _gx_display_driver_4bpp_horizontal_pattern_line_draw;
    display -> gx_display_driver_vertical_pattern_line_draw    = _gx_display_driver_4bpp_vertical_pattern_line_draw;
    display -> gx_display_driver_horizontal_pixelmap_line_draw = _gx_display_driver_4bpp_horizontal_pixelmap_line_draw;
    display -> gx_display_driver_pixel_write                   = _gx_display_driver_4bpp_pixel_write;

    display -> gx_display_driver_native_color_get              = _gx_display_driver_4bpp_native_color_get;
    display -> gx_display_driver_row_pitch_get                 = _gx_display_driver_4bpp_row_pitch_get;
    display -> gx_display_driver_block_move                    = _gx_display_driver_4bpp_block_move;
    display -> gx_display_driver_pixelmap_draw                 = _gx_display_driver_4bpp_pixelmap_draw;

    display -> gx_display_driver_alphamap_draw                 = GX_NULL;
    display -> gx_display_driver_anti_aliased_line_draw        = GX_NULL;
    display -> gx_display_driver_anti_aliased_wide_line_draw   = GX_NULL;
    display -> gx_display_driver_pixelmap_rotate               = _gx_display_driver_4bpp_pixelmap_rotate;
        
    display -> gx_display_driver_simple_wide_line_draw         = _gx_display_driver_generic_simple_wide_line_draw;
    display -> gx_display_driver_polygon_draw                  = _gx_display_driver_generic_polygon_draw;
    display -> gx_display_driver_polygon_fill                  = _gx_display_driver_generic_polygon_fill;

#if defined(GX_ARC_DRAWING_SUPPORT)
    display -> gx_display_driver_anti_aliased_circle_draw      = GX_NULL;
    display -> gx_display_driver_anti_aliased_ellipse_draw     = GX_NULL;
    display -> gx_display_driver_circle_draw                   = _gx_display_driver_generic_circle_draw;
    display -> gx_display_driver_circle_fill                   = _gx_display_driver_generic_circle_fill;
    display -> gx_display_driver_pie_fill                      = _gx_display_driver_generic_pie_fill;
    display -> gx_display_driver_anti_aliased_arc_draw         = GX_NULL;
    display -> gx_display_driver_arc_draw                      = _gx_display_driver_generic_arc_draw;
    display -> gx_display_driver_arc_fill                      = _gx_display_driver_generic_arc_fill;
    display -> gx_display_driver_ellipse_draw                  = _gx_display_driver_generic_ellipse_draw;
    display -> gx_display_driver_ellipse_fill                  = _gx_display_driver_generic_ellipse_fill;
    display -> gx_display_driver_anti_aliased_wide_circle_draw = GX_NULL;
    display -> gx_display_driver_wide_circle_draw              = _gx_display_driver_generic_wide_circle_draw;
    display -> gx_display_driver_anti_aliased_wide_ellipse_draw= GX_NULL;
    display -> gx_display_driver_wide_ellipse_draw             = _gx_display_driver_generic_wide_ellipse_draw;
    display -> gx_display_driver_anti_aliased_wide_arc_draw    = GX_NULL;
    display -> gx_display_driver_wide_arc_draw                 = _gx_display_driver_generic_wide_arc_draw;
#endif

    display -> gx_display_driver_8bit_glyph_draw               = GX_NULL;
    display -> gx_display_driver_4bit_glyph_draw               = _gx_display_driver_4bpp_glyph_4bit_draw;
    display -> gx_display_driver_1bit_glyph_draw               = _gx_display_driver_4bpp_glyph_1bit_draw;


    display -> gx_display_driver_palette_set                   = GX_NULL;
    display -> gx_display_driver_buffer_toggle                 = toggle_function;

    display -> gx_display_driver_canvas_blend                  = GX_NULL;
    display -> gx_display_driver_pixel_blend                   = GX_NULL;
    display -> gx_display_driver_pixelmap_blend                = GX_NULL;

    display -> gx_display_driver_8bit_compressed_glyph_draw    = GX_NULL;
    display -> gx_display_driver_4bit_compressed_glyph_draw    = GX_NULL;
    display -> gx_display_driver_1bit_compressed_glyph_draw    = GX_NULL;

#if defined(GX_SOFTWARE_DECODER_SUPPORT)
    display -> gx_display_driver_jpeg_draw                     = GX_NULL;
    display -> gx_display_driver_png_draw                      = GX_NULL;
#endif
}

