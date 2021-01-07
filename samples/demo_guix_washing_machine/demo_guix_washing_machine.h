/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"

#include "demo_guix_washing_machine_resources.h"
#include "demo_guix_washing_machine_specifications.h"

#define POWER_ON  1
#define POWER_OFF 2

#define TEMPERATURE_WINDOW_LONG_LINE_WIDTH  60
#define TEMPERATURE_WINDOW_SHORT_LINE_WIDTH 18

VOID washer_on_page_init();
VOID washer_on_page_power_off();
VOID garments_page_init();
void garments_page_power_off();
VOID water_level_page_init();
VOID water_level_page_power_off();
VOID temperature_page_init();
VOID temperature_page_power_off();
VOID widget_enable_disable(GX_WIDGET *widget, INT status);
void memory_free(VOID *mem);
UINT string_length_get(GX_CONST GX_CHAR* input_string, UINT max_string_length);
GX_BOOL is_power_on();