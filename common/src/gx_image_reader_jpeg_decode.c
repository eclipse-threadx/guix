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

#define GX_SATURATE_TO_UBYTE(result, i) \
    {                                   \
        if ((i) < 0)                    \
        {                               \
            (result) = 0;               \
        }                               \
        else if ((i) > 255)             \
        {                               \
            (result) = 255;             \
        }                               \
        else                            \
        {                               \
            (result) = (GX_UBYTE)(i);   \
        }                               \
    }

#define GX_SATURATE_TO_BYTE(result, i) \
    {                                  \
        if ((i) < -128)                \
        {                              \
            (result) = -128;           \
        }                              \
        else if ((i) > 127)            \
        {                              \
            (result) = 127;            \
        }                              \
        else                           \
        {                              \
            (result) = (GX_BYTE)(i);   \
        }                              \
    }

#define GX_JPEG_BITS_GET(jpeg_info, num_of_bits)                                                                                                                    \
    while (jpeg_info -> gx_jpeg_bit_count <= num_of_bits)                                                                                                           \
    {                                                                                                                                                               \
        if ((jpeg_info -> gx_jpeg_data_index < jpeg_info -> gx_jpeg_data_size) && (jpeg_info -> gx_jpeg_bit_count <= 24))                                           \
        {                                                                                                                                                           \
            jpeg_info -> gx_jpeg_bit_buffer |= ((UINT)(jpeg_info -> gx_jpeg_data[jpeg_info -> gx_jpeg_data_index]) << (UINT)(24 - jpeg_info -> gx_jpeg_bit_count)); \
                                                                                                                                                                    \
            /* 2 byte 'FF00' sequence should be considered as just a byte 0xFF. */                                                                                  \
            if ((jpeg_info -> gx_jpeg_data[jpeg_info -> gx_jpeg_data_index] == 0xff) &&                                                                             \
                (jpeg_info -> gx_jpeg_data_index + 1 < jpeg_info -> gx_jpeg_data_size) &&                                                                           \
                (jpeg_info -> gx_jpeg_data[jpeg_info -> gx_jpeg_data_index + 1] == 0x00))                                                                           \
            {                                                                                                                                                       \
                jpeg_info -> gx_jpeg_data_index += 2;                                                                                                               \
            }                                                                                                                                                       \
            else                                                                                                                                                    \
            {                                                                                                                                                       \
                jpeg_info -> gx_jpeg_data_index += 1;                                                                                                               \
            }                                                                                                                                                       \
        }                                                                                                                                                           \
                                                                                                                                                                    \
        jpeg_info -> gx_jpeg_bit_count += 8;                                                                                                                        \
    }

