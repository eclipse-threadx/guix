/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_system.h"

TEST_PARAM test_parameter = {
    "guix_text_scroll_wheel_flat_draw", /* Test name */
    100, 117, 525, 336  /* Define the coordinates of the capture area.
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

char test_comment[255];

static VOID control_thread_entry(ULONG input)
{
INT                      frame_id = 1;
GX_STRING_SCROLL_WHEEL  *month_wheel;
INT                      index;

    /* Create scroll wheel screen dynamically. */
    gx_studio_named_widget_create("scroll_wheel_screen", GX_NULL, (GX_WIDGET **)&pScrollWheelScreen);

    gx_widget_find(pScrollWheelScreen, ID_MONTH_WHEEL, GX_SEARCH_DEPTH_INFINITE, &month_wheel);

    /* Removed round style. */
    gx_widget_style_remove(month_wheel, GX_STYLE_TEXT_SCROLL_WHEEL_ROUND);

    /* Toggle from button screen to scroll wheel screen. */
    ToggleScreen((GX_WINDOW *)pScrollWheelScreen, pButtonScreen);

    for(index = 0; index < 3; index++)
    {
        switch(index)
        {
        case 0:
            gx_widget_hide(month_wheel);
            gx_scroll_wheel_total_rows_set(month_wheel, 2);
            gx_widget_show(month_wheel);
            sprintf(test_comment, "set total rows = 2");
            break;

        case 1:
            gx_text_scroll_wheel_font_set(month_wheel, 1000, 1000);
            sprintf(test_comment, "set invalid font id");
            break;

        case 2:
            gx_text_scroll_wheel_font_set(month_wheel, 0, 0);
            month_wheel->gx_text_scroll_wheel_text_get = GX_NULL;
            sprintf(test_comment, "set valid font id, set gx_text_scroll_wheel_text_get NULL");
            break;
        }

        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(test_comment);
        gx_validation_screen_refresh();
    }
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}

