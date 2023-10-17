#include "gx_api.h"
static GX_CONST CHAR pic1_1bpp_nocompressed_data[] =
{
    0xAA, 0xAA, 0xAA,
    0x55, 0x55, 0x55,
    0xAA, 0xAA, 0xAA,
    0x55, 0x55, 0x55,
    0xAA, 0xAA, 0xAA,
    0x55, 0x55, 0x55,
    0xAA, 0xAA, 0xAA,
    0x55, 0x55, 0x55,
    0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF,
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
};
GX_CONST GX_PIXELMAP pic1_1bpp_nocompressed_pixelmap =
{
    0x00000001,                              /* major version                  */
    0x00000000,                              /* minor version                  */
    0,                                       /* flags                          */
    GX_COLOR_FORMAT_MONOCHROME,              /* Format                         */
    (GX_UBYTE *)pic1_1bpp_nocompressed_data,
    sizeof(pic1_1bpp_nocompressed_data),    /* the size of pixelmap_data*/
    NULL,
    0,                                       /* auxiliary data size            */
    0x0000,                                  /* used for transparent GIF iamges*/
    24,                                     /* width in pixel                 */
    16                                      /* height in pixel                */
};