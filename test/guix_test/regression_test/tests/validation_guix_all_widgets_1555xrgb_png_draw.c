/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_system.h"

TEST_PARAM test_parameter = {
    "guix_all_widgets_1555xrgb_png_draw", /* Test name */
    367, 75, 590, 365  /* Define the coordinates of the capture area.
                         In this test, we only need to capture the pixelmap
                         drawing area.  */
};

extern void mark_dirty();
extern void convert_start();

static VOID      control_thread_entry(ULONG);
int main(int argc, char ** argv)
{
    /* Parse the command line argument. */
    gx_validation_setup(argc, argv);

    /* Start ThreadX system */
    tx_kernel_enter(); 
    return(0);
}

VOID tx_application_define(void *first_unused_memory)
{

    /* Create a dedicated thread to perform various operations
       on the pixelmap drawing example. These operations simulate 
       user input. */
    gx_validation_control_thread_create(control_thread_entry);

    /* Termiante the test if it runs for more than 100 ticks */
    /* This function is not implemented yet. */
    gx_validation_watchdog_create(100);

    /* Call the actual line example routine. */
    gx_validation_application_define(first_unused_memory);

}


/* Replace the default graphics driver with the validation driver. */
#ifdef win32_graphics_driver_setup_1555xrgb
#undef win32_graphics_driver_setup_1555xrgb
#endif
#define win32_graphics_driver_setup_1555xrgb  gx_validation_graphics_driver_setup_1555xrgb


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_guix_all_widgets_1555xrgb.c"

char test_comment[256];

VOID png_window_draw(GX_WINDOW *window)
{
int xpos;
int ypos;
GX_PIXELMAP *map;

    xpos = window->gx_window_client.gx_rectangle_left - 10;
    ypos = window->gx_window_client.gx_rectangle_top - 10;

    gx_widget_pixelmap_get(window, GX_PIXELMAP_ID_FISH_PNG, &map);
    gx_canvas_pixelmap_draw(xpos, ypos, map);
}

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int      frame_id = 1;

    /* Toggle to image convert screen. */
    ToggleScreen((GX_WINDOW *)&image_convert_screen, (GX_WINDOW *)&rotate_screen);

    /* Reset draw function. */
    gx_widget_draw_set(&image_convert_screen.image_convert_screen_jpeg_to_1555xrgb_win, png_window_draw);

    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Test 1555xrgb display driver png draw");
    gx_validation_screen_refresh();

    gx_system_memory_allocator_set(GX_NULL, GX_NULL);

    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Set memory allocator null");
    gx_validation_screen_refresh();

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
