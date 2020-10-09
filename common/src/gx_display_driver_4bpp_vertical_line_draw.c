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
/*    _gx_display_driver_4bpp_vertical_line_draw          PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Vertical line draw function for 4bpp display driver.                */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Drawing context               */
/*    ystart                                y-coord of top endpoint       */
/*    yend                                  y-coord of bottom endpoint    */
/*    xpos                                  x-coord of left edge          */
/*    width                                 width of the line             */
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
VOID _gx_display_driver_4bpp_vertical_line_draw(GX_DRAW_CONTEXT *context, INT ystart, INT yend, INT xpos, INT width, GX_COLOR color)
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

    /* pick up starting address of canvas memory */
    putrow =  (GX_UBYTE *)context -> gx_draw_context_memory;
    putrow += ystart * stride;
    putrow += (xpos >> 1);

    /* draw line from top to bottom */
    for (row = ystart; row <= yend; row++)
    {
        put = putrow;

        if (xpos & 0x01)
        {
            mask = 0x0f;
        }
        else
        {
            mask = 0xf0;
        }

        /* draw line width from left to right */
        for (column = 0; column < width;)
        {
            if ((mask == 0xf0) && (width - column >= 2))
            {
                *put++ = pixel;
                column += 2;
            }
            else
            {
                /* Set bits first */
                *put &= (GX_UBYTE)(~mask);
                *put |= (pixel & mask);
                mask >>= 4;

                if (mask == 0)
                {
                    put++;
                    mask = 0xf0;
                }
                column++;
            }
        }

        /* advance to the next scaneline */
        putrow +=  stride;
    }
}

