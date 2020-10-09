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
/*    _gx_display_driver_1bpp_horizontal_line_draw        PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Horizontal line draw function for 1bpp display driver.              */
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
VOID _gx_display_driver_1bpp_horizontal_line_draw(GX_DRAW_CONTEXT *context, INT xstart, INT xend, INT ypos, INT width, GX_COLOR color)
{
INT       row;
INT       column;
GX_UBYTE *put;
GX_UBYTE *putrow;
GX_UBYTE  mask;
INT       stride;

    /* Get row pitch in bytes.  */
    stride = (context -> gx_draw_context_pitch + 7) >> 3;

    /* pick up start address of canvas memory */
    putrow = (GX_UBYTE *)context -> gx_draw_context_memory;
    putrow += ypos * stride;
    putrow += (xstart >> 3);

    for (row = 0; row < width; row++)
    {
        put = putrow;

        mask = (GX_UBYTE)(0x80 >> (xstart & 0x07));

        column = xstart;
        while (column <= xend)
        {
            if ((mask == 0x80) && (xend - column + 1 >= 8))
            {
                while (xend - column + 1 >= 8)
                {
                    if (color == 0x00)
                    {
                        *put++ = 0x00;
                    }
                    else if (color == 0x01)
                    {
                        *put++ = 0xff;
                    }

                    column += 8;
                }
            }
            else
            {
                if (color == 0x00)
                {
                    *put = (GX_UBYTE)((*put) & (~mask));
                }
                else if (color == 0x01)
                {
                    *put |= mask;
                }

                mask >>= 1;

                if (mask == 0)
                {
                    put++;
                    mask = 0x80;
                }

                column++;
            }
        }

        putrow += stride;
    }
}

