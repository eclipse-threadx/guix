/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_system.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_ml_text_input_left_arrow", /* Test name */
    30, 120, 245, 330  /* Define the coordinates of the capture area.
                         In this test, we only need to capture the multi 
                         line text input drawing area.  */
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
       on the multi line text input example. These operations simulate 
       user input. */
    gx_validation_control_thread_create(control_thread_entry);

    /* Terminate the test if it runs for more than 100 ticks */
    /* This function is not implemented yet. */
    gx_validation_watchdog_create(100);

    /* Call the actual multi line text input routine. */
    gx_validation_application_define(first_unused_memory);

}


/* Replace the default graphics driver with the validation driver. */
#ifdef win32_graphics_driver_setup_565rgb  
#undef win32_graphics_driver_setup_565rgb  
#endif
#define win32_graphics_driver_setup_565rgb  gx_validation_graphics_driver_setup_565rgb


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_guix_ml_text_input_16bpp.c"


char test_comment[256];
GX_UBYTE test_string[]={
 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0xe4, 0xb8, 0x8d, 0xe8, 0xa6, 0x81, 0xe7, 0xa5, 0x9e, 0xe7, 0x9a, 0x84, 0xe5, 0x85, 0x89, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x00};

GX_UBYTE test_string_1[]={
 0x80, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0xe4, 0xb8, 0x8d, 0xe8, 0xa6, 0x81, 0xe7, 0xa5, 0x9e, 0xe7, 0x9a, 0x84, 0xe5, 0x85, 0x89, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x00};

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int                       frame_id = 1;
GX_MULTI_LINE_TEXT_INPUT *text_input;
GX_EVENT                  my_event;
int                       loop;
int                       index;
GX_UBYTE                 *string;

    text_input = &((WINDOW_CONTROL_BLOCK *)pMainScreen)->window_ml_text_input;
    memset(&my_event, 0, sizeof(GX_EVENT));

    /* Remove the cursor blink flag. */
    gx_multi_line_text_input_style_remove(text_input, GX_STYLE_CURSOR_BLINK);
    gx_multi_line_text_input_style_add(text_input, GX_STYLE_CURSOR_ALWAYS);

    gx_multi_line_text_view_font_set((GX_MULTI_LINE_TEXT_VIEW *)text_input, GX_FONT_ID_CHINESE);

    for(index = 0; index < 2; index++)
    {
        switch(index)
        {
        case 0:
            string = test_string;
            sprintf(test_comment, "set test string");
            break;

        case 1:
            string = test_string_1;
            sprintf(test_comment, "set test string that contain invalid utf8 code");
            break;
        }

        gx_multi_line_text_input_text_set(text_input, (GX_CONST GX_CHAR *)string);

        gx_validation_set_frame_id(frame_id++);
        strcat(test_comment, ", click to gain input focus.");
        gx_validation_set_frame_comment(test_comment);

        /* Send pen down event to gain input focus. */
        my_event.gx_event_display_handle = 1;
        my_event.gx_event_type = GX_EVENT_PEN_DOWN;
        my_event.gx_event_payload.gx_event_pointdata.gx_point_x = 36;
        my_event.gx_event_payload.gx_event_pointdata.gx_point_y = 150;
        gx_system_event_send(&my_event);

        for(loop = 0; loop < 15; loop ++)
        {
            my_event.gx_event_type = GX_EVENT_KEY_DOWN;
            my_event.gx_event_payload.gx_event_ushortdata[0] = GX_KEY_LEFT_ARROW;
            gx_system_event_send(&my_event);
        
            gx_validation_set_frame_id(frame_id++);
            sprintf(test_comment, "Left key down");
            gx_validation_set_frame_comment(test_comment);
            gx_validation_screen_refresh();
        }
    }

    gx_validation_end();

    exit(0);
}
