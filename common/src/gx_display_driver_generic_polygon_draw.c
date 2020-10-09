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
#include "gx_canvas.h"
#include "gx_context.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_generic_polygon_draw             PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Generic polygon draw function.                                      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Drawing context               */
/*    vertex                                Array of vertexes             */
/*    num                                   Number of vertexes            */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_canvas_line_draw                  Draw a line into canvas       */
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
VOID _gx_display_driver_generic_polygon_draw(GX_DRAW_CONTEXT *context, GX_POINT *vertex, INT num)
{
INT index;
INT brush_width = context -> gx_draw_context_brush.gx_brush_width;
#if defined (GX_BRUSH_ALPHA_SUPPORT)
GX_UBYTE     temp_alpha;
UINT         brush_style;
    temp_alpha = context -> gx_draw_context_brush.gx_brush_alpha;
    brush_style = context -> gx_draw_context_brush.gx_brush_style;
    if (brush_width > 1)
    {
        if (temp_alpha != 0xff)
        {
            if ((brush_style & GX_BRUSH_ROUND) || (brush_style & GX_BRUSH_ALIAS))
            {
                context -> gx_draw_context_brush.gx_brush_alpha = GX_ALPHA_VALUE_OPAQUE;
            }
        }
    }
#endif

    if (brush_width > 0)
    {
        /*round end*/
        for (index = 0; index < num; index++)
        {
            _gx_canvas_line_draw(vertex[index].gx_point_x, vertex[index].gx_point_y,
                                 vertex[(index + 1) % num].gx_point_x, vertex[(index + 1) % num].gx_point_y);
        }
    }
#if defined (GX_BRUSH_ALPHA_SUPPORT)
    context -> gx_draw_context_brush.gx_brush_alpha = temp_alpha;
#endif
}

