/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_widget_style_set", /* Test name */
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

#include "demo_guix_all_widgets.c"
GX_CHAR test_comment[256];

static VOID control_thread_entry(ULONG input)
{
int        frame_id = 1;
int        index;
GX_WIDGET *window_5 = (GX_WIDGET *)&window_screen.window_screen_window_5;
GX_WIDGET *next_button = (GX_WIDGET *)&window_screen.window_screen_next_button_1;

    ToggleScreen((GX_WINDOW *)&window_screen, (GX_WINDOW *)&button_screen);    

    for(index = 0; index < 2; index++)
    {

        if(index)
        {
            gx_widget_hide(window_5);
            gx_widget_hide(next_button);
        }
        gx_widget_style_set(window_5, GX_STYLE_ENABLED | GX_STYLE_TRANSPARENT);
        gx_widget_style_set(next_button, GX_STYLE_ENABLED | GX_STYLE_TRANSPARENT);

        sprintf(test_comment, "set style GX_STYLE_ENABLED|GX_STYLE_TRANSPARENT");
        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(test_comment);
        gx_validation_screen_refresh();

        gx_widget_style_set(window_5, GX_STYLE_ENABLED | GX_STYLE_TRANSPARENT);
        gx_widget_style_set(next_button, GX_STYLE_ENABLED | GX_STYLE_TRANSPARENT);

        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(test_comment);
        gx_validation_screen_refresh();

        sprintf(test_comment, "set style GX_STYLE_ENABLED");
        gx_widget_style_set(window_5, GX_STYLE_ENABLED);
        gx_widget_style_set(next_button, GX_STYLE_ENABLED);

        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(test_comment);
        gx_validation_screen_refresh();

        gx_widget_style_set(window_5, GX_STYLE_ENABLED);
        gx_widget_style_set(next_button, GX_STYLE_ENABLED);

        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(test_comment);
        gx_validation_screen_refresh();
    }
  
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
