/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"
#include "demo_display_resolution_MAIN_DISPLAY_resources.h"
#include "demo_display_resolution_SECONDARY_resources.h"
#include "demo_display_resolution_specifications.h"

#define DROP_LIST_VISIBLE_ROWS 5

GX_WINDOW *pTextScreen;
GX_WINDOW *pWindowScreen;
GX_WINDOW *pButtonScreen;
GX_WINDOW *pIndicatorScreen;
GX_SCROLLBAR list_scroll;

/* Define the ThreadX demo thread control block and stack.  */

GX_WINDOW_ROOT    *root_1;
GX_WINDOW_ROOT    *root_2;

typedef struct {
    GX_PROMPT prompt;
    CHAR text[80];
} DROP_LIST_WIDGET;


/* Define prototypes.   */
VOID start_guix(VOID);
extern UINT win32_graphics_driver_setup_565rgb(GX_DISPLAY *display);
extern UINT win32_graphics_driver_setup_8bit_palette(GX_DISPLAY *display);

int main(int argc, char ** argv)
{
  tx_kernel_enter();
  return(0);
}


VOID tx_application_define(void *first_unused_memory)
{
    /* Create the main demo thread.  */

    start_guix();
}

VOID start_guix(void)
{
    /* Initialize GUIX.  */
    gx_system_initialize();

    gx_studio_display_configure(MAIN_DISPLAY, win32_graphics_driver_setup_565rgb, 
                                MAIN_DISPLAY_LANGUAGE_ENGLISH, MAIN_DISPLAY_THEME_1, &root_1);

    /* create the windows screen */
    gx_studio_named_widget_create("MAIN_DISPLAY_main_window", (GX_WIDGET *) root_1, (GX_WIDGET **) &pWindowScreen);

    gx_studio_display_configure(SECONDARY, win32_graphics_driver_setup_8bit_palette,
        SECONDARY_LANGUAGE_ENGLISH, SECONDARY_THEME_1, &root_2);

    /* create the windows screen */
    gx_studio_named_widget_create("SECONDARY_window", (GX_WIDGET *)root_2, GX_NULL);


    /* Show the root window to make it and patients screen visible.  */
    gx_widget_show(root_1);
    gx_widget_show(root_2);

    /* start the GUIX thread */
    gx_system_start();
}


