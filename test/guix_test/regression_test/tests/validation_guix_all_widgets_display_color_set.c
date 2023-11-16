/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_all_widgets_display_color_set", /* Test name */
    73, 7, 576, 476  /* Define the coordinates of the capture area.
                         In this test, we only need to capture the line
                         drawing area.  */
};

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
       on the line drawing example. These operations simulate 
       user input. */
    gx_validation_control_thread_create(control_thread_entry);

    /* Termiante the test if it runs for more than 100 ticks */
    /* This function is not implemented yet. */
    gx_validation_watchdog_create(100);

    /* Call the actual line example routine. */
    gx_validation_application_define(first_unused_memory);

}


/* Replace the default graphics driver with the validation driver. */
#ifdef win32_graphics_driver_setup_24xrgb  
#undef win32_graphics_driver_setup_24xrgb  
#endif
#define win32_graphics_driver_setup_24xrgb  gx_validation_graphics_driver_setup_24xrgb


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_guix_all_widgets.c"

#define      display_color_table_size   35
static GX_COLOR display_color_table[display_color_table_size] =
{
    0x00000000,
    0x00787c78,
    0x00e2e2e2,
    0x009b9a73,
    0x007599aa,
    0x00000000,
    0x00ffffff,
    0x000000b8,
    0x00808080,
    0x00dadada,
    0x00e0c060,
    0x00f8f8e0,
    0x00f8ecb0,
    0x00000000,
    0xffbababa,
    0xff7d7d7d,
    0x00ffffff,
    0x00282c28,
    0x00000000,
    0x00c0c0c0,
    0x00ffffff,
    0x00000000,
    0x00808080,
    0x00c0c0c0,
    0x00e0c060,
    0x00a0a0a0,
    0x00787c78,
    0xffffffff,
    0x00787c78,
    0x00ff8040,
    0x00000000,
    0x000000ff,
    0x00ffffff,
    0x004f9d9d,
    0x00387272
};

static VOID control_thread_entry(ULONG input)
{
int       frame_id = 1;

    gx_display_color_table_set(root->gx_window_root_canvas->gx_canvas_display, display_color_table, display_color_table_size);
    gx_display_color_set(root->gx_window_root_canvas->gx_canvas_display, GX_COLOR_ID_WINDOW_FILL, 0xff0000); // set window fill red

    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Set color of index, window fill color, in display color table to red.");
    gx_validation_screen_refresh();

    gx_display_color_table_set(root->gx_window_root_canvas->gx_canvas_display, GX_NULL, 0);
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Set color table null");
    gx_validation_screen_refresh();

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
