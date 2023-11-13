/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"
#include "sprite_resources.h"
#include "sprite_specifications.h"

/* Define the ThreadX demo thread control block and stack.  */

GX_WINDOW_ROOT    *root;
GX_BOOL            on_off = GX_TRUE;

/* Define prototypes.   */
VOID start_guix(VOID);
extern UINT win32_graphics_driver_setup_24xrgb(GX_DISPLAY *display);

int main(int argc, char ** argv)
{
  tx_kernel_enter();
  return(0);
}

VOID tx_application_define(void *first_unused_memory)
{
    start_guix();
}

VOID start_guix(void)
{
    GX_RECTANGLE winsize;

    /* Initialize GUIX.  */
    gx_system_initialize();

    gx_studio_display_configure(PRIMARY, win32_graphics_driver_setup_24xrgb, 
                                LANGUAGE_ENGLISH, PRIMARY_THEME_1, &root);

    /* create the button screen */
    gx_studio_named_widget_create("main_screen", root, GX_NULL);

    /* Show the root window to make it and patients screen visible.  */
    gx_widget_show(root);

    /* start the GUIX thread */
    gx_system_start();
}

UINT main_screen_event_handler(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
    case GX_SIGNAL(ID_BUTTON, GX_EVENT_CLICKED):
        if (on_off)
        {
            on_off = GX_FALSE;
            gx_sprite_start(&main_screen.main_screen_sprite_1, 0);
        }
        else
        {
            on_off = GX_TRUE;
            gx_sprite_start(&main_screen.main_screen_sprite_2, 0);
        }
        break;

    default:
        return gx_window_event_process(window, event_ptr);
    }

    return 0;
}
