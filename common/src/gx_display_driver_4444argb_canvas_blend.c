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

#define ALPHAVAL(_c) (GX_UBYTE)(((_c) >> 12) & 0xf)
#define REDVAL(_c)   (GX_UBYTE)(((_c) >> 8) & 0xf)
#define GREENVAL(_c) (GX_UBYTE)(((_c) >> 4) & 0xf)
#define BLUEVAL(_c)  (GX_UBYTE)((_c) & 0xf)

#define ASSEMBLECOLOR(_a, _r, _g, _b) \
    ((((_a) & 0xf) << 12)   |         \
     (((_r) & 0xf) << 8)   |          \
     (((_g) & 0xf) << 4)    |         \
     ((_b) & 0xf))


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_4444argb_canvas_blend            PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Canvas blend function for 4444argb color foramt.                    */
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
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
VOID _gx_display_driver_4444argb_canvas_blend(GX_CANVAS *canvas, GX_CANVAS *composite)
{
GX_RECTANGLE dirty;
GX_RECTANGLE overlap;
USHORT      *read;
USHORT      *read_start;
USHORT      *write;
USHORT      *write_start;
USHORT       fcolor;
GX_UBYTE     falpha;
GX_UBYTE     fred;
GX_UBYTE     fgreen;
GX_UBYTE     fblue;
GX_UBYTE     balpha;
GX_UBYTE     bred;
GX_UBYTE     bgreen;
GX_UBYTE     bblue;
GX_UBYTE     inv_alpha;
GX_UBYTE     combined_alpha;
USHORT       bcolor;
INT          row;
INT          col;

    dirty.gx_rectangle_left = dirty.gx_rectangle_top = 0;
    dirty.gx_rectangle_right = (GX_VALUE)(canvas -> gx_canvas_x_resolution - 1);
    dirty.gx_rectangle_bottom = (GX_VALUE)(canvas -> gx_canvas_y_resolution - 1);

    _gx_utility_rectangle_shift(&dirty, canvas -> gx_canvas_display_offset_x, canvas -> gx_canvas_display_offset_y);

    if (_gx_utility_rectangle_overlap_detect(&dirty, &composite -> gx_canvas_dirty_area, &overlap))
    {

        read_start = (USHORT *)canvas -> gx_canvas_memory;

        /* index into starting row */
        read_start += (overlap.gx_rectangle_top - dirty.gx_rectangle_top) * canvas -> gx_canvas_x_resolution;

        /* index into pixel */

        read_start += overlap.gx_rectangle_left - dirty.gx_rectangle_left;

        /* calculate the write pointer */
        write_start = (USHORT *)composite -> gx_canvas_memory;
        write_start += overlap.gx_rectangle_top * composite -> gx_canvas_x_resolution;
        write_start += overlap.gx_rectangle_left;

        for (row = overlap.gx_rectangle_top; row <= overlap.gx_rectangle_bottom; row++)
        {
            read = read_start;
            write = write_start;

            for (col = overlap.gx_rectangle_left; col <= overlap.gx_rectangle_right; col++)
            {
                /* read the foreground color */
                fcolor = *read++;

                /* split foreground into red, green, and blue components */
                falpha = ALPHAVAL(fcolor);
                falpha = (GX_UBYTE)(falpha | (falpha << 4));
                fred = REDVAL(fcolor);
                fgreen = GREENVAL(fcolor);
                fblue = BLUEVAL(fcolor);

                combined_alpha = (GX_UBYTE)((falpha * canvas -> gx_canvas_alpha) >> 8);

                /* read background color */
                bcolor = *write;

                /* split background color into alpha, red, green, and blue components */
                balpha = ALPHAVAL(bcolor);
                balpha = (GX_UBYTE)(balpha | (balpha << 4));
                bred = REDVAL(bcolor);
                bgreen = GREENVAL(bcolor);
                bblue = BLUEVAL(bcolor);

                /* background alpha is inverse of foreground alpha */
                inv_alpha = (GX_UBYTE)(256 - combined_alpha);

                /* blend foreground and background, each color channel */
                falpha = (GX_UBYTE)(((balpha * inv_alpha) + (falpha * combined_alpha)) >> 8);
                fred = (GX_UBYTE)(((bred * inv_alpha) + (fred * combined_alpha)) >> 8);
                fgreen = (GX_UBYTE)(((bgreen * inv_alpha) + (fgreen * combined_alpha)) >> 8);
                fblue = (GX_UBYTE)(((bblue * inv_alpha) + (fblue * combined_alpha)) >> 8);

                /* re-assemble into 16-bit color and write it out */
                *write++ = (USHORT)ASSEMBLECOLOR(falpha, fred, fgreen, fblue);
            }
            write_start += composite -> gx_canvas_x_resolution;
            read_start += canvas -> gx_canvas_x_resolution;
        }
    }
}

