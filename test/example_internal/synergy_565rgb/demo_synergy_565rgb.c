/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"

#include "synergy_565rgb_resources.h"
#include "synergy_565rgb_specifications.h"

MAIN_WINDOW_CONTROL_BLOCK *pMainWin;
GX_WINDOW *pic_window;

/* Define the ThreadX demo thread control block and stack.  */

TX_THREAD          demo_thread;
UCHAR              demo_thread_stack[4096];

GX_WINDOW_ROOT    *root;
GX_BOOL            alpha = GX_FALSE;
GX_BOOL            compressed = GX_FALSE;
INT                xshift;
INT                yshift;
INT                horizontal_slider = 0;
INT                vertical_slider = 0;

/* Define prototypes.   */
VOID  demo_thread_entry(ULONG thread_input);
extern UINT win32_dave2d_graphics_driver_setup_565rgb(GX_DISPLAY *display);


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


    gx_studio_display_configure(DISPLAY_1, win32_dave2d_graphics_driver_setup_565rgb, 
                                LANGUAGE_ENGLISH, DISPLAY_1_THEME_1, &root);

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

        case GX_SIGNAL(ID_COMPRESSED, GX_EVENT_TOGGLE_ON):
            compressed = GX_TRUE;
            gx_system_dirty_mark(pic_window);
            break;

        case GX_SIGNAL(ID_COMPRESSED, GX_EVENT_TOGGLE_OFF):
            compressed = GX_FALSE;
            gx_system_dirty_mark(pic_window);
            break;

        default:
            return gx_window_event_process(window, myevent);
    }
    return status;
}

VOID pic_win_draw(GX_WINDOW *window)
{

    if(compressed)
        {
            gx_window_wallpaper_set(window, GX_PIXELMAP_ID_RED_APPLE, GX_FALSE);
        }
    else
        {
            gx_window_wallpaper_set(window, GX_PIXELMAP_ID_FISH, GX_FALSE);
        }

    gx_window_draw((GX_WINDOW*) window);
}