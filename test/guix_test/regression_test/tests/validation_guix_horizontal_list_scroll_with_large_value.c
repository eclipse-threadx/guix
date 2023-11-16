/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_horizontal_list_scroll_with_large_value", /* Test name */
    54, 54, 289, 303  /* Define the coordinates of the capture area.
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
char *test_comment_array[1];

VOID send_drag_event_to_scroll_list(GX_HORIZONTAL_LIST *list, int distance)
{
GX_EVENT my_event;

    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_display_handle = 1;
    my_event.gx_event_target = (GX_WIDGET *)list;
    my_event.gx_event_payload.gx_event_pointdata.gx_point_x = list->gx_widget_size.gx_rectangle_left + 10;
    my_event.gx_event_payload.gx_event_pointdata.gx_point_y = list->gx_widget_size.gx_rectangle_top + 10;
    my_event.gx_event_type = GX_EVENT_PEN_DOWN;
    gx_system_event_send(&my_event);
    tx_thread_sleep(10);

    my_event.gx_event_payload.gx_event_pointdata.gx_point_x += distance;
    my_event.gx_event_type = GX_EVENT_PEN_DRAG;
    gx_system_event_send(&my_event);
    tx_thread_sleep(10);

    my_event.gx_event_type = GX_EVENT_PEN_UP;
    gx_system_event_send(&my_event);
}

VOID send_scroll_event_to_wrap_list(GX_HORIZONTAL_LIST *list, int distance)
{
GX_EVENT my_event;

    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_display_handle = 1;

    my_event.gx_event_target = (GX_WIDGET *)list;
    my_event.gx_event_payload.gx_event_intdata[1] = distance;
    my_event.gx_event_payload.gx_event_intdata[0] = 0;
    my_event.gx_event_type = GX_EVENT_HORIZONTAL_SCROLL;
    gx_system_event_send(&my_event);
}

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
INT                 frame_id = 1;
GX_HORIZONTAL_LIST *list = &window.window_horizontal_list_scroll;
GX_HORIZONTAL_LIST *wrap_list = &window.window_horizontal_list_wrap;
INT                 index = 0;
GX_BOOL             end_test = GX_FALSE;

    test_comment_array[0] = test_comment;

    while(1)
    {
        switch(index)
        {
        case 0:
            /* Drag list down by 30000.  */
GX_ENTER_CRITICAL
            send_drag_event_to_scroll_list(list, 30000);
            sprintf(test_comment, "Drag scroll list right by 30000");
            gx_validation_capture_frames(frame_id, 23, (char **)test_comment_array, 1, 2000);
GX_EXIT_CRITICAL

            gx_validation_capture_frames_wait();
            gx_validation_current_frame_id_get(&frame_id);
            frame_id++;
            index++;
            continue;

        case 1:
            /* Drag list up by 3000.  */
GX_ENTER_CRITICAL
            send_drag_event_to_scroll_list(list, -30000);
            sprintf(test_comment, "Drag scroll list left by 30000");
            gx_validation_capture_frames(frame_id, 17, (char **)test_comment_array, 1, 2000);
GX_EXIT_CRITICAL

            gx_validation_capture_frames_wait();
            gx_validation_current_frame_id_get(&frame_id);
            frame_id++;
            index++;
            continue;

        case 2:
            /* Scroll wrap list down by 70000.  */
            send_scroll_event_to_wrap_list(wrap_list, 70000);
            sprintf(test_comment, "scroll wrap list right by 70000");
            break;

        case 3:
            /* Scroll wrap list up by 70000.  */
            send_scroll_event_to_wrap_list(wrap_list, -70000);
            sprintf(test_comment, "scroll wrap list left by 70000");
            break;

        default:
            end_test = GX_TRUE;
            break;
        }

        if(end_test)
        {
            break;
        }

        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(test_comment);
        gx_validation_screen_refresh();

        index++;
    }

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
