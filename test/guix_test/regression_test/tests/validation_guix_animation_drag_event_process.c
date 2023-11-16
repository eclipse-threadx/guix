/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_system.h"
#include "gx_window.h"
#include "gx_animation.h"

TEST_PARAM test_parameter = {
    "guix_animation_drag_event_process", /* Test name */
    50, 50, 290, 160  /* Define the coordinates of the capture area.
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
#ifdef win32_graphics_driver_setup_24xrgb
#undef win32_graphics_driver_setup_24xrgb
#endif
#define win32_graphics_driver_setup_24xrgb  gx_validation_graphics_driver_setup_24xrgb


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_screen_drag_animation.c"
typedef struct TEST_STRUCT{
int event_type;
int point_x;
int point_y;
int wait_times;
char *comment;
}TEST;

TEST test_list[]={
{GX_EVENT_TIMER,              0,   0, 0, "send GX_EVENT_TIMER event"},
{GX_EVENT_VERTICAL_FLICK,     0,   0, 0, "send GX_EVENT_VERTICAL_FLICK"},
{GX_EVENT_HORIZONTAL_FLICK,   0,   0, 0, "send GX_EVENT_HORIZONTAL_FLICK"},
{GX_EVENT_PEN_UP,           111, 126, 0, "send GX_EVENT_PEN_UP"},
{GX_EVENT_PEN_DRAG,         111, 126, 0, "send GX_EVENT_PEN_DRAG"},
{GX_EVENT_PEN_DOWN,         111, 126, 0, "pen down"},
{GX_EVENT_PEN_DOWN,         111, 126, 0, "pen down"},
{GX_EVENT_PEN_DRAG,         112, 126, 0, "drag right by one pixel"},
{GX_EVENT_PEN_DRAG,         590, 126, 0, "drag right"},
{GX_EVENT_PEN_UP,           590, 126, 50, "pen up"},
{GX_EVENT_PEN_DOWN,         111, 126, 0, "pen down"},
{GX_EVENT_PEN_UP,           111, 126, 0, "pen up"},
{0,                           0,   0, 0, ""}
};

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
INT                 frame_id = 1;
TEST               *test;
GX_EVENT            my_event;

    memset(&my_event, 0, sizeof(GX_EVENT));
  
    my_event.gx_event_display_handle = 1; 

    /* Start animation slide timer. */
    gx_system_timer_start((GX_WIDGET *)&main_screen.main_screen_h_menu_window, GX_ANIMATION_SLIDE_TIMER, 1, 1);

    tx_thread_sleep(3);

    test = test_list;

    while(test->event_type)
    {
        my_event.gx_event_type = test->event_type;
        my_event.gx_event_payload.gx_event_pointdata.gx_point_x = test->point_x;
        my_event.gx_event_payload.gx_event_pointdata.gx_point_y = test->point_y;
        my_event.gx_event_target = (GX_WIDGET *)&main_screen.main_screen_h_menu_window;

        gx_system_event_send(&my_event);

        if(test->wait_times)
        {
            tx_thread_sleep(test->wait_times);
        }
        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(test->comment);
        gx_validation_screen_refresh();
        test++;
    }

    h_screen_drag_animation.gx_animation_original_event_process_function = GX_NULL;
    my_event.gx_event_type = GX_EVENT_PEN_DOWN;
    gx_system_event_send(&my_event);

    tx_thread_sleep(2);

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
