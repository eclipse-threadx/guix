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
/**   Image Reader Management (Image Reader)                              */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/


/**************************************************************************/
/*                                                                        */
/*  COMPONENT DEFINITION                                   RELEASE        */
/*                                                                        */
/*    gx_image_reader.h                                   PORTABLE C      */
/*                                                           6.3.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This file defines the GUIX image reader management component,       */
/*    including all data types and external references.  It is assumed    */
/*    that gx_api.h and gx_port.h have already been included.             */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*  06-02-2021     Ting Zhu                 Modified comment(s), and      */
/*                                            improved png decoding       */
/*                                            performance,                */
/*                                            resulting in version 6.1.7  */
/*  10-31-2022     Kenneth Maxwell          Modified comment(s),          */
/*                                            added definitions for fixed */
/*                                            size table dimensions,      */
/*                                            resulting in version 6.2.0  */
/*  10-31-2023     Ting Zhu                 Modified comment(s), added    */
/*                                            support for ARM Helium,     */
/*                                            added partial canvas buffer */
/*                                            support,                    */
/*                                            resulting in version 6.3.0  */
/*                                                                        */
/**************************************************************************/
#if defined(GX_SOFTWARE_DECODER_SUPPORT)
#ifndef GX_IMAGE_READER_H
#define GX_IMAGE_READER_H

#if defined(GX_ENABLE_ARM_HELIUM)
#include <arm_mve.h>
#endif

#define GX_IMAGE_FORMAT_1BPP                    0xf0
#define GX_IMAGE_FORMAT_2BPP                    0xf1
#define GX_IMAGE_FORMAT_4BPP                    0xf2
#define GX_IMAGE_FORMAT_8BPP                    0xf3
#define GX_IMAGE_FORMAT_16BPP_GRAY              0xf4 /* Internal format: 16bits for each gray value. */
#define GX_IMAGE_FORMAT_16BPP_GRAY_ALPHA        0xf5 /* Internal format: gray: alpha stream, 8 bits. */
#define GX_IMAGE_FORMAT_32BPP_GRAY_ALPHA        0xf6 /* Internal format: gray: alpha steam, 16bits. */
#define GX_IMAGE_FORMAT_24BPP                   0xf7 /* Internal format: r:g:b stream, 8bits for each channel. */
#define GX_IMAGE_FORMAT_32BPP                   0xf8 /* Internal format: r:g:b:a strem, 8bits for each channel. */
#define GX_IMAGE_FORMAT_48BPP                   0xf9 /* Internal format: r:g:b stream, 16bits for each channel. */
#define GX_IMAGE_FORMAT_64BPP                   0xfa /* Internal format: r:g:b:a stream, 16bits for each channel */

#define GX_TRANSPARENT_COLOR                    0xff

#define GX_PNG_CRC_TABLE_SIZE                   256
#define GX_PNG_HUFFMAN_LIT_TABLE_SIZE           286
#define GX_PNG_HUFFMAN_LIT_CODE_LEN_TABLE_SIZE  288
#define GX_PNG_HUFFMAN_DIST_CODE_LEN_TABLE_SIZE 32
#define GX_PNG_HUFFMAN_DIST_TABLE_SIZE          30
#define GX_PNG_PALETTE_TABLE_SIZE               256
#define GX_PNG_SCRATCH_BUFFER_SIZE              (GX_PNG_CRC_TABLE_SIZE +                   \
                                                 GX_PNG_HUFFMAN_LIT_TABLE_SIZE +           \
                                                 GX_PNG_HUFFMAN_LIT_CODE_LEN_TABLE_SIZE +  \
                                                 GX_PNG_HUFFMAN_DIST_CODE_LEN_TABLE_SIZE + \
                                                 GX_PNG_PALETTE_TABLE_SIZE)
#define JPG_MAX_COMPONENTS                      3
#define HUFF_TABLE_DIMENSION                    2
#define JPG_QUANT_TABLE_DIMENSION               4

/* Control block used internally for jpeg reader.  */
typedef struct GX_HUFFCODE_INFO_STRUCT
{
    USHORT   start;
    USHORT   end;
    USHORT   index;
    GX_UBYTE bits;
} GX_HUFFCODE_INFO;

