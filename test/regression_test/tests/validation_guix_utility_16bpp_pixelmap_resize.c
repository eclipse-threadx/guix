/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_utility_16bpp_pixelmap_resize", /* Test name */
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
#ifdef win32_graphics_driver_setup_565rgb  
#undef win32_graphics_driver_setup_565rgb  
#endif
#define win32_graphics_driver_setup_565rgb  gx_validation_graphics_driver_setup_565rgb


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"
#include "demo_guix_pixelmap_resize_16bpp.c"

char test_comment[255];
GX_RESOURCE_ID image[3] = {GX_PIXELMAP_ID_ICON_FOOT, GX_PIXELMAP_ID_RED_APPLE, GX_PIXELMAP_ID_FISH};

#define TEST_MEMORY_SIZE_20000 20000
#define TEST_MEMORY_SIZE_60000 60000

GX_COLOR  test_memory_20000[TEST_MEMORY_SIZE_20000];
GX_COLOR  test_memory_60000[TEST_MEMORY_SIZE_60000];

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int frame_id = 1;
int index;
int memory_index;

    x_ratio = 0.5;
    y_ratio = 0.5;

    for (index = 0; index < (int)(sizeof(image)/sizeof(GX_RESOURCE_ID)); index ++)
    {
        pixelmap_id = image[index];
        gx_validation_set_frame_id(frame_id);
        sprintf(test_comment, "pixelmap index:%d", index);

        gx_validation_set_frame_comment(test_comment);
        gx_validation_screen_refresh();

        /* Increment frame ID. */
        frame_id ++;
    }

    x_ratio = 1.5;
    y_ratio = 1.5;

    for(memory_index = 0; memory_index < 2; memory_index++)
    {
        tx_byte_pool_delete(&resize_pool);
        if(memory_index)
        {
            tx_byte_pool_create(&resize_pool, "rotate_pool_20000", test_memory_20000,
                        TEST_MEMORY_SIZE_20000 * sizeof(GX_COLOR));
        }
        else
        {
            tx_byte_pool_create(&resize_pool, "rotate_pool_60000", test_memory_60000,
                        TEST_MEMORY_SIZE_60000 * sizeof(GX_COLOR));
        }

        for (index = 0; index < (int)(sizeof(image)/sizeof(GX_RESOURCE_ID)); index ++)
        {
            pixelmap_id = image[index];
            gx_validation_set_frame_id(frame_id);
            sprintf(test_comment, "pixelmap index:%d", index);
            if(memory_index)
            {
                strcat(test_comment, ", memory pool size = 20000 * 4(bytes)");
            }
            else
            {
                strcat(test_comment, ", memory pool size = 60000 * 4(bytes)");
            }

            gx_validation_set_frame_comment(test_comment);
            gx_validation_screen_refresh();

            /* Increment frame ID. */
            frame_id ++;
        }
    } 
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
