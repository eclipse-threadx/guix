/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_circular_gauge_16bpp", /* Test name */
    167, 80, 437, 360  /* Define the coordinates of the capture area.
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
#include "demo_guix_all_widgets_16bpp.c"

char test_comment[255];

#define TEST_MEMORY_SIZE 1000
GX_UBYTE test_memory[TEST_MEMORY_SIZE];

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int                frame_id = 1;
GX_CIRCULAR_GAUGE *gauge;
int                index;

    gauge = &gauge_screen.gauge_screen_gauge_1;

    gx_circular_gauge_animation_set(gauge, 0, 1);

    /* Toggle from button screen to gauge screen. */
    ToggleScreen((GX_WINDOW *)&gauge_screen, (GX_WINDOW *)&button_screen);

    for(index = 0; index < 5; index++)
    {   
        switch(index) 
        {
        case 0:
            gx_circular_gauge_angle_set(gauge, 30);
            sprintf(test_comment, "set needle angle = 30");
            break;

        case 1:
            gx_system_memory_allocator_set(rotate_memory_allocate, GX_NULL);
            sprintf(test_comment, "set memory free = GX_NULL");
            break;

        case 2:
            gx_circular_gauge_angle_set(gauge, 0);
            sprintf(test_comment, "set needle angle = 0");
            break;

        case 3:
            tx_byte_pool_delete(&rotate_pool);
            tx_byte_pool_create(&rotate_pool, "test_memory_pool", test_memory, TEST_MEMORY_SIZE);
            gx_system_memory_allocator_set(rotate_memory_allocate, rotate_memory_free);
            gx_circular_gauge_angle_set(gauge, 30);
            sprintf(test_comment, "set memory pool size = %d, needle angle = 30", TEST_MEMORY_SIZE);
            break;

        case 4:
            gx_widget_hide(gauge);
            gauge -> gx_circular_gauge_needle_source = GX_NULL;
            gauge -> gx_circular_gauge_info.gx_circular_gauge_info_needle_pixelmap = 0;
            gx_widget_show(gauge);
            sprintf(test_comment, "set needle source id = 0");
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
