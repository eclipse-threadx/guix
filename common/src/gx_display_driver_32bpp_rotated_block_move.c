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
/*    _gx_display_driver_32bpp_rotated_block_move         PORTABLE C      */
/*                                                           6.1.4        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Generic rotated 32bpp color format display driver block moving      */
/*    function.                                                           */
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
/*    memmove                                Move memory content          */
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
VOID _gx_display_driver_32bpp_rotated_block_move(GX_DRAW_CONTEXT *context,
                                                 GX_RECTANGLE *block, INT xshift, INT yshift)
{
GX_COLOR    *pGet;
GX_COLOR    *pPut;
int          width;
int          width_in_bytes;
int          y;
int          height;
GX_RECTANGLE rotated_block;

    GX_SWAP_VALS(xshift, yshift);

    if (context -> gx_draw_context_display -> gx_display_rotation_angle == GX_SCREEN_ROTATION_CW)
    {
        rotated_block.gx_rectangle_left = block -> gx_rectangle_top;
        rotated_block.gx_rectangle_right = block -> gx_rectangle_bottom;
        rotated_block.gx_rectangle_top = (GX_VALUE)(context -> gx_draw_context_canvas -> gx_canvas_x_resolution - block -> gx_rectangle_right - 1);
        rotated_block.gx_rectangle_bottom = (GX_VALUE)(context -> gx_draw_context_canvas -> gx_canvas_x_resolution - block -> gx_rectangle_left - 1);

        yshift = -yshift;
    }
    else
    {
        rotated_block.gx_rectangle_left = (GX_VALUE)(context -> gx_draw_context_canvas -> gx_canvas_y_resolution - block -> gx_rectangle_bottom - 1);
        rotated_block.gx_rectangle_right = (GX_VALUE)(context -> gx_draw_context_canvas -> gx_canvas_y_resolution - block -> gx_rectangle_top - 1);
        rotated_block.gx_rectangle_top = block -> gx_rectangle_left;
        rotated_block.gx_rectangle_bottom = block -> gx_rectangle_right;

        xshift = -xshift;
    }

    if (xshift)
    {
        if (xshift > 0)
        {
            /* Have to copy from left to right.  */
            pPut = context -> gx_draw_context_memory;
            pPut += rotated_block.gx_rectangle_top * context -> gx_draw_context_pitch;
            pPut += rotated_block.gx_rectangle_left + xshift;

            pGet = context -> gx_draw_context_memory;
            pGet += rotated_block.gx_rectangle_top * context -> gx_draw_context_pitch;
            pGet += rotated_block.gx_rectangle_left;

            width = rotated_block.gx_rectangle_right - rotated_block.gx_rectangle_left + 1 - xshift;
            width_in_bytes = width * (int)sizeof(GX_COLOR);

            if (width_in_bytes <= 0)
            {
                return;
            }

            for (y = rotated_block.gx_rectangle_top; y <= rotated_block.gx_rectangle_bottom; y++)
            {
                memmove(pPut, pGet, (size_t)width_in_bytes);

                pPut += context -> gx_draw_context_pitch;
                pGet += context -> gx_draw_context_pitch;
            }
        }
        else
        {
            /* Have to copy from right to left.  */
            pPut = context -> gx_draw_context_memory;
            pPut += rotated_block.gx_rectangle_top * context -> gx_draw_context_pitch;
            pPut += rotated_block.gx_rectangle_left;

            pGet = context -> gx_draw_context_memory;
            pGet += rotated_block.gx_rectangle_top * context -> gx_draw_context_pitch;
            pGet += rotated_block.gx_rectangle_left - xshift;

            width = rotated_block.gx_rectangle_right - rotated_block.gx_rectangle_left + 1 + xshift;
            width_in_bytes = width * (int)sizeof(GX_COLOR);

            if (width_in_bytes <= 0)
            {
                return;
            }

            for (y = rotated_block.gx_rectangle_top; y <= rotated_block.gx_rectangle_bottom; y++)
            {
                memmove(pPut, pGet, (size_t)width_in_bytes);

                pPut += context -> gx_draw_context_pitch;
                pGet += context -> gx_draw_context_pitch;
            }
        }
    }
    else
    {
        width = rotated_block.gx_rectangle_right - rotated_block.gx_rectangle_left + 1;
        width_in_bytes = width * (int)sizeof(GX_COLOR);

        if (yshift > 0)
        {
            /* Have to copy from top to bottom.  */
            pPut = context -> gx_draw_context_memory;
            pPut += rotated_block.gx_rectangle_bottom * context -> gx_draw_context_pitch;
            pPut += rotated_block.gx_rectangle_left;

            pGet = context -> gx_draw_context_memory;
            pGet += (rotated_block.gx_rectangle_bottom - yshift) * context -> gx_draw_context_pitch;
            pGet += rotated_block.gx_rectangle_left;

            height = rotated_block.gx_rectangle_bottom - rotated_block.gx_rectangle_top + 1 - yshift;

            for (y = 0; y < height; y++)
            {
                memmove(pPut, pGet, (size_t)width_in_bytes);

                pPut -= context -> gx_draw_context_pitch;
                pGet -= context -> gx_draw_context_pitch;
            }
        }
        else
        {
            /* Have to copy from bottom to top.  */
            pPut = context -> gx_draw_context_memory;
            pPut += rotated_block.gx_rectangle_top * context -> gx_draw_context_pitch;
            pPut += rotated_block.gx_rectangle_left;

            pGet = context -> gx_draw_context_memory;
            pGet += (rotated_block.gx_rectangle_top - yshift) * context -> gx_draw_context_pitch;
            pGet += rotated_block.gx_rectangle_left;

            height = rotated_block.gx_rectangle_bottom - rotated_block.gx_rectangle_top + 1 + yshift;

            for (y = 0; y < height; y++)
            {
                memmove(pPut, pGet, (size_t)width_in_bytes);

                pPut += context -> gx_draw_context_pitch;
                pGet += context -> gx_draw_context_pitch;
            }
        }
    }
}

