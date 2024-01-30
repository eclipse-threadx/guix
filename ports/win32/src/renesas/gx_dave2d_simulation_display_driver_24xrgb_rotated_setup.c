
/***************************************************************************
 * Copyright (c) 2024 Microsoft Corporation 
 * 
 * This program and the accompanying materials are made available under the
 * terms of the MIT License which is available at
 * https://opensource.org/licenses/MIT.
 * 
 * SPDX-License-Identifier: MIT
 **************************************************************************/


/**************************************************************************/
/**************************************************************************/
/**                                                                       */
/** GUIX Component                                                        */
/**                                                                       */
/**   Synergy Simulation Display Management (Display)                     */
/**                                                                       */
/**************************************************************************/
#define GX_SOURCE_CODE

#include "gx_api.h"
#include "gx_display.h"
#include "gx_dave2d_simulation_display_driver.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_dave2d_simulation_display_driver_24xrgb_rotated_setup           */
/*                                                                        */
/*                                                        PORTABLE C      */
/*                                                           6.4.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Ting Zhu, Microsoft Corporation                                     */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Synergy simulation generic 32-bit XRGB rotated color format display */
/*    driver setup routine.                                               */
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
/*  12-31-2023     Ting Zhu                 Initial Version 6.4.0         */
/*                                                                        */
/**************************************************************************/
VOID _gx_dave2d_simulation_display_driver_24xrgb_rotated_setup(GX_DISPLAY *display, VOID *aux_data,
                                                                VOID (*toggle_function)(struct GX_CANVAS_STRUCT *canvas,
                                                                                        GX_RECTANGLE *dirty_area))
{
    _gx_display_driver_24xrgb_rotated_setup(display, aux_data, toggle_function);

    display -> gx_display_driver_pixelmap_draw                 = _gx_dave2d_simulation_display_driver_24xrgb_rotated_pixelmap_draw;
    display -> gx_display_driver_pixelmap_blend                = _gx_dave2d_simulation_display_driver_24xrgb_rotated_pixelmap_blend;
    display -> gx_display_driver_4bit_glyph_draw               = _gx_dave2d_simulation_display_driver_rotated_glyph_4bit_draw;
    display -> gx_display_driver_1bit_glyph_draw               = _gx_dave2d_simulation_display_driver_32bpp_rotated_glyph_1bit_draw;
    display -> gx_display_driver_8bit_compressed_glyph_draw    = _gx_dave2d_simulation_display_driver_rotated_compressed_glyph_8bit_draw;
    display -> gx_display_driver_4bit_compressed_glyph_draw    = _gx_dave2d_simulation_display_driver_rotated_compressed_glyph_4bit_draw;
    display -> gx_display_driver_1bit_compressed_glyph_draw    = _gx_dave2d_simulation_display_driver_32bpp_rotated_compressed_glyph_1bit_draw;
    display -> gx_display_driver_horizontal_pixelmap_line_draw = _gx_dave2d_simulation_display_driver_32bpp_rotated_horizontal_pixelmap_line_draw;
}

