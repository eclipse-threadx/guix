/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_4bpp_simple_line_draw", /* Test name */
    39, 71, 359, 391  /* Define the coordinates of the capture area.
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
#ifdef win32_graphics_driver_setup_4bpp_grayscale
#undef win32_graphics_driver_setup_4bpp_grayscale  
#endif
#define win32_graphics_driver_setup_4bpp_grayscale  gx_validation_graphics_driver_setup_4bpp_grayscale


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_guix_all_widgets_4bpp.c"

VOID test_line_win_draw(GX_WINDOW *window)
{
    gx_window_draw(window);
    gx_context_brush_width_set(1);
    gx_context_line_color_set(GX_COLOR_ID_WHITE);
    gx_canvas_line_draw(32, 92, 95, 63);
    gx_canvas_line_draw(232, 64, 374, 100);
    gx_canvas_line_draw(40, 51, 80, 91);
    gx_canvas_line_draw(40, 370, 80, 410);
    gx_canvas_line_draw(60, 51, 100, 92);
    gx_canvas_line_draw(60, 370, 100, 411);
    gx_canvas_line_draw(63, 55, 31, 229);
    gx_canvas_line_draw(32, 276, 78, 401);
    gx_canvas_line_draw(33, 82, 200, 48);
    gx_canvas_line_draw(302, 66, 608, 153);
    gx_canvas_line_draw(314, 396, 597, 298);
    gx_canvas_line_draw(35, 93, 98, 5);
    gx_canvas_line_draw(339, 56, 449, 364);

    gx_context_brush_pattern_set(0xf0f0f0f0);

    /* Draw horizontal pattern line. */
    gx_canvas_line_draw(40, 200, 300, 200);

    /* Draw vertical pattern line. */
    gx_canvas_line_draw(200, 72, 200, 300);
}

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
    ToggleScreen(pLineScreen, pSpriteScreen);
    
    gx_widget_draw_set((GX_WIDGET *)&line_screen.line_screen_line_window, test_line_win_draw);

    gx_validation_set_frame_id(1);
    gx_validation_set_frame_comment("test 4bpp simple line draw");
    gx_validation_screen_refresh();
 
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);

}





