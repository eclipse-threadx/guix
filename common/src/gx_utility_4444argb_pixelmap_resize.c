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
/**   Utility (Utility)                                                   */
/**                                                                       */
/**************************************************************************/

#define ALPHAVAL(_c)   (GX_UBYTE)(((_c) >> 12) & 0xf)
#define REDVAL(_c)     (GX_UBYTE)(((_c) >> 8) & 0xf)
#define GREENVAL(_c)   (GX_UBYTE)(((_c) >> 4) & 0xf)
#define BLUEVAL(_c)    (GX_UBYTE)(((_c)) & 0xf)

#define ASSEMBLECOLOR(_a, _r, _g, _b)              \
      (USHORT)((((_a) & 0xf) << 12)              | \
               (((_r) & 0xf) << 8)               | \
               (((_g) & 0xf) << 4)               | \
               ((_b) & 0xf))

#define BYTE_RANGE(_c) _c > 15 ? 15 : _c

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_utility.h"
#include "gx_system.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_4444argb_pixelmap_resize                PORTABLE C      */
/*                                                           6.1.7        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    4444argb pixelmap resize function that handles uncompress,          */
/*    with alpha channel.                                                 */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    src                                   The source pixelmap           */
/*    destination                           The resized pixelmap to be    */
/*                                            returned                    */
/*    width                                 New width                     */
/*    height                                New height                    */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
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
/*  06-02-2021     Kenneth Maxwell          Modified comment(s),          */
/*                                            removed unused variable     */
/*                                            assignment,                 */
/*                                            resulting in version 6.1.7  */
/*                                                                        */
/**************************************************************************/
UINT _gx_utility_4444argb_pixelmap_resize(GX_PIXELMAP *src, GX_PIXELMAP *destination, INT width, INT height)
{
USHORT  *get;
USHORT  *put;
INT      xdiff;
INT      ydiff;
INT      xradio;
INT      yradio;
INT      x;
INT      y;
INT      xx;
INT      yy;
USHORT   neighbor_pixels[2][2];
GX_COLOR alpha[4];
GX_COLOR red;
GX_COLOR green;
GX_COLOR blue;

    /* Calculate scale ratio and enlarge it by 256 times to keep precision.  */
    xradio = ((src -> gx_pixelmap_width) << 8) / width;
    yradio = ((src -> gx_pixelmap_height) << 8) / height;

    /* Fill property values into destination pixelmap structure. */
    destination -> gx_pixelmap_flags = src -> gx_pixelmap_flags;
    destination -> gx_pixelmap_format = src -> gx_pixelmap_format;

    destination -> gx_pixelmap_height = (GX_VALUE)height;
    destination -> gx_pixelmap_width = (GX_VALUE)width;

    /* Safe int math is not required here, calling function limits max width, height to 14 bits so
       overflow cannot occur. */
    destination -> gx_pixelmap_data_size = (UINT)(height * width) * sizeof(USHORT);

    /* Allocate memory to load pixelmap data. */
    destination -> gx_pixelmap_data = (GX_UBYTE *)_gx_system_memory_allocator(destination -> gx_pixelmap_data_size);

    if (destination -> gx_pixelmap_data == GX_NULL)
    {
        return GX_SYSTEM_MEMORY_ERROR;
    }

    put = (USHORT *)destination -> gx_pixelmap_data;

    for (y = 0; y < height; y++)
    {
        for (x = 0; x < width; x++)
        {
            xx = (xradio * x) >> 8;
            yy = (yradio * y) >> 8;

            /* The coordinates of the original source pixel are truncate value,
               calucate their distance between the mathematical coordinates. */
            xdiff = (xradio * x) & 0xff;
            ydiff = (yradio * y) & 0xff;

            get = (USHORT *)src -> gx_pixelmap_data;
            get += yy * src -> gx_pixelmap_width;
            get += xx;

            neighbor_pixels[0][0] = *get;

            if ((xx < src -> gx_pixelmap_width - 1) && (yy < src -> gx_pixelmap_height - 1))
            {
                neighbor_pixels[0][1] = *(get + 1);
                neighbor_pixels[1][0] = *(get + src -> gx_pixelmap_width);
                neighbor_pixels[1][1] = *(get + src -> gx_pixelmap_width + 1);
            }
            else
            {
                if ((xx == src -> gx_pixelmap_width - 1) &&
                    (yy == src -> gx_pixelmap_height - 1))
                {
                    /* Handle right bottom corder pixel.  */
                    neighbor_pixels[0][1] = neighbor_pixels[0][0];
                    neighbor_pixels[1][0] = neighbor_pixels[0][0];
                    neighbor_pixels[1][1] = neighbor_pixels[0][0];
                }
                else if (xx == src -> gx_pixelmap_width - 1)
                {
                    /* Handle pixels in right edge.  */
                    neighbor_pixels[0][1] = neighbor_pixels[0][0];
                    neighbor_pixels[1][0] = *(get + src -> gx_pixelmap_width);
                    neighbor_pixels[1][1] = neighbor_pixels[1][0];
                }
                else
                {
                    /* Handle pixels in bottom edge.  */
                    neighbor_pixels[0][1] = *(get + 1);
                    neighbor_pixels[1][0] = neighbor_pixels[0][0];
                    neighbor_pixels[1][1] = neighbor_pixels[0][1];
                }
            }
            alpha[0] = ALPHAVAL(neighbor_pixels[0][0]);
            alpha[1] = ALPHAVAL(neighbor_pixels[0][1]);
            alpha[2] = ALPHAVAL(neighbor_pixels[1][0]);
            alpha[3] = ALPHAVAL(neighbor_pixels[1][1]);

            /* Calulate pixel values by interpolating 4 neighboring pixels. */
            red = (REDVAL(neighbor_pixels[0][0]) * (alpha[0]) * (256 - (GX_COLOR)xdiff) * (256 - (GX_COLOR)ydiff) + \
                   REDVAL(neighbor_pixels[0][1]) * (alpha[1]) * (GX_COLOR)xdiff * (256 - (GX_COLOR)ydiff) +         \
                   REDVAL(neighbor_pixels[1][0]) * (alpha[2]) * (GX_COLOR)ydiff * (256 - (GX_COLOR)xdiff) +         \
                   REDVAL(neighbor_pixels[1][1]) * (alpha[3]) * (GX_COLOR)xdiff * (GX_COLOR)ydiff) >> 16;

            green = (GREENVAL(neighbor_pixels[0][0]) * (alpha[0]) * (256 - (GX_COLOR)xdiff) * (256 - (GX_COLOR)ydiff) + \
                     GREENVAL(neighbor_pixels[0][1]) * (alpha[1]) * (GX_COLOR)xdiff * (256 - (GX_COLOR)ydiff) +         \
                     GREENVAL(neighbor_pixels[1][0]) * (alpha[2]) * (GX_COLOR)ydiff * (256 - (GX_COLOR)xdiff) +         \
                     GREENVAL(neighbor_pixels[1][1]) * (alpha[3]) * (GX_COLOR)xdiff * (GX_COLOR)ydiff) >> 16;

            blue = (BLUEVAL(neighbor_pixels[0][0]) * (alpha[0]) * (256 - (GX_COLOR)xdiff) * (256 - (GX_COLOR)ydiff) + \
                    BLUEVAL(neighbor_pixels[0][1]) * (alpha[1]) * (GX_COLOR)xdiff * (256 - (GX_COLOR)ydiff) +         \
                    BLUEVAL(neighbor_pixels[1][0]) * (alpha[2]) * (GX_COLOR)ydiff * (256 - (GX_COLOR)xdiff) +         \
                    BLUEVAL(neighbor_pixels[1][1]) * (alpha[3]) * (GX_COLOR)xdiff * (GX_COLOR)ydiff) >> 16;

            alpha[0] = ((alpha[0]) * (256 - (GX_COLOR)xdiff) * (256 - (GX_COLOR)ydiff) + \
                        (alpha[1]) * (GX_COLOR)xdiff * (256 - (GX_COLOR)ydiff) +         \
                        (alpha[2]) * (GX_COLOR)ydiff * (256 - (GX_COLOR)xdiff) +         \
                        (alpha[3]) * (GX_COLOR)xdiff * (GX_COLOR)ydiff) >> 16;

            if (alpha[0])
            {
                red /= alpha[0];
                green /= alpha[0];
                blue /= alpha[0];
            }

            alpha[0] = BYTE_RANGE(alpha[0]);
            red = BYTE_RANGE(red);
            green = BYTE_RANGE(green);
            blue = BYTE_RANGE(blue);

            *put++ = ASSEMBLECOLOR(alpha[0], red, green, blue);
        }
    }

    return GX_SUCCESS;
}

