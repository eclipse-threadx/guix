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
/*    _gx_display_driver_1bpp_block_move                  PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    1-bit color palette display driver block moving function.           */
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
VOID _gx_display_driver_1bpp_block_move(GX_DRAW_CONTEXT *context,
                                        GX_RECTANGLE *block, INT xshift, INT yshift)
{
GX_UBYTE *putrow;
GX_UBYTE *getrow;
GX_UBYTE *pGet;
GX_UBYTE *pPut;
INT       width;
INT       column;
INT       y;
INT       height;
INT       stride;
GX_UBYTE  putmask;
GX_UBYTE  getmask;
INT       move_width;

    putrow = (GX_UBYTE *)context -> gx_draw_context_memory;

    stride = (context -> gx_draw_context_pitch + 7) >> 3;

    if (xshift)
    {
        if (xshift > 0)
        {
            /* Copy from left to right.  */
            width = block -> gx_rectangle_right - block -> gx_rectangle_left + 1;
            width -= xshift;

            putrow += block -> gx_rectangle_top * stride;
            getrow = putrow;

            putrow += block -> gx_rectangle_right >> 3;
            getrow += (block -> gx_rectangle_left + width - 1) >> 3;

            for (y = block -> gx_rectangle_top; y <= block -> gx_rectangle_bottom; y++)
            {
                pPut = putrow;
                pGet = getrow;

                putmask = (GX_UBYTE)(0x80 >> (block -> gx_rectangle_right & 0x07));
                getmask = (GX_UBYTE)(0x80 >> ((block -> gx_rectangle_left + width - 1) & 0x07));

                for (column = 0; column < width; column++)
                {
                    if ((*pGet) & getmask)
                    {
                        *pPut |= putmask;
                    }
                    else
                    {
                        *pPut = (GX_UBYTE)((*pPut) & (~putmask));
                    }

                    if (getmask == 0x80)
                    {
                        getmask = 0x01;
                        pGet--;
                    }
                    else
                    {
                        getmask = (GX_UBYTE)(getmask << 1);
                    }

                    if (putmask == 0x80)
                    {
                        putmask = 0x01;
                        pPut--;
                    }
                    else
                    {
                        putmask = (GX_UBYTE)(putmask << 1);
                    }
                }

                putrow += stride;
                getrow += stride;
            }
        }
        else
        {
            /* Copy from right to left.  */
            width = block -> gx_rectangle_right - block -> gx_rectangle_left + 1;
            width += xshift;

            putrow += block -> gx_rectangle_top * stride;
            getrow = putrow;

            putrow += block -> gx_rectangle_left >> 3;
            getrow += (block -> gx_rectangle_left - xshift) >> 3;

            for (y = block -> gx_rectangle_top; y <= block -> gx_rectangle_bottom; y++)
            {
                pPut = putrow;
                pGet = getrow;

                putmask = (GX_UBYTE)(0x80 >> (block -> gx_rectangle_left & 0x07));
                getmask = (GX_UBYTE)(0x80 >> ((block -> gx_rectangle_left - xshift) & 0x07));

                for (column = 0; column < width; column++)
                {
                    if ((*pGet) & getmask)
                    {
                        *pPut |= putmask;
                    }
                    else
                    {
                        *pPut &= (GX_UBYTE)(~putmask);
                    }

                    getmask >>= 1;
                    putmask >>= 1;

                    if (!getmask)
                    {
                        getmask = 0x80;
                        pGet++;
                    }
                    if (!putmask)
                    {
                        putmask = 0x80;
                        pPut++;
                    }
                }

                putrow += stride;
                getrow += stride;
            }
        }
    }
    else
    {
        width = block -> gx_rectangle_right - block -> gx_rectangle_left + 1;

        if (yshift > 0)
        {
            /* Copy from top to bottom.  */
            putrow += (block -> gx_rectangle_bottom * stride);
            putrow += (block -> gx_rectangle_left >> 3);

            getrow = putrow;
            getrow -= yshift * stride;

            height = block -> gx_rectangle_bottom - block -> gx_rectangle_top + 1;
            height -= yshift;

            for (y = 0; y < height; y++)
            {
                pPut = putrow;
                pGet = getrow;

                putmask = (GX_UBYTE)(0x80 >> (block -> gx_rectangle_left & 0x07));

                column = 0;
                while (column < width)
                {
                    if ((putmask == 0x80) && (width - column >= 8))
                    {
                        move_width = (width - column) >> 3;
                        memmove(pPut, pGet, (size_t)move_width);
                        pPut += move_width;
                        pGet += move_width;
                        column += (move_width << 3);
                    }
                    else
                    {
                        if ((*pGet) & putmask)
                        {
                            *pPut |= putmask;
                        }
                        else
                        {
                            *pPut &= (GX_UBYTE)(~putmask);
                        }

                        putmask >>= 1;

                        if (!putmask)
                        {
                            putmask = 0x80;
                            pPut++;
                            pGet++;
                        }

                        column++;
                    }
                }

                putrow -= stride;
                getrow -= stride;
            }
        }
        else
        {
            /* Copy from bottom to top.  */
            putrow += (block -> gx_rectangle_top * stride);
            putrow += (block -> gx_rectangle_left >> 3);

            getrow = putrow;
            getrow -= yshift * stride;

            height = block -> gx_rectangle_bottom - block -> gx_rectangle_top + 1;
            height += yshift;

            for (y = 0; y < height; y++)
            {
                pPut = putrow;
                pGet = getrow;

                putmask = (GX_UBYTE)(0x80 >> (block -> gx_rectangle_left & 0x07));

                column = 0;

                while (column < width)
                {
                    if ((putmask == 0x80) && (width - column >= 8))
                    {
                        move_width = (width - column) >> 3;
                        memmove(pPut, pGet, (size_t)move_width);
                        pPut += move_width;
                        pGet += move_width;
                        column += (move_width << 3);
                    }
                    else
                    {
                        if ((*pGet) & putmask)
                        {
                            *pPut |= putmask;
                        }
                        else
                        {
                            *pPut &= (GX_UBYTE)(~putmask);
                        }

                        putmask >>= 1;

                        if (!putmask)
                        {
                            putmask = 0x80;
                            pPut++;
                            pGet++;
                        }

                        column++;
                    }
                }

                putrow += stride;
                getrow += stride;
            }
        }
    }
}

