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

static int   width, height;
static int   header_created = 0;
static char *color_format = COLOR_FORMAT_565RGB;
static USHORT *rotated_memory = GX_NULL;

static void _gx_validation_rotate_canvas(GX_CANVAS *canvas)
{
/* first pass, just copy the entire canvas, ignoring the dirty
   rectangle. Come back and apply dirty rectangle to improve
   efficiency.
 */

USHORT *pReadStart;
USHORT *pWriteStart;
USHORT *pRead;
USHORT *pWrite;
INT     copy_width;
INT     copy_height;
INT     row;
INT     column;
INT     write_sign;

    /* copy left-to-right from source canvas
       and top-to-bottom in destination bitmap (90 degree rotation)
     */

    pReadStart = (USHORT *)canvas -> gx_canvas_memory;
    pWriteStart = (USHORT *)rotated_memory;
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
/*Define 16bpp screen toggle function                                             */
/**********************************************************************************/
VOID _gx_validation_display_buffer_16bpp_toggle(GX_CANVAS *canvas, GX_RECTANGLE *dirty)
{

    USHORT *memptr;
    USHORT *frame_buffer;
    GX_RECTANGLE overlap;
    GX_RECTANGLE display_size;
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

        width = width + (width & 1);

        gx_validation_create_output_file(color_format, width, height);
        gx_validation_create_frame_buffer(width * height * sizeof(USHORT));

        if((canvas->gx_canvas_display->gx_display_rotation_angle != GX_SCREEN_ROTATION_NONE) &&
           (canvas->gx_canvas_display->gx_display_rotation_angle != GX_SCREEN_ROTATION_FLIP))
        {
            /* Create padded memory. */
            rotated_memory = (USHORT *)malloc(canvas->gx_canvas_x_resolution * canvas->gx_canvas_y_resolution * sizeof(USHORT));
        }

        header_created = 1;
    }

    if(!gx_validation_frame_buffer)
    {
        return;
    }

    display_size.gx_rectangle_left = test_parameter.x_start;
    display_size.gx_rectangle_right = test_parameter.x_end;
    display_size.gx_rectangle_top = test_parameter.y_start;
    display_size.gx_rectangle_bottom = test_parameter.y_end;

    /* Copy dirty area to validation frame buffer. */
    if(gx_utility_rectangle_overlap_detect(dirty, &display_size, &overlap))
    {
        int xsrc = overlap.gx_rectangle_left;
        int ysrc = overlap.gx_rectangle_top;
        gx_utility_rectangle_shift(&overlap, canvas -> gx_canvas_display_offset_x, canvas -> gx_canvas_display_offset_y);

        if(gx_utility_rectangle_overlap_detect(&display_size, &overlap, &overlap))
        {
            if(rotated_memory)
            {
                /* Rotate canvas memory to padded memory. */
                _gx_validation_rotate_canvas(canvas);
                memptr = (USHORT *)rotated_memory;
            }
            else
            {
                memptr = (USHORT *)(canvas -> gx_canvas_memory);
            }

#if defined(GX_ENABLE_CANVAS_PARTIAL_FRAME_BUFFER)
            memptr += (ysrc - canvas->gx_canvas_memory_offset_y) * canvas -> gx_canvas_memory_width;
            memptr += (xsrc - canvas->gx_canvas_memory_offset_x);
#else
            memptr += ysrc * canvas -> gx_canvas_x_resolution;
            memptr += xsrc;
#endif

            frame_buffer = (USHORT *)(gx_validation_frame_buffer);
            frame_buffer += (overlap.gx_rectangle_top - test_parameter.y_start) * width;
            frame_buffer += (overlap.gx_rectangle_left - test_parameter.x_start);

            copy_width = overlap.gx_rectangle_right - overlap.gx_rectangle_left + 1;

            for(y = overlap.gx_rectangle_top; y <= overlap.gx_rectangle_bottom; y++)
            {
                memcpy((char *)frame_buffer, (char *)memptr, copy_width * sizeof(USHORT));
                frame_buffer += width;
#if defined(GX_ENABLE_CANVAS_PARTIAL_FRAME_BUFFER)
                memptr += canvas -> gx_canvas_memory_width;
#else
                memptr += canvas -> gx_canvas_x_resolution;
#endif
            }
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
/*Define 565rgb display driver setup function                                     */
/**********************************************************************************/
UINT gx_validation_graphics_driver_setup_565rgb(GX_DISPLAY *display)
{

    /* Initialize the low-level drawing function pointers

       for windows, these are always just the generic functions,
       but for some hardware, these will be customized,
       optimized functions specific to that hardware
     */
    _gx_display_driver_565rgb_setup(display, (VOID*)1, _gx_validation_display_buffer_16bpp_toggle);


    return(GX_SUCCESS);
}

VOID gx_validation_graphics_driver_cleanup_565rgb(GX_DISPLAY *display)
{

    /* Do nothing */
    if(rotated_memory)
    {
        free(rotated_memory);
    }
}

/**********************************************************************************/
/*Define 565rgb display driver setup function                                     */
/**********************************************************************************/
UINT gx_validation_graphics_driver_setup_565rgb_rotated(GX_DISPLAY *display)
{

    /* Initialize the low-level drawing function pointers

       for windows, these are always just the generic functions,
       but for some hardware, these will be customized,
       optimized functions specific to that hardware
     */
    _gx_display_driver_565rgb_rotated_setup(display, (VOID*)1, _gx_validation_display_buffer_16bpp_toggle);


    return(GX_SUCCESS);
}

/**********************************************************************************/
/*Define 565bgr display driver setup function                                     */
/**********************************************************************************/
UINT gx_validation_graphics_driver_setup_565bgr(GX_DISPLAY *display)
{

    /* Initialize the low-level drawing function pointers

       for windows, these are always just the generic functions,
       but for some hardware, these will be customized,
       optimized functions specific to that hardware
     */
    color_format = COLOR_FORMAT_565BGR;
    _gx_display_driver_565rgb_setup(display, (VOID*)1, _gx_validation_display_buffer_16bpp_toggle);


    return(GX_SUCCESS);
}


/**********************************************************************************/
/*Define 4444argb display driver setup function                                   */
/**********************************************************************************/
UINT gx_validation_graphics_driver_setup_4444argb(GX_DISPLAY *display)
{

    /* Initialize the low-level drawing function pointers

       for windows, these are always just the generic functions,
       but for some hardware, these will be customized,
       optimized functions specific to that hardware
     */
    color_format = COLOR_FORMAT_4444ARGB;
    _gx_display_driver_4444argb_setup(display, (VOID*)1, _gx_validation_display_buffer_16bpp_toggle);


    return(GX_SUCCESS);
}

/**********************************************************************************/
/*Define 1555xrgb display driver setup function                                   */
/**********************************************************************************/
UINT gx_validation_graphics_driver_setup_1555xrgb(GX_DISPLAY *display)
{

    /* Initialize the low-level drawing function pointers

       for windows, these are always just the generic functions,
       but for some hardware, these will be customized,
       optimized functions specific to that hardware
     */
    color_format = COLOR_FORMAT_1555XRGB;
    _gx_display_driver_1555xrgb_setup(display, (VOID*)1, _gx_validation_display_buffer_16bpp_toggle);


    return(GX_SUCCESS);
}

/**********************************************************************************/
/*Define 565rgb dave2d simulation display driver setup function                  */
/**********************************************************************************/
UINT gx_validation_dave2d_graphics_driver_setup_565rgb(GX_DISPLAY *display)
{

    /* Initialize the low-level drawing function pointers

       for windows, these are always just the generic functions,
       but for some hardware, these will be customized,
       optimized functions specific to that hardware
     */
    _gx_dave2d_simulation_display_driver_565rgb_setup(display, (VOID*)1, _gx_validation_display_buffer_16bpp_toggle);


    return(GX_SUCCESS);
}

/**********************************************************************************/
/*Define 565rgb dave2d simulation display driver rotated setup function          */
/**********************************************************************************/
UINT gx_validation_dave2d_graphics_driver_setup_565rgb_rotated(GX_DISPLAY *display)
{

    /* Initialize the low-level drawing function pointers

       for windows, these are always just the generic functions,
       but for some hardware, these will be customized,
       optimized functions specific to that hardware
     */
    _gx_dave2d_simulation_display_driver_565rgb_rotated_setup(display, (VOID*)1, _gx_validation_display_buffer_16bpp_toggle);


    return(GX_SUCCESS);
}
