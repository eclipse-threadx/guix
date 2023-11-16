/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_system.h"

TEST_PARAM test_parameter = {
   "guix_system_screen_stack", /* Test name */
    0 , 0, 240, 320  /* Define the coordinates of the capture area.
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

#include "demo_guix_system_screen_stack.c"

typedef struct TEST_STRUCT{
int xpos;
int ypos;
int steps;
char *comment;
}TEST;

TEST test_list[]={
{151, 135, 20, "click on bookshelf button"},
{112,  76,  1, "click on one book"},
{112,  76,  1, "click on setting button"},
{112,  76,  1, "click on language setting button"},
{179,  13, 10, "click home button"},
{151, 135, 20, "click on bookshelf button"},
{112,  76,  1, "click on one book"},
{112,  76,  1, "click on setting button"},
{112,  76,  1, "click on language setting button"},
{29,   12,  1, "click on back icon"},
{29,   12,  1, "click on back icon"},
{29,   12,  1, "click on back icon"},
{29,   12, 20, "click on back icon"},
{59,  179,  1, "click on setting button"},
{112,  76,  1, "click on language setting button"},
{29,   12,  1, "click on back icon"},
{29,   12,  1, "click on back icon"},
{59,  179,  1, "click on setting button"},
{112,  76,  1, "click on language setting button"},
{179,  13, 10, "click on home button"},
{0,     0,  0, ""}
};

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
GX_EVENT my_event;
int      frame_id = 1;
TEST    *entry = test_list;
char *comment[1];

    memset(&my_event, 0, sizeof(GX_EVENT));

    while(entry->steps)
    {
        gx_validation_current_frame_id_get(&frame_id);
        comment[0] = entry->comment;
GX_ENTER_CRITICAL
        tx_thread_sleep(1);
        gx_validation_capture_frames(frame_id, entry->steps, comment, 1, 1000);
        my_event.gx_event_display_handle = 1;
        my_event.gx_event_payload.gx_event_pointdata.gx_point_x = entry->xpos;
        my_event.gx_event_payload.gx_event_pointdata.gx_point_y = entry->ypos;
        my_event.gx_event_type = GX_EVENT_PEN_DOWN;
        gx_system_event_send(&my_event);
 
        my_event.gx_event_type = GX_EVENT_PEN_UP;
        gx_system_event_send(&my_event);
GX_EXIT_CRITICAL
        gx_validation_capture_frames_wait();
        entry++;
    }

     /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);

}
