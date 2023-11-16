/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_system.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_horizontal_list_event_process_pen_up", /* Test name */
    180, 217, 337, 280  /* Define the coordinates of the capture area.
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
    /* Call the actual line example routine. */
    gx_validation_application_define(first_unused_memory);
    /* Termiante the test if it runs for more than 100 ticks */
    /* This function is not implemented yet. */
    gx_validation_watchdog_create(100);

    /* Create a dedicated thread to perform various operations
       on the line drawing example. These operations simulate 
       user input. */
    gx_validation_control_thread_create(control_thread_entry);

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
GX_EVENT           my_event; 
int                frame_id = 1;
GX_HORIZONTAL_LIST *hlist;

    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_display_handle = 1;
    hlist = &window_screen.window_screen_horizontal_list_1;
    gx_widget_detach(&button_screen);
    gx_widget_attach(root, &window_screen);
    gx_widget_hide((GX_WIDGET *)&nested_root);
    gx_canvas_delete(&animation_canvas);

    my_event.gx_event_type = GX_EVENT_PEN_DOWN;
    my_event.gx_event_payload.gx_event_pointdata.gx_point_x = hlist->gx_widget_size.gx_rectangle_left + 10;
    my_event.gx_event_payload.gx_event_pointdata.gx_point_y = hlist->gx_widget_size.gx_rectangle_top + 10;
    gx_system_event_send(&my_event);
    
    gx_validation_set_frame_id(frame_id++);
    sprintf(test_comment, "Send pen down event. pen pos (%d, %d)",my_event.gx_event_payload.gx_event_pointdata.gx_point_x, my_event.gx_event_payload.gx_event_pointdata.gx_point_y);
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();

    my_event.gx_event_type = GX_EVENT_PEN_UP;
    my_event.gx_event_payload.gx_event_pointdata.gx_point_x = hlist->gx_widget_size.gx_rectangle_left + 10;
    my_event.gx_event_payload.gx_event_pointdata.gx_point_y = hlist->gx_widget_size.gx_rectangle_top + 10;
    gx_system_event_send(&my_event);

    gx_validation_set_frame_id(frame_id++);
    sprintf(test_comment, "Send pen up event. pen pos (%d, %d)",my_event.gx_event_payload.gx_event_pointdata.gx_point_x, my_event.gx_event_payload.gx_event_pointdata.gx_point_y);
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();
    
    my_event.gx_event_type = GX_EVENT_PEN_DOWN;
    my_event.gx_event_payload.gx_event_pointdata.gx_point_x = hlist->gx_widget_size.gx_rectangle_left - 10;
    my_event.gx_event_payload.gx_event_pointdata.gx_point_y = hlist->gx_widget_size.gx_rectangle_top + 10;
    gx_system_event_send(&my_event);
    
    gx_validation_set_frame_id(frame_id++);
    sprintf(test_comment, "Send pen down event. pen pos (%d, %d)",my_event.gx_event_payload.gx_event_pointdata.gx_point_x, my_event.gx_event_payload.gx_event_pointdata.gx_point_y);
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();

    my_event.gx_event_type = GX_EVENT_PEN_UP;
    my_event.gx_event_payload.gx_event_pointdata.gx_point_x = hlist->gx_widget_size.gx_rectangle_left + 10;
    my_event.gx_event_payload.gx_event_pointdata.gx_point_y = hlist->gx_widget_size.gx_rectangle_top + 10;
    gx_system_event_send(&my_event);

    gx_validation_set_frame_id(frame_id++);
    sprintf(test_comment, "Send pen up event. pen pos (%d, %d)",my_event.gx_event_payload.gx_event_pointdata.gx_point_x, my_event.gx_event_payload.gx_event_pointdata.gx_point_y);
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();
    exit(0);
}
