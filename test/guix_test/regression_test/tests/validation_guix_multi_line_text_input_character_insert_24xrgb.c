/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_multi_line_text_input_character_insert_24xrgb", /* Test name */
    311, 232, 582, 404  /* Define the coordinates of the capture area.
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

#include "demo_guix_all_widgets.c"

/* Define the nonprintable key value.*/
static USHORT noprint_key_value[] = {GX_KEY_SPACE, GX_KEY_SELECT};

char test_comment[256];

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int noprint_key_value_index;
int frame_id = 1, i;
TEXT_SCREEN_CONTROL_BLOCK *text_screen;
GX_EVENT my_event;
USHORT key_value;

    text_screen = (TEXT_SCREEN_CONTROL_BLOCK *)pTextScreen;

    /* Toggle button_screen to text_screen. */
    ToggleScreen(pTextScreen, pButtonScreen);
    gx_multi_line_text_view_text_color_set((GX_MULTI_LINE_TEXT_VIEW *)&text_screen -> text_screen_text_input_2,
                                           GX_COLOR_ID_ORANGE, GX_COLOR_ID_ORANGE, GX_COLOR_ID_ORANGE);

    /* Send pen down event to the system. */
    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_display_handle = 1;
    my_event.gx_event_type = GX_EVENT_PEN_DOWN;
    my_event.gx_event_payload.gx_event_pointdata.gx_point_x = 400;
    my_event.gx_event_payload.gx_event_pointdata.gx_point_y = 300;
    gx_system_event_send(&my_event);
    gx_validation_set_frame_id(frame_id);
    sprintf(test_comment, "xpos 400 ypos 300 click inside");
    gx_validation_set_frame_comment(test_comment);
    /* Force a screen refresh. */
    gx_validation_screen_refresh();
    frame_id++;

    my_event.gx_event_type = GX_EVENT_KEY_DOWN;

    /* Remove the cursor blink flag. */
    gx_multi_line_text_input_style_remove(&(text_screen->text_screen_text_input_2), GX_STYLE_CURSOR_BLINK);
    gx_multi_line_text_input_style_add(&(text_screen->text_screen_text_input_2), GX_STYLE_CURSOR_ALWAYS);

    for(key_value = 0x21; key_value <= 0x7e; key_value++)
    {
        my_event.gx_event_payload.gx_event_ushortdata[0] = key_value;

        /* Inform the validation system 
        (1) Frame ID, which identifies a specific test configuration;
        (2) Start recording frame on the next toggle operation.
        */
        gx_system_event_send(&my_event);
        gx_validation_set_frame_id(frame_id);
        sprintf(test_comment, "input character %c", key_value);
        gx_validation_set_frame_comment(test_comment);
        /* Force a screen refresh. */
        gx_validation_screen_refresh();

        /* Increment frame ID */
        frame_id ++;
    }

    /* Send pen down event to the system. */
    my_event.gx_event_type = GX_EVENT_PEN_DOWN;
    my_event.gx_event_payload.gx_event_pointdata.gx_point_x = 400;
    my_event.gx_event_payload.gx_event_pointdata.gx_point_y = 300;
    gx_system_event_send(&my_event);

    my_event.gx_event_type = GX_EVENT_KEY_DOWN;
    for(i = 0; i < 3; i++)
    {
        for(noprint_key_value_index = 0; noprint_key_value_index < (INT)(sizeof(noprint_key_value)/sizeof(USHORT)); noprint_key_value_index++)
        {
            my_event.gx_event_payload.gx_event_ushortdata[0] = noprint_key_value[noprint_key_value_index];
            switch(noprint_key_value[noprint_key_value_index])
            {
            case GX_KEY_SPACE:
                sprintf(test_comment, "press space key");
                if(i == 0)
                {
                    strcat(test_comment, " in xpos 400 ypos 300");
                }
                break;
            case GX_KEY_SELECT:
                sprintf(test_comment, "press return key");
                break;
            }

            gx_system_event_send(&my_event);
            gx_validation_set_frame_id(frame_id);
            gx_validation_set_frame_comment(test_comment);
            /* Force a screen refresh. */
            gx_validation_screen_refresh();


            /* Increment frame ID */
            frame_id ++;
        }
    }

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
