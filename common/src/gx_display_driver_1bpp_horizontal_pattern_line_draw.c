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
/*    _gx_display_driver_1bpp_horizontal_pattern_line_draw                */
/*                                                        PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Horizontal pattern line draw function for 1bpp display driver.      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Drawing context               */
/*    xstart                                x-coord of left endpoint      */
/*    xend                                  x-coord of right endpoint     */
/*    ypos                                  y-coord of line top           */
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
VOID _gx_display_driver_1bpp_horizontal_pattern_line_draw(GX_DRAW_CONTEXT *context, INT xstart, INT xend, INT ypos)
{
INT       column;
GX_UBYTE *put;
GX_UBYTE *rowstart;
ULONG     pattern;
ULONG     mask;
GX_UBYTE  on_color;
GX_UBYTE  off_color;
INT       pos;
INT       len = xend - xstart + 1;
INT       start_in_byte = 0;
GX_UBYTE  start_mask;

    /* pick up start address of canvas memory */
    rowstart = (GX_UBYTE *)context -> gx_draw_context_memory;

    pos = context -> gx_draw_context_pitch * ypos + xstart;

    /* pick up the requested pattern and mask */
    pattern = context -> gx_draw_context_brush.gx_brush_line_pattern;
    mask = context -> gx_draw_context_brush.gx_brush_pattern_mask;
    on_color = (GX_UBYTE)context -> gx_draw_context_brush.gx_brush_line_color & 0x01;
    off_color = (GX_UBYTE)context -> gx_draw_context_brush.gx_brush_fill_color & 0x01;

    put = rowstart;
    put +=  pos >> 3;
    start_in_byte = pos & 0x07;
    start_mask = (GX_UBYTE)(((GX_UBYTE)0x80) >> start_in_byte);

    for (column = 0; column < len; column++)
    {
        if (pattern & mask)
        {
            if (on_color == 0x00)
            {
                *put = (GX_UBYTE)((*put) & (~start_mask));
            }
            else
            {
                *put |= start_mask;
            }
        }
        else
        {
            if (off_color == 0x00)
            {
                *put &= (GX_UBYTE)((*put) & (~start_mask));
            }
            else
            {
                *put |= start_mask;
            }
        }
        start_mask >>= 1;
        if (start_mask == 0)
        {
            put++;
            start_mask = 0x80;
        }

        mask >>= 1;
        if (!mask)
        {
            mask = 0x80000000;
        }
    }

    /* save current masks value back to brush */
    context -> gx_draw_context_brush.gx_brush_pattern_mask = mask;
}

