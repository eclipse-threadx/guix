/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_widget.h"

TEST_PARAM test_parameter = {
    "guix_widget_children_draw", /* Test name */
    0, 0, 342, 195  /* Define the coordinates of the capture area.
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
int          frame_id = 1;
GX_WIDGET   *parent;
GX_PROMPT    test_prompt[4];
GX_RECTANGLE size;
int          index;

    parent = (GX_WIDGET *)&window_screen.window_screen_window_8;
    for(index =0; index < 4; index++)
    {
        /* Create child prompt for child window. */
        switch(index)
        {
        case 0:
            gx_utility_rectangle_define(&size, 217, 97, 297, 153);
            break;

        case 1:
            gx_utility_rectangle_define(&size, 223, 103, 291, 145);
            parent = (GX_WIDGET *)&test_prompt[0];
            break;

        case 2:
            gx_utility_rectangle_define(&size, 228, 110, 288, 141);
            parent = (GX_WIDGET *)&test_prompt[1];
            break;

        case 3:
            gx_utility_rectangle_define(&size, 235, 114, 284, 138);
            parent = (GX_WIDGET *)&test_prompt[2];
            break;
        }
        memset(&test_prompt[index], 0, sizeof(GX_PROMPT));
        gx_prompt_create(&test_prompt[index], "test_prompt", parent, 0, GX_STYLE_ENABLED | GX_STYLE_BORDER_THIN, 1024 + index, &size);
    }

    ToggleScreen((GX_WINDOW *)&window_screen, (GX_WINDOW *)&button_screen);

    /* Toggle to text screen. */
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Create nested widgets that exceed GX_MAX_CONTEXT_NESTING");
    gx_validation_screen_refresh();

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
