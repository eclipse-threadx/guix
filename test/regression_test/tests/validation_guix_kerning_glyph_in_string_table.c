/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_kerning_glyph_in_string_table", /* Test name */
    10, 20, 310, 220  /* Define the coordinates of the capture area.
                         In this test, we only need to capture the line
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

#define REGRESSION_TEST_MODE

#include "gx_validation_wrapper.h"

#include "demo_kerning_glyph_in_string_table.c"

char test_comment[256];

static VOID control_thread_entry(ULONG input)
{
int      frame_id = 1;
GX_EVENT select_event;

    /* Initiate event structure.  */
    memset(&select_event, 0, sizeof(GX_EVENT));

    select_event.gx_event_type = GX_EVENT_SELECT;
    select_event.gx_event_display_handle = 1;
    select_event.gx_event_sender = 0;

    select_event.gx_event_target = (GX_WIDGET *)&window.window_radio_button_theme2;    
    sprintf(test_comment, "Select button theme2");
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment(test_comment);
    gx_system_event_send(&select_event);
    gx_validation_screen_refresh();
    
    
    select_event.gx_event_target = (GX_WIDGET *)&window.window_radio_button_theme1;    
    sprintf(test_comment, "Select button theme1");
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment(test_comment);
    gx_system_event_send(&select_event);
    gx_validation_screen_refresh();

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
