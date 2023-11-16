/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_system.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_pixelmap_slider_event_process", /* Test name */
    150, 124, 201, 282  /* Define the coordinates of the capture area.
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

#include "demo_guix_all_widgets.c"

typedef struct TEST_STRUCT{
   INT event_type;
   GX_VALUE point_x;
   GX_VALUE point_y;
   char *comment;
}TEST;

TEST test_list[]={
   {GX_EVENT_PEN_DOWN, 168, 105, "pen down on (168, 108), not inside slider area"},
   {GX_EVENT_PEN_DRAG, 172, 201, "pen drag on (172, 201), inside slider area"},
   {GX_EVENT_PEN_UP,   172, 201, "pen up on (172, 201), inside slider area"},
   {0, 0, 0, ""}
};
/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{

int frame_id = 1;
GX_EVENT my_event;
TEST *test = test_list;

    ToggleScreen(pIndicatorScreen, pButtonScreen);

    memset(&my_event, 0, sizeof(GX_EVENT));

    my_event.gx_event_display_handle = 1;

    while(test->event_type)
    {  
        my_event.gx_event_type = test->event_type;
        my_event.gx_event_payload.gx_event_pointdata.gx_point_x = test->point_x;
        my_event.gx_event_payload.gx_event_pointdata.gx_point_y = test->point_y;
        gx_system_event_send(&my_event);
  
        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(test->comment);
        gx_validation_screen_refresh();

        test++;
    }

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}





