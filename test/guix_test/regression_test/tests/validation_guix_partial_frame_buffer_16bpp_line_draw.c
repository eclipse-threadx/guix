/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_partial_frame_buffer_16bpp_line_draw", /* Test name */
    0, 0, 640, 480  /* Define the coordinates of the capture area.
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
#ifdef win32_graphics_driver_setup_565rgb  
#undef win32_graphics_driver_setup_565rgb  
#endif
#define win32_graphics_driver_setup_565rgb  gx_validation_graphics_driver_setup_565rgb


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_guix_partial_frame_buffer_16bpp.c"

static int test_brush_alpha = 255;
static int test_brush_width = 1;

static VOID test_line_win_draw(GX_WINDOW *window)
{
GX_BRUSH *brush;
GX_RECTANGLE *size = &window->gx_widget_size;
int xstart, xend, ystart, yend;

    gx_window_draw((GX_WINDOW*) window);

    gx_context_brush_get(&brush);
    brush -> gx_brush_alpha = test_brush_alpha;
  
    gx_context_brush_width_set(test_brush_width);
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

    gx_context_line_color_set(GX_COLOR_ID_RED);
    gx_canvas_line_draw(20, 51, 60, 91);
    gx_canvas_line_draw(20, 51, 60, 92);

    /* Draw a vertical line in center. */
    gx_canvas_line_draw((size->gx_rectangle_left + size->gx_rectangle_right) >> 1,
                        size->gx_rectangle_top,
                        (size->gx_rectangle_left + size->gx_rectangle_right) >> 1,
                        size->gx_rectangle_bottom);

    /* Draw a horizontal line in center. */
    gx_canvas_line_draw(size->gx_rectangle_left,
                        (size->gx_rectangle_top + size->gx_rectangle_bottom) >> 1,
                        size->gx_rectangle_right,
                        (size->gx_rectangle_top + size->gx_rectangle_bottom) >> 1);

    xstart = size->gx_rectangle_right - 50;
    xend = size->gx_rectangle_right + 50;
    ystart = ((size->gx_rectangle_top + size->gx_rectangle_bottom) >> 1) - 10;
    yend = ((size->gx_rectangle_top + size->gx_rectangle_bottom) >> 1) + 10;

    gx_canvas_line_draw(xstart, ystart, xend, yend);

    xstart = size->gx_rectangle_left - 50;
    xend = size->gx_rectangle_left + 50;
    ystart = ((size->gx_rectangle_top + size->gx_rectangle_bottom) >> 1) - 10;
    yend = ((size->gx_rectangle_top + size->gx_rectangle_bottom) >> 1) + 10;

    gx_canvas_line_draw(xstart, ystart, xend, yend);

    xstart = ((size->gx_rectangle_left + size->gx_rectangle_right) >> 1) - 50;
    xend = ((size->gx_rectangle_left + size->gx_rectangle_right) >> 1) + 50;
    ystart = size->gx_rectangle_bottom - 10;
    yend = size->gx_rectangle_bottom + 10;
    gx_canvas_line_draw(xstart, ystart, xend, yend);

    xstart = ((size->gx_rectangle_left + size->gx_rectangle_right) >> 1) - 50;
    xend = ((size->gx_rectangle_left + size->gx_rectangle_right) >> 1) + 50;
    ystart = size->gx_rectangle_top - 10;
    yend = size->gx_rectangle_top + 10;
    gx_canvas_line_draw(xstart, ystart, xend, yend);

    xstart = size->gx_rectangle_right - 10;
    xend = size->gx_rectangle_right + 10;
    ystart = ((size->gx_rectangle_top + size->gx_rectangle_bottom) >> 1) - 50;
    yend = ((size->gx_rectangle_top + size->gx_rectangle_bottom) >> 1) + 50;

    gx_canvas_line_draw(xstart, ystart, xend, yend);

    xstart = size->gx_rectangle_left - 10;
    xend = size->gx_rectangle_left + 10;
    ystart = ((size->gx_rectangle_top + size->gx_rectangle_bottom) >> 1) - 50;
    yend = ((size->gx_rectangle_top + size->gx_rectangle_bottom) >> 1) + 50;

    gx_canvas_line_draw(xstart, ystart, xend, yend);

    xstart = ((size->gx_rectangle_left + size->gx_rectangle_right) >> 1) - 10;
    xend = ((size->gx_rectangle_left + size->gx_rectangle_right) >> 1) + 10;
    ystart = size->gx_rectangle_bottom - 50;
    yend = size->gx_rectangle_bottom + 50;
    gx_canvas_line_draw(xstart, ystart, xend, yend);

    xstart = ((size->gx_rectangle_left + size->gx_rectangle_right) >> 1) - 10;
    xend = ((size->gx_rectangle_left + size->gx_rectangle_right) >> 1) + 10;
    ystart = size->gx_rectangle_top - 50;
    yend = size->gx_rectangle_top + 50;
    gx_canvas_line_draw(xstart, ystart, xend, yend);

    xstart = ((size->gx_rectangle_left + size->gx_rectangle_right) >> 1) + 50;
    xend = ((size->gx_rectangle_left + size->gx_rectangle_right) >> 1) - 50;
    ystart = size->gx_rectangle_top - 10;
    yend = size->gx_rectangle_top + 10;
    gx_canvas_line_draw(xstart, ystart, xend, yend);

    xstart = ((size->gx_rectangle_left + size->gx_rectangle_right) >> 1) + 50;
    xend = ((size->gx_rectangle_left + size->gx_rectangle_right) >> 1) - 50;
    ystart = size->gx_rectangle_bottom - 10;
    yend = size->gx_rectangle_bottom + 10;
    gx_canvas_line_draw(xstart, ystart, xend, yend);
}

#define TEST_SIZE 3
static char test_comment[255];
static int test_width_list[TEST_SIZE] = {1, 7, 10};
static int test_alpha_list[TEST_SIZE] = {0, 100, 255};

extern int line_angle;
extern int line_width;

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int frame_id = 1;
int width_index;
int alpha_index;

    ToggleScreen((GX_WINDOW *)&line_screen, (GX_WINDOW *)&button_screen);

    for(line_width = 1; line_width < 11; line_width += 5)
    {
        for(line_angle = 0; line_angle < 260; line_angle ++)
        {
            sprintf(test_comment, "line width = %d, line angle = %d", line_width, line_angle);

            gx_validation_screen_refresh();
            gx_validation_set_frame_id(frame_id++);
            gx_validation_set_frame_comment(test_comment);
            gx_validation_write_frame_buffer();
        }
    }

    gx_widget_draw_set((GX_WIDGET *)&line_screen.line_screen_line_window, (void(*)(GX_WIDGET *))test_line_win_draw);

    for(alpha_index = 0; alpha_index < TEST_SIZE;  alpha_index ++)
    {
        test_brush_alpha = test_alpha_list[alpha_index];
        for(width_index = 0; width_index < TEST_SIZE; width_index ++)
        {
            test_brush_width = test_width_list[width_index];
            sprintf(test_comment, "brush alpha = %d, line width = %d", test_brush_alpha, test_brush_width);

            gx_validation_screen_refresh();
            gx_validation_set_frame_id(frame_id++);
            gx_validation_set_frame_comment(test_comment);
            gx_validation_write_frame_buffer();
        }
    }
    
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}