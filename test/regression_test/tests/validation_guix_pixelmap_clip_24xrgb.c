/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_pixelmap_clip_24xrgb", /* Test name */
    50, 20, 259, 173  /* Define the coordinates of the capture area.
                         In this test, we only need to capture the wallpaper area.  */
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

#ifdef win32_graphics_driver_setup_24xrgb  
#undef win32_graphics_driver_setup_24xrgb  
#endif
/* Replace the default graphics driver with the validation driver. */
#define win32_graphics_driver_setup_24xrgb  gx_validation_graphics_driver_setup_24xrgb

#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_guix_pixelmap_clip_32bpp.c"

char test_comment[256];

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int alpha, compressed;
int frame_id = 1;

    for(alpha = 0; alpha < 2; alpha++)
    {
        for (compressed = 0; compressed < 2; compressed++)
        {
            gx_validation_set_frame_id(frame_id);

            if (alpha)
            {
                sprintf(test_comment, "alpha, ");
                if (compressed)
                {
                    strcat(test_comment, "compressed");
                    gx_window_wallpaper_set(pPixelmapWin, GX_PIXELMAP_ID_RAINBOW200_ALPHA_COMPRESS, GX_FALSE);
                }
                else
                {
                    strcat(test_comment, "noncompressed");
                    gx_window_wallpaper_set(pPixelmapWin, GX_PIXELMAP_ID_RAINBOW200_ALPHA_NONCOMPRESS, GX_FALSE);
                }
            }
            else
            {
                sprintf(test_comment, "noalpha, ");
                if (compressed)
                {
                    strcat(test_comment, "compressed");
                    gx_window_wallpaper_set(pPixelmapWin, GX_PIXELMAP_ID_RAINBOW200_NOALPHA_COMPRESS, GX_FALSE);
                }
                else
                {
                    strcat(test_comment, "noncompressed");
                    gx_window_wallpaper_set(pPixelmapWin, GX_PIXELMAP_ID_RAINBOW200_NOALPHA_NONCOMPRESS, GX_FALSE);
                }
            }

            gx_validation_set_frame_comment(test_comment);

            /* Mark the window "dirty" */
            gx_system_dirty_mark(pPixelmapWin);

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





