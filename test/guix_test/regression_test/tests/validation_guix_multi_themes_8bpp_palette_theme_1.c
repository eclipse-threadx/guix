/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_multi_themes_8bpp_palette_theme_1", /* Test name */
    5, 5, 630, 470  /* Define the coordinates of the capture area.
                         In this test, we only need to capture the line
                         drawing area.  */
};

static VOID      control_thread_entry(ULONG);
int main(int argc, char ** argv)
{
#ifdef GX_UTF8_SUPPORT
    /* Parse the command line argument. */
    gx_validation_setup(argc, argv);
#else
    printf("guix_multi_themes_8bpp_palette:Skip Testing!\n");
#endif
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
#ifdef win32_graphics_driver_setup_8bit_palette 
#undef win32_graphics_driver_setup_8bit_palette
#endif
#define win32_graphics_driver_setup_8bit_palette  gx_validation_graphics_driver_setup_8bit_palette


#ifdef WIN32
#undef WIN32
#endif

#define REGRESSION_TEST_MODE

#include "gx_validation_wrapper.h"

#include "demo_guix_multi_themes_8bpp_palette.c"

char test_comment[256];

extern SIMPLE_WINDOW_CONTROL_BLOCK simple_window;

static VOID control_thread_entry(ULONG input)
{
#ifdef GX_UTF8_SUPPORT
int      frame_id = 1;
int      language_id;
GX_EVENT select_event;
GX_CONST GX_THEME *theme_ptr;

    theme_ptr = main_display_theme_table[MAIN_DISPLAY_THEME_1];
    gx_validation_write_palette(theme_ptr -> theme_palette, theme_ptr->theme_palette_size);
    
    /* Initiate event structure.  */
    memset(&select_event, 0, sizeof(GX_EVENT));

    select_event.gx_event_type = GX_EVENT_SELECT;
    select_event.gx_event_display_handle = 1;
    select_event.gx_event_sender = 0;

    for(language_id = 0; language_id < 4; language_id++)
    {
        switch(language_id)
        {
        case 0:
            select_event.gx_event_target = (GX_WIDGET *)&simple_window.simple_window_radio_button_English;
            sprintf(test_comment, "Select language \"English\"");
            break;
        case 1:
            select_event.gx_event_target = (GX_WIDGET *)&simple_window.simple_window_radio_button_Chinese;
            sprintf(test_comment, "Select language \"Chinese\"");
            break;
        case 2:
            select_event.gx_event_target = (GX_WIDGET *)&simple_window.simple_window_radio_button_Japanese;
            sprintf(test_comment, "Select language \"Japanese\"");
            break;
        case 3:
            select_event.gx_event_target = (GX_WIDGET *)&simple_window.simple_window_radio_button_Spanish;
            sprintf(test_comment, "Select language \"Spanish\"");
            break;
        }

        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(test_comment);
        gx_system_event_send(&select_event);
        gx_validation_screen_refresh();
    }

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();
#endif
    exit(0);
}
