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
/*    _gx_display_driver_24xrgb_rotated_pixel_blend       PORTABLE C      */
/*                                                           6.1.4        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Rotated pixel blend function for 24xrgb color format.               */
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
/*    REDVAL_24BPP                          Extract the red component     */
/*    GREENVAL_24BPP                        Extract the green component   */
/*    BLUEVAL_24BPP                         Extract the blue component    */
/*    ASSEMBLECOLOR_24BPP                   Assemble color components     */
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
VOID _gx_display_driver_24xrgb_rotated_pixel_blend(GX_DRAW_CONTEXT *context, INT x, INT y, GX_COLOR fcolor, GX_UBYTE alpha)
{
GX_UBYTE fred, fgreen, fblue;
GX_UBYTE bred, bgreen, bblue;
GX_UBYTE balpha;
ULONG    bcolor;
ULONG   *put;


    /* Is the pixel non-transparent?  */
    if (alpha > 0)
    {
        /* Calculate address of pixel.  */
        put = (ULONG *)context -> gx_draw_context_memory;

        if (context -> gx_draw_context_display -> gx_display_rotation_angle == GX_SCREEN_ROTATION_CW)
        {
            put += context -> gx_draw_context_pitch * (context -> gx_draw_context_canvas -> gx_canvas_x_resolution - x - 1);
            put += y;
        }
        else
        {
            put += context -> gx_draw_context_pitch * x;
            put += (context -> gx_draw_context_canvas -> gx_canvas_y_resolution - y - 1);
        }

        /* No need to blend if alpha value is 255.  */
        if (alpha == 255)
        {
            *put = (ULONG)(fcolor | 0xff000000);

            return;
        }

        /* Split foreground into red, green, and blue components.  */
        fred = REDVAL_24BPP(fcolor);
        fgreen = GREENVAL_24BPP(fcolor);
        fblue = BLUEVAL_24BPP(fcolor);

        /* Read background color.  */
        bcolor = *put;

        /* Split background color into red, green, and blue components.  */
        bred = REDVAL_24BPP(bcolor);
        bgreen = GREENVAL_24BPP(bcolor);
        bblue = BLUEVAL_24BPP(bcolor);

        /* Background alpha is inverse of foreground alpha.  */
        balpha = (GX_UBYTE)(256 - alpha);

        /* Blend foreground and background, each color channel.  */
        fred = (GX_UBYTE)(((bred * balpha) + (fred * alpha)) >> 8);
        fgreen = (GX_UBYTE)(((bgreen * balpha) + (fgreen * alpha)) >> 8);
        fblue = (GX_UBYTE)(((bblue * balpha) + (fblue * alpha)) >> 8);

        /* Re-assemble into 32-bit color and write it out.  */
        *put = (ULONG)(ASSEMBLECOLOR_32ARGB(0xff, fred, fgreen, fblue));
    }
}

