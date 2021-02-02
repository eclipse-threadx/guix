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

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_24xrgb_rotated_canvas_blend      PORTABLE C      */
/*                                                           6.1.4        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Rotated canvas blend function for 24xrgb color format.              */
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
/*    REDVAL_24BPP                          Extrace Red from canvas       */
/*    GREENVAL_24BPP                        Extrace Green from canvas     */
/*    BLUEVAL_24BPP                         Extrace Blue from canvas      */
/*    ASSEMBLECOLOR_24BPP                   Compose the RGB color         */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    GUIX Internal Code                                                  */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  02-02-2021     Kenneth Maxwell          Initial Version 6.1.4         */
/*                                                                        */
/**************************************************************************/
VOID _gx_display_driver_24xrgb_rotated_canvas_blend(GX_CANVAS *canvas, GX_CANVAS *composite)
{
GX_RECTANGLE dirty;
GX_RECTANGLE overlap;
ULONG       *read;
ULONG       *read_start;
ULONG       *write;
ULONG       *write_start;
ULONG        fcolor;
GX_UBYTE     fred, fgreen, fblue;
GX_UBYTE     bred, bgreen, bblue;
GX_UBYTE     alpha, balpha;

ULONG        bcolor;
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

        read_start = (ULONG *)canvas -> gx_canvas_memory;
        write_start = (ULONG *)composite -> gx_canvas_memory;

        if (canvas -> gx_canvas_display -> gx_display_rotation_angle == GX_SCREEN_ROTATION_CW)
        {
            /* Index into starting row.  */
            read_start += (dirty.gx_rectangle_right - overlap.gx_rectangle_right) * canvas -> gx_canvas_y_resolution;

            /* Index into pixel.  */
            read_start += overlap.gx_rectangle_top - dirty.gx_rectangle_top;

            /* Calculate the write pointer.  */
            write_start += (composite -> gx_canvas_x_resolution - overlap.gx_rectangle_right - 1) * composite -> gx_canvas_y_resolution;
            write_start += overlap.gx_rectangle_top;
        }
        else
        {
            /* Index into starting row.  */
            read_start += (overlap.gx_rectangle_left - dirty.gx_rectangle_left) * canvas -> gx_canvas_y_resolution;

            /* Index into pixel.  */
            read_start += dirty.gx_rectangle_bottom - overlap.gx_rectangle_bottom;

            /* Calculate the write pointer.  */
            write_start += overlap.gx_rectangle_left * composite -> gx_canvas_y_resolution;
            write_start += (composite -> gx_canvas_y_resolution - overlap.gx_rectangle_bottom - 1);
        }

        for (row = overlap.gx_rectangle_left; row <= overlap.gx_rectangle_right; row++)
        {
            read = read_start;
            write = write_start;

            for (col = overlap.gx_rectangle_top; col <= overlap.gx_rectangle_bottom; col++)
            {
                /* Read the foreground color.  */
                fcolor = *read++;

                /* Split foreground into red, green, and blue components.  */
                fred = REDVAL_24BPP(fcolor);
                fgreen = GREENVAL_24BPP(fcolor);
                fblue = BLUEVAL_24BPP(fcolor);

                /* Read background color.  */
                bcolor = *write;

                /* Split background color into red, green, and blue components.  */
                bred = REDVAL_24BPP(bcolor);
                bgreen = GREENVAL_24BPP(bcolor);
                bblue = BLUEVAL_24BPP(bcolor);

                /* Blend foreground and background, each color channel.  */
                fred = (GX_UBYTE)(((bred * balpha) + (fred * alpha)) >> 8);
                fgreen = (GX_UBYTE)(((bgreen * balpha) + (fgreen * alpha)) >> 8);
                fblue = (GX_UBYTE)(((bblue * balpha) + (fblue * alpha)) >> 8);

                /* Re-assemble into 16-bit color and write it out.  */
                *write++ = ASSEMBLECOLOR_32ARGB((ULONG)0xff, (ULONG)fred, (ULONG)fgreen, (ULONG)fblue);
            }

            write_start += composite -> gx_canvas_y_resolution;
            read_start += canvas -> gx_canvas_y_resolution;
        }
    }
}

