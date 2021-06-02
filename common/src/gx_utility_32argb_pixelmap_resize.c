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

#define REDVAL(_c)     (GX_UBYTE)((_c) >> 16)
#define GREENVAL(_c)   (GX_UBYTE)((_c) >> 8)
#define BLUEVAL(_c)    (GX_UBYTE)(_c)

#define ASSEMBLECOLOR(_r, _g, _b) \
    ((GX_COLOR)(((_r) << 16) |    \
                ((_g) << 8)) |    \
                (_b))

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
/*    _gx_utility_32argb_pixelmap_raw_resize              PORTABLE C      */
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
static UINT _gx_utility_32argb_pixelmap_raw_resize(GX_PIXELMAP *src, GX_PIXELMAP *destination, INT width, INT height)
{
/* The pixelmap resize function is implemented from bilinear interpolation
   image scaling algorithm.  */

GX_COLOR *get;
GX_COLOR *put;
INT       xdiff;
INT       ydiff;
INT       xradio;
INT       yradio;
INT       x;
INT       y;
INT       xx;
INT       yy;
GX_COLOR  neighbor_pixels[2][2];
INT       red;
INT       green;
INT       blue;


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
    destination -> gx_pixelmap_data_size = (UINT)(height * width) * sizeof(GX_COLOR);

    /* Allocate memory for destination pixelmap.  */
    destination -> gx_pixelmap_data = (GX_UBYTE *)_gx_system_memory_allocator(destination -> gx_pixelmap_data_size);

    if (destination -> gx_pixelmap_data == GX_NULL)
    {
        return GX_SYSTEM_MEMORY_ERROR;
    }

    put = (GX_COLOR *)destination -> gx_pixelmap_data;

    /* Loop throught destination's pixels and fill each pixel with the interpolation of
       4 neighboring pixels. */
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

            get = (GX_COLOR *)src -> gx_pixelmap_data;
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
                    /* Handle the bottom right pixel.  */
                    neighbor_pixels[0][1] = neighbor_pixels[0][0];
                    neighbor_pixels[1][0] = neighbor_pixels[0][0];
                    neighbor_pixels[1][1] = neighbor_pixels[0][0];
                }
                else if (xx == src -> gx_pixelmap_width - 1)
                {
                    /* Handle the pixel in right-most edge. */
                    neighbor_pixels[0][1] = neighbor_pixels[0][0];
                    neighbor_pixels[1][0] = *(get + src -> gx_pixelmap_width);
                    neighbor_pixels[1][1] = neighbor_pixels[1][0];
                }
                else
                {
                    /* Handle the pixel in bottom edge. */
                    neighbor_pixels[0][1] = *(get + 1);
                    neighbor_pixels[1][0] = neighbor_pixels[0][0];
                    neighbor_pixels[1][1] = neighbor_pixels[0][1];
                }
            }

            /* Calulate pixel values by interpolating 4 neighboring pixels. */
            red = (REDVAL(neighbor_pixels[0][0]) * (256 - xdiff) * (256 - ydiff) + \
                   REDVAL(neighbor_pixels[0][1]) * xdiff * (256 - ydiff) +         \
                   REDVAL(neighbor_pixels[1][0]) * ydiff * (256 - xdiff) +         \
                   REDVAL(neighbor_pixels[1][1]) * xdiff * ydiff) >> 16;

            green = (GREENVAL(neighbor_pixels[0][0]) * (256 - xdiff) * (256 - ydiff) + \
                     GREENVAL(neighbor_pixels[0][1]) * xdiff * (256 - ydiff) +         \
                     GREENVAL(neighbor_pixels[1][0]) * ydiff * (256 - xdiff) +         \
                     GREENVAL(neighbor_pixels[1][1]) * xdiff * ydiff) >> 16;

            blue = (BLUEVAL(neighbor_pixels[0][0]) * (256 - xdiff) * (256 - ydiff) + \
                    BLUEVAL(neighbor_pixels[0][1]) * xdiff * (256 - ydiff) +         \
                    BLUEVAL(neighbor_pixels[1][0]) * ydiff * (256 - xdiff) +         \
                    BLUEVAL(neighbor_pixels[1][1]) * xdiff * ydiff) >> 16;

            red = BYTE_RANGE(red);
            green = BYTE_RANGE(green);
            blue = BYTE_RANGE(blue);

            *put++ = ASSEMBLECOLOR((GX_COLOR)red, (GX_COLOR)green, (GX_COLOR)blue);
        }
    }

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_32argb_pixelmap_alpha_resize            PORTABLE C      */
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
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*  06-02-2021     Kenneth Maxwell          Modified comment(s),          */
/*                                            removed unused variable     */
/*                                            assignment,                 */
/*                                            resulting in version 6.1.7  */
/*                                                                        */
/**************************************************************************/
static UINT _gx_utility_32argb_pixelmap_alpha_resize(GX_PIXELMAP *src, GX_PIXELMAP *destination, INT width, INT height)
{
/* The pixelmap resize function is implemented from bilinear interpolation
   image scaling algorithm.  */

GX_COLOR *get;
GX_COLOR *put;
INT       xdiff;
INT       ydiff;
INT       xradio;
INT       yradio;
INT       x;
INT       y;
INT       xx;
INT       yy;
GX_COLOR  neighbor_pixels[2][2] = {{0, 0}, {0, 0}};
INT       red;
INT       green;
INT       blue;
INT       alpha;

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
    destination -> gx_pixelmap_data_size = (UINT)(height * width) * sizeof(GX_COLOR);

    /* Allocate memory to load pixelmap data. */
    destination -> gx_pixelmap_data = (GX_UBYTE *)_gx_system_memory_allocator(destination -> gx_pixelmap_data_size);

    if (destination -> gx_pixelmap_data == GX_NULL)
    {
        return GX_SYSTEM_MEMORY_ERROR;
    }

    put = (GX_COLOR *)destination -> gx_pixelmap_data;

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
            xdiff = xradio * x - (xx << 8);
            ydiff = yradio * y - (yy << 8);

            get = (GX_COLOR *)src -> gx_pixelmap_data;
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
                    /* Hanle pixels in right bottom corner. */
                    neighbor_pixels[0][1] = neighbor_pixels[0][0];
                    neighbor_pixels[1][0] = neighbor_pixels[0][0];
                    neighbor_pixels[1][1] = neighbor_pixels[0][0];
                }
                else if (xx == src -> gx_pixelmap_width - 1)
                {
                    /* Handle pixels in right edge. */
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
            red = (INT)((REDVAL(neighbor_pixels[0][0]) * (neighbor_pixels[0][0] >> 24) * (256 - (ULONG)xdiff) * (256 - (ULONG)ydiff) + \
                         REDVAL(neighbor_pixels[0][1]) * (neighbor_pixels[0][1] >> 24) * (ULONG)xdiff * (256 - (ULONG)ydiff) +         \
                         REDVAL(neighbor_pixels[1][0]) * (neighbor_pixels[1][0] >> 24) * (ULONG)ydiff * (256 - (ULONG)xdiff) +         \
                         REDVAL(neighbor_pixels[1][1]) * (neighbor_pixels[1][1] >> 24) * (ULONG)xdiff * (ULONG)ydiff) >> 16);

            green = (INT)((GREENVAL(neighbor_pixels[0][0]) * (neighbor_pixels[0][0] >> 24) * (256 - (ULONG)xdiff) * (256 - (ULONG)ydiff) + \
                           GREENVAL(neighbor_pixels[0][1]) * (neighbor_pixels[0][1] >> 24) * (ULONG)xdiff * (256 - (ULONG)ydiff) +         \
                           GREENVAL(neighbor_pixels[1][0]) * (neighbor_pixels[1][0] >> 24) * (ULONG)ydiff * (256 - (ULONG)xdiff) +         \
                           GREENVAL(neighbor_pixels[1][1]) * (neighbor_pixels[1][1] >> 24) * (ULONG)xdiff * (ULONG)ydiff) >> 16);

            blue = (INT)((BLUEVAL(neighbor_pixels[0][0]) * (neighbor_pixels[0][0] >> 24) * (256 - (ULONG)xdiff) * (256 - (ULONG)ydiff) + \
                          BLUEVAL(neighbor_pixels[0][1]) * (neighbor_pixels[0][1] >> 24) * (ULONG)xdiff * (256 - (ULONG)ydiff) +         \
                          BLUEVAL(neighbor_pixels[1][0]) * (neighbor_pixels[1][0] >> 24) * (ULONG)ydiff * (256 - (ULONG)xdiff) +         \
                          BLUEVAL(neighbor_pixels[1][1]) * (neighbor_pixels[1][1] >> 24) * (ULONG)xdiff * (ULONG)ydiff) >> 16);

            alpha = (INT)(((neighbor_pixels[0][0] >> 24) * (256 - (ULONG)xdiff) * (256 - (ULONG)ydiff) + \
                           (neighbor_pixels[0][1] >> 24) * (ULONG)xdiff * (256 - (ULONG)ydiff) +         \
                           (neighbor_pixels[1][0] >> 24) * (ULONG)ydiff * (256 - (ULONG)xdiff) +         \
                           (neighbor_pixels[1][1] >> 24) * (ULONG)xdiff * (ULONG)ydiff) >> 16);

            if (alpha)
            {
                red /= alpha;
                green /= alpha;
                blue /= alpha;
            }

            alpha = BYTE_RANGE(alpha);
            red = BYTE_RANGE(red);
            green = BYTE_RANGE(green);
            blue = BYTE_RANGE(blue);

            *put++ = ASSEMBLECOLOR((ULONG)red, (ULONG)green, (ULONG)blue) + ((ULONG)alpha << 24);
        }
    }

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_32argb_pixelmap_resize                  PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    32argb pixelmap resize function that handles uncompress, with or    */
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
/*     _gx_utility_32argb_pixelmap_alpha_resize                           */
/*     _gx_utility_32argb_pixelmap_raw_resize                             */
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
UINT _gx_utility_32argb_pixelmap_resize(GX_PIXELMAP *src, GX_PIXELMAP *destination, INT width, INT height)
{
UINT status;

    if (src -> gx_pixelmap_flags & GX_PIXELMAP_ALPHA)
    {
        /* alpha, no compression */

        status = _gx_utility_32argb_pixelmap_alpha_resize(src, destination, width, height);
    }
    else
    {
        /* no compression or alpha */

        status = _gx_utility_32argb_pixelmap_raw_resize(src, destination, width, height);
    }

    return status;
}

