/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_pixelmap_24xrgb", /* Test name */
    9, 5, 250, 168  /* Define the coordinates of the capture area.
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

#include "demo_guix_pixelmap_32bpp.c"
/* Define the move values for the horizontal slider. */
static int horizontal_value[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

/* Define the move values for the vertical slider.*/
static int vertical_value[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

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

int horizontal_index, vertical_index, alpha_index, compressed_index;
int frame_id = 1;
GX_SLIDER *slider_h;
GX_SLIDER *slider_v;

    gx_widget_find(pMainWin, ID_HORIZONTALMOVE, 0, &slider_h);
    gx_widget_find(pMainWin, ID_VERTICALMOVE, 0, &slider_v);

    for(alpha_index = 0; alpha_index < 2; alpha_index++)
    {
        alpha = alpha_value[alpha_index];
        for(compressed_index = 0; compressed_index < 2; compressed_index++)
        {
            compressed = compressed_value[compressed_index];
            for(horizontal_index = 0; horizontal_index < (INT)(sizeof(horizontal_value) / sizeof(int)); horizontal_index++)
            {
                gx_slider_value_set(slider_h, &slider_h -> gx_slider_info, horizontal_value[horizontal_index]);

                for(vertical_index = 0; vertical_index < (INT)(sizeof(vertical_value) / sizeof(int)); vertical_index++)
                {
                    /* Inform the validation system 
                    (1) Frame ID, which identifies a specific test configuration;
                    (2) Start recording frame on the next toggle operation.
                    */
                    gx_validation_set_frame_id(frame_id);

                    /* Set the vertical slider value */
                    gx_slider_value_set(slider_v, &slider_v -> gx_slider_info, vertical_value[vertical_index]);

                    sprintf(test_comment, "xshift %d yshift %d ", horizontal_value[horizontal_index], vertical_value[vertical_index]);
                    if(alpha) 
                        strcat(test_comment, "alpha ");
                    else
                        strcat(test_comment, "without alpha ");
                    if(compressed)
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
        }
    }
    
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}





