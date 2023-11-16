/* This is a small demo of the high-performance GUIX graphics framework. */


#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_system.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_multi_line_text_input_text_select", /* Test name */
    321, 242, 572, 394  /* Define the coordinates of the capture area.
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
    /* Call the actual line example routine. */
    gx_validation_application_define(first_unused_memory);
    
    /* Termiante the test if it runs for more than 100 ticks */
    /* This function is not implemented yet. */
    gx_validation_watchdog_create(100);
    
    
    /* Create a dedicated thread to perform various operations
       on the pixelmap drawing example. These operations simulate 
       user input. */
    gx_validation_control_thread_create(control_thread_entry);

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
GX_CHAR test_string[]=
"0 do you remember the day\r\n we used\r\n to be ABCDEFGHIJKLMN!\r\n@$@*^*\r\n%(*&($#1275\r\n384635098765";

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG uinput)
{
int frame_id = 1;
GX_EVENT my_event;
GX_MULTI_LINE_TEXT_INPUT *input;
    
    input = &text_screen.text_screen_text_input_2;
    gx_multi_line_text_input_text_set(input, test_string);
    gx_multi_line_text_input_style_remove(input, GX_STYLE_CURSOR_BLINK);
    gx_widget_detach(pButtonScreen);
    gx_widget_attach(root, pTextScreen);
    
    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_display_handle = 1;
    my_event.gx_event_type = GX_EVENT_PEN_DOWN;
    my_event.gx_event_target = (GX_WIDGET *)input;
    my_event.gx_event_payload.gx_event_pointdata.gx_point_x = input->gx_widget_size.gx_rectangle_right - 5;
    my_event.gx_event_payload.gx_event_pointdata.gx_point_y =  input->gx_widget_size.gx_rectangle_bottom - 5;
    gx_system_event_send(&my_event);
    
    gx_validation_set_frame_id(frame_id++);
    sprintf(test_comment, "Set text to single line text input and send pen down event.");
    gx_validation_set_frame_comment(test_comment);
    /* Force a screen refresh. */
    gx_validation_screen_refresh(); 
    
    my_event.gx_event_type = GX_EVENT_KEY_DOWN;
    my_event.gx_event_payload.gx_event_ushortdata[0] = GX_KEY_HOME;
    my_event.gx_event_target = (GX_WIDGET *)input;
    gx_system_event_send(&my_event);
    gx_validation_set_frame_id(frame_id++);
    sprintf(test_comment, "Send home key down event to widget.");
    gx_validation_set_frame_comment(test_comment);
    /* Force a screen refresh. */
    gx_validation_screen_refresh();
    
    my_event.gx_event_type = GX_EVENT_MARK_DOWN;
    my_event.gx_event_target = (GX_WIDGET *)input;
    gx_system_event_send(&my_event);
    gx_validation_set_frame_id(frame_id++);
    sprintf(test_comment, "Send mark down event to widget.");
    gx_validation_set_frame_comment(test_comment);
    /* Force a screen refresh. */
    gx_validation_screen_refresh();
    
    gx_multi_line_text_input_text_select(input, 2, 50);
    gx_validation_set_frame_id(frame_id++);
    sprintf(test_comment, "Call function gx_single_line_text_input_text_select.");
    gx_validation_set_frame_comment(test_comment);
    /* Force a screen refresh. */
    gx_validation_screen_refresh();
    
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}





