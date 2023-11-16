/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_332rgb_simple_line_alpha_draw", /* Test name */
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
#ifdef win32_graphics_driver_setup_332rgb
#undef win32_graphics_driver_setup_332rgb
#endif
#define win32_graphics_driver_setup_332rgb  gx_validation_graphics_driver_setup_332rgb


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_all_widgets_332rgb.c"

/*NOTICE: Number of xpos_value and ypos_valus should be same.*/
char test_comment[256];

VOID test_line_win_draw(GX_WINDOW *window)
{
    GX_BRUSH *brush = GX_NULL;

    gx_window_draw((GX_WINDOW*)window);

    gx_context_brush_width_set(1);
    gx_context_brush_get(&brush);

    brush->gx_brush_alpha = 128;

    /* Clip in two sides, delta_x >= delta_y. */
    gx_canvas_line_draw(40, 51, 80, 91);
    gx_canvas_line_draw(40, 370, 80, 410);
    gx_canvas_line_draw(31, 99, 147, 61);
    gx_canvas_line_draw(177, 61, 368, 127);

    /* Clip in two sides, delta_x < delta_y. */
    gx_canvas_line_draw(60, 51, 100, 92);
    gx_canvas_line_draw(60, 370, 100, 411);
    gx_canvas_line_draw(75, 60, 32, 318);
    gx_canvas_line_draw(33, 195, 63, 401);

    /* Clip in one sides, delta_x >= delta_y. */
    gx_canvas_line_draw(32, 93, 339, 1);
    gx_canvas_line_draw(281, 62, 633, 17);

    /* Clip in one sides, delta_x < delta_y. */
    gx_canvas_line_draw(34, 92, 102, 5);
    gx_canvas_line_draw(341, 64, 474, 387);
}

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int frame_id = 1;

    ToggleScreen((GX_WINDOW *)&line_screen, pSpriteScreen);

    gx_widget_draw_set((GX_WIDGET *)&line_screen.line_screen_line_window, test_line_win_draw);
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("test 332rgb simple line alpha draw");
    gx_validation_screen_refresh();

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);

}





