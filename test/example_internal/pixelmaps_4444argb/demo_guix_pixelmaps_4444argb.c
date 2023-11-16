/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include <string.h>
#include "gx_api.h"

#include "pixelmaps_4444argb_resources.h"
#include "pixelmaps_4444argb_specifications.h"

MAIN_WINDOW_CONTROL_BLOCK *pMainWin;
GX_WINDOW *pic_window;

/* Define the ThreadX demo thread control block and stack.  */

TX_THREAD          demo_thread;
UCHAR              demo_thread_stack[4096];

GX_WINDOW_ROOT    *root;
INT                alpha = 1;
INT                compress = 2;
INT                style_index = 3;
INT                depth_inex = 0;
GX_UBYTE           brush_alpha = 0xff;

/* maps[depth_inex][style_index] */
GX_RESOURCE_ID   maps[4] = { GX_PIXELMAP_ID_RAW_4444ARGB, GX_PIXELMAP_ID_ALPHA_4444ARGB, GX_PIXELMAP_ID_COMPRESS_4444ARGB, GX_PIXELMAP_ID_COMPRESS_ALPHA_4444ARGB };

/* Define prototypes.   */
VOID  demo_thread_entry(ULONG thread_input);
extern UINT win32_graphics_driver_setup_4444argb(GX_DISPLAY *display);


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


    gx_studio_display_configure(DISPLAY_1, win32_graphics_driver_setup_4444argb,
                                LANGUAGE_ENGLISH, DISPLAY_1_DEFAULT_THEME, &root);

    /* create the main screen */
    gx_studio_named_widget_create("main_window", (GX_WIDGET *) root, (GX_WIDGET **) &pMainWin);
    pic_window = &pMainWin ->main_window_pic_window;

    /* Show the root window to make it and patients screen visible.  */
    gx_widget_show(root);

    /* let GUIX run */
    gx_system_start();
}

VOID update_prompt_value(GX_PROMPT *pp, INT value)
{
    static GX_CHAR str_buf[10];
    GX_STRING str;

    if (pp)
    {
        gx_utility_ltoa(value, str_buf, 10);
        str.gx_string_ptr = str_buf;
        str.gx_string_length = strnlen(str_buf, sizeof(str_buf) - 1);
        gx_prompt_text_set_ext(pp, &str);
    }
}

UINT main_event_handler(GX_WINDOW *window, GX_EVENT *myevent)
{
    GX_PROMPT *prompt;

    switch (myevent->gx_event_type)
    {
    case GX_SIGNAL(ID_BRUSH_ALPHA_SLIDER, GX_EVENT_SLIDER_VALUE):
        brush_alpha = (GX_UBYTE)myevent->gx_event_payload.gx_event_longdata;
        prompt = &pMainWin -> main_window_prompt_12_1;
        update_prompt_value(prompt, brush_alpha);
        break;

    case GX_SIGNAL(ID_ALPHA, GX_EVENT_TOGGLE_ON):
        alpha = 1;
        break;

    case GX_SIGNAL(ID_ALPHA, GX_EVENT_TOGGLE_OFF):
        alpha = 0;
        break;

    case GX_SIGNAL(ID_COMPRESSED, GX_EVENT_TOGGLE_ON):
        compress = 2;
        break;

    case GX_SIGNAL(ID_COMPRESSED, GX_EVENT_TOGGLE_OFF):
        compress = 0;
        break;

    default:
        return gx_window_event_process(window, myevent);
    }

    style_index = alpha + compress;
    gx_window_wallpaper_set(pic_window, maps[style_index], GX_FALSE);
    return 0;
}

VOID pic_win_draw(GX_WINDOW *window)
{
GX_BRUSH *brush;

    gx_context_brush_get(&brush);
    brush -> gx_brush_alpha = brush_alpha;

    gx_window_draw((GX_WINDOW*) window);
}
