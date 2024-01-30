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


VOID _gx_validation_display_buffer_monochrome_toggle(GX_CANVAS *canvas, GX_RECTANGLE *dirty)
{

    GX_UBYTE *read;
    GX_UBYTE *write;
    GX_RECTANGLE overlap;
    int read_pos;
    int write_pos;
    GX_UBYTE read_mask;
    GX_UBYTE write_mask;
    int y;
    int x;

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
        
        if(width & 0x1f)
        {
            width += 32 - (width & 0x1f);
        }

        gx_validation_create_output_file("1bpp", width, height);
        gx_validation_create_frame_buffer((width >> 3) * height);

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
            read += (read_pos >> 3);
            read_mask = (GX_UBYTE)(0x80 >> (read_pos & 0x7));

            write = (GX_UBYTE *)gx_validation_frame_buffer;
            write += (write_pos >> 3);
            write_mask = (GX_UBYTE)(0x80 >> (write_pos & 0x07));

            for(x = overlap.gx_rectangle_left; x <= overlap.gx_rectangle_right; x++)
            {
                if((*read) & read_mask)
                {
                    *write |= write_mask;
                }
                else
                {
                    *write = (GX_UBYTE)((*write)&(~write_mask));
                }
                read_mask >>= 1;
                write_mask >>= 1;
    
                if(!read_mask)
                {
                    read++;
                    read_mask = 0x80;
                }

                if(!write_mask)
                {
                    write++;
                    write_mask = 0x80;
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



UINT gx_validation_graphics_driver_setup_monochrome(GX_DISPLAY *display)
{

    /* Initialize the low-level drawing function pointers

       for windows, these are always just the generic funcions,
       but for some hardware, these will be customized,
       optimized functions specific to that hardware
     */

    _gx_display_driver_monochrome_setup(display, (VOID*)1, _gx_validation_display_buffer_monochrome_toggle);


    return(GX_SUCCESS);
}

VOID gx_validation_graphics_driver_cleanup_1bpp(GX_DISPLAY *display)
{

    /* Do nothing */

}


