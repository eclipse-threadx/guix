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

#define REDVAL(_c)     (GX_UBYTE)(((_c) >> 11) & 0x1f)
#define GREENVAL(_c)   (GX_UBYTE)(((_c) >> 5) & 0x3f)
#define BLUEVAL(_c)    (GX_UBYTE)(((_c)) & 0x1f)

#define ASSEMBLECOLOR(_r, _g, _b)                     \
    (USHORT)((((_r) & 0x1f) << 11)                  | \
             (((_g) & 0x3f) << 5)                   | \
             ((_b) & 0x1f))

#define BYTE_RANGE(_c) _c > 255 ? 255 : _c

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_utility.h"
#include "gx_system.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_16bpp_pixelmap_raw_resize               PORTABLE C      */
/*                                                           6.1.7        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that resize an uncompressed pixelmap       */
/*      without alpha.                                                    */
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
/*    _gx_system_memory_allocator                                         */
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
static UINT _gx_utility_16bpp_pixelmap_raw_resize(GX_PIXELMAP *src, GX_PIXELMAP *destination, INT width, INT height)
{
/* The pixelmap resize function is implemented from bilinear interpolation
   image scaling algorithm.  */

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

    /* Allocate memory for destination pixelmap.  */
    destination -> gx_pixelmap_data = (GX_UBYTE *)_gx_system_memory_allocator(destination -> gx_pixelmap_data_size);
    destination -> gx_pixelmap_aux_data = GX_NULL;

    if (destination -> gx_pixelmap_data == GX_NULL)
    {
        return GX_SYSTEM_MEMORY_ERROR;
    }

    put = (USHORT *)destination -> gx_pixelmap_data;

    /* Loop through destination's pixel and fill each pixel with
       the interpolation of 4 nearest neighboring pixels.*/
    for (y = 0; y < height; y++)
    {
        for (x = 0; x < width; x++)
        {
            /* Find the original source pixel that the destination pixel conrespond to. */
            xx = (xradio * x) >> 8;
            yy = (yradio * y) >> 8;

            /* The coordinates of the original source pixel are truncate value,
               calucate their distance between the mathematical coordinates. */
            xdiff = (xradio * x) & 0xff;
            ydiff = (yradio * y) & 0xff;

            get = (USHORT *)src -> gx_pixelmap_data;
            get += yy * src -> gx_pixelmap_width;
            get += xx;

            /* Calculate 4 nearest neighboring pixels around the mathematical point of original pixel. */
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
                    /* Handle pixels in right bottom corner.  */
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


            /* Calulate pixel values by interpolating 4 neighboring pixels. */
            red = (REDVAL(neighbor_pixels[0][0]) * (256 - (GX_COLOR)xdiff) * (256 - (GX_COLOR)ydiff) + \
                   REDVAL(neighbor_pixels[0][1]) * (GX_COLOR)xdiff * (256 - (GX_COLOR)ydiff) +         \
                   REDVAL(neighbor_pixels[1][0]) * (GX_COLOR)ydiff * (256 - (GX_COLOR)xdiff) +         \
                   REDVAL(neighbor_pixels[1][1]) * (GX_COLOR)xdiff * (GX_COLOR)ydiff) >> 16;

            green = (GREENVAL(neighbor_pixels[0][0]) * (256 - (GX_COLOR)xdiff) * (256 - (GX_COLOR)ydiff) + \
                     GREENVAL(neighbor_pixels[0][1]) * (GX_COLOR)xdiff * (256 - (GX_COLOR)ydiff) +         \
                     GREENVAL(neighbor_pixels[1][0]) * (GX_COLOR)ydiff * (256 - (GX_COLOR)xdiff) +         \
                     GREENVAL(neighbor_pixels[1][1]) * (GX_COLOR)xdiff * (GX_COLOR)ydiff) >> 16;

            blue = (BLUEVAL(neighbor_pixels[0][0]) * (256 - (GX_COLOR)xdiff) * (256 - (GX_COLOR)ydiff) + \
                    BLUEVAL(neighbor_pixels[0][1]) * (GX_COLOR)xdiff * (256 - (GX_COLOR)ydiff) +         \
                    BLUEVAL(neighbor_pixels[1][0]) * (GX_COLOR)ydiff * (256 - (GX_COLOR)xdiff) +         \
                    BLUEVAL(neighbor_pixels[1][1]) * (GX_COLOR)xdiff * (GX_COLOR)ydiff) >> 16;

            red = BYTE_RANGE(red);
            green = BYTE_RANGE(green);
            blue = BYTE_RANGE(blue);

            *put++ = ASSEMBLECOLOR(red, green, blue);
        }
    }

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_565rgb_pixelmap_alpha_resize            PORTABLE C      */
/*                                                           6.1.7        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that resize an uncompressed pixelmap       */
/*      with alpha.                                                       */
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
/*    _gx_system_memory_allocator                                         */
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
static UINT _gx_utility_16bpp_pixelmap_alpha_resize(GX_PIXELMAP *src, GX_PIXELMAP *destination, INT width, INT height)
{
/* The pixelmap resize function is implemented from bilinear interpolation
   image scaling algorithm.  */

USHORT   *get;
USHORT   *put;
GX_UBYTE *getalpha;
GX_UBYTE *putalpha;
INT       xdiff;
INT       ydiff;
INT       xradio;
INT       yradio;
INT       x;
INT       y;
INT       xx;
INT       yy;
USHORT    neighbor_pixels[2][2];
GX_COLOR  alpha[4];
GX_COLOR  red;
GX_COLOR  green;
GX_COLOR  blue;

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
    destination -> gx_pixelmap_aux_data_size = (UINT)(height * width) * sizeof(GX_UBYTE);

    /* Allocate memory to load pixelmap data. */
    destination -> gx_pixelmap_data = (GX_UBYTE *)_gx_system_memory_allocator(destination -> gx_pixelmap_data_size);

    if (destination -> gx_pixelmap_data == GX_NULL)
    {
        return GX_SYSTEM_MEMORY_ERROR;
    }

    destination -> gx_pixelmap_aux_data = (GX_UBYTE *)_gx_system_memory_allocator(destination -> gx_pixelmap_aux_data_size);

    if (destination -> gx_pixelmap_aux_data == GX_NULL)
    {
        _gx_system_memory_free((void *)destination -> gx_pixelmap_data);
        return GX_SYSTEM_MEMORY_ERROR;
    }

    put = (USHORT *)destination -> gx_pixelmap_data;
    putalpha = (GX_UBYTE *)destination -> gx_pixelmap_aux_data;

    /* Loop through destination's pixel and fill each pixel with
       the interpolation of 4 nearest neighboring pixels.*/
    for (y = 0; y < height; y++)
    {
        for (x = 0; x < width; x++)
        {
            /* Find the original source pixel that the destination pixel conrespond to. */
            xx = (xradio * x) >> 8;
            yy = (yradio * y) >> 8;

            /* The coordinates of the original source pixel are truncate value,
               calucate their distance between the mathematical coordinates. */
            xdiff = (xradio * x) & 0xff;
            ydiff = (yradio * y) & 0xff;

            get = (USHORT *)src -> gx_pixelmap_data;
            get += yy * src -> gx_pixelmap_width;
            get += xx;

            getalpha = (GX_UBYTE *)src -> gx_pixelmap_aux_data;
            getalpha += yy * src -> gx_pixelmap_width;
            getalpha += xx;


            /* Calculate 4 nearest neighboring pixels around the mathematical point of original pixel. */
            neighbor_pixels[0][0] = *get;
            alpha[0] = *getalpha;

            if ((xx < src -> gx_pixelmap_width - 1) && (yy < src -> gx_pixelmap_height - 1))
            {
                neighbor_pixels[0][1] = *(get + 1);
                neighbor_pixels[1][0] = *(get + src -> gx_pixelmap_width);
                neighbor_pixels[1][1] = *(get + src -> gx_pixelmap_width + 1);

                alpha[1] = *(getalpha + 1);
                alpha[2] = *(getalpha + src -> gx_pixelmap_width);
                alpha[3] = *(getalpha + src -> gx_pixelmap_width + 1);
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

                    alpha[1] = alpha[0];
                    alpha[2] = alpha[0];
                    alpha[3] = alpha[0];
                }
                else if (xx == src -> gx_pixelmap_width - 1)
                {
                    /* Handle pixels in right edge.  */
                    neighbor_pixels[0][1] = neighbor_pixels[0][0];
                    neighbor_pixels[1][0] = *(get + src -> gx_pixelmap_width);
                    neighbor_pixels[1][1] = neighbor_pixels[1][0];

                    alpha[1] = alpha[0];
                    alpha[2] = *(getalpha + src -> gx_pixelmap_width);
                    alpha[3] = alpha[2];
                }
                else
                {
                    /* Handle pixels in bottom edge.  */
                    neighbor_pixels[0][1] = *(get + 1);
                    neighbor_pixels[1][0] = neighbor_pixels[0][0];
                    neighbor_pixels[1][1] = neighbor_pixels[0][1];

                    alpha[1] = *(getalpha + 1);
                    alpha[2] = alpha[0];
                    alpha[3] = alpha[1];
                }
            }

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

            *put++ = ASSEMBLECOLOR(red, green, blue);
            *putalpha++ = (GX_UBYTE)alpha[0];
        }
    }

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_16bpp_pixelmap_resize                   PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    565rgb pixelmap resize function that handles uncompress, with or    */
/*    alpha channel.                                                      */
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
/*     _gx_utility_565rgb_pixelmap_alpha_resize                           */
/*     _gx_utility_565rgb_pixelmap_raw_resize                             */
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
UINT _gx_utility_16bpp_pixelmap_resize(GX_PIXELMAP *src, GX_PIXELMAP *destination, INT width, INT height)
{
UINT status;

    if (src -> gx_pixelmap_flags & GX_PIXELMAP_ALPHA)
    {
        /* alpha, no compression */

        status = _gx_utility_16bpp_pixelmap_alpha_resize(src, destination, width, height);
    }
    else
    {
        /* no compression or alpha */

        status = _gx_utility_16bpp_pixelmap_raw_resize(src, destination, width, height);
    }

    return status;
}

