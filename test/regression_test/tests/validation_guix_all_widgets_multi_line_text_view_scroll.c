/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_all_widgets_multi_line_text_view_scroll", /* Test name */
    39, 25, 582, 443  /* Define the coordinates of the capture area.
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

GX_CHAR text[] = "every year when the autumn comes, there will be a sports meeting in my school. I am so impressed by the opening ceremony. Every class will march on the ground and show their features, which is very funny. Last year, a group of students wore the co-splay dresses. When they showed up, they caught all people’s attention and won the biggest applause. It is a great event for every student, and also a good chance to let students develop their creativity. This year, my class decided to make a big surprise. Everyone has provided their idea and I love the atmosphere. I am so looking forward to the opening ceremony.every year when the autumn comes, there will be a sports meeting in my school. I am so impressed by the opening ceremony. Every class will march on the ground and show their features, which is very funny. Last year, a group of students wore the co-splay dresses. When they showed up, they caught all people’s attention and won the biggest applause. It is a great event for every student, and also a good chance to let students develop their creativity. This year, my class decided to make a big surprise. Everyone has provided their idea and I love the atmosphere. I am so looking forward to the opening ceremony.every year when the autumn comes, there will be a sports meeting in my school. I am so impressed by the opening ceremony. Every class will march on the ground and show their features, which is very funny. Last year, a group of students wore the co-splay dresses. When they showed up, they caught all people’s attention and won the biggest applause. It is a great event for every student, and also a good chance to let students develop their creativity. This year, my class decided to make a big surprise. Everyone has provided their idea and I love the atmosphere. I am so looking forward to the opening ceremony.";

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int                        frame_id = 1;
GX_MULTI_LINE_TEXT_VIEW   *ml_text_view;
GX_EVENT                   my_event;

    gx_widget_detach(pButtonScreen);
    gx_widget_attach(root, pTextScreen);
    ml_text_view = &text_screen.text_screen_text_view_1;
    
    gx_multi_line_text_view_text_set(ml_text_view, text);
    
    sprintf(test_comment, "Set text to ml_text_view.");
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();

    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_target = (GX_WIDGET *)ml_text_view;
    my_event.gx_event_type = GX_EVENT_VERTICAL_SCROLL;
    my_event.gx_event_display_handle = 1;
    my_event.gx_event_payload.gx_event_intdata[0] = 0;
    my_event.gx_event_payload.gx_event_intdata[1] = 0;
    gx_system_event_send(&my_event);
    
    sprintf(test_comment, "Set scroll event to text view, shift value: 0.");
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();
    
    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_target = (GX_WIDGET *)ml_text_view;
    my_event.gx_event_type = GX_EVENT_VERTICAL_SCROLL;
    my_event.gx_event_display_handle = 1;
    my_event.gx_event_payload.gx_event_intdata[0] = 0;
    my_event.gx_event_payload.gx_event_intdata[1] = 200;
    gx_system_event_send(&my_event);
    
    sprintf(test_comment, "Set scroll event to text view, shift value: 200.");
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();
    
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();
    
    exit(0);
}
