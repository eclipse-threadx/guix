/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_vertical_list_event_process", /* Test name */
    464, 93, 613, 332  /* Define the coordinates of the capture area.
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

#include "demo_guix_vertical_list_32bpp.c"

char test_comment[256];

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
INT                 frame_id = 1;
GX_VERTICAL_LIST   *list = &window.window_vertical_list;
GX_WIDGET          *widget = (GX_WIDGET *)list;
GX_RECTANGLE        size;
GX_EVENT            my_event;
INT                 index;

    /* Resize to make children height < widget height. */
    gx_utility_rectangle_define(&size, 464, 93, 613, 332);

    gx_widget_resize(widget, &size);

    memset(&my_event, 0, sizeof(my_event));
    my_event.gx_event_display_handle = 1;
    my_event.gx_event_target = widget;

    for(index = 0; index < 11; index++)
    {
        switch(index)
        {
        case 0:
            my_event.gx_event_type = GX_EVENT_TIMER;
            my_event.gx_event_payload.gx_event_timer_id = 2000;
            gx_system_event_send(&my_event);
            sprintf(test_comment, "send timer event with timer id = 20000");
            break;

        case 1:
            my_event.gx_event_type = GX_EVENT_VERTICAL_FLICK;
            gx_system_event_send(&my_event);
            sprintf(test_comment, "send vertical flick event");
            break;

        case 2:
            my_event.gx_event_type = GX_EVENT_PEN_DRAG;
            gx_system_event_send(&my_event);
            sprintf(test_comment, "send pen drag event");
            break;

        case 3:
            my_event.gx_event_type = GX_EVENT_PEN_DOWN;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_x = size.gx_rectangle_left + 10;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_y = size.gx_rectangle_top + 10;
            gx_system_event_send(&my_event);
            sprintf(test_comment, "send pen down event to capture input");
            break;

        case 4:
            my_event.gx_event_type = GX_EVENT_PEN_DRAG;
            gx_system_event_send(&my_event);
            sprintf(test_comment, "send pen drag event with y_shift = 0");
            break;

        case 5:
            my_event.gx_event_type = GX_EVENT_PEN_DRAG;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_y = size.gx_rectangle_top + 20;
            gx_system_event_send(&my_event);
            sprintf(test_comment, "send pen drag event with y_shift = 20");
            break;

        case 6:
            my_event.gx_event_type = GX_EVENT_PEN_UP;
            gx_system_event_send(&my_event);
            sprintf(test_comment, "send pen up event");
            break;

        case 7:
            my_event.gx_event_type = GX_EVENT_PEN_UP;
            gx_system_event_send(&my_event);
            sprintf(test_comment, "send pen up event");
            break;

        case 8:
            gx_vertical_list_selected_set(list, 0);
            my_event.gx_event_type = GX_EVENT_FOCUS_PREVIOUS;
            gx_system_event_send(&my_event);
            sprintf(test_comment, "select first list item, send GX_EVENT_FOCUS_PREVIOUS event");
            break;

        case 9:
            gx_vertical_list_selected_set(list, list->gx_vertical_list_child_count - 1);
            my_event.gx_event_type = GX_EVENT_FOCUS_NEXT;
            gx_system_event_send(&my_event);
            sprintf(test_comment, "select last list item, send GX_EVENT_FOCUS_NEXT event");
            break;

        case 10:
            gx_widget_detach(widget);
            my_event.gx_event_type = GX_SIGNAL(1000, GX_EVENT_CLICKED);
            my_event.gx_event_sender = 1000;
            gx_system_event_send(&my_event);
            sprintf(test_comment, "detach widget and send an invalid click event");
            break;
          
        }
        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(test_comment);
        gx_validation_screen_refresh();
    }
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
