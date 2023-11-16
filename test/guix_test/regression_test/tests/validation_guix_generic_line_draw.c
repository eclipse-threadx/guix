/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_display.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_generic_line_draw", /* Test name */
    25, 17, 344, 336  /* Define the coordinates of the capture area.
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

#include "demo_guix_lines.c"

char test_comment[256];
VOID test_line_win_draw(GX_WINDOW *window)
{
    gx_window_draw((GX_WINDOW*) window);

    gx_context_brush_define(GX_COLOR_ID_CANVAS, window->gx_widget_normal_fill_color, GX_BRUSH_ALIAS);
    gx_context_brush_width_set(1);

    gx_canvas_line_draw(24, 13, 23, 12);

    /* Line with round end. aliased. */
    gx_context_brush_define(GX_COLOR_ID_CANVAS, window->gx_widget_normal_fill_color, GX_BRUSH_ALIAS | GX_BRUSH_ROUND);
    gx_context_brush_width_set(5);

    gx_canvas_line_draw(200, 10, 150, 400);
    gx_canvas_line_draw(15, 200, 400, 150);

    /* Line with round end, not aliased. */
    gx_context_brush_define(GX_COLOR_ID_CANVAS, window->gx_widget_normal_fill_color, GX_BRUSH_ROUND);
    gx_canvas_line_draw(220, 10, 170, 400);
    gx_canvas_line_draw(15, 220, 400, 170);

}
/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int frame_id = 1;
GX_DRAW_CONTEXT context;
GX_DISPLAY display;

    gx_widget_draw_set(&main_window.main_window_line_window, test_line_win_draw);

    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("draw one pixel aliased line");
    gx_validation_screen_refresh(); 

    /* Test aliased filled circle draw with null pointer blend func. */
    display.gx_display_driver_pixel_blend = GX_NULL;
    context.gx_draw_context_display = &display;
    _gx_display_driver_generic_aliased_filled_circle_draw(&context, 0, 0, 5);

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}





