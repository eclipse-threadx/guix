/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include <string.h>
#include "gx_api.h"

#include "alphamap_16bpp_resources.h"
#include "alphamap_16bpp_specifications.h"

/* Define the ThreadX demo thread control block and stack.  */

TX_THREAD          demo_thread;
UCHAR              demo_thread_stack[4096];

GX_WINDOW_ROOT    *root;
#define            COLOR_ID_BLUE   0xf0
#define            COLOR_ID_RED 0xf0f0
#define            COLOR_ID_GREEN 0xf00

GX_COLOR          fill_color = COLOR_ID_BLUE;
GX_UBYTE          alpha_value = 0xff;
/* Define prototypes.   */
VOID  demo_thread_entry(ULONG thread_input);
extern UINT win32_graphics_driver_setup_565rgb(GX_DISPLAY *display);


int main(int argc, char ** argv)
{
  tx_kernel_enter();
  return(0);
}


VOID tx_application_define(void *first_unused_memory)
{

    /* Create the main demo thread.  */
    tx_thread_create(&demo_thread, "GUIX Demo Thread", demo_thread_entry, 
                     0,  demo_thread_stack, sizeof(demo_thread_stack), 
                     1, 1, TX_NO_TIME_SLICE, TX_AUTO_START);
}


VOID  demo_thread_entry(ULONG thread_input)
{
    /* Initialize GUIX.  */
    gx_system_initialize();


    gx_studio_display_configure(DISPLAY_1, win32_graphics_driver_setup_565rgb, 
                                LANGUAGE_ENGLISH, DISPLAY_1_DEFAULT_THEME, &root);

    /* create the main screen */
    gx_studio_named_widget_create("main_window", (GX_WIDGET *) root, NULL);

    /* Show the root window to make it and patients screen visible.  */
    gx_widget_show(root);

    /* let GUIX run */
    gx_system_start();
}

VOID pixelmap_wnd_draw(GX_WINDOW *widget)
{
GX_BRUSH    *brush;
GX_PIXELMAP *pixelmap = GX_NULL;

    gx_context_brush_get(&brush);
    brush->gx_brush_fill_color = fill_color;
    brush->gx_brush_alpha = alpha_value;

    gx_widget_pixelmap_get(widget, widget->gx_window_wallpaper, &pixelmap);
    gx_canvas_pixelmap_draw(widget->gx_widget_size.gx_rectangle_left,
                            widget->gx_widget_size.gx_rectangle_top,
                            pixelmap);
}

VOID update_prompt_value(GX_PROMPT *pp, INT value)
{
    GX_STRING str;
    static GX_CHAR str_buf[10];

    if (pp)
    {
        gx_utility_ltoa(value, str_buf, 10);
        str.gx_string_ptr = str_buf;
        str.gx_string_length = strnlen(str_buf, 10);
        gx_prompt_text_set_ext(pp, &str);
    }
}

UINT window_event_handler(GX_WINDOW *window, GX_EVENT *event_ptr)
{
GX_PROMPT *prompt;

    switch (event_ptr->gx_event_type)
    {
    case GX_SIGNAL(ID_RADIO_BUTTON_BLUE, GX_EVENT_RADIO_SELECT):
        fill_color = COLOR_ID_BLUE;
        gx_system_dirty_mark((GX_WIDGET *)&main_window);
        break;

    case GX_SIGNAL(ID_RADIO_BUTTON_RED, GX_EVENT_RADIO_SELECT):
        fill_color = COLOR_ID_RED;
        gx_system_dirty_mark((GX_WIDGET *)&main_window);
        break;

    case GX_SIGNAL(ID_RADIO_BUTTON_GREEN, GX_EVENT_RADIO_SELECT):
        fill_color = COLOR_ID_GREEN;
        gx_system_dirty_mark((GX_WIDGET *)&main_window);
        break;

    case GX_SIGNAL(ID_SLIDER_ALPHA, GX_EVENT_SLIDER_VALUE):
        alpha_value = (GX_UBYTE)event_ptr->gx_event_payload.gx_event_longdata;
        prompt = &(main_window.main_window_prompt_alpha);
        update_prompt_value(prompt, alpha_value);
        gx_system_dirty_mark((GX_WIDGET *)&main_window);
        break;
    }

    return gx_window_event_process(window, event_ptr);
}
