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
#include "gx_system.h"
#include "gx_utility.h"
#include "gx_display.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_1bpp_glyph_1bpp_draw             PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This draws the specified text using the current context,            */
/*      clipped to one viewport                                           */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Draw context                  */
/*    draw_area                             The rectangle where the glyph */
/*                                            is to be drawn.             */
/*    map_offset                            Offset into the glyph         */
/*    glyph                                 Pointer to glyph structure    */
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
/*    _gx_canvas_text_draw                                                */
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

/* Draw area defines where the glyph is drawn.  The draw area is not larger than the glyph.  If it is smaller than
   the glyph, the map_offset defines the how many rows on the top to skip, and how many col on the left to skip. */

VOID _gx_display_driver_1bpp_glyph_1bpp_draw(GX_DRAW_CONTEXT *context, GX_RECTANGLE *draw_area, GX_POINT *map_offset, GX_CONST GX_GLYPH *glyph)
{
INT             xval;
INT             yval;
INT             width;
GX_UBYTE       *putrow;
GX_UBYTE       *getrow;
GX_UBYTE       *put;
GX_CONST GX_UBYTE *get;
GX_UBYTE        putmask;
GX_UBYTE        getmask;
INT             putstride;
INT             getstride;
GX_UBYTE        text_color;

    text_color = (GX_UBYTE)context -> gx_draw_context_brush.gx_brush_line_color;

    putstride = (context -> gx_draw_context_pitch + 7) >> 3;
    getstride = (glyph -> gx_glyph_width + 7) >> 3;

    putrow = (GX_UBYTE *)context -> gx_draw_context_memory;
    putrow += (draw_area -> gx_rectangle_top * putstride);
    putrow += (draw_area -> gx_rectangle_left >> 3);

    getrow = (GX_UBYTE *)(glyph -> gx_glyph_map);
    getrow += (map_offset -> gx_point_y * getstride);
    getrow += (map_offset -> gx_point_x >> 3);

    width = draw_area -> gx_rectangle_right - draw_area -> gx_rectangle_left + 1;

    for (yval = draw_area -> gx_rectangle_top; yval <= draw_area -> gx_rectangle_bottom; yval++)
    {
        put = putrow;
        get = getrow;

        putmask = (GX_UBYTE)(0x80 >> (draw_area -> gx_rectangle_left & 0x07));
        getmask = (GX_UBYTE)(0x80 >> (map_offset -> gx_point_x & 0x07));

        for (xval = 0; xval < width; xval++)
        {
            if (text_color == 0x01)
            {
                if ((*get) & getmask)
                {
                    *put |= putmask;
                }
            }
            else if (text_color == 0x00)
            {
                if ((*get) & getmask)
                {
                    *put = (GX_UBYTE)((*put) & (~putmask));
                }
            }

            getmask >>= 1;
            putmask >>= 1;

            if (!getmask)
            {
                getmask = 0x80;
                get++;
            }

            if (!putmask)
            {
                putmask = 0x80;
                put++;
            }
        }

        putrow += putstride;
        getrow += getstride;
    }
}

