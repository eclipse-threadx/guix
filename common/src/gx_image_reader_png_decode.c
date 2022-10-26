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
#include "gx_system.h"
#include "gx_utility.h"
#include "gx_image_reader.h"


#if defined(GX_SOFTWARE_DECODER_SUPPORT)
static UINT _bit_buffer;
static UINT _bit_count;

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_png_crc_table_make                 PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function creates crc table that speed the crc calculation.     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    png                                   PNG control block             */
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
/*    _gx_image_reader_png_decode                                         */
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
static VOID _gx_image_reader_png_crc_table_make(GX_PNG *png)
{
UINT crc;
UINT n;
UINT k;

    for (n = 0; n < 256; n++)
    {
        crc = n;
        for (k = 0; k < 8; k++)
        {
            if (crc & 1)
            {
                crc = 0xedb88320 ^ (crc >> 1);
            }
            else
            {
                crc = crc >> 1;
            }
        }
        png -> gx_png_crc_table[n] = crc;
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_png_crc_get                      PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function calculates Cyclic Redundancy Check of a pecified      */
/*    data stream.                                                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    png                                   PNG control block             */
/*    crc                                   Generated crc value           */
/*    len                                   Data length                   */
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
/*    _gx_image_reader_png_decode                                         */
/*    _gx_image_reader_png_bits_get                                       */
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
static VOID _gx_image_reader_png_crc_get(GX_PNG *png, UINT *crc, UINT len)
{
UINT      n;
GX_UBYTE *buf = png -> gx_png_data + png -> gx_png_data_index;
UINT      crc_cal = (*crc);

    for (n = 0; n < len; n++)
    {
        crc_cal = png -> gx_png_crc_table[(crc_cal ^ buf[n]) & 0xff] ^ (crc_cal >> 8);
    }

    (*crc) = crc_cal ^ 0xffffffff;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_png_4bytes_read                    PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function reads 4 bytes and packed it as an INT value.          */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    png                                   PNG control block             */
/*    value                                 Retrieved INT value           */
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
/*    _gx_image_reader_png_decode                                         */
/*    _gx_image_reader_png_bits_get                                       */
/*    _gx_image_reader_png_IHDR_chunk_read                                */
/*    _gx_image_reader_png_gAMA_chunk_read                                */
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
static VOID _gx_image_reader_png_4bytes_read(GX_PNG *png, INT *value)
{
INT get_value;

    get_value = (png -> gx_png_data[png -> gx_png_data_index] << 24);
    png -> gx_png_data_index++;

    get_value |= (png -> gx_png_data[png -> gx_png_data_index] << 16);
    png -> gx_png_data_index++;

    get_value |= (png -> gx_png_data[png -> gx_png_data_index] << 8);
    png -> gx_png_data_index++;

    get_value |= png -> gx_png_data[png -> gx_png_data_index];
    png -> gx_png_data_index++;

    (*value) = get_value;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_png_chunk_type_read                PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function reads the type information of a chunk.                */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    png                                   PNG control block             */
/*    chunk_type                            Retrieved chunk type info     */
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
/*    _gx_image_reader_png_bits_get                                       */
/*    _gx_image_reader_png_decode                                         */
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
static VOID _gx_image_reader_png_chunk_type_read(GX_PNG *png, CHAR *chunk_type)
{
    chunk_type[0] = (CHAR)png -> gx_png_data[png -> gx_png_data_index++];
    chunk_type[1] = (CHAR)png -> gx_png_data[png -> gx_png_data_index++];
    chunk_type[2] = (CHAR)png -> gx_png_data[png -> gx_png_data_index++];
    chunk_type[3] = (CHAR)png -> gx_png_data[png -> gx_png_data_index++];
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_png_bits_get                       PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function Extract a specified number of bits from PNG data      */
/*    stream and advance the read pointer of the PNG data stream.         */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    png                                   PNG control block             */
/*    num_of_bits                           number of bits to retrieve    */
/*    return_value                          Retrieved bits data           */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_image_reader_png_4bytes_read      Read 4 bytes and pack it to a */
/*                                            INT type value              */
/*    _gx_image_reader_png_crc_get          Calculate crc value with      */
/*                                            specified data              */
/*    _gx_image_reader_png_chunk_type_read  Read type information         */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_image_reader_png_clen_huffman_read                              */
/*    _gx_image_reader_png_ll_huffman_read                                */
/*    _gx_image_reader_png_huffcode_decode                                */
/*    _gx_image_reader_png_IDAT_chunk_read                                */
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
static UINT _gx_image_reader_png_bits_get(GX_PNG *png, UINT num_of_bits, UINT *return_value)
{
GX_UBYTE get_byte;
INT      checksum;
CHAR     chunk_type[4];
INT      index = png -> gx_png_trunk_end_index;

    while (_bit_count < num_of_bits)
    {
        if (png -> gx_png_data_index >= png -> gx_png_data_size)
        {
            return GX_FAILURE;
        }

        get_byte = png -> gx_png_data[png -> gx_png_data_index++];

        _bit_buffer |= (UINT)(get_byte << (UINT)_bit_count);
        _bit_count += 8;

        if (png -> gx_png_data_index >= index)
        {
            /* data_len*/
            _gx_image_reader_png_4bytes_read(png, &checksum);

            if (png -> gx_png_trunk_crc != (UINT)checksum)
            {
                return GX_FAILURE;
            }

            _gx_image_reader_png_4bytes_read(png, &index);

            png -> gx_png_trunk_crc = 0xffffffff;

            _gx_image_reader_png_crc_get(png, &png -> gx_png_trunk_crc, (UINT)(4 + index));

            _gx_image_reader_png_chunk_type_read(png, chunk_type);

            if (strncmp(chunk_type, "IDAT", 4) == 0)
            {
                index += png -> gx_png_data_index;
                png -> gx_png_trunk_end_index = index;
            }
        }
    }

    (*return_value) = _bit_buffer;

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_png_bits_revert                    PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function reorders bits stream in opposite order.               */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    bits                                  Variable contain bits stream  */
/*    bits_len                              The number of bits to revert  */
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
/*    _gx_image_reader_png_dynamic_huffman_code_find                      */
/*    _gx_image_reader_png_fixed_ll_huffman_code_find                     */
/*    _gx_image_reader_png_huffcode_decode                                */
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
static void _gx_image_reader_png_bits_revert(UINT *bits, UINT bits_len)
{
UINT reverted = 0;
UINT i;

    for (i = 0; i < bits_len; i++)
    {
        reverted |= ((*bits) & 0x1) << (bits_len - i - 1);
        (*bits) >>= 1;
    }

    (*bits) = reverted;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_png_bits_skip                      PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function skips bits from tempory PNG data buffer.              */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    num_of_skip_bits                      The number of bits to skip    */
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
/*    _gx_image_reader_png_clen_huffman_read                              */
/*    _gx_image_reader_png_huffcode_decode                                */
/*    _gx_image_reader_png_tRNS_chunk_read                                */
/*    _gx_image_reader_png_IDAT_chunk_read                                */
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
static VOID _gx_image_reader_png_bits_skip(UINT num_of_skip_bits)
{
    _bit_buffer >>= num_of_skip_bits;

    _bit_count -= num_of_skip_bits;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_png_IHDR_chunk_read                PORTABLE C      */
/*                                                           6.2.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function reads in IHDR chunk information.                      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    png                                   PNG control block             */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_display_driver_png_4bytes_read    Read 4 bytes and pack it to a */
/*                                            INT type value              */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_image_reader_png_decode                                         */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            added data boundary check,  */
/*                                            resulting in version 6.1    */
/*  10-31-2022     Ting Zhu                 Modified comment(s),          */
/*                                            added invalid value check,  */
/*                                            resulting in version 6.2.0  */
/*                                                                        */
/**************************************************************************/
static UINT _gx_image_reader_png_IHDR_chunk_read(GX_PNG *png)
{
    /* The IHDR chunk contains:
        Width: 4 bytes
        Height: 4 bytes
        Bit depth: 1 byte
        Color type: 1 byte
        Compression method: 1 byte
        Filter method: 1 byte
        Interlace method: 1 byte */

    if (png -> gx_png_data_index + 13 > png -> gx_png_trunk_end_index)
    {
        return GX_INVALID_SIZE;
    }

    _gx_image_reader_png_4bytes_read(png, &png -> gx_png_width);
    _gx_image_reader_png_4bytes_read(png, &png -> gx_png_height);

    /* Limited max png width to 14 bits. */
    if ((png -> gx_png_width < 0) || (png -> gx_png_width > GX_MAX_PIXELMAP_RESOLUTION))
    {
        return GX_INVALID_WIDTH;
    }

    /* Limited max png height to 14 bits. */
    if ((png -> gx_png_height < 0) || (png -> gx_png_height > GX_MAX_PIXELMAP_RESOLUTION))
    {
        return GX_INVALID_HEIGHT;
    }

    /* Bits per sample or per palette index, valid values are 1, 2 ,4, 8, 16. */
    png -> gx_png_bit_depth = png -> gx_png_data[png -> gx_png_data_index++];

    switch (png -> gx_png_bit_depth)
    {
    case 1:
    case 2:
    case 4:
    case 8:
    case 16:
        break;

    default:
        return GX_INVALID_FORMAT;
    }

    /* 0 gray sample
       2 RGB tripple
       3 palette index
       4 gray sample followed by an alpha sample
       6 RGB triple followed by an alpha sample */
    png -> gx_png_color_type = png -> gx_png_data[png -> gx_png_data_index++];

    switch (png -> gx_png_color_type)
    {
    case 0:
    /* Each pixel is agrayscale sample. */
    case 3:
        /* Each pixel is a palette index, a PLTE chunk must appear. */
        png -> gx_png_bpp = png -> gx_png_bit_depth;
        break;
    case 2:
        /* Each pixel is an RGB triple. */
        png -> gx_png_bpp = (GX_UBYTE)(png -> gx_png_bit_depth * 3);
        break;

    case 4:
        /* Each pixel is a grayscale sample, followed by an alpha sample. */
        png -> gx_png_bpp = (GX_UBYTE)(png -> gx_png_bit_depth * 2);
        break;
    case 6:
        /* Each pixel is an R,G,B triple, followed by an alpha sample. */
        png -> gx_png_bpp = (GX_UBYTE)(png -> gx_png_bit_depth * 4);
        break;

    default:
        return GX_FAILURE;
    }

    /* At present, only method 0 is defined. */
    png -> gx_png_compression_method = png -> gx_png_data[png -> gx_png_data_index++];

    /* At present, only method 0 is defined. */
    png -> gx_png_filter_method = png -> gx_png_data[png -> gx_png_data_index++];

    if ((png -> gx_png_compression_method != 0) || (png -> gx_png_filter_method != 0))
    {
        /* Invalid value. */
        return GX_FAILURE;
    }

    /* 0 no interlace
       1 Adam7 interlace */
    png -> gx_png_interlace_method = png -> gx_png_data[png -> gx_png_data_index++];

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_png_dynamic_huffman_code_find      PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function retrieves code value from dynamic huffman table.      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    png                                   PNG control block             */
/*    huffman_bits_count                    Array records codes number for*/
/*                                          every code len                */
/*    scan_buffer                           Buffer to search from         */
/*    bit_len                               Retrieved Huffman Code Length */
/*    code_value                            Retrieved Huffman Code        */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Status code                                                         */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_image_reader_png_bits_revert      Reorder bits stream in        */
/*                                            opposite order              */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_image_reader_png_huffcode_decode                                */
/*    _gx_image_reader_png_ll_huffman_read                                */
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
static UINT _gx_image_reader_png_dynamic_huffman_code_find(INT  *huffman_table,
                                                           UINT  *huffman_bits_count,
                                                           UINT   scan_buffer,
                                                           UINT  *bit_len,
                                                           UINT *code_value)
{
UINT i_bit;
UINT bit_count;
UINT code;
UINT code_cal = 0;
UINT code_index = 0;

    for (i_bit = 1; i_bit < 16; i_bit++)
    {
        bit_count = huffman_bits_count[i_bit] - huffman_bits_count[i_bit - 1];

        if (bit_count)
        {
            code = (scan_buffer) << (32 - i_bit);
            code = (code) >> (32 - i_bit);

            _gx_image_reader_png_bits_revert(&code, i_bit);

            if (code <= code_cal + bit_count - 1)
            {
                *bit_len = i_bit;
                *code_value = (UINT)huffman_table[code_index + code - code_cal];

                return GX_SUCCESS;
            }
            else
            {
                code_index += bit_count;
                code_cal += bit_count;
            }
        }

        code_cal <<= 1;
    }

    return GX_NOT_FOUND;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_png_fixed_ll_huffman_code_find     PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function retrieves code value from fixed huffman table.        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    scan_buffer                           Buffer that contains Huffman  */
/*                                          code                          */
/*    bit_len                               Retrieved Huffman Code length */
/*    code_value                            Retrieved code value of the   */
/*                                          matched Huffman Code          */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_image_reader_png_bits_revert      Reorder bits stream in        */
/*                                            opposite order              */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_image_reader_png_huffcode_decode                                */
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
static UINT _gx_image_reader_png_fixed_ll_huffman_code_find(UINT scan_buffer,
                                                            UINT *bit_len,
                                                            UINT *code_value)
{
UINT i_bit;
UINT code;

    for (i_bit = 7; i_bit <= 9; i_bit++)
    {
        code = scan_buffer << (32 - i_bit);
        code = code >> (32 - i_bit);

        _gx_image_reader_png_bits_revert(&code, i_bit);

        if (i_bit == 7)
        {
            if (code <= 0x17)
            {
                *bit_len = 7;
                *code_value = 256 + code;
                break;
            }
        }
        else if (i_bit == 8)
        {
            if (code <= 0xbf)
            {
                /* code >= 0x30. */
                *bit_len = 8;
                *code_value = code - 0x30;
                break;
            }
            else if (code <= 0xc7)
            {
                /* code >= 0xc0. */
                *bit_len = 8;
                *code_value = 280 + code - 0xc0;
                break;
            }
        }
        else
        {
            /* code >= 0x190 && code <= 0x1ff. */
            *bit_len = 9;
            *code_value = 144 + code - 0x190;
        }
    }

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_png_clen_huffman_read              PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function reads code length huffman table.                      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    png                                   PNG control block             */
/*    hclen                                 The number of "length" codes  */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_image_reader_png_bits_get         Extract bits from PNG data    */
/*                                            stream                      */
/*    _gx_image_reader_png_bits_skip        Skip bits from PNG data stream*/
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_image_reader_png_IDAT_chunk_read                                */
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
static UINT _gx_image_reader_png_clen_huffman_read(GX_PNG *png, UINT hclen)
{

INT  code_value[19] = {16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15};
INT  code_len[19] = {0};
UINT len;
UINT i;
INT  pos[16];

    for (i = 0; i < hclen; i++)
    {
        if (_gx_image_reader_png_bits_get(png, 3, &len) != GX_SUCCESS)
        {
            return GX_FAILURE;
        }
        _gx_image_reader_png_bits_skip(3);
        len &= 0x7;

        /* record code len for code len alphabet */
        code_len[code_value[i]] = (INT)len;

        /* record codes number for every code len */
        if (len)
        {
            png -> gx_png_huffman_clen_bits_count[len]++;
        }
    }

    for (i = 1; i < 16; i++)
    {
        png -> gx_png_huffman_clen_bits_count[i] += png -> gx_png_huffman_clen_bits_count[i - 1];
    }

    memset(pos, -1, 16 * sizeof(INT));

    for (i = 0; i < 19; i++)
    {
        if (code_len[i])
        {
            if (pos[code_len[i]] == -1)
            {
                pos[code_len[i]] = png -> gx_png_huffman_clen_bits_count[code_len[i] - 1];
            }
            png -> gx_png_huffman_clen_table[pos[code_len[i]]++] = (INT)i;
        }
    }

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_png_ll_huffman_read                PORTABLE C      */
/*                                                           6.2.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function reads in literal and length huffman table.            */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    png                                   PNG control block.            */
/*    hlit                                  The number of "literal" codes */
/*    hdist                                 The number of "distance" codes*/
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_image_reader_png_bits_get         Extract bits from PNG data    */
/*                                            stream                      */
/*    _gx_image_reader_png_bits_skip        Skip bits from PNG data stream*/
/*    _gx_image_reader_png_dynamic_huffman_code_find                      */
/*                                          Retrieve code value from      */
/*                                            dynamic huffman table       */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_image_reader_png_IDAT_chunk_read                                */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*  06-02-2021     Ting Zhu                 Modified comment(s),          */
/*                                            added invalid value check,  */
/*                                            resulting in version 6.1.7  */
/*  10-31-2022     Ting Zhu                 Modified comment(s),          */
/*                                            added invalid value check,  */
/*                                            resulting in version 6.2.0  */
/*                                                                        */
/**************************************************************************/
static UINT _gx_image_reader_png_ll_huffman_read(GX_PNG *png, UINT hlit, UINT hdist)
{
UINT scan_buffer;
UINT bit_len;
UINT code_value;
UINT repeat;
UINT i;
UINT count = 0;
INT  pos[16];
INT  index;

    memset(png -> gx_png_huffman_lit_code_len, 0, sizeof(INT) * GX_PNG_HUFFMAN_LIT_CODE_LEN_TABLE_SIZE);
    memset(png -> gx_png_huffman_dist_code_len, 0, sizeof(INT) * GX_PNG_HUFFMAN_DIST_CODE_LEN_TABLE_SIZE);

    for (i = 0; i < hlit + hdist;)
    {
        count++;
        if (_gx_image_reader_png_bits_get(png, 16, &scan_buffer) != GX_SUCCESS)
        {
            return GX_FAILURE;
        }

        if (_gx_image_reader_png_dynamic_huffman_code_find(png -> gx_png_huffman_clen_table,
                                                           (UINT *)png -> gx_png_huffman_clen_bits_count,
                                                           scan_buffer,
                                                           &bit_len,
                                                           &code_value) == 0)
        {
            _gx_image_reader_png_bits_skip(bit_len);

            if (code_value <= 15)
            {
                /* Represent code lengths of 0-15 */
                if (i < hlit)
                {
                    png -> gx_png_huffman_lit_code_len[i] = (INT)code_value;

                    png -> gx_png_huffman_lit_bits_count[code_value]++;
                }
                else
                {
                    png -> gx_png_huffman_dist_code_len[i - hlit] = (INT)code_value;
                    png -> gx_png_huffman_dist_bits_count[code_value]++;
                }

                i++;
            }
            else
            {
                if (code_value == 16)
                {
                    /* repeat previous */
                    /* 2 bits repeat length*/
                    _gx_image_reader_png_bits_get(png, 2, &repeat);
                    _gx_image_reader_png_bits_skip(2);
                    repeat &= 0x3;
                    repeat += 3;

                    if (i < 1)
                    {
                        return GX_FALSE;
                    }
                    else if ((i - 1) < hlit)
                    {
                        code_value = (UINT)png -> gx_png_huffman_lit_code_len[i - 1];
                    }
                    else
                    {
                        code_value = (UINT)png -> gx_png_huffman_dist_code_len[i - 1 - hlit];
                    }

                    if (i < hlit)
                    {
                        png -> gx_png_huffman_lit_bits_count[code_value] += (INT)repeat;

                        while (repeat--)
                        {
                            png -> gx_png_huffman_lit_code_len[i++] = (INT)code_value;
                        }
                    }
                    else
                    {
                        png -> gx_png_huffman_dist_bits_count[code_value] += (INT)repeat;

                        while (repeat--)
                        {
                            png -> gx_png_huffman_dist_code_len[i - hlit] = (INT)code_value;
                            i++;
                        }
                    }
                }
                else
                {
                    if (code_value == 17)
                    {
                        /* repeat 0 */
                        /* 3 bits repeat length */
                        _gx_image_reader_png_bits_get(png, 3, &repeat);
                        _gx_image_reader_png_bits_skip(3);
                        repeat &= 0x7;
                        repeat += 3;
                    }
                    else
                    {
                        /* code_value = 18. */
                        /* repeat 0*/
                        /* 7 bits repeat length */
                        _gx_image_reader_png_bits_get(png, 7, &repeat);
                        _gx_image_reader_png_bits_skip(7);
                        repeat &= 0x7f;
                        repeat += 11;
                    }

                    if (i < hlit)
                    {
                        memset(png -> gx_png_huffman_lit_code_len + i, 0, repeat * sizeof(INT));
                    }
                    else
                    {
                        memset(png -> gx_png_huffman_dist_code_len + i - hlit, 0, repeat * sizeof(INT));
                    }

                    i += repeat;
                }
            }
        }
        else
        {
            /*error*/
            return GX_FAILURE;
        }
    }

    png -> gx_png_huffman_lit_bits_count[0] = 0;
    png -> gx_png_huffman_dist_bits_count[0] = 0;

    for (i = 2; i <= 16; i++)
    {
        png -> gx_png_huffman_lit_bits_count[i] += png -> gx_png_huffman_lit_bits_count[i - 1];
        png -> gx_png_huffman_dist_bits_count[i] += png -> gx_png_huffman_dist_bits_count[i - 1];
    }

    memset(pos, -1, 16 * sizeof(INT));

    for (i = 0; i < hlit; i++)
    {
        index = png -> gx_png_huffman_lit_code_len[i];
        if (index)
        {
            if (pos[index] == -1)
            {
                pos[index] = png -> gx_png_huffman_lit_bits_count[index - 1];
            }
            png -> gx_png_huffman_lit_table[pos[index]++] = (INT)i;
        }
    }

    memset(pos, -1, 16 * sizeof(INT));

    for (i = 0; i < hdist; i++)
    {
        index = png -> gx_png_huffman_dist_code_len[i];
        if (index)
        {
            if (pos[index] == -1)
            {
                pos[index] = png -> gx_png_huffman_dist_bits_count[index - 1];
            }

            if (pos[index] >= GX_PNG_HUFFMAN_DIST_TABLE_SIZE)
            {
                /* Invalid data. */
                return GX_FAILURE;
            }

            png -> gx_png_huffman_dist_table[pos[index]++] = (INT)i;
        }
    }

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_png_huffcode_decode                PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function decodes the Huffman codes.                            */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    png                                   PNG control block.            */
/*    dynamic                               True, use dynamic huff table  */
/*                                          False, use fixed huff table   */
/*    decoded_data_size                     Expected decoded data size    */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_image_reader_png_bits_get         Extract bits from PNG data    */
/*                                            stream                      */
/*    _gx_image_reader_png_bits_skip        Skip bits from PNG data stream*/
/*    _gx_image_reader_png_dynamic_huffman_code_find                      */
/*                                          Retrieve code value from      */
/*                                            dynamic huffman table       */
/*    _gx_image_reader_png_fixed_ll_huffman_code_find                     */
/*                                          Retrieve code value from      */
/*                                            fixed huffman table         */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_image_reader_png_IDAT_chunk_read                                */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            added data boundary check,  */
/*                                            fixed gcc compile warning,  */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
static UINT _gx_image_reader_png_huffcode_decode(GX_PNG *png, GX_BOOL dynamic, UINT decoded_data_size)
{
UINT scan_buffer;
UINT bit_len = 0;
UINT code_value = 300;
UINT length;
UINT distance;
UINT extra_bits;
UINT copy_len;

    while (1)
    {
        if (_gx_image_reader_png_bits_get(png, 16, &scan_buffer) != GX_SUCCESS)
        {
            return GX_FAILURE;
        }

        /* Decode literal/length value from input stream */
        if (dynamic)
        {
            _gx_image_reader_png_dynamic_huffman_code_find(png -> gx_png_huffman_lit_table,
                                                           (UINT *)png -> gx_png_huffman_lit_bits_count,
                                                           scan_buffer,
                                                           &bit_len,
                                                           &code_value);
        }
        else
        {
            _gx_image_reader_png_fixed_ll_huffman_code_find(scan_buffer, &bit_len, &code_value);
        }

        _gx_image_reader_png_bits_skip(bit_len);

        if (code_value < 256)
        {
            if ((UINT)(png -> gx_png_decoded_data_len + 1) > decoded_data_size)
            {
                /* Decoded data size exceed allocated buffer size. */
                return GX_FAILURE;
            }

            png -> gx_png_decoded_data[png -> gx_png_decoded_data_len++] = (GX_UBYTE)code_value;
        }
        else if (code_value == 256)
        {
            /* End of the block */
            break;
        }
        else
        {
            if (code_value < 265)
            {
                /* 0 extra bits */
                length = 3 + code_value - 257;
            }
            else if (code_value < 285)
            {
                extra_bits = 1 + (code_value - 265) / 4;
                _gx_image_reader_png_bits_get(png, extra_bits, &length);
                _gx_image_reader_png_bits_skip(extra_bits);
                length <<= (32 - extra_bits);
                length >>= (32 - extra_bits);

                length += 3u + (1u << (extra_bits + 2)) + ((1u << extra_bits) * ((code_value - 265) & 0x3));
            }
            else if (code_value == 285)
            {
                /* 0 extra bits */
                length = 258;
            }
            else
            {
                /* This should not happen. */
                return GX_FAILURE;
            }

            if (dynamic)
            {
                _gx_image_reader_png_bits_get(png, 16, &scan_buffer);

                /*decode distance from input stream */
                _gx_image_reader_png_dynamic_huffman_code_find(png -> gx_png_huffman_dist_table,
                                                               (UINT *)png -> gx_png_huffman_dist_bits_count,
                                                               scan_buffer,
                                                               &bit_len,
                                                               &code_value);

                _gx_image_reader_png_bits_skip(bit_len);
            }
            else
            {
                /* Decodes 5-bits distance codes. */
                _gx_image_reader_png_bits_get(png, 5, &scan_buffer);
                _gx_image_reader_png_bits_skip(5);
                code_value = scan_buffer & 0x1f;
                _gx_image_reader_png_bits_revert(&code_value, 5);
            }

            if (code_value < 4)
            {
                distance = 1 + code_value;
            }
            else if (code_value < 30)
            {
                extra_bits = 1 + (code_value - 4) / 2;

                _gx_image_reader_png_bits_get(png, extra_bits, &distance);
                _gx_image_reader_png_bits_skip(extra_bits);

                distance <<= (32 - extra_bits);
                distance >>= (32 - extra_bits);

                distance += 1u + (1u << (extra_bits + 1)) + (1u << extra_bits) * ((code_value - 4) & 0x1);
            }
            else
            {
                /* This should not happen. */
                return GX_FALSE;
            }

            /* move backwards distance bytes in the output stream, and copy
               length bytes from this position to the output stream. */

            while (length)
            {
                if (length > distance)
                {
                    copy_len = distance;
                    length -= distance;
                }
                else
                {
                    copy_len = length;
                    length = 0;
                }

                if ((distance > (UINT)png -> gx_png_decoded_data_len) ||
                    ((UINT)png -> gx_png_decoded_data_len + copy_len > decoded_data_size))
                {

                    /* Distance exceed current decoded data length or copied length exceed remaining buffer size. */
                    return GX_FAILURE;
                }

                memcpy(png -> gx_png_decoded_data + png -> gx_png_decoded_data_len, /* Use case of memcpy is verified. */
                       png -> gx_png_decoded_data + png -> gx_png_decoded_data_len - distance,
                       copy_len * sizeof(GX_UBYTE));
                png -> gx_png_decoded_data_len += (INT)copy_len;
            }
        }
    }

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_png_tRNS_chunk_read                PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function reads in the tRNS chunk information.                  */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    png                                   PNG control block.            */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_memory_allocator           Application defined memory    */
/*                                            allocation function         */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_image_reader_png_decode                                         */
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
static UINT _gx_image_reader_png_tRNS_chunk_read(GX_PNG *png)
{
INT index;

    if ((png -> gx_png_color_type == 4) || (png -> gx_png_color_type == 6))
    {
        /* tRNS is prohibited for the other color types. */
        return GX_FAILURE;
    }

    if (png -> gx_png_color_type == 3)
    {
        /* Contains a series of one-byte alpha values.  */
        png -> gx_png_trans_num = png -> gx_png_trunk_end_index - png -> gx_png_data_index;

        /* For color type 3, tRNS can contain fewer values than there are palette entries. */
        if (png -> gx_png_trans_num > 256)
        {
            return GX_INVALID_VALUE;
        }
    }
    else
    {
        /* For color type 0, the tRNS chunk contains a single two-bytes gray level value.
           For color type 2, the tRNS chunk contains a single two-bytes RGB color value. */
        png -> gx_png_trans_num = (png -> gx_png_trunk_end_index - png -> gx_png_data_index) >> 1;

        if (png -> gx_png_trans_num > 3)
        {
            return GX_INVALID_VALUE;
        }
    }

    /* Then trans num is no bigger than 256, multiplication will not overflow. */
    png -> gx_png_trans = (GX_COLOR *)_gx_system_memory_allocator((ULONG)png -> gx_png_trans_num * sizeof(GX_COLOR));

    if (png -> gx_png_trans == GX_NULL)
    {
        return GX_SYSTEM_MEMORY_ERROR;
    }

    for (index = 0; index < png -> gx_png_trans_num; index++)
    {
        png -> gx_png_trans[index] = png -> gx_png_data[png -> gx_png_data_index++];

        if (png -> gx_png_color_type != 3)
        {
            png -> gx_png_trans[index] <<= 8;
            png -> gx_png_trans[index] |= png -> gx_png_data[png -> gx_png_data_index++];
        }
    }

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_png_IDAT_chunk_read                PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function reads in the IDAT chunk information and start         */
/*    decoding.                                                           */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    png                                   PNG control block.            */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_image_reader_png_bits_get         Extract bits from PNG data    */
/*                                            stream                      */
/*    _gx_image_reader_png_bits_skip        Skip bits from PNG data stream*/
/*    _gx_image_reader_png_clen_huffman_read                              */
/*                                          Read code length huffman table*/
/*    _gx_image_reader_png_ll_huffman_read  Read literal and length       */
/*                                            huffman table               */
/*    _gx_image_reader_png_huffcode_decode  Decode huffman codes          */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_image_reader_png_decode                                         */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            added data boundary check,  */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
static UINT _gx_image_reader_png_IDAT_chunk_read(GX_PNG *png)
{
/*zlib format
   +---+---+
   |CMF|FLG|
   +---+---+
   if FLG.FDICT set
   +---+---+---+---+
   |     DTCTID    |
   +---+---+---+---+
   +=====================+---+---+---+---+
   |...compressed data...| 32bit checksum|
   +=====================+---+---+---+---+
*/
UINT CMF;
UINT FLG;
INT  compress_method;
INT  window_size;
INT  fdict;
UINT header;
INT  hlit;
INT  hdist;
INT  hclen;
UINT alloc_size;
INT  len;
INT  nlen;

    if (_gx_image_reader_png_bits_get(png, 8, &CMF) != GX_SUCCESS)
    {
        return GX_FAILURE;
    }

    _gx_image_reader_png_bits_skip(8);

    if (_gx_image_reader_png_bits_get(png, 8, &FLG) != GX_SUCCESS)
    {
        return GX_FAILURE;
    }

    _gx_image_reader_png_bits_skip(8);

    compress_method = CMF & 0x0f;
    window_size = 2 << (((CMF & 0xf0) >> 4) + 8);
    fdict = FLG & 0x20;

    if ((fdict != 0) ||
        (window_size > 65536) ||
        (compress_method != 8))
    {
        return GX_FAILURE;
    }

    /* Safe int math is not required here, calling function limits max width, height to 14 bits so
       overflow cannot occur. */
    alloc_size = (UINT)(png -> gx_png_height * ((png -> gx_png_width * png -> gx_png_bpp + 7) / 8));
    alloc_size = alloc_size + (UINT)png -> gx_png_height;
    png -> gx_png_decoded_data = (GX_UBYTE *)_gx_system_memory_allocator(alloc_size);
    png -> gx_png_decoded_data_len = 0;

    if (png -> gx_png_decoded_data == GX_NULL)
    {
        return GX_SYSTEM_MEMORY_ERROR;
    }

    do
    {
        /* The header of one block */
        if (_gx_image_reader_png_bits_get(png, 8, &header) != GX_SUCCESS)
        {
            return GX_FAILURE;
        }

        if ((header & 0x6) == 0x00)
        {
            _gx_image_reader_png_bits_skip(3);

            /* no compression */
            _gx_image_reader_png_bits_skip(_bit_count & 0x7);
            if (_bit_count)
            {
                png -> gx_png_data_index -= (INT)(_bit_count / 8);
                _gx_image_reader_png_bits_skip(_bit_count);
            }

            /* 4 = 2 bytes len + 2 bytes nlen. */
            if (png -> gx_png_data_index + 4 > png -> gx_png_data_size)
            {
                return GX_INVALID_SIZE;
            }

            /* Read "LEN".  */
            len = png -> gx_png_data[png -> gx_png_data_index];
            png -> gx_png_data_index++;
            len |= (png -> gx_png_data[png -> gx_png_data_index] << 8);
            png -> gx_png_data_index++;

            /* Read "NLEN".  */
            nlen = png -> gx_png_data[png -> gx_png_data_index];
            png -> gx_png_data_index++;
            nlen |= (png -> gx_png_data[png -> gx_png_data_index] << 8);
            png -> gx_png_data_index++;

            if (len + nlen != 65535)
            {
                /* NLEN is not one's complement of LEN. */
                return GX_FAILURE;
            }

            /* Read data to outbuffer directly */
            if (len)
            {
                if ((UINT)(png -> gx_png_decoded_data_len + len) > alloc_size)
                {
                    /* Copied size exceed remaining buffer size. */
                    return GX_FAILURE;
                }

                if (png -> gx_png_data_index + len > png -> gx_png_data_size)
                {
                    return GX_INVALID_SIZE;
                }

                memcpy(png -> gx_png_decoded_data + png -> gx_png_decoded_data_len, png -> gx_png_data + png -> gx_png_data_index, (UINT)len); /* Use case of memcpy is verified. */
                png -> gx_png_data_index += len;
                png -> gx_png_decoded_data_len += len;
            }
        }
        else if ((header & 0x6) == 0x2)
        {
            _gx_image_reader_png_bits_skip(3);

            /* compressed with fixed Huffman codes */
            if (_gx_image_reader_png_huffcode_decode(png, GX_FALSE, alloc_size) != GX_SUCCESS)
            {
                return GX_FAILURE;
            }
        }
        else if ((header & 0x6) == 0x4)
        {
            /* compressed with dynamic Huffman codes */
            _gx_image_reader_png_bits_skip(3);

            /* # of Literal/Length codes */
            _gx_image_reader_png_bits_get(png, 5, (unsigned int *)&hlit);

            _gx_image_reader_png_bits_skip(5);
            hlit = (hlit & 0x1f) + 257;

            /* # of Distance codes */
            if (_gx_image_reader_png_bits_get(png, 5, (unsigned int *)&hdist) != GX_SUCCESS)
            {
                return GX_FAILURE;
            }
            _gx_image_reader_png_bits_skip(5);
            hdist = (hdist & 0x1f) + 1;

            /* # of Code Length codes */
            if (_gx_image_reader_png_bits_get(png, 4, (unsigned int *)&hclen) != GX_SUCCESS)
            {
                return GX_FAILURE;
            }
            _gx_image_reader_png_bits_skip(4);
            hclen = (hclen & 0xf) + 4;

            memset(png -> gx_png_huffman_clen_bits_count, 0, 17 * sizeof(INT));
            memset(png -> gx_png_huffman_clen_table, 0, 20 * sizeof(INT));
            memset(png -> gx_png_huffman_lit_bits_count, 0, 17 * sizeof(INT));
            memset(png -> gx_png_huffman_lit_table, 0, GX_PNG_HUFFMAN_LIT_TABLE_SIZE * sizeof(int));
            memset(png -> gx_png_huffman_dist_bits_count, 0, 17 * sizeof(INT));
            memset(png -> gx_png_huffman_dist_table, 0, 30 * sizeof(INT));

            if (_gx_image_reader_png_clen_huffman_read(png, (UINT)hclen) != GX_SUCCESS)
            {
                return GX_FAILURE;
            }

            if (_gx_image_reader_png_ll_huffman_read(png, (UINT)hlit, (UINT)hdist) != GX_SUCCESS)
            {
                return GX_FAILURE;
            }

            if (_gx_image_reader_png_huffcode_decode(png, GX_TRUE, alloc_size) != GX_SUCCESS)
            {
                return GX_FAILURE;
            }
        }
        else
        {
            /* error */
            return GX_FAILURE;
        }
    } while ((header & 0x1) != 0x1);

    /* Skip checksum */
    png -> gx_png_data_index = png -> gx_png_trunk_end_index;
    _gx_image_reader_png_bits_skip(_bit_count);

    if (alloc_size != (UINT)png -> gx_png_decoded_data_len)
    {
        /* Decoded data size doesn't match prediction. */
        return GX_FAILURE;
    }

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_png_gAMA_chunk_read                PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function reads in the gAMA chunk information.                  */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    png                                   PNG control block.            */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_image_reader_png_4bytes_read      Read 4 bytes and pact it to   */
/*                                            an INT value                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_image_reader_png_decode                                         */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            added data boundary check,  */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
static UINT _gx_image_reader_png_gAMA_chunk_read(GX_PNG *png)
{
    if (png -> gx_png_data_index + 4 > png -> gx_png_trunk_end_index)
    {
        return GX_INVALID_SIZE;
    }

    /* Specifies the gamma of the camera that produced the image. */

    /* A gamma of 0.45 would be stored as teh integer 45000. */
    _gx_image_reader_png_4bytes_read(png, &png -> gx_png_gamma);

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_png_PLTE_chunk_read                PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function reads in the PLTE chunk information.                  */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    png                                   PNG control block.            */
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
/*    _gx_image_reader_png_decode                                         */
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
static UINT _gx_image_reader_png_PLTE_chunk_read(GX_PNG *png)
{
INT      index;
GX_UBYTE red;
GX_UBYTE green;
GX_UBYTE blue;

    png -> gx_png_palette_table_size = (png -> gx_png_trunk_end_index - png -> gx_png_data_index) / 3;

    /* The PLTE chunk contains from 1 to 256 palette entries. */
    if (png -> gx_png_palette_table_size > 256)
    {
        return GX_INVALID_VALUE;
    }

    for (index = 0; index < png -> gx_png_palette_table_size; index++)
    {
        red = png -> gx_png_data[png -> gx_png_data_index++];
        green = png -> gx_png_data[png -> gx_png_data_index++];
        blue = png -> gx_png_data[png -> gx_png_data_index++];

        png -> gx_png_palette_table[index] = (GX_COLOR)((red << 16) | (green << 8) | blue);
    }

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_png_paeth_predictor                PORTABLE C      */
/*                                                           6.1.8        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function performs Paeth filter algorithm.                      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    a                                     Byte of left pixel.           */
/*    b                                     Byte of above pixel.          */
/*    c                                     Byte of upper left pixel.     */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                The nearest of a, b, c.       */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_image_reader_png_unfilter                                       */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*  08-02-2021     Kenneth Maxwell          Modified comment(s),          */
/*                                            replaced abs with GX_ABS,   */
/*                                            resulting in version 6.1.8  */
/*                                                                        */
/**************************************************************************/
static GX_UBYTE _gx_image_reader_png_paeth_predictor(GX_UBYTE a, GX_UBYTE b, GX_UBYTE c)
{
INT p;
INT pa, pb, pc;

    p = a + b - c;
    pa = GX_ABS(p - a);
    pb = GX_ABS(p - b);
    pc = GX_ABS(p - c);

    /*return nearest of a, b, c */

    if ((pa <= pb) && (pa <= pc))
    {
        return a;
    }
    else if (pb <= pc)
    {
        return b;
    }
    else
    {
        return c;
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_png_unfilter                       PORTABLE C      */
/*                                                           6.2.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function reverts the effect of filter.                         */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    png                                   PNG control block.            */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    memmove                                                             */
/*    _gx_image_reader_png_paeth_predictor  Perform Paeth filter algorithm*/
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_image_reader_png_decode                                         */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*  10-31-2022     Kenneth Maxwell          Modified comment(s),          */
/*                                            added null pointer check,   */
/*                                            resulting in version 6.2.0  */
/*                                                                        */
/**************************************************************************/
static UINT _gx_image_reader_png_unfilter(GX_PNG *png)
{
INT filter_type;
INT byte_width;
INT pos;
INT bpp = png -> gx_png_bpp;
INT x;
INT y;

    if (png -> gx_png_decoded_data == GX_NULL)
    {
        /* This happens when IDAT chunk is missing. */
        return GX_FAILURE;
    }

    byte_width = (png -> gx_png_width * bpp + 7) >> 3;
    bpp = (bpp + 7) >> 3;

    for (y = 0; y < png -> gx_png_height; y++)
    {
        filter_type = png -> gx_png_decoded_data[y * byte_width + y];

        pos = y * byte_width;
        memmove(png -> gx_png_decoded_data + pos, png -> gx_png_decoded_data + pos + y + 1, (UINT)byte_width);

        switch (filter_type)
        {
        case 0:
            break;
        case 1:
            /* sub(x) + raw(x-bpp)*/
            for (x = bpp; x < byte_width; x++)
            {
                png -> gx_png_decoded_data[pos + x] = (GX_UBYTE)(png -> gx_png_decoded_data[pos + x] + png -> gx_png_decoded_data[pos + x - bpp]);
            }
            break;
        case 2:
            /*up(x) + prior(x)*/
            if (y == 0)
            {
                break;
            }

            for (x = 0; x < byte_width; x++)
            {
                png -> gx_png_decoded_data[pos + x] = (GX_UBYTE)(png -> gx_png_decoded_data[pos + x] + png -> gx_png_decoded_data[pos + x - byte_width]);
            }
            break;
        case 3:
            /* average(x) + floor((raw(x-bpp)+prior(x))/2 */
            if (y == 0)
            {
                for (x = bpp; x < byte_width; x++)
                {
                    png -> gx_png_decoded_data[pos + x] = (GX_UBYTE)(png -> gx_png_decoded_data[pos + x] + (png -> gx_png_decoded_data[pos + x - bpp] >> 1));
                }
            }
            else
            {
                for (x = 0; x < bpp; x++)
                {
                    png -> gx_png_decoded_data[pos + x] = (GX_UBYTE)(png -> gx_png_decoded_data[pos + x] + (png -> gx_png_decoded_data[pos + x - byte_width] >> 1));
                }

                for (x = bpp; x < byte_width; x++)
                {
                    png -> gx_png_decoded_data[pos + x] = (GX_UBYTE)(png -> gx_png_decoded_data[pos + x] + ((png -> gx_png_decoded_data[pos + x - bpp] +
                                                                                                             png -> gx_png_decoded_data[pos + x - byte_width]) >> 1));
                }
            }
            break;
        case 4:
            /* paeth(x) + paethpredictor(raw(x-bpp), prior(x), prior(x-bpp)) */
            if (y == 0)
            {
                for (x = bpp; x < byte_width; x++)
                {
                    png -> gx_png_decoded_data[pos + x] = (GX_UBYTE)(png -> gx_png_decoded_data[pos + x] + png -> gx_png_decoded_data[pos + x - bpp]);
                }
            }
            else
            {
                for (x = 0; x < bpp; x++)
                {
                    png -> gx_png_decoded_data[pos + x] = (GX_UBYTE)(png -> gx_png_decoded_data[pos + x] + png -> gx_png_decoded_data[pos + x - byte_width]);
                }

                for (x = bpp; x < byte_width; x++)
                {
                    png -> gx_png_decoded_data[pos + x] = (GX_UBYTE)(png -> gx_png_decoded_data[pos + x] + _gx_image_reader_png_paeth_predictor(png -> gx_png_decoded_data[pos + x - bpp],
                                                                                                                                                png -> gx_png_decoded_data[pos + x - byte_width],
                                                                                                                                                png -> gx_png_decoded_data[pos + x - byte_width - bpp]));
                }
            }
            break;
        default:
            return GX_FAILURE;
        }
    }

    png -> gx_png_decoded_data_len -= png -> gx_png_height;

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_png_decode                         PORTABLE C      */
/*                                                           6.2.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function decode a PNG format image.                            */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    inmap                                 Input pixelmap                */
/*    outmap                                Output pixelmap               */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    strncmp                                                             */
/*    _gx_system_memory_allocator           Application defined memory    */
/*                                            allocation function         */
/*    _gx_system_memory_free                Application defined memory    */
/*                                            free function               */
/*    _gx_image_reader_png_crc_table_make   Create a crc table            */
/*    _gx_image_reader_png_crc_get          Calculate crc value of        */
/*                                            specified data              */
/*    _gx_image_reader_png_4bytes_read      Read 4 bytes and pack it to   */
/*                                            INT type value              */
/*    _gx_image_reader_png_chunk_type_read  Read chunk type information   */
/*    _gx_image_reader_png_IDAT_chunk_read  Read IDAT chunk information   */
/*                                            and start decoding          */
/*    _gx_image_reader_png_IHDR_chunk_read  Read IHDR chunk information   */
/*    _gx_image_reader_png_gAMA_chunk_read  Read gAMA chunk information   */
/*    _gx_image_reader_png_PLTE_chunk_read  Read PLTE chunk information   */
/*    _gx_image_reader_png_tRNS_chunk_read  Read tRNS chunk information   */
/*    _gx_image_reader_png_unfilter         Revert effect of filter       */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_image_reader_image_decode                                       */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            added data boundary check,  */
/*                                            resulting in version 6.1    */
/*  06-02-2021     Ting Zhu                 Modified comment(s), and      */
/*                                            improved png decoding       */
/*                                            performance,                */
/*                                            resulting in version 6.1.7  */
/*  10-31-2022     Ting Zhu                 Modified comment(s), and      */
/*                                            added invalid value check,  */
/*                                            resulting in version 6.2.0  */
/*                                                                        */
/**************************************************************************/
UINT _gx_image_reader_png_decode(GX_CONST GX_UBYTE *read_data, ULONG data_size, GX_PIXELMAP *outmap)
{
UINT    status = GX_SUCCESS;
GX_PNG  png;
INT     data_len;
CHAR    chunk_type[4];
INT     checksum;
GX_BOOL idat_done = GX_FALSE;
INT    *scratch_buffer;
INT     scratch_index = 0;
GX_BOOL decoded_done = GX_FALSE;

    if (data_size < 8)
    {
        return GX_INVALID_SIZE;
    }

    if ((read_data[0] == 137) &&
        (read_data[1] == 80) &&
        (read_data[2] == 78) &&
        (read_data[3] == 71) &&
        (read_data[4] == 13) &&
        (read_data[5] == 10) &&
        (read_data[6] == 26) &&
        (read_data[7] == 10))
    {
        if (!_gx_system_memory_allocator)
        {
            return GX_SYSTEM_MEMORY_ERROR;
        }

        memset(&png, 0, sizeof(GX_PNG));
        scratch_buffer = (INT *)_gx_system_memory_allocator(GX_PNG_SCRATCH_BUFFER_SIZE * sizeof(ULONG));

        if (scratch_buffer == GX_NULL)
        {
            return GX_SYSTEM_MEMORY_ERROR;
        }

        png.gx_png_crc_table = (UINT *)scratch_buffer;
        scratch_index += GX_PNG_CRC_TABLE_SIZE;
        png.gx_png_huffman_lit_table = scratch_buffer + scratch_index;
        scratch_index += GX_PNG_HUFFMAN_LIT_TABLE_SIZE;
        png.gx_png_huffman_lit_code_len = scratch_buffer + scratch_index;
        scratch_index += GX_PNG_HUFFMAN_LIT_CODE_LEN_TABLE_SIZE;
        png.gx_png_huffman_dist_code_len = scratch_buffer + scratch_index;
        scratch_index += GX_PNG_HUFFMAN_DIST_CODE_LEN_TABLE_SIZE;
        png.gx_png_palette_table = (GX_COLOR *)scratch_buffer + scratch_index;

        png.gx_png_data = (GX_UBYTE *)read_data;
        png.gx_png_data_size = (INT)data_size;
        png.gx_png_data_index = 8;

        _gx_image_reader_png_crc_table_make(&png);

        _bit_buffer = 0;
        _bit_count = 0;

        while (png.gx_png_data_index < png.gx_png_data_size - 4)
        {
            /* data_len*/
            _gx_image_reader_png_4bytes_read(&png, &data_len);

            png.gx_png_trunk_crc = 0xffffffff;

            if ((data_len < 0) ||
                (data_len > png.gx_png_data_size - png.gx_png_data_index - 4))
            {
                status = GX_INVALID_SIZE;
                break;
            }

            /* Calculate checksum of the chunk data.  */
            _gx_image_reader_png_crc_get(&png, &png.gx_png_trunk_crc, (UINT)(4 + data_len));

            /* Read chunk type.  */
            _gx_image_reader_png_chunk_type_read(&png, chunk_type);

            png.gx_png_trunk_end_index = png.gx_png_data_index + data_len;

            if ((strncmp(chunk_type, "IDAT", 4) == 0) && (!idat_done))
            {
                idat_done = GX_TRUE;

                status = _gx_image_reader_png_IDAT_chunk_read(&png);
            }
            else if (strncmp(chunk_type, "IHDR", 4) == 0)
            {
                status = _gx_image_reader_png_IHDR_chunk_read(&png);
            }
            else if (strncmp(chunk_type, "gAMA", 4) == 0)
            {
                status = _gx_image_reader_png_gAMA_chunk_read(&png);
            }
            else if (strncmp(chunk_type, "PLTE", 4) == 0)
            {
                status = _gx_image_reader_png_PLTE_chunk_read(&png);
            }
            else if ((strncmp(chunk_type, "tRNS", 4) == 0) && (png.gx_png_trans == GX_NULL))
            {
                /* Read transparent information. */
                status = _gx_image_reader_png_tRNS_chunk_read(&png);
            }
            else if (strncmp(chunk_type, "IEND", 4) == GX_SUCCESS)
            {
                /* End */
                decoded_done = GX_TRUE;
                break;
            }
            else
            {
                /* Ignore chunk */
                png.gx_png_data_index += data_len;
            }

            if (status != GX_SUCCESS)
            {
                break;
            }

            _gx_image_reader_png_4bytes_read(&png, &checksum);

            if (checksum != (INT)png.gx_png_trunk_crc)
            {
                status = GX_FAILURE;
                break;
            }
        }

        if(status == GX_SUCCESS && (!decoded_done))
        {
            status = GX_FAILURE;
        }

        if (status == GX_SUCCESS)
        {
            /* revert filter */
            status = _gx_image_reader_png_unfilter(&png);
        }

        if (status == GX_SUCCESS)
        {
            outmap -> gx_pixelmap_data = png.gx_png_decoded_data;
            outmap -> gx_pixelmap_data_size = (ULONG)png.gx_png_decoded_data_len;
            outmap -> gx_pixelmap_width = (GX_VALUE)png.gx_png_width;
            outmap -> gx_pixelmap_height = (GX_VALUE)png.gx_png_height;
            outmap -> gx_pixelmap_flags = GX_PIXELMAP_RAW_FORMAT;

            if (png.gx_png_trans_num)
            {
                outmap -> gx_pixelmap_flags |= GX_PIXELMAP_ALPHA;
                outmap -> gx_pixelmap_transparent_color = (ULONG)png.gx_png_trans;
            }

            switch (png.gx_png_color_type)
            {
            case 6:
                /* Each pixel is an RGB triple, followed by an alpha sample. */
                if (png.gx_png_bit_depth == 8)
                {
                    outmap -> gx_pixelmap_format = GX_IMAGE_FORMAT_32BPP;
                }
                else
                {
                    /* Bit depth = 16. */
                    outmap -> gx_pixelmap_format = GX_IMAGE_FORMAT_64BPP;
                }
                outmap -> gx_pixelmap_flags |= GX_PIXELMAP_ALPHA;
                break;
            case 4:
                /* Each pixel is a grayscale sample followed by an alpha sample. */
                if (png.gx_png_bit_depth == 8)
                {
                    outmap -> gx_pixelmap_format = GX_IMAGE_FORMAT_16BPP_GRAY_ALPHA;
                }
                else
                {
                    /* Bit depth = 16. */
                    outmap -> gx_pixelmap_format = GX_IMAGE_FORMAT_32BPP_GRAY_ALPHA;
                }
                outmap -> gx_pixelmap_flags |= GX_PIXELMAP_ALPHA;
                break;
            case 3:
                /* Each pixel is a palette index */
                switch (png.gx_png_bit_depth)
                {
                case 1:
                    outmap -> gx_pixelmap_format = GX_IMAGE_FORMAT_1BPP;
                    break;
                case 2:
                    outmap -> gx_pixelmap_format = GX_IMAGE_FORMAT_2BPP;
                    break;
                case 4:
                    outmap -> gx_pixelmap_format = GX_IMAGE_FORMAT_4BPP;
                    break;
                default:
                    /* Bit depth = 8. */
                    outmap -> gx_pixelmap_format = GX_IMAGE_FORMAT_8BPP;
                    break;
                }

                /* Palette table size is no bigger than 256, aux data size will not overflow. */
                outmap -> gx_pixelmap_aux_data_size = (ULONG)(png.gx_png_palette_table_size) * sizeof(GX_COLOR);
                outmap -> gx_pixelmap_aux_data = (GX_UBYTE *)_gx_system_memory_allocator(outmap -> gx_pixelmap_aux_data_size);

                if (!outmap -> gx_pixelmap_aux_data)
                {
                    return GX_SYSTEM_MEMORY_ERROR;
                }

                memcpy((GX_UBYTE *)outmap -> gx_pixelmap_aux_data, png.gx_png_palette_table, outmap -> gx_pixelmap_aux_data_size); /* Use case of memcpy is verified. */
                break;
            case 2:
                /* Each pixel is an RGB triple. */
                if (png.gx_png_bit_depth == 8)
                {
                    outmap -> gx_pixelmap_format = GX_IMAGE_FORMAT_24BPP;
                }
                else
                {
                    /* Bit depth = 16. */
                    outmap -> gx_pixelmap_format = GX_IMAGE_FORMAT_48BPP;
                }
                break;
            default:
                /* Each pixel is a grayscale sample. */
                switch (png.gx_png_bit_depth)
                {
                case 16:
                    outmap -> gx_pixelmap_format = GX_IMAGE_FORMAT_16BPP_GRAY;
                    break;
                case 8:
                    outmap -> gx_pixelmap_format = GX_IMAGE_FORMAT_8BPP;
                    break;
                case 4:
                    outmap -> gx_pixelmap_format = GX_IMAGE_FORMAT_4BPP;
                    break;
                case 2:
                    outmap -> gx_pixelmap_format = GX_IMAGE_FORMAT_2BPP;
                    break;
                default:
                    /* Bit depth = 1. */
                    outmap -> gx_pixelmap_format = GX_IMAGE_FORMAT_1BPP;
                    break;
                }
                break;
            }
        }

        if (status != GX_SUCCESS)
        {
            if (png.gx_png_decoded_data)
            {
                _gx_system_memory_free(png.gx_png_decoded_data);
            }

            if (png.gx_png_trans)
            {
                _gx_system_memory_free(png.gx_png_trans);
            }
        }

        /* Free scratch buffer. */
        _gx_system_memory_free(scratch_buffer);
    }
    else
    {
        return GX_FAILURE;
    }

    return status;
}
#endif

