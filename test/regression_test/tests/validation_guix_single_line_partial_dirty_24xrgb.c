/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_single_line_partial_dirty_24xrgb", /* Test name */
    56, 196, 363, 231  /* Define the coordinates of the capture area.
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

static char input_buffer[] = "single line text input partial dirty mark test abcd";
static USHORT key_value[] = {GX_KEY_DELETE, 'q', GX_KEY_LEFT_ARROW, GX_KEY_HOME};
char test_comment[255];

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
GX_SINGLE_LINE_TEXT_INPUT *text_input_ptr;
GX_EVENT my_event;
int frame_id = 1;
int i, key_index;

    ToggleScreen(pTextScreen, pButtonScreen);
    text_input_ptr = &((TEXT_SCREEN_CONTROL_BLOCK *)pTextScreen) -> text_screen_text_input_1;

    gx_single_line_text_input_style_remove(text_input_ptr, GX_STYLE_CURSOR_BLINK);
    gx_single_line_text_input_style_add(text_input_ptr, GX_STYLE_CURSOR_ALWAYS);

    strcat(text_input_ptr -> gx_single_line_text_input_buffer, input_buffer);
    text_input_ptr -> gx_single_line_text_input_string_size = GX_STRLEN(text_input_ptr -> gx_single_line_text_input_buffer); 

    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_payload.gx_event_pointdata.gx_point_x = 300;
    my_event.gx_event_payload.gx_event_pointdata.gx_point_y = 210;
    my_event.gx_event_display_handle = 1;
    my_event.gx_event_type = GX_EVENT_PEN_DOWN;
    gx_system_event_send(&my_event);

    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_payload.gx_event_ushortdata[0] = GX_KEY_END;
    my_event.gx_event_display_handle = 1;
    my_event.gx_event_type = GX_EVENT_KEY_DOWN;
    gx_system_event_send(&my_event);

    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_payload.gx_event_pointdata.gx_point_x = 280;
    my_event.gx_event_payload.gx_event_pointdata.gx_point_y = 210;
    my_event.gx_event_display_handle = 1;
    my_event.gx_event_type = GX_EVENT_PEN_DOWN;
    gx_system_event_send(&my_event);

    sprintf(test_comment, "click on x: 280, y: 210");
    gx_validation_set_frame_id(frame_id);
    gx_validation_set_frame_comment(test_comment);
    /* Force a screen refresh. */
    gx_validation_screen_refresh();

    frame_id ++;

    for (i = 0; i < 11; i++)
    {
       my_event.gx_event_payload.gx_event_ushortdata[0] = GX_KEY_DELETE;
        my_event.gx_event_display_handle = 1;
        my_event.gx_event_type = GX_EVENT_KEY_DOWN;
        gx_system_event_send(&my_event);
        sprintf(test_comment, "click on x: 280, y: 210, delete key");
        gx_validation_set_frame_id(frame_id);
        gx_validation_set_frame_comment(test_comment);
        /* Force a screen refresh. */
        gx_validation_screen_refresh(); 
        frame_id ++;
    }

    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_payload.gx_event_pointdata.gx_point_x = 56;
    my_event.gx_event_payload.gx_event_pointdata.gx_point_y = 210;
    my_event.gx_event_display_handle = 1;
    my_event.gx_event_type = GX_EVENT_PEN_DOWN;
    gx_system_event_send(&my_event);
    sprintf(test_comment, "click on x: 56, y: 210");
    gx_validation_set_frame_id(frame_id);
    gx_validation_set_frame_comment(test_comment);

    /* Force a screen refresh. */
    gx_validation_screen_refresh();
    frame_id ++;

    /* This is testing the situation where xpos < widget_left and text_end < widget_right.  */
    for (key_index = 0; key_index < (INT)(sizeof(key_value) / sizeof(USHORT)); key_index++)
    {
        sprintf(test_comment, "click on x: 56, y: 210, ");
        switch(key_value[key_index])
        {
        case GX_KEY_HOME:
            strcat(test_comment, "Home key ");
            break;
        case GX_KEY_DELETE:
            strcat(test_comment, "Delete key ");
            break;
        case GX_KEY_LEFT_ARROW:
            strcat(test_comment, "Left Arrow key ");
            break;
        default:
            strcat(test_comment, "insert a character ");
            break;
        }
        memset(&my_event, 0, sizeof(GX_EVENT));
        my_event.gx_event_payload.gx_event_ushortdata[0] = key_value[key_index];
        my_event.gx_event_display_handle = 1;
        my_event.gx_event_type = GX_EVENT_KEY_DOWN;
        gx_system_event_send(&my_event);
        gx_validation_set_frame_id(frame_id);
        gx_validation_set_frame_comment(test_comment);
        /* Force a screen refresh. */
        gx_validation_screen_refresh();

        frame_id ++;
    }

    sprintf(test_comment, "insert a character ");
    for (i =0; i < 3; i ++)
    {
        memset(&my_event, 0, sizeof(GX_EVENT));
        my_event.gx_event_payload.gx_event_ushortdata[0] = 'a';
        my_event.gx_event_display_handle = 1;
        my_event.gx_event_type = GX_EVENT_KEY_DOWN;
        gx_system_event_send(&my_event);
        gx_validation_set_frame_id(frame_id);
        gx_validation_set_frame_comment(test_comment);
        /* Force a screen refresh. */
        gx_validation_screen_refresh();


        frame_id ++;
    }

    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_payload.gx_event_ushortdata[0] = GX_KEY_END;
    my_event.gx_event_display_handle = 1;
    my_event.gx_event_type = GX_EVENT_KEY_DOWN;
    gx_system_event_send(&my_event);
    sprintf(test_comment, "end key");
    gx_validation_set_frame_id(frame_id);
    gx_validation_set_frame_comment(test_comment);
    /* Force a screen refresh. */
    gx_validation_screen_refresh();


    frame_id ++;

    /* This is testing the situation where xpos < widget_left before backspace and xpos = widget_left after backspace.  */
    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_payload.gx_event_ushortdata[0] = GX_KEY_BACKSPACE;
    my_event.gx_event_display_handle = 1;
    my_event.gx_event_type = GX_EVENT_KEY_DOWN;
    gx_system_event_send(&my_event);

    sprintf(test_comment, "backspace key");
    gx_validation_set_frame_id(frame_id);
    gx_validation_set_frame_comment(test_comment);
   /* Force a screen refresh. */
    gx_validation_screen_refresh();


    frame_id ++;

    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_payload.gx_event_ushortdata[0] = 'a';
    my_event.gx_event_display_handle = 1;
    my_event.gx_event_type = GX_EVENT_KEY_DOWN;
    gx_system_event_send(&my_event);
    sprintf(test_comment, "insert a character ");
    gx_validation_set_frame_id(frame_id);
    gx_validation_set_frame_comment(test_comment);
    /* Force a screen refresh. */
    gx_validation_screen_refresh();

 
    frame_id ++;

    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_payload.gx_event_pointdata.gx_point_x = 280;
    my_event.gx_event_payload.gx_event_pointdata.gx_point_y = 210;
    my_event.gx_event_display_handle = 1;
    my_event.gx_event_type = GX_EVENT_PEN_DOWN;
    gx_system_event_send(&my_event);
    sprintf(test_comment, "click on x: 280, y: 210");
    gx_validation_set_frame_id(frame_id);
    gx_validation_set_frame_comment(test_comment);
    /* Force a screen refresh. */
    gx_validation_screen_refresh();

    frame_id ++;

    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_payload.gx_event_ushortdata[0] = GX_KEY_DELETE;
    my_event.gx_event_display_handle = 1;
    my_event.gx_event_type = GX_EVENT_KEY_DOWN;
    gx_system_event_send(&my_event);
 
    sprintf(test_comment, "delete a character ");
    gx_validation_set_frame_id(frame_id);
    gx_validation_set_frame_comment(test_comment);
     /* Force a screen refresh. */
    gx_validation_screen_refresh();

    frame_id ++;

    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_payload.gx_event_ushortdata[0] = GX_KEY_DELETE;
    my_event.gx_event_display_handle = 1;
    my_event.gx_event_type = GX_EVENT_KEY_DOWN;
    gx_system_event_send(&my_event);
    sprintf(test_comment, "delete a character ");
    gx_validation_set_frame_id(frame_id);
    gx_validation_set_frame_comment(test_comment);
    /* Force a screen refresh. */
    gx_validation_screen_refresh();

    frame_id ++;

    /* This is testing the situation where xpos < widget_left and text_width < widget_width.  */
    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_payload.gx_event_ushortdata[0] = GX_KEY_END;
    my_event.gx_event_display_handle = 1;
    my_event.gx_event_type = GX_EVENT_KEY_DOWN;
    gx_system_event_send(&my_event);
    sprintf(test_comment, "end key ");
    gx_validation_set_frame_id(frame_id);
    gx_validation_set_frame_comment(test_comment);
   /* Force a screen refresh. */
    gx_validation_screen_refresh();


    frame_id ++;

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
