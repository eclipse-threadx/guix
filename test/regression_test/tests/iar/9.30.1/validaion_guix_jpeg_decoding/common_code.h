#include "gx_api.h"
#include "gx_display.h"
#include <stdio.h>

typedef struct MAP_INFO_STRUCT{
  GX_RESOURCE_ID map_id;
  GX_CHAR name[20];
  GX_VALUE xpos;
  GX_VALUE ypos;
}MAP_INFO;

VOID *memory_allocate(ULONG size);
void memory_free(VOID *mem);
UINT graphics_driver_565rgb_setup(GX_DISPLAY *display);
UINT graphics_driver_1555xrgb_setup(GX_DISPLAY *display);
UINT graphics_driver_24xrgb_setup(GX_DISPLAY *display);
UINT graphics_driver_8bit_palette_setup(GX_DISPLAY *display);
UINT graphics_driver_565rgb_rotated_setup(GX_DISPLAY *display);
UINT graphics_driver_1555xrgb_rotated_setup(GX_DISPLAY *display);
UINT graphics_driver_24xrgb_rotated_setup(GX_DISPLAY *display);
UINT graphics_driver_8bit_palette_rotated_setup(GX_DISPLAY *display);
VOID test_start(GX_BOOL g, GX_BOOL t, GX_BOOL s, UINT *results, UINT count);
VOID test_verify_canvas(GX_CANVAS *canvas, INT frame_id);
UINT test_end();
UINT crc_calculate(UCHAR *data, INT len);
VOID test_jpeg_to_canvas_draw(GX_CANVAS *canvas, GX_WINDOW *window, MAP_INFO *test, INT frame_id);
VOID test_jpeg_to_pixelmap_draw(GX_CANVAS *canvas, GX_WINDOW *window, MAP_INFO *test, GX_UBYTE outformat, GX_UBYTE flags, INT frame_id);