/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_system.h"
#include "gx_widget.h"

TEST_PARAM test_parameter = {
    "guix_root_window_focus_no_output", /* Test name */
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
       on the pixelmap drawing example. These operations simulate 
       user input. */
    gx_validation_control_thread_create(control_thread_entry);

}

#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"
#include "demo_guix_all_widgets.c"

static VOID control_thread_entry(ULONG input)
{
GX_BOOL test_successed = GX_TRUE;

    /* Toggle to window screen. */
    ToggleScreen((GX_WINDOW *)&window_screen, (GX_WINDOW *)&button_screen);

    /* Set focus to nested window that attached to the nested root. */
    gx_system_focus_claim(&NestedWindow);

    if(root->gx_widget_status & GX_STATUS_HAS_FOCUS)
    {
        PRINT_ERROR("GX_STATUS_HAS_FOCUS status should not be set to root.");
        test_successed = GX_FALSE;
    }

    /* Set focus to window screen that attached to the Primary root. */
    gx_system_focus_claim(&window_screen);

    if(!(root->gx_widget_status & GX_STATUS_HAS_FOCUS))
    {
        PRINT_ERROR("GX_STATUS_HAS_FOCUS status should be set to root.")
        test_successed = GX_FALSE;
    }

    
    if(test_successed)
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
