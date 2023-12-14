/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_system.h"

TEST_PARAM test_parameter = {
    "guix_all_widgets_widget_focus", /* Test name */
    0, 0, 640, 480  /* Define the coordinates of the capture area.
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
int frame_id = 1;
int index;
GX_SINGLE_LINE_TEXT_INPUT *sl_input;
GX_MULTI_LINE_TEXT_INPUT  *ml_input;

    sl_input = &text_screen.text_screen_text_input_1;
    ml_input = &text_screen.text_screen_text_input_2;

    gx_single_line_text_input_style_remove(sl_input, GX_STYLE_CURSOR_BLINK);
    gx_multi_line_text_input_style_remove(ml_input, GX_STYLE_CURSOR_BLINK);

    ToggleScreen((GX_WINDOW *)&text_screen, (GX_WINDOW *)&button_screen);

    gx_widget_focus_next(GX_NULL);
    gx_widget_focus_previous(GX_NULL);

    sprintf(test_comment, "Test null pointer of gx_widget_focus_next and gx_widget_focus_previous");
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();

    for(index = 0; index < 4; index++)
    {

        if(index < 2)
        {
            gx_widget_focus_next(_gx_system_focus_owner);
            sprintf(test_comment, "move focus to next item");
        }
        else
        {
            gx_widget_focus_previous(_gx_system_focus_owner);
            sprintf(test_comment, "Move focus to previous item");
        }
        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(test_comment);
        gx_validation_screen_refresh();
    }

    ToggleScreen((GX_WINDOW *)&focus_test_screen, (GX_WINDOW *)&text_screen);
    gx_system_focus_claim((GX_WIDGET *)&focus_test_screen.focus_test_screen_prompt_1);
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Claim focus on prompt 1");
    gx_validation_screen_refresh();

    _gx_system_focus_claim((GX_WIDGET *)&focus_test_screen.focus_test_screen_prompt_2);
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Claim focus on prompt 2");
    gx_validation_screen_refresh();

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
