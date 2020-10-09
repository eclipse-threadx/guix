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
/**   Utility Management (Utility)                                        */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE

/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_context.h"
#include "gx_system.h"
#include "gx_utility.h"
#include "gx_image_reader.h"

#if defined(GX_SOFTWARE_DECODER_SUPPORT)

static UINT              _gx_jpg_bit_buffer;
static UINT              _gx_jpg_bit_count;

static GX_CONST GX_UBYTE _gx_jpg_reorder_index[] =
{
    0, 1, 8, 16, 9, 2, 3, 10,
    17, 24, 32, 25, 18, 11, 4, 5,
    12, 19, 26, 33, 40, 48, 41, 34,
    27, 20, 13, 6, 7, 14, 21, 28,
    35, 42, 49, 56, 57, 50, 43, 36,
    29, 22, 15, 23, 30, 37, 44, 51,
    58, 59, 52, 45, 38, 31, 39, 46,
    53, 60, 61, 54, 47, 55, 62, 63
};

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_jpeg_bits_get                      PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Extract a specified number of bits from JPEG data stream and        */
/*    advance the read pointer of the JPEG data stream.                   */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    jpeg_info                             JPEG data context             */
/*    num_of_bits                           Number of bits to extract     */
/*    return_value                          Extracted data.               */
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
/*    _gx_image_reader_jpeg_dc_decode                                     */
/*    _gx_image_reader_jpeg_ac_decode                                     */
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
static void _gx_image_reader_jpeg_bits_get(GX_JPEG_INFO *jpeg_info, UINT num_of_bits, UINT *return_value)
{
INT index = jpeg_info -> gx_jpeg_data_index;

    while (_gx_jpg_bit_count <= num_of_bits)
    {
        if ((index < jpeg_info -> gx_jpeg_data_size) && (_gx_jpg_bit_count <= 24))
        {
            _gx_jpg_bit_buffer |= ((UINT)(jpeg_info -> gx_jpeg_data[index]) << (UINT)(24 - _gx_jpg_bit_count));

            if ((jpeg_info -> gx_jpeg_data[index] == 0xff) &&
                (index + 1 < jpeg_info -> gx_jpeg_data_size) &&
                (jpeg_info -> gx_jpeg_data[index + 1] == 0x00))
            {
                index += 2;
            }
            else
            {
                index += 1;
            }
        }

        _gx_jpg_bit_count += 8;
    }

    jpeg_info -> gx_jpeg_data_index = index;

    (*return_value) = (UINT)_gx_jpg_bit_buffer;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_jpeg_bits_skip                     PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Skips bits from tempory JPEG data stream.                           */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    skip_bits                             Number of bits to skip        */
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
/*    _gx_image_reader_jpeg_dc_decode                                     */
/*    _gx_image_reader_jpeg_ac_decode                                     */
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
static void _gx_image_reader_jpeg_bits_skip(UINT skip_bits)
{
    _gx_jpg_bit_buffer <<= skip_bits;

    _gx_jpg_bit_count -= skip_bits;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_jpeg_range                         PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Limit the value to be in the range of [0, 255]                      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    value                                 Value to be checked           */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    GX_UBYTE                              Output value.                 */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_image_reader_jpeg_dequantize_idct                               */
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
static GX_UBYTE _gx_image_reader_jpeg_range(INT i)
{
    if (i < 0)
    {
        return 0;
    }
    else if (i > 255)
    {
        return 255;
    }
    else
    {
        return((GX_UBYTE)i);
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_jpeg_YCbCr2RGB_888                 PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Converts YCbCr value to 888RGB color space and write it to memory.  */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    y                                     Luminance                     */
/*    cb                                    Chroma (Blue-difference)      */
/*    cr                                    Chroma (Red-difference)       */
/*    put                                   Retrieved 888RGB color        */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Value                                 565 RGB value                 */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_image_reader_jpeg_range           Limit value in range [0, 255] */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_image_reader_jpeg_one_mcu_write                                 */
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
static GX_COLOR _gx_image_reader_jpeg_YCbCr2RGB_888(INT y, INT cb, INT cr, GX_UBYTE *put)
{
INT      temp_r;
INT      temp_g;
INT      temp_b;
GX_UBYTE r;
GX_UBYTE g;
GX_UBYTE b;


    cb -= 128;
    cr -= 128;

    temp_r = y + cr + (cr >> 2) + (cr >> 3);
    temp_g = y - ((cb >> 2) + (cb >> 4) + (cb >> 5)) - ((cr >> 1) + (cr >> 3) + (cr >> 4) + (cr >> 6));
    temp_b = y + cb + (cb >> 1) + (cb >> 2);


    /* Make sure the range of the RGB values are within bound. */
    r = _gx_image_reader_jpeg_range(temp_r);
    g = _gx_image_reader_jpeg_range(temp_g);
    b = _gx_image_reader_jpeg_range(temp_b);

    *put = r;
    *(put + 1) = g;
    *(put + 2) = b;

    return 0;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_dislay_driver_jpeg_quantization_table_set       PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Sets the JPEG quantization table.                                   */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    jpeg_info                             JPEG data control block       */
/*    segment_len                           Initial length of the segment */
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
/*    _gx_image_reader_jpeg_decode                                        */
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
static UINT _gx_image_reader_jpeg_quantization_table_set(GX_JPEG_INFO *jpeg_info, UINT segment_len)
{
GX_UBYTE  table_index;
GX_UBYTE *jpeg_data = jpeg_info -> gx_jpeg_data + jpeg_info -> gx_jpeg_data_index;
INT       index;

    /* Minus two-byte length. */
    jpeg_info -> gx_jpeg_data_index += (INT)segment_len;
    segment_len -= 2;
    jpeg_data += 2;

    while (segment_len)
    {
        /* The upper 4 bits specify the element precision: 0 indicates 8-bit, 1 indecates 16-bit. */
        if ((*jpeg_data) & 0xf0)
        {
            /* Baseline DCT-based jpeg only support 8-bit precision. */
            return GX_NOT_SUPPORTED;
        }

        /* The lower 4 bits specify the table destination identifier, specify one of four possible destinations. */
        table_index = (*jpeg_data++) & 0x03;

        /* Read quantization table element. */
        for (index = 0; index < 64; index++)
        {
            jpeg_info -> gx_jpeg_quantization_table[table_index][index] = *jpeg_data++;
        }

        segment_len -= 65;
    }

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_jpeg_huffcode_find                 PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Lookup the huffman code.                                            */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    jpeg_info                             JPEG data control block       */
/*    table_class                           Table class, 0 = DC table,    */
/*                                          1 = AC table                  */
/*    table_id                              Table index                   */
/*    scan_buffer                           Buffer to search from         */
/*    bit_len                               Retrieved Huffman Code Length */
/*    code_value                            Retrieved Huffman Code        */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Status Code                                                         */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_image_reader_jpeg_dc_decode                                     */
/*    _gx_image_reader_jpeg_ac_decode                                     */
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
static UINT _gx_image_reader_jpeg_huffcode_find(GX_JPEG_INFO *jpeg_info,
                                                UINT table_class,
                                                UINT table_id,
                                                UINT scan_buffer,
                                                UINT *bit_len,
                                                GX_UBYTE *code_value)
{
UINT i_bit;
UINT bit_count;
UINT code;
UINT code_cal = 0;
UINT code_index = 0;

    for (i_bit = 0; i_bit < 16; i_bit++)
    {
        bit_count = (UINT)(jpeg_info -> gx_jpeg_huffman_bits_count[table_class][table_id][i_bit]);

        if (bit_count)
        {
            code = (scan_buffer) >> (31 - i_bit);

            if (code <= code_cal + bit_count - 1)
            {
                *bit_len = i_bit + 1;
                *code_value = (GX_UBYTE)jpeg_info -> gx_jpeg_huffman_table[table_class][table_id][code_index + code - code_cal];
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

    return GX_FAILURE;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_huffman_table_set                  PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Sets up the huffman table.                                          */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    jpeg_info                             JPEG data control block       */
/*    segment_len                           Initial length of the segment */
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
/*    _gx_image_reader_jpeg_decode_blocks                                 */
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
static UINT _gx_image_reader_huffman_table_set(GX_JPEG_INFO *jpeg_info, UINT segment_len)
{
GX_UBYTE *jpeg_data = jpeg_info -> gx_jpeg_data + jpeg_info -> gx_jpeg_data_index;
GX_UBYTE  table_class;
GX_UBYTE  table_id;
UINT      code_index;
UINT      i_bits;
UINT      i_counts;
USHORT    i_table_size;

    /* Minus two-byte length. */
    jpeg_info -> gx_jpeg_data_index += (INT)segment_len;
    segment_len -= 2;
    jpeg_data += 2;

    while (segment_len)
    {

        /* table_calss: 0 DC 1 AC */
        table_class = ((*jpeg_data) >> 4) & 1;
        table_id = (*jpeg_data++) & 0x01;

        segment_len -= 17;

        i_table_size = 0;
        code_index = 0;

        /* Read the number of Huffman codes for each bit length, from 1 to 16. */
        for (i_bits = 0; i_bits < 16; i_bits++)
        {
            jpeg_info -> gx_jpeg_huffman_bits_count[table_class][table_id][i_bits] = (*jpeg_data);

            segment_len -= (*jpeg_data);

            i_table_size = (USHORT)(i_table_size + (*jpeg_data++));
        }

        /* The max i_table_size is 16 * 255, overflow cannot occur. */
        jpeg_info -> gx_jpeg_huffman_table[table_class][table_id] = (INT *)_gx_system_memory_allocator(i_table_size * sizeof(INT));

        if (jpeg_info -> gx_jpeg_huffman_table[table_class][table_id] == GX_NULL)
        {
            return GX_SYSTEM_MEMORY_ERROR;
        }

        /* Read the bytes that the Huffman codes represent, and generate Huffman tree that
           map a Huffman code to a represent value. */
        for (i_bits = 0; i_bits < 16; i_bits++)
        {
            for (i_counts = 0; i_counts < (UINT)jpeg_info -> gx_jpeg_huffman_bits_count[table_class][table_id][i_bits]; i_counts++)
            {
                /* Read byte values the Huffman code represents. */
                jpeg_info -> gx_jpeg_huffman_table[table_class][table_id][code_index++] = (*jpeg_data++);
            }
        }
    }

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_jpeg_frame_header_read           PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Reads in the frame header infomration.                              */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    jpeg_info                             JPEG data control block       */
/*    segment_len                           Initial length of the segment */
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
/*    _gx_image_reader_jpeg_decode_blocks                                 */
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
static UINT _gx_image_reader_jpeg_frame_header_read(GX_JPEG_INFO *jpeg_info, UINT segment_len)
{
GX_UBYTE *jpeg_data = jpeg_info -> gx_jpeg_data + jpeg_info -> gx_jpeg_data_index;
INT       i_component;

    jpeg_info -> gx_jpeg_data_index += (INT)segment_len;
    jpeg_data += 2;

    /* Skip precision field. */
    jpeg_data++;

    /* Read image width, WORD */
    jpeg_info -> gx_jpeg_height = *jpeg_data++;
    jpeg_info -> gx_jpeg_height = (USHORT)(jpeg_info -> gx_jpeg_height << 8);
    jpeg_info -> gx_jpeg_height = (USHORT)(jpeg_info -> gx_jpeg_height | (*jpeg_data++));

    /* Limit max jpeg height to 14 bits. */
    if (jpeg_info -> gx_jpeg_height > GX_MAX_PIXELMAP_RESOLUTION)
    {
        return GX_INVALID_HEIGHT;
    }

    /* Read image height, WORD */
    jpeg_info -> gx_jpeg_width = *jpeg_data++;
    jpeg_info -> gx_jpeg_width = (USHORT)(jpeg_info -> gx_jpeg_width << 8);
    jpeg_info -> gx_jpeg_width = (USHORT)(jpeg_info -> gx_jpeg_width | (*jpeg_data++));

    /* Limit max jpeg width to 14 bits. */
    if (jpeg_info -> gx_jpeg_width > GX_MAX_PIXELMAP_RESOLUTION)
    {
        return GX_INVALID_WIDTH;
    }

    /* Read image components. */
    jpeg_info -> gx_jpeg_num_of_components = *jpeg_data++;

    for (i_component = 0; i_component < jpeg_info -> gx_jpeg_num_of_components; i_component++)
    {
        /* Read component id */
        jpeg_info -> gx_jpeg_component_id[i_component] = *jpeg_data++;

        /* Read sample factor */
        jpeg_info -> gx_jpeg_sample_factor[i_component] = *jpeg_data++;

        /* Read quantization table index */
        jpeg_info -> gx_jpeg_qantization_table_index[i_component] = *jpeg_data++;
    }

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_jpeg_scan_header_read              PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Reads in the scan header information.                               */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    jpeg_info                             JPEG data control block       */
/*    segment_len                           Initial length of the segment */
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
/*    _gx_image_reader_jpeg_decode_blocks                                 */
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
static UINT _gx_image_reader_jpeg_scan_header_read(GX_JPEG_INFO *jpeg_info, UINT segment_len)
{
GX_UBYTE *jpeg_data = jpeg_info -> gx_jpeg_data + jpeg_info -> gx_jpeg_data_index;
INT       index;

    jpeg_data += 2;
    jpeg_info -> gx_jpeg_data_index += (INT)segment_len;

    /* Read the number of image components.  */
    jpeg_info -> gx_jpeg_num_of_components = *jpeg_data++;

    for (index = 0; index < jpeg_info -> gx_jpeg_num_of_components; index++)
    {
        /* skip image component */
        jpeg_data++;

        /* high bits correspond to DC table index.
           low bits correspond to AC table index
           0: Y Huffman table
           1: Chrominance Huffman table. */
        jpeg_info -> gx_jpeg_dc_table_index[index] = ((*jpeg_data) & 0xf0) >> 4;
        jpeg_info -> gx_jpeg_ac_table_index[index] = (*jpeg_data++) & 0x0f;
    }

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_jpeg_dc_decode                     PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Decode the DC component.                                            */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    jpeg_info                             JPEG data control block       */
/*    i_component                           Component index               */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Status                                                              */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_image_reader_jpeg_bits_get         Extract a specified number of*/
/*                                             bits from JPEG data stream */
/*    _gx_image_reader_jpeg_bits_skip        Skips bits from tempory JPEG */
/*                                             data stream                */
/*    _gx_image_reader_jpeg_huffman_code_find                             */
/*                                           Lookup the huffman code      */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_image_reader_jpeg_one_block_decode                              */
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
static UINT _gx_image_reader_jpeg_dc_decode(GX_JPEG_INFO *jpeg_info, UINT i_component)
{
UINT     scan_buffer;
UINT     i_bits = 0;
GX_UBYTE code_value;
INT      Diff;
UINT     table_index = jpeg_info -> gx_jpeg_dc_table_index[i_component];
GX_BOOL  negative;

    _gx_image_reader_jpeg_bits_get(jpeg_info, 16, &scan_buffer);

    if (_gx_image_reader_jpeg_huffcode_find(jpeg_info, 0, table_index, scan_buffer, &i_bits, &code_value) == 0)
    {
        _gx_image_reader_jpeg_bits_skip(i_bits);

        if (code_value == 0)
        {
            jpeg_info -> gx_jpeg_vecter[0] = jpeg_info -> gx_jpeg_pre_dc[i_component];
        }
        else
        {

            _gx_image_reader_jpeg_bits_get(jpeg_info, code_value, (UINT *)&Diff);
            _gx_image_reader_jpeg_bits_skip(code_value);

            Diff = (INT)(((UINT)Diff) >> (32 - code_value));

            negative = !(Diff >> (code_value - 1));

            if (negative)
            {
                Diff += 1 - (1 << code_value);
            }

            jpeg_info -> gx_jpeg_vecter[0] = Diff + jpeg_info -> gx_jpeg_pre_dc[i_component];
            jpeg_info -> gx_jpeg_pre_dc[i_component] = jpeg_info -> gx_jpeg_vecter[0];
        }
    }
    else
    {
        return GX_FAILURE;
    }

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_jpeg_ac_decode                     PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Decode the AC component.                                            */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    jpeg_info                             JPEG data control block       */
/*    i_component                           Component index               */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Status Code                                                         */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_image_reader_jpeg_bits_get         Extract a specified number of*/
/*                                             bits from JPEG data stream */
/*    _gx_image_reader_jpeg_bits_skip        Skips bits from tempory JPEG */
/*                                             data stream                */
/*    _gx_image_reader_jpeg_huffman_code_find                             */
/*                                           Lookup the huffman code      */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_image_reader_jpeg_one_block_decode                              */
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
static UINT _gx_image_reader_jpeg_ac_decode(GX_JPEG_INFO *jpeg_info, UINT i_component)
{
UINT     scan_buffer;
UINT     i_bits = 0;
GX_UBYTE code_value;
GX_UBYTE catogory;
GX_UBYTE runs_of_zero;
INT      ac_coefficient;
UINT     ac_counter = 1;
UINT     table_index = jpeg_info -> gx_jpeg_ac_table_index[i_component];
INT      negative;

    while (ac_counter < 64)
    {
        i_bits = 0;
        _gx_image_reader_jpeg_bits_get(jpeg_info, 16, &scan_buffer);

        if (_gx_image_reader_jpeg_huffcode_find(jpeg_info, 1, table_index, scan_buffer, &i_bits, &code_value) == 0)
        {
            _gx_image_reader_jpeg_bits_skip(i_bits);

            runs_of_zero = (0xf0 & code_value) >> 4;
            catogory = 0x0f & code_value;

            if (catogory == 0)
            {
                /* EOB encountered. */
                if (runs_of_zero == 0)
                {
                    ac_counter = 64;
                }
                else if (runs_of_zero == 0x0f)
                {
                    ac_counter += 16;
                }
            }
            else
            {
                ac_counter += runs_of_zero;

                _gx_image_reader_jpeg_bits_get(jpeg_info, catogory, (UINT *)&ac_coefficient);
                _gx_image_reader_jpeg_bits_skip(catogory);

                ac_coefficient = (INT)(((UINT)ac_coefficient) >> (32 - catogory));

                negative = !(ac_coefficient >> (catogory - 1));

                if (negative)
                {
                    ac_coefficient += 1 - (1 << catogory);
                }

                if (ac_counter < 64)
                {
                    jpeg_info -> gx_jpeg_vecter[ac_counter++] = ac_coefficient;
                }
            }
        }
        else
        {
            return GX_FAILURE;
        }
    }

    return GX_SUCCESS;
}

/* Define the triple Bufferfly Addition operation */
#define TRIPPLE_BUTTERFLY_ADDITION(a, b, c, d, r)  p = a + b, n = a - b, a = p + c + r, b = n + d + r, c = p - c + r, d = n - d + r

/* Define the butterfly Multiplication */
#define BUTTERFLY_MULTIPLICATION(a, b, k1, k2, sh) n = k1 * (a + b), p = a, a = (n + (k2 - k1) * b) >> sh, b = (n - (k2 + k1) * p) >> sh

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_jpeg_1d_idct                       PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Performs 1D Inverse Discrete Consine Transformation.  It is an      */
/*    implementation of LLM (Loeffler, Lighenberg, Moschytz) algorithm.   */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    input_data                            Input data                    */
/*    output_data                           Buffer for output data        */
/*    post_scale                            Post scale value              */
/*    round                                 Value to reduce round error   */
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
/*    _gx_image_reader_jpeg_2d_idct                                       */
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
static VOID _gx_image_reader_jpeg_1d_idct(INT *input_data, INT *output_data, INT post_scale, INT round)
{
INT p;
INT n;

    /* Prescale */
    input_data[0] <<= 9;
    input_data[1] <<= 7;
    input_data[3] *= 181;
    input_data[4] <<= 9;
    input_data[5] *= 181;
    input_data[7] <<= 7;

    /* iDCT computation .*/
    BUTTERFLY_MULTIPLICATION(input_data[6], input_data[2], 277, 669, 0);
    TRIPPLE_BUTTERFLY_ADDITION(input_data[0], input_data[4], input_data[6], input_data[2], round);
    TRIPPLE_BUTTERFLY_ADDITION(input_data[1], input_data[7], input_data[3], input_data[5], 0);
    BUTTERFLY_MULTIPLICATION(input_data[5], input_data[3], 251, 50, 6);
    BUTTERFLY_MULTIPLICATION(input_data[1], input_data[7], 213, 142, 6);

    /* Post-scale */
    output_data[0] = (input_data[0] + input_data[1]) >> post_scale;
    output_data[8] = (input_data[4] + input_data[5]) >> post_scale;
    output_data[16] = (input_data[2] + input_data[3]) >> post_scale;
    output_data[24] = (input_data[6] + input_data[7]) >> post_scale;
    output_data[32] = (input_data[6] - input_data[7]) >> post_scale;
    output_data[40] = (input_data[2] - input_data[3]) >> post_scale;
    output_data[48] = (input_data[4] - input_data[5]) >> post_scale;
    output_data[56] = (input_data[0] - input_data[1]) >> post_scale;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_jpeg_2d_idct                       PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Performs 2D Inverse Discrete Consine Transformation.                */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    block                                 Input data                    */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*     _gx_image_reader_jpeg_1d_idct        Perform 1D Inverse Discrete   */
/*                                            Consine Transformation      */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*     _gx_image_reader_jpeg_dequantize_idct                              */
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
static VOID _gx_image_reader_jpeg_2d_idct(INT *block)
{
INT i;
INT temp_block[64];

    for (i = 0; i < 8; i++)
    {
        _gx_image_reader_jpeg_1d_idct(block + i * 8, temp_block + i, 9, 512); /* row */
    }

    for (i = 0; i < 8; i++)
    {
        _gx_image_reader_jpeg_1d_idct(temp_block + i * 8, block + i, 12, 2048); /* col */
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_jpeg_dequantize_idct               PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Dequatilizes decoded data and performs Inverse Discrete Consine     */
/*    Transformation.                                                     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    jpeg_info                             JPEG data control block       */
/*    data                                  Pointer to decoded data       */
/*    i_component                           Component index               */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_image_reader_jpeg_2d_idct         Perform 2D Inverse Discrete   */
/*                                            Consine Transformation      */
/*    _gx_image_reader_jpeg_range           Limit value in range [0, 255] */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_image_reader_jpeg_one_block_decode                              */
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
static void _gx_image_reader_jpeg_dequantize_idct(GX_JPEG_INFO *jpeg_info, GX_UBYTE *data, UINT i_component)
{
INT       table_index;
INT       stride = ((jpeg_info -> gx_jpeg_sample_factor[i_component] & 0xf0) >> 1);
GX_UBYTE *outptr = data;
INT       index;
INT       x;
INT       y;
INT       jpeg_block[64];

    table_index = jpeg_info -> gx_jpeg_qantization_table_index[i_component];

    for (index = 0; index < 64; index++)
    {
        jpeg_info -> gx_jpeg_vecter[index] *= jpeg_info -> gx_jpeg_quantization_table[table_index][index];

        /* Reorder from zig-zag order to 8*8 block */
        jpeg_block[_gx_jpg_reorder_index[index]] = jpeg_info -> gx_jpeg_vecter[index];
    }

    _gx_image_reader_jpeg_2d_idct(jpeg_block);

    for (y = 0; y < 8; y++)
    {
        for (x = 0; x < 8; x++)
        {
            outptr[x] = _gx_image_reader_jpeg_range(jpeg_block[y * 8 + x] + 128);
        }

        outptr += stride;
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_jpeg_one_block_decode              PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Decode one block of JPEG data.                                      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    jpeg_info                             JPEG data control block       */
/*    icomponent                            Component index               */
/*    block_data                            Pointer to decoded data       */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Status Code                                                         */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    memset                                                              */
/*    _gx_image_reader_jpeg_dc_decode       Decode dc component           */
/*    _gx_image_reader_jpeg_ac_decode       Decode ac component           */
/*    _gx_image_reader_jpeg_dequantize_idct Dequatilize decoded data      */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_image_reader_jpeg_decode                                        */
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
static UINT _gx_image_reader_jpeg_one_block_decode(GX_JPEG_INFO *jpeg_info, UINT i_component, GX_UBYTE *block_data)
{
    memset(jpeg_info -> gx_jpeg_vecter, 0, 64 * sizeof(UINT));
    _gx_image_reader_jpeg_dc_decode(jpeg_info, i_component);
    _gx_image_reader_jpeg_ac_decode(jpeg_info, i_component);

    _gx_image_reader_jpeg_dequantize_idct(jpeg_info, block_data, i_component);

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_jpeg_one_mcu_write                 PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Write decoded data of one MCU block into specified memory.          */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    jpeg_info                             JPEG control block            */
/*    xpos                                  X position in image           */
/*    ypos                                  y position in image           */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Status Code                                                         */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_image_reader_jpeg_YCbCr2RGB_888  Convert YCbCr value to 888RGB  */
/*                                           color space                  */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_image_reader_jpeg_decode                                        */
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
static UINT _gx_image_reader_jpeg_one_mcu_write(GX_JPEG_INFO *jpeg_info, INT xpos, INT ypos)
{
GX_UBYTE *put;
INT       x;
INT       y;
INT       w;
INT       h;
INT       coff;
INT       Y;
INT       Cb;
INT       Cr;
INT       width_in_bytes;

    h = (jpeg_info -> gx_jpeg_sample_factor[0] >> 4);
    w = (jpeg_info -> gx_jpeg_sample_factor[0] & 0x0f);

    width_in_bytes = jpeg_info -> gx_jpeg_width * 3;
    put = (GX_UBYTE *)jpeg_info -> gx_jpeg_decoded_data;
    put += ypos * width_in_bytes;
    put += xpos * 3;

    for (y = 0; y < 8 * h; y++)
    {
        if (ypos + y >= jpeg_info -> gx_jpeg_height)
        {
            break;
        }

        for (x = 0; x < 8 * w; x++)
        {
            if (xpos + x >= jpeg_info -> gx_jpeg_width)
            {
                break;
            }
            coff = x / w + ((y / h) << 3);

            Y = jpeg_info -> gx_jpeg_Y_block[x + y * w * 8];
            Cb = jpeg_info -> gx_jpeg_Cb_block[coff];
            Cr = jpeg_info -> gx_jpeg_Cr_block[coff];

            _gx_image_reader_jpeg_YCbCr2RGB_888(Y, Cb, Cr, put + x * 3);
        }

        put += width_in_bytes;
    }

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_jpeg_decompress                    PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Decompress JPG data stream.                                         */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    jpeg_info                             JPEG control block            */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Status Code                                                         */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_image_reader_jpeg_one_block_decode                              */
/*                                          Decode one blcok of jpeg data */
/*    _gx_image_reader_jpeg_one_mcu_write   Write decoded data to         */
/*                                            specified memory            */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_image_reader_jpeg_decode                                        */
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
static UINT _gx_image_reader_jpeg_decompress(GX_JPEG_INFO *jpeg_info)
{
int h;
int w;
int x;
int y;
int xx;
int yy;

    _gx_jpg_bit_buffer = 0;
    _gx_jpg_bit_count = 0;

    h = jpeg_info -> gx_jpeg_height * 3;
    w = jpeg_info -> gx_jpeg_width * 3;

    h = (jpeg_info -> gx_jpeg_sample_factor[0] >> 4);
    w = (jpeg_info -> gx_jpeg_sample_factor[0] & 0x0f);

    if (jpeg_info -> gx_jpeg_mcu_draw == GX_NULL)
    {
        /* Safe int math is not required here, max width and height are limited to 14 bits so
           overflow cannot occur. */
        jpeg_info -> gx_jpeg_decoded_data_size = (UINT)(jpeg_info -> gx_jpeg_width * jpeg_info -> gx_jpeg_height * 3);

        /* Allocate memory to load decoded data. */
        jpeg_info -> gx_jpeg_decoded_data = (GX_UBYTE *)_gx_system_memory_allocator((ULONG)jpeg_info -> gx_jpeg_decoded_data_size);

        if (jpeg_info -> gx_jpeg_decoded_data == GX_NULL)
        {
            return GX_SYSTEM_MEMORY_ERROR;
        }
    }

    for (y = 0; y < jpeg_info -> gx_jpeg_height; y += 8 * h)
    {
        for (x = 0; x < jpeg_info -> gx_jpeg_width; x += 8 * w)
        {
            /* Decode one MCU*/
            for (yy = 0; yy < h; yy++)
            {
                for (xx = 0; xx < w; xx++)
                {
                    /* Y */
                    _gx_image_reader_jpeg_one_block_decode(jpeg_info, 0, jpeg_info -> gx_jpeg_Y_block + yy * 128 + xx * 8);
                }
            }

            if (jpeg_info -> gx_jpeg_num_of_components > 1)
            {
                /* Cb */
                _gx_image_reader_jpeg_one_block_decode(jpeg_info, 1, jpeg_info -> gx_jpeg_Cb_block);

                /* Cr*/
                _gx_image_reader_jpeg_one_block_decode(jpeg_info, 2, jpeg_info -> gx_jpeg_Cr_block);
            }

            if (jpeg_info -> gx_jpeg_mcu_draw)
            {
                if (jpeg_info -> gx_jpeg_draw_context)
                {
                    jpeg_info -> gx_jpeg_mcu_draw(jpeg_info,
                                                  jpeg_info -> gx_jpeg_draw_xpos + x,
                                                  jpeg_info -> gx_jpeg_draw_ypos + y);
                }
            }
            else
            {
                _gx_image_reader_jpeg_one_mcu_write(jpeg_info, x, y);
            }
        }
    }

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_jpeg_decode_blocks                 PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function decode a jpg format image.                            */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    jpeg_info                             Jpeg decode control block     */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_image_reader_jpeg_quantization_table_set                        */
/*                                          Set jpeg quantization table   */
/*    _gx_image_reader_jpeg_frame_header_read                             */
/*                                          Read frame header information */
/*    _gx_image_reader_huffman_table_set    Set up huffman table          */
/*    _gx_image_reader_jpeg_scan_header_read                              */
/*                                          Read scan header information  */
/*    _gx_image_reader_jpeg_decompress      Decompress jped data stream   */
/*    _gx_system_memory_free                Application defined memory    */
/*                                            free function               */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_image_reader_jpeg_decode                                        */
/*    _gx_image_reader_jpeg_mcu_decode                                    */
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
static UINT _gx_image_reader_jpeg_decode_blocks(GX_JPEG_INFO *jpeg_info)
{
GX_UBYTE *jpeg_data;
GX_UBYTE  marker;
UINT      segment_len;
UINT      index;
UINT      table_class;
UINT      status = GX_SUCCESS;

    if (jpeg_info -> gx_jpeg_data == GX_NULL || jpeg_info -> gx_jpeg_data_size < 10)
    {
        return GX_INVALID_VALUE;
    }

    jpeg_data = jpeg_info -> gx_jpeg_data;

    /* Read JPEG File flag that mark the start of a JPEG image. */
    if ((*jpeg_data++ != 0xff) || (*jpeg_data++ != 0xd8))
    {
        return GX_INVALID_FORMAT; /*Not a jpeg file */
    }

    jpeg_info -> gx_jpeg_data_index += 2;

    while ((jpeg_info -> gx_jpeg_data_index + 3 < jpeg_info -> gx_jpeg_data_size) &&
           (status == GX_SUCCESS))
    {
        jpeg_data = (jpeg_info -> gx_jpeg_data + jpeg_info -> gx_jpeg_data_index);
        marker = *(jpeg_data + 1);

        if ((*jpeg_data == 0xff) && (marker != 0) && (marker != 0xff))
        {
            jpeg_data += 2;
            jpeg_info -> gx_jpeg_data_index += 2;

            /* Read WORD length */
            segment_len = *(jpeg_data);
            segment_len <<= 8;
            segment_len |= *(jpeg_data + 1);

            if ((UINT)jpeg_info -> gx_jpeg_data_index + segment_len > (UINT)jpeg_info -> gx_jpeg_data_size)
            {
                /* Invalid data size. */
                status = GX_FAILURE;
                break;
            }

            switch (marker)
            {
            case 0xdb:
                /* Define Quantization Table */
                status = _gx_image_reader_jpeg_quantization_table_set(jpeg_info, segment_len);
                break;

            case 0xc0:
                /* Start of Frame */
                status = _gx_image_reader_jpeg_frame_header_read(jpeg_info, segment_len);
                break;

            case 0xc4:
                /* Define Huffman Table */
                status = _gx_image_reader_huffman_table_set(jpeg_info, segment_len);
                break;

            case 0xdd:
                /* Read restart interval which specifies the number of MCU in restart interval. */
                jpeg_data += 2;
                jpeg_info -> gx_jpeg_restart_interval = *jpeg_data++;
                jpeg_info -> gx_jpeg_restart_interval <<= 8;
                jpeg_info -> gx_jpeg_restart_interval |= *jpeg_data++;
                jpeg_info -> gx_jpeg_data_index += (INT)segment_len;
                break;

            case 0xda:
                /* Start of Scan, stores which Huffman tables are associated with which components
                   The program start decoding the data section directly after it reads in this header. */
                _gx_image_reader_jpeg_scan_header_read(jpeg_info, segment_len);

                /* Start decoding jpeg data stream. */
                status = _gx_image_reader_jpeg_decompress(jpeg_info);
                break;

            default:
                /* Unkown marker, skip */
                jpeg_info -> gx_jpeg_data_index += (INT)segment_len;
                break;
            }
        }
        else
        {
            jpeg_info -> gx_jpeg_data_index++;
        }
    }

    for (table_class = 0; table_class < 2; table_class++)
    {
        for (index = 0; index < 2; index++)
        {
            if (jpeg_info -> gx_jpeg_huffman_table[table_class][index])
            {
                _gx_system_memory_free(jpeg_info -> gx_jpeg_huffman_table[table_class][index]);
            }
        }
    }

    return status;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_jpeg_decode                        PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function decodes a jpg format image and saves the decoded data */
/*    to a GX_PIXELMAP structure.                                         */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    read_data                             Input JPEG data stream        */
/*    data_size                             JPEG size in bytes            */
/*    outmap                                Output pixelmap               */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_image_reader_jpeg_decode_blocks   Decode a jpeg format image    */
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
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
UINT _gx_image_reader_jpeg_decode(GX_CONST GX_UBYTE *read_data, ULONG data_size,
                                  GX_PIXELMAP *outmap)
{
UINT          status;
GX_JPEG_INFO *jpeg_info;


    if ((!_gx_system_memory_allocator) || (!_gx_system_memory_free))
    {
        return GX_SYSTEM_MEMORY_ERROR;
    }

    jpeg_info = (GX_JPEG_INFO *)_gx_system_memory_allocator(sizeof(GX_JPEG_INFO));

    if (!jpeg_info)
    {
        return GX_SYSTEM_MEMORY_ERROR;
    }

    memset(jpeg_info, 0, sizeof(GX_JPEG_INFO));

    jpeg_info -> gx_jpeg_data = (GX_UBYTE *)read_data;
    jpeg_info -> gx_jpeg_data_size = (INT)data_size;
    jpeg_info -> gx_jpeg_data_index = 0;
    jpeg_info -> gx_jpeg_draw_context = GX_NULL;

    status = _gx_image_reader_jpeg_decode_blocks(jpeg_info);

    if (status == GX_SUCCESS)
    {
        outmap -> gx_pixelmap_data = jpeg_info -> gx_jpeg_decoded_data;
        outmap -> gx_pixelmap_data_size = jpeg_info -> gx_jpeg_decoded_data_size;
        outmap -> gx_pixelmap_width = (GX_VALUE)jpeg_info -> gx_jpeg_width;
        outmap -> gx_pixelmap_height = (GX_VALUE)jpeg_info -> gx_jpeg_height;
        outmap -> gx_pixelmap_flags = GX_PIXELMAP_RAW_FORMAT;
        outmap -> gx_pixelmap_format = GX_IMAGE_FORMAT_24BPP;
    }

    _gx_system_memory_free((void *)jpeg_info);

    return status;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_jpeg_mcu_decode                    PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function decodes a jpg format image and draw it to canvas      */
/*    directly.                                                           */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    read_data                             Input JPEG data               */
/*    data_size                             JPEG size in bytes            */
/*    context                               Drawing contex                */
/*    xpos                                  X-coord of draw start point in*/
/*                                            canvas                      */
/*    ypos                                  Y-coord of draw start point in*/
/*                                            canvas                      */
/*    draw_function                         Callback of one mcu draw      */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_image_reader_jpeg_decode_blocks   Decode a jpeg format image    */
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
UINT _gx_image_reader_jpeg_mcu_decode(GX_CONST GX_UBYTE *read_data, ULONG data_size,
                                      GX_DRAW_CONTEXT *context, INT xpos, INT ypos,
                                      UINT(draw_function)(GX_JPEG_INFO *, INT, INT))
{
UINT          status;
GX_JPEG_INFO *jpeg_info;

    if ((!_gx_system_memory_allocator) || (!_gx_system_memory_free))
    {
        return GX_SYSTEM_MEMORY_ERROR;
    }

    jpeg_info = (GX_JPEG_INFO *)_gx_system_memory_allocator(sizeof(GX_JPEG_INFO));

    if (!jpeg_info)
    {
        return GX_SYSTEM_MEMORY_ERROR;
    }

    memset(jpeg_info, 0, sizeof(GX_JPEG_INFO));

    jpeg_info -> gx_jpeg_data = (GX_UBYTE *)read_data;
    jpeg_info -> gx_jpeg_data_size = (INT)data_size;
    jpeg_info -> gx_jpeg_data_index = 0;
    jpeg_info -> gx_jpeg_draw_context = context;
    jpeg_info -> gx_jpeg_mcu_draw = draw_function;
    jpeg_info -> gx_jpeg_draw_xpos = xpos;
    jpeg_info -> gx_jpeg_draw_ypos = ypos;

    status = _gx_image_reader_jpeg_decode_blocks(jpeg_info);

    _gx_system_memory_free((void *)jpeg_info);

    return status;
}
#endif

