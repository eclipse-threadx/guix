/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_binres_language_table_load_use_old_api", /* Test name */
    5, 5, 630, 470  /* Define the coordinates of the capture area.
                         In this test, we only need to capture the line
                         drawing area.  */
};

static VOID      control_thread_entry(ULONG);
int main(int argc, char ** argv)
{
#ifdef GX_UTF8_SUPPORT
    /* Parse the command line argument. */
    gx_validation_setup(argc, argv);
#else
    printf("guix_binres_language_table_load_use_old_api:Skip Testing!\n");
#endif
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

#define REGRESSION_TEST_MODE

#include "gx_validation_wrapper.h"
#include "demo_guix_multi_themes_16bpp.c"
GX_CHAR ***old_language_table = GX_NULL;

UINT load_binres_language_table_load_use_old_api(GX_UBYTE *root_address)
{
UINT  status = GX_SUCCESS;

    if (old_language_table)
    {
        memory_free(old_language_table);
        old_language_table = GX_NULL;
    }

    status = gx_binres_language_table_load(root_address, (GX_UBYTE ****)&old_language_table);

    if (old_language_table)
    {
        gx_display_language_table_set(root->gx_window_root_canvas->gx_canvas_display, old_language_table, 1, MAIN_DISPLAY_STRING_TABLE_SIZE);
        gx_system_active_language_set(0);
    }
    return status;
}

char test_comment[256];
extern SIMPLE_WINDOW_CONTROL_BLOCK simple_window;

static VOID control_thread_entry(ULONG input)
{
#ifdef GX_UTF8_SUPPORT
int frame_id = 1;
int index;

    for(index = 0; index  < 2; index ++)
    {
        switch(index)
        {
        case 0:
            load_binres_language_table_load_use_old_api(Japanese_root_address);
            sprintf(test_comment, "Load Japanese use old api");
            break;

        case 1:
            load_binres_language_table_load_use_old_api(binres_root_address);
            sprintf(test_comment, "Load Spanish use old api");
            break;
        }

        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(test_comment);
        gx_validation_screen_refresh();
    }

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();
#endif
    exit(0);
}
