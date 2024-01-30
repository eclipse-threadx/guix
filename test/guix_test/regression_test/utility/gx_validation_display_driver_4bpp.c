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

extern int gx_validation_record_frame;
extern TEST_PARAM test_parameter;

static int width, height;
static int header_created = 0;

ULONG palette_4bpp_grayscale[16];

VOID _gx_validation_display_buffer_4bpp_grayscale_toggle(GX_CANVAS *canvas, GX_RECTANGLE *dirty)
{

GX_UBYTE *read;
GX_UBYTE *write;
GX_RECTANGLE overlap;
int read_pos;
int write_pos;
GX_UBYTE read_mask = 0xf0;
GX_UBYTE write_mask = 0xf0;
int y;
int x;
GX_UBYTE color;

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
        
        if(width & 0x07)
        {
            width += 8 - (width & 0x07);
        }

        test_parameter.x_end = test_parameter.x_start + width - 1;

        gx_validation_create_output_file("4bpp", width, height);
        gx_validation_create_frame_buffer((width >> 1) * height);

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
        read_pos = overlap.gx_rectangle_top * canvas -> gx_canvas_x_resolution;
        read_pos += overlap.gx_rectangle_left;
        write_pos = (overlap.gx_rectangle_top - test_parameter.y_start) * width;
        write_pos += (overlap.gx_rectangle_left - test_parameter.x_start);

        for(y = overlap.gx_rectangle_top; y <= overlap.gx_rectangle_bottom; y++)
        {
            read = (GX_UBYTE *)canvas -> gx_canvas_memory;
            read += (read_pos >> 1);

            if(read_pos & 0x1)
            {
                read_mask = 0x0f;
            }

            write = (GX_UBYTE *)gx_validation_frame_buffer;
            write += (write_pos >> 1);
            
            if(write_pos & 0x1)
            {
                write_mask = 0x0f;
            }

            for(x = overlap.gx_rectangle_left; x <= overlap.gx_rectangle_right; x++)
            {
                color = (*read) & read_mask;
                *write = (GX_UBYTE)((*write) & (~write_mask));

                if(color)
                {
                    if(read_mask & write_mask)
                    {
                        *write |= color;
                    }
                    else
                    {
                        if(write_mask & 0x0f)
                        {
                            *write |= (GX_UBYTE)(color >> 4);
                        }
                        else
                        {
                            *write |= (GX_UBYTE)(color << 4);
                        }
                    }
                }

                read_mask >>= 4;
                write_mask >>= 4;
    
                if(!read_mask)
                {
                    read++;
                    read_mask = 0xf0;
                }

                if(!write_mask)
                {
                    write++;
                    write_mask = 0xf0;
                }
            }
            read_pos += canvas -> gx_canvas_x_resolution;
            write_pos += width;
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

VOID _gx_validation_graphics_driver_4bpp_grayscale_palette_set()
{
INT i = 0;
INT pixel = 0;

    for(i = 0; i < 16; i++)
    {
        palette_4bpp_grayscale[i] = 0xff000000 | (pixel << 16) | (pixel << 8) | pixel;
        pixel += 0x11;
    }
}

UINT gx_validation_graphics_driver_setup_4bpp_grayscale(GX_DISPLAY *display)
{

    /* Initialize the low-level drawing function pointers

       for windows, these are always just the generic funcions,
       but for some hardware, these will be customized,
       optimized functions specific to that hardware
     */

    _gx_display_driver_4bpp_grayscale_setup(display, (VOID*)1, _gx_validation_display_buffer_4bpp_grayscale_toggle);
    _gx_validation_graphics_driver_4bpp_grayscale_palette_set();

    return(GX_SUCCESS);
}

VOID gx_validation_graphics_driver_cleanup_4bpp_grayscale(GX_DISPLAY *display)
{

    /* Do nothing */

}


