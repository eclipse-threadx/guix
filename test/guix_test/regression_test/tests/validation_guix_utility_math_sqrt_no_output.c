/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_utility_math_sqrt_no_output", /* Test name */
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

    /* Create a dedicated thread to perform various operations
       on the pixelmap drawing example. These operations simulate 
       user input. */
    gx_validation_control_thread_create(control_thread_entry);
       
}

#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"
#include "demo_guix_all_widgets.c"


/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */

static VOID control_thread_entry(ULONG input)
{
UINT return_value;
UINT status = GX_SUCCESS;

    return_value = gx_utility_math_sqrt(0x7FFEA811);
    if (return_value != 0xB504) status |= GX_FAILURE;
    return_value = gx_utility_math_sqrt(0x40000001);
    if (return_value != 0x8000) status |= GX_FAILURE;
    return_value = gx_utility_math_sqrt(0x10000001);
    if (return_value != 0x4000) status |= GX_FAILURE;
    return_value = gx_utility_math_sqrt(0x4000001);
    if (return_value != 0x2000) status |= GX_FAILURE;
    
    return_value = gx_utility_math_sqrt(0x101);
    if (return_value != 0x10) status |= GX_FAILURE;

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

