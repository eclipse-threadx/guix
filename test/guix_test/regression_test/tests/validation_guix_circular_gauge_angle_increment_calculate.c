/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_circular_gauge_angle_increment_calculate", /* Test name */
    160, 78, 450, 370  /* Define the coordinates of the capture area.
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

char test_comment[255];

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int                frame_id = 1;
GX_CIRCULAR_GAUGE *gauge = &((GAUGE_SCREEN_CONTROL_BLOCK *)pGaugeScreen)->gauge_screen_gauge_1;
 
    gx_widget_event_process_set((GX_WIDGET *)&gauge_screen, gx_window_event_process);
    ToggleScreen((GX_WINDOW *)&gauge_screen, (GX_WINDOW *)&button_screen);
    
    gauge->gx_circular_gauge_info.gx_circular_gauge_info_animation_delay = 0;
    gx_circular_gauge_angle_set(gauge, -114);
    
    gx_validation_capture_frames(frame_id, 10, GX_NULL, 0, 1000);
    
    gx_validation_capture_frames_wait(); 

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
