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
/*    _gx_display_driver_4bpp_horizontal_line_draw        PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Horizontal line draw function for 4bpp display driver.              */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Drawing context               */
/*    xstart                                x-coord of left endpoint      */
/*    xend                                  x-coord of right endpoint     */
/*    ypos                                  y-coord of line top           */
/*    width                                 Width (height) of the line    */
/*    color                                 Color of line to write        */
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
VOID _gx_display_driver_4bpp_horizontal_line_draw(GX_DRAW_CONTEXT *context, INT xstart, INT xend, INT ypos, INT width, GX_COLOR color)
{
INT       row;
INT       column;
GX_UBYTE *put;
GX_UBYTE *putrow;
GX_UBYTE  mask;
GX_UBYTE  pixel;
INT       stride;

    /* Get row pitch in bytes.  */
    stride = (context -> gx_draw_context_pitch + 1) >> 1;
    pixel = (GX_UBYTE)(color & 0x0f);
    pixel |= (GX_UBYTE)(pixel << 4);

    /* pick up start address of canvas memory */
    putrow = (GX_UBYTE *)context -> gx_draw_context_memory;
    putrow += ypos * stride;
    putrow += (xstart >> 1);

    for (row = 0; row < width; row++)
    {
        put = putrow;

        if (xstart & 0x01)
        {
            mask = 0x0f;
        }
        else
        {
            mask = 0xf0;
        }

        column = xstart;
        while (column <= xend)
        {
            if ((mask == 0xf0) && (xend - column) > 2)
            {
                while ((xend - column) > 2)
                {
                    *put++ = pixel;
                    column += 2;
                }
            }
            else
            {
                /*Set bits first.*/
                *put &= (GX_UBYTE)(~mask);
                *put |= (GX_UBYTE)(pixel & mask);
                mask >>= 4;

                if (mask == 0)
                {
                    mask = 0xf0;
                    put++;
                }
                column++;
            }
        }
        putrow += stride;
    }
}

