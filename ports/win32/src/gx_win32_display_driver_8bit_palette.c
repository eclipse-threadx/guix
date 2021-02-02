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
/**   Win32 Display Management (Display)                                  */
/**                                                                       */
/**************************************************************************/
#ifdef WIN32
#include "tx_api.h"
#include "gx_api.h"
#include "gx_system.h"
#include "gx_display.h"
#include "windows.h"
#include "gx_win32_display_driver.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    win32_display_driver_8bit_palette_set               PORTABLE C      */
/*                                                           6.1.4        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function set bitmap palette for 8bpp.                          */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    display                               Pointer to GX_DISPLAY         */
/*    palette                               Pointer to palette table      */
/*    count                                 Palette table count           */
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
/*    win32_graphics_driver_setup_8bit_palette                            */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  12-31-2020     Kenneth Maxwell          Initial Version 6.1.3         */
/*  02-02-2021     Kenneth Maxwell          Modified comment(s),          */
/*                                            made the function public,   */
/*                                            resulting in version 6.1.4  */
/*                                                                        */
/**************************************************************************/
VOID win32_display_driver_8bit_palette_set(GX_DISPLAY *display, GX_COLOR *palette, INT count)
{
GX_COLOR                      color;
int                           loop;
GX_WIN32_DISPLAY_DRIVER_DATA *instance;

    display -> gx_display_palette = palette;
    display -> gx_display_palette_size = count;

    instance = (GX_WIN32_DISPLAY_DRIVER_DATA *)display -> gx_display_driver_data;
    instance -> win32_driver_bmpinfo.gx_bmp_header.biClrUsed = count;
    instance -> win32_driver_bmpinfo.gx_bmp_header.biClrImportant = count;

    for (loop = 0; loop < count; loop++)
    {
        color = *palette++;
        instance -> win32_driver_bmpinfo.gx_bmp_colors[loop] = color;
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    win32_8bit_palette_bitmap_header_create             PORTABLE C      */
/*                                                           6.1.3        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function creates bitmap header for 8bpp driver.                */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    display                               Pointer to GX_DISPLAY         */
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
/*    win32_graphics_driver_setup_8bit_palette                            */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  12-31-2020     Kenneth Maxwell          Initial Version 6.1.3         */
/*                                                                        */
/**************************************************************************/
VOID win32_8bit_palette_bitmap_header_create(GX_DISPLAY *display)
{

GX_WIN32_DISPLAY_DRIVER_DATA *instance = (GX_WIN32_DISPLAY_DRIVER_DATA *)display -> gx_display_driver_data;

    instance -> win32_driver_bmpinfo.gx_bmp_header.biSize = sizeof(BITMAPINFOHEADER);
    instance -> win32_driver_bmpinfo.gx_bmp_header.biWidth = display -> gx_display_width;
    instance -> win32_driver_bmpinfo.gx_bmp_header.biHeight = display -> gx_display_height;

    instance -> win32_driver_bmpinfo.gx_bmp_header.biPlanes = 1;
    instance -> win32_driver_bmpinfo.gx_bmp_header.biBitCount = 8;
    instance -> win32_driver_bmpinfo.gx_bmp_header.biSizeImage = display -> gx_display_width * display -> gx_display_height;
    instance -> win32_driver_bmpinfo.gx_bmp_header.biClrUsed = 0;
    instance -> win32_driver_bmpinfo.gx_bmp_header.biClrImportant = 0;
    instance -> win32_driver_bmpinfo.gx_bmp_header.biCompression = BI_RGB;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    win32_graphics_driver_setup_8bit_palette            PORTABLE C      */
/*                                                           6.1.3        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function creates a Windows specific 8bpp display driver.       */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    display                               Pointer to GX_DISPLAY         */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_display_driver_8bit_palette_setup guix display setup funciton.  */
/*    win32_8bit_palette_bitmap_header_create                             */
/*                                          Create bitmap header info     */
/*    gx_win32_get_free_data_instance       Get display data instance     */
/*    [gx_display_driver_palette_set]       Set palette table for 8bpp    */
/*    GX_WIN32_EVENT_THREAD_CREATE          Create event thread           */
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
UINT win32_graphics_driver_setup_8bit_palette(GX_DISPLAY *display)
{
GX_WIN32_DISPLAY_DRIVER_DATA *data;

    /* Initialize the low-level drawing function pointers.

       For windows, these are always just the generic funcions,
       but for some hardware, these will be customized,
       optimized functions specific to that hardware. */

    data = gx_win32_get_free_data_instance();
    if (!data)
    {
        /* We don't have any free display data instance. */
        return(GX_FAILURE);
    }

    /* Save off the format of display driver. */
    data -> win32_driver_type = GX_COLOR_FORMAT_8BIT_PALETTE;

    _gx_display_driver_8bit_palette_setup(display, data, gx_win32_display_buffer_toggle);
    display -> gx_display_driver_palette_set = win32_display_driver_8bit_palette_set;

    /* Create the GUIX / Windows event thread
       This thread is a substitute for a touch display
       or keyboard driver thread that would be running
       on embedded hardware. */
    GX_WIN32_EVENT_THREAD_CREATE(data, "GUI-WIN32-8bit-palette");

    /* Create bitmap header for 8bit palette driver. */
    win32_8bit_palette_bitmap_header_create(display);

    return(GX_SUCCESS);
}
#endif /* WIN32 */

