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

#define _COLOR_FORMAT_ 565rgb /*what's this for??*/
#define PIXEL_LOC      USHORT

#define ALPHAVAL(_c)   (GX_UBYTE)(((_c) >> 12) & 0xf)
#define REDVAL(_c)     (GX_UBYTE)(((_c) >> 8) & 0xf)
#define GREENVAL(_c)   (GX_UBYTE)(((_c) >> 4) & 0xf)
#define BLUEVAL(_c)    (GX_UBYTE)((_c) & 0xf)

#define ASSEMBLECOLOR_4444ARGB(_a, _r, _g, _b) \
    (((_a) << 12)   |          \
     ((_r) << 8)    |          \
     ((_g) << 4)    |          \
     ((_b) & 0xf))

#define ExtendToByte(_a)  _a |= (GX_UBYTE)(_a << 4)

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_display.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_4444argb_pixel_blend            PORTABLE C       */
/*                                                           6.1          */
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
/*                                                                        */
/**************************************************************************/
VOID _gx_display_driver_4444argb_pixel_blend(GX_DRAW_CONTEXT *context, INT x, INT y, GX_COLOR fcolor, GX_UBYTE alpha)
{
GX_UBYTE falpha, fred, fgreen, fblue;
GX_UBYTE balpha, bred, bgreen, bblue;
GX_UBYTE inv_alpha;
INT      combined_alpha;
USHORT   bcolor;
USHORT  *put;


    falpha = ALPHAVAL(fcolor);
    falpha = (GX_UBYTE)(falpha | (falpha << 4));
    combined_alpha = falpha * alpha;
    combined_alpha /= 255;

    /* Is the pixel non-transparent? */
    if (combined_alpha > 0)
    {
        /* calculate address of pixel */
        put = (USHORT *)context -> gx_draw_context_memory;
        put += context -> gx_draw_context_pitch * y;
        put += x;

        /* No need to blend if alpha value is 255. */
        if (combined_alpha == 255)
        {
            *put = (USHORT)fcolor;
            return;
        }

        /* split foreground into red, green, and blue components */
        fred = REDVAL(fcolor);
        ExtendToByte(fred);
        fgreen = GREENVAL(fcolor);
        ExtendToByte(fgreen);
        fblue = BLUEVAL(fcolor);
        ExtendToByte(fblue);

        /* read background color */
        bcolor = *put;

        /* split background color into red, green, and blue components */
        balpha = ALPHAVAL(bcolor);
        ExtendToByte(balpha);
        bred = REDVAL(bcolor);
        ExtendToByte(bred);
        bgreen = GREENVAL(bcolor);
        ExtendToByte(bgreen);
        bblue = BLUEVAL(bcolor);
        ExtendToByte(bblue);

        /* background alpha is inverse of foreground alpha */
        inv_alpha = (GX_UBYTE)(256 - combined_alpha);

        /* blend foreground and background, each color channel */
        falpha = (GX_UBYTE)(((balpha * inv_alpha) + (falpha * combined_alpha)) >> 12);
        fred = (GX_UBYTE)(((bred * inv_alpha) + (fred * combined_alpha)) >> 12);
        fgreen = (GX_UBYTE)(((bgreen * inv_alpha) + (fgreen * combined_alpha)) >> 12);
        fblue = (GX_UBYTE)(((bblue * inv_alpha) + (fblue * combined_alpha)) >> 12);


        /* re-assemble into 16-bit color and write it out */
        *put = (USHORT)ASSEMBLECOLOR_4444ARGB(falpha, fred, fgreen, fblue);
    }
}

