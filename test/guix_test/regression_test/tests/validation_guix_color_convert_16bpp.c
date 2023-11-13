/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_color_convert_16bpp", /* Test name */
    0, 0, 640, 480  /* Define the coordinates of the capture area.
                    In this test, we only need to capture the prompt
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
    on the prompt drawing example. These operations simulate
    user input. */
    gx_validation_control_thread_create(control_thread_entry);

    /* Termiante the test if it runs for more than 100 ticks */
    /* This function is not implemented yet. */
    gx_validation_watchdog_create(100);

    /* Call the actual prompt routine. */
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

#include "demo_guix_color_convert_16bpp.c"

char test_comment[256];
INT  pixelmaps[]={
    GX_PIXELMAP_ID_RED_APPLE_PNG,
    GX_PIXELMAP_ID_ICON_FOOT_JPG,
    GX_PIXELMAP_ID_FISH_SMALL_411,
    GX_PIXELMAP_ID_FISH_SMALL_422,
    GX_PIXELMAP_ID_FISH_SMALL_444,
    GX_PIXELMAP_ID_FISH_SMALL_420,
    GX_PIXELMAP_ID_BG_WASHER_ON_PNG};

static VOID control_thread_entry(ULONG input)
{
INT  frame_id = 1;
UINT compress_index;
UINT alpha_index;
UINT dither_index;
UINT map_index;


    for(compress_index = 0; compress_index < 2; compress_index++)
    {
        for(alpha_index = 0; alpha_index < 2; alpha_index++)
        {
            for(dither_index = 0; dither_index < 2; dither_index++)
            {
                image_reader_mode = 0;
                
                if(compress_index)
                {
                    image_reader_mode |= GX_IMAGE_READER_MODE_COMPRESS;
                    sprintf(test_comment, "compressed, ");
                }
                else
                {
                    sprintf(test_comment, "un-compressed, ");
                }

                if(alpha_index)
                {
                    image_reader_mode |= GX_IMAGE_READER_MODE_ALPHA;
                    strcat(test_comment, "with alpha, ");
                }
                else
                {
                    strcat(test_comment, "without alpha, ");
                }

                if(dither_index)
                {
                    image_reader_mode |= GX_IMAGE_READER_MODE_DITHER;
                    strcat(test_comment, "dither");
                }
                else
                {
                    strcat(test_comment, "no dither");
                }
                
                for(map_index = 0; map_index < sizeof(pixelmaps)/sizeof(INT); map_index++)
                {
                    map_id = pixelmaps[map_index];

                    gx_validation_set_frame_id(frame_id);

                    frame_id++;

                    gx_validation_set_frame_comment(test_comment);

                    /* Mark the window "dirty". */
                    gx_system_dirty_mark(pMainWin);

                    /* Force a screen refresh. */
                    gx_validation_screen_refresh();
                }
            }
        }
    }

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