typedef struct GX_JPEG_INFO_STRUCT
{
    USHORT           gx_jpeg_width;
    USHORT           gx_jpeg_height;
    INT              gx_jpeg_num_of_components;
    GX_UBYTE         gx_jpeg_sample_factor[JPG_MAX_COMPONENTS];
    GX_UBYTE         gx_jpeg_component_id[JPG_MAX_COMPONENTS];
    GX_UBYTE         gx_jpeg_qantization_table_index[JPG_MAX_COMPONENTS];
    GX_UBYTE         gx_jpeg_dc_table_index[JPG_MAX_COMPONENTS];
    GX_UBYTE         gx_jpeg_ac_table_index[JPG_MAX_COMPONENTS];
    INT              gx_jpeg_quantization_table[JPG_QUANT_TABLE_DIMENSION][64];
    GX_UBYTE        *gx_jpeg_huffman_table[HUFF_TABLE_DIMENSION][HUFF_TABLE_DIMENSION];
    GX_HUFFCODE_INFO gx_jpeg_huffman_code_info[HUFF_TABLE_DIMENSION][HUFF_TABLE_DIMENSION][16];
    INT              gx_jpeg_restart_interval;
    GX_BYTE          gx_jpeg_Y_block[256];
    GX_BYTE          gx_jpeg_Cr_block[64];
    GX_BYTE          gx_jpeg_Cb_block[64];
    INT              gx_jpeg_pre_dc[JPG_MAX_COMPONENTS];
    INT              gx_jpeg_vecter[64];
    GX_UBYTE        *gx_jpeg_data;
    INT              gx_jpeg_data_size;
    INT              gx_jpeg_data_index;
    UINT             gx_jpeg_bit_buffer;
    UINT             gx_jpeg_bit_count;
#if defined(GX_ENABLE_ARM_HELIUM)
    VOID             (*gx_jpeg_pixel_write_helium)(struct GX_JPEG_INFO_STRUCT *jpeg_info, uint8x16_t vred, uint8x16_t vgreen, uint8x16_t vblue, INT size);
#else
    VOID             (*gx_jpeg_pixel_write)(struct GX_JPEG_INFO_STRUCT *jpeg_info, GX_UBYTE red, GX_UBYTE green, GX_UBYTE blue);
#endif
    GX_UBYTE        *gx_jpeg_output_buffer;
#if defined(GX_ENABLE_CANVAS_PARTIAL_FRAME_BUFFER)
    GX_VALUE         gx_jpeg_output_buffer_offset_x;
    GX_VALUE         gx_jpeg_output_buffer_offset_y;
#endif
    USHORT           gx_jpeg_output_width;
    USHORT           gx_jpeg_output_height;
    INT              gx_jpeg_output_stride;
    GX_RECTANGLE     gx_jpeg_output_clip;
    INT              gx_jpeg_output_xpos;
    INT              gx_jpeg_output_ypos;
    GX_UBYTE         gx_jpeg_output_bpp;
    GX_UBYTE         gx_jpeg_output_color_format;
    USHORT           gx_jpeg_output_rotation_angle;
    GX_UBYTE        *gx_jpeg_putdata;
} GX_JPEG_INFO;

/* control block used internally for png reader */
typedef struct GX_PNG_STRUCT
{
    GX_UBYTE *gx_png_data;
    INT       gx_png_data_size;
    INT       gx_png_data_index;
    GX_UBYTE *gx_png_decoded_data;
    INT       gx_png_decoded_data_len;
    INT       gx_png_width;
    INT       gx_png_height;
    GX_UBYTE  gx_png_bit_depth;
    INT       gx_png_color_type;
    GX_UBYTE  gx_png_bpp; /* bits per pixel */
    INT       gx_png_compression_method;
    INT       gx_png_filter_method;
    INT       gx_png_interlace_method;
    UINT     *gx_png_crc_table;   /* 256 */
    INT       gx_png_gamma;
    INT       gx_png_huffman_clen_table[20];
    INT       gx_png_huffman_clen_bits_count[17];
    INT      *gx_png_huffman_lit_table;    /* 286 */
    INT      *gx_png_huffman_lit_code_len; /* 286 */
    INT       gx_png_huffman_lit_bits_count[17];
    INT       gx_png_huffman_dist_table[GX_PNG_HUFFMAN_DIST_TABLE_SIZE];
    INT      *gx_png_huffman_dist_code_len; /* 32 */
    INT       gx_png_huffman_dist_bits_count[17];
    GX_COLOR *gx_png_palette_table;         /* 256 */
    INT       gx_png_palette_table_size;
    GX_COLOR *gx_png_trans;
    INT       gx_png_trans_num;
    INT       gx_png_trunk_end_index;
    UINT      gx_png_trunk_crc;
} GX_PNG;

/* Define API functions for image reader.  */
UINT _gx_image_reader_start(GX_IMAGE_READER *image_reader, GX_PIXELMAP *outmap);
UINT _gx_image_reader_colorspace_convert(GX_IMAGE_READER *image_reader, GX_PIXELMAP *outmap);

UINT _gx_image_reader_create(GX_IMAGE_READER *image_reader,
                             GX_CONST GX_UBYTE *read_data,
                             INT read_data_size,
                             GX_UBYTE color_format,
                             GX_UBYTE mode);

/* Define internal function prototypes.  */
UINT _gx_image_reader_png_decode(GX_CONST GX_UBYTE *read_data, ULONG read_data_size, GX_PIXELMAP *outmap);
UINT _gx_image_reader_jpeg_decode(GX_IMAGE_READER *image_reader, GX_PIXELMAP *outmap);
UINT _gx_image_reader_jpeg_mcu_decode(GX_CONST GX_UBYTE *read_data, ULONG data_size,
                                      GX_DRAW_CONTEXT *context, INT xpos, INT ypos);

UINT _gx_image_reader_pixel_read_callback_set(GX_IMAGE_READER *image_reader, GX_PIXELMAP *outmap);
UINT _gx_image_reader_pixel_write_callback_set(GX_IMAGE_READER *image_reader, GX_PIXELMAP *outmap);
UINT _gx_image_reader_image_decode(GX_IMAGE_READER *image_reader, GX_PIXELMAP *outmap);
UINT _gx_image_reader_palette_set(GX_IMAGE_READER *image_reader, GX_COLOR *pal, UINT palsize);
UINT _gx_image_reader_nearest_palette_color_get(GX_IMAGE_READER *image_reader, GX_PIXEL *want_color, INT *index);
VOID _gx_image_reader_rgb2gray(GX_PIXEL *pixel, GX_UBYTE *gray);
UINT _gx_image_reader_rle_encode(GX_IMAGE_READER *image_reader, GX_PIXELMAP *pRLEmap);
UINT _gx_image_reader_rle_encode_size_get(GX_IMAGE_READER *image_reader, UINT *data_size, UINT *aux_size);
VOID _gx_image_reader_rgb2gray(GX_PIXEL *pixel, GX_UBYTE *gray);

/* Define error checking shells for API services.  These are only referenced by the
   application.  */

UINT _gxe_image_reader_create(GX_IMAGE_READER *image_reader,
                              GX_CONST GX_UBYTE  *read_data,
                              INT read_data_size,
                              GX_UBYTE color_format,
                              GX_UBYTE mode);

UINT _gxe_image_reader_palette_set(GX_IMAGE_READER *image_reader, GX_COLOR *pal, UINT palsize);
UINT _gxe_image_reader_start(GX_IMAGE_READER *image_reader, GX_PIXELMAP *outmap);


#define GX_JPEG_DECODE_YCBCR2RGB(red, green, blue, y, cb, cr)                                                \
    red = y + cr + (cr >> 2) + (cr >> 3) + 128;                                                              \
    green = y - ((cb >> 2) + (cb >> 4) + (cb >> 5)) - ((cr >> 1) + (cr >> 3) + (cr >> 4) + (cr >> 5)) + 128; \
    blue = y + cb + (cb >> 1) + (cb >> 2) + (cb >> 6) + 128;

#if defined(GX_ENABLE_ARM_HELIUM)

/* This offset table contains four offset vectors that used to gather load 16 cb/cr values to a 8x16 vector.
   The table index represents the subsampling factor in horizontal. */
static uint8x16_t _gx_jpeg_cbcr_offset_table[7] = {
    {0, 1, 2, 3, 4, 5, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0}, /* h = 1 */
    {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7}, /* h = 2 */
    {0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 7}, /* h = 2 */
    {0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3}, /* h = 4 */
    {0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4}, /* h = 4 */
    {0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4}, /* h = 4 */
    {0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4}, /* h = 4 */
};

static uint8x16_t _gx_jpeg_cbcr_offset_rotated_table_cw[3] = {
    {0, 8, 16, 24, 32, 40, 48, 56,  0,  0,  0,  0,  0,  0,  0,  0}, /* cw, v = 1 */
    {0, 0,  8,  8, 16, 16, 24, 24, 32, 32, 40, 40, 48, 48, 56, 56}, /* cw, v = 2 */
    {0, 8,  8, 16, 16, 24, 24, 32, 32, 40, 40, 48, 48, 56, 56, 56} /* cw, v = 2 */
};

