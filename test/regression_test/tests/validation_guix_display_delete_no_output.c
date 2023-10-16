/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_display_delete_no_output", /* Test name */
    0, 0, 0, 0  /* Define the coordinates of the capture area.*/
};

int main(int argc, char ** argv)
{
    /* Start ThreadX system */
    tx_kernel_enter(); 
    return(0);
}

static VOID      control_thread_entry(ULONG);

VOID tx_application_define(void *first_unused_memory)
{
    gx_validation_application_define(first_unused_memory);
    
    /* Termiante the test if it runs for more than 100 ticks */
    /* This function is not implemented yet. */
    gx_validation_watchdog_create(100);

    /* Create a dedicated thread to perform various operations
       on the line drawing example. These operations simulate 
       user input. */
    gx_validation_control_thread_create(control_thread_entry);
}


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"
#include "demo_guix_all_widgets.c"

static VOID cleanup_function(GX_DISPLAY *display){}

static VOID control_thread_entry(ULONG input)
{
int failed_tests = 0;
GX_DISPLAY *display = GX_NULL;
static GX_DISPLAY display1;
static GX_DISPLAY display2;

    display = root->gx_window_root_canvas->gx_canvas_display;
    if(!display) 
    {
        failed_tests++;
    }
    
    if (gx_display_create(&display1, "display1", gx_validation_display_driver_setup_24xrgb, 0, 0) != GX_SUCCESS)
    {
        failed_tests++;
    }
    
    if (gx_display_create(&display2, "display2", gx_validation_display_driver_setup_24xrgb, 0, 0) != GX_SUCCESS)
    {
        failed_tests++;
    }    
    
    if(gx_display_delete(&display1, cleanup_function) != GX_SUCCESS)
    {
        failed_tests++;
    }   
    
    if(gx_display_delete(&display2, cleanup_function) != GX_SUCCESS)
    {
        failed_tests++;
    }   
    
    if(gx_display_delete(display, cleanup_function) != GX_SUCCESS)
    {
        failed_tests++;
    }   

    if(failed_tests == 0) 
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

