/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_window.h"

TEST_PARAM test_parameter = {
    "guix_vertical_list_wrap", /* Test name */
    273, 85, 425, 330  /* Define the coordinates of the capture area.
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
INT               frame_id = 1;
GX_EVENT          my_event;
GX_VERTICAL_LIST *list = &window.window_vertical_list_wrap;
GX_VERTICAL_LIST *scroll_list = &window.window_vertical_list_scroll;
GX_SCROLLBAR     *scroll = &window.window_vertical_scroll;
int               index;

    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_target = (GX_WIDGET *)list;

    gx_vertical_list_total_rows_set(list, 0);
    list->gx_vertical_list_total_rows = 20;
 
    for(index = 0; index < 2; index++)
    {
        switch(index)
        {
        case 0:
           my_event.gx_event_payload.gx_event_intdata[1] = 50;
           my_event.gx_event_payload.gx_event_intdata[0] = 0;
           my_event.gx_event_type = GX_EVENT_VERTICAL_SCROLL;
           gx_system_event_send(&my_event);
           sprintf(test_comment, "total rows = 20, child widgets count = 0, set scroll event to wrap up");
           break;
 
        case 1:
           my_event.gx_event_payload.gx_event_intdata[1] = 0;
           my_event.gx_event_payload.gx_event_intdata[0] = 50;
           my_event.gx_event_type = GX_EVENT_VERTICAL_SCROLL;
           gx_system_event_send(&my_event);
           sprintf(test_comment, "total rows = 20, child widgets count = 0, set scroll event to wrap down");
           break;
        }
        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(test_comment);
        gx_validation_screen_refresh();
    }

    /* Test _gx_vertical_list_total_rows_set. */
    gx_widget_hide(list);
    gx_vertical_list_total_rows_set(list, 0);

    list->gx_vertical_list_child_count = 2;
    gx_vertical_list_total_rows_set(list, 0);

    list->gx_vertical_list_top_index = 1;    
    gx_vertical_list_total_rows_set(list, 7);

    /* Test _gx_vertical_list_slide_back_check. */
    list->gx_vertical_list_child_count = 0;
    _gx_vertical_list_slide_back_check(list);

    list->gx_vertical_list_child_count = 1;
    list->gx_vertical_list_top_index = 10;
    _gx_vertical_list_slide_back_check(list);

    list->gx_vertical_list_top_index = 0;
    _gx_vertical_list_slide_back_check(list);


    /* Test _gx_vertical_list_scroll_info_get. */    
    gx_vertical_list_total_rows_set(scroll_list, 0);
    scroll_list->gx_vertical_list_total_rows = 20;
    gx_scrollbar_reset(scroll, GX_NULL);
    
    scroll_list->gx_vertical_list_top_index = -1;
    gx_scrollbar_reset(scroll, GX_NULL);

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
