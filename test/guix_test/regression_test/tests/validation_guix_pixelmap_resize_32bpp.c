/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_pixelmap_resize_32bpp", /* Test name */
    42, 91, 595, 428     /* Define the coordinates of the capture area.
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

#include "gx_validation_wrapper.h"
#include "demo_guix_pixelmap_resize_32bpp.c"

char test_comment[255];
extern GX_CONST GX_PIXELMAP *display_1_theme_1_pixelmap_table[];
GX_RESOURCE_ID image[3] = {GX_PIXELMAP_ID_ICON_FOOT, GX_PIXELMAP_ID_RED_APPLE, GX_PIXELMAP_ID_FISH};
float ratio[10] = {0.2, 0.4, 0.6, 0.8, 1, 1.2, 1.4, 1.6, 1.8, 2};

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int frame_id = 1;
int index, ratio_i;

    for (index = 0; index < (int)(sizeof(image)/sizeof(GX_RESOURCE_ID)); index ++)
    {
	pixelmap_id = image[index];

	for(ratio_i = 0; ratio_i < (int)(sizeof(ratio)/sizeof(float)); ratio_i++)
        {
            x_ratio = ratio[ratio_i];
            y_ratio = ratio[ratio_i];

            gx_validation_set_frame_id(frame_id);
            sprintf(test_comment, "pixelmap index:%d, scaling ratio:%.2f ", index, ratio[ratio_i]);

            gx_validation_set_frame_comment(test_comment);

            /* Mark the image draw window dirty. */
            gx_system_dirty_mark(pPixelmapWin);

            gx_validation_screen_refresh();

            /* Increment frame ID. */
            frame_id ++;
	}
    }

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
