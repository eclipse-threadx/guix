/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_all_widgets_drop_list_event_process", /* Test name */
    180, 310, 430, 460  /* Define the coordinates of the capture area.
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

char test_comment[255];

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
GX_DROP_LIST *drop_list;
int           frame_id = 1;
GX_EVENT      my_event;

    drop_list = &window_screen.window_screen_drop_list;
    gx_widget_detach(pButtonScreen);
    gx_widget_attach(root, pWindowScreen);
    gx_drop_list_open(drop_list);
    gx_drop_list_pixelmap_set(drop_list, GX_PIXELMAP_ID_FISH);
    
    sprintf(test_comment, "Set pixelmap fish to drop list.");
    gx_system_dirty_mark(drop_list);
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();
    
    gx_widget_hide(drop_list);
    sprintf(test_comment, "Hide drop list.");
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();

    gx_widget_show(drop_list);
    sprintf(test_comment, "Show drop list.");
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();
    
    gx_drop_list_open(drop_list);
    sprintf(test_comment, "Open drop list.");
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();    
    
    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_display_handle = 1;
    my_event.gx_event_target = (GX_WIDGET *)drop_list;
    my_event.gx_event_type = GX_EVENT_CLOSE_POPUP;
    gx_system_event_send(&my_event);
    
    sprintf(test_comment, "Send close popup event to drop list.");
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();   

    my_event.gx_event_type =  GX_SIGNAL(ID_DROP_LIST_BUTTON, GX_EVENT_CLICKED);
    gx_system_event_send(&my_event);
    
    sprintf(test_comment, "Send child button clicked event to drop list.");
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();   
    
    my_event.gx_event_type =  GX_SIGNAL(ID_DROP_LIST_BUTTON, GX_EVENT_CLICKED);
    gx_system_event_send(&my_event);
    
    sprintf(test_comment, "Send child button clicked event to drop list again.");
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh(); 
    
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
