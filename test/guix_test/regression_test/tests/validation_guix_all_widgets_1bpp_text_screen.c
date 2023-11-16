/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_all_widgets_1bpp_text_screen", /* Test name */
   40 , 30, 580, 440  /* Define the coordinates of the capture area.
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
#ifdef win32_graphics_driver_setup_monochrome  
#undef win32_graphics_driver_setup_monochrome  
#endif
#define win32_graphics_driver_setup_monochrome  gx_validation_graphics_driver_setup_monochrome


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_guix_all_widgets_1bpp.c"
/* Define the xpos values for the mouse. */
static int xpos_value[] = {60, 350};

/* Define the ypos values for the mouse.*/
static int ypos_value[] = {200, 250};

char test_comment[256];

static VOID control_thread_entry(ULONG input)
{

int pos_index;
int frame_id = 1;
GX_EVENT my_event;
USHORT key_value;


    ToggleScreen(pTextScreen, pSpriteScreen);
    gx_multi_line_text_input_style_remove(&(((TEXT_SCREEN_CONTROL_BLOCK *)pTextScreen)->text_screen_text_input_2),GX_STYLE_CURSOR_BLINK);
    gx_single_line_text_input_style_remove(&(((TEXT_SCREEN_CONTROL_BLOCK *)pTextScreen)->text_screen_text_input_1), GX_STYLE_CURSOR_BLINK);

    for(pos_index = 0; pos_index < (INT)(sizeof(xpos_value) / sizeof(int)); pos_index++)
    {
        /* Inform the validation system 
        (1) Frame ID, which identifies a specific test configuration;
        (2) Start recording frame on the next toggle operation.
        */
            
            
        memset(&my_event, 0, sizeof(GX_EVENT));

        my_event.gx_event_payload.gx_event_pointdata.gx_point_x = xpos_value[pos_index];
        my_event.gx_event_payload.gx_event_pointdata.gx_point_y = ypos_value[pos_index];
        my_event.gx_event_type = GX_EVENT_PEN_DOWN;

        my_event.gx_event_display_handle = 1;
        gx_system_event_send(&my_event);
        my_event.gx_event_type = GX_EVENT_PEN_UP;
        gx_system_event_send(&my_event);

        for(key_value = 0x21; key_value <= 0x7e; key_value++)
        {
            my_event.gx_event_payload.gx_event_ushortdata[0] = key_value;

            /* Inform the validation system 
            (1) Frame ID, which identifies a specific test configuration;
            (2) Start recording frame on the next toggle operation.
            */
            gx_validation_set_frame_id(frame_id);
            sprintf(test_comment, "input character %c", key_value);
            gx_validation_set_frame_comment(test_comment);
            my_event.gx_event_type = GX_EVENT_KEY_DOWN;
            gx_system_event_send(&my_event);
            my_event.gx_event_type = GX_EVENT_KEY_UP;
            gx_system_event_send(&my_event);


            /* Increment frame ID */
            frame_id ++;
        }

        gx_validation_set_frame_comment(test_comment);
        gx_validation_set_frame_id(frame_id);

        /* Increment frame ID */
        frame_id ++;
    }


    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}









