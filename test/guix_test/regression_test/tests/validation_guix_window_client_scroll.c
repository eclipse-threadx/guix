/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_window_client_scroll", /* Test name */
    0, 0, 639, 479  /* Define the coordinates of the capture area.
                         In this test, we only need to capture the scrollbar
                         drawing area.  */
};

static VOID      control_thread_entry(ULONG);
int main(int argc, char ** argv)
{
    /* Parse the command scrollbar argument. */
    gx_validation_setup(argc, argv);

    /* Start ThreadX system */
    tx_kernel_enter(); 
    return(0);
}

VOID tx_application_define(void *first_unused_memory)
{

    /* Create a dedicated thread to perform various operations
       on the scrollbar drawing example. These operations simulate 
       user input. */
    gx_validation_control_thread_create(control_thread_entry);

    /* Termiante the test if it runs for more than 100 ticks */
    /* This function is not implemented yet. */
    gx_validation_watchdog_create(100);

    /* Call the actual scrollbar routine. */
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
int frame_id = 1;
GX_EVENT my_event;
GX_SCROLLBAR vscrollbar[2];
GX_SCROLLBAR hscrollbar;
UINT         status;

    ToggleScreen(pWindowScreen, pButtonScreen);
    memset(&vscrollbar[0], 0, sizeof(GX_SCROLLBAR));
    memset(&vscrollbar[1], 0, sizeof(GX_SCROLLBAR));
    memset(&hscrollbar, 0, sizeof(GX_SCROLLBAR));

    gx_vertical_scrollbar_create(&vscrollbar[0], "test_vscrollbar", &window_screen, GX_NULL, 0);
    status = gx_vertical_scrollbar_create(&vscrollbar[1], "test_vscrollbar", GX_NULL, GX_NULL, 0);
    status |= gx_horizontal_scrollbar_create(&hscrollbar, "test_hscrollbar", GX_NULL, GX_NULL, 0);

    if(status != GX_SUCCESS)
    {
       exit(0);
    }
    gx_widget_detach(&window_screen.window_screen_vertical_scroll_1);
    gx_widget_detach(&window_screen.window_screen_hscroll_1);

    gx_window_wallpaper_set(&window_screen.window_screen_window_4, 1000, GX_TRUE);

    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_display_handle = 1;

    my_event.gx_event_payload.gx_event_intdata[1] = 50;
    my_event.gx_event_payload.gx_event_intdata[0] = 0;
    my_event.gx_event_type = GX_EVENT_HORIZONTAL_SCROLL;
    gx_system_event_send(&my_event);

    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("detach scrollbars, set wallpaper id = 1000, send horizontal scroll event.");
    gx_validation_screen_refresh();
    
    hscrollbar.gx_scrollbar_upleft.gx_widget_size.gx_rectangle_left = 100;
    hscrollbar.gx_scrollbar_upleft.gx_widget_size.gx_rectangle_right = 100;
    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_type = GX_EVENT_PEN_DOWN;
    my_event.gx_event_target = (GX_WIDGET *)&hscrollbar;
    gx_system_event_send(&my_event);

    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("gx_scrollbar_upleft.gx_widget_size.gx_rectangle_left == gx_scrollbar_upleft.gx_widget_size.gx_rectangle_right");
    gx_validation_screen_refresh();
    
    gx_widget_detach(&hscrollbar);
    my_event.gx_event_type = GX_EVENT_HIDE;
    my_event.gx_event_target = (GX_WIDGET *)&hscrollbar;
    gx_system_event_send(&my_event);

    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Send GX_EVENT_HIDE to scrollbar");
    gx_validation_screen_refresh();
    
    gx_widget_detach(&hscrollbar);
    my_event.gx_event_type = GX_EVENT_SHOW;
    my_event.gx_event_target = (GX_WIDGET *)&hscrollbar;
    gx_system_event_send(&my_event);

    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Send GX_EVENT_SHOW to scrollbar");
    gx_validation_screen_refresh();
    
    vscrollbar[0].gx_widget_type = GX_TYPE_VERTICAL_SCROLL;
    gx_widget_style_add((GX_WIDGET *)&vscrollbar, GX_STYLE_TRANSPARENT);
    my_event.gx_event_type = GX_EVENT_SHOW;
    my_event.gx_event_target = (GX_WIDGET *)&vscrollbar;
    gx_system_event_send(&my_event);

    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Send GX_EVENT_SHOW to scrollbar");
    gx_validation_screen_refresh();

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
