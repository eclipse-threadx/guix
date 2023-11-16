/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_system.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_ml_text_input_create", /* Test name */
    0, 0, 100, 100  /* Define the coordinates of the capture area.
                         In this test, we only need to capture the multi 
                         line text input drawing area.  */
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
       on the multi line text input example. These operations simulate 
       user input. */
    gx_validation_control_thread_create(control_thread_entry);

    /* Terminate the test if it runs for more than 100 ticks */
    /* This function is not implemented yet. */
    gx_validation_watchdog_create(100);

    /* Call the actual multi line text input routine. */
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

#include "demo_guix_ml_text_input_16bpp.c"


char test_comment[256];
char input_buffer[10];
/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int                       frame_id = 1;
GX_MULTI_LINE_TEXT_INPUT  text_input;
GX_RECTANGLE              size;

    gx_utility_rectangle_define(&size, 0, 0, 100, 100);

    memset(input_buffer, 'x', sizeof(input_buffer));
    memset(&text_input, 0, sizeof(GX_MULTI_LINE_TEXT_INPUT));
    gx_multi_line_text_input_create(&text_input, "ml_input", GX_NULL, input_buffer, 10, GX_STYLE_ENABLED | GX_STYLE_CURSOR_ALWAYS, 1024, &size);

    gx_widget_attach((GX_WIDGET *)&window, (GX_WIDGET *)&text_input);
 
    gx_validation_set_frame_id(frame_id++);
    sprintf(test_comment, "Create a multi line text input with GX_STYLE_CURSOR_ALWAYS style");
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();

    /* Assign focus for the created multi line text input. */
    gx_system_focus_claim(&text_input);

    gx_multi_line_text_input_style_remove(&text_input, GX_STYLE_CURSOR_ALWAYS);
    gx_validation_set_frame_id(frame_id++);
    sprintf(test_comment, "remove style GX_STYLE_CURSOR_ALWAYS");
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();

    gx_validation_end();

    exit(0);
}
