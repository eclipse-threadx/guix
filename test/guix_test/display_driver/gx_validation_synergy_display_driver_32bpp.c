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
/**   Application Interface (API)                                         */
/**                                                                       */
/**************************************************************************/
#include "gx_api.h"
#include "gx_system.h"
#include "gx_display.h"
#include "stdio.h"
#include "gx_dave2d_simulation_display_driver.h"

UINT gx_validation_dave2d_display_driver_setup_24xrgb(GX_DISPLAY *display);
UINT gx_validation_dave2d_display_driver_setup_24xrgb_rotated(GX_DISPLAY *display);

static VOID _gx_validation_display_buffer_24xrgb_toggle(GX_CANVAS *canvas, GX_RECTANGLE *dirty)
{
    GX_PARAMETER_NOT_USED(canvas);
    GX_PARAMETER_NOT_USED(dirty);
}


UINT gx_validation_dave2d_display_driver_setup_24xrgb(GX_DISPLAY *display)
{

    /* Initialize the low-level drawing function pointers

       for windows, these are always just the generic funcions,
       but for some hardware, these will be customized,
       optimized functions specific to that hardware
     */


    _gx_dave2d_simulation_display_driver_24xrgb_setup(display, (VOID *)1, _gx_validation_display_buffer_24xrgb_toggle);


    return(GX_SUCCESS);
}

UINT gx_validation_dave2d_display_driver_setup_24xrgb_rotated(GX_DISPLAY *display)
{

    /* Initialize the low-level drawing function pointers

       for windows, these are always just the generic funcions,
       but for some hardware, these will be customized,
       optimized functions specific to that hardware
     */


    _gx_dave2d_simulation_display_driver_24xrgb_rotated_setup(display, (VOID *)1, _gx_validation_display_buffer_24xrgb_toggle);


    return(GX_SUCCESS);
}

/*static VOID gx_validation_graphics_driver_cleanup_24xrgb(GX_DISPLAY *display)
   {

   }*/

