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
static char* color_format = COLOR_FORMAT_8BPP;
static GX_UBYTE *rotated_memory = GX_NULL;

static void _gx_validation_rotate_canvas(GX_CANVAS *canvas)
{
/* First pass, just copy the entire canvas, ignoring the dirty
   rectangle. Come back and apply dirty rectangle to improve
   efficiency.  */

GX_UBYTE *pReadStart;
GX_UBYTE *pWriteStart;
GX_UBYTE *pRead;
GX_UBYTE *pWrite;
INT       copy_width;
INT       copy_height;
INT       row;
INT       column;
INT       write_sign;

    /* Copy left-to-right from source canvas
       and top-to-bottom in destination bitmap (90 degree rotation).  */

    pReadStart = (GX_UBYTE *)canvas -> gx_canvas_memory;
    pWriteStart = (GX_UBYTE *)rotated_memory;
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
/*Define 8bpp screen toggle  function                                             */
/**********************************************************************************/
VOID _gx_validation_display_buffer_8bpp_toggle(GX_CANVAS *canvas, GX_RECTANGLE *dirty)
{
    GX_UBYTE *memptr;
    GX_UBYTE *frame_buffer;
    GX_RECTANGLE overlap;
    int copy_width;
    int y;

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

        if(width & 3)
        {
            width += 4 - (width & 3);
        }

        gx_validation_create_output_file(color_format, width, height);
        gx_validation_create_frame_buffer(width * height);

        if((canvas->gx_canvas_display->gx_display_rotation_angle != GX_SCREEN_ROTATION_NONE) &&
           (canvas->gx_canvas_display->gx_display_rotation_angle != GX_SCREEN_ROTATION_FLIP))
        {
            /* Create padded memory. */
            rotated_memory = (GX_UBYTE *)malloc(canvas->gx_canvas_x_resolution * canvas->gx_canvas_y_resolution);
        }

        header_created = 1;
    }

    if(!gx_validation_frame_buffer)
    {
        return;
    }

    gx_utility_rectangle_define(&overlap, test_parameter.x_start, test_parameter.y_start,
        test_parameter.x_end, test_parameter.y_end);

    if(gx_utility_rectangle_overlap_detect(dirty, &overlap, &overlap))
    {
        if(rotated_memory)
        {
            /* Rotate canvas memory to padded memory. */
            _gx_validation_rotate_canvas(canvas);
            memptr = (GX_UBYTE *)rotated_memory;
        }
        else
        {
            memptr = (GX_UBYTE *)(canvas -> gx_canvas_memory);
        }

        memptr += overlap.gx_rectangle_top * canvas -> gx_canvas_x_resolution;
        memptr += overlap.gx_rectangle_left;

        frame_buffer = (GX_UBYTE *)gx_validation_frame_buffer;
        frame_buffer += (overlap.gx_rectangle_top - test_parameter.y_start) * width;
        frame_buffer += (overlap.gx_rectangle_left - test_parameter.x_start);

        copy_width = overlap.gx_rectangle_right - overlap.gx_rectangle_left + 1;

        for(y = overlap.gx_rectangle_top; y <= overlap.gx_rectangle_bottom; y ++)
        {
            memcpy((char *)frame_buffer, (char *)memptr, copy_width);
            frame_buffer += width;
            memptr += canvas -> gx_canvas_x_resolution;
        }
    }

    if(gx_validation_record_frame == 0)
    {
        return;
    }

    gx_validation_record_frame = 0;

    /* Dump frame area */
    gx_validation_write_frame_buffer();
}
/**********************************************************************************/
/*Define 8bit palette display driver palette setup function                             */
/**********************************************************************************/
VOID _gx_validation_graphics_driver_8bit_palette_set(GX_DISPLAY *display, GX_COLOR *palette, INT count)
{
    display -> gx_display_palette = palette;
    display -> gx_display_palette_size = count;  
}

/**********************************************************************************/
/*Define 8bit palette display driver setup function                               */
/**********************************************************************************/
UINT gx_validation_graphics_driver_setup_8bit_palette(GX_DISPLAY *display)
{

    /* Initialize the low-level drawing function pointers

       for windows, these are always just the generic funcions,
       but for some hardware, these will be customized,
       optimized functions specific to that hardware
     */

    color_format = COLOR_FORMAT_8BPP;
    _gx_display_driver_8bit_palette_setup(display, (VOID*)1, _gx_validation_display_buffer_8bpp_toggle);
    display -> gx_display_driver_palette_set = _gx_validation_graphics_driver_8bit_palette_set;


    return(GX_SUCCESS);
}

/**********************************************************************************/
/*Define 8bit palette rotated display driver setup function                       */
/**********************************************************************************/
UINT gx_validation_graphics_driver_setup_8bit_palette_rotated(GX_DISPLAY *display)
{

    /* Initialize the low-level drawing function pointers

       for windows, these are always just the generic funcions,
       but for some hardware, these will be customized,
       optimized functions specific to that hardware
     */

    color_format = COLOR_FORMAT_8BPP;
    _gx_display_driver_8bit_palette_rotated_setup(display, (VOID*)1, _gx_validation_display_buffer_8bpp_toggle);
    display -> gx_display_driver_palette_set = _gx_validation_graphics_driver_8bit_palette_set;


    return(GX_SUCCESS);
}

VOID gx_validation_graphics_driver_cleanup_8bit_palette(GX_DISPLAY *display)
{
    if(rotated_memory)
    {
        free(rotated_memory);
    }
}

UINT gx_validation_graphics_driver_setup_332rgb(GX_DISPLAY *display)
{

    /* Initialize the low-level drawing function pointers

       for windows, these are always just the generic funcions,
       but for some hardware, these will be customized,
       optimized functions specific to that hardware
     */
    color_format = COLOR_FORMAT_332RGB;
    _gx_display_driver_332rgb_setup(display, (VOID*)1, _gx_validation_display_buffer_8bpp_toggle);

    return(GX_SUCCESS);
}

