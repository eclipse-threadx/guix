/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_8bpp_simple_line_draw", /* Test name */
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
#ifdef win32_graphics_driver_setup_8bit_palette  
#undef win32_graphics_driver_setup_8bit_palette  
#endif
#define win32_graphics_driver_setup_8bit_palette  gx_validation_graphics_driver_setup_8bit_palette


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_guix_line_8bpp.c"
/* Define the angles values for the line. */
extern GX_STUDIO_DISPLAY_INFO lines_8bpp_display_table[];

static int brush_alpha = 255;

VOID test_line_win_draw(GX_WINDOW *window)
{
GX_BRUSH   *brush;
GX_DISPLAY *display;
GX_COLOR   *old_palette;
int         old_palette_size;
GX_BOOL palette_test_failed = GX_FALSE;

    gx_context_brush_get(&brush);
    brush->gx_brush_alpha = brush_alpha;

    gx_window_draw((GX_WINDOW*)window);
    gx_context_brush_width_set(1);
    gx_context_line_color_set(GX_COLOR_ID_GREEN);
    gx_canvas_line_draw(20, -6, 60, 34);
    gx_canvas_line_draw(20, 313, 60, 353);
    gx_canvas_line_draw(7, 36, 96, 6);
    gx_canvas_line_draw(270, 7, 370, 41);

    gx_canvas_line_draw(40, -6, 80, 35);
    gx_canvas_line_draw(40, 313, 80, 354);
    gx_canvas_line_draw(51, 6, 16, 161);
    gx_canvas_line_draw(18, 176, 60, 339);

    gx_canvas_line_draw(12, 24, 152, 0);
    gx_canvas_line_draw(295, 9, 616, 134);
    gx_canvas_line_draw(22, 33, 44, -7);
    gx_canvas_line_draw(330, 9, 412, 315);

    display = root->gx_window_root_canvas->gx_canvas_display;
    old_palette = display->gx_display_palette;
    old_palette_size = display->gx_display_palette_size;
    display->gx_display_driver_palette_set(display, old_palette, 0);
    gx_context_raw_line_color_set(0x123456);

    if(brush->gx_brush_line_color != 0)
    {
        palette_test_failed = GX_TRUE;
    }
    brush->gx_brush_line_color = 1;

    display->gx_display_driver_palette_set(display, GX_NULL, 0);
    gx_context_raw_line_color_set(0x123456);
    if(brush->gx_brush_line_color != 0)
    {
       palette_test_failed = GX_TRUE;
    }

    if(!palette_test_failed)
    {
        display->gx_display_driver_palette_set(display, old_palette, old_palette_size);
    }
}

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int frame_id = 1;
GX_CONST GX_THEME *theme_ptr;

    /*Save palette. */
    theme_ptr = lines_8bpp_display_table[DISPLAY_1].theme_table[0];
    gx_validation_write_palette(theme_ptr -> theme_palette, theme_ptr->theme_palette_size);

    gx_widget_draw_set((GX_WIDGET *)&main_window.main_window_line_window, test_line_win_draw);

    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Test 8bpp simple line draw, brush_alpha = 255");
    gx_validation_screen_refresh();

    brush_alpha = 0;
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Test 8bpp simple line draw, brush_alpha = 0");
    gx_validation_screen_refresh();

    brush_alpha = 128;
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Test 8bpp simple line draw, brush_alpha = 128");
    gx_validation_screen_refresh();

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}





