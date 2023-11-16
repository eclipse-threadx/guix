/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_radial_progress_bar_alpha_blend", /* Test name */
    50, 100, 300, 350  /* Define the coordinates of the capture area.
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

#define REGRESSION_TEST

#include "gx_validation_wrapper.h"
#include "demo_guix_radial_progress_bar.c"

char test_comment[255];

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int      frame_id = 1;
int      radius = 0;
int      index;
GX_RECTANGLE size;


    /* Test enbale style.  */
    for(radial_progress_bar_alpha = 0; radial_progress_bar_alpha < 256; radial_progress_bar_alpha ++)
    {

        if(radius < 5)
        {
            radius = 100;
        }
        else
        {
            radius -= 5;
        }

        info->gx_radial_progress_bar_info_radius = radius;
        gx_radial_progress_bar_info_set(progress_bar, info);

        gx_validation_set_frame_id(frame_id++);
        sprintf(test_comment, "set radial progress bar blend alpha to %d", radial_progress_bar_alpha);
        gx_validation_set_frame_comment(test_comment);

        gx_validation_screen_refresh();
    }

    for(index = 0; index < 5; index++)
    {
        switch(index)
        {
            case 0:
                radial_progress_bar_alpha = 128;
                sprintf(test_comment, "Set alpha to 128");
                break;
            case 1:
                gx_widget_hide((GX_WIDGET *)progress_bar);
                sprintf(test_comment, "Hide progress bar");
                break;
            case 2:
                gx_widget_show((GX_WIDGET *)progress_bar);
                sprintf(test_comment, "Show progress bar");
                break;
            case 3:
                size = progress_bar->gx_widget_size;
                size.gx_rectangle_left += 10;
                gx_widget_resize((GX_WIDGET *)progress_bar, &size);
                sprintf(test_comment, "Change widget's width");
                break;
            case 4:
                size = progress_bar->gx_widget_size;
                size.gx_rectangle_top +=10;
                gx_widget_resize((GX_WIDGET *)progress_bar, &size);
                sprintf(test_comment, "Change widget's height");
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
