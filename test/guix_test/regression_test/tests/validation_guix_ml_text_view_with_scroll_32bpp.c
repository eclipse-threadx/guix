/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_system.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_ml_text_view_with_scroll_32bpp", /* Test name */
    20, 45, 229, 243  /* Define the coordinates of the capture area.
                         In this test, we only need to capture the multi 
                         line text view drawing area.  */
};

static VOID      control_thread_entry(ULONG);
int main(int argc, char ** argv)
{
#ifdef GX_UTF8_SUPPORT
    /* Parse the command line argument. */
    gx_validation_setup(argc, argv);
#else
    printf("guix_ml_text_view_with_scroll_32bpp: Skip Testing!\n");
#endif
    /* Start ThreadX system */
    tx_kernel_enter(); 
    return(0);
}

VOID tx_application_define(void *first_unused_memory)
{

    /* Create a dedicated thread to perform various operations
       on the multi line text view example. These operations simulate 
       user input. */
    gx_validation_control_thread_create(control_thread_entry);

    /* Terminate the test if it runs for more than 100 ticks */
    /* This function is not implemented yet. */
    gx_validation_watchdog_create(100);

    /* Call the actual multi line text view routine. */
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

#include "demo_guix_ml_text_with_scroll_32bpp.c"

char test_comment[256];

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
#ifdef GX_UTF8_SUPPORT
int ypos;
int frame_id = 1;
GX_EVENT scroll_event;

    /* multi line text view scroll down. */
    for(ypos = 75; ypos < 370; ypos += 10)
    {
        /* Inform the validation system 
        (1) Frame ID, which identifies a specific test configuration;
        (2) Start recording frame on the next toggle operation.
        */
        gx_validation_set_frame_id(frame_id);
	
        sprintf(test_comment, "scroll down by 5");
        gx_validation_set_frame_comment(test_comment);
        
        GX_ENTER_CRITICAL
        scroll_event.gx_event_display_handle = 1;
	scroll_event.gx_event_type = GX_EVENT_PEN_DOWN;
        scroll_event.gx_event_payload.gx_event_pointdata.gx_point_y = ypos;
        scroll_event.gx_event_payload.gx_event_pointdata.gx_point_x = 220;
        gx_system_event_send(&scroll_event);

        scroll_event.gx_event_type = GX_EVENT_PEN_DRAG;
        scroll_event.gx_event_payload.gx_event_pointdata.gx_point_y = ypos + 5;
        gx_system_event_send(&scroll_event);
        GX_EXIT_CRITICAL
        /* Force a screen refresh. */
        gx_validation_screen_refresh();

        /* Increment frame ID. */
        frame_id ++;
    }

    /* multi line text view scroll up */
    for(ypos = 280; ypos >= 80; ypos -= 10)
    {
        /* Inform the validation system 
        (1) Frame ID, which identifies a specific test configuration;
        (2) Start recording frame on the next toggle operation.
        */
        gx_validation_set_frame_id(frame_id);
	
        sprintf(test_comment, "scroll up by 5");
        gx_validation_set_frame_comment(test_comment);
        
        GX_ENTER_CRITICAL
        scroll_event.gx_event_display_handle = 1;
	scroll_event.gx_event_type = GX_EVENT_PEN_DOWN;
        scroll_event.gx_event_payload.gx_event_pointdata.gx_point_y = ypos;
        scroll_event.gx_event_payload.gx_event_pointdata.gx_point_x = 220;
        gx_system_event_send(&scroll_event);

        scroll_event.gx_event_type = GX_EVENT_PEN_DRAG;
        scroll_event.gx_event_payload.gx_event_pointdata.gx_point_y = ypos - 5;
        gx_system_event_send(&scroll_event);
        GX_EXIT_CRITICAL
        /* Force a screen refresh. */
        gx_validation_screen_refresh();

        /* Increment frame ID. */
        frame_id ++;
    }
   

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();
#endif
    exit(0);
}





