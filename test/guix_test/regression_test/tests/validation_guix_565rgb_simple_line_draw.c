/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_565rgb_simple_line_draw", /* Test name */
    20, 65, 350, 400  /* Define the coordinates of the capture area.
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

#ifdef win32_graphics_driver_setup_565rgb  
#undef win32_graphics_driver_setup_565rgb  
#endif
/* Replace the default graphics driver with the validation driver. */
#define win32_graphics_driver_setup_565rgb  gx_validation_graphics_driver_setup_565rgb


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_guix_all_widgets_16bpp.c"

#define LINE_CENTER_X_OFFSET 159
#define LINE_CENTER_Y_OFFSET 159
#define LINE_LENGTH 100

static int test_brush_alpha = 255;

static VOID test_line_win_draw(GX_WINDOW *window)
{
GX_BRUSH *brush;

    gx_window_draw((GX_WINDOW*) window);

    gx_context_brush_get(&brush);
    brush -> gx_brush_alpha = test_brush_alpha;
  
    gx_context_brush_width_set(1);
    gx_context_line_color_set(GX_COLOR_ID_ORANGE);
    gx_canvas_line_draw(24, 47, 383, 195);

    gx_context_line_color_set(GX_COLOR_ID_BLACK);
    gx_canvas_line_draw(12, 354, 277, 400);

    gx_context_line_color_set(GX_COLOR_ID_BLUE);
    gx_canvas_line_draw(7, 88, 131, 60);

    gx_context_line_color_set(GX_COLOR_ID_BROWN);
    gx_canvas_line_draw(52, 47, 12, 204);

    gx_context_line_color_set(GX_COLOR_ID_PINK);
    gx_canvas_line_draw(14, 106, 622, 6);

    gx_context_line_color_set(GX_COLOR_ID_GREEN);
    gx_canvas_line_draw(20, 107, 83, 5);

    gx_context_line_color_set(GX_COLOR_ID_PURPLE);
    gx_canvas_line_draw(253, 61, 622, 209);

    gx_context_line_color_set(GX_COLOR_ID_GRAY);
    gx_canvas_line_draw(318, 51, 447, 472);

    gx_context_line_color_set(GX_COLOR_ID_INDIAN_RED);
    gx_canvas_line_draw(20, 51, 60, 91);
    gx_canvas_line_draw(20, 51, 60, 92);
}

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int frame_id = 1;

    gx_widget_draw_set((GX_WIDGET *)&lines_screen.lines_screen_line_window, (void(*)(GX_WIDGET *))test_line_win_draw);
    ToggleScreen((GX_WINDOW *)&lines_screen, (GX_WINDOW *)&button_screen);

    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Test 565rgb simple line draw");
    gx_validation_screen_refresh();

    test_brush_alpha = 0;
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Set brush alpha 0");
    gx_validation_screen_refresh();

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}





