/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"

#include "pixelmap_clip_32bpp_resources.h"
#include "pixelmap_clip_32bpp_specifications.h"

WINDOW_CONTROL_BLOCK *pMainWin;
GX_WINDOW *pPixelmapWin;

/* Define the ThreadX demo thread control block and stack.  */

TX_THREAD          demo_thread;
UCHAR              demo_thread_stack[4096];

GX_WINDOW_ROOT    *root;
GX_BOOL  alpha = GX_FALSE;
GX_BOOL  compressed = GX_FALSE;

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

    gx_studio_display_configure(MAIN_DISPLAY, win32_graphics_driver_setup_24xrgb, 
        LANGUAGE_ENGLISH, MAIN_DISPLAY_DEFAULT_THEME, &root);


    /* Create the hello world screen.  */
    gx_studio_named_widget_create("window", (GX_WIDGET *) root, (GX_WIDGET **) &pMainWin);

    /* Get pointer to pixelmap window.  */
    pPixelmapWin = &pMainWin ->window_pixelmap_window;

    gx_widget_status_add(pPixelmapWin, GX_STATUS_MOVABLE);

    /* Show the root window to make it and patients screen visible.  */
    gx_widget_show(root);

    /* let GUIX run */
    gx_system_start();
}


UINT main_event_handler(GX_WINDOW *widget, GX_EVENT *event_ptr)
{

    switch (event_ptr->gx_event_type)
    {
    case GX_SIGNAL(ID_CHECKBOX_ALPHA, GX_EVENT_TOGGLE_ON):
        alpha = GX_TRUE;
        break;

    case GX_SIGNAL(ID_CHECKBOX_ALPHA, GX_EVENT_TOGGLE_OFF):
        alpha = GX_FALSE;
        break;

    case GX_SIGNAL(ID_CHECKBOX_COMPRESSED, GX_EVENT_TOGGLE_ON):
        compressed = GX_TRUE;
        break;;

    case GX_SIGNAL(ID_CHECKBOX_COMPRESSED, GX_EVENT_TOGGLE_OFF):
        compressed = GX_FALSE;
        break;;

    }

    if (alpha)
    {
        if (compressed)
        {
            gx_window_wallpaper_set(pPixelmapWin, GX_PIXELMAP_ID_RAINBOW200_ALPHA_COMPRESS, GX_FALSE);
        }
        else
        {
            gx_window_wallpaper_set(pPixelmapWin, GX_PIXELMAP_ID_RAINBOW200_ALPHA_NONCOMPRESS, GX_FALSE);
        }
    }
    else
    {
        if (compressed)
        {
            gx_window_wallpaper_set(pPixelmapWin, GX_PIXELMAP_ID_RAINBOW200_NOALPHA_COMPRESS, GX_FALSE);
        }
        else
        {
            gx_window_wallpaper_set(pPixelmapWin, GX_PIXELMAP_ID_RAINBOW200_NOALPHA_NONCOMPRESS, GX_FALSE);
        }
    }
    return (gx_window_event_process(widget, event_ptr));
}

