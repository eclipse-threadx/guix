/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include <string.h>
#include "gx_api.h"

#include "radial_progress_bar_resources.h"
#include "radial_progress_bar_specifications.h"

/* Define the ThreadX demo thread control block and stack.  */

TX_THREAD          demo_thread;
UCHAR              demo_thread_stack[4096];
TX_BYTE_POOL       memory_pool;

GX_WINDOW_ROOT    *root;
GX_RADIAL_PROGRESS_BAR *progress_bar = &progress_bar_window.progress_bar_window_radial_progress_bar;
GX_RADIAL_PROGRESS_BAR_INFO  *info = &progress_bar_window.progress_bar_window_radial_progress_bar.gx_radial_progress_bar_info;

#define            SCRATCHPAD_PIXELS (DISPLAY_1_X_RESOLUTION * DISPLAY_1_Y_RESOLUTION)
GX_COLOR           scratchpad[SCRATCHPAD_PIXELS];

/* Define prototypes.   */
extern UINT win32_graphics_driver_setup_24xrgb(GX_DISPLAY *display);

GX_CHAR anchor_value[5];
GX_CHAR normal_width[5];
GX_CHAR selected_width[5];
GX_CHAR radius[5];
INT     radial_progress_bar_alpha = 255;

VOID *memory_allocate(ULONG size)
{
    VOID *memptr;

    if (tx_byte_allocate(&memory_pool, &memptr, size, TX_NO_WAIT) == TX_SUCCESS)
    {
        return memptr;
    }

    return NULL;
}

VOID memory_free(VOID *mem)
{
    tx_byte_release(mem);
}

/******************************************************************************************/
int main(int argc, char ** argv)
{
  tx_kernel_enter();
  return(0);
}

/******************************************************************************************/
VOID tx_application_define(void *first_unused_memory)
{
    /* create byte pool for rotate to use */
    tx_byte_pool_create(&memory_pool, "scratchpad", scratchpad,
        SCRATCHPAD_PIXELS * sizeof(GX_COLOR));

    /* Initialize GUIX.  */
    gx_system_initialize();

    /* install our memory allocator and de-allocator */
    gx_system_memory_allocator_set(memory_allocate, memory_free);

    gx_studio_display_configure(DISPLAY_1, win32_graphics_driver_setup_24xrgb,
        LANGUAGE_ENGLISH, DISPLAY_1_THEME_1, &root);

    /* create the gauge_window */
    gx_studio_named_widget_create("progress_bar_window", (GX_WIDGET *)root, GX_NULL);

    /* Show the root window to make it and patients screen visible.  */
    gx_widget_show(root);

    /* let GUIX run */
    gx_system_start();
}


