/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_system.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_ml_text_view_cache_32bpp", /* Test name */
    20, 101, 264, 424  /* Define the coordinates of the capture area.
                         In this test, we only need to capture the multi 
                         line text view drawing area.  */
};

static VOID      control_thread_entry(ULONG);
int main(int argc, char ** argv)
{
#ifdef GX_UTF8_SUPPORT
    /* Parse the command line argument. */
    gx_validation_setup(argc, argv);
#else
    printf("guix_ml_text_view_32bpp: Skip Testing!\n");
#endif
    /* Start ThreadX system */
    tx_kernel_enter(); 
    return(0);
}

VOID tx_application_define(void *first_unused_memory)
{

    /* Create a dedicated thread to perform various operations
       on the multi line text view example. These operations simulate 
       user input. */
    gx_validation_control_thread_create(control_thread_entry);

    /* Terminate the test if it runs for more than 100 ticks */
    /* This function is not implemented yet. */
    gx_validation_watchdog_create(100);

    /* Call the actual multi line text view routine. */
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

#include "demo_guix_ml_text_view_32bpp.c"

char test_comment[256];

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int frame_id = 1;
GX_EVENT scroll_event;
GX_MULTI_LINE_TEXT_VIEW *view;
GX_STRING string;
GX_CHAR string_buffer[255];
int index;

    memset(&scroll_event, 0, sizeof(GX_EVENT));

    view = &pMainScreen->window_text_view;

    gx_multi_line_text_view_text_id_set(view, GX_STRING_ID_STRING_WITH_35_LINES);
    gx_validation_set_frame_id(frame_id++);
    sprintf(test_comment, "Set multi line text view text id(36 lines text).");
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();

    gx_widget_border_style_set(view, GX_STYLE_BORDER_THIN);
    gx_validation_set_frame_id(frame_id++);
    sprintf(test_comment, "set thin border");
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();

    scroll_event.gx_event_display_handle = 1;
    scroll_event.gx_event_type = GX_EVENT_VERTICAL_SCROLL;
    scroll_event.gx_event_target = (GX_WIDGET *)view;
    scroll_event.gx_event_payload.gx_event_intdata[1] = 10;
    scroll_event.gx_event_payload.gx_event_intdata[0] = 0;
    gx_system_event_send(&scroll_event);
    gx_validation_set_frame_id(frame_id++);
    sprintf(test_comment, "scroll up by 10");
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();

    scroll_event.gx_event_payload.gx_event_intdata[1] = 0;
    scroll_event.gx_event_payload.gx_event_intdata[0] = 500;
    gx_system_event_send(&scroll_event);
    sprintf(test_comment, "scroll down by 500");
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();

    /* Create a string that has 200 lines. */
    for(index = 0; index < 200; index += 2)
    {
        string_buffer[index] = 't';
        string_buffer[index + 1] = '\r';
    }
    string_buffer[index] = '\0';
    string.gx_string_ptr = string_buffer;
    string.gx_string_length = index;

    /* Set multi line text view text for testing. */
    gx_multi_line_text_view_text_set_ext(view, &string);
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("set string that has 101 lines");
    gx_validation_screen_refresh();

    scroll_event.gx_event_payload.gx_event_intdata[1] = 0;
    scroll_event.gx_event_payload.gx_event_intdata[0] = 1000;
    gx_system_event_send(&scroll_event);
    sprintf(test_comment, "scroll down by 1000");
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();
 
    gx_multi_line_text_view_line_space_set(view, -22);
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("set line height to -22");
    gx_validation_screen_refresh();
  
    gx_widget_hide((GX_WIDGET *)view);
    gx_multi_line_text_view_line_space_set(view, 0);
    gx_multi_line_text_view_text_id_set(view, GX_STRING_ID_STRING_ASCII);
    gx_widget_show((GX_WIDGET *)view);

    sprintf(test_comment, "hide widget, set line space 0, set text id, show widget");
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
