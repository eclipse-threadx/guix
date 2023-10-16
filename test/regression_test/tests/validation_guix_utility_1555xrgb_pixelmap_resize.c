/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_system.h"

TEST_PARAM test_parameter = {
    "guix_utility_1555xrgb_pixelmap_resize", /* Test name */
    40, 90, 600, 430  /* Define the coordinates of the capture area.
                         In this test, we only need to capture the pixelmap
                         drawing area.  */
};

extern void mark_dirty();
extern void convert_start();

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
#ifdef win32_graphics_driver_setup_1555xrgb
#undef win32_graphics_driver_setup_1555xrgb
#endif
#define win32_graphics_driver_setup_1555xrgb  gx_validation_graphics_driver_setup_1555xrgb


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_guix_all_widgets_1555xrgb.c"

GX_RESOURCE_ID resize_pixelmap_id_list[]={
    GX_PIXELMAP_ID_ROTATE_FOOT,
    GX_PIXELMAP_ID_ROTATE_FISH
};

char test_comment[256];

extern GX_RESOURCE_ID resize_pixelmap_id;

#define TEST_MEMORY_SIZE_20000 20000
#define TEST_MEMORY_SIZE_31000 31000

GX_COLOR  test_memory_20000[TEST_MEMORY_SIZE_20000];
GX_COLOR  test_memory_31000[TEST_MEMORY_SIZE_31000];

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int          frame_id = 1;
int          pixelmap_index;
extern float x_ratio;
extern float y_ratio;
int          index;

    /* Toggle to Image Convert Screen. */
    ToggleScreen((GX_WINDOW *)&resize_screen, (GX_WINDOW *)&rotate_screen);
    x_ratio = 1.5;
    y_ratio = 1.5;

    for(pixelmap_index = 0; pixelmap_index < 2; pixelmap_index++)
    {
        resize_pixelmap_id = resize_pixelmap_id_list[pixelmap_index];

        /* Capture a frame. */
        gx_validation_set_frame_id(frame_id++);
        sprintf(test_comment, "pixelmap_id = %d",(int)resize_pixelmap_id);
        gx_validation_set_frame_comment(test_comment);
        gx_validation_screen_refresh();
    }
    
    for(index = 0; index < 2; index++)
    { 
        tx_byte_pool_delete(&rotate_pool);
        if(index)
        {
            tx_byte_pool_create(&rotate_pool, "rotate_pool_20000", test_memory_20000,
                        TEST_MEMORY_SIZE_20000 * sizeof(GX_COLOR));
        }
        else
        {
            tx_byte_pool_create(&rotate_pool, "rotate_pool_31000", test_memory_31000,
                        TEST_MEMORY_SIZE_31000 * sizeof(GX_COLOR));
        }

        for(pixelmap_index = 0; pixelmap_index < 2; pixelmap_index++)
        {
            resize_pixelmap_id = resize_pixelmap_id_list[pixelmap_index];

            /* Capture a frame. */
            gx_validation_set_frame_id(frame_id++);
            sprintf(test_comment, "pixelmap_id = %d",(int)resize_pixelmap_id);
            if(index)
            {
                strcat(test_comment, ", memory pool size = 20000 * 4(bytes)");
            }
            else
            {
                strcat(test_comment, ", memory pool size = 31000 * 4(bytes)");
            }
            gx_validation_set_frame_comment(test_comment);
            gx_validation_screen_refresh();
        }
    }
    
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
