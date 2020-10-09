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
#include "gx_system.h"
#include "gx_utility.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_8bpp_block_move                  PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    8-bit color format display driver block moving function.           */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Draw context                  */
/*    block                                 The rectangle to be moved     */
/*    xshift                                Amount to move on X-axis      */
/*    yshift                                Amount to move on Y-axis      */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    memmove                               Move a block of data          */
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
VOID _gx_display_driver_8bpp_block_move(GX_DRAW_CONTEXT *context,
                                        GX_RECTANGLE *block, INT xshift, INT yshift)
{
GX_UBYTE *pGet;
GX_UBYTE *pPut;
int       width;
int       y;
int       height;

    if (xshift)
    {
        if (xshift > 0)
        {
            pPut = (GX_UBYTE *)context -> gx_draw_context_memory;
            pPut += block -> gx_rectangle_top * context -> gx_draw_context_pitch;
            pPut += block -> gx_rectangle_left + xshift;

            pGet = (GX_UBYTE *)context -> gx_draw_context_memory;
            pGet += block -> gx_rectangle_top * context -> gx_draw_context_pitch;
            pGet += block -> gx_rectangle_left;

            width = block -> gx_rectangle_right - block -> gx_rectangle_left + 1 - xshift;

            if (width <= 0)
            {
                /* Shift distance is bigger than block with. */
                return;
            }

            for (y = block -> gx_rectangle_top; y <= block -> gx_rectangle_bottom; y++)
            {
                memmove(pPut, pGet, (size_t)width);

                pPut += context -> gx_draw_context_pitch;
                pGet += context -> gx_draw_context_pitch;
            }
        }
        else
        {
            /* have to copy from left to right */
            pPut = (GX_UBYTE *)context -> gx_draw_context_memory;
            pPut += block -> gx_rectangle_top * context -> gx_draw_context_pitch;
            pPut += block -> gx_rectangle_left;

            pGet = (GX_UBYTE *)context -> gx_draw_context_memory;
            pGet += block -> gx_rectangle_top * context -> gx_draw_context_pitch;
            pGet += block -> gx_rectangle_left - xshift;

            width = block -> gx_rectangle_right - block -> gx_rectangle_left + 1 + xshift;

            if (width <= 0)
            {
                return;
            }

            for (y = block -> gx_rectangle_top; y <= block -> gx_rectangle_bottom; y++)
            {
                memmove(pPut, pGet, (size_t)width);

                pPut += context -> gx_draw_context_pitch;
                pGet += context -> gx_draw_context_pitch;
            }
        }
    }
    else
    {
        width = block -> gx_rectangle_right - block -> gx_rectangle_left + 1;

        if (yshift > 0)
        {
            /* have to copy from bottom to top */
            pPut = (GX_UBYTE *)context -> gx_draw_context_memory;
            pPut += block -> gx_rectangle_bottom * context -> gx_draw_context_pitch;
            pPut += block -> gx_rectangle_left;

            pGet = (GX_UBYTE *)context -> gx_draw_context_memory;
            pGet += (block -> gx_rectangle_bottom - yshift) * context -> gx_draw_context_pitch;
            pGet += block -> gx_rectangle_left;

            height = block -> gx_rectangle_bottom - block -> gx_rectangle_top + 1 - yshift;

            for (y = 0; y < height; y++)
            {
                memmove(pPut, pGet, (size_t)width);

                pPut -= context -> gx_draw_context_pitch;
                pGet -= context -> gx_draw_context_pitch;
            }
        }
        else
        {
            /* have to copy from top to bottom */
            pPut = (GX_UBYTE *)context -> gx_draw_context_memory;
            pPut += block -> gx_rectangle_top * context -> gx_draw_context_pitch;
            pPut += block -> gx_rectangle_left;

            pGet = (GX_UBYTE *)context -> gx_draw_context_memory;
            pGet += (block -> gx_rectangle_top - yshift) * context -> gx_draw_context_pitch;
            pGet += block -> gx_rectangle_left;

            height = block -> gx_rectangle_bottom - block -> gx_rectangle_top + 1 + yshift;

            for (y = 0; y < height; y++)
            {
                memmove(pPut, pGet, (size_t)width);

                pPut += context -> gx_draw_context_pitch;
                pGet += context -> gx_draw_context_pitch;
            }
        }
    }
}

