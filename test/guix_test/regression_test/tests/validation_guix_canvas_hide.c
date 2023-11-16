/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_canvas_hide", /* Test name */
    0, 0, 639, 479  /* Define the coordinates of the capture area.
                         In this test, we only need to capture the scrollbar
                         drawing area.  */
};

static VOID      control_thread_entry(ULONG);
int main(int argc, char ** argv)
{
    /* Parse the command scrollbar argument. */
    gx_validation_setup(argc, argv);

    /* Start ThreadX system */
    tx_kernel_enter(); 
    return(0);
}

VOID tx_application_define(void *first_unused_memory)
{

    /* Create a dedicated thread to perform various operations
       on the scrollbar drawing example. These operations simulate 
       user input. */
    gx_validation_control_thread_create(control_thread_entry);

    /* Termiante the test if it runs for more than 100 ticks */
    /* This function is not implemented yet. */
    gx_validation_watchdog_create(100);

    /* Call the actual scrollbar routine. */
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

#include "demo_guix_all_widgets_16bpp.c"

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
INT frame_id = 1;
GX_EVENT myevent;

    /* Simulate click on next button. */
    memset(&myevent, 0, sizeof(GX_EVENT));
    myevent.gx_event_type = GX_SIGNAL(IDB_NEXT, GX_EVENT_CLICKED);
    myevent.gx_event_target = (GX_WIDGET *)&button_screen;
    gx_system_event_send(&myevent);

    gx_canvas_hide(&animation_canvas);
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Hide animation canvas.");
    gx_validation_screen_refresh();

    gx_canvas_offset_set(root->gx_window_root_canvas, 0, 10);
    gx_canvas_offset_set(&animation_canvas, 0, 10);
    gx_canvas_show(&animation_canvas);
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Set canvas offset (0, 10), show animation canvas.");
    gx_validation_screen_refresh();

    gx_canvas_hide(&animation_canvas);
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Hide animation canvas");
    gx_validation_screen_refresh();

    gx_canvas_offset_set(root->gx_window_root_canvas, 10, 10);
    gx_canvas_offset_set(&animation_canvas, 10, 10);
    gx_canvas_show(&animation_canvas);
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Set canvas offset (10, 10), show animation canvas.");
    gx_validation_screen_refresh();

    gx_canvas_hide(&animation_canvas);
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Hide animation canvas.");
    gx_validation_screen_refresh();

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
