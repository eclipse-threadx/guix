/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"

#include "demo_guix_washing_machine_resources.h"
#include "demo_guix_washing_machine_specifications.h"

VOID remain_time_set(INT seconds);

VOID animation_wave_draw(GX_WINDOW* window);
VOID wash_cycle_window_init(INT remain_seconds);
VOID wash_cycle_window_remain_time_update(INT remain_seconds);

VOID mode_select_window_init();
VOID mode_select_window_remain_time_update(INT remain_seconds);

VOID water_level_window_init();