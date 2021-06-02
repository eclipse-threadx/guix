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
/*                                                           6.1.7        */
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
/*                                                                        */
/**************************************************************************/
#if defined(GX_SOFTWARE_DECODER_SUPPORT)
#ifndef GX_IMAGE_READER_H
#define GX_IMAGE_READER_H

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
#define GX_PNG_PALETTE_TABLE_SIZE               256
#define GX_PNG_SCRATCH_BUFFER_SIZE              (GX_PNG_CRC_TABLE_SIZE +                   \
                                                 GX_PNG_HUFFMAN_LIT_TABLE_SIZE +           \
                                                 GX_PNG_HUFFMAN_LIT_CODE_LEN_TABLE_SIZE +  \
                                                 GX_PNG_HUFFMAN_DIST_CODE_LEN_TABLE_SIZE + \
                                                 GX_PNG_PALETTE_TABLE_SIZE)

/* Control block used internally for jpeg reader.  */

typedef struct GX_JPEG_INFO_STRUCT
{
    USHORT           gx_jpeg_width;
    USHORT           gx_jpeg_height;
    INT              gx_jpeg_num_of_components;
    GX_UBYTE         gx_jpeg_sample_factor[3];
    GX_UBYTE         gx_jpeg_component_id[3];
    GX_UBYTE         gx_jpeg_qantization_table_index[3];
    GX_UBYTE         gx_jpeg_dc_table_index[3];
    GX_UBYTE         gx_jpeg_ac_table_index[3];
    INT              gx_jpeg_quantization_table[4][64];
    INT             *gx_jpeg_huffman_table[2][2];
    GX_VALUE         gx_jpeg_huffman_bits_count[2][2][16];
    INT              gx_jpeg_restart_interval;
    GX_UBYTE         gx_jpeg_Y_block[256];
    GX_UBYTE         gx_jpeg_Cr_block[64];
    GX_UBYTE         gx_jpeg_Cb_block[64];
    INT              gx_jpeg_pre_dc[3];
    INT              gx_jpeg_vecter[64];
    GX_UBYTE        *gx_jpeg_data;
    INT              gx_jpeg_data_size;
    INT              gx_jpeg_data_index;
    GX_UBYTE        *gx_jpeg_decoded_data;
    UINT             gx_jpeg_decoded_data_size;
    GX_DRAW_CONTEXT *gx_jpeg_draw_context;
    UINT                 (*gx_jpeg_mcu_draw)(struct GX_JPEG_INFO_STRUCT *, INT, INT);
    INT              gx_jpeg_draw_xpos;
    INT              gx_jpeg_draw_ypos;
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
    INT       gx_png_huffman_dist_table[30];
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
UINT _gx_image_reader_jpeg_decode(GX_CONST GX_UBYTE *read_data, ULONG data_size, GX_PIXELMAP *outmap);
UINT _gx_image_reader_jpeg_mcu_decode(GX_CONST GX_UBYTE * read_data, ULONG data_size,
                                      GX_DRAW_CONTEXT * context, INT xpos, INT ypos,
                                      UINT(draw_function)(GX_JPEG_INFO *, INT, INT));


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


#endif
#endif

