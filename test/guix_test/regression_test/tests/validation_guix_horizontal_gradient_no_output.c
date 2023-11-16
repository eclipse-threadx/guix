/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_horizontal_gradient_no_output", /* Test name */
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
GX_GRADIENT horizontal_gradient;
GX_GRADIENT horizontal_gradient1;

INT width = 100; /* set width to 100 to test. */

    horizontal_gradient.gx_gradient_alpha_start = 0;
    horizontal_gradient.gx_gradient_alpha_end = 255;
    horizontal_gradient1.gx_gradient_alpha_start = 0;
    horizontal_gradient1.gx_gradient_alpha_end = 255;
    
    /* Create horizontal gradient */
    /* error checking function gx_utility_gradient_create isn't correct for now. So call function _gx_utility_gradient_create directly.
       This should be fixed while _gxe_utility_gradient_create is fixed. */
    gx_utility_gradient_create(&horizontal_gradient, 1,  3,  GX_GRADIENT_TYPE_ALPHA, horizontal_gradient.gx_gradient_alpha_start, horizontal_gradient.gx_gradient_alpha_start); //should return GX_INVALID_SIZE
    gx_utility_gradient_create(&horizontal_gradient, (GX_VALUE)width,  3,  GX_GRADIENT_TYPE_MIRROR, horizontal_gradient.gx_gradient_alpha_start, horizontal_gradient.gx_gradient_alpha_start); //should return GX_NOT_SUPPORTED
    
    gx_utility_gradient_create(&horizontal_gradient, (GX_VALUE)width,  3,  GX_GRADIENT_TYPE_ALPHA, horizontal_gradient.gx_gradient_alpha_start, horizontal_gradient.gx_gradient_alpha_start);//should return GX_SUCCESS
    width = 2;
    gx_utility_gradient_create(&horizontal_gradient1, (GX_VALUE)width,  3,  GX_GRADIENT_TYPE_ALPHA|GX_GRADIENT_TYPE_MIRROR, horizontal_gradient.gx_gradient_alpha_start, horizontal_gradient.gx_gradient_alpha_start);//should return GX_SUCCESS

    gx_validation_print_test_result(TEST_SUCCESS);

    exit(0);
}

