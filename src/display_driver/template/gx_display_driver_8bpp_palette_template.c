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
  buffer_toggle_8bpp

    1) Switch hardware frame pointer to point to GUIX canvas memory
    2) Copy modified portion of canvas memory to opposite buffer
    3) Switch GUIX canvas memory to opposite buffer

*** NOTE ***
    The display and canvas X-resolution must be an even modulo-4 value,
    because this function copies ULONGs to improve the speed of the buffer
    toggle operation.
*/
/**************************************************************************/
static void generic_buffer_toggle_8bpp(GX_CANVAS *canvas, GX_RECTANGLE *copy)
{
    GX_RECTANGLE display_size;
    GX_RECTANGLE copy_aligned;
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
    copy_aligned = *copy;

    // align the copy region on a 4-byte boundry so that we can copy ULONGs
    copy_aligned.gx_rectangle_left &= 0xfffc;
    copy_aligned.gx_rectangle_right |= 0x0003;

    gx_utility_rectangle_overlap_detect(&copy_aligned, &display_size, &copy_aligned);
    copy_width = (copy_aligned.gx_rectangle_right - copy_aligned.gx_rectangle_left + 1);
    copy_height = copy_aligned.gx_rectangle_bottom - copy_aligned.gx_rectangle_top + 1;

    if (copy_width <= 0 ||
        copy_height <= 0)
    {
        return;
    }

    // Toggle the hardware frame buffer pointer:
    LCD_hardware_frame_address_set(canvas -> gx_canvas_memory);

       
    // copy modified portion of canvas memory to the second buffer:

    copy_width /= 4;            // copying ULONGs
    pGetRow = (ULONG *) canvas -> gx_canvas_memory;
    pGetRow += copy_aligned.gx_rectangle_top * (canvas->gx_canvas_x_resolution / 4);
    pGetRow += copy_aligned.gx_rectangle_left / 4;

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

    pPutRow += (canvas ->gx_canvas_display_offset_y + copy_aligned.gx_rectangle_top) * DISPLAY_XRES / 4;
    pPutRow += (canvas ->gx_canvas_display_offset_x + copy_aligned.gx_rectangle_left) / 4;
    flushaddress = pPutRow;

    for (row = 0; row < copy_height; row++)
    {
        pGet = pGetRow;
        pPut = pPutRow;
        
        for (col = 0; col < copy_width; col++)
        {
            *pPut++ = *pGet++;
        }
        pGetRow += canvas->gx_canvas_x_resolution / 4;
        pPutRow += DISPLAY_XRES / 4;
    }
    HardwareCacheFlush(flushaddress, copy_height * DISPLAY_XRES);
}


/**************************************************************************/
/*
  ConfigureDisplayHardware8bitPalette
    1) Do low-level hardware setup
    2) Initialize frame buffer pointer(s)
*/
/**************************************************************************/
static void ConfigureDisplayHardware8BitPalette(GX_DISPLAY *display)
{
    INT buffer_size;

    /* initialize hardware */

    /* initialize frame buffer pointers */
    
    /* frame_buffer_1 =  ??             */
    /* frame_buffer_2 =  ??             */

    /* intialize frame buffers to black */
    buffer_size = DISPLAY_XRES * DISPLAY_YRES;
    memset(frame_buffer_1, 0, buffer_size);
    memset(frame_buffer_2, 0, buffer_size);

    /* start with buffer_2 visible, and application will assign buffer_1
       to GUIX canvas memory
    */
    LCD_hardware_frame_address_set(frame_buffer_2);
}

/**************************************************************************/
/*
  template_display_driver_8bit_palette_set
    1) Function called by Studio or by the application to install the
       color palette. This palette is usually defined by Studio and written
       to the application resource file.
*/
/**************************************************************************/
static void template_display_driver_8bit_palette_set(GX_DISPLAY *display,
                                                GX_COLOR *palette, INT count)
{
GX_COLOR                      color;
int                           loop;

    // save palette pointer and palette size
    display -> gx_display_palette = palette;
    display -> gx_display_palette_size = count;

    for (loop = 0; loop < count; loop++)
    {
        color = *palette++;

        /* Write the color to the hardware palette register */
        /* hardware_reg[loop] = color;                      */
    }
}

/**************************************************************************/
UINT template_graphics_driver_setup_8bpp_palette(GX_DISPLAY *display)
{
    /* install all of the generic palette mode drawing functions */
    _gx_display_driver_8bit_palette_setup(display, GX_NULL, generic_buffer_toggle_8bpp);
    
    display -> gx_display_handle = DISPLAY_HANDLE;
    ConfigureDisplayHardware8BitPalette(display);

    /* install our palette definition function, which is hardware specific: */
    display -> gx_display_driver_palette_set = template_display_driver_8bit_palette_set;

    return GX_SUCCESS;
}
    
