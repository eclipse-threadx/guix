/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_radial_progress_bar_background_draw", /* Test name */
    0, 0, 300, 350  /* Define the coordinates of the capture area.
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

#define TEST_MEMORY_SIZE 1000
GX_COLOR TEST_MEMORY[TEST_MEMORY_SIZE];

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int      frame_id = 1;
int      index;

    radial_progress_bar_alpha = 128;

    gx_widget_style_add(progress_bar, GX_STYLE_TRANSPARENT);
    
    for(index = 0; index < 4; index++)
    {
        switch(index)
        {
        case 0:
            gx_widget_shift(progress_bar, -progress_bar->gx_widget_size.gx_rectangle_left, 0, GX_TRUE);
            sprintf(test_comment, "shift progress bar left");
            break;

        case 1:
            gx_widget_shift(progress_bar, 0, -progress_bar->gx_widget_size.gx_rectangle_top, GX_TRUE);
            sprintf(test_comment, "shift progress bar top");
            break;

        case 2:
            gx_widget_hide(progress_bar);
            tx_byte_pool_delete(&memory_pool);
            tx_byte_pool_create(&memory_pool, "memory_pool", TEST_MEMORY, TEST_MEMORY_SIZE * sizeof(GX_COLOR));
            gx_widget_show(progress_bar);
            sprintf(test_comment, "recreate memory pool to assin a small memory block");
            break;

        case 3:
            gx_system_memory_allocator_set(GX_NULL, GX_NULL);
            sprintf(test_comment, "set memory allocator null");
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
