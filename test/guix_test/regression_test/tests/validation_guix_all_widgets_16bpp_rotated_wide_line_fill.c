/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_system.h"
#include "gx_display.h"

TEST_PARAM test_parameter = {
    "guix_all_widgets_16bpp_rotated_wide_line_fill", /* Test name */
    20, 65, 350, 400  /* Define the coordinates of the capture area.
                         In this test, we only need to capture the line
                         drawing area.  */
};

extern GX_WINDOW     *pLineWin;

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

#ifdef win32_graphics_driver_setup_565rgb_rotated  
#undef win32_graphics_driver_setup_565rgb_rotated
#endif
#define win32_graphics_driver_setup_565rgb_rotated  gx_validation_graphics_driver_setup_565rgb_rotated


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_guix_all_widgets_16bpp_rotated.c"

static VOID test_line_win_draw(GX_WINDOW *window)
{
GX_RECTANGLE   *size;
GX_FIXED_POINT *points;

    gx_window_draw((GX_WINDOW*) window);

    gx_context_brush_define(GX_COLOR_ID_CANVAS, GX_COLOR_ID_CANVAS, GX_BRUSH_ALIAS);
    gx_context_brush_width_set(1);

    _gx_system_current_draw_context->gx_draw_context_clip = &window->gx_widget_size;
    size = &window->gx_widget_size;

    /* Test _gx_display_generic_wide_line_points_calcualte. */
    points = _gx_display_driver_generic_wide_line_points_calculate(_gx_system_current_draw_context,
                           size->gx_rectangle_left + 50, size->gx_rectangle_top + 50,
                           size->gx_rectangle_left + 50, size->gx_rectangle_top + 100, 4, GX_FALSE);

    _gx_display_driver_generic_rotated_wide_line_fill(_gx_system_current_draw_context, points);

    points = _gx_display_driver_generic_wide_line_points_calculate(_gx_system_current_draw_context,
                           size->gx_rectangle_left + 50, size->gx_rectangle_top + 50,
                           size->gx_rectangle_left + 100, size->gx_rectangle_top + 50, 4, GX_FALSE);

    _gx_display_driver_generic_rotated_wide_line_fill(_gx_system_current_draw_context, points);
}

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int frame_id = 1;

    gx_widget_draw_set((GX_WIDGET *)pLineWin, (void(*)(GX_WIDGET *))test_line_win_draw);
    ToggleScreen((GX_WINDOW *)&lines_screen, (GX_WINDOW *)&button_screen);

    gx_validation_set_frame_id(frame_id);
    gx_validation_set_frame_comment("Draw horizontal and vertical wide line.");
    gx_validation_screen_refresh();

    root->gx_window_root_canvas->gx_canvas_display->gx_display_driver_pixel_blend = GX_NULL;

    gx_validation_set_frame_id(frame_id);
    gx_validation_set_frame_comment("Set blend func to GX_NULL.");
    gx_validation_screen_refresh();

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}





