/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"

#include "generic_16bpp_32bppdriver_resources.h"
#include "generic_16bpp_32bppdriver_specifications.h"

#define  format_565rgb      0
#define  format_4444argb    1

MAIN_WINDOW_CONTROL_BLOCK *pMainWin;
GX_WINDOW *pic_window;

/* Define the ThreadX demo thread control block and stack.  */

TX_THREAD          demo_thread;
UCHAR              demo_thread_stack[4096];

GX_WINDOW_ROOT    *root;
GX_BOOL            alpha = GX_FALSE;
GX_BOOL            compressed = GX_FALSE;
GX_UBYTE           format = format_565rgb;

/* Define prototypes.   */
VOID  demo_thread_entry(ULONG thread_input);
extern UINT win32_graphics_driver_setup_24xrgb(GX_DISPLAY *display);


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


    gx_studio_display_configure(DISPLAY_1, win32_graphics_driver_setup_24xrgb, 
                                LANGUAGE_ENGLISH, DISPLAY_1_DEFAULT_THEME, &root);

    /* create the main screen */
    gx_studio_named_widget_create("main_window", (GX_WIDGET *) root, (GX_WIDGET **) &pMainWin);
    pic_window = &pMainWin ->main_window_pic_window;

    /* Show the root window to make it and patients screen visible.  */
    gx_widget_show(root);

    /* let GUIX run */
    gx_system_start();
}

UINT main_event_handler(GX_WINDOW *window, GX_EVENT *myevent)
{
    UINT status = 0;

    switch(myevent -> gx_event_type)
    {
    case GX_SIGNAL(ID_ALPHA, GX_EVENT_TOGGLE_ON):
        alpha = GX_TRUE;
        gx_system_dirty_mark(pic_window);
        break;

    case GX_SIGNAL(ID_ALPHA, GX_EVENT_TOGGLE_OFF):
        alpha = GX_FALSE;
        gx_system_dirty_mark(pic_window);
        break;

    case GX_SIGNAL(ID_COMPRESSED, GX_EVENT_TOGGLE_ON):
        compressed = GX_TRUE;
        gx_system_dirty_mark(pic_window);
        break;

    case GX_SIGNAL(ID_COMPRESSED, GX_EVENT_TOGGLE_OFF):
        compressed = GX_FALSE;
        gx_system_dirty_mark(pic_window);
        break;

    case GX_SIGNAL(ID_565RGB,GX_EVENT_RADIO_SELECT):
        format = format_565rgb;
        gx_system_dirty_mark(pic_window);
        break;

    case GX_SIGNAL(ID_4444ARGB,GX_EVENT_RADIO_SELECT):
        format = format_4444argb;
        gx_system_dirty_mark(pic_window);
        break;

    default:
        status = gx_window_event_process(window, myevent);
        break;
    }
    return status;
}



VOID pic_win_draw(GX_WINDOW *window)
{
    switch(format)
    {
    case format_4444argb:
        if(compressed)
        {
            gx_window_wallpaper_set(window, GX_PIXELMAP_ID_RAINBOW200_ALPHA_COMPRESS, GX_FALSE);
        }
        else
        {
            gx_window_wallpaper_set(window, GX_PIXELMAP_ID_RAINBOW200_ALPHA_NONCOMPRESS, GX_FALSE);
        }
        break;

    case format_565rgb:
        if(alpha)
        {
            if(compressed)
            {
                gx_window_wallpaper_set(window, GX_PIXELMAP_ID_CAPSULE, GX_FALSE);
            }
            else
            {
                gx_window_wallpaper_set(window, GX_PIXELMAP_ID_ICON_FOOT_ALPHA, GX_FALSE);
            }
        }
        else
        {
            if(compressed)
            {
                gx_window_wallpaper_set(window, GX_PIXELMAP_ID_FISH, GX_FALSE);
            }
            else
            {
                gx_window_wallpaper_set(window, GX_PIXELMAP_ID_RED_APPLE, GX_FALSE);
            }
        }
        break;
    }

    gx_window_draw((GX_WINDOW*) window);
}
