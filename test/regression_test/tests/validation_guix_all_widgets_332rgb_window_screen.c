/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_all_widgets_332rgb_window_screen", /* Test name */
    0, 0, 640, 480  /* Define the coordinates of the capture area.
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
#ifdef win32_graphics_driver_setup_332rgb  
#undef win32_graphics_driver_setup_332rgb  
#endif
#define win32_graphics_driver_setup_332rgb  gx_validation_graphics_driver_setup_332rgb


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_all_widgets_332rgb.c"

static int scroll_xvalue[] = {470, 590, 570, 590, 330};
static int scroll_yvalue[] = {182, 100, 182, 160, 182};

static int click_xpos[] = {70, 80, 300, 500};
static int click_ypos[] = {280, 350, 300, 320};

char test_comment[256];
/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int index;
int frame_id = 1;
int x = 400, y = 300;
GX_EVENT my_event;

    memset(&my_event, 0, sizeof(GX_EVENT));

    /* Toggle to window screen. */
    ToggleScreen(pWindowScreen, pShapesScreen);

    for(index = 0; index < (INT)(sizeof(scroll_xvalue)/sizeof(int)); index++)
    {
        /* Inform the validation system 
        (1) Frame ID, which identifies a specific test configuration;
        (2) Start recording frame on the next toggle operation.
        */
        /* Send pen down event. */
        my_event.gx_event_payload.gx_event_pointdata.gx_point_x = scroll_xvalue[index];
        my_event.gx_event_payload.gx_event_pointdata.gx_point_y = scroll_yvalue[index];
        my_event.gx_event_type = GX_EVENT_PEN_DOWN; 
        my_event.gx_event_display_handle = 1;
        gx_system_event_send(&my_event);
        
        /* Send pen up event. */
        my_event.gx_event_type = GX_EVENT_PEN_UP;
        gx_system_event_send(&my_event);
   
        /* Set frame id. */
        gx_validation_set_frame_id(frame_id++);

        sprintf(test_comment, "click pos: x=%d, y=%d ", scroll_xvalue[index], scroll_yvalue[index]);

        /* Set frame comments.*/
        gx_validation_set_frame_comment(test_comment);

        /* Force a screen refresh. */
        gx_validation_screen_refresh();
    }

    for(index = 0; index < (INT)(sizeof(click_xpos)/sizeof(int)); index++)
    {
        /* Inform the validation system 
        (1) Frame ID, which identifies a specific test configuration;
        (2) Start recording frame on the next toggle operation.
        */
        
        /* Send pen down event. */
        my_event.gx_event_payload.gx_event_pointdata.gx_point_x = click_xpos[index];
        my_event.gx_event_payload.gx_event_pointdata.gx_point_y = click_ypos[index];
        my_event.gx_event_type = GX_EVENT_PEN_DOWN;
        my_event.gx_event_display_handle = 1;
        gx_system_event_send(&my_event);
        
        /* Send pen up event. */
        my_event.gx_event_type = GX_EVENT_PEN_UP;
        gx_system_event_send(&my_event);
   
        /* Set frame id. */
        gx_validation_set_frame_id(frame_id++);

        sprintf(test_comment, "click pos: x=%d, y=%d", click_xpos[index], click_ypos[index]);

        /* Set frame comments.*/
        gx_validation_set_frame_comment(test_comment);

        /* Force a screen refresh. */
        gx_validation_screen_refresh();
    }

    my_event.gx_event_payload.gx_event_pointdata.gx_point_x = 575;
    my_event.gx_event_payload.gx_event_pointdata.gx_point_y = 180;
    my_event.gx_event_type = GX_EVENT_PEN_DOWN;
    my_event.gx_event_display_handle = 1;
    gx_system_event_send(&my_event);
 
    gx_validation_set_frame_id(frame_id++);
    sprintf(test_comment, "pen down pos: x=70, y=315");
    gx_validation_set_frame_comment(test_comment);    
    gx_validation_screen_refresh();
  
    my_event.gx_event_type = GX_EVENT_PEN_UP;
    gx_system_event_send(&my_event);

    x = 350;
    for(index = 0; index < 3; index++)
    {
        my_event.gx_event_payload.gx_event_pointdata.gx_point_x = x;
        my_event.gx_event_payload.gx_event_pointdata.gx_point_y = y;
        my_event.gx_event_type = GX_EVENT_PEN_DOWN;
        my_event.gx_event_display_handle = 1;
        gx_system_event_send(&my_event);
 
        sprintf(test_comment, "pen down xpos %d ypos %d ", x, 300); 
        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(test_comment);
        gx_validation_screen_refresh();

        my_event.gx_event_payload.gx_event_pointdata.gx_point_y = y;
        my_event.gx_event_type = GX_EVENT_PEN_DRAG;
        gx_system_event_send(&my_event);
        my_event.gx_event_type = GX_EVENT_PEN_UP;
        gx_system_event_send(&my_event);
       
        x += 62;
    }
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
