/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"

#include "extended_unicode_resources.h"
#include "extended_unicode_specifications.h"

GX_WINDOW_ROOT    *root;
extern SCREEN_BASE_CONTROL_BLOCK screen_base;

/* Define prototypes.   */
extern UINT win32_graphics_driver_setup_24xrgb(GX_DISPLAY *display);

int main(int argc, char ** argv)
{
  tx_kernel_enter();
  return(0);
}

/******************************************************************************************/
VOID tx_application_define(void *first_unused_memory)
{

    /* Initialize GUIX.  */
    gx_system_initialize();

    gx_studio_display_configure(MAIN_DISPLAY, win32_graphics_driver_setup_24xrgb,
        LANGUAGE_ENGLISH, MAIN_DISPLAY_DEFAULT_THEME, &root);

    /* create the main screen */
    gx_studio_named_widget_create("screen_base", (GX_WIDGET *)root, GX_NULL);

    /* Show the root window to make it and patients screen visible.  */
    gx_widget_show(root);

    /* start GUIX thread */
    gx_system_start();
}