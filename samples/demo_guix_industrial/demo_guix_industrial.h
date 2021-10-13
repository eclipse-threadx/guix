/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"

#include "demo_guix_industrial_resources.h"
#include "demo_guix_industrial_specifications.h"

#define ANIMATION_NONE       0
#define ANIMATION_ACTIVE     1


extern SEQUENCE_NUMBER_CONTROL_BLOCK sequence_number;
extern COMPLETE_WINDOW_CONTROL_BLOCK complete_window;
extern SEQUENCE_WINDOW_CONTROL_BLOCK sequence_window;
extern MAIN_SCREEN_CONTROL_BLOCK main_screen;

VOID *memory_allocate(ULONG size);
VOID  memory_free(VOID *mem);

VOID button_indicator_attach(GX_PIXELMAP_BUTTON *button);
VOID screen_toggle_animation_start(GX_WINDOW *show, GX_WINDOW *hide);

VOID sequence_window_init();
VOID sequence_animation_start();
VOID sequence_animation_update();
VOID sequence_animation_stop();

VOID complete_window_timer_start();
VOID complete_window_timer_stop();
VOID complete_window_init();

UINT string_length_get(GX_CONST GX_CHAR* input_string, UINT max_string_length);