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
/**   Text Input Management (Single Line Text Input)                      */
/**                                                                       */
/**************************************************************************/

/* Include necessary system files.  */
#define GX_SOURCE_CODE

#include "gx_api.h"
#include "gx_utility.h"

#define BI_RGB       0L
#define BI_BITFIELDS 3L

#define DATA_OFFSET  54
typedef struct bitmap_file_header_struct
{
    USHORT filetype;
    UINT   filesize;
    UINT   reserved;
    UINT   offset;
} bitmap_file_header;

typedef struct bmp_info_struct
{
    UINT   bi_Size;
    INT    bi_Width;
    INT    bi_Height;
    USHORT bi_Planes;
    USHORT bi_BitCount;
    UINT   bi_Compression;
    UINT   bi_SizeImage;
    UINT   bi_XPelsPerMeter;
    UINT   bi_YPelsPerMeter;
    UINT   bi_ClrUsed;
    UINT   bi_ClrImportant;
} bmp_info;


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_write_bitmap_header                     PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function create bitmap header and write it to bmp file.        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    canvas                                Canvas control block          */
/*    rectangle                             Rectangle specification       */
/*    write_data                            Write data callback function  */
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
/*    _gx_utility_canvas_to_bmp                                           */
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
static UINT _gx_utility_write_bitmap_header(GX_CANVAS *canvas, GX_RECTANGLE *rect, UINT (*write_data)(GX_UBYTE *byte_data, UINT data_count))
{
bitmap_file_header bmp_fheader;
bmp_info           b_info;
GX_COLOR           color;
GX_COLOR          *palette;
INT                count;
INT                r;
INT                g;
INT                b;
INT                red;
INT                green;
INT                blue;

    memset(&bmp_fheader, 0, sizeof(bitmap_file_header));
    memset(&b_info, 0, sizeof(bmp_info));
    bmp_fheader.filetype = 0x4d42; /* "MB" */
    bmp_fheader.reserved = 0;

    /* calculate data offset */
    bmp_fheader.offset = DATA_OFFSET;

    /* set common datas for bmp info. */
    b_info.bi_Width = rect -> gx_rectangle_right - rect -> gx_rectangle_left + 1;
    b_info.bi_Height = rect -> gx_rectangle_bottom - rect -> gx_rectangle_top + 1;
    b_info.bi_Size = sizeof(bmp_info);
    b_info.bi_Planes = 1;
    b_info.bi_Compression = BI_RGB;
    b_info.bi_XPelsPerMeter = 0;
    b_info.bi_YPelsPerMeter = 0;
    b_info.bi_ClrUsed = 0;
    b_info.bi_ClrImportant = 0;

    switch (canvas -> gx_canvas_display -> gx_display_color_format)
    {
    case GX_COLOR_FORMAT_32ARGB:
    case GX_COLOR_FORMAT_24XRGB:
        b_info.bi_BitCount = 32;
        b_info.bi_SizeImage = (UINT)((b_info.bi_Width << 2) * b_info.bi_Height);
        break;

    case GX_COLOR_FORMAT_1555XRGB:
    case GX_COLOR_FORMAT_4444ARGB:
    case GX_COLOR_FORMAT_565RGB:
        b_info.bi_BitCount = 16;
        b_info.bi_SizeImage = (UINT)((((UINT)(b_info.bi_Width + 1) & 0xfffffffe) << 1) * (UINT)b_info.bi_Height);
        b_info.bi_ClrUsed = 3;
        b_info.bi_ClrImportant = 3;
        b_info.bi_Compression = BI_BITFIELDS;
        /* Three color entries in palette */
        bmp_fheader.offset += 3 * sizeof(GX_COLOR);
        break;

    case GX_COLOR_FORMAT_8BIT_PALETTE:
        if ((canvas -> gx_canvas_display -> gx_display_palette == GX_NULL) ||
            (canvas -> gx_canvas_display -> gx_display_palette_size == 0))
        {
            return GX_FAILURE;
        }
        b_info.bi_BitCount = 8;
        b_info.bi_SizeImage = (UINT)(((UINT)(b_info.bi_Width + 3) & 0xfffffffc) * (UINT)b_info.bi_Height);
        b_info.bi_ClrUsed = canvas -> gx_canvas_display -> gx_display_palette_size;
        b_info.bi_ClrImportant = canvas -> gx_canvas_display -> gx_display_palette_size;
        bmp_fheader.offset += canvas -> gx_canvas_display -> gx_display_palette_size * sizeof(GX_COLOR);
        break;

    case GX_COLOR_FORMAT_8BIT_PACKED_PIXEL:
        b_info.bi_BitCount = 8;
        b_info.bi_SizeImage = (UINT)(((UINT)(b_info.bi_Width + 3) & 0xfffffffc) * (UINT)b_info.bi_Height);
        /* Palette for 332rgb is default to 256. */
        b_info.bi_ClrImportant = 256;
        b_info.bi_ClrUsed = 256;
        bmp_fheader.offset += 256 * sizeof(GX_COLOR);
        break;

    case GX_COLOR_FORMAT_4BIT_GRAY:
        b_info.bi_BitCount = 4;
        b_info.bi_SizeImage = (UINT)((((UINT)(b_info.bi_Width + 7) & 0xfffffff8) >> 1) * (UINT)b_info.bi_Height);
        b_info.bi_ClrUsed = 16;
        b_info.bi_ClrImportant = 16;
        bmp_fheader.offset += 16 * sizeof(GX_COLOR);
        break;

    case GX_COLOR_FORMAT_MONOCHROME:
        b_info.bi_BitCount = 1;
        b_info.bi_SizeImage = (UINT)((((UINT)(b_info.bi_Width + 63) & 0xffffffc0) >> 3) * (UINT)b_info.bi_Height);
        b_info.bi_ClrUsed = 2;
        b_info.bi_ClrImportant = 2;
        bmp_fheader.offset += 2 * sizeof(GX_COLOR);
        break;

    default:
        break;
    }

    /* total file size */
    bmp_fheader.filesize = bmp_fheader.offset + b_info.bi_SizeImage;

    /* write bmp file header */
    write_data((GX_UBYTE *)&bmp_fheader.filetype, sizeof(USHORT));
    write_data((GX_UBYTE *)&bmp_fheader.filesize, sizeof(UINT));
    write_data((GX_UBYTE *)&bmp_fheader.reserved, sizeof(UINT));
    write_data((GX_UBYTE *)&bmp_fheader.offset, sizeof(UINT));

    /* height value is negtive means we can write image data from top to bottom. */
    b_info.bi_Height = -b_info.bi_Height;
    /* write bmp info */
    write_data((GX_UBYTE *)&b_info, sizeof(bmp_info));

    /* I don't want to add a matrix with 256 to store the palette.
       so let's write the palette by colors.*/
    switch (canvas -> gx_canvas_display -> gx_display_color_format)
    {
    case GX_COLOR_FORMAT_565RGB:
        color = 0x0000f800;
        write_data((GX_UBYTE *)&color, sizeof(GX_COLOR));
        color = 0x000007e0;
        write_data((GX_UBYTE *)&color, sizeof(GX_COLOR));
        color = 0x0000001f;
        write_data((GX_UBYTE *)&color, sizeof(GX_COLOR));
        break;

    case GX_COLOR_FORMAT_1555XRGB:
        color = 0x00007c00;
        write_data((GX_UBYTE *)&color, sizeof(GX_COLOR));
        color = 0x000003e0;
        write_data((GX_UBYTE *)&color, sizeof(GX_COLOR));
        color = 0x0000001f;
        write_data((GX_UBYTE *)&color, sizeof(GX_COLOR));
        break;

    case GX_COLOR_FORMAT_4444ARGB:
        color = 0x00000f00;
        write_data((GX_UBYTE *)&color, sizeof(GX_COLOR));
        color = 0x000000f0;
        write_data((GX_UBYTE *)&color, sizeof(GX_COLOR));
        color = 0x0000000f;
        write_data((GX_UBYTE *)&color, sizeof(GX_COLOR));
        break;

    case GX_COLOR_FORMAT_8BIT_PALETTE:
        palette = canvas -> gx_canvas_display -> gx_display_palette;
        count = (INT)(canvas -> gx_canvas_display -> gx_display_palette_size);
        while (count)
        {
            write_data((GX_UBYTE *)palette, sizeof(GX_COLOR));
            palette++;
            count--;
        }
        break;

    case GX_COLOR_FORMAT_8BIT_PACKED_PIXEL:
        for (r = 0; r < 8; r++)
        {
            red = r << 21;
            if (red & 0x200000)
            {
                red |= 0x1f0000;
            }
            for (g = 0; g < 8; g++)
            {
                green = g << 13;
                if (green & 0x2000)
                {
                    green |= 0x1f00;
                }
                for (b = 0; b < 4; b++)
                {
                    blue = b << 6;
                    if (blue & 0x40)
                    {
                        blue |= 0x3f;
                    }
                    color = (GX_COLOR)(red | green | blue);
                    write_data((GX_UBYTE *)&color, sizeof(GX_COLOR));
                }
            }
        }
        break;

    case GX_COLOR_FORMAT_4BIT_GRAY:
        for (r = 0; r <= 0xff; r += 0x11)
        {
            color = (GX_COLOR)((GX_COLOR)((r << 16) | (r << 8) | r) | 0xff000000);
            write_data((GX_UBYTE *)&color, sizeof(GX_COLOR));
        }
        break;

    case GX_COLOR_FORMAT_MONOCHROME:
        color = 0xff000000;
        write_data((GX_UBYTE *)&color, sizeof(GX_COLOR));
        color = 0xffffffff;
        write_data((GX_UBYTE *)&color, sizeof(GX_COLOR));
        break;

    default:
        break;
    }

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_write_bitmap_data_32bpp                 PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function read canvas memory data and write it to bmp file.     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    canvas                                Canvas control block          */
/*    rectangle                             Rectangle specification       */
/*    write_data                            Write data callback function  */
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
/*    _gx_utility_canvas_to_bmp                                           */
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
static void _gx_utility_write_bitmap_data_32bpp(GX_CANVAS *canvas, GX_RECTANGLE *rect, UINT (*write_data)(GX_UBYTE *byte_data, UINT data_count))
{
GX_COLOR *get;
GX_COLOR *get_row;
INT       x;
INT       y;

    get_row = (GX_COLOR *)(canvas -> gx_canvas_memory);
    get_row += canvas -> gx_canvas_x_resolution * rect -> gx_rectangle_top;
    get_row += rect -> gx_rectangle_left;

    for (y = rect -> gx_rectangle_top; y <= rect -> gx_rectangle_bottom; y++)
    {
        get = get_row;
        for (x = rect -> gx_rectangle_left; x <= rect -> gx_rectangle_right; x++)
        {
            write_data((GX_UBYTE *)get, sizeof(GX_COLOR));
            get++;
        }
        get_row += canvas -> gx_canvas_x_resolution;
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_write_bitmap_data_16bpp                 PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function read canvas memory data and write it to bmp file.     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    canvas                                Canvas control block          */
/*    rectangle                             Rectangle specification       */
/*    write_data                            Write data callback function  */
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
/*    _gx_utility_canvas_to_bmp                                           */
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
static void _gx_utility_write_bitmap_data_16bpp(GX_CANVAS *canvas, GX_RECTANGLE *rect, UINT (*write_data)(GX_UBYTE *byte_data, UINT data_count))
{
USHORT *get;
USHORT *get_row;
INT     x;
INT     y;
INT     width;

    get_row = (USHORT *)(canvas -> gx_canvas_memory);
    get_row += canvas -> gx_canvas_x_resolution * rect -> gx_rectangle_top;
    get_row += rect -> gx_rectangle_left;
    width = rect -> gx_rectangle_right - rect -> gx_rectangle_left + 1;

    for (y = rect -> gx_rectangle_top; y <= rect -> gx_rectangle_bottom; y++)
    {
        get = get_row;
        for (x = 0; x < width; x++)
        {
            write_data((GX_UBYTE *)get, sizeof(USHORT));
            get++;
        }
        if (width & 1)
        {
            /* Fix data to 4 bytes aligned. */
            write_data((GX_UBYTE *)get, sizeof(USHORT));
        }
        get_row += canvas -> gx_canvas_x_resolution;
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_write_bitmap_data_8bpp                  PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function read canvas memory data and write it to bmp file.     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    canvas                                Canvas control block          */
/*    rectangle                             Rectangle specification       */
/*    write_data                            Write data callback function  */
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
/*    _gx_utility_canvas_to_bmp                                           */
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
static void _gx_utility_write_bitmap_data_8bpp(GX_CANVAS *canvas, GX_RECTANGLE *rect, UINT (*write_data)(GX_UBYTE *byte_data, UINT data_count))
{
GX_UBYTE *get;
GX_UBYTE *get_row;
INT       x;
INT       y;
INT       width;
INT       fixed_count;

    get_row = (GX_UBYTE *)(canvas -> gx_canvas_memory);
    get_row += canvas -> gx_canvas_x_resolution * rect -> gx_rectangle_top;
    get_row += rect -> gx_rectangle_left;
    width = rect -> gx_rectangle_right - rect -> gx_rectangle_left + 1;
    fixed_count = width & 3;
    if (fixed_count)
    {
        fixed_count = 4 - fixed_count;
    }

    for (y = rect -> gx_rectangle_top; y <= rect -> gx_rectangle_bottom; y++)
    {
        get = get_row;
        for (x = 0; x < width; x++)
        {
            write_data((GX_UBYTE *)get, sizeof(GX_UBYTE));
            get++;
        }

        /* Fix data to 4 bytes aligned. */
        for (x = 0; x < fixed_count; x++)
        {
            write_data((GX_UBYTE *)get, sizeof(GX_UBYTE));
        }
        get_row += canvas -> gx_canvas_x_resolution;
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_write_bitmap_data_4bpp                  PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function read canvas memory data and write it to bmp file.     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    canvas                                Canvas control block          */
/*    rectangle                             Rectangle specification       */
/*    write_data                            Write data callback function  */
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
/*    _gx_utility_canvas_to_bmp                                           */
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
static void _gx_utility_write_bitmap_data_4bpp(GX_CANVAS *canvas, GX_RECTANGLE *rect, UINT (*write_data)(GX_UBYTE *byte_data, UINT data_count))
{
GX_UBYTE *get;
GX_UBYTE *get_row;
GX_UBYTE  color = 0;
INT       x;
INT       y;
INT       fixed_count;
INT       row_count;

    get_row = (GX_UBYTE *)(canvas -> gx_canvas_memory);
    get_row += ((canvas -> gx_canvas_x_resolution + 1) >> 1) * rect -> gx_rectangle_top;
    get_row += rect -> gx_rectangle_left >> 1;
    row_count = rect -> gx_rectangle_right - rect -> gx_rectangle_left + 1;
    row_count = (row_count + 1) >> 1;
    fixed_count = row_count & 3;
    if (fixed_count)
    {
        fixed_count = 4 - fixed_count;
    }

    if (rect -> gx_rectangle_left & 1)
    {
        /* If start pixel is not at the first bit of byte. We need to make it aligned. */
        for (y = rect -> gx_rectangle_top; y <= rect -> gx_rectangle_bottom; y++)
        {
            get = get_row;
            for (x = 0; x < row_count; x++)
            {
                color = (GX_UBYTE)((*get & 0x0f) << 4);
                get++;
                color |= (*get & 0xf0) >> 4;

                write_data((GX_UBYTE *)&color, sizeof(GX_UBYTE));
            }

            /* Fix data to 4 bytes aligned. */
            for (x = 0; x < fixed_count; x++)
            {
                write_data((GX_UBYTE *)&color, sizeof(GX_UBYTE));
            }

            get_row += (canvas -> gx_canvas_x_resolution + 1) >> 1;
        }
    }
    else
    {
        for (y = rect -> gx_rectangle_top; y <= rect -> gx_rectangle_bottom; y++)
        {
            get = get_row;
            for (x = 0; x < row_count; x++)
            {
                write_data((GX_UBYTE *)get, sizeof(GX_UBYTE));
                get++;
            }

            /* Fix data to 4 bytes aligned. */
            for (x = 0; x < fixed_count; x++)
            {
                write_data((GX_UBYTE *)&color, sizeof(GX_UBYTE));
            }
            get_row += (canvas -> gx_canvas_x_resolution + 1) >> 1;
        }
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_write_bitmap_data_monochrome            PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function read canvas memory data and write it to bmp file.     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    canvas                                Canvas control block          */
/*    rectangle                             Rectangle specification       */
/*    write_data                            Write data callback function  */
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
/*    _gx_utility_canvas_to_bmp                                           */
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
static void _gx_utility_write_bitmap_data_monochrome(GX_CANVAS *canvas, GX_RECTANGLE *rect, UINT (*write_data)(GX_UBYTE *byte_data, UINT data_count))
{
GX_UBYTE *get;
GX_UBYTE *get_row;
INT       x;
INT       y;
INT       row_count;
INT       fixed_count;
INT       bits_in_first_byte;
GX_UBYTE  color = 0;

    get_row = (GX_UBYTE *)(canvas -> gx_canvas_memory);
    get_row += ((canvas -> gx_canvas_x_resolution + 7) >> 3) * rect -> gx_rectangle_top;
    get_row += rect -> gx_rectangle_left >> 3;
    row_count = rect -> gx_rectangle_right - rect -> gx_rectangle_left + 1;
    row_count = (row_count + 7) >> 3;
    bits_in_first_byte = rect -> gx_rectangle_left & 7;
    fixed_count = row_count & 3;
    if (fixed_count)
    {
        fixed_count = 4 - fixed_count;
    }

    if (bits_in_first_byte)
    {
        bits_in_first_byte = 8 - (rect -> gx_rectangle_left & 7);

        /* If start pixel is not at the first bit of byte. We need to make it aligned. */
        for (y = rect -> gx_rectangle_top; y <= rect -> gx_rectangle_bottom; y++)
        {
            get = get_row;
            for (x = 0; x < row_count; x++)
            {
                color = (GX_UBYTE)((*get) << (8 - bits_in_first_byte));
                get++;
                color = (GX_UBYTE)(color | ((*get) >> bits_in_first_byte));

                write_data((GX_UBYTE *)&color, sizeof(GX_UBYTE));
            }

            /* Fix data to 4 bytes aligned. */
            for (x = 0; x < fixed_count; x++)
            {
                write_data((GX_UBYTE *)&color, sizeof(GX_UBYTE));
            }

            get_row += (canvas -> gx_canvas_x_resolution + 7) >> 3;
        }
    }
    else
    {
        for (y = rect -> gx_rectangle_top; y <= rect -> gx_rectangle_bottom; y++)
        {
            get = get_row;
            for (x = 0; x < row_count; x++)
            {
                write_data((GX_UBYTE *)get, sizeof(GX_UBYTE));
                get++;
            }

            /* Fix data to 4 bytes aligned. */
            for (x = 0; x < fixed_count; x++)
            {
                write_data((GX_UBYTE *)&color, sizeof(GX_UBYTE));
            }

            get_row += (canvas -> gx_canvas_x_resolution + 7) >> 3;
        }
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_canvas_to_bmp                           PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Covert canvas memory data to bmp file.                              */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    canvas                                Canvas control block          */
/*    rectangle                             Rectangle specification       */
/*    write_data                            Write data callback function  */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_rectangle_define          Define a rectangle            */
/*    _gx_utility_rectangle_inside_detect   Detect rect inside of another */
/*    _gx_utility_write_bitmap_header       Create bmp file header        */
/*    _gx_utility_write_bitmap_data_monochrome                            */
/*    _gx_utility_write_bitmap_data_4bpp                                  */
/*    _gx_utility_write_bitmap_data_8bpp                                  */
/*    _gx_utility_write_bitmap_data_16bpp                                 */
/*    _gx_utility_write_bitmap_data_32bpp   Above functions read canvas   */
/*                                            memory and write data to    */
/*                                            file.                       */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gxe_utility_canvas_to_bmp            Error checking function       */
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
UINT _gx_utility_canvas_to_bmp(GX_CANVAS *canvas, GX_RECTANGLE *rect, UINT (*write_data)(GX_UBYTE *byte_data, UINT data_count))
{
UINT         status;
UINT         pixelmap_format;
GX_RECTANGLE canvas_size;

    _gx_utility_rectangle_define(&canvas_size, 0, 0, (GX_VALUE)(canvas -> gx_canvas_x_resolution - 1), (GX_VALUE)(canvas -> gx_canvas_y_resolution - 1));

    if (!_gx_utility_rectangle_inside_detect(&canvas_size, rect))
    {
        return GX_INVALID_SIZE;
    }

    /* write bmp file header. */
    status = _gx_utility_write_bitmap_header(canvas, rect, write_data);

    if (status == GX_SUCCESS)
    {
        /* write bmp image data*/
        pixelmap_format = canvas -> gx_canvas_display -> gx_display_color_format;

        switch (pixelmap_format)
        {
        case GX_COLOR_FORMAT_MONOCHROME:
            _gx_utility_write_bitmap_data_monochrome(canvas, rect, write_data);
            break;


        case GX_COLOR_FORMAT_4BIT_GRAY:
            _gx_utility_write_bitmap_data_4bpp(canvas, rect, write_data);
            break;

        case GX_COLOR_FORMAT_8BIT_PACKED_PIXEL:
        case GX_COLOR_FORMAT_8BIT_PALETTE:
            _gx_utility_write_bitmap_data_8bpp(canvas, rect, write_data);
            break;

        case GX_COLOR_FORMAT_1555XRGB:
        case GX_COLOR_FORMAT_4444ARGB:
        case GX_COLOR_FORMAT_565RGB:
            _gx_utility_write_bitmap_data_16bpp(canvas, rect, write_data);
            break;

        case GX_COLOR_FORMAT_24XRGB:
        case GX_COLOR_FORMAT_32ARGB:
            _gx_utility_write_bitmap_data_32bpp(canvas, rect, write_data);
            break;

        case GX_COLOR_FORMAT_2BIT_GRAY: /* Not support now. */
        default:
            return GX_NOT_SUPPORTED;
        }
    }

    return status;
}

