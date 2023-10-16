/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_bidi_text_line_break_static", /* Test name */
    0, 0, 639, 479  /* Define the coordinates of the capture area.
                         In this test, we only need to capture the line
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
#include "demo_guix_bidi_text_line_break_static.c"

static char test_comment[255];
static const char test_string_1[] = "a\ra";
static const char test_string_2[] = "a\r\na";
static const char test_string_3[] = "a\na";
static const char test_string_4[] = "a\r";

char *test_string[]={
    "a\\ra",
    "a\\r\\na",
    "a\\na",
    "a\\r"
};

GX_STRING test_string_list[]={
    {test_string_1, sizeof(test_string_1) - 1},
    {test_string_2, sizeof(test_string_2) - 1},
    {test_string_3, sizeof(test_string_3) - 1},
    {test_string_4, sizeof(test_string_4) - 1}
};

static VOID control_thread_entry(ULONG input)
{
int frame_id = 1;
GX_EVENT my_event;
int index;

    gx_widget_status_add(&main_screen.main_screen_ml_view_Arabic, GX_STATUS_LINE_BREAK_PROCESSED);
    gx_widget_status_add(&main_screen.main_screen_ml_view_English, GX_STATUS_LINE_BREAK_PROCESSED);
    gx_widget_status_add(&main_screen.main_screen_ml_view_Hebrew, GX_STATUS_LINE_BREAK_PROCESSED);

    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Test static bidi text line break.");
    gx_validation_screen_refresh();

    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_display_handle = 1;
    my_event.gx_event_type = GX_EVENT_VERTICAL_SCROLL;
    my_event.gx_event_target = (GX_WIDGET *)&main_screen.main_screen_ml_view_Arabic;
    my_event.gx_event_payload.gx_event_intdata[1] = 0;
    my_event.gx_event_payload.gx_event_intdata[0] = 500;

    for(index = 0; index < 2; index++)
    {
        gx_system_event_send(&my_event);
        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment("scroll down by 500");
        gx_validation_screen_refresh();
    }
    
    for(index = 0; index < sizeof(test_string_list)/sizeof(GX_STRING); index++)
    {
        gx_multi_line_text_view_text_set_ext(&main_screen.main_screen_ml_view_Arabic, &test_string_list[index]);

        gx_validation_set_frame_id(frame_id++);

        sprintf(test_comment, "set string: %s", test_string[index]);
        gx_validation_set_frame_comment(test_comment);
        gx_validation_screen_refresh();
    }

    gx_widget_attach(root, &second_screen);
    gx_widget_hide((GX_WIDGET *)&main_screen);
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Capture second screen.");
    gx_validation_screen_refresh();

    load_theme_2();
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Load theme 2.");
    gx_validation_screen_refresh();

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
