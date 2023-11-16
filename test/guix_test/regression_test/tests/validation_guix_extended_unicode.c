/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_extended_unicode", /* Test name */
    0, 0, 640, 480  /* Define the coordinates of the capture area.
                    In this test.  */
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
#ifdef win32_graphics_driver_setup_24xrgb
#undef win32_graphics_driver_setup_24xrgb
#endif
#define win32_graphics_driver_setup_24xrgb  gx_validation_graphics_driver_setup_24xrgb


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "extended_unicode.c"

char test_comment[256];

static VOID control_thread_entry(ULONG input)
{
INT frame_id = 1;

    gx_validation_set_frame_id(frame_id);

    sprintf(test_comment, "Screen shot of extended unicode draw demo.");

    gx_validation_set_frame_comment(test_comment);

    /* Mark the window "dirty". */
    gx_system_dirty_mark((GX_WIDGET *)&screen_base);

    /* Force a screen refresh. */
    gx_validation_screen_refresh();

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
