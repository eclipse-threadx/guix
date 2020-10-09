
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
/**   Dispaly Management (Dispaly)                                        */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_display.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_4bpp_mouse_restore               PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service restores captured memory under the mouse area.         */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    display                               Display control block         */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    GUIX Internal Code                                                  */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
#if defined(GX_MOUSE_SUPPORT)
#if !defined(GX_HARDWARE_MOUSE_SUPPORT)
VOID _gx_display_driver_4bpp_mouse_restore(GX_DISPLAY *display)
{
INT         row;
INT         column;
INT         height;
INT         width;
GX_UBYTE   *putrow;
GX_UBYTE   *put;
GX_UBYTE   *get;
GX_UBYTE   *getrow;
GX_UBYTE    getmask;
GX_UBYTE    putmask;
GX_UBYTE    pixel;
GX_CANVAS  *canvas;

    if (display -> gx_display_mouse.gx_mouse_cursor_info)
    {
        if (display -> gx_display_mouse.gx_mouse_capture_memory &&
            (display -> gx_display_mouse.gx_mouse_status & GX_MOUSE_VISIBLE))
        {
            canvas = display -> gx_display_mouse.gx_mouse_canvas;
            height = display -> gx_display_mouse.gx_mouse_rect.gx_rectangle_bottom - display -> gx_display_mouse.gx_mouse_rect.gx_rectangle_top + 1;
            width = display -> gx_display_mouse.gx_mouse_rect.gx_rectangle_right - display -> gx_display_mouse.gx_mouse_rect.gx_rectangle_left + 1;

            if (width > 0 && height > 0)
            {
                getrow = (GX_UBYTE *)display -> gx_display_mouse.gx_mouse_capture_memory;
                putrow = (GX_UBYTE *)canvas -> gx_canvas_memory;
                putrow += ((canvas -> gx_canvas_x_resolution + 1) >> 1) * display -> gx_display_mouse.gx_mouse_rect.gx_rectangle_top;
                putrow += display -> gx_display_mouse.gx_mouse_rect.gx_rectangle_left >> 1;

                for (row = 0; row < height; row++)
                {
                    getmask = 0xf0;
                    if (display -> gx_display_mouse.gx_mouse_rect.gx_rectangle_left & 1)
                    {
                        putmask = 0x0f;
                    }
                    else
                    {
                        putmask = 0xf0;
                    }
                    put = putrow;
                    get = getrow;
                    for (column = 0; column < width; column++)
                    {
                        pixel = (*get) & getmask;
                        if (getmask == 0x0f)
                        {
                            get++;
                            getmask = 0xf0;
                            pixel |= (GX_UBYTE)(pixel << 4);
                        }
                        else
                        {
                            getmask = 0x0f;
                            pixel |= pixel >> 4;
                        }

                        *put &= (GX_UBYTE)(~putmask);
                        *put |= pixel & putmask;
                        putmask >>= 4;
                        if (putmask == 0)
                        {
                            putmask = 0xf0;
                            put++;
                        }
                    }
                    putrow += (display -> gx_display_width + 1) >> 1;
                    getrow += (width + 1) >> 1;
                }
            }
        }
        display -> gx_display_mouse.gx_mouse_status &= (GX_UBYTE)(~GX_MOUSE_VISIBLE);
    }
}
#endif
#endif

