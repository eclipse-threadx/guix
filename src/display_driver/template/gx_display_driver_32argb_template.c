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

#include    <stdio.h>
#include    <string.h>

#include    "gx_api.h"
#include    "gx_display.h"

/* The display handle is used only for routing touch/mouse/keyboard input events.
   The touch driver must use the same handle to route touch screen input 
   to this display. This can be any value, assigned by driver author.
*/
#define DISPLAY_HANDLE                0x12345678

#define DISPLAY_XRES                  800
#define DISPLAY_YRES                  600

ULONG    *frame_buffer_1;
ULONG    *frame_buffer_2;


/**************************************************************************/
/* LCD_hardware_frame_address_set
   Configure hardware to use the indicated frame buffer.
   Wait for the change to take effect.
/**************************************************************************/
static void LCD_hardware_frame_address_set(ULONG *frame_ptr)
{
    /*  Assign hardware frame pointer */
    /* 
    *hardware_register = (ULONG) frame_ptr;
    */

    /* wait for vertical frame interrupt */
    /* which indicates new address is in effect */
    /*
    while (!vertical_refresh_interrupt)
    {
    }
    vertical_refresh_interrupt = FALSE;
    */
}

/**************************************************************************/
/*
  HardwareCacheFlush

    If the graphics frame buffer is in a cached RAM region, the cache must  
    be flushed to the physical memory in order to update the LCD
    correctly.
*/
/**************************************************************************/
static void HardwareCacheFlush(ULONG *start_addr, ULONG size)
{
}


/**************************************************************************/
/*
  buffer_toggle_32argb

    1) Switch hardware frame pointer to point to GUIX canvas memory
    2) Copy modified portion of canvas memory to opposite buffer
    3) Switch GUIX canvas memory to opposite buffer
*/
/**************************************************************************/
static void generic_buffer_toggle_32argb(GX_CANVAS *canvas, GX_RECTANGLE *copy)
{
    GX_RECTANGLE display_size;
    GX_RECTANGLE copy_clip;
    void   *flushaddress;
    ULONG *pPutRow;
    ULONG *pGetRow;
    ULONG *pGet;
    ULONG *pPut;
    int row;
    int col;
    int copy_width;
    int copy_height;

    gx_utility_rectangle_define(&display_size, 0, 0, DISPLAY_XRES - 1, DISPLAY_YRES - 1);
    copy_clip = *copy;

    gx_utility_rectangle_shift(&copy_clip, canvas->gx_canvas_display_offset_x, canvas->gx_canvas_display_offset_y);
    gx_utility_rectangle_overlap_detect(&copy_clip, &display_size, &copy_clip);
    copy_width = (copy_clip.gx_rectangle_right - copy_clip.gx_rectangle_left + 1);
    copy_height = copy_clip.gx_rectangle_bottom - copy_clip.gx_rectangle_top + 1;

    if (copy_width <= 0 || copy_height <= 0)
    {
        return;
    }
    
    // Toggle the hardware frame buffer pointer:
    LCD_hardware_frame_address_set(canvas -> gx_canvas_memory);
    
    // copy modified portion of canvas memory to the second buffer:

    pGetRow = (ULONG *) canvas -> gx_canvas_memory;
    pGetRow += copy->gx_rectangle_top * canvas->gx_canvas_x_resolution;
    pGetRow += copy->gx_rectangle_left;

    if (canvas -> gx_canvas_memory == frame_buffer_1)
    {
        // copy from buffer_1 to buffer_2
        pPutRow = frame_buffer_2;

        // and change canvas memory to buffer_2
        canvas -> gx_canvas_memory = frame_buffer_2;
    }
    else
    {
        // copy from buffer_2 to buffer_1
        pPutRow = frame_buffer_1;

        // and change canvas memory to buffer_1
        canvas -> gx_canvas_memory = frame_buffer_1;
    }

    pPutRow += (canvas ->gx_canvas_display_offset_y + copy->gx_rectangle_top) * DISPLAY_XRES;
    pPutRow += (canvas ->gx_canvas_display_offset_x + copy->gx_rectangle_left);
    flushaddress = pPutRow;

    for (row = 0; row < copy_height; row++)
    {
        pGet = pGetRow;
        pPut = pPutRow;
        
        for (col = 0; col < copy_width; col++)
        {
            *pPut++ = *pGet++;
        }
        pGetRow += canvas->gx_canvas_x_resolution;
        pPutRow += DISPLAY_XRES;
    }
    HardwareCacheFlush(flushaddress, copy_height * DISPLAY_XRES * 4);
}


/**************************************************************************/
/*
  ConfigureDisplayHardware32argb
    1) Do low-level hardware setup
    2) Initialize frame buffer pointer(s)
*/
/**************************************************************************/
static void ConfigureDisplayHardware32argb(GX_DISPLAY *display)
{
    INT buffer_size;

    /* initialize hardware */

    /* initialize frame buffer pointers */
    
    /* frame_buffer_1 =  ??             */
    /* frame_buffer_2 =  ??             */

    /* intialize frame buffers to transparent */
    buffer_size = DISPLAY_XRES * DISPLAY_YRES * 4;
    memset(frame_buffer_1, 0, buffer_size);
    memset(frame_buffer_2, 0, buffer_size);

    /* start with buffer_2 visible, and application will assign buffer_1
       to GUIX canvas memory
    */
    LCD_hardware_frame_address_set(frame_buffer_2);
}

/**************************************************************************/
UINT template_graphics_driver_setup_32argb(GX_DISPLAY *display)
{
    _gx_display_driver_32argb_setup(display, GX_NULL, generic_buffer_toggle_32argb);

    display -> gx_display_handle = DISPLAY_HANDLE;
    ConfigureDisplayHardware32argb(display);
    return GX_SUCCESS;
}
    