/******************************************************************************************/
UINT window_event_handle(GX_WINDOW *win, GX_EVENT *event_ptr)
{
int pos;
GX_STRING str;

    switch (event_ptr->gx_event_type)
    {
    case GX_SIGNAL(ID_POS, GX_EVENT_SLIDER_VALUE):
        pos = event_ptr->gx_event_payload.gx_event_longdata;
        gx_radial_progress_bar_value_set(progress_bar, -pos);
        break;

    case GX_SIGNAL(ID_SHOW_TEXT, GX_EVENT_TOGGLE_ON):
        gx_widget_style_add((GX_WIDGET *)progress_bar, GX_STYLE_PROGRESS_TEXT_DRAW);
        break;

    case GX_SIGNAL(ID_SHOW_TEXT, GX_EVENT_TOGGLE_OFF):
        gx_widget_style_remove((GX_WIDGET *)progress_bar, GX_STYLE_PROGRESS_TEXT_DRAW);
        break;

    case GX_SIGNAL(ID_SHOW_PERCENT, GX_EVENT_TOGGLE_ON):
        gx_widget_style_add((GX_WIDGET *)progress_bar, GX_STYLE_PROGRESS_PERCENT);
        break;

    case GX_SIGNAL(ID_SHOW_PERCENT, GX_EVENT_TOGGLE_OFF):
        gx_widget_style_remove((GX_WIDGET *)progress_bar, GX_STYLE_PROGRESS_PERCENT);
        break;

    case GX_SIGNAL(ID_TRANSPARENT, GX_EVENT_TOGGLE_ON):
        gx_widget_style_add((GX_WIDGET *)progress_bar, GX_STYLE_TRANSPARENT);
        break;

    case GX_SIGNAL(ID_TRANSPARENT, GX_EVENT_TOGGLE_OFF):
        gx_widget_style_remove((GX_WIDGET *)progress_bar, GX_STYLE_TRANSPARENT);
        break;

    case GX_SIGNAL(ID_ALIAS, GX_EVENT_TOGGLE_ON):
        gx_widget_style_add((GX_WIDGET *)progress_bar, GX_STYLE_RADIAL_PROGRESS_ALIAS);
        break;

    case GX_SIGNAL(ID_ALIAS, GX_EVENT_TOGGLE_OFF):
        gx_widget_style_remove((GX_WIDGET *)progress_bar, GX_STYLE_RADIAL_PROGRESS_ALIAS);
        break;

    case GX_SIGNAL(ID_ROUND_END, GX_EVENT_TOGGLE_ON):
        gx_widget_style_add((GX_WIDGET *)progress_bar, GX_STYLE_RADIAL_PROGRESS_ROUND);
        break;

    case GX_SIGNAL(ID_ROUND_END, GX_EVENT_TOGGLE_OFF):
        gx_widget_style_remove((GX_WIDGET *)progress_bar, GX_STYLE_RADIAL_PROGRESS_ROUND);
        break;

    case GX_SIGNAL(ID_NORMAL_WIDTH_SLIDER, GX_EVENT_SLIDER_VALUE):
        pos = event_ptr->gx_event_payload.gx_event_longdata;
        gx_utility_ltoa(pos, normal_width, 5);
        str.gx_string_ptr = normal_width;
        str.gx_string_length = strnlen(normal_width, sizeof(normal_width) - 1);
        gx_prompt_text_set_ext(&progress_bar_window.progress_bar_window_normal_width_prompt, &str);
        info->gx_radial_progress_bar_info_normal_brush_width = pos;
        gx_radial_progress_bar_info_set(progress_bar, info);
        break;

    case GX_SIGNAL(ID_SELECTED_WIDTH_SLIDER, GX_EVENT_SLIDER_VALUE):
        pos = event_ptr->gx_event_payload.gx_event_longdata;
        gx_utility_ltoa(pos, selected_width, 5);
        str.gx_string_ptr = selected_width;
        str.gx_string_length = strnlen(selected_width, sizeof(selected_width) - 1);
        gx_prompt_text_set_ext(&progress_bar_window.progress_bar_window_selected_width_prompt, &str);
        info->gx_radial_progress_bar_info_selected_brush_width = pos;
        gx_radial_progress_bar_info_set(progress_bar, info);
        break;

    case GX_SIGNAL(ID_RADIUS_SLIDER, GX_EVENT_SLIDER_VALUE):
        pos = event_ptr->gx_event_payload.gx_event_longdata;
        gx_utility_ltoa(pos, radius, 5);
        str.gx_string_ptr = radius;
        str.gx_string_length = strnlen(radius, sizeof(radius) - 1);
        gx_prompt_text_set_ext(&progress_bar_window.progress_bar_window_radius_prompt, &str);
        info->gx_radial_progress_bar_info_radius = pos;
        gx_radial_progress_bar_info_set(progress_bar, info);
        break;

    case GX_SIGNAL(ID_ANCHOR_SLIDER, GX_EVENT_SLIDER_VALUE):
        pos = event_ptr->gx_event_payload.gx_event_longdata;
        gx_utility_ltoa(pos, anchor_value, 5);
        str.gx_string_ptr = anchor_value;
        str.gx_string_length = strnlen(anchor_value, sizeof(anchor_value) - 1);
        gx_prompt_text_set_ext(&progress_bar_window.progress_bar_window_anchor_prompt, &str);
        gx_radial_progress_bar_anchor_set(progress_bar, pos);
        break;

    case GX_SIGNAL(ID_ALPHA_SLIDER, GX_EVENT_SLIDER_VALUE):
        radial_progress_bar_alpha = event_ptr->gx_event_payload.gx_event_longdata;
        gx_numeric_prompt_value_set(&progress_bar_window.progress_bar_window_alpha_prompt, radial_progress_bar_alpha);
        gx_system_dirty_mark((GX_WIDGET *)&progress_bar_window.progress_bar_window_radial_progress_bar);
        break;
    }

    return gx_window_event_process(win, event_ptr);
}


VOID radial_progress_bar_draw(GX_RADIAL_PROGRESS_BAR *progress_bar)
{
GX_BRUSH *brush;

    gx_context_brush_get(&brush);
    brush->gx_brush_alpha = radial_progress_bar_alpha;

    gx_radial_progress_bar_draw(progress_bar);
}
