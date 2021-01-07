
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
/**   ST Simulation Display Management (Display)                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE
/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_display.h"


extern VOID _gx_chromart_simulation_display_driver_565rgb_pixelmap_draw(GX_DRAW_CONTEXT *context,
                                                                        INT xpos, INT ypos, GX_PIXELMAP *pixelmap);
/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_chromart_simulation_display_driver_565rgb_setup PORTABLE C      */
/*                                                           6.1.3        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function initiates ST simuation display driver.                */
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
/*    Application Code                                                    */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  12-31-2020     Kenneth Maxwell          Initial Version 6.1.3         */
/*                                                                        */
/**************************************************************************/
VOID _gx_chromart_simulation_display_driver_565rgb_setup(GX_DISPLAY *display, VOID *aux_data,
                                                         VOID (*toggle_function)(struct GX_CANVAS_STRUCT *canvas,
                                                                                 GX_RECTANGLE *dirty_area))
{
    /* Call generic 565rgb display driver setup. */
    _gx_display_driver_565rgb_setup(display, aux_data, toggle_function);

    display -> gx_display_driver_pixelmap_draw                 = _gx_chromart_simulation_display_driver_565rgb_pixelmap_draw;
}

