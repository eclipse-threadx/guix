/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_system.h"

TEST_PARAM test_parameter = {
    "guix_sl_input_input_focus_release", /* Test name */
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
#include "demo_guix_sl_text_input_16bpp.c"

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
GX_SINGLE_LINE_TEXT_INPUT *text_input;
INT                        frame_id = 1;
GX_EVENT                   event;

    gx_widget_detach(pMainScreen);
    gx_widget_attach(root, &input_focus_release_test);
    text_input = &input_focus_release_test.input_focus_release_test_text_input;

    /* Remove cursor blink style. */
    gx_single_line_text_input_style_remove(text_input, GX_STYLE_CURSOR_BLINK);

    memset(&event, 0, sizeof(GX_EVENT));
    event.gx_event_type = GX_EVENT_PEN_DOWN;
    event.gx_event_target = (GX_WIDGET *)text_input;

    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Pen down on text input");
    gx_system_event_send(&event);

    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Pen up on text input");
    event.gx_event_type = GX_EVENT_PEN_UP;
    gx_system_event_send(&event);

     
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
