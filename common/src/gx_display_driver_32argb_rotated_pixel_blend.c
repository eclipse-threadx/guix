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


#include "gx_api.h"
#include "gx_display.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_32argb_rotated_pixel_blend       PORTABLE C      */
/*                                                           6.1.4        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Rotated Pixel blend function for 32argb color format.               */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Drawing context               */
/*    x                                     X coordinate                  */
/*    y                                     Y coordinate                  */
/*    color                                 Color of line to write        */
/*    alpha                                 blending value 0 to 255       */
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
/*  02-02-2021     Kenneth Maxwell          Initial Version 6.1.4         */
/*                                                                        */
/**************************************************************************/
VOID _gx_display_driver_32argb_rotated_pixel_blend(GX_DRAW_CONTEXT *context, INT x, INT y, GX_COLOR fcolor, GX_UBYTE alpha)
{
GX_UBYTE falpha, fred, fgreen, fblue;
GX_UBYTE balpha, bred, bgreen, bblue;
GX_UBYTE oalpha;
ULONG    bcolor;
ULONG   *put;
INT      combined_alpha;


    falpha = ALPHAVAL_32BPP(fcolor);
    falpha = (GX_UBYTE)((falpha * alpha) / 255);

    /* Is the pixel non-transparent?  */
    if (falpha > 0)
    {

        /* Calculate address of pixel.  */
        put = (ULONG *)context -> gx_draw_context_memory;

        GX_SWAP_VALS(x, y);
        if (context -> gx_draw_context_display -> gx_display_rotation_angle == GX_SCREEN_ROTATION_CW)
        {
            y = context -> gx_draw_context_canvas -> gx_canvas_x_resolution - y - 1;
        }
        else
        {
            x = context -> gx_draw_context_canvas -> gx_canvas_y_resolution - x - 1;
        }

        put += context -> gx_draw_context_pitch * y;
        put += x;

        /* No need to blend if alpha value is 255.  */
        if (falpha == 255)
        {
            *put = (ULONG)fcolor;
            return;
        }

        /* Split foreground into alpha, red, green, and blue components.  */
        fred = REDVAL_32BPP(fcolor);
        fgreen = GREENVAL_32BPP(fcolor);
        fblue = BLUEVAL_32BPP(fcolor);

        /* Read background color.  */
        bcolor = *put;

        /* Split background color into red, green, and blue components.  */
        balpha = ALPHAVAL_32BPP(bcolor);
        bred = REDVAL_32BPP(bcolor);
        bgreen = GREENVAL_32BPP(bcolor);
        bblue = BLUEVAL_32BPP(bcolor);

        /* Background alpha is inverse of foreground alpha.  */
        combined_alpha = (falpha * balpha) / 0xff;

        /* Blend foreground and background, each color channel.  */
        oalpha = (GX_UBYTE)(falpha + balpha - combined_alpha);
        fred = (GX_UBYTE)((fred * falpha + bred * balpha - bred * combined_alpha) / oalpha);
        fgreen = (GX_UBYTE)((fgreen * falpha + bgreen * balpha - bgreen * combined_alpha) / oalpha);
        fblue = (GX_UBYTE)((fblue * falpha + bblue * balpha - bblue * combined_alpha) / oalpha);

        /* Re-assemble into 16-bit color and write it out.  */
        *put = (ULONG)ASSEMBLECOLOR_32ARGB(oalpha, fred, fgreen, fblue);
    }
}

