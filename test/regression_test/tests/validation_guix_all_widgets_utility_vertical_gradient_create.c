/* This is a small demo of the high-performance GUIX graphics framework. */


#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_system.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_all_widgets_utility_vertical_gradient_create", /* Test name */
    75, 10, 273, 307  /* Define the coordinates of the capture area.
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

GX_GRADIENT gradient;

void button_screen_draw(GX_WINDOW *window)
{
    gx_window_draw(window);
    gx_context_line_color_set(GX_COLOR_ID_ORANGE);
    gx_context_fill_color_set(GX_COLOR_ID_ORANGE);
    gx_canvas_pixelmap_draw( window->gx_widget_size.gx_rectangle_left + 10, window->gx_widget_size.gx_rectangle_top + 10, &gradient.gx_gradient_pixelmap);
}

char test_comment[256];

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int frame_id = 1;
    
    gx_utility_gradient_create(&gradient, 100, 200, GX_GRADIENT_TYPE_ALPHA | GX_GRADIENT_TYPE_VERTICAL, 0, 255);
    gx_widget_draw_set(pButtonScreen, (void(*)(GX_WIDGET *))button_screen_draw);
    
    gx_validation_set_frame_id(frame_id++);
    sprintf(test_comment, "Create vertical gradient and draw its pixelmap.");
    gx_validation_set_frame_comment(test_comment);
    /* Force a screen refresh. */
    gx_validation_screen_refresh();
    
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}





