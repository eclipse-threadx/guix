/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_utility_32argb_pixelmap_resize", /* Test name */
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

GX_RESOURCE_ID image[3] = {GX_PIXELMAP_ID_ICON_FOOT, GX_PIXELMAP_ID_RED_APPLE, GX_PIXELMAP_ID_FISH};
VOID *test_allocate(ULONG size)
{
   return GX_NULL;
}

VOID test_free(VOID *mem)
{
    return;
}
/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int frame_id = 1;
int index;

    x_ratio = 0.5;
    y_ratio = 0.5;

    for (index = 0; index < (int)(sizeof(image)/sizeof(GX_RESOURCE_ID)); index ++)
    {
		pixelmap_id = image[index];

        gx_validation_set_frame_id(frame_id);
        sprintf(test_comment, "pixelmap index:%d ", index);

        gx_validation_set_frame_comment(test_comment);

        /* Mark the image draw window dirty. */
        gx_system_dirty_mark(pPixelmapWin);

        gx_validation_screen_refresh();

        /* Increment frame ID. */
        frame_id ++;
    }
    gx_system_memory_allocator_set(test_allocate, test_free);
    for (index = 0; index < (int)(sizeof(image)/sizeof(GX_RESOURCE_ID)); index ++)
    {
		pixelmap_id = image[index];

        gx_validation_set_frame_id(frame_id);
        sprintf(test_comment, "Set system allocate function to return null then call rotate. pixelmap_id = %d",(int)index);

        gx_validation_set_frame_comment(test_comment);

        /* Mark the image draw window dirty. */
        gx_system_dirty_mark(pPixelmapWin);

        gx_validation_screen_refresh();

        /* Increment frame ID. */
        frame_id ++;
    }
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
