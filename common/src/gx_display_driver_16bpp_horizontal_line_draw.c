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
/*    _gx_display_driver_16bpp_horizontal_line_draw       PORTABLE C      */
/*                                                           6.3.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Generic 16bpp horizontal line draw function.                        */
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
/*    NOne                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_display_driver_horizontal_line_alpha_draw                       */
/*                                          Display driver basic          */
/*                                            horiztonal alpha line draw  */
/*                                            route                       */
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
/*  10-31-2023     Ting Zhu                 Modified comment(s),          */
/*                                            added partial canvas buffer */
/*                                            support,                    */
/*                                            resulting in version 6.3.0  */
/*                                                                        */
/**************************************************************************/
VOID _gx_display_driver_16bpp_horizontal_line_draw(GX_DRAW_CONTEXT *context, INT xstart, INT xend, INT ypos, INT width, GX_COLOR color)
{
INT     row;
INT     column;
USHORT *put;
USHORT *rowstart;
INT     len = xend - xstart + 1;

#if defined GX_BRUSH_ALPHA_SUPPORT  
GX_UBYTE alpha;

    alpha = context -> gx_draw_context_brush.gx_brush_alpha;
    if (alpha == 0)
    {
        /* Nothing to drawn. Just return. */
        return;
    }

    if (alpha != 0xff)
    {
        _gx_display_driver_horizontal_line_alpha_draw(context, xstart, xend, ypos, width, color, alpha);
        return;
    }
#endif

    /* pick up start address of canvas memory */
    rowstart = (USHORT *)context -> gx_draw_context_memory;

    GX_CALCULATE_PUTROW(rowstart, xstart, ypos, context);

    /* draw 1-pixel hi lines to fill width */
    for (row = 0; row < width; row++)
    {
        put = rowstart;

        /* draw one line, left to right */
        for (column = 0; column < len; column++)
        {
            *put++ = (USHORT)color;
        }
        rowstart += context -> gx_draw_context_pitch;
    }

}
