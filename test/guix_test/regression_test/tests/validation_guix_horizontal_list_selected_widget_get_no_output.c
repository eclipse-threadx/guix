/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_horizontal_list_selected_widget_get_no_output", /* Test name */
    0, 0, 0, 0  /* Define the coordinates of the capture area.*/
};

static VOID      control_thread_entry(ULONG);
int main(int argc, char ** argv)
{
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


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_guix_all_widgets.c"


static VOID control_thread_entry(ULONG input)
{
GX_HORIZONTAL_LIST *hlist;
GX_WIDGET         *child;
UINT     status;

    hlist = &window_screen.window_screen_horizontal_list_1;
    
    while(hlist->gx_widget_first_child)
    {
        gx_widget_detach(hlist->gx_widget_first_child);
        
        status = gx_horizontal_list_selected_widget_get(hlist, &child);
        if((status == GX_SUCCESS) && (hlist -> gx_widget_first_child)) continue;
        if((status == GX_FAILURE) && (hlist -> gx_widget_first_child == GX_NULL)) continue;
    
        gx_validation_print_test_result(TEST_FAIL);    
        exit(1);

    }
   
    gx_validation_print_test_result(TEST_SUCCESS); 

    exit(0);
}
