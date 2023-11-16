/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_widget_text_draw", /* Test name */
    0, 0, 639, 479  /* Define the coordinates of the capture area.
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
#include "demo_guix_all_widgets.c"

static void test_widget_text_draw(GX_WINDOW * window)
{
GX_STRING string;

    gx_window_background_draw(window);

#ifdef GX_ENABLE_DEPRECATED_STRING_API    
    gx_widget_text_draw(window, GX_COLOR_ID_ORANGE, GX_FONT_ID_SYSTEM, "test string 1", 10, 10);
#endif

    string.gx_string_ptr = "test string 3";
    string.gx_string_length = strlen(string.gx_string_ptr);
    gx_widget_text_blend_ext(window, GX_COLOR_ID_ORANGE, 1024, &string, 10, 50, 128);

    string.gx_string_ptr = GX_NULL;
    string.gx_string_length = 0;
    gx_widget_text_blend_ext(window, GX_COLOR_ID_ORANGE, GX_FONT_ID_SYSTEM, &string, 10, 100, 128);

}

static void test_widget_text_id_draw(GX_WINDOW *window)
{

    gx_window_background_draw(window);

    /* Draw text at (20, 30) of current window.  */
    gx_widget_text_id_draw(window, GX_COLOR_ID_ORANGE, GX_FONT_ID_PROMPT,
                           GX_STRING_ID_STRING_5, 20, 30);

    /* Draw a null string at (10, 20) of current window.  */
    gx_widget_text_id_draw(window, GX_COLOR_ID_ORANGE, GX_FONT_ID_PROMPT,
                           0, 10, 20);
}

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
INT frame_id = 1;

    gx_widget_draw_set(pButtonScreen, test_widget_text_draw);
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Test gx_widget_text_draw");
    gx_validation_screen_refresh();

    gx_widget_draw_set(pButtonScreen, test_widget_text_id_draw);
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Test gx_widget_text_id_draw");
    gx_validation_screen_refresh();

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
