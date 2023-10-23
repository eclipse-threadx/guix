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

#define _COLOR_FORMAT_ 565rgb
#define PIXEL_LOC      USHORT

#define REDVAL(_c)     (GX_UBYTE)(((_c) >> 11) & 0x1f)
#define GREENVAL(_c)   (GX_UBYTE)(((_c) >> 5) & 0x3f)
#define BLUEVAL(_c)    (GX_UBYTE)(((_c)) & 0x1f)

#define ASSEMBLECOLOR(_r, _g, _b) \
    ((((_r) & 0x1f) << 11) |      \
     (((_g) & 0x3f) << 5) |       \
     (((_b) & 0x1f)))

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_display.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_565rgb_pixel_blend              PORTABLE C       */
/*                                                           6.3.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Pixel blend function for 565rgb color format.                       */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Drawing context               */
/*    x                                     X coordinate                  */
/*    y                                     Y coordinate                  */
/*    color                                 Color of line to write        */
/*    alpha                                 Alpha value                   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    REDVAL                                Extract the red component     */
/*    GREENVAL                              Extract the green component   */
/*    BLUEVAL                               Extract the blue component    */
/*    ASSEMBLECOLOR                         Assemble color components     */
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
VOID _gx_display_driver_565rgb_pixel_blend(GX_DRAW_CONTEXT *context, INT x, INT y, GX_COLOR fcolor, GX_UBYTE alpha)
{
GX_UBYTE fred, fgreen, fblue;
GX_UBYTE bred, bgreen, bblue;
GX_UBYTE balpha;

USHORT   bcolor;
USHORT  *put;

    /* Is the pixel non-transparent? */
    if (alpha > 0)
    {
        /* calculate address of pixel */
        put = (USHORT *)context -> gx_draw_context_memory;
        GX_CALCULATE_PUTROW(put, x, y, context);

        /* No need to blend if alpha value is 255. */
        if (alpha == 255)
        {
            *put = (USHORT)fcolor;
            return;
        }

        /* split foreground into red, green, and blue components */
        fred = REDVAL(fcolor);
        fgreen = GREENVAL(fcolor);
        fblue = BLUEVAL(fcolor);

        /* read background color */
        bcolor = *put;

        /* split background color into red, green, and blue components */
        bred = REDVAL(bcolor);
        bgreen = GREENVAL(bcolor);
        bblue = BLUEVAL(bcolor);

        /* background alpha is inverse of foreground alpha */
        balpha = (GX_UBYTE)(256 - alpha);

        /* blend foreground and background, each color channel */
        fred = (GX_UBYTE)(((bred * balpha) + (fred * alpha)) >> 8);
        fgreen = (GX_UBYTE)(((bgreen * balpha) + (fgreen * alpha)) >> 8);
        fblue = (GX_UBYTE)(((bblue * balpha) + (fblue * alpha)) >> 8);

        /* re-assemble into 16-bit color and write it out */
        *put = (USHORT)ASSEMBLECOLOR(fred, fgreen, fblue);
    }
}

