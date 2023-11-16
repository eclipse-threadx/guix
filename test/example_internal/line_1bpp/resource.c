/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"

/* CHECKBOX_ON pixelmap data                                                   */

static GX_CONST GX_UBYTE PRIMARY_CHECKBOX_ON_pixelmap_data[32] =
{
    0x00, 0x00, 0x7f, 0xfc, 0x7f, 0xfe, 0x7f, 0xfe, 0x7f, 0xe6, 0x7f, 0xce, 0x77,
    0x8e, 0x73, 0x1e, 0x72, 0x1e, 0x70, 0x3c, 0x78, 0x7c, 0x7c, 0xf8, 0x7f, 0xfc,
    0x7f, 0xec, 0x3f, 0x80, 0x00, 0x00
};
GX_CONST GX_PIXELMAP PRIMARY_CHECKBOX_ON_pixelmap =
{
    0x00000001,                              /* major version                  */
    0x00000000,                              /* minor version                  */
    0,                                       /* flags                          */
    GX_COLOR_FORMAT_MONOCHROME,              /* Format                         */
    (GX_UBYTE *)PRIMARY_CHECKBOX_ON_pixelmap_data,
    sizeof(PRIMARY_CHECKBOX_ON_pixelmap_data),    /* the size of pixelmap_data */
    NULL,
    0,                                       /* auxiliary data size            */
    0x00,                                    /* used for transparent iamges    */
    16,                                      /* width in pixel                 */
    16                                       /* height in pixel                */
};

/* CHECKBOX_OFF pixelmap data                                                  */

static GX_CONST GX_UBYTE PRIMARY_CHECKBOX_OFF_pixelmap_data[32] =
{
    0x00, 0x00, 0x7f, 0xfc, 0x7f, 0xfc, 0x7f, 0xfc, 0x7f, 0xfc, 0x7f, 0xfc, 0x7f,
    0xf8, 0x7f, 0xf8, 0x7f, 0xf8, 0x7f, 0xf8, 0x7f, 0xf8, 0x7f, 0xf8, 0x7f, 0xf8,
    0x7c, 0x00, 0x00, 0x00, 0x00, 0x00
};
GX_CONST GX_PIXELMAP PRIMARY_CHECKBOX_OFF_pixelmap =
{
    0x00000001,                              /* major version                  */
    0x00000000,                              /* minor version                  */
    0,                                       /* flags                          */
    GX_COLOR_FORMAT_MONOCHROME,              /* Format                         */
    (GX_UBYTE *)PRIMARY_CHECKBOX_OFF_pixelmap_data,
    sizeof(PRIMARY_CHECKBOX_OFF_pixelmap_data),    /* the size of pixelmap_data*/
    NULL,
    0,                                       /* auxiliary data size            */
    0x00,                                    /* used for transparent iamges    */
    16,                                      /* width in pixel                 */
    16                                       /* height in pixel                */
};