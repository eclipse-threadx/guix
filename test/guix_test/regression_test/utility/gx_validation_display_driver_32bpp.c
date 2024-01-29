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
#include "gx_validation_utility.h"
#include "gx_dave2d_simulation_display_driver.h"

extern int gx_validation_record_frame;
extern TEST_PARAM test_parameter;

static int width, height;
static int header_created = 0;
static GX_COLOR *rotated_memory = GX_NULL;

static void _gx_validation_rotate_canvas(GX_CANVAS *canvas)
{
/* first pass, just copy the entire canvas, ignoring the dirty
   rectangle. Come back and apply dirty rectangle to improve
   efficiency.
 */

GX_COLOR *pReadStart;
GX_COLOR *pWriteStart;
GX_COLOR *pRead;
GX_COLOR *pWrite;
INT     copy_width;
INT     copy_height;
INT     row;
INT     column;
INT     write_sign;

    /* copy left-to-right from source canvas
       and top-to-bottom in destination bitmap (90 degree rotation)
     */

    pReadStart = (GX_COLOR *)canvas -> gx_canvas_memory;
    pWriteStart = (GX_COLOR *)rotated_memory;
    copy_width = canvas -> gx_canvas_y_resolution;
    copy_height = canvas -> gx_canvas_x_resolution;

    if (canvas -> gx_canvas_display -> gx_display_rotation_angle == GX_SCREEN_ROTATION_CW)
    {
        pWriteStart += copy_height - 1;
        write_sign = -1;
    }
    else
    {
        pWriteStart += (copy_width - 1) * copy_height;
        write_sign = 1;
    }

    for (row = 0; row < copy_height; row++)
    {
        pRead = pReadStart;
        pWrite = pWriteStart;

        for (column = 0; column < copy_width; column++)
        {
            *pWrite = *pRead++;
            pWrite -= copy_height * write_sign;
        }
        pReadStart += copy_width;
        pWriteStart += write_sign;
    }
}

/**********************************************************************************/
/*Define 32bpp screen toggle  function                                            */
/**********************************************************************************/
VOID _gx_validation_display_buffer_32bpp_toggle(GX_CANVAS *canvas, GX_RECTANGLE *dirty)
{

ULONG       *memptr;
GX_COLOR    *frame_buffer;
GX_RECTANGLE overlap;
int          y;
int          copy_width;

    if(header_created == 0)
    {
        if(test_parameter.x_start < 0)
            test_parameter.x_start = 0;

        
        if(test_parameter.y_start < 0)
            test_parameter.y_start = 0;

        if((test_parameter.x_end < 0) || (test_parameter.x_end >= canvas -> gx_canvas_x_resolution))
            test_parameter.x_end = canvas -> gx_canvas_x_resolution - 1;

        if((test_parameter.y_end < 0) || (test_parameter.y_end >= canvas -> gx_canvas_y_resolution))
            test_parameter.y_end = canvas -> gx_canvas_y_resolution - 1;

        width = test_parameter.x_end - test_parameter.x_start + 1;
        height = test_parameter.y_end - test_parameter.y_start + 1;

        gx_validation_create_output_file("24xrgb", width, height);
        gx_validation_create_frame_buffer(width * height * sizeof(GX_COLOR));

        if((canvas->gx_canvas_display->gx_display_rotation_angle != GX_SCREEN_ROTATION_NONE) &&
           (canvas->gx_canvas_display->gx_display_rotation_angle != GX_SCREEN_ROTATION_FLIP))
        {
            /* Create padded memory. */
            rotated_memory = (GX_COLOR *)malloc(canvas->gx_canvas_x_resolution * canvas->gx_canvas_y_resolution * sizeof(GX_COLOR));
        }

        header_created = 1;

    }

    if(!gx_validation_frame_buffer)
    {
        return;
    }

    overlap.gx_rectangle_left = test_parameter.x_start;
    overlap.gx_rectangle_right = test_parameter.x_end;
    overlap.gx_rectangle_top = test_parameter.y_start;
    overlap.gx_rectangle_bottom = test_parameter.y_end;
 
    /* Write dirty area to gx_validation_frame_buffer. */
    if(gx_utility_rectangle_overlap_detect(dirty, &overlap, &overlap))
    {
        if(rotated_memory)
        {
            /* Rotate canvas memory to padded memory. */
            _gx_validation_rotate_canvas(canvas);
            memptr = (GX_COLOR *)rotated_memory;
        }
        else
        {
            memptr = (GX_COLOR *)(canvas -> gx_canvas_memory);
        }

        memptr = memptr + overlap.gx_rectangle_top * canvas -> gx_canvas_x_resolution;
        memptr += overlap.gx_rectangle_left;
    
        frame_buffer = (GX_COLOR *)gx_validation_frame_buffer;
        frame_buffer += (overlap.gx_rectangle_top - test_parameter.y_start) * width;
        frame_buffer += (overlap.gx_rectangle_left - test_parameter.x_start);

        copy_width = overlap.gx_rectangle_right - overlap.gx_rectangle_left + 1;

        for(y = overlap.gx_rectangle_top; y <= overlap.gx_rectangle_bottom; y++)
        {
            memcpy((char *)frame_buffer, (char *)memptr, copy_width * sizeof(GX_COLOR));
        
            frame_buffer += width;
            memptr += canvas -> gx_canvas_x_resolution;
        }
    }

    if(gx_validation_record_frame == 0)
    {
        return;
    }

    gx_validation_record_frame = 0;

    /* Dump frame data. */
    gx_validation_write_frame_buffer();
}

/**********************************************************************************/
/*Define 24xrgb display driver setup function                                     */
/**********************************************************************************/
UINT gx_validation_graphics_driver_setup_24xrgb(GX_DISPLAY *display)
{

    /* Initialize the low-level drawing function pointers

       for windows, these are always just the generic funcions,
       but for some hardware, these will be customized,
       optimized functions specific to that hardware
     */

    _gx_display_driver_24xrgb_setup(display, (VOID*)1, _gx_validation_display_buffer_32bpp_toggle);


    return(GX_SUCCESS);
}

/**********************************************************************************/
/*Define 24xrgb display driver setup function                                     */
/**********************************************************************************/
UINT gx_validation_graphics_driver_setup_24xrgb_rotated(GX_DISPLAY *display)
{

    /* Initialize the low-level drawing function pointers

       for windows, these are always just the generic functions,
       but for some hardware, these will be customized,
       optimized functions specific to that hardware
     */

    _gx_display_driver_24xrgb_rotated_setup(display, (VOID*)1, _gx_validation_display_buffer_32bpp_toggle);


    return(GX_SUCCESS);
}

VOID gx_validation_graphics_driver_cleanup_24xrgb(GX_DISPLAY *display)
{

    if(rotated_memory)
    {
        free(rotated_memory);
    }

}

/**********************************************************************************/
/*Define 32argb display driver setup function                                     */
/**********************************************************************************/
UINT gx_validation_graphics_driver_setup_32argb(GX_DISPLAY *display)
{

    /* Initialize the low-level drawing function pointers

       for windows, these are always just the generic funcions,
       but for some hardware, these will be customized,
       optimized functions specific to that hardware
     */

    _gx_display_driver_32argb_setup(display, (VOID*)1, _gx_validation_display_buffer_32bpp_toggle);


    return(GX_SUCCESS);
}

/**********************************************************************************/
/*Define rotated 32argb display driver setup function                             */
/**********************************************************************************/
UINT gx_validation_graphics_driver_setup_32argb_rotated(GX_DISPLAY *display)
{

    /* Initialize the low-level drawing function pointers

       for windows, these are always just the generic funcions,
       but for some hardware, these will be customized,
       optimized functions specific to that hardware
     */

    _gx_display_driver_32argb_rotated_setup(display, (VOID*)1, _gx_validation_display_buffer_32bpp_toggle);


    return(GX_SUCCESS);
}


/**********************************************************************************/
/*Define 24xrgb dave2d simulation display driver setup function                  */
/**********************************************************************************/
UINT gx_validation_dave2d_graphics_driver_setup_24xrgb(GX_DISPLAY *display)
{

    /* Initialize the low-level drawing function pointers

       for windows, these are always just the generic funcions,
       but for some hardware, these will be customized,
       optimized functions specific to that hardware
     */

    _gx_dave2d_simulation_display_driver_24xrgb_setup(display, (VOID*)1, _gx_validation_display_buffer_32bpp_toggle);


    return(GX_SUCCESS);
}


/**********************************************************************************/
/*Define 24xrgb dave2d simulation display driver rotated setup function          */
/**********************************************************************************/
UINT gx_validation_dave2d_graphics_driver_setup_24xrgb_rotated(GX_DISPLAY *display)
{

    /* Initialize the low-level drawing function pointers

       for windows, these are always just the generic functions,
       but for some hardware, these will be customized,
       optimized functions specific to that hardware
     */

    _gx_dave2d_simulation_display_driver_24xrgb_rotated_setup(display, (VOID*)1, _gx_validation_display_buffer_32bpp_toggle);


    return(GX_SUCCESS);
}
