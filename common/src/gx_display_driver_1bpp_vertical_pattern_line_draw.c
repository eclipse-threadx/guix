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
/*    _gx_display_driver_1bpp_vertical_pattern_line_draw  PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Vertical pattern line draw function for 1bpp display driver.        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Drawing context               */
/*    ystart                                y-coord of top endpoint       */
/*    yend                                  y-coord of bottom endpoint    */
/*    xpos                                  x-coord of left edge          */
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
VOID _gx_display_driver_1bpp_vertical_pattern_line_draw(GX_DRAW_CONTEXT *context, INT ystart, INT yend, INT xpos)
{
INT       row;
GX_UBYTE *put;
GX_UBYTE *putrow;
ULONG     pattern;
ULONG     pattern_mask;
GX_UBYTE  on_color;
GX_UBYTE  off_color;
GX_UBYTE  draw_mask;
INT       stride;

    /* pick up row pitch in bytes.  */
    stride = (context -> gx_draw_context_pitch + 7) >> 3;

    /* pick up starting address of canvas memory */
    putrow = (GX_UBYTE *)context -> gx_draw_context_memory;
    putrow += ystart * stride;
    putrow += (xpos >> 3);

    /* pick up the requested pattern and mask */
    pattern = context -> gx_draw_context_brush.gx_brush_line_pattern;
    pattern_mask = context -> gx_draw_context_brush.gx_brush_pattern_mask;

    on_color = (GX_UBYTE)context -> gx_draw_context_brush.gx_brush_line_color & 0x01;
    off_color = (GX_UBYTE)context -> gx_draw_context_brush.gx_brush_fill_color & 0x01;
    draw_mask = (GX_UBYTE)(0x80 >> (xpos & 0x07));

    /* draw line from top to bottom */
    for (row = ystart; row <= yend; row++)
    {
        put = putrow;

        if (pattern & pattern_mask)
        {
            if (on_color == 0x00)
            {
                *put = (GX_UBYTE)((*put) & (~draw_mask));
            }
            else
            {
                *put |= draw_mask;
            }
        }
        else
        {
            if (off_color == 0x00)
            {
                *put = (GX_UBYTE)((*put) & (~draw_mask));
            }
            else
            {
                *put |= draw_mask;
            }
        }

        pattern_mask >>= 1;
        if (!pattern_mask)
        {
            pattern_mask = 0x80000000;
        }

        /* advance to the next scaneline */
        putrow += stride;
    }

    /* save current masks value back to brush */
    context -> gx_draw_context_brush.gx_brush_pattern_mask = pattern_mask;
}

