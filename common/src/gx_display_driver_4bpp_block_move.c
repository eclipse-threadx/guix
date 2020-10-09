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
/*    _gx_display_driver_4bpp_block_move                  PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    4-bpp display driver block moving function.                         */
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
VOID _gx_display_driver_4bpp_block_move(GX_DRAW_CONTEXT *context,
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

    stride = (context -> gx_draw_context_pitch + 1) >> 1;

    if (xshift)
    {
        if (xshift > 0)
        {
            /* Copy from right to left.  */
            width = block -> gx_rectangle_right - block -> gx_rectangle_left + 1;
            width -= xshift;

            putrow += block -> gx_rectangle_top * stride;
            getrow = putrow;

            putrow += block -> gx_rectangle_right >> 1;
            getrow += (block -> gx_rectangle_left + width - 1) >> 1;

            for (y = block -> gx_rectangle_top; y <= block -> gx_rectangle_bottom; y++)
            {
                pPut = putrow;
                pGet = getrow;

                if (block -> gx_rectangle_right & 0x01)
                {
                    putmask = 0x0f;
                }
                else
                {
                    putmask = 0xf0;
                }

                if ((block -> gx_rectangle_left + width - 1) & 0x01)
                {
                    getmask = 0x0f;
                }
                else
                {
                    getmask = 0xf0;
                }

                for (column = 0; column < width; column++)
                {
                    /* Set bits to 0. */
                    *pPut &= (GX_UBYTE)(~putmask);

                    if ((*pGet) & getmask)
                    {
                        if (getmask == putmask)
                        {
                            *pPut |= ((*pGet) & getmask);
                        }
                        else
                        {
                            if (getmask == 0xf0)
                            {
                                *pPut |= (GX_UBYTE)((((*pGet) & getmask) >> 4) & 0x0f);
                            }
                            else
                            {
                                *pPut |= (GX_UBYTE)((GX_UBYTE)(((*pGet) & getmask) << 4) & 0xf0);
                            }
                        }
                    }

                    if (getmask == 0xf0)
                    {
                        getmask = 0x0f;
                        pGet--;
                    }
                    else
                    {
                        getmask = 0xf0;
                    }

                    if (putmask == 0xf0)
                    {
                        putmask = 0x0f;
                        pPut--;
                    }
                    else
                    {
                        putmask = 0xf0;
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

            putrow += block -> gx_rectangle_left >> 1;
            getrow += (block -> gx_rectangle_left - xshift) >> 1;

            for (y = block -> gx_rectangle_top; y <= block -> gx_rectangle_bottom; y++)
            {
                pPut = putrow;
                pGet = getrow;

                if (block -> gx_rectangle_left & 0x01)
                {
                    putmask = 0x0f;
                }
                else
                {
                    putmask = 0xf0;
                }

                if ((block -> gx_rectangle_left - xshift) & 0x01)
                {
                    getmask = 0x0f;
                }
                else
                {
                    getmask = 0xf0;
                }

                for (column = 0; column < width; column++)
                {
                    /* Set bits to 0. */
                    *pPut &= (GX_UBYTE)(~putmask);

                    if ((*pGet) & getmask)
                    {
                        if (getmask == putmask)
                        {
                            *pPut |= ((*pGet) & getmask);
                        }
                        else
                        {
                            if (getmask == 0xf0)
                            {
                                *pPut |= (GX_UBYTE)((((*pGet) & getmask) >> 4) & 0x0f);
                            }
                            else
                            {
                                *pPut |= (GX_UBYTE)((GX_UBYTE)(((*pGet) & getmask) << 4) & 0xf0);
                            }
                        }
                    }

                    getmask >>= 4;
                    if (getmask == 0)
                    {
                        getmask = 0xf0;
                        pGet++;
                    }

                    putmask >>= 4;
                    if (putmask == 0)
                    {
                        putmask = 0xf0;
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
            putrow += (block -> gx_rectangle_left >> 1);

            getrow = putrow;
            getrow -= yshift * stride;

            height = block -> gx_rectangle_bottom - block -> gx_rectangle_top + 1;
            height -= yshift;

            for (y = 0; y < height; y++)
            {
                pPut = putrow;
                pGet = getrow;

                if (block -> gx_rectangle_left & 0x01)
                {
                    putmask = 0x0f;
                }
                else
                {
                    putmask = 0xf0;
                }

                column = 0;
                while (column < width)
                {
                    if ((putmask == 0xf0) && (width - column >= 2))
                    {
                        move_width = (width - column) >> 1;
                        memmove(pPut, pGet, (size_t)move_width);
                        pPut += move_width;
                        pGet += move_width;
                        column += (move_width << 1);
                    }
                    else
                    {
                        *pPut &= (GX_UBYTE)(~putmask);
                        if ((*pGet) & putmask)
                        {
                            *pPut |= (GX_UBYTE)((*pGet) & putmask);
                        }


                        putmask >>= 4;

                        if (putmask == 0)
                        {
                            putmask = 0xf0;
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
            putrow += (block -> gx_rectangle_left >> 1);

            getrow = putrow;
            getrow -= yshift * stride;

            height = block -> gx_rectangle_bottom - block -> gx_rectangle_top + 1;
            height += yshift;

            for (y = 0; y < height; y++)
            {
                pPut = putrow;
                pGet = getrow;

                if (block -> gx_rectangle_left & 0x01)
                {
                    putmask = 0x0f;
                }
                else
                {
                    putmask = 0xf0;
                }

                column = 0;

                while (column < width)
                {
                    if ((putmask == 0xf0) && (width - column >= 2))
                    {
                        move_width = (width - column) >> 1;
                        memmove(pPut, pGet, (size_t)move_width);
                        pPut += move_width;
                        pGet += move_width;
                        column += (move_width << 1);
                    }
                    else
                    {
                        *pPut &= (GX_UBYTE)(~putmask);
                        if ((*pGet) & putmask)
                        {
                            *pPut |= (GX_UBYTE)((*pGet) & putmask);
                        }

                        putmask >>= 4;

                        if (putmask == 0)
                        {
                            putmask = 0xf0;
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

