#include "gx_api.h"
static GX_CONST CHAR pic1_1bpp_compressed_data[] =
{
    0x16, 0xAA, 0xAA, 0xAA, 0x82, 0x15, 0xAA, 0xAA, 0xA8, 0x83,
    0x15, 0x55, 0x55, 0x54, 0x82, 0x15, 0xAA, 0xAA, 0xA8, 0x83,
    0x15, 0x55, 0x55, 0x54, 0x82, 0x15, 0xAA, 0xAA, 0xA8, 0x83,
    0x15, 0x55, 0x55, 0x54, 0x82, 0x15, 0xAA, 0xAA, 0xA8, 0xFF,
    0xC1, 0xFE, 0xBE,
};
GX_CONST GX_PIXELMAP pic1_1bpp_compressed_pixelmap =
{
    0x00000001,                              /* major version                  */
    0x00000000,                              /* minor version                  */
    GX_PIXELMAP_COMPRESSED,                  /* flags                          */
    GX_COLOR_FORMAT_MONOCHROME,              /* Format                         */
    (GX_UBYTE *)pic1_1bpp_compressed_data,
    sizeof(pic1_1bpp_compressed_data),    /* the size of pixelmap_data*/
    NULL,
    0,                                       /* auxiliary data size            */
    0x0000,                                  /* used for transparent GIF iamges*/
    24,                                     /* width in pixel                 */
    16                                      /* height in pixel                */
};