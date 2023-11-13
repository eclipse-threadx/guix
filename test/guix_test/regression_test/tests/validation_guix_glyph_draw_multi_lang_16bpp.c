/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_glyph_draw_multi_lang_16bpp", /* Test name */
    30, 20, 240, 210  /* Define the coordinates of the capture area.
                      In this test, we only need to capture the prompt
                      drawing area.  */
};

static VOID      control_thread_entry(ULONG);
int main(int argc, char ** argv)
{
#ifdef GX_UTF8_SUPPORT
    /* Parse the command prompt argument. */
    gx_validation_setup(argc, argv);
#else
    printf("guix_glyph_draw_multi_lang_16bpp:Skip Testing!\n");
#endif
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

#include "demo_glyph_draw_multi_lang_16bpp.c"

char test_comment[256];

static VOID control_thread_entry(ULONG input)
{
#ifdef GX_UTF8_SUPPORT
int frame_id = 1;

    gx_validation_set_frame_id(frame_id);

    sprintf(test_comment, "Screen shot.");
    gx_validation_set_frame_comment(test_comment);

    /* Mark the window "dirty". */
    gx_system_dirty_mark(pMainWin);

    /* Force a screen refresh. */
    gx_validation_screen_refresh();

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();
#endif
    exit(0);
}





