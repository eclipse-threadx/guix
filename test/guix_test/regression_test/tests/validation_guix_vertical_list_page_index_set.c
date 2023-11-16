/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_vertical_list_page_index_set", /* Test name */
    70, 85, 430, 330  /* Define the coordinates of the capture area.
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

int index_list[]={18,26,1,10,0,5,7,0,15,25,19};

char test_comment[256];

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int frame_id = 1;
int index;
int page_index;
GX_EVENT my_event;
GX_WIDGET *selected;
GX_VERTICAL_LIST *wrap_list;
GX_VERTICAL_LIST *scroll_list;

    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_display_handle = 1;

    wrap_list = &window.window_vertical_list_wrap;
    scroll_list = &window.window_vertical_list_scroll;

    for(index = 0; index < (int)(sizeof(index_list)/sizeof(int)); index++)
    {
        page_index = index_list[index];
#if defined(GX_DISABLE_ERROR_CHECKING)
        if((page_index >= 0) && (page_index < list_total_rows))
        {
#endif
            gx_vertical_list_page_index_set(&window.window_vertical_list_scroll, page_index);
            gx_vertical_list_page_index_set(&window.window_vertical_list_wrap, page_index);
#if defined(GX_DISABLE_ERROR_CHECKING)
        }
#endif
        sprintf(test_comment, "set page index to %d", page_index);
        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(test_comment);
        gx_validation_screen_refresh();

        if(page_index == 19)
        {
            gx_vertical_list_selected_set(wrap_list, 1);
            gx_vertical_list_selected_widget_get(wrap_list, &selected);
            gx_vertical_list_selected_set(wrap_list, 0);
        
            my_event.gx_event_type = GX_EVENT_PEN_DOWN;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_x = selected->gx_widget_size.gx_rectangle_left + 1;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_y = selected->gx_widget_size.gx_rectangle_top + 1;
            gx_system_event_send(&my_event);
            tx_thread_sleep(5);
            
            my_event.gx_event_type = GX_EVENT_PEN_UP;
            gx_system_event_send(&my_event);
            tx_thread_sleep(5);
       
            sprintf(test_comment, "clicked to select wrap list index 1");                
            gx_validation_set_frame_id(frame_id++);
            gx_validation_set_frame_comment(test_comment);
            gx_validation_screen_refresh();

            gx_vertical_list_selected_set(scroll_list, 1);
            gx_vertical_list_selected_widget_get(scroll_list, &selected);
            gx_vertical_list_selected_set(scroll_list, 0);

            my_event.gx_event_type = GX_EVENT_PEN_DOWN;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_x = selected->gx_widget_size.gx_rectangle_left + 1;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_y = selected->gx_widget_size.gx_rectangle_top + 1;
            gx_system_event_send(&my_event);
            tx_thread_sleep(5);
            
            my_event.gx_event_type = GX_EVENT_PEN_UP;
            gx_system_event_send(&my_event);
            tx_thread_sleep(5);

            sprintf(test_comment, "clicked to select scroll list index 1");                
            gx_validation_set_frame_id(frame_id++);
            gx_validation_set_frame_comment(test_comment);
            gx_validation_screen_refresh();
        }
    }

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
