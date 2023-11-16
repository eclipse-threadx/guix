/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_all_widgets_button_repeat", /* Test name */
    111, 90, 490, 150  /* Define the coordinates of the capture area.
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

#include "gx_validation_wrapper.h"

#include "demo_guix_all_widgets.c"
GX_CHAR test_comments[256];

static VOID control_thread_entry(ULONG input)
{
int        frame_id = 1;
GX_EVENT   my_event;
GX_BUTTON *button;

    gx_widget_find((GX_WIDGET *)&button_screen, ID_BUTTON, ID_BUTTON, &button);

    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_target = (GX_WIDGET *)button;

    /* Test button repeat style. */    
    gx_validation_capture_frames(frame_id, 30, GX_NULL, 0, 1000);

    my_event.gx_event_type = GX_EVENT_PEN_DOWN;
    my_event.gx_event_payload.gx_event_pointdata.gx_point_x = button->gx_widget_size.gx_rectangle_left + 5;
    my_event.gx_event_payload.gx_event_pointdata.gx_point_y = button->gx_widget_size.gx_rectangle_top + 5;
    tx_thread_sleep(1);
    gx_system_event_send(&my_event);
    gx_validation_capture_frames_wait();
   
    my_event.gx_event_type = GX_EVENT_PEN_UP;
    gx_system_event_send(&my_event);

    /* Test GX_STYLE_BUTTON_EVENT_ON_PUSH style. */
    gx_widget_style_remove(button, GX_STYLE_BUTTON_REPEAT);
    gx_widget_style_add(button, GX_STYLE_BUTTON_EVENT_ON_PUSH);
    
    gx_validation_current_frame_id_get(&frame_id);
    frame_id++;
    gx_validation_set_frame_id(frame_id);
    sprintf(test_comments, "click on button with GX_STYLE_BUTTON_EVENT_ON_PUSH style");
    gx_validation_set_frame_comment(test_comments);
    my_event.gx_event_type = GX_EVENT_PEN_DOWN;
    gx_system_event_send(&my_event);

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
