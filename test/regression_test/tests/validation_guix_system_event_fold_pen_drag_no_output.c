/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_system.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_system_event_fold_pen_drag_no_output", /* Test name */
    0, 0, 0, 0  /* Define the coordinates of the capture area.*/
};

static VOID      control_thread_entry(ULONG);
int main(int argc, char ** argv)
{
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

#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"
#include "demo_guix_all_widgets.c"

char    test_comment[255];
GX_BOOL guix_sleep = GX_TRUE;

static UINT test_event_process(GX_WINDOW *window, GX_EVENT* event_ptr)
{
    if(guix_sleep)
    {
        guix_sleep = GX_FALSE;
        tx_thread_sleep(1000000);
    }
    return gx_window_event_process(window, event_ptr);
}

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
GX_EVENT           my_event; 
int                index = 0;
GX_BOOL            test_successed = GX_TRUE;

    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_display_handle = 1;
    
    gx_widget_detach(&button_screen);
    gx_widget_attach(root, &window_screen);
    gx_widget_hide((GX_WIDGET *)&nested_root);
    gx_canvas_delete(&animation_canvas);

    gx_widget_event_process_set((GX_WIDGET *)&window_screen, (UINT (*)(GX_WIDGET *, GX_EVENT *))test_event_process);
    my_event.gx_event_type = GX_EVENT_PEN_DOWN;
    my_event.gx_event_target = (GX_WIDGET *)&window_screen;
    gx_system_event_send(&my_event);
    
    while(index < GX_MAX_QUEUE_EVENTS)
    {
        my_event.gx_event_type = GX_EVENT_PEN_DRAG + (index / 2);
        my_event.gx_event_payload.gx_event_pointdata.gx_point_x = 360 + index;
        my_event.gx_event_payload.gx_event_pointdata.gx_point_y = 170 + index;
        gx_system_event_fold(&my_event);
        index++;
    }

    if(_gx_system_event_queue.tx_queue_enqueued != GX_MAX_QUEUE_EVENTS / 2)
    {
        test_successed = GX_FALSE;        
    }

    while(index < GX_MAX_QUEUE_EVENTS * 2 + 1)
    {
        my_event.gx_event_type = GX_EVENT_PEN_DRAG + (index / 2);
        my_event.gx_event_payload.gx_event_pointdata.gx_point_x = 360 + index;
        my_event.gx_event_payload.gx_event_pointdata.gx_point_y = 170 + index;
        gx_system_event_fold(&my_event);
        index++;
    }

    if(_gx_system_event_queue.tx_queue_enqueued != GX_MAX_QUEUE_EVENTS)
    {
        test_successed = GX_FALSE;        
    }

    gx_widget_delete(&button_screen);
    gx_widget_delete(&window_screen);
    if(((GX_EVENT *)(_gx_system_event_queue.tx_queue_read))->gx_event_type != 0)
    {
        test_successed = GX_FALSE;
    }

    if(test_successed) 
    {
        gx_validation_print_test_result(TEST_SUCCESS);
        exit(0);
    }
    else 
    {
        gx_validation_print_test_result(TEST_FAIL);
        exit(1);
    }
}
