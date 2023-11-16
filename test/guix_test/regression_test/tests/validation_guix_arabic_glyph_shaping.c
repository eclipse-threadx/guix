/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_arabic_glyph_shaping", /* Test name */
    0, 0, 639, 479  /* Define the coordinates of the capture area.
                         In this test, we only need to capture the line
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

#include "demo_arabic_glyph_shaping.c"

static VOID control_thread_entry(ULONG input)
{
int frame_id = 1;
char pathname[255];
int  pathlen;

    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("test generated arabic string resource data");
    gx_validation_screen_refresh();

    gx_validation_extract_path(__FILE__, pathname, &pathlen);
    strcat(pathname, "..//..//..//example_internal//arabic_glyph_shaping//arabic_string_res.bin");

    load_language_table(pathname);

    if (language_table)
    {
        /* Set language table. */
        gx_display_language_table_set_ext(root->gx_window_root_canvas->gx_canvas_display, (GX_CONST GX_STRING **)language_table, 1, DISPLAY_1_STRING_TABLE_SIZE);
                                 
        /* Set active language. */
        gx_system_active_language_set(0);
    }

    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("test generated arabic string binary data");
    gx_validation_screen_refresh();

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
