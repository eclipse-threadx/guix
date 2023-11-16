/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_all_widgets_24xrgb_rotated_ccw_glyph_draw", /* Test name */
    0, 0, 640, 480  /* Define the coordinates of the capture area.
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
#ifdef win32_graphics_driver_setup_24xrgb_rotated  
#undef win32_graphics_driver_setup_24xrgb_rotated
#endif
#define win32_graphics_driver_setup_24xrgb_rotated  gx_validation_graphics_driver_setup_24xrgb_rotated


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_guix_all_widgets_24xrgb_rotated_ccw.c"

static const char test_string[] = "Test 4bit draw";

VOID test_button_screen_draw(GX_WINDOW *window)
{
GX_RECTANGLE *size;
GX_STRING string;

    size = &window->gx_window_client;
    gx_window_draw(window);

    gx_context_font_set(GX_FONT_ID_NORMAL_4BPP);
    gx_context_line_color_set(GX_COLOR_ID_PURPLE);

    string.gx_string_ptr = test_string;
    string.gx_string_length = sizeof(test_string) - 1;
    gx_canvas_text_draw_ext(size->gx_rectangle_left - 10, size->gx_rectangle_top - 10, &string);
    gx_canvas_text_draw_ext(size->gx_rectangle_right - 10, size->gx_rectangle_top - 11, &string);
    gx_canvas_text_draw_ext(size->gx_rectangle_left - 11, size->gx_rectangle_bottom - 10, &string);
    gx_canvas_text_draw_ext(size->gx_rectangle_right - 11, size->gx_rectangle_bottom - 11, &string);
}

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int frame_id = 1;

    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("brush alpha = 255");
    gx_validation_screen_refresh();

    gx_widget_draw_set(&button_screen, test_button_screen_draw);
    gx_prompt_font_set(&button_screen.button_screen_title_1, GX_FONT_ID_NORMAL_4BPP);
    gx_widget_style_add(&button_screen, GX_STYLE_USE_LOCAL_ALPHA);
    button_screen.gx_widget_alpha = 128;

    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("brush alpha = 128");
    gx_validation_screen_refresh();
 
    button_screen.gx_widget_alpha = 255;
    gx_widget_style_add(&button_screen.button_screen_title_1, GX_STYLE_USE_LOCAL_ALPHA);
    button_screen.button_screen_title_1.gx_widget_alpha = 0;

    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("brush alpha = 255, set title brush alpha to 0");
    gx_validation_screen_refresh();

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}





