/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"
#include "widget_find_resources.h"
#include "widget_find_specifications.h"

/* Define the ThreadX demo thread control block and stack.  */

GX_WINDOW_ROOT    *root;
ULONG *canvas_memory;


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
    /* Initialize GUIX.  */
    gx_system_initialize();

    gx_studio_display_configure(PRIMARY, win32_graphics_driver_setup_24xrgb, 
                                LANGUAGE_ENGLISH, PRIMARY_THEME_1, &root);

   // Use the canvas memory define API
    canvas_memory = (ULONG *) malloc(PRIMARY_X_RESOLUTION * PRIMARY_Y_RESOLUTION * sizeof(ULONG));
    gx_canvas_memory_define(root->gx_window_root_canvas, canvas_memory,
                            PRIMARY_X_RESOLUTION * PRIMARY_Y_RESOLUTION * sizeof(ULONG));

    /* create the main screen */
    gx_studio_named_widget_create("main_screen", (GX_WIDGET *) root, GX_NULL);

    /* Show the root window to make it and patients screen visible.  */
    gx_widget_show(root);

    /* start the GUIX thread */
    gx_system_start();
}

UINT main_screen_event_process(GX_WINDOW *widget, GX_EVENT *event_ptr)
{
    GX_WIDGET *test_1;
    GX_WIDGET *test_2;

    if (event_ptr->gx_event_type == GX_EVENT_SHOW)
    {
        // Use gx_widget_first_child_get API:
        gx_widget_first_child_get(&main_screen, &test_1);

        if (!test_1)
        {
            return GX_FAILURE;
        }

        // Use the gx_widget_next_sibling_get API:
        gx_widget_next_sibling_get(test_1, &test_2);

        if (!test_2)
        {
            return GX_FAILURE;
        }

        // Use gx_widget_last_child_get API:
        gx_widget_last_child_get(&main_screen, &test_1);

        if (!test_1)
        {
            return GX_FAILURE;
        }

        // Use the gx_widget_previous_sibling_get API:
        gx_widget_previous_sibling_get(test_1, &test_2);
        if (!test_2)
        {
            return GX_FAILURE;
        }

        // Use gx_widget_parent_get API
        gx_widget_parent_get(test_2, &test_1);
        if (test_1 != (GX_WIDGET *) &main_screen)
        {
            return GX_FAILURE;
        }

        // Use gx_widget_top_visible_child_find
        gx_widget_top_visible_child_find(&main_screen, &test_1);

        if (!test_1)
        {
            return GX_FAILURE;
        }
    }
    return gx_window_event_process(widget, event_ptr);
}