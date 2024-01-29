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

VOID _gx_validation_display_buffer_24xrgb_toggle(GX_CANVAS *canvas, GX_RECTANGLE *dirty);
UINT gx_validation_display_driver_setup_24xrgb(GX_DISPLAY *display);
VOID gx_validation_graphics_driver_cleanup_24xrgb(GX_DISPLAY *display);
UINT gx_validation_display_driver_setup_32argb(GX_DISPLAY *display);

#define _gx_validation_display_buffer_32argb_toggle _gx_validation_display_buffer_24xrgb_toggle

VOID _gx_validation_display_buffer_24xrgb_toggle(GX_CANVAS *canvas, GX_RECTANGLE *dirty)
{
    GX_PARAMETER_NOT_USED(canvas);
    GX_PARAMETER_NOT_USED(dirty);
}



UINT gx_validation_display_driver_setup_24xrgb(GX_DISPLAY *display)
{

    /* Initialize the low-level drawing function pointers

       for windows, these are always just the generic funcions,
       but for some hardware, these will be customized,
       optimized functions specific to that hardware
     */


    _gx_display_driver_24xrgb_setup(display, (VOID *)1, _gx_validation_display_buffer_24xrgb_toggle);


    return(GX_SUCCESS);
}

UINT gx_validation_display_driver_setup_24xrgb_rotated(GX_DISPLAY *display)
{

    /* Initialize the low-level drawing function pointers

       for windows, these are always just the generic funcions,
       but for some hardware, these will be customized,
       optimized functions specific to that hardware
     */


    _gx_display_driver_24xrgb_rotated_setup(display, (VOID *)1, _gx_validation_display_buffer_24xrgb_toggle);


    return(GX_SUCCESS);
}

VOID gx_validation_graphics_driver_cleanup_24xrgb(GX_DISPLAY *display)
{

    /* Do nothing */
    GX_PARAMETER_NOT_USED(display);
}

UINT gx_validation_display_driver_setup_32argb(GX_DISPLAY *display)
{

    /* Initialize the low-level drawing function pointers

       for windows, these are always just the generic funcions,
       but for some hardware, these will be customized,
       optimized functions specific to that hardware
     */


    _gx_display_driver_32argb_setup(display, (VOID *)1, _gx_validation_display_buffer_32argb_toggle);


    return(GX_SUCCESS);
}

UINT gx_validation_display_driver_setup_32argb_rotated(GX_DISPLAY *display)
{

    /* Initialize the low-level drawing function pointers

       for windows, these are always just the generic funcions,
       but for some hardware, these will be customized,
       optimized functions specific to that hardware
     */


    _gx_display_driver_32argb_rotated_setup(display, (VOID *)1, _gx_validation_display_buffer_32argb_toggle);


    return(GX_SUCCESS);
}
