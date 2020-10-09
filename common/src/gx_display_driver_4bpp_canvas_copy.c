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
/*    _gx_display_driver_4bpp_canvas_copy                 PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Generic 4bpp canvas copy function.                                  */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*   canvas                                 The canvas to copy from       */
/*   composite                              The canvas to copy to         */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_rectangle_shift           Move the rectangle            */
/*    _gx_utility_rectangle_overlap_detect  Detect two rectangles being   */
/*                                            overlap to each other       */
/*    memcpy                                Move canvas data              */
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
VOID _gx_display_driver_4bpp_canvas_copy(GX_CANVAS *canvas, GX_CANVAS *composite)
{
GX_RECTANGLE dirty;
GX_RECTANGLE overlap;
GX_UBYTE    *read;
GX_UBYTE    *write;
GX_UBYTE     color;
INT          row;
INT          column;
UINT         read_pos;
UINT         write_pos;
GX_UBYTE     read_mask;
GX_UBYTE     write_mask;
INT          readstride;
INT          writestride;
INT          offset;

    dirty.gx_rectangle_left = dirty.gx_rectangle_top = 0;
    dirty.gx_rectangle_right = (GX_VALUE)(canvas -> gx_canvas_x_resolution - 1);
    dirty.gx_rectangle_bottom = (GX_VALUE)(canvas -> gx_canvas_y_resolution - 1);
    readstride = (canvas->gx_canvas_x_resolution + 1) >> 1;
    writestride = (composite->gx_canvas_x_resolution + 1) >> 1;

    _gx_utility_rectangle_shift(&dirty, canvas -> gx_canvas_display_offset_x, canvas -> gx_canvas_display_offset_y);

    if (_gx_utility_rectangle_overlap_detect(&dirty, &composite -> gx_canvas_dirty_area, &overlap))
    {
        offset = overlap.gx_rectangle_left - dirty.gx_rectangle_left;
        read_pos = (UINT)((overlap.gx_rectangle_top - dirty.gx_rectangle_top) * readstride + (offset >> 1));
        write_pos = (UINT)(overlap.gx_rectangle_top * writestride + (overlap.gx_rectangle_left >> 1));

        for (row = overlap.gx_rectangle_top; row <= overlap.gx_rectangle_bottom; row++)
        {
            read = (GX_UBYTE *)canvas->gx_canvas_memory;
            write = (GX_UBYTE *)composite->gx_canvas_memory;

            read += read_pos;
            write += write_pos;
            /* If position is odd, it means the low bits. */
            if (offset & 0x01)
            {
                read_mask = 0x0f;
            }
            else
            {
                read_mask = 0xf0;
            }

            if (overlap.gx_rectangle_left & 0x01)
            {
                write_mask = 0x0f;
            }
            else
            {
                write_mask = 0xf0;
            }

            for (column = overlap.gx_rectangle_left; column <= overlap.gx_rectangle_right; column++)
            {
                color = (*read) & read_mask;
                *write = (GX_UBYTE)((*write) & (~write_mask));

                if (color)
                {
                    /* Read and write have same mask bits. */
                    if (read_mask & write_mask)
                    {
                        *write |= color;
                    }
                    else
                    {
                        /* Read and write are malposed. */
                        /* If position is odd, it means the low bits. */
                        if (write_mask & 0x01)
                        {
                            *write |= (GX_UBYTE)(color >> 4);
                        }
                        else
                        {
                            *write |= (GX_UBYTE)(color << 4);
                        }
                    }
                }

                read_mask >>= 4;
                write_mask >>= 4;
                if (!read_mask)
                {
                    read++;
                    read_mask = 0xf0;
                }
                if (!write_mask)
                {
                    write++;
                    write_mask = 0xf0;
                }
            }
            write_pos = (UINT)((INT)write_pos + writestride);
            read_pos = (UINT)((INT)read_pos + readstride);
        }
    }
}

