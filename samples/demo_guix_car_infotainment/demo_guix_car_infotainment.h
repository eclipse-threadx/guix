/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"

#include "demo_guix_car_infotainment_resources.h"
#include "demo_guix_car_infotainment_specifications.h"

extern VIDEO_SCREEN_CONTROL_BLOCK video_screen;
extern NAVIGATION_SCREEN_CONTROL_BLOCK navigation_screen;
extern CAMERA_SCREEN_CONTROL_BLOCK camera_screen;
extern SPEAKER_WINDOW_CONTROL_BLOCK speaker_window;
extern AUDIO_SCREEN_CONTROL_BLOCK audio_screen;
extern VEHICLE_SCREEN_CONTROL_BLOCK vehicle_screen;
extern SCREEN_BASE_CONTROL_BLOCK screen_base;
extern MAIN_SCREEN_CONTROL_BLOCK main_screen;

VOID *memory_allocate(ULONG size);
void memory_free(VOID *mem);

void screen_toggle(GX_WINDOW *new_win, GX_WINDOW *old_win);
VOID wave_slider_create();
VOID jpg_decode(GX_RESOURCE_ID res_id, GX_PIXELMAP *decoded_map);
VOID map_delete(GX_PIXELMAP *decoded_map);
UINT string_length_get(GX_CONST GX_CHAR* input_string, UINT max_string_length);

extern GX_PIXELMAP    decoded_map;
extern GX_PIXELMAP    resized_map;

extern INT blend_alpha;
extern GX_CONST GX_PIXELMAP *main_display_theme_1_pixelmap_table[];