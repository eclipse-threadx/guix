/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_system.h"

TEST_PARAM test_parameter = {
    "guix_all_widgets_ml_input_cursor_animation", /* Test name */
    56, 242, 573, 395  /* Define the coordinates of the capture area.
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

#include "demo_guix_all_widgets.c"
GX_CHAR test_comment[256];

static VOID control_thread_entry(ULONG input)
{
int frame_id = 1;
GX_MULTI_LINE_TEXT_INPUT  *ml_input;
GX_CHAR *comment[1];
GX_EVENT my_event;

    ml_input = &text_screen.text_screen_text_input_2;

    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_display_handle = 1;
    
    sprintf(test_comment, "Test cursor animation.");
    comment[0] = test_comment;
    gx_validation_capture_frames(frame_id, 4, comment, 1, 1000);
    gx_multi_line_text_input_style_add(ml_input, GX_STYLE_CURSOR_ALWAYS);

    ToggleScreen((GX_WINDOW *)&text_screen, (GX_WINDOW *)&button_screen);

    gx_validation_capture_frames_wait();

    gx_widget_focus_next(_gx_system_focus_owner);
    gx_widget_focus_next(_gx_system_focus_owner);/* Gain focus. */
    gx_widget_focus_previous(_gx_system_focus_owner);/* Lose focus. */

    gx_multi_line_text_input_style_remove(ml_input, GX_STYLE_CURSOR_ALWAYS);
    gx_widget_focus_next(_gx_system_focus_owner);/* Gain focus. */

    /* Insert a character. */
    my_event.gx_event_type = GX_EVENT_KEY_DOWN;
    my_event.gx_event_payload.gx_event_ushortdata[0] = 0x22;
    gx_system_event_send(&my_event);

    gx_widget_focus_previous(_gx_system_focus_owner);/* Lose focus.*/

    sprintf(test_comment, "Gain focus, lose focus, gain focus, insert a character, remove GX_STYLE_CURSOR_ALWAYS, lose focus");
    gx_validation_current_frame_id_get(&frame_id);
    frame_id++;
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();

    gx_multi_line_text_input_style_remove(ml_input, GX_STYLE_CURSOR_BLINK);
    gx_multi_line_text_input_style_add(ml_input, GX_STYLE_TEXT_INPUT_NOTIFY_ALL);

    gx_widget_focus_next(_gx_system_focus_owner);/* Gain focus. */
    my_event.gx_event_payload.gx_event_ushortdata[0] = 0x23;
    gx_system_event_send(&my_event);

    sprintf(test_comment, "Added GX_STYLE_TEXT_INPUT_NOTIFY_ALL style, and insert a character");
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
