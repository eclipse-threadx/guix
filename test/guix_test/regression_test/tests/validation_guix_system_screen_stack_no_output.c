/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_utility.h"

TEST_PARAM test_parameter = {
    "guix_system_screen_stack_no_output", /* Test name */
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
#include "demo_guix_all_widgets.c"


/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */

#define SCREEN_STACK_SIZE 2
GX_WIDGET *screen_stack_memory[SCREEN_STACK_SIZE];

static VOID control_thread_entry(ULONG input)
{
UINT status;
int  failed_count = 0;
GX_WIDGET *parent = (GX_WIDGET *)&button_screen;

    status = gx_system_screen_stack_create(GX_NULL, 0);
    if(status != GX_PTR_ERROR)
    {
        failed_count++;
    }

    status = gx_system_screen_stack_create(screen_stack_memory, 0);
    if(status != GX_INVALID_VALUE)
    {
        failed_count++;
    }

    status = _gxe_system_screen_stack_create(screen_stack_memory, 4, 4);
    if(status != GX_INVALID_SIZE)
    {
        failed_count++;
    }

    status = gx_system_screen_stack_pop();
    if(status != GX_PTR_ERROR)
    {
        failed_count++;
    }

    status = gx_system_screen_stack_get(GX_NULL, GX_NULL);
    if(status != GX_PTR_ERROR)
    {
        failed_count++;
    }

    status = gx_system_screen_stack_push(GX_NULL);
    if(status != GX_PTR_ERROR)
    {
        failed_count++;
    }

    gx_system_screen_stack_create(screen_stack_memory, sizeof(GX_WIDGET *) * SCREEN_STACK_SIZE);
    status = gx_system_screen_stack_pop();
    if(status != GX_FAILURE)
    {
        failed_count++;
    }

    gx_system_screen_stack_push((GX_WIDGET *)&window_screen);
    status = gx_system_screen_stack_push((GX_WIDGET *)&button_screen);
    if(status != GX_FAILURE)
    {
        failed_count++;
    }

    gx_system_screen_stack_get(&parent, GX_NULL);
    if(parent != GX_NULL)
    {
        failed_count++;
    }

    gx_system_screen_stack_push((GX_WIDGET *)&window_screen);
    gx_system_screen_stack_pop();

    if(failed_count == 0)
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

