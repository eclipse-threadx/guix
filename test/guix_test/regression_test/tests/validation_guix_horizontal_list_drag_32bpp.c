/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_horizontal_list_drag_32bpp", /* Test name */
   50, 50, 290, 310  /* Define the coordinates of the capture area.
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

#include "demo_guix_horizontal_list_32bpp.c"

char test_comment[256];

#define XPOS_1 200
#define YPOS_1 120
#define XPOS_2 200
#define YPOS_2 250

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int horizontal_index;
int frame_id = 1;
GX_EVENT my_event;
INT      shift;

    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_display_handle = 1;
 
    gx_horizontal_list_page_index_set(&window.window_horizontal_list_wrap, 19);
    sprintf(test_comment, "Set page index to 19");
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();
 
    for(horizontal_index = 0; horizontal_index < 56; horizontal_index++)
    {   
        if(horizontal_index < 27)
        {
            shift = -20;
            sprintf(test_comment, "scroll left by 20");
        }
        else
        {
            shift = 20;
            sprintf(test_comment, "scroll right by 20");
        }

        my_event.gx_event_type = GX_EVENT_PEN_DOWN;
        my_event.gx_event_payload.gx_event_pointdata.gx_point_x = XPOS_1;
        my_event.gx_event_payload.gx_event_pointdata.gx_point_y = YPOS_1;
        gx_system_event_send(&my_event);
        tx_thread_sleep(5);

        my_event.gx_event_payload.gx_event_pointdata.gx_point_x = XPOS_1 + shift;
        my_event.gx_event_type = GX_EVENT_PEN_DRAG;
        gx_system_event_send(&my_event);
        tx_thread_sleep(11);

        my_event.gx_event_type = GX_EVENT_PEN_UP;
        gx_system_event_send(&my_event);
        tx_thread_sleep(5);

        my_event.gx_event_type = GX_EVENT_PEN_DOWN;
        my_event.gx_event_payload.gx_event_pointdata.gx_point_x = XPOS_2;
        my_event.gx_event_payload.gx_event_pointdata.gx_point_y = YPOS_2;
        gx_system_event_send(&my_event);
        tx_thread_sleep(5);

        my_event.gx_event_payload.gx_event_pointdata.gx_point_x = XPOS_2 + shift;
        my_event.gx_event_type = GX_EVENT_PEN_DRAG;
        tx_thread_sleep(11);
        gx_system_event_send(&my_event); 
        my_event.gx_event_type = GX_EVENT_PEN_UP;
        gx_system_event_send(&my_event);
        tx_thread_sleep(5);

        gx_validation_set_frame_id(frame_id);

        gx_validation_set_frame_comment(test_comment);
        
        /* Force a screen refresh. */
        gx_validation_screen_refresh();
        
        /* Increment frame ID */
        frame_id ++;
    }

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
