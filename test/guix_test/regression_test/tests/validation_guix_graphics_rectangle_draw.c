/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_graphics_rectangle_draw", /* Test name */
    28, 24, 400, 430  /* Define the coordinates of the capture area.
                         In this test, we only need to capture the pixelmap
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
#include "demo_guix_graphics_32bpp.c"

char test_comment[255];

VOID test_graphics_draw(GX_WINDOW *window)
{
    GX_RECTANGLE rect;

    gx_window_background_draw(window);

    gx_context_brush_width_set(4);
    gx_context_raw_line_color_set(GX_COLOR_LIGHTGRAY);
    gx_context_raw_fill_color_set(GX_COLOR_GREEN);
                            
    rect = window->gx_widget_size;
    gx_utility_rectangle_shift(&rect, 0, -4);
    gx_canvas_rectangle_draw(&rect);
}

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int          frame_id = 1;
GX_WINDOW    test_top_window;
GX_RECTANGLE size;

    draw_shape = RECTANGLE;

    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Test rectangle draw");
    gx_validation_screen_refresh();

    /* Reset draw function of graphics window for testing. */
    gx_widget_draw_set(pGraphicsWin, test_graphics_draw);
    memset(&test_top_window, 0, sizeof(GX_WINDOW));
    size = pGraphicsWin->gx_widget_size;
    gx_utility_rectangle_shift(&size, size.gx_rectangle_right, 0);
    gx_window_create(&test_top_window, "test_window", root, 0, 0, &size);

    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Create second top screen and test rectangle draw");
    gx_validation_screen_refresh();
    
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
