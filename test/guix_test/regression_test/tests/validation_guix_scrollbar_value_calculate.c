/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_scrollbar_value_calculate", /* Test name */
    347, 43, 567, 187  /* Define the coordinates of the capture area.
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
int              frame_id = 1;
GX_EVENT         my_event;
GX_SCROLLBAR    *test_scroll = &window_screen.window_screen_vertical_scroll_1;
GX_SCROLL_THUMB *test_thumb = &test_scroll->gx_scrollbar_thumb;
GX_VALUE         height;

    ToggleScreen(pWindowScreen, pButtonScreen);
    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_display_handle = 1;

    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("toggle to window_screen");
    gx_validation_screen_refresh();

    my_event.gx_event_target = (GX_WIDGET *)test_thumb;
    my_event.gx_event_type = GX_EVENT_VERTICAL_SCROLL;
    my_event.gx_event_payload.gx_event_intdata[0] = test_thumb->gx_widget_size.gx_rectangle_top;
    
    gx_widget_height_get(test_thumb, &height);
    my_event.gx_event_payload.gx_event_intdata[1] = height;
    gx_system_event_send(&my_event);

    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("send vertical scroll event to scroll thumb, with thumb position unchanged.");
    gx_validation_screen_refresh();

    height = (test_scroll->gx_widget_size.gx_rectangle_bottom -
              test_scroll->gx_widget_size.gx_rectangle_top + 1);
    height -= (test_scroll->gx_scrollbar_appearance.gx_scroll_thumb_travel_max +
               test_scroll->gx_scrollbar_appearance.gx_scroll_thumb_travel_min);

    my_event.gx_event_payload.gx_event_intdata[1] = height;
    gx_system_event_send(&my_event);

    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("send vertical scroll event to scroll thumb, with thumb height equal to scroll height");
    gx_validation_screen_refresh();

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