static uint8x16_t _gx_jpeg_cbcr_offset_rotated_table_ccw[2] = {
    {56, 48, 40, 32, 24, 16,  8,  0,  0,  0,  0,  0, 0, 0, 0, 0}, /* ccw, v = 1 */
    {56, 56, 48, 48, 40, 40, 32, 32, 24, 24, 16, 16, 8, 8, 0, 0}, /* ccw, v = 2 */
};

static uint8x16_t _gx_jpeg_y_offset_rotated_table_cw[3] = {
    {0,  8, 16, 24,  32,  40,  48,  56,  64,  72,  80,  88,  96, 104, 112, 120}, /* cw, h = 1, v = 1 */
    {0, 16, 32, 48,  64,  80,  96, 112, 128, 144, 160, 176, 192, 208, 224, 240}, /* cw, h = 2, v = 1 or v = 2 */
    {0, 32, 64, 96, 128, 160, 192, 224, 224, 224, 224, 224, 224, 224, 224, 224} /* cw, h = 4, v = 1 */
};

static uint8x16_t _gx_jpeg_y_offset_rotated_table_ccw[4] = {
    {56,   48,  40,  32,  24,  16,   8,   0,   0,   0,   0,   0,   0,   0,   0, 0}, /* ccw, h = 1, v = 1 */
    {112,  96,  80,  64,  48,  32,  16,   0,   0,   0,   0,   0,   0,   0,   0, 0}, /* ccw, h = 2, v = 1 */
    {240, 224, 208, 192, 176, 160, 144, 128, 112,  96,  80,  64,  48,  32,  16, 0}, /* ccw, h = 2, v = 2 */
    {224, 192, 160, 128,  96,  64,  32,   0,   0,   0,   0,   0,   0,   0,   0, 0} /* ccw, h = 4, v = 1 */
};


#define GX_JPEG_DECODE_YCBCR2RGB_HELIUM(vred, vgreen, vblue, y, cb, cr) \
    {                                                                   \
        int8x16_t vt;                                                   \
        vred = (uint8x16_t)vqaddq_s8(y, cr);                            \
        vt = vshrq_n_s8(cr, 2);                                         \
        vred = (uint8x16_t)vqaddq_s8((int8x16_t)vred, vt);              \
        vt = vshrq_n_s8(cr, 3);                                         \
        vred = (uint8x16_t)vqaddq_s8((int8x16_t)vred, vt);              \
        vred = vaddq_n_u8(vred, 128);                                   \
                                                                        \
        vt = vshrq_n_s8(cb, 2);                                         \
        vgreen = (uint8x16_t)vshrq_n_s8(cb, 4);                         \
        vgreen = (uint8x16_t)vqaddq_s8((int8x16_t)vgreen, vt);          \
        vt = vshrq_n_s8(cb, 5);                                         \
        vgreen = (uint8x16_t)vqaddq_s8((int8x16_t)vgreen, vt);          \
        vt = vshrq_n_s8(cr, 1);                                         \
        vgreen = (uint8x16_t)vqaddq_s8((int8x16_t)vgreen, vt);          \
        vt = vshrq_n_s8(cr, 3);                                         \
        vgreen = (uint8x16_t)vqaddq_s8((int8x16_t)vgreen, vt);          \
        vt = vshrq_n_s8(cr, 4);                                         \
        vgreen = (uint8x16_t)vqaddq_s8((int8x16_t)vgreen, vt);          \
        vt = vshrq_n_s8(cr, 5);                                         \
        vgreen = (uint8x16_t)vqaddq_s8((int8x16_t)vgreen, vt);          \
        vgreen = (uint8x16_t)vqsubq_s8(y, (int8x16_t)vgreen);           \
        vgreen = vaddq_n_u8(vgreen, 128);                               \
                                                                        \
        vblue = (uint8x16_t)vqaddq_s8(y, cb);                           \
        vt = vshrq_n_s8(cb, 1);                                         \
        vblue = (uint8x16_t)vqaddq_s8((int8x16_t)vblue, vt);            \
        vt = vshrq_n_s8(cb, 2);                                         \
        vblue = (uint8x16_t)vqaddq_s8((int8x16_t)vblue, vt);            \
        vt = vshrq_n_s8(cb, 6);                                         \
        vblue = (uint8x16_t)vqaddq_s8((int8x16_t)vblue, vt);            \
        vblue = vaddq_n_u8(vblue, 128);                                 \
    }


#endif /* GX_ENABLE_ARM_HELIUM */

#endif /* GX_IMAGE_READER_H */

#endif /* GX_SOFTWARE_DECODER_SUPPORT */

