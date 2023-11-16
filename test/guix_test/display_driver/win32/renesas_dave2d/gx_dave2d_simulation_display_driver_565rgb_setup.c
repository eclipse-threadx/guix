
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
/*    _gx_dave2d_s_display_driver_565rgb_setup           PORTABLE C       */
/*                                                           6.2.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Generic 16-bit 565RGB color format display driver setup routine.    */
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
/*  12-31-2020     Kenneth Maxwell          Initial Version 6.1.3         */
/*  10-31-2022     Kenneth Maxwell          Modify comments, change       */
/*                                            function name,              */
/*                                            resulting in Version 6.2.0  */
/*                                                                        */
/**************************************************************************/

VOID _gx_dave2d_simulation_display_driver_565rgb_setup(GX_DISPLAY *display, VOID *aux_data,
                                                        VOID (*toggle_function)(struct GX_CANVAS_STRUCT *canvas,
                                                                                GX_RECTANGLE *dirty_area))
{

    _gx_display_driver_565rgb_setup(display, aux_data, toggle_function);

    display -> gx_display_driver_pixelmap_draw                 = _gx_dave2d_simulation_display_driver_16bpp_pixelmap_draw;
    display -> gx_display_driver_pixelmap_blend                = _gx_dave2d_simulation_display_driver_565rgb_pixelmap_blend;
    display -> gx_display_driver_4bit_glyph_draw               = _gx_dave2d_simulation_display_driver_glyph_4bit_draw;
    display -> gx_display_driver_1bit_glyph_draw               = _gx_dave2d_simulation_display_driver_16bpp_glyph_1bit_draw;
    display -> gx_display_driver_8bit_compressed_glyph_draw    = _gx_dave2d_simulation_display_driver_compressed_glyph_8bit_draw;
    display -> gx_display_driver_4bit_compressed_glyph_draw    = _gx_dave2d_simulation_display_driver_compressed_glyph_4bit_draw;
    display -> gx_display_driver_1bit_compressed_glyph_draw    = _gx_dave2d_simulation_display_driver_16bpp_compressed_glyph_1bit_draw;
    display -> gx_display_driver_horizontal_pixelmap_line_draw = _gx_dave2d_simulation_display_driver_565rgb_horizontal_pixelmap_line_draw;
}

