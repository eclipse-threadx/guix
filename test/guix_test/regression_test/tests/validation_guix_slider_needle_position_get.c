/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_system.h"
#include "gx_validation_utility.h"
#include "gx_slider.h"

TEST_PARAM test_parameter = {
    "guix_slider_needle_position_get", /* Test name */
    197, 124, 256, 282  /* Define the coordinates of the capture area.
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
GX_SLIDER *vslider;
GX_SLIDER *hslider;
GX_SLIDER_INFO slider_info;
GX_EVENT       my_event;

    memset(&my_event, 0, sizeof(GX_EVENT));

    ToggleScreen(pIndicatorScreen, pButtonScreen);

    vslider = &indicator_screen.indicator_screen_slider_1;
    hslider = &indicator_screen.indicator_screen_slider;

    /* Change slider info. */
    slider_info = vslider -> gx_slider_info;
    slider_info.gx_slider_info_max_val = slider_info.gx_slider_info_min_val;
    slider_info.gx_slider_info_needle_width = 0;

    /* Set slider value. */
    gx_slider_value_set(vslider, &slider_info, slider_info.gx_slider_info_min_val);

    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("set slider value");
    gx_validation_screen_refresh();

    /* Call slider needle position get with return position set to null. */
    _gx_slider_needle_position_get(vslider, &slider_info, GX_NULL);
    _gx_slider_needle_position_get(hslider, &hslider->gx_slider_info, GX_NULL);

    vslider->gx_slider_tick_count = 1;
    my_event.gx_event_type = GX_EVENT_PEN_DRAG;
    my_event.gx_event_target = (GX_WIDGET *)vslider;
    gx_system_event_send(&my_event);
    
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("set tick_count = 1, send GX_EVENT_PEN_DRAG event");
    gx_validation_screen_refresh();
   
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}





