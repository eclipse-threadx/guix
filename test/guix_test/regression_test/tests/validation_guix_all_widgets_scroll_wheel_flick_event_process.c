/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_all_widgets_scroll_wheel_flick_event_process", /* Test name */
    200, 100, 380, 360  /* Define the coordinates of the capture area.
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
GX_EVENT      my_event;

static void send_drag_event(int direct)
{
    int y;    
    
    if(direct) y = 190;
    else       y = 230;
    
    my_event.gx_event_type = GX_EVENT_PEN_DOWN;
    my_event.gx_event_payload.gx_event_pointdata.gx_point_x = 300;
    my_event.gx_event_payload.gx_event_pointdata.gx_point_y = 210;
    gx_system_event_send(&my_event);
    tx_thread_sleep(10);

    my_event.gx_event_type = GX_EVENT_PEN_DRAG;
    my_event.gx_event_payload.gx_event_pointdata.gx_point_x = 300;
    my_event.gx_event_payload.gx_event_pointdata.gx_point_y = y;
    gx_system_event_send(&my_event);
    
    tx_thread_sleep(10);
}

#define up 1
#define down 0

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
GX_STRING_SCROLL_WHEEL *month_wheel = GX_NULL;
int           frame_id = 1;
GX_CHAR      *comment[1];

    comment[0] = test_comment;
    gx_studio_named_widget_create("scroll_wheel_screen", GX_NULL, (GX_WIDGET **)&pScrollWheelScreen);
    gx_widget_detach(pButtonScreen);
    gx_widget_attach(root, pScrollWheelScreen);
    
    gx_widget_find(pScrollWheelScreen, ID_MONTH_WHEEL, GX_SEARCH_DEPTH_INFINITE, &month_wheel);
    if(!month_wheel) exit(1);
    
    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_display_handle = 1;
    my_event.gx_event_target = (GX_WIDGET *)month_wheel;
    
    send_drag_event(up); 
    my_event.gx_event_type = GX_EVENT_VERTICAL_FLICK;
    my_event.gx_event_payload.gx_event_intdata[0] = 5120;
    sprintf(test_comment, "Send flick event to wheel, intdata[0] of payload data: %d", my_event.gx_event_payload.gx_event_intdata[0]);
    gx_validation_capture_frames(frame_id, 20, comment, 0, 1000);
    gx_system_event_send(&my_event);
    gx_validation_capture_frames_wait(); 
    
    send_drag_event(down); 
    my_event.gx_event_type = GX_EVENT_VERTICAL_FLICK;
    my_event.gx_event_payload.gx_event_intdata[0] = 5120;
    sprintf(test_comment, "Send flick event to wheel, intdata[0] of payload data: %d", my_event.gx_event_payload.gx_event_intdata[0]);
    gx_validation_capture_frames(frame_id, 20, comment, 0, 1000);
    gx_system_event_send(&my_event);
    gx_validation_capture_frames_wait(); 
    
    send_drag_event(up); 
    my_event.gx_event_type = GX_EVENT_VERTICAL_FLICK;
    my_event.gx_event_payload.gx_event_intdata[0] = -5120;
    sprintf(test_comment, "Send flick event to wheel, intdata[0] of payload data: %d", my_event.gx_event_payload.gx_event_intdata[0]);
    gx_validation_capture_frames(frame_id, 20, comment, 0, 1000);
    gx_system_event_send(&my_event);
    gx_validation_capture_frames_wait();  
    
    send_drag_event(down); 
    my_event.gx_event_type = GX_EVENT_VERTICAL_FLICK;
    my_event.gx_event_payload.gx_event_intdata[0] = -5120;
    sprintf(test_comment, "Send flick event to wheel, intdata[0] of payload data: %d", my_event.gx_event_payload.gx_event_intdata[0]);
    gx_validation_capture_frames(frame_id, 20, comment, 0, 1000);
    gx_system_event_send(&my_event);
    gx_validation_capture_frames_wait();     
    
    send_drag_event(up); 
    my_event.gx_event_type = GX_EVENT_VERTICAL_FLICK;
    my_event.gx_event_payload.gx_event_intdata[0] = 8192;
    sprintf(test_comment, "Send flick event to wheel, intdata[0] of payload data: %d", my_event.gx_event_payload.gx_event_intdata[0]);
    gx_validation_capture_frames(frame_id, 20, comment, 0, 1000);
    gx_system_event_send(&my_event);
    gx_validation_capture_frames_wait(); 
    
    send_drag_event(down); 
    my_event.gx_event_type = GX_EVENT_VERTICAL_FLICK;
    my_event.gx_event_payload.gx_event_intdata[0] = 8192;
    sprintf(test_comment, "Send flick event to wheel, intdata[0] of payload data: %d", my_event.gx_event_payload.gx_event_intdata[0]);
    gx_validation_capture_frames(frame_id, 20, comment, 0, 1000);
    gx_system_event_send(&my_event);
    gx_validation_capture_frames_wait(); 

    send_drag_event(up); 
    my_event.gx_event_type = GX_EVENT_VERTICAL_FLICK;
    my_event.gx_event_payload.gx_event_intdata[0] = -8192;
    sprintf(test_comment, "Send flick event to wheel, intdata[0] of payload data: %d", my_event.gx_event_payload.gx_event_intdata[0]);
    gx_validation_capture_frames(frame_id, 20, comment, 0, 1000);
    gx_system_event_send(&my_event);
    gx_validation_capture_frames_wait(); 

    send_drag_event(down); 
    my_event.gx_event_type = GX_EVENT_VERTICAL_FLICK;
    my_event.gx_event_payload.gx_event_intdata[0] = -8192;
    sprintf(test_comment, "Send flick event to wheel, intdata[0] of payload data: %d", my_event.gx_event_payload.gx_event_intdata[0]);
    gx_validation_capture_frames(frame_id, 20, comment, 0, 1000);
    gx_system_event_send(&my_event);
    gx_validation_capture_frames_wait(); 

    
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
