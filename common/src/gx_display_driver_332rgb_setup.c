
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

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_332rgb_setup                     PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Generic 8-bit 332RGB color format display driver setup routine.     */
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
/*                                                                        */
/**************************************************************************/
VOID _gx_display_driver_332rgb_setup(GX_DISPLAY *display, VOID *aux_data,
                                     VOID (*toggle_function)(struct GX_CANVAS_STRUCT *canvas,
                                                             GX_RECTANGLE *dirty_area))
{
    _gx_display_driver_8bit_palette_setup(display, aux_data, toggle_function);

    display -> gx_display_color_format                         = GX_COLOR_FORMAT_8BIT_PACKED_PIXEL;
    display -> gx_display_driver_native_color_get              = _gx_display_driver_332rgb_native_color_get;
    display -> gx_display_driver_pixelmap_draw                 = _gx_display_driver_332rgb_pixelmap_draw;
    display -> gx_display_driver_alphamap_draw                 = _gx_display_driver_generic_alphamap_draw;
    display -> gx_display_driver_anti_aliased_line_draw        = _gx_display_driver_generic_aliased_line_draw;
    display -> gx_display_driver_anti_aliased_wide_line_draw   = _gx_display_driver_generic_aliased_wide_line_draw;

#if defined(GX_ARC_DRAWING_SUPPORT)
    display -> gx_display_driver_anti_aliased_circle_draw      = _gx_display_driver_generic_aliased_circle_draw;
    display -> gx_display_driver_anti_aliased_ellipse_draw     = _gx_display_driver_generic_aliased_ellipse_draw;
    display -> gx_display_driver_anti_aliased_arc_draw         = _gx_display_driver_generic_aliased_arc_draw;
    display -> gx_display_driver_anti_aliased_wide_circle_draw = _gx_display_driver_generic_aliased_wide_circle_draw;
    display -> gx_display_driver_anti_aliased_wide_ellipse_draw = _gx_display_driver_generic_aliased_wide_ellipse_draw;
    display -> gx_display_driver_anti_aliased_wide_arc_draw    = _gx_display_driver_generic_aliased_wide_arc_draw;

#endif

    display -> gx_display_driver_horizontal_pixelmap_line_draw = _gx_display_driver_332rgb_horizontal_pixelmap_line_draw;
    display -> gx_display_driver_pixel_blend                   = _gx_display_driver_332rgb_pixel_blend;
    display -> gx_display_driver_pixelmap_blend                = _gx_display_driver_8bpp_pixelmap_blend;
    display -> gx_display_driver_pixelmap_rotate               = _gx_display_driver_332rgb_pixelmap_rotate;
    display -> gx_display_driver_4bit_glyph_draw               = _gx_display_driver_generic_glyph_4bit_draw;
    display -> gx_display_driver_8bit_glyph_draw               = GX_NULL;
    display -> gx_display_driver_8bit_compressed_glyph_draw    = GX_NULL;
    display -> gx_display_driver_4bit_compressed_glyph_draw    = GX_NULL;
    display -> gx_display_driver_1bit_compressed_glyph_draw    = GX_NULL;
}