#define GX_JPEG_BITS_SKIP(jpeg_info, skip_bits)        \
    (jpeg_info) -> gx_jpeg_bit_buffer <<= (skip_bits); \
    (jpeg_info) -> gx_jpeg_bit_count -= (skip_bits);

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_dislay_driver_jpeg_quantization_table_set       PORTABLE C      */
/*                                                           6.2.0        */
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
/*  10-31-2022     Kenneth Maxwell          Modified comment(s),          */
/*                                            prevented underflow from    */
/*                                            bad input data,             */
/*                                            resulting in version 6.2.0  */
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

        if (segment_len < 65)
        {
            return GX_INVALID_FORMAT;
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
/*                                                           6.3.0        */
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
/*  10-31-2022     Kenneth Maxwell          Modified comment(s),          */
/*                                            changed bit_count to        */
/*                                            GX_VALUE data type,         */
/*                                            resulting in version 6.2.0  */
/*  10-31-2023     Ting Zhu                 Modified comment(s),          */
/*                                            improved logic,             */
/*                                            resulting in version 6.3.0  */
/*                                                                        */
/**************************************************************************/
static UINT _gx_image_reader_jpeg_huffcode_find(GX_JPEG_INFO *jpeg_info,
                                                UINT table_class,
                                                UINT table_id,
                                                UINT *bit_len,
                                                GX_UBYTE *code_value)
{
GX_UBYTE          index;
USHORT            code;
USHORT            code_index;
GX_HUFFCODE_INFO *code_info;

    for (index = 0; index < 16; index++)
    {
        code_info = &jpeg_info -> gx_jpeg_huffman_code_info[table_class][table_id][index];
        if (code_info -> bits)
        {
            code = (USHORT)((jpeg_info -> gx_jpeg_bit_buffer) >> (32 - code_info -> bits));

            if (code <= code_info -> end)
            {
                code_index = (USHORT)(code_info -> index + code - code_info -> start);
                *bit_len = code_info -> bits;
                *code_value = jpeg_info -> gx_jpeg_huffman_table[table_class][table_id][code_index];
                return GX_SUCCESS;
            }
        }
        else
        {
            break;
        }
    }

    return GX_FAILURE;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_huffman_table_set                  PORTABLE C      */
/*                                                           6.3.0        */
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
/*  10-31-2022     Kenneth Maxwell          Modified comment(s),          */
/*                                            added range test to prevent */
/*                                            underflow,                  */
/*                                            resulting in version 6.2.0  */
/*  10-31-2023     Ting Zhu                 Modified comment(s),          */
/*                                            improved logic,             */
/*                                            resulting in version 6.3.0  */
/*                                                                        */
/**************************************************************************/
static UINT _gx_image_reader_huffman_table_set(GX_JPEG_INFO *jpeg_info, UINT segment_len)
{
GX_UBYTE         *jpeg_data = jpeg_info -> gx_jpeg_data + jpeg_info -> gx_jpeg_data_index;
GX_UBYTE          table_class;
GX_UBYTE          table_id;
GX_UBYTE          bit_count;
UINT              i_bits;
USHORT            i_table_size;
GX_HUFFCODE_INFO *code_info;
INT               index = 0;
USHORT            code = 0;

    /* must have at least one code for each of 16 huffman bit lengths */
    if (segment_len < 19)
    {
        return GX_INVALID_FORMAT;
    }

    /* Minus two-byte length. */
    jpeg_info -> gx_jpeg_data_index += (INT)segment_len;
    segment_len -= 2;
    jpeg_data += 2;

    while (segment_len)
    {
        /* table_calss: 0 DC 1 AC */
        table_class = ((*jpeg_data) >> 4) & 1;
        table_id = (*jpeg_data++) & 0x01;

        if (segment_len < 17)
        {
            return GX_INVALID_FORMAT;
        }
        segment_len -= 17;

        i_table_size = 0;

        index = 0;
        code = 0;

        /* Read the number of Huffman codes for each bit length, from 1 to 16. */
        for (i_bits = 0; i_bits < 16; i_bits++)
        {
            bit_count = *jpeg_data++;

            if (bit_count)
            {
                code_info = &jpeg_info -> gx_jpeg_huffman_code_info[table_class][table_id][index++];
                code_info -> index = i_table_size;
                code_info -> start = code;
                code_info -> end = (USHORT)(code + bit_count - 1);
                code_info -> bits = (GX_UBYTE)(i_bits + 1);
            }
            code = (USHORT)((code + bit_count) << 1);

            i_table_size = (USHORT)(i_table_size + bit_count);
        }

        if (segment_len < i_table_size)
        {
            return GX_INVALID_FORMAT;
        }

        segment_len -= i_table_size;

        /* Load the start address of the specified huffman table. */
        jpeg_info -> gx_jpeg_huffman_table[table_class][table_id] = jpeg_data;
        jpeg_data += i_table_size;
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

    if (jpeg_info -> gx_jpeg_num_of_components > JPG_MAX_COMPONENTS)
    {
        return GX_FAILURE;
    }

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

    if (jpeg_info -> gx_jpeg_num_of_components > JPG_MAX_COMPONENTS)
    {
        return GX_FAILURE;
    }

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
/*                                                           6.3.0        */
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
/*    GX_JPEG_BITS_GET                       Extract a specified number of*/
/*                                             bits from JPEG data stream */
/*    GX_JPEG_BITS_SKIP                      Skips bits from tempory JPEG */
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
/*  10-31-2022     Kenneth Maxwell          Modified comment(s),          */
/*                                            added range test,           */
/*                                            resulting in version 6.2.0  */
/*  10-31-2023     Ting Zhu                 Modified comment(s),          */
/*                                            improved logic,             */
/*                                            resulting in version 6.3.0  */
/*                                                                        */
/**************************************************************************/
static UINT _gx_image_reader_jpeg_dc_decode(GX_JPEG_INFO *jpeg_info, UINT i_component)
{
UINT     i_bits = 0;
GX_UBYTE code_value;
INT      diff;
UINT     table_index = jpeg_info -> gx_jpeg_dc_table_index[i_component];
GX_BOOL  negative;

    if (table_index >= HUFF_TABLE_DIMENSION)
    {
        return GX_FAILURE;
    }

    GX_JPEG_BITS_GET(jpeg_info, 16);

    if (_gx_image_reader_jpeg_huffcode_find(jpeg_info, 0, table_index, &i_bits, &code_value) == 0)
    {
        GX_JPEG_BITS_SKIP(jpeg_info, i_bits);

        if (code_value == 0)
        {
            jpeg_info -> gx_jpeg_vecter[0] = jpeg_info -> gx_jpeg_pre_dc[i_component];
        }
        else
        {

            GX_JPEG_BITS_GET(jpeg_info, code_value);
            diff = (INT)(((UINT)jpeg_info -> gx_jpeg_bit_buffer) >> (32 - code_value));
            GX_JPEG_BITS_SKIP(jpeg_info, code_value);

            negative = !(diff >> (code_value - 1));

            if (negative)
            {
                diff += 1 - (1 << code_value);
            }

            jpeg_info -> gx_jpeg_vecter[0] = diff + jpeg_info -> gx_jpeg_pre_dc[i_component];
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
/*                                                           6.3.0        */
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
/*    GX_JPEG_BITS_GET                       Extract a specified number of*/
/*                                             bits from JPEG data stream */
/*    GX_JPEG_BITS_SKIP                      Skips bits from tempory JPEG */
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
/*  10-31-2022     Kenneth Maxwell          Modified comment(s),          */
/*                                            added range test,           */
/*                                            resulting in version 6.2.0  */
/*  10-31-2023     Ting Zhu                 Modified comment(s),          */
/*                                            improved logic,             */
/*                                            resulting in version 6.3.0  */
/*                                                                        */
/**************************************************************************/
static UINT _gx_image_reader_jpeg_ac_decode(GX_JPEG_INFO *jpeg_info, UINT i_component)
{
UINT     i_bits = 0;
GX_UBYTE code_value;
GX_UBYTE catogory;
GX_UBYTE runs_of_zero;
INT      ac_coefficient;
UINT     ac_counter = 1;
UINT     table_index = jpeg_info -> gx_jpeg_ac_table_index[i_component];
INT      negative;

    if (table_index >= HUFF_TABLE_DIMENSION)
    {
        return GX_FAILURE;
    }

    while (ac_counter < 64)
    {
        i_bits = 0;
        GX_JPEG_BITS_GET(jpeg_info, 16);

        if (_gx_image_reader_jpeg_huffcode_find(jpeg_info, 1, table_index, &i_bits, &code_value) == 0)
        {
            GX_JPEG_BITS_SKIP(jpeg_info, i_bits);

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

                GX_JPEG_BITS_GET(jpeg_info, catogory);
                ac_coefficient = (INT)((jpeg_info -> gx_jpeg_bit_buffer) >> (32 - catogory));
                GX_JPEG_BITS_SKIP(jpeg_info, catogory);

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

#if defined(GX_ENABLE_ARM_HELIUM)
/* Define the triple Bufferfly Addition operation */
#define VBUTTERFLY_ADDITION(a, b) \
    vtemp = vaddq_s32(a, b);      \
    b = vsubq_s32(a, b);          \
    a = vtemp

/* Define the butterfly Multiplication */
#define VBUTTERFLY_MULTIPLICATION(a, b, cos, sin) \
    vtempa = vmulq_n_s32(a, cos);                 \
    vtempb = vmulq_n_s32(b, sin);                 \
    vtemp = vaddq_s32(vtempa, vtempb);            \
    vtempa = vmulq_n_s32(a, sin);                 \
    vtempb = vmulq_n_s32(b, cos);                 \
    b = vsubq_s32(vtempb, vtempa);                \
    a = vtemp

#define VBUTTERFLY_MULTIPLICATION_SHR6(a, b, cos, sin) \
    vtempa = vmulq_n_s32(a, cos);                      \
    vtempb = vmulq_n_s32(b, sin);                      \
    vtemp = vaddq_s32(vtempa, vtempb);                 \
    vtempa = vmulq_n_s32(a, sin);                      \
    vtempb = vmulq_n_s32(b, cos);                      \
    b = vsubq_s32(vtempb, vtempa);                     \
    b = vshrq_n_s32(b, 6);                             \
    a = vshrq_n_s32(vtemp, 6)
#else

/* Define the triple Bufferfly Addition operation */
#define BUTTERFLY_ADDITION(a, b) \
    t = a + b;                   \
    b = a - b;                   \
    a = t

/* Define the butterfly Multiplication */
#define BUTTERFLY_MULTIPLICATION(a, b, cos, sin) \
    t = (a * cos + b * sin);                     \
    b = (b * cos - a * sin);                     \
    a = t

#define BUTTERFLY_MULTIPLICATION_SHR6(a, b, cos, sin) \
    t = (a * cos + b * sin) >> 6;                     \
    b = (b * cos - a * sin) >> 6;                     \
    a = t

#endif

/* Define constants.  */
#define R2_SHR7   181 /* = sqrt(2) << 7.  */
#define C1_SHR8   251 /* = cos(pi/16) << 8.  */
#define S1_SHR8   50  /* = sin(pi/16) << 8.  */
#define C3_SHR8   213 /* = cos(3pi/16)*sqrt(2) << 8.  */
#define S3_SHR8   142 /* = sin(3pi/16)*sqrt(2) << 8.  */
#define C6R2_SHR9 277 /* = cos(pi/16)*sqrt(2) << 9.  */
#define S6R2_SHR9 669 /* = sin(3pi/16)*sqrt(2) << 9.  */
#define POSTSH1   9
#define POSTSH2   12

#if defined(GX_ENABLE_ARM_HELIUM)
/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_jpeg_dequantize_idct               PORTABLE C      */
/*                                                           6.3.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Ting Zhu, Microsoft Corporation                                     */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Dequatilizes decoded data and performs Inverse Discrete Consine     */
/*    Transformation using helium intrinsics.                             */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    block                                 Pointer to decoded data       */
/*    quant_table                           Pointer to quantization table */
/*    out                                   Buffer for output data        */
/*    stride                                Stride of output data         */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    vldrwq_gather_shifted_offset_s32      Gather data from memory       */
/*    vmulq_s32                             Multiply two vectors          */
/*    vmulq_n_s32                           Multiply by scaler            */
/*    vqrshlq_n_s32                         Shift each element of a       */
/*                                            vector register left by     */
/*                                            the immediate value         */
/*    vaddq_s32                             Add two vectors               */
/*    vrshrq_n_s32                          Shift each element of a       */
/*                                            vector register right by    */
/*                                            the immediate value         */
/*    vstrhq_scatter_offset_s32             Scatter store data to memory  */
/*    vldrhq_s32                            Load vector register          */
/*    vqrshrnbq_n_s32                       Shift right with saturation,  */
/*                                            and write the result to the */
/*                                            bottom half of the result   */
/*                                            element                     */
/*    vqmovntq_s16                          Saturate to half width and    */
/*                                            write the result to the     */
/*                                            top of the result element   */
/*    vstrbq_scatter_offset_s8              Scatter store data to memory  */
/*    VBUTTERFLY_ADDITION                   Perform butterfly addition    */
/*    VBUTTERFLY_MULTIPLICATION             Perform butterfly             */
/*                                            multiplication              */
/*    VBUTTERFLY_MULTIPLICATION_SHR6        Perform butterfly             */
/*                                            multiplication with shift   */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_image_reader_jpeg_one_block_decode                              */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  10-31-2023     Ting Zhu                 Initial Version 6.3.0         */
/*                                                                        */
/**************************************************************************/
static VOID _gx_image_reader_jpeg_dequantize_idct(INT *block, INT *quant_table, GX_BYTE *out, INT stride)
{
static uint32x4_t voffset[8][2] = {
    {{0,  2,  3,  9}, {10, 20, 21, 35}},
    {{1,  4,  8,  11}, {19, 22, 34, 36}},
    {{5,  7,  12, 18}, {23, 33, 37, 48}},
    {{6,  13, 17, 24}, {32, 38, 47, 49}},
    {{14, 16, 25, 31}, {39, 46, 50, 57}},
    {{15, 26, 30, 40}, {45, 51, 56, 58}},
    {{27, 29, 41, 44}, {52, 55, 59, 62}},
    {{28, 42, 43, 53}, {54, 60, 61, 63}}
};
static uint32x4_t vstroffset1= {0, 16, 32, 48};
INT               index;
GX_VALUE          temp_block[64];
GX_VALUE         *output_data;
int32x4_t         vrow0, vrow1,  vrow2, vrow3, vrow4, vrow5, vrow6, vrow7, vtemp, vtempa, vtempb;
uint8x16_t        vstroffset = {0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3};
int16x8_t         vtemp16;
int8x16_t         vtemp8;
int16_t const    *base;

    /* Perform 2-D DCT by applying first a 1-D DCT over the rows
       followed by a 1-D DCT over the columns of the input-data matrix.*/
    for (index = 0; index < 4; index++)
    {
        vstroffset[0 + index * 4] += (stride * 3);
        vstroffset[1 + index * 4] += stride;
        vstroffset[2 + index * 4] += (stride << 1);
    }


    for (index = 0; index < 2; index++)
    {
        output_data = temp_block + 32 * index;

        /* Load data. */
        vrow0 = vldrwq_gather_shifted_offset_s32(block, voffset[0][index]);
        vtemp = vldrwq_gather_shifted_offset_s32(quant_table, voffset[0][index]);
        vrow0 = vmulq_s32(vrow0, vtemp);

        vrow1 = vldrwq_gather_shifted_offset_s32(block, voffset[1][index]);
        vtemp = vldrwq_gather_shifted_offset_s32(quant_table, voffset[1][index]);
        vrow1 = vmulq_s32(vrow1, vtemp);

        vrow2 = vldrwq_gather_shifted_offset_s32(block, voffset[2][index]);
        vtemp = vldrwq_gather_shifted_offset_s32(quant_table, voffset[2][index]);
        vrow2 = vmulq_s32(vrow2, vtemp);

        vrow3 = vldrwq_gather_shifted_offset_s32(block, voffset[3][index]);
        vtemp = vldrwq_gather_shifted_offset_s32(quant_table, voffset[3][index]);
        vrow3 = vmulq_s32(vrow3, vtemp);

        vrow4 = vldrwq_gather_shifted_offset_s32(block, voffset[4][index]);
        vtemp = vldrwq_gather_shifted_offset_s32(quant_table, voffset[4][index]);
        vrow4 = vmulq_s32(vrow4, vtemp);

        vrow5 = vldrwq_gather_shifted_offset_s32(block, voffset[5][index]);
        vtemp = vldrwq_gather_shifted_offset_s32(quant_table, voffset[5][index]);
        vrow5 = vmulq_s32(vrow5, vtemp);

        vrow6 = vldrwq_gather_shifted_offset_s32(block, voffset[6][index]);
        vtemp = vldrwq_gather_shifted_offset_s32(quant_table, voffset[6][index]);
        vrow6 = vmulq_s32(vrow6, vtemp);

        vrow7 = vldrwq_gather_shifted_offset_s32(block, voffset[7][index]);
        vtemp = vldrwq_gather_shifted_offset_s32(quant_table, voffset[7][index]);
        vrow7 = vmulq_s32(vrow7, vtemp);

        /*  Prescale.  */
        vrow0 = vqrshlq_n_s32(vrow0, 9);
        vrow1 = vqrshlq_n_s32(vrow1, 7);
        vrow4 = vqrshlq_n_s32(vrow4, 9);
        vrow7 = vqrshlq_n_s32(vrow7, 7);

        /* stage 1.  */
        VBUTTERFLY_ADDITION(vrow1, vrow7);

        vrow3 = vmulq_n_s32(vrow3, R2_SHR7);
        vrow5 = vmulq_n_s32(vrow5, R2_SHR7);

        /* stage 2.  */
        VBUTTERFLY_ADDITION(vrow0, vrow4);
        VBUTTERFLY_MULTIPLICATION(vrow6, vrow2, C6R2_SHR9, S6R2_SHR9);
        VBUTTERFLY_ADDITION(vrow7, vrow5);
        VBUTTERFLY_ADDITION(vrow1, vrow3);

        /* stage 3.  */
        VBUTTERFLY_ADDITION(vrow0, vrow6);
        VBUTTERFLY_ADDITION(vrow4, vrow2);
        VBUTTERFLY_MULTIPLICATION_SHR6(vrow5, vrow3, C1_SHR8, S1_SHR8);
        VBUTTERFLY_MULTIPLICATION_SHR6(vrow1, vrow7, C3_SHR8, S3_SHR8);

        /* stage 4.  */
        vtemp = vaddq_s32(vrow0, vrow1);
        vtemp = vrshrq_n_s32(vtemp, POSTSH1);
        vstrhq_scatter_offset_s32(output_data, vstroffset1, vtemp);

        vtemp = vaddq_s32(vrow4, vrow5);
        vtemp = vrshrq_n_s32(vtemp, POSTSH1);
        vstrhq_scatter_offset_s32(output_data + 1, vstroffset1, vtemp);

        vtemp = vaddq_s32(vrow2, vrow3);
        vtemp = vrshrq_n_s32(vtemp, POSTSH1);
        vstrhq_scatter_offset_s32(output_data + 2, vstroffset1, vtemp);

        vtemp = vaddq_s32(vrow6, vrow7);
        vtemp = vrshrq_n_s32(vtemp, POSTSH1);
        vstrhq_scatter_offset_s32(output_data + 3, vstroffset1, vtemp);

        vtemp = vsubq_s32(vrow6, vrow7);
        vtemp = vrshrq_n_s32(vtemp, POSTSH1);
        vstrhq_scatter_offset_s32(output_data + 4, vstroffset1, vtemp);

        vtemp = vsubq_s32(vrow2, vrow3);
        vtemp = vrshrq_n_s32(vtemp, POSTSH1);
        vstrhq_scatter_offset_s32(output_data + 5, vstroffset1, vtemp);

        vtemp = vsubq_s32(vrow4, vrow5);
        vtemp = vrshrq_n_s32(vtemp, POSTSH1);
        vstrhq_scatter_offset_s32(output_data + 6, vstroffset1, vtemp);

        vtemp = vsubq_s32(vrow0, vrow1);
        vtemp = vrshrq_n_s32(vtemp, POSTSH1);
        vstrhq_scatter_offset_s32(output_data + 7, vstroffset1, vtemp);
    }

    for (index = 0; index < 2; index++)
    {
        base = (int16_t const *)(temp_block + 4 * index);

        /* Load data. */
        vrow0 = vldrhq_s32(base);
        vrow1 = vldrhq_s32(base + 8);
        vrow2 = vldrhq_s32(base + 16);
        vrow3 = vldrhq_s32(base + 24);
        vrow4 = vldrhq_s32(base + 32);
        vrow5 = vldrhq_s32(base + 40);
        vrow6 = vldrhq_s32(base + 48);
        vrow7 = vldrhq_s32(base + 56);

        /*  Prescale.  */
        vrow0 = vqrshlq_n_s32(vrow0, 9);
        vrow1 = vqrshlq_n_s32(vrow1, 7);
        vrow4 = vqrshlq_n_s32(vrow4, 9);
        vrow7 = vqrshlq_n_s32(vrow7, 7);

        /* stage 1.  */
        VBUTTERFLY_ADDITION(vrow1, vrow7);

        vrow3 = vmulq_n_s32(vrow3, R2_SHR7);
        vrow5 = vmulq_n_s32(vrow5, R2_SHR7);

        /* stage 2.  */
        VBUTTERFLY_ADDITION(vrow0, vrow4);
        VBUTTERFLY_MULTIPLICATION(vrow6, vrow2, C6R2_SHR9, S6R2_SHR9);
        VBUTTERFLY_ADDITION(vrow7, vrow5);
        VBUTTERFLY_ADDITION(vrow1, vrow3);

        /* stage 3.  */
        VBUTTERFLY_ADDITION(vrow0, vrow6);
        VBUTTERFLY_ADDITION(vrow4, vrow2);
        VBUTTERFLY_MULTIPLICATION_SHR6(vrow5, vrow3, C1_SHR8, S1_SHR8);
        VBUTTERFLY_MULTIPLICATION_SHR6(vrow1, vrow7, C3_SHR8, S3_SHR8);

        /* stage 4.  */

        vtemp = vaddq_s32(vrow0, vrow1);
        vtemp16 = vqrshrntq_n_s32(vtemp16, vtemp, POSTSH2);

        vtemp = vaddq_s32(vrow4, vrow5);
        vtemp16 = vqrshrnbq_n_s32(vtemp16, vtemp, POSTSH2);
        vtemp8 = vqmovntq_s16(vtemp8, vtemp16);

        vtemp = vaddq_s32(vrow2, vrow3);
        vtemp16 = vqrshrntq_n_s32(vtemp16, vtemp, POSTSH2);

        vtemp = vaddq_s32(vrow6, vrow7);
        vtemp16 = vqrshrnbq_n_s32(vtemp16, vtemp, POSTSH2);
        vtemp8 = vqmovnbq_s16(vtemp8, vtemp16);
        vstrbq_scatter_offset_s8(out + 4 * index, vstroffset, vtemp8);

        vtemp = vsubq_s32(vrow6, vrow7);
        vtemp16 = vqrshrntq_n_s32(vtemp16, vtemp, POSTSH2);

        vtemp = vsubq_s32(vrow2, vrow3);
        vtemp16 = vqrshrnbq_n_s32(vtemp16, vtemp, POSTSH2);
        vtemp8 = vqmovntq_s16(vtemp8, vtemp16);

        vtemp = vsubq_s32(vrow4, vrow5);
        vtemp16 = vqrshrntq_n_s32(vtemp16, vtemp, POSTSH2);

        vtemp = vsubq_s32(vrow0, vrow1);
        vtemp16 = vqrshrnbq_n_s32(vtemp16, vtemp, POSTSH2);
        vtemp8 = vqmovnbq_s16(vtemp8, vtemp16);
        vstrbq_scatter_offset_s8(out + (stride << 2) + 4 * index, vstroffset, vtemp8);
    }
}
#else

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_jpeg_1d_idct                       PORTABLE C      */
/*                                                           6.3.0        */
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
/*  10-31-2023     Ting Zhu                 Modified comment(s),          */
/*                                            improved logic,             */
/*                                            resulting in version 6.3.0  */
/*                                                                        */
/**************************************************************************/
static VOID _gx_image_reader_jpeg_1d_idct(INT *input_data, INT *output_data, INT post_scale, INT round)
{
INT t;

    input_data[0] <<= 9;
    input_data[1] <<= 7;
    input_data[4] <<= 9;
    input_data[7] <<= 7;

    /* iDCT computation .*/

    /* stage 1.  */
    BUTTERFLY_ADDITION(input_data[1], input_data[7]);

    input_data[3] *= R2_SHR7;
    input_data[5] *= R2_SHR7;

    /* stage 2.  */
    BUTTERFLY_ADDITION(input_data[0], input_data[4]);
    BUTTERFLY_MULTIPLICATION(input_data[6], input_data[2], C6R2_SHR9, S6R2_SHR9);
    BUTTERFLY_ADDITION(input_data[7], input_data[5]);
    BUTTERFLY_ADDITION(input_data[1], input_data[3]);

    /* staget 3.  */
    BUTTERFLY_ADDITION(input_data[0], input_data[6]);
    BUTTERFLY_ADDITION(input_data[4], input_data[2]);
    BUTTERFLY_MULTIPLICATION_SHR6(input_data[5], input_data[3], C1_SHR8, S1_SHR8);
    BUTTERFLY_MULTIPLICATION_SHR6(input_data[1], input_data[7], C3_SHR8, S3_SHR8);

    /* stage 4.  */
    input_data[0] += round;
    input_data[4] += round;
    input_data[2] += round;
    input_data[6] += round;

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
/*    _gx_image_reader_jpeg_dequantize_idct               PORTABLE C      */
/*                                                           6.3.0        */
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
/*    block                                 Pointer to decoded data       */
/*    quant_table                           Pointer to quantization table */
/*    out                                   Buffer for output data        */
/*    stride                                Stride of output data         */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_image_reader_jpeg_1d_idct         Perform 1D Inverse Discrete   */
/*                                            Consine Transformation      */
/*    GX_SATURATE_TO_BYTE                   Saturate to [-128, 127]       */
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
/*  10-31-2022     Kenneth Maxwell          Modified comment(s),          */
/*                                            added range check for       */
/*                                            stride, changed return val, */
/*                                            added range check for       */
/*                                            table_index,                */
/*                                            resulting in version 6.2.0  */
/*  10-31-2023     Ting Zhu                 Modified comment(s),          */
/*                                            improved logic,             */
/*                                            resulting in version 6.3.0  */
/*                                                                        */
/**************************************************************************/
static VOID _gx_image_reader_jpeg_dequantize_idct(INT *block, INT *quant_table, GX_BYTE *out, INT stride)
{
static GX_CONST GX_UBYTE reorder_index[] = {
    0, 1, 8, 16, 9, 2, 3, 10,
    17, 24, 32, 25, 18, 11, 4, 5,
    12, 19, 26, 33, 40, 48, 41, 34,
    27, 20, 13, 6, 7, 14, 21, 28,
    35, 42, 49, 56, 57, 50, 43, 36,
    29, 22, 15, 23, 30, 37, 44, 51,
    58, 59, 52, 45, 38, 31, 39, 46,
    53, 60, 61, 54, 47, 55, 62, 63
};
INT                      index;
INT                      jpeg_block[64];
INT                      temp_block[64];
INT                      row;

    for (index = 0; index < 64; index++)
    {
        block[index] *= quant_table[index];

        /* Reorder from zig-zag order to 8*8 block */
        jpeg_block[reorder_index[index]] = block[index];
    }

    for (index = 0; index < 8; index++)
    {
        _gx_image_reader_jpeg_1d_idct(jpeg_block + index * 8, temp_block + index, 9, 256);     /* row */
    }

    for (index = 0; index < 8; index++)
    {
        _gx_image_reader_jpeg_1d_idct(temp_block + index * 8, jpeg_block + index, 12, 2048);     /* col */
    }

    for (row = 0; row < 8; row++)
    {
        for (index = 0; index < 8; index++)
        {
            GX_SATURATE_TO_BYTE(out[index], jpeg_block[row * 8 + index]);
        }

        out += stride;
    }
}
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_jpeg_one_block_decode              PORTABLE C      */
/*                                                           6.3.0        */
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
/*  10-31-2022     Kenneth Maxwell          Modified comment(s),          */
/*                                            returned result of          */
/*                                            dequantize_idct,            */
/*                                            resulting in version 6.2.0  */
/*  10-31-2023     Ting Zhu                 Modified comment(s),          */
/*                                            improved logic,             */
/*                                            resulting in version 6.3.0  */
/*                                                                        */
/**************************************************************************/
static UINT _gx_image_reader_jpeg_one_block_decode(GX_JPEG_INFO *jpeg_info, UINT i_component, GX_BYTE *block_data)
{
INT table_index;
INT stride;

    memset(jpeg_info -> gx_jpeg_vecter, 0, 64 * sizeof(UINT));
    _gx_image_reader_jpeg_dc_decode(jpeg_info, i_component);
    _gx_image_reader_jpeg_ac_decode(jpeg_info, i_component);

    if (i_component >= JPG_MAX_COMPONENTS)
    {
        return GX_FAILURE;
    }

    stride = ((jpeg_info -> gx_jpeg_sample_factor[i_component] & 0xf0) >> 1);

    if (i_component == 0)
    {
        if (stride > 32)
        {
            return GX_FAILURE;
        }
    }
    else
    {
        if (stride > 8)
        {
            return GX_FAILURE;
        }
    }

    table_index = jpeg_info -> gx_jpeg_qantization_table_index[i_component];

    if (table_index >= JPG_QUANT_TABLE_DIMENSION)
    {
        return GX_FAILURE;
    }

    _gx_image_reader_jpeg_dequantize_idct(jpeg_info -> gx_jpeg_vecter, jpeg_info -> gx_jpeg_quantization_table[table_index], block_data, stride);
    return GX_SUCCESS;
}


#if defined(GX_ENABLE_ARM_HELIUM)
/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_jpeg_24xrgb_pixel_write_helium     PORTABLE C      */
/*                                                           6.3.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Ting Zhu, Microsoft Corporation                                     */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Write 24xrgb pixel to memory using Helium intrinsics.               */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    jpeg_info                             JPEG control block            */
/*    vred                                  Red value vector              */
/*    vgreen                                Green value vector            */
/*    vblue                                 Blue value vector             */
/*    size                                  Number of pixels to write     */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    vstrbq_scatter_offset_u8              Scatter 8-bit values to       */
/*                                            memory                      */
/*    vstrbq_scatter_offset_p_u8            Optionaly scatter 8-bit       */
/*                                            values to memory            */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_image_reader_jpeg_one_mcu_write                                 */
/*    _gx_image_reader_jpeg_one_mcu_rotated_write                         */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  10-31-2023     Ting Zhu                 Initial Version 6.3.0         */
/*                                                                        */
/**************************************************************************/
static VOID _gx_image_reader_jpeg_24xrgb_pixel_write_helium(GX_JPEG_INFO *jpeg_info, uint8x16_t vred, uint8x16_t vgreen, uint8x16_t vblue, INT size)
{
INT index;

    for(index = 0; index < size; index++)
    {
        *((GX_COLOR *)jpeg_info -> gx_jpeg_putdata) = 0xff000000 | ((ULONG)vred[index] << 16) | ((ULONG)vgreen[index] << 8) | (ULONG)vblue[index];

        jpeg_info -> gx_jpeg_putdata += 4;
    }
}
#else
/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_jpeg_1555xrgb_pixel_write          PORTABLE C      */
/*                                                           6.3.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Ting Zhu, Microsoft Corporation                                     */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Write 24xrgb pixel to memory.                                       */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    jpeg_info                             JPEG control block            */
/*    red                                   Red value                     */
/*    green                                 Green value                   */
/*    blue                                  Blue value                    */
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
/*    _gx_image_reader_jpeg_one_mcu_write                                 */
/*    _gx_image_reader_jpeg_one_mcu_rotated_write                         */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  10-31-2023     Ting Zhu                 Initial Version 6.3.0         */
/*                                                                        */
/**************************************************************************/
static VOID _gx_image_reader_jpeg_24xrgb_pixel_write(GX_JPEG_INFO *jpeg_info, GX_UBYTE red, GX_UBYTE green, GX_UBYTE blue)
{
    *((GX_COLOR *)jpeg_info -> gx_jpeg_putdata) = 0xff000000 | ((ULONG)red << 16) | ((ULONG)green << 8) | (ULONG)blue;

    jpeg_info -> gx_jpeg_putdata += 4;
}
#endif

#if defined(GX_ENABLE_ARM_HELIUM)
/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_jpeg_24bpp_pixel_write_helium      PORTABLE C      */
/*                                                           6.3.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Ting Zhu, Microsoft Corporation                                     */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function to write 24xrgb pixel to memory using      */
/*    Helium intrinsics.                                                  */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    jpeg_info                             JPEG control block            */
/*    vred                                  Red value vector              */
/*    vgreen                                Green value vector            */
/*    vblue                                 Blue value vector            */
/*    size                                  Number of pixels to write     */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    vstrbq_scatter_offset_u8              Scatter 8-bit values to       */
/*                                            memory                      */
/*    vstrbq_scatter_offset_p_u8            Optionally scatter 8-bit      */
/*                                            values to memory            */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_image_reader_jpeg_one_mcu_write                                 */
/*    _gx_image_reader_jpeg_one_mcu_rotated_write                         */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  10-31-2023     Ting Zhu                 Initial Version 6.3.0         */
/*                                                                        */
/**************************************************************************/
static VOID _gx_image_reader_jpeg_24bpp_pixel_write_helium(GX_JPEG_INFO *jpeg_info, uint8x16_t vred, uint8x16_t vgreen, uint8x16_t vblue, INT size)
{
static uint8x16_t voffset = {0, 3, 6, 9, 12, 15, 18, 21, 24, 27, 30, 33, 36, 39, 42, 45};
mve_pred16_t      p;

    if (size == 16)
    {
        vstrbq_scatter_offset_u8(jpeg_info -> gx_jpeg_putdata++, voffset, vred);
        vstrbq_scatter_offset_u8(jpeg_info -> gx_jpeg_putdata++, voffset, vgreen);
        vstrbq_scatter_offset_u8(jpeg_info -> gx_jpeg_putdata++, voffset, vblue);
        jpeg_info -> gx_jpeg_putdata += 45;
    }
    else
    {
        /* Write the specified size of RGB values to memory.  */
        p = 0xffff >> (16 - size);
        vstrbq_scatter_offset_p_u8(jpeg_info -> gx_jpeg_putdata++, voffset, vred, p);
        vstrbq_scatter_offset_p_u8(jpeg_info -> gx_jpeg_putdata++, voffset, vgreen, p);
        vstrbq_scatter_offset_p_u8(jpeg_info -> gx_jpeg_putdata++, voffset, vblue, p);
        jpeg_info -> gx_jpeg_putdata += (size - 1) * 3;
    }
}
#else
/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_jpeg_24bpp_pixel_write             PORTABLE C      */
/*                                                           6.3.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Ting Zhu, Microsoft Corporation                                     */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Write 24bpp pixel to memory.                                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    jpeg_info                             JPEG control block            */
/*    red                                   Red value                     */
/*    green                                 Green value                   */
/*    blue                                  Blue value                    */
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
/*    _gx_image_reader_jpeg_one_mcu_write                                 */
/*    _gx_image_reader_jpeg_one_mcu_rotated_write                         */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  10-31-2023     Ting Zhu                 Initial Version 6.3.0         */
/*                                                                        */
/**************************************************************************/
static VOID _gx_image_reader_jpeg_24bpp_pixel_write(GX_JPEG_INFO *jpeg_info, GX_UBYTE red, GX_UBYTE green, GX_UBYTE blue)
{
    *jpeg_info -> gx_jpeg_putdata++ = red;
    *jpeg_info -> gx_jpeg_putdata++ = green;
    *jpeg_info -> gx_jpeg_putdata++ = blue;
}
#endif

#if defined(GX_ENABLE_ARM_HELIUM)
/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_jpeg_565rgb_pixel_write_helium     PORTABLE C      */
/*                                                           6.3.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Ting Zhu, Microsoft Corporation                                     */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function to write 565rgb pixel to memory using      */
/*    Helium intrinsics.                                                  */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    jpeg_info                             JPEG control block            */
/*    vred                                  Red value vector              */
/*    vgreen                                Green value vector            */
/*    vblue                                 Blue value vector            */
/*    size                                  Number of pixels to write     */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    vshrq_n_u8                            Unsigned 8-bit shift right    */
/*    vldrbq_u16                            Load 8-bit value to a         */
/*                                            destination register        */
/*    vshlq_n_u16                           Unsigned 16-bit shift left    */
/*    vorrq_u16                             Unsigned 16-bit OR            */
/*    vstrhq_u16                            Store 16-bit values from      */
/*                                            register to memory          */
/*    vstrhq_p_u16                          Optionally store 16-bit       */
/*                                            values from register to     */
/*                                            memory                      */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_image_reader_jpeg_one_mcu_write                                 */
/*    _gx_image_reader_jpeg_one_mcu_rotated_write                         */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  10-31-2023     Ting Zhu                 Initial Version 6.3.0         */
/*                                                                        */
/**************************************************************************/
static VOID _gx_image_reader_jpeg_565rgb_pixel_write_helium(GX_JPEG_INFO *jpeg_info, uint8x16_t vred, uint8x16_t vgreen, uint8x16_t vblue, INT size)
{
uint16x8_t   vresult;
uint16x8_t   vtemp;
INT          index;
mve_pred16_t p;
uint16_t    *put = (uint16_t *)jpeg_info -> gx_jpeg_putdata;
GX_UBYTE     red[16];
GX_UBYTE     green[16];
GX_UBYTE     blue[16];

    vred = vshrq_n_u8(vred, 3);
    vgreen = vshrq_n_u8(vgreen, 2);
    vblue = vshrq_n_u8(vblue, 3);
    
    vstrbq(red, vred);
    vstrbq(green, vgreen);
    vstrbq(blue, vblue);

    for (index = 0; index <= 8; index += 8)
    {
        vtemp = vldrbq_u16(&red[index]);
        vresult = vshlq_n_u16(vtemp, 11);

        vtemp = vldrbq_u16(&green[index]);
        vtemp = vshlq_n_u16(vtemp, 5);
        vresult = vorrq_u16(vresult, vtemp);

        vtemp = vldrbq_u16(&blue[index]);
        vresult = vorrq_u16(vresult, vtemp);
        
        if (size >= 8)
        {
            vstrhq_u16(put, vresult);
            put += 8;
            size -= 8;
        }
        else
        {
            p = 0xffff >> (16 - (size << 1));
            vstrhq_p_u16(put, vresult, p);
            put += size;
            break;
        }
    }

    jpeg_info -> gx_jpeg_putdata = (GX_UBYTE *)put;
}
#else
/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_jpeg_1555xrgb_pixel_write          PORTABLE C      */
/*                                                           6.3.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Ting Zhu, Microsoft Corporation                                     */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Write 1555xrgb pixel to memory.                                     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    jpeg_info                             JPEG control block            */
/*    red                                   Red value                     */
/*    green                                 Green value                   */
/*    blue                                  Blue value                    */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    GX_SATURATE_TO_5BIT                   Saturate the value to 5 bits  */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_image_reader_jpeg_one_mcu_write                                 */
/*    _gx_image_reader_jpeg_one_mcu_rotated_write                         */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  10-31-2023     Ting Zhu                 Initial Version 6.3.0         */
/*                                                                        */
/**************************************************************************/
static VOID _gx_image_reader_jpeg_565rgb_pixel_write(GX_JPEG_INFO *jpeg_info, GX_UBYTE red, GX_UBYTE green, GX_UBYTE blue)
{
    /* Make sure the range of the RGB values are within bound. */
    red >>= 3;
    green >>= 2;
    blue >>= 3;

    *((USHORT *)jpeg_info -> gx_jpeg_putdata) = (USHORT)((red << 11) | (green << 5 | blue));
    jpeg_info -> gx_jpeg_putdata += 2;
}
#endif

#if defined(GX_ENABLE_ARM_HELIUM)
/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_jpeg_1555xrgb_pixel_write_helium   PORTABLE C      */
/*                                                           6.3.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Ting Zhu, Microsoft Corporation                                     */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function to write 1555xrgb pixel to memory using    */
/*    Helium intrinsics.                                                  */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    jpeg_info                             JPEG control block            */
/*    vred                                  Red value vector              */
/*    vgreen                                Green value vector            */
/*    vblue                                 Blue value vector            */
/*    size                                  Number of pixels to write     */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    vshrq_n_u8                            Unsigned 8-bit shift right    */
/*    vldrbq_u16                            Load 8-bit value to a         */
/*                                            destination register        */
/*    vshlq_n_u16                           Unsigned 16-bit shift left    */
/*    vorrq_u16                             Unsigned 16-bit OR            */
/*    vstrhq_u16                            Store 16-bit values from      */
/*                                            register to memory          */
/*    vstrhq_p_u16                          Optionally store 16-bit       */
/*                                            values from register to     */
/*                                            memory                      */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_image_reader_jpeg_one_mcu_write                                 */
/*    _gx_image_reader_jpeg_one_mcu_rotated_write                         */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  10-31-2023     Ting Zhu                 Initial Version 6.3.0         */
/*                                                                        */
/**************************************************************************/
static VOID _gx_image_reader_jpeg_1555xrgb_pixel_write_helium(GX_JPEG_INFO *jpeg_info, uint8x16_t vred, uint8x16_t vgreen, uint8x16_t vblue, INT size)
{
uint16x8_t   vresult;
uint16x8_t   vtemp;
INT          index;
uint16_t    *put = (uint16_t *)jpeg_info -> gx_jpeg_putdata;
mve_pred16_t p;
GX_UBYTE     red[16];
GX_UBYTE     green[16];
GX_UBYTE     blue[16];

    vred = vshrq_n_u8(vred, 3);
    vgreen = vshrq_n_u8(vgreen, 3);
    vblue = vshrq_n_u8(vblue, 3);

    vstrbq(red, vred);
    vstrbq(green, vgreen);
    vstrbq(blue, vblue);
    
    for (index = 0; index <= 8; index += 8)
    {
        vtemp = vldrbq_u16(&red[index]);
        vresult = vshlq_n_u16(vtemp, 10);

        vtemp = vldrbq_u16(&green[index]);
        vtemp = vshlq_n_u16(vtemp, 5);
        vresult = vorrq_u16(vresult, vtemp);

        vtemp = vldrbq_u16(&blue[index]);
        vresult = vorrq_u16(vresult, vtemp);

        if (size >= 8)
        {
            vstrhq_u16(put, vresult);
            put += 8;
            size -= 8;
        }
        else
        {
            p = 0xffff >> (16 - (size << 1));
            vstrhq_p_u16(put, vresult, p);
            put += size;
            break;
        }
    }

    jpeg_info -> gx_jpeg_putdata = (GX_UBYTE *)put;
}
#else
/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_jpeg_1555xrgb_pixel_write          PORTABLE C      */
/*                                                           6.3.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Ting Zhu, Microsoft Corporation                                     */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Write 1555xrgb pixel to memory.                                     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    jpeg_info                             JPEG control block            */
/*    red                                   Red value                     */
/*    green                                 Green value                   */
/*    blue                                  Blue value                    */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    GX_SATURATE_TO_5BIT                   Saturate the value to 5 bits  */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_image_reader_jpeg_one_mcu_write                                 */
/*    _gx_image_reader_jpeg_one_mcu_rotated_write                         */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  10-31-2023     Ting Zhu                 Initial Version 6.3.0         */
/*                                                                        */
/**************************************************************************/
static VOID _gx_image_reader_jpeg_1555xrgb_pixel_write(GX_JPEG_INFO *jpeg_info, GX_UBYTE red, GX_UBYTE green, GX_UBYTE blue)
{
    /* Make sure the range of the RGB values are within bound. */
    red >>= 3;
    green >>= 3;
    blue >>= 3;

    *((USHORT *)jpeg_info -> gx_jpeg_putdata) = (USHORT)((red << 10) | (green << 5 | blue));
    jpeg_info -> gx_jpeg_putdata += 2;
}
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_jpeg_pixel_write_info_set          PORTABLE C      */
/*                                                           6.3.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Ting Zhu, Microsoft Corporation                                     */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Set information for writing pixel to memory.                        */
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
/*    _gx_image_reader_jpeg_565rgb_pixel_write_helium                     */
/*                                          Write 565rgb pixel to memory  */
/*    _gx_image_reader_jpeg_565rgb_pixel_write                            */
/*                                          Write 565rgb pixel to memory  */
/*    _gx_image_reader_jpeg_1555xrgb_pixel_write_helium                   */
/*                                          Write 1555xrgb pixel to memory*/
/*    _gx_image_reader_jpeg_1555xrgb_pixel_write                          */
/*                                          Write 1555xrgb pixel to memory*/
/*    _gx_image_reader_jpeg_24xrgb_24bpp_pixel_write_helium               */
/*                                          Write 24xrgb or 24rgb pixel   */
/*                                            to memory                   */
/*    _gx_image_reader_jpeg_24xrgb_24bpp_pixel_write                      */
/*                                          Write 24xrgb or 24rgb pixel   */
/*                                            to memory                   */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_image_reader_jpeg_decompress                                    */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  10-31-2023     Ting Zhu                 Initial Version 6.3.0         */
/*                                                                        */
/**************************************************************************/
static UINT _gx_image_reader_jpeg_pixel_write_info_set(GX_JPEG_INFO *jpeg_info)
{
    if (!jpeg_info -> gx_jpeg_output_width)
    {
        jpeg_info -> gx_jpeg_output_width = jpeg_info -> gx_jpeg_width;
        jpeg_info -> gx_jpeg_output_height = jpeg_info -> gx_jpeg_height;

        _gx_utility_rectangle_define(&jpeg_info -> gx_jpeg_output_clip, 0, 0, (GX_VALUE)(jpeg_info -> gx_jpeg_width - 1), (GX_VALUE)(jpeg_info -> gx_jpeg_height - 1));
    }

    jpeg_info -> gx_jpeg_output_stride = jpeg_info -> gx_jpeg_output_rotation_angle == 0 ? jpeg_info -> gx_jpeg_output_width : jpeg_info -> gx_jpeg_output_height;

    /* Set pixel write callback.  */
    switch (jpeg_info -> gx_jpeg_output_color_format)
    {
    case GX_COLOR_FORMAT_565RGB:
#if defined(GX_ENABLE_ARM_HELIUM)
        jpeg_info -> gx_jpeg_pixel_write_helium = _gx_image_reader_jpeg_565rgb_pixel_write_helium;
#else
        jpeg_info -> gx_jpeg_pixel_write = _gx_image_reader_jpeg_565rgb_pixel_write;
#endif
        jpeg_info -> gx_jpeg_output_bpp = 2;
        jpeg_info -> gx_jpeg_output_stride <<= 1;
        break;

    case GX_COLOR_FORMAT_1555XRGB:
#if defined(GX_ENABLE_ARM_HELIUM)
        jpeg_info -> gx_jpeg_pixel_write_helium = _gx_image_reader_jpeg_1555xrgb_pixel_write_helium;
#else
        jpeg_info -> gx_jpeg_pixel_write = _gx_image_reader_jpeg_1555xrgb_pixel_write;
#endif
        jpeg_info -> gx_jpeg_output_bpp = 2;
        jpeg_info -> gx_jpeg_output_stride <<= 1;
        break;

    case GX_COLOR_FORMAT_32ARGB:
    case GX_COLOR_FORMAT_24XRGB:
#if defined(GX_ENABLE_ARM_HELIUM)
        jpeg_info -> gx_jpeg_pixel_write_helium = _gx_image_reader_jpeg_24xrgb_pixel_write_helium;
#else
        jpeg_info -> gx_jpeg_pixel_write = _gx_image_reader_jpeg_24xrgb_pixel_write;
#endif
        jpeg_info -> gx_jpeg_output_bpp = 4;
        jpeg_info -> gx_jpeg_output_stride <<= 2;
        break;

    case GX_IMAGE_FORMAT_24BPP:
    default:
#if defined(GX_ENABLE_ARM_HELIUM)
        jpeg_info -> gx_jpeg_pixel_write_helium = _gx_image_reader_jpeg_24bpp_pixel_write_helium;
#else
        jpeg_info -> gx_jpeg_pixel_write = _gx_image_reader_jpeg_24bpp_pixel_write;
#endif
        jpeg_info -> gx_jpeg_output_bpp = 3;
        jpeg_info -> gx_jpeg_output_stride = (jpeg_info -> gx_jpeg_output_width * 3);
        jpeg_info -> gx_jpeg_output_color_format = GX_IMAGE_FORMAT_24BPP;

        if (jpeg_info -> gx_jpeg_output_buffer)
        {
            return GX_NOT_SUPPORTED;
        }
        break;
    }

    if (!jpeg_info -> gx_jpeg_output_buffer)
    {
        jpeg_info -> gx_jpeg_output_buffer = (GX_UBYTE *)_gx_system_memory_allocator((ULONG)(jpeg_info -> gx_jpeg_height * jpeg_info -> gx_jpeg_width * jpeg_info -> gx_jpeg_output_bpp));
        if (!jpeg_info -> gx_jpeg_output_buffer)
        {
            return GX_SYSTEM_MEMORY_ERROR;
        }
    }

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_jpeg_one_mcu_write                 PORTABLE C      */
/*                                                           6.3.0        */
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
/*    h                                     Horizontal sampling factor    */
/*    v                                     Vertical sampling factor      */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Status Code                                                         */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    vldrbq_gather_offset_s8               Gather bytes from memory      */
/*    GX_JPEG_DECODE_YCBCR2RGB_HELIUM       Convert YCbCr to RGB          */
/*    GX_JPEG_DECODE_YCBCR2RGB              Convert YCbCr to RGB          */
/*    [gx_jpeg_pixel_write_helium]          Write pixel to memory         */
/*    [gx_jpeg_pixel_write]                 Write pixel to memory         */
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
/*  10-31-2023     Ting Zhu                 Modified comment(s),          */
/*                                            added Helium support,       */
/*                                            added partial canvas buffer */
/*                                            support,                    */
/*                                            resulting in version 6.3.0  */
/*                                                                        */
/**************************************************************************/
static UINT _gx_image_reader_jpeg_one_mcu_write(GX_JPEG_INFO *jpeg_info, INT xpos, INT ypos, INT h, INT v)
{
GX_UBYTE *put;
INT       x;
INT       y;
INT       coff;
INT       xstart = 0;
INT       xend;
INT       ystart = 0;
INT       yend;

#if defined(GX_ENABLE_ARM_HELIUM)
int8x16_t  vY;
int8x16_t  vCb;
int8x16_t  vCr;
GX_UBYTE   size;
uint8x16_t vred;
uint8x16_t vgreen;
uint8x16_t vblue;
GX_UBYTE   index;
#else
GX_BYTE    Y;
GX_BYTE    Cb;
GX_BYTE    Cr;
INT        red;
INT        green;
INT        blue;
#endif

    yend = (v << 3);
    xend = (h << 3);


    if (xpos < jpeg_info -> gx_jpeg_output_clip.gx_rectangle_left)
    {
        xstart = jpeg_info -> gx_jpeg_output_clip.gx_rectangle_left - xpos;
    }

    if (xpos + xend > jpeg_info -> gx_jpeg_output_clip.gx_rectangle_right + 1)
    {
        xend = jpeg_info -> gx_jpeg_output_clip.gx_rectangle_right + 1 - xpos;
    }

    if (ypos < jpeg_info -> gx_jpeg_output_clip.gx_rectangle_top)
    {
        ystart = jpeg_info -> gx_jpeg_output_clip.gx_rectangle_top - ypos;
    }

    if (ypos + yend > jpeg_info -> gx_jpeg_output_clip.gx_rectangle_bottom + 1)
    {
        yend = jpeg_info -> gx_jpeg_output_clip.gx_rectangle_bottom  + 1 - ypos;
    }

    put = (GX_UBYTE *)jpeg_info -> gx_jpeg_output_buffer;
#if defined(GX_ENABLE_CANVAS_PARTIAL_FRAME_BUFFER)
    put += (ypos + ystart - jpeg_info -> gx_jpeg_output_buffer_offset_y) * jpeg_info -> gx_jpeg_output_stride;
    put += (xpos + xstart - jpeg_info -> gx_jpeg_output_buffer_offset_x) * jpeg_info -> gx_jpeg_output_bpp;
#else
    put += (ypos + ystart) * jpeg_info -> gx_jpeg_output_stride;
    put += (xpos + xstart) * jpeg_info -> gx_jpeg_output_bpp;
#endif

#if defined(GX_ENABLE_ARM_HELIUM)
    index = (h == 1 ? 0 : (h - 1 + (xstart % h)));
#endif

    for (y = ystart; y < yend; y++)
    {
        jpeg_info -> gx_jpeg_putdata = put;

#if defined(GX_ENABLE_ARM_HELIUM)
        for (x = xstart; x < xend; x += size)
        {
            size = xend - x;

            if (size > 16)
            {
                size = 16;
            }

            coff = x / h + ((y / v) << 3);

            vY = vldrbq_s8(jpeg_info -> gx_jpeg_Y_block + x + y * h * 8);
            vCb = vldrbq_gather_offset_s8(jpeg_info -> gx_jpeg_Cb_block + coff, _gx_jpeg_cbcr_offset_table[index]);
            vCr = vldrbq_gather_offset_s8(jpeg_info -> gx_jpeg_Cr_block + coff, _gx_jpeg_cbcr_offset_table[index]);

            /* Convert YCbCr to RGB.  */
            GX_JPEG_DECODE_YCBCR2RGB_HELIUM(vred, vgreen, vblue, vY, vCb, vCr);

            jpeg_info -> gx_jpeg_pixel_write_helium(jpeg_info, vred, vgreen, vblue, size);
        }
#else
        for (x = xstart; x < xend; x++)
        {
            coff = x / h + ((y / v) << 3);

            Y = jpeg_info -> gx_jpeg_Y_block[x + y * h * 8];
            Cb = jpeg_info -> gx_jpeg_Cb_block[coff];
            Cr = jpeg_info -> gx_jpeg_Cr_block[coff];

            GX_JPEG_DECODE_YCBCR2RGB(red, green, blue, Y, Cb, Cr);

            GX_SATURATE_TO_UBYTE(red, red);
            GX_SATURATE_TO_UBYTE(green, green);
            GX_SATURATE_TO_UBYTE(blue, blue);

            jpeg_info -> gx_jpeg_pixel_write(jpeg_info, (GX_UBYTE)red, (GX_UBYTE)green, (GX_UBYTE)blue);
        }
#endif

        put += jpeg_info -> gx_jpeg_output_stride;
    }

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_jpeg_one_mcu_rotated_write         PORTABLE C      */
/*                                                           6.3.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Ting Zhu, Microsoft Corporation                                     */
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
/*    h                                     Horizontal sampling factor    */
/*    v                                     Vertical sampling factor      */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Status Code                                                         */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    vldrbq_gather_offset_s8               Gather bytes from memory      */
/*    GX_JPEG_DECODE_YCBCR2RGB_HELIUM       Convert YCbCr to RGB          */
/*    GX_JPEG_DECODE_YCBCR2RGB              Convert YCbCr to RGB          */
/*    [gx_jpeg_pixel_write_helium]          Write pixel to memory         */
/*    [gx_jpeg_pixel_write]                 Write pixel to memory         */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_image_reader_jpeg_decode                                        */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  10-31-2023     Ting Zhu                 Initial Version 6.3.0         */
/*                                                                        */
/**************************************************************************/
static UINT _gx_image_reader_jpeg_one_mcu_rotated_write(GX_JPEG_INFO *jpeg_info, INT xpos, INT ypos, INT h, INT v)
{
GX_UBYTE *put;
INT       x;
INT       coff;
INT       xstart = 0;
INT       xend;
INT       ystart = 0;
INT       yend;
INT       stride;

#if defined(GX_ENABLE_ARM_HELIUM)
int8x16_t  vY;
int8x16_t  vCb;
int8x16_t  vCr;
uint8x16_t vred;
uint8x16_t vgreen;
uint8x16_t vblue;
INT        size;
GX_UBYTE   index;
uint8x16_t yoffset;
uint8x16_t cbcroffset;
#else
GX_BYTE    Y;
GX_BYTE    Cb;
GX_BYTE    Cr;
INT        red;
INT        green;
INT        blue;
INT        y;
GX_BYTE    sign = 1;
#endif

    xend = (h << 3) - 1;
    yend = (v << 3) - 1;

    if (xpos < jpeg_info -> gx_jpeg_output_clip.gx_rectangle_left)
    {
        xstart = jpeg_info -> gx_jpeg_output_clip.gx_rectangle_left - xpos;
    }

    if (xpos + xend > jpeg_info -> gx_jpeg_output_clip.gx_rectangle_right)
    {
        xend = jpeg_info -> gx_jpeg_output_clip.gx_rectangle_right - xpos;
    }

    if (xstart > xend)
    {
        return GX_SUCCESS;
    }

    if (ypos < jpeg_info -> gx_jpeg_output_clip.gx_rectangle_top)
    {
        ystart = jpeg_info -> gx_jpeg_output_clip.gx_rectangle_top - ypos;
    }

    if (ypos + yend > jpeg_info -> gx_jpeg_output_clip.gx_rectangle_bottom)
    {
        yend = jpeg_info -> gx_jpeg_output_clip.gx_rectangle_bottom - ypos;
    }

    if (ystart > yend)
    {
        return GX_SUCCESS;
    }

#if defined(GX_ENABLE_ARM_HELIUM)
    size = yend - ystart + 1;
#endif

    stride = jpeg_info -> gx_jpeg_output_stride;
    put = (GX_UBYTE *)jpeg_info -> gx_jpeg_output_buffer;

    if (jpeg_info -> gx_jpeg_output_rotation_angle == GX_SCREEN_ROTATION_CW)
    {
        put += (jpeg_info -> gx_jpeg_output_width - xpos - 1 - xstart) * stride;
        put += (ypos + ystart) * jpeg_info -> gx_jpeg_output_bpp;
        stride = (-stride);
    }
    else
    {
        put += (xpos + xstart) * stride;
        put += (jpeg_info -> gx_jpeg_output_height - ypos - 1 - yend) * jpeg_info -> gx_jpeg_output_bpp;

#if defined(GX_ENABLE_ARM_HELIUM)
        ystart = (v << 3) - 1 - yend;
#else
        GX_SWAP_VALS(ystart, yend);
        sign = -1;
#endif
    }

#if defined(GX_ENABLE_ARM_HELIUM)
    if (jpeg_info -> gx_jpeg_output_rotation_angle == GX_SCREEN_ROTATION_CW)
    {
        index = (v == 2 ? (1 + (ystart % 2)) : 0);

        yoffset = _gx_jpeg_y_offset_rotated_table_cw[h >> 1];
        cbcroffset = _gx_jpeg_cbcr_offset_rotated_table_cw[index];
    }
    else
    {
        yoffset = _gx_jpeg_y_offset_rotated_table_ccw[h + v - 2];
        cbcroffset = _gx_jpeg_cbcr_offset_rotated_table_ccw[v - 1];
        
        if(ystart)
        {
            for (x = 0; x < 16 - ystart; x++)
            {
                yoffset[x] = yoffset[ystart + x];
                cbcroffset[x] = cbcroffset[ystart + x];
            }

            ystart = 0;
        }
    }
#endif

    for (x = xstart; x <= xend; x++)
    {
        jpeg_info -> gx_jpeg_putdata = put;

#if defined(GX_ENABLE_ARM_HELIUM)
        coff = x / h + ((ystart / v) << 3);

        vY = vldrbq_gather_offset_s8(jpeg_info -> gx_jpeg_Y_block + x + ystart * h * 8, yoffset);
        vCb = vldrbq_gather_offset_s8(jpeg_info -> gx_jpeg_Cb_block + coff, cbcroffset);
        vCr = vldrbq_gather_offset_s8(jpeg_info -> gx_jpeg_Cr_block + coff, cbcroffset);

        /* Convert YCbCr to RGB.  */
        GX_JPEG_DECODE_YCBCR2RGB_HELIUM(vred, vgreen, vblue, vY, vCb, vCr);

        jpeg_info -> gx_jpeg_pixel_write_helium(jpeg_info, vred, vgreen, vblue, size);

#else
        for (y = ystart; y != yend + sign; y += sign)
        {
            coff = x / h + ((y / v) << 3);

            Y = jpeg_info -> gx_jpeg_Y_block[x + y * h * 8];
            Cb = jpeg_info -> gx_jpeg_Cb_block[coff];
            Cr = jpeg_info -> gx_jpeg_Cr_block[coff];

            GX_JPEG_DECODE_YCBCR2RGB(red, green, blue, Y, Cb, Cr);

            GX_SATURATE_TO_UBYTE(red, red);
            GX_SATURATE_TO_UBYTE(green, green);
            GX_SATURATE_TO_UBYTE(blue, blue);

            jpeg_info -> gx_jpeg_pixel_write(jpeg_info, (GX_UBYTE)red, (GX_UBYTE)green, (GX_UBYTE)blue);
        }
#endif
        put += stride;
    }

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_jpeg_decompress                    PORTABLE C      */
/*                                                           6.3.0        */
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
/*  10-31-2022     Kenneth Maxwell          Modified comment(s),          */
/*                                            abort if block decode fails,*/
/*                                            resulting in version 6.2.0  */
/*  10-31-2023     Ting Zhu                 Modified comment(s),          */
/*                                            improved logic,             */
/*                                            resulting in version 6.3.0  */
/*                                                                        */
/**************************************************************************/
static UINT _gx_image_reader_jpeg_decompress(GX_JPEG_INFO *jpeg_info)
{
int  h;
int  v;
int  x;
int  y;
int  xx;
int  yy;
UINT status = GX_SUCCESS;
UINT (*one_mcu_write)(GX_JPEG_INFO *jpeg_info, INT xpos, INT ypos, INT h, INT v);

    h = (jpeg_info -> gx_jpeg_sample_factor[0] >> 4);
    v = (jpeg_info -> gx_jpeg_sample_factor[0] & 0x0f);

    if (v > 2)
    {
        return GX_INVALID_FORMAT;
    }

    status = _gx_image_reader_jpeg_pixel_write_info_set(jpeg_info);

    if ((jpeg_info -> gx_jpeg_output_rotation_angle != 0) &&
        (jpeg_info -> gx_jpeg_output_color_format != GX_IMAGE_FORMAT_24BPP))
    {
        one_mcu_write = _gx_image_reader_jpeg_one_mcu_rotated_write;
    }
    else
    {
        one_mcu_write = _gx_image_reader_jpeg_one_mcu_write;
    }

    for (y = 0; y < jpeg_info -> gx_jpeg_height; y += 8 * v)
    {
        for (x = 0; x < jpeg_info -> gx_jpeg_width; x += 8 * h)
        {
            /* Decode one MCU */
            for (yy = 0; yy < v && status == GX_SUCCESS; yy++)
            {
                for (xx = 0; xx < h && status == GX_SUCCESS; xx++)
                {
                    /* Y */
                    status = _gx_image_reader_jpeg_one_block_decode(jpeg_info, 0, jpeg_info -> gx_jpeg_Y_block + yy * 128 + xx * 8);
                }
            }

            if (status == GX_SUCCESS && jpeg_info -> gx_jpeg_num_of_components > 1)
            {
                /* Cb */
                status = _gx_image_reader_jpeg_one_block_decode(jpeg_info, 1, jpeg_info -> gx_jpeg_Cb_block);

                /* Cr */
                if (status == GX_SUCCESS)
                {
                    status = _gx_image_reader_jpeg_one_block_decode(jpeg_info, 2, jpeg_info -> gx_jpeg_Cr_block);
                }
            }

            if (status == GX_SUCCESS)
            {
                one_mcu_write(jpeg_info, jpeg_info -> gx_jpeg_output_xpos + x, jpeg_info -> gx_jpeg_output_ypos + y, h, v);
            }
        }
    }

    return status;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_jpeg_decode_blocks                 PORTABLE C      */
/*                                                           6.3.0        */
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
/*  10-31-2023     Ting Zhu                 Modified comment(s), removed  */
/*                                            huffman table free logic,   */
/*                                            resulting in version 6.3.0  */
/*                                                                        */
/**************************************************************************/
static UINT _gx_image_reader_jpeg_decode_blocks(GX_JPEG_INFO *jpeg_info)
{
GX_UBYTE *jpeg_data;
GX_UBYTE  marker;
UINT      segment_len;
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

    return status;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_jpeg_decode                        PORTABLE C      */
/*                                                           6.3.0        */
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
/*  10-31-2023     Ting Zhu                 Modified comment(s),          */
/*                                            improved logic,             */
/*                                            resulting in version 6.3.0  */
/*                                                                        */
/**************************************************************************/
UINT _gx_image_reader_jpeg_decode(GX_IMAGE_READER *image_reader, GX_PIXELMAP *outmap)
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


    jpeg_info -> gx_jpeg_data = (GX_UBYTE *)image_reader -> gx_image_reader_source_data;
    jpeg_info -> gx_jpeg_data_size = (INT)image_reader -> gx_image_reader_source_data_size;
    jpeg_info -> gx_jpeg_data_index = 0;

    if (image_reader -> gx_image_reader_mode & GX_IMAGE_READER_MODE_DITHER)
    {
        jpeg_info -> gx_jpeg_output_color_format  = GX_IMAGE_FORMAT_24BPP;
    }
    else
    {
        jpeg_info -> gx_jpeg_output_color_format  = image_reader -> gx_image_reader_color_format;
    }

    if (image_reader -> gx_image_reader_mode & GX_IMAGE_READER_MODE_ROTATE_CW)
    {
        jpeg_info -> gx_jpeg_output_rotation_angle =  GX_SCREEN_ROTATION_CW;
    }
    else if (image_reader -> gx_image_reader_mode & GX_IMAGE_READER_MODE_ROTATE_CCW)
    {
        jpeg_info -> gx_jpeg_output_rotation_angle =  GX_SCREEN_ROTATION_CCW;
    }

    status = _gx_image_reader_jpeg_decode_blocks(jpeg_info);

    if (status == GX_SUCCESS)
    {
        outmap -> gx_pixelmap_data = jpeg_info -> gx_jpeg_output_buffer;
        outmap -> gx_pixelmap_data_size = (ULONG)(jpeg_info -> gx_jpeg_output_stride * jpeg_info -> gx_jpeg_height);
        outmap -> gx_pixelmap_width = (GX_VALUE)jpeg_info -> gx_jpeg_width;
        outmap -> gx_pixelmap_height = (GX_VALUE)jpeg_info -> gx_jpeg_height;
        outmap -> gx_pixelmap_flags = 0;
        outmap -> gx_pixelmap_format = jpeg_info -> gx_jpeg_output_color_format;
    }

    _gx_system_memory_free((void *)jpeg_info);

    return status;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_image_reader_jpeg_mcu_decode                    PORTABLE C      */
/*                                                           6.3.0        */
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
/*  10-31-2023     Ting Zhu                 Modified comment(s),          */
/*                                            improved logic,             */
/*                                            resulting in version 6.3.0  */
/*                                                                        */
/**************************************************************************/
UINT _gx_image_reader_jpeg_mcu_decode(GX_CONST GX_UBYTE *read_data, ULONG data_size,
                                      GX_DRAW_CONTEXT *context, INT xpos, INT ypos)
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

    if (!context)
    {
        return GX_INVALID_CONTEXT;
    }

    memset(jpeg_info, 0, sizeof(GX_JPEG_INFO));

    jpeg_info -> gx_jpeg_data = (GX_UBYTE *)read_data;
    jpeg_info -> gx_jpeg_data_size = (INT)data_size;
    jpeg_info -> gx_jpeg_data_index = 0;
    jpeg_info -> gx_jpeg_output_xpos = xpos;
    jpeg_info -> gx_jpeg_output_ypos = ypos;
    jpeg_info -> gx_jpeg_output_color_format = context -> gx_draw_context_display -> gx_display_color_format;
    jpeg_info -> gx_jpeg_output_rotation_angle = context -> gx_draw_context_display -> gx_display_rotation_angle;
    jpeg_info -> gx_jpeg_output_buffer = (GX_UBYTE *)context -> gx_draw_context_memory;
#ifdef GX_ENABLE_CANVAS_PARTIAL_FRAME_BUFFER
    jpeg_info -> gx_jpeg_output_width = (USHORT)context -> gx_draw_context_canvas -> gx_canvas_memory_width;
    jpeg_info -> gx_jpeg_output_height = (USHORT)context -> gx_draw_context_canvas -> gx_canvas_memory_height;
    jpeg_info -> gx_jpeg_output_buffer_offset_x = context -> gx_draw_context_offset_x;
    jpeg_info -> gx_jpeg_output_buffer_offset_y = context -> gx_draw_context_offset_y;
#else
    jpeg_info -> gx_jpeg_output_width = (USHORT)context -> gx_draw_context_canvas -> gx_canvas_x_resolution;
    jpeg_info -> gx_jpeg_output_height = (USHORT)context -> gx_draw_context_canvas -> gx_canvas_y_resolution;
#endif
    jpeg_info -> gx_jpeg_output_clip = *context -> gx_draw_context_clip;


    status = _gx_image_reader_jpeg_decode_blocks(jpeg_info);

    _gx_system_memory_free((void *)jpeg_info);

    return status;
}
#endif

