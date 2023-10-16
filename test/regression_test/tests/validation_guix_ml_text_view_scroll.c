/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_ml_text_view_scroll", /* Test name */
    50, 119, 224, 317  /* Define the coordinates of the capture area.
                         In this test, we only need to capture the pixelmap slider
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
#ifdef win32_graphics_driver_setup_565rgb  
#undef win32_graphics_driver_setup_565rgb  
#endif
#define win32_graphics_driver_setup_565rgb  gx_validation_graphics_driver_setup_565rgb


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_guix_ml_text_view_16bpp.c"

char test_comment[256];

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
INT             frame_id = 1;
GX_EVENT        my_event;
GX_MULTI_LINE_TEXT_VIEW *view;

    view = &main_screen.main_screen_text_view;

    memset(&my_event, 0, sizeof(GX_EVENT));

    /* Detach vertical scrollbar. */
    gx_widget_detach((GX_WIDGET *)&main_screen.main_screen_vertical_scroll_1);
    
    gx_widget_style_add((GX_WIDGET *)view, GX_STYLE_TRANSPARENT);

    sprintf(test_comment, "Remove scrollbar and add transparent style");
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();

    my_event.gx_event_display_handle = 1;
    my_event.gx_event_type = GX_EVENT_VERTICAL_SCROLL;
    my_event.gx_event_target = (GX_WIDGET *)view;
    my_event.gx_event_payload.gx_event_intdata[1] = 0;
    my_event.gx_event_payload.gx_event_intdata[0] = 100;
    gx_system_event_send(&my_event);

    sprintf(test_comment, "scroll down by 100");
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();
       
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
