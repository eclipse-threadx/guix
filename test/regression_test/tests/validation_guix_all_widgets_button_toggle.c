/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_all_widgets_button_toggle", /* Test name */
    130, 250, 255, 320  /* Define the coordinates of the capture area.
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

typedef struct TEST_STRUCT{
    INT  event_type;
    GX_CHAR *comments;
}TEST;

TEST test_list[] = {
    {GX_EVENT_PEN_DOWN, "pen down"},
    {GX_EVENT_PEN_UP, "pen up"},
    {GX_EVENT_PEN_DOWN, "pen down"},
    {GX_EVENT_PEN_UP, "pen up"},
    {0, 0}
};

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
GX_EVENT            my_event;
INT                 frame_id = 1;
GX_PIXELMAP_BUTTON *pButton;
TEST               *test = test_list;

    gx_widget_find(pButtonScreen, ID_PIXELMAP_BUTTON, 0, (GX_WIDGET **) &pButton);

    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_target = (GX_WIDGET *)pButton;

    gx_widget_style_add(pButton, GX_STYLE_BUTTON_TOGGLE);
    
    while(test->event_type)
    {
        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(test->comments);
        my_event.gx_event_type = test->event_type;
        gx_system_event_send(&my_event);
        test++;
    }
    
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
