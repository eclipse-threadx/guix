/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_1bpp_simple_line_draw", /* Test name */
   20, 20, 400, 460  /* Define the coordinates of the capture area.
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
#ifdef win32_graphics_driver_setup_monochrome  
#undef win32_graphics_driver_setup_monochrome  
#endif
#define win32_graphics_driver_setup_monochrome  gx_validation_graphics_driver_setup_monochrome


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_line_1bpp.c"

char test_comment[256];

static VOID test_line_win_draw(GX_WIDGET *widget)
{
    gx_context_brush_width_set(line_width);

    gx_context_line_color_set(GX_1BPP_BLACK);

    gx_canvas_line_draw(6, 45, 143, 6);
    gx_canvas_line_draw(270, 469, 424, 426);
    gx_canvas_line_draw(218, 5, 416, 46);
    gx_canvas_line_draw(19, 419, 200, 470);
    gx_canvas_line_draw(42, 4, 11, 229);
    gx_canvas_line_draw(362, 0, 408, 218);
    gx_canvas_line_draw(12, 266, 41, 468);
    gx_canvas_line_draw(408, 275, 365, 470);
    gx_canvas_line_draw(7, 34, 394, 2);
    gx_canvas_line_draw(333, 13, 622, 153);
    gx_canvas_line_draw(12, 391, 80, 574);
    gx_canvas_line_draw(381, 1, 512, 351);

    gx_canvas_line_draw(20, 0, 60, 40);
    gx_canvas_line_draw(20, 440, 60, 480);

    gx_canvas_line_draw(60, 0, 100, 41);
    gx_canvas_line_draw(60, 440, 100, 481);

    gx_context_raw_line_color_set(0x00ff00);
    gx_canvas_line_draw(50, 440, 90, 481);
     
    gx_context_raw_line_color_set(0x0000ff);
    gx_canvas_line_draw(40, 440, 80, 481);
}

static VOID control_thread_entry(ULONG input)
{
int frame_id = 1;

    /* Reset line wind draw function because line color is hard-coded in original draw function in demo file,
       Rewrite this draw function to change line color. */
    gx_widget_draw_set((GX_WIDGET *)&line_window, test_line_win_draw);
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Test 1bpp simple line draw with clip");
    gx_validation_screen_refresh();

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}









