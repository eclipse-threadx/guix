/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_horizontal_list_selected_set", /* Test name */
    50, 50, 290, 310  /* Define the coordinates of the capture area.
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

VOID list_scroll(GX_WIDGET *widget, INT shift)
{
GX_EVENT my_event;
     memset(&my_event, 0, sizeof(GX_EVENT));

     my_event.gx_event_display_handle = 1;

     my_event.gx_event_type = GX_EVENT_PEN_DOWN;
     my_event.gx_event_payload.gx_event_pointdata.gx_point_x = widget->gx_widget_size.gx_rectangle_right - 10;
     my_event.gx_event_payload.gx_event_pointdata.gx_point_y = widget->gx_widget_size.gx_rectangle_top + 1;
     gx_system_event_send(&my_event);
     tx_thread_sleep(5);
     
     my_event.gx_event_payload.gx_event_pointdata.gx_point_x = widget->gx_widget_size.gx_rectangle_right + shift;
     my_event.gx_event_type = GX_EVENT_PEN_DRAG;
     gx_system_event_send(&my_event);
     tx_thread_sleep(11);

     my_event.gx_event_type = GX_EVENT_PEN_UP;
     gx_system_event_send(&my_event);
     tx_thread_sleep(5);
}

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int frame_id = 1;
int index;
GX_EVENT   my_event;
GX_HORIZONTAL_LIST *wrap_list;
GX_HORIZONTAL_LIST *scroll_list;
GX_WIDGET *selected;
INT  shift;


    gx_horizontal_list_selected_set(&window.window_horizontal_list, 1);
    gx_horizontal_list_selected_set(&window.window_horizontal_list, -1);

    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_display_handle = 1;
    wrap_list = &window.window_horizontal_list_wrap;
    scroll_list = &window.window_horizontal_list_scroll;

    gx_horizontal_list_selected_set(scroll_list, -1);
    gx_horizontal_list_selected_set(wrap_list, -1);
   
    sprintf(test_comment, "set selected index to -1");
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();

    gx_horizontal_list_selected_set(scroll_list, 4);
    gx_horizontal_list_selected_set(wrap_list, 4);

    sprintf(test_comment, "set selected index to 4");
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();

    /* Scroll up to make item 4 invisible. */
    for(index = 0; index < 2; index++)
    {
        if(index)
        {
            selected = (GX_WIDGET *)scroll_list;
            shift = (scroll_list->gx_horizontal_list_child_width / 2) * 11;
        }
        else
        {
            selected = (GX_WIDGET *)wrap_list;
            shift = (scroll_list->gx_horizontal_list_child_width / 2) * 11;
        }
        list_scroll(selected, -shift);

        sprintf(test_comment, "scroll left by %d", shift);
        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(test_comment);
        gx_validation_screen_refresh();
    }

    gx_horizontal_list_selected_set(scroll_list, 5);
    gx_horizontal_list_selected_set(wrap_list, 5);

    sprintf(test_comment, "set selected index to 5");
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();

    /* Scroll up to make item 4 visible. */
    for(index = 0; index < 2; index++)
    {
        if(index)
        {
            selected = (GX_WIDGET *)scroll_list;
            shift = (scroll_list->gx_horizontal_list_child_width / 2) * 11;
        }
        else
        {
            selected = (GX_WIDGET *)wrap_list;
            shift = (scroll_list->gx_horizontal_list_child_width / 2) * 11;
        }
        list_scroll(selected, shift);
        sprintf(test_comment, "scroll right by %d", shift);
        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(test_comment);
        gx_validation_screen_refresh();
    }

    gx_horizontal_list_total_columns_set(scroll_list, 0);
    gx_horizontal_list_selected_set(scroll_list, -1);
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Set scroll list total columns to 0 and select index -1.");
    gx_validation_screen_refresh();

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
