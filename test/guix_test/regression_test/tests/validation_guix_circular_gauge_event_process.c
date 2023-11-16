/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_circular_gauge_event_process", /* Test name */
    33, 28, 600, 450  /* Define the coordinates of the capture area.
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
#ifdef win32_graphics_driver_setup_24xrgb  
#undef win32_graphics_driver_setup_24xrgb  
#endif
#define win32_graphics_driver_setup_24xrgb  gx_validation_graphics_driver_setup_24xrgb


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_guix_all_widgets.c"

char test_comment[256];


/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int frame_id = 1;
GX_CIRCULAR_GAUGE *gauge;
GX_EVENT my_event;
GX_CIRCULAR_GAUGE new_gauge;
GX_CIRCULAR_GAUGE_INFO circular_gauge_info;

    gauge = &gauge_screen.gauge_screen_gauge_1;
    gx_widget_event_process_set(pGaugeScreen, gx_window_event_process);
    ToggleScreen(pGaugeScreen,pButtonScreen);

    gx_widget_show((GX_WIDGET *)gauge);
    gx_widget_hide((GX_WIDGET *)gauge);
    gx_widget_show((GX_WIDGET *)gauge);
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Send GX_EVENT_SHOW event to a circular gauge with needle source.");
    gx_validation_screen_refresh();
    
    /* gx_circular_gauge_info_animation_steps = 0. */
    gx_circular_gauge_animation_set(gauge, 0, 0);
    memset(&my_event, 0, sizeof(my_event));
    my_event.gx_event_type = GX_EVENT_HIDE;
    my_event.gx_event_target = (GX_WIDGET *)gauge;
    gx_system_event_send(&my_event);
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Send GX_EVENT_HIDE event to a circular gauge with 0 animation steps.");
    gx_validation_screen_refresh();

    memset(&my_event, 0, sizeof(my_event));
    my_event.gx_event_type = GX_EVENT_TIMER;
    my_event.gx_event_target = (GX_WIDGET *)gauge;
    my_event.gx_event_payload.gx_event_timer_id = GX_CIRCULAR_GAUGE_TIMER + 1;
    gx_system_event_send(&my_event);
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Send invalid GX_EVENT_TIMER event to a circular gauge.");
    gx_validation_screen_refresh();

    /* animation_delay == 0 */
    memset(&circular_gauge_info, 0, sizeof(circular_gauge_info));
    circular_gauge_info.gx_circular_gauge_info_animation_delay = 0;
    gx_circular_gauge_create(&new_gauge, "new_circular_gauge", GX_NULL, &circular_gauge_info, 0, 0, 0, 0, 0);

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
