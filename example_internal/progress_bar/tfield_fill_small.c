#include "gx_api.h"

/* TFIELD_FILL_SMALL pixelmap data                                             */

static GX_CONST UINT TFIELD_FILL_SMALL_pixelmap_data[36] =
{
    0xff864f2a, 0xffbe6f3b, 0xffca895d, 0xffcc8d62, 0xffcb8c60, 0xffcb8b5f, 0xffca895d, 
    0xffc9875b, 0xffc98659, 0xffc88558, 0xffc78355, 0xffc68254, 0xffc58051, 0xffc47e4e, 
    0xffc37b4b, 0xffc37a4a, 0xffc27847, 0xffc17644, 0xffbf723f, 0xffbe703d, 0xffbc6e3a, 
    0xffba6c39, 0xffb86b38, 0xffb56836, 0xffb26736, 0xffaf6635, 0xffab6434, 0xffa96233, 
    0xffa56032, 0xffa25f32, 0xffa05d31, 0xff9d5c30, 0xff9b5b2f, 0xff98592f, 0xff94572e, 
    0xd486502a
};

/* TFIELD_FILL_SMALL auxiliary data                                            */

static GX_CONST GX_UBYTE TFIELD_FILL_SMALL_pixelmap_aux_data[36] =
{
    0x83, 0x83, 0x83, 0x83, 0x83, 0x83, 0x83, 0x83, 0x83, 0x83, 0x83, 0x83, 0x83, 
    0x83, 0x83, 0x83, 0x83, 0x83, 0x83, 0x83, 0x83, 0x83, 0x83, 0x83, 0x83, 0x83, 
    0x83, 0x83, 0x83, 0x83, 0x83, 0x83, 0x83, 0x83, 0x83, 0x83
};
GX_CONST GX_PIXELMAP TFIELD_FILL_SMALL_pixelmap =
{
    0x00000001,                              /* major version                  */
    0x00000000,                              /* minor version                  */
    GX_PIXELMAP_COMPRESSED|GX_PIXELMAP_ALPHA,         /* flags                 */
    GX_COLOR_FORMAT_24XRGB,              /* Format                         */
    (GX_UBYTE *)TFIELD_FILL_SMALL_pixelmap_data,
    sizeof(TFIELD_FILL_SMALL_pixelmap_data),    /* the size of pixelmap_data   */
    (GX_UBYTE *)TFIELD_FILL_SMALL_pixelmap_aux_data,
    sizeof(TFIELD_FILL_SMALL_pixelmap_aux_data),    /* the size of pixelmap_data*/
    0x0000,                                  /* used for transparent GIF iamges*/
    4,                                       /* width in pixel                 */
    36                                       /* height in pixel                */
};
