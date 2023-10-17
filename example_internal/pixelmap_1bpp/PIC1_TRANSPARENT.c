#include "gx_api.h"
static GX_CONST CHAR pic1_1bpp_transparent_data[] =
{
    0xAE, 0x0B, 0x44, 0x44, 0x44,
    0xAE, 0x0B, 0x11, 0x11, 0x11,
    0xAE, 0x0B, 0x44, 0x44, 0x44,
    0xAE, 0x0B, 0x11, 0x11, 0x11,
    0xAE, 0x0B, 0x44, 0x44, 0x44,
    0xAE, 0x0B, 0x11, 0x11, 0x11,
    0xAE, 0x0B, 0x44, 0x44, 0x44,
    0xAE, 0x0B, 0x11, 0x11, 0x11,
    0xAE, 0xAD, 0xAE, 0xAD, 0xAE,
    0xAD, 0xAE, 0xAD, 0xAE, 0xAC,
    0xAE, 0xAC, 0xAE, 0xAC, 0xAE,
    0xAC,
};
GX_CONST GX_PIXELMAP pic1_1bpp_transparent_pixelmap =
{
    0x00000001,                              /* major version                  */
    0x00000000,                              /* minor version                  */
    GX_PIXELMAP_COMPRESSED|GX_PIXELMAP_TRANSPARENT, /* flags                          */
    GX_COLOR_FORMAT_MONOCHROME,              /* Format                         */
    (GX_UBYTE *)pic1_1bpp_transparent_data,
    sizeof(pic1_1bpp_transparent_data),    /* the size of pixelmap_data*/
    NULL,
    0,                                       /* auxiliary data size            */
    0x0000,                                  /* used for transparent GIF iamges*/
    24,                                     /* width in pixel                 */
    16                                      /* height in pixel                */
};