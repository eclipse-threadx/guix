/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_all_widgets_16bpp_fill_text_color_test", /* Test name */
    0, 0, 639, 479  /* Define the coordinates of the capture area.
                         In this test, we only need to capture the graphics
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
#ifdef win32_graphics_driver_setup_565rgb  
#undef win32_graphics_driver_setup_565rgb  
#endif
#define win32_graphics_driver_setup_565rgb  gx_validation_graphics_driver_setup_565rgb


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_guix_all_widgets_16bpp.c"

char test_comment[256];

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
INT frame_id = 1;
GX_WIDGET *child;
INT add_style;
INT remove_style;

    ToggleScreen((GX_WINDOW *)&widget_color_test_screen, pButtonScreen);

    for(frame_id = 1; frame_id <= 3; frame_id++)
    {
        child = widget_color_test_screen.gx_widget_first_child;
        switch(frame_id)
        {
        case 1:
            add_style = GX_STYLE_ENABLED;
            remove_style = 0;
            sprintf(test_comment, "test normal fill/text color");
            break;
        case 2:
            add_style = GX_STYLE_DRAW_SELECTED | GX_STYLE_BUTTON_PUSHED;
            remove_style= 0;
            sprintf(test_comment, "test selected fill/text color");
            break;
        case 3:
            add_style = 0;
            remove_style = GX_STYLE_ENABLED;
            sprintf(test_comment, "test disabled fill/test color");
            break;
        }
        while(child)
        {
            gx_widget_style_add(child, add_style);
            gx_widget_style_remove(child, remove_style);
            child = child->gx_widget_next;
        }
        gx_validation_set_frame_id(frame_id);
        gx_validation_set_frame_comment(test_comment);
        gx_validation_screen_refresh();
    }

    add_style = GX_STYLE_ENABLED|GX_STYLE_TEXT_INPUT_READONLY;
    gx_widget_style_add((GX_WIDGET *)&widget_color_test_screen.widget_color_test_screen_ml_text_input, add_style);
    gx_widget_style_add((GX_WIDGET *)&widget_color_test_screen.widget_color_test_screen_sl_text_input, add_style);
    sprintf(test_comment, "test readonly fill/text color");
    gx_validation_set_frame_id(frame_id);
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();

    /* Signal the end of the test case, Verify the outout.  */
    gx_validation_end();

    exit(0);
}
