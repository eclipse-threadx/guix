/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_all_widgets_332rgb_pixelmap_screen", /* Test name */
    0, 0, 640, 480  /* Define the coordinates of the capture area.
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
#ifdef win32_graphics_driver_setup_332rgb  
#undef win32_graphics_driver_setup_332rgb  
#endif
#define win32_graphics_driver_setup_332rgb  gx_validation_graphics_driver_setup_332rgb


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_all_widgets_332rgb.c"
/* Define the alpha settings. */
static int alpha_value[] = {GX_TRUE, GX_FALSE};

/* Define the compressed settings. */
static int compressed_value[] = {GX_TRUE, GX_FALSE};

char test_comment[256];
/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int alpha_index, compressed_index;
int frame_id = 1;

    ToggleScreen(pPicScreen, pShapesScreen);
    for(alpha_index = 0; alpha_index < 2; alpha_index++)
    {
        alpha = alpha_value[alpha_index];
        for(compressed_index = 0; compressed_index < 2; compressed_index++)
        {
            compress = compressed_value[compressed_index];
            /* Inform the validation system 
            (1) Frame ID, which identifies a specific test configuration;
            (2) Start recording frame on the next toggle operation.
            */
            gx_validation_set_frame_id(frame_id);

            if(alpha) 
                strcat(test_comment, "alpha ");
            else
                strcat(test_comment, "without alpha ");
            if(compress)
                strcat(test_comment, "compressed ");
            else
                strcat(test_comment, "without compressed ");

            gx_validation_set_frame_comment(test_comment);

            /* Mark the window "dirty" */
            gx_system_dirty_mark(pic_window);

            /* Force a screen refresh. */
            gx_validation_screen_refresh();

            /* Increment frame ID */
            frame_id ++;
        }
    }
    
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}





