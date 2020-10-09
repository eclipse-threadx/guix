
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
/*    _gx_display_driver_4444argb_setup                   PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Generic 16-bit 4444ARGB color format display driver setup routine.  */
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
/*    _gx_display_driver_565rgb_setup       Basic display driver setup .  */
/*                                            function for generic 565rgb */
/*                                            color format                */
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
VOID _gx_display_driver_4444argb_setup(GX_DISPLAY *display, VOID *aux_data,
                                       VOID (*toggle_function)(struct GX_CANVAS_STRUCT *canvas,
                                                               GX_RECTANGLE *dirty_area))
{
    _gx_display_driver_565rgb_setup(display, aux_data, toggle_function);
    display -> gx_display_color_format                           = GX_COLOR_FORMAT_4444ARGB;
    display -> gx_display_driver_native_color_get                = _gx_display_driver_4444argb_native_color_get;
    display -> gx_display_driver_pixelmap_draw                   = _gx_display_driver_4444argb_pixelmap_draw;
    display -> gx_display_driver_pixelmap_blend                  = _gx_display_driver_4444argb_pixelmap_blend;
    display -> gx_display_driver_canvas_blend                    = _gx_display_driver_4444argb_canvas_blend;
    display -> gx_display_driver_pixel_blend                     = _gx_display_driver_4444argb_pixel_blend;
    display -> gx_display_driver_horizontal_pixelmap_line_draw   = _gx_display_driver_4444argb_horizontal_pixelmap_line_draw;
    display -> gx_display_driver_pixelmap_rotate                 = _gx_display_driver_4444argb_pixelmap_rotate;
}

