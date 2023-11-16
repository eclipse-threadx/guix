/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_system.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_ml_text_input_32bpp", /* Test name */
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
#ifdef win32_graphics_driver_setup_24xrgb  
#undef win32_graphics_driver_setup_24xrgb  
#endif
#define win32_graphics_driver_setup_24xrgb  gx_validation_graphics_driver_setup_24xrgb


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_guix_ml_text_input_32bpp.c"

char test_comment[256];

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int frame_id = 1;
int ypos;
WINDOW_CONTROL_BLOCK *main_screen;
GX_EVENT my_event;
USHORT key_value;
GX_MULTI_LINE_TEXT_INPUT *text_input;

    main_screen = (WINDOW_CONTROL_BLOCK *)pMainScreen;
    text_input = &main_screen->window_ml_text_input;

    /* Remove the cursor blink flag. */
    gx_multi_line_text_input_style_remove(text_input, GX_STYLE_CURSOR_BLINK);
    gx_multi_line_text_input_style_add(text_input, GX_STYLE_CURSOR_ALWAYS);

    /* Set frame id. */
    gx_validation_set_frame_id(frame_id++);
    sprintf(test_comment, "xpos 400 ypos 300 click inside");
    gx_validation_set_frame_comment(test_comment);

    /* Send pen down event to gain input focus. */
    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_display_handle = 1;
    my_event.gx_event_type = GX_EVENT_PEN_DOWN;
    my_event.gx_event_payload.gx_event_pointdata.gx_point_x = 100;
    my_event.gx_event_payload.gx_event_pointdata.gx_point_y = 200;
    gx_system_event_send(&my_event);

    my_event.gx_event_type = GX_EVENT_PEN_UP;
    gx_system_event_send(&my_event);

    for(key_value = 0x21; key_value <= 0x7e; key_value++)
    {      
        /* Inform the validation system 
        (1) Frame ID, which identifies a specific test configuration;
        (2) Start recording frame on the next toggle operation.
        */
        gx_validation_set_frame_id(frame_id++);
        sprintf(test_comment, "Insert character %c", key_value);
        gx_validation_set_frame_comment(test_comment);

        my_event.gx_event_type = GX_EVENT_KEY_DOWN;
        my_event.gx_event_payload.gx_event_ushortdata[0] = key_value;
        gx_system_event_send(&my_event);

        if(key_value & 0x3)
        {
            gx_validation_set_frame_id(frame_id++);
            sprintf(test_comment, "Insert a line break.");
            gx_validation_set_frame_comment(test_comment);

            my_event.gx_event_type = GX_EVENT_KEY_DOWN;
            my_event.gx_event_payload.gx_event_ushortdata[0] = GX_KEY_SELECT;
            gx_system_event_send(&my_event);
        }
    }

    /* multi line text input scroll up. */
    for( ypos = 290; ypos >= 123; ypos -= 10)
    {
        /* Inform the validation system 
        (1) Frame ID, which identifies a specific test configuration;
        (2) Start recording frame on the next toggle operation.
        */
                
        GX_ENTER_CRITICAL
        my_event.gx_event_display_handle = 1;
        my_event.gx_event_type = GX_EVENT_PEN_DOWN;
        my_event.gx_event_payload.gx_event_pointdata.gx_point_y = ypos;
        my_event.gx_event_payload.gx_event_pointdata.gx_point_x = 230;
        gx_system_event_send(&my_event);

        my_event.gx_event_type = GX_EVENT_PEN_DRAG;
        my_event.gx_event_payload.gx_event_pointdata.gx_point_y = ypos - 5;
        gx_system_event_send(&my_event);
        GX_EXIT_CRITICAL

        gx_validation_set_frame_id(frame_id++);

        sprintf(test_comment, "scroll up by 5");
        gx_validation_set_frame_comment(test_comment);
        gx_validation_screen_refresh();
    }

    /* multi line text input scroll down. */
    for(ypos = 150; ypos < 320; ypos += 10)
    {
        /* Inform the validation system 
        (1) Frame ID, which identifies a specific test configuration;
        (2) Start recording frame on the next toggle operation.
        */
        GX_ENTER_CRITICAL
        my_event.gx_event_display_handle = 1;
        my_event.gx_event_type = GX_EVENT_PEN_DOWN;
        my_event.gx_event_payload.gx_event_pointdata.gx_point_y = ypos;
        my_event.gx_event_payload.gx_event_pointdata.gx_point_x = 230;
        gx_system_event_send(&my_event);

        my_event.gx_event_type = GX_EVENT_PEN_DRAG;
        my_event.gx_event_payload.gx_event_pointdata.gx_point_y = ypos + 5;
        gx_system_event_send(&my_event);
        GX_EXIT_CRITICAL
        
        gx_validation_set_frame_id(frame_id++);

        sprintf(test_comment, "scroll down by 5");
        gx_validation_set_frame_comment(test_comment);
        gx_validation_screen_refresh();
    }

    gx_validation_set_frame_id(frame_id++);
    sprintf(test_comment, "cursor height: 20, cursor width: 2");
    gx_validation_set_frame_comment(test_comment);

    gx_text_input_cursor_height_set(&text_input->gx_multi_line_text_input_cursor_instance, 20);
    gx_text_input_cursor_width_set(&text_input->gx_multi_line_text_input_cursor_instance, 2);
    gx_system_dirty_mark((GX_WIDGET *)text_input);
    gx_validation_screen_refresh();

    gx_validation_end();

    exit(0);
}
