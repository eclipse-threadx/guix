/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_utility_canvas_to_bmp_16bpp_no_output", /* Test name */
    0, 0, 0, 0  /* Define the coordinates of the capture area.*/
};

static VOID      control_thread_entry(ULONG);

int main(int argc, char ** argv)
{
  tx_kernel_enter();
  return(0);
}

VOID tx_application_define(void *first_unused_memory)
{

    /* Create a dedicated thread to perform various operations
       on the line drawing example. These operations simulate 
       user input. */
    gx_validation_application_define(first_unused_memory);
    
    /* Termiante the test if it runs for more than 100 ticks */
    /* This function is not implemented yet. */
    gx_validation_watchdog_create(100);

    /* Create a dedicated thread to perform various operations
       on the pixelmap drawing example. These operations simulate 
       user input. */
    gx_validation_control_thread_create(control_thread_entry);       
}

#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"
#include "gx_validation_compare_file_data.h"

#define REGRESSION_TEST_MODE

#include "demo_guix_screenshot_to_bitmap_16bpp.c"


/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */

static VOID control_thread_entry(ULONG input)
{
UINT status;
char golden_file[255];
char output_file[255];
int  pathlen;
char relative_path[]="../../../example_internal/screenshot_to_bitmap/screenshot_to_bitmap_16bpp/";

    generate_screen_shot();

    gx_validation_extract_path(__FILE__, golden_file, &pathlen);
    memcpy(output_file, golden_file, pathlen);

    sprintf(golden_file + pathlen, "%s%s", relative_path, golden_bmp_file);
    sprintf(output_file + pathlen, "%s%s", relative_path, bmp_file);
    status = gx_validation_compare_file_data(output_file, golden_file);
    remove(output_file);

    sprintf(golden_file + pathlen, "%s%s", relative_path, scroll_wheel_golden_bmp_file);
    sprintf(output_file + pathlen, "%s%s", relative_path, scroll_wheel_bmp_file);
    status = gx_validation_compare_file_data(output_file, golden_file);
    remove(output_file);

    if(status == GX_SUCCESS)
    {
        gx_validation_print_test_result(TEST_SUCCESS);
        exit(0);
    }
    else 
    {
        gx_validation_print_test_result(TEST_FAIL);
        exit(1);
    }
}

