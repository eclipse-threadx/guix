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
#include "gx_utility.h"

#define REDVAL(_c)   (GX_UBYTE)(((_c) >> 11) & 0x1f)
#define GREENVAL(_c) (GX_UBYTE)(((_c) >> 5) & 0x3f)
#define BLUEVAL(_c)  (GX_UBYTE)(((_c)) & 0x1f)


/* Define macros for assembling a 16-bit r:g:b value from 3 components.  */

#define ASSEMBLECOLOR(_r, _g, _b) \
    ((((_r) & 0x1f) << 11) |      \
     (((_g) & 0x3f) << 5) |       \
     (((_b) & 0x1f)))


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_565rgb_rotated_canvas_blend      PORTABLE C      */
/*                                                           6.1.3        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Rotated canvas blend function for 565rgb color format.              */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*   canvas                                 The canvas to blend to        */
/*   composite                              The canvas to blend from      */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_rectangle_shift           Adjust the rectangle          */
/*    _gx_utility_recttangle_overlap_detect Detect whether two areas      */
/*                                            overlap                     */
/*    REDVAL                                Extrace Red from canvas       */
/*    GREENVAL                              Extrace Green from canvas     */
/*    BLUEVAL                               Extrace Blue from canvas      */
/*    ASSEMBLECOLOR                         Compose the RGB color         */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    GUIX Internal Code                                                  */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  12-31-2020     Kenneth Maxwell          Initial Version 6.1.3         */
/*                                                                        */
/**************************************************************************/
VOID _gx_display_driver_565rgb_rotated_canvas_blend(GX_CANVAS *canvas, GX_CANVAS *composite)
{
GX_RECTANGLE dirty;
GX_RECTANGLE overlap;
USHORT      *read;
USHORT      *read_start;
USHORT      *write;
USHORT      *write_start;
USHORT       fcolor;
GX_UBYTE     fred, fgreen, fblue;
GX_UBYTE     bred, bgreen, bblue;
GX_UBYTE     alpha, balpha;

USHORT       bcolor;
INT          row;
INT          col;

    dirty.gx_rectangle_left = dirty.gx_rectangle_top = 0;
    dirty.gx_rectangle_right = (GX_VALUE)(canvas -> gx_canvas_x_resolution - 1);
    dirty.gx_rectangle_bottom = (GX_VALUE)(canvas -> gx_canvas_y_resolution - 1);

    _gx_utility_rectangle_shift(&dirty, canvas -> gx_canvas_display_offset_x, canvas -> gx_canvas_display_offset_y);

    if (_gx_utility_rectangle_overlap_detect(&dirty, &composite -> gx_canvas_dirty_area, &overlap))
    {
        alpha = canvas -> gx_canvas_alpha;
        balpha = (GX_UBYTE)(256 - alpha);

        read_start = (USHORT *)canvas -> gx_canvas_memory;
        write_start = (USHORT *)composite -> gx_canvas_memory;

        if (canvas -> gx_canvas_display -> gx_display_rotation_angle == GX_SCREEN_ROTATION_CW)
        {
            /* index into starting row */
            read_start += (dirty.gx_rectangle_right - overlap.gx_rectangle_right) * canvas -> gx_canvas_y_resolution;

            /* index into pixel */
            read_start += overlap.gx_rectangle_top - dirty.gx_rectangle_top;

            /* calculate the write pointer */
            write_start += (composite -> gx_canvas_x_resolution - overlap.gx_rectangle_right - 1) * composite -> gx_canvas_y_resolution;
            write_start += overlap.gx_rectangle_top;
        }
        else
        {
            /* index into starting row */
            read_start += (overlap.gx_rectangle_left - dirty.gx_rectangle_left) * canvas -> gx_canvas_y_resolution;

            /* index into pixel */
            read_start += dirty.gx_rectangle_bottom - overlap.gx_rectangle_bottom;

            /* calculate the write pointer */
            write_start += overlap.gx_rectangle_left * composite -> gx_canvas_y_resolution;
            write_start += (composite -> gx_canvas_y_resolution - overlap.gx_rectangle_bottom - 1);
        }

        for (row = overlap.gx_rectangle_left; row <= overlap.gx_rectangle_right; row++)
        {
            read = read_start;
            write = write_start;

            for (col = overlap.gx_rectangle_top; col <= overlap.gx_rectangle_bottom; col++)
            {
                /* read the foreground color */
                fcolor = *read++;

                /* split foreground into red, green, and blue components */
                fred = REDVAL(fcolor);
                fgreen = GREENVAL(fcolor);
                fblue = BLUEVAL(fcolor);

                /* read background color */
                bcolor = *write;

                /* split background color into red, green, and blue components */
                bred = REDVAL(bcolor);
                bgreen = GREENVAL(bcolor);
                bblue = BLUEVAL(bcolor);

                /* blend foreground and background, each color channel */
                fred = (GX_UBYTE)(((bred * balpha) + (fred * alpha)) >> 8);
                fgreen = (GX_UBYTE)(((bgreen * balpha) + (fgreen * alpha)) >> 8);
                fblue = (GX_UBYTE)(((bblue * balpha) + (fblue * alpha)) >> 8);

                /* re-assemble into 16-bit color and write it out */
                *write++ = (USHORT)ASSEMBLECOLOR(fred, fgreen, fblue);
            }
            write_start += composite -> gx_canvas_y_resolution;
            read_start += canvas -> gx_canvas_y_resolution;
        }
    }
}

