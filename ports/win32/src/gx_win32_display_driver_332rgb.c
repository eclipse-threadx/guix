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
/*    win32_332rgb_bitmap_header_create                   PORTABLE C      */
/*                                                           6.1.3        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function creates bitmap header for 332rgb driver.              */
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
/*    win32_graphics_driver_setup_332rgb                                  */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  12-31-2020     Kenneth Maxwell          Initial Version 6.1.3         */
/*                                                                        */
/**************************************************************************/
static void win32_332rgb_bitmap_header_create(GX_DISPLAY *display)
{
GX_COLOR                      color;
INT                           r = 0;
INT                           g = 0;
INT                           b = 0;
INT                           i = 0;
INT                           red;
INT                           green;
INT                           blue;

GX_WIN32_DISPLAY_DRIVER_DATA *instance = (GX_WIN32_DISPLAY_DRIVER_DATA *)display -> gx_display_driver_data;

    instance -> win32_driver_bmpinfo.gx_bmp_header.biSize = sizeof(BITMAPINFOHEADER);
    instance -> win32_driver_bmpinfo.gx_bmp_header.biWidth = display -> gx_display_width;
    instance -> win32_driver_bmpinfo.gx_bmp_header.biHeight = display -> gx_display_height;

    instance -> win32_driver_bmpinfo.gx_bmp_header.biPlanes = 1;
    instance -> win32_driver_bmpinfo.gx_bmp_header.biBitCount = 8;
    instance -> win32_driver_bmpinfo.gx_bmp_header.biSizeImage = display -> gx_display_width * display -> gx_display_height;
    instance -> win32_driver_bmpinfo.gx_bmp_header.biClrUsed = 256;
    instance -> win32_driver_bmpinfo.gx_bmp_header.biClrImportant = 256;
    instance -> win32_driver_bmpinfo.gx_bmp_header.biCompression = BI_RGB;

    for (r = 0; r < 8; r++)
    {
        red = r << 21;
        if (red & 0x200000)
        {
            red |= 0x1f0000;
        }
        for (g = 0; g < 8; g++)
        {
            green = g << 13;
            if (green & 0x2000)
            {
                green |= 0x1f00;
            }
            for (b = 0; b < 4; b++)
            {
                blue = b << 6;
                if (blue & 0x40)
                {
                    blue |= 0x3f;
                }
                color = red | green | blue;
                instance -> win32_driver_bmpinfo.gx_bmp_colors[i] = color;
                i++;
            }
        }
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    win32_graphics_driver_setup_332rgb                  PORTABLE C      */
/*                                                           6.1.3        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function creates a Windows specific 332rgb display driver.     */
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
/*    _gx_display_driver_332rgb_setup       guix display setup funciton.  */
/*    win32_332rgb_bitmap_header_create     Create bitmap header info     */
/*    gx_win32_get_free_data_instance       Get display data instance     */
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
UINT win32_graphics_driver_setup_332rgb(GX_DISPLAY *display)
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

    /* Save off the format of display driver.*/
    data -> win32_driver_type = GX_COLOR_FORMAT_8BIT_PACKED_PIXEL;

    _gx_display_driver_332rgb_setup(display, data, gx_win32_display_buffer_toggle);

    /* Create the GUIX / Windows event thread
       This thread is a substitute for a touch display
       or keyboard driver thread that would be running
       on embedded hardware. */
    GX_WIN32_EVENT_THREAD_CREATE(data, "GUI-WIN32-332rgb");

    /* Create bitmap header for 332rgb display driver. */
    win32_332rgb_bitmap_header_create(display);

    return(GX_SUCCESS);
}
#endif /* WIN32 */

