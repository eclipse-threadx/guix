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
/**   Display Management (Display)                                        */
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
/*    _gx_display_driver_4bpp_pixel_write                 PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Pixel write function for the 4bpp display driver.                   */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Drawing context               */
/*    x                                     X coordinate                  */
/*    y                                     Y coordinate                  */
/*    color                                 Color of pixel to write       */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_display_driver_4bpp_pixel_write   Calculate row pitch           */
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
VOID _gx_display_driver_4bpp_pixel_write(GX_DRAW_CONTEXT *context, INT x, INT y, GX_COLOR color)
{
GX_UBYTE *put = (GX_UBYTE *)context -> gx_draw_context_memory;
GX_UBYTE  mask;
UINT      stride;
GX_UBYTE  put_color;

    /* Get row pitch in bytes. */
    stride = _gx_display_driver_4bpp_row_pitch_get((USHORT)(context -> gx_draw_context_pitch));
    put_color = color & 0x0f;
    put_color |= (GX_UBYTE)(put_color << 4);

    /* Calculate address of writing byte.  */
    put = (GX_UBYTE *)((INT)put + y * (INT)stride);
    put += x >> 1;

    if (x & 0x01)
    {
        mask = 0x0f;
    }
    else
    {
        mask = 0xf0;
    }

    *put &= (GX_UBYTE)(~mask);
    *put |= mask & put_color;
}

