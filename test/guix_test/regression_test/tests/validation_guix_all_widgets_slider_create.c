/* This is a small demo of the high-performance GUIX graphics framework. */


#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_system.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_all_widgets_slider_create", /* Test name */
    146, 53, 465, 386  /* Define the coordinates of the capture area.
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
       on the pixelmap drawing example. These operations simulate 
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


char test_comment[256];
GX_SLIDER  vertical_slider;
GX_SLIDER  horizontal_slider;
GX_SLIDER_INFO vslider_info =
{
    0, //INT      gx_slider_info_min_val;
    100, //INT      gx_slider_info_max_val;
    50, //INT      gx_slider_info_current_val;
    10, //INT      gx_slider_info_increment;
    0, //GX_VALUE gx_slider_info_min_travel;
    0, //GX_VALUE gx_slider_info_max_travel;
    0, //GX_VALUE gx_slider_info_needle_width;
    0, //GX_VALUE gx_slider_info_needle_height;
    0, //GX_VALUE gx_slider_info_needle_inset;
    0 //GX_VALUE gx_slider_info_needle_hotspot_offset;
};

GX_SLIDER_INFO hslider_info =
{
    0, //INT      gx_slider_info_min_val;
    100, //INT      gx_slider_info_max_val;
    50, //INT      gx_slider_info_current_val;
    10, //INT      gx_slider_info_increment;
    0, //GX_VALUE gx_slider_info_min_travel;
    0, //GX_VALUE gx_slider_info_max_travel;
    0, //GX_VALUE gx_slider_info_needle_width;
    0, //GX_VALUE gx_slider_info_needle_height;
    0, //GX_VALUE gx_slider_info_needle_inset;
    0 //GX_VALUE gx_slider_info_needle_hotspot_offset;
};

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int frame_id = 1;
GX_RECTANGLE size;
    
    ToggleScreen(pIndicatorScreen, pButtonScreen);
    size.gx_rectangle_left = 180;
    size.gx_rectangle_top = 325;
    size.gx_rectangle_right = 209;
    size.gx_rectangle_bottom = 374;
    
    gx_slider_create(&vertical_slider, "vertical_slider", GX_NULL, 0, &vslider_info, GX_STYLE_SLIDER_VERTICAL, 0, &size);
    gx_widget_attach(pIndicatorScreen, (GX_WIDGET *)&vertical_slider);
    
    gx_system_dirty_mark(pIndicatorScreen);
    
    gx_validation_set_frame_id(frame_id++);
    sprintf(test_comment, "Create vertical slider");
    gx_validation_set_frame_comment(test_comment);
    /* Force a screen refresh. */
    gx_validation_screen_refresh();
    
    size.gx_rectangle_left = 235;
    size.gx_rectangle_top = 340;
    size.gx_rectangle_right = 334;
    size.gx_rectangle_bottom = 364;
    
    gx_slider_create(&horizontal_slider, "horizontal_slider", GX_NULL, 0, &hslider_info, 0, 0, &size);
    gx_widget_attach(pIndicatorScreen, (GX_WIDGET *)&horizontal_slider);
    gx_system_dirty_mark(pIndicatorScreen);
    
    gx_validation_set_frame_id(frame_id++);
    sprintf(test_comment, "Create vertical slider");
    gx_validation_set_frame_comment(test_comment);
    /* Force a screen refresh. */
    gx_validation_screen_refresh();
    
    
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}





