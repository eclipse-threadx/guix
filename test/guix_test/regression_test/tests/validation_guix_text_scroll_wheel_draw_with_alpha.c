/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_system.h"

TEST_PARAM test_parameter = {
    "guix_text_scroll_wheel_draw_with_alpha", /* Test name */
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

VOID test_numeric_wheel_draw(GX_NUMERIC_SCROLL_WHEEL* wheel)
{
    GX_BRUSH* brush;

    gx_context_brush_get(&brush);
    brush->gx_brush_alpha = 100;
    gx_text_scroll_wheel_draw((GX_TEXT_SCROLL_WHEEL*)wheel);
}

VOID test_string_wheel_draw(GX_STRING_SCROLL_WHEEL* wheel)
{
    GX_BRUSH* brush;

    gx_context_brush_get(&brush);
    brush->gx_brush_alpha = 100;
    gx_text_scroll_wheel_draw((GX_TEXT_SCROLL_WHEEL*)wheel);
}


static VOID control_thread_entry(ULONG input)
{
GX_STRING_SCROLL_WHEEL  *day_wheel;
GX_STRING_SCROLL_WHEEL  *month_wheel;
GX_STRING_SCROLL_WHEEL  *year_wheel;

    /* Create scroll wheel screen dynamically. */
    gx_studio_named_widget_create("scroll_wheel_screen", GX_NULL, (GX_WIDGET **)&pScrollWheelScreen);

    gx_widget_find(pScrollWheelScreen, ID_DAY_WHEEL, GX_SEARCH_DEPTH_INFINITE, &day_wheel);
    gx_widget_find(pScrollWheelScreen, ID_MONTH_WHEEL, GX_SEARCH_DEPTH_INFINITE, &month_wheel);
    gx_widget_find(pScrollWheelScreen, ID_YEAR_WHEEL, GX_SEARCH_DEPTH_INFINITE, &year_wheel);

    gx_widget_draw_set(day_wheel, test_numeric_wheel_draw);
    gx_widget_draw_set(month_wheel, test_string_wheel_draw);
    gx_widget_draw_set(year_wheel, test_numeric_wheel_draw);

    gx_widget_style_remove(year_wheel, GX_STYLE_TEXT_SCROLL_WHEEL_ROUND);

    /* Toggle from button screen to scroll wheel screen. */
    ToggleScreen((GX_WINDOW *)pScrollWheelScreen, pButtonScreen);

    gx_validation_set_frame_id(1);
    gx_validation_set_frame_comment("Capture screen shot");
    gx_validation_screen_refresh();

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}

