/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_drop_list_open", /* Test name */
    173, 288, 441, 460  /* Define the coordinates of the capture area.
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

char test_comment[256];

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int               frame_id = 1;
GX_EVENT          my_event;
GX_DROP_LIST     *drop_list;
GX_VERTICAL_LIST *list;

    ToggleScreen(pWindowScreen, pButtonScreen);
    gx_widget_find((GX_WIDGET *) pWindowScreen, ID_DROP_LIST, GX_SEARCH_DEPTH_INFINITE, (GX_WIDGET **) &drop_list);
    list = &drop_list->gx_drop_list_popup.gx_popup_list_list;


    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_type = GX_SIGNAL(ID_DROP_LIST_BUTTON, GX_EVENT_CLICKED);
    my_event.gx_event_target = (GX_WIDGET *)drop_list;
    gx_system_event_send(&my_event);

   
    gx_vertical_list_total_rows_set(list, 0);
    gx_system_event_send(&my_event);
    gx_system_event_send(&my_event);

    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("set popup list total rows = 0, send click event");
    gx_validation_screen_refresh();

    list -> gx_vertical_list_pen_index = -1;
    my_event.gx_event_type = GX_EVENT_PEN_UP;
    my_event.gx_event_target = (GX_WIDGET *)&(drop_list -> gx_drop_list_popup);
    gx_system_event_send(&my_event);

    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Send PEN_UP event to popup list");
    gx_validation_screen_refresh();

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
