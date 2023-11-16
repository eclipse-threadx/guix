/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_horizontal_list_children_position_no_output", /* Test name */
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

#include "demo_guix_horizontal_list_32bpp.c"

#define TEST_LIST_ID  1024
#define TEST_CHILD_ID 1025

int select_count = 0;
UINT test_event_process(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    switch(event_ptr->gx_event_type)
    {
    case GX_SIGNAL(TEST_LIST_ID, GX_EVENT_LIST_SELECT):
        select_count++;
        break;   
    }

    return gx_window_event_process(window, event_ptr);
}
static VOID control_thread_entry(ULONG input)
{
GX_BOOL             test_successed = GX_TRUE;
GX_HORIZONTAL_LIST *list = &window.window_horizontal_list;
GX_WIDGET          *child = (GX_WIDGET *)&window.window_button_1;

    child->gx_widget_id = TEST_CHILD_ID;
    gx_horizontal_list_children_position(&window.window_horizontal_list);

    if(child->gx_widget_id != TEST_CHILD_ID)
    {
        test_successed = GX_FALSE;
    }

    list->gx_widget_id = TEST_LIST_ID;
    gx_widget_event_process_set(&window, test_event_process);
    gx_horizontal_list_selected_set(list, 2);

    gx_horizontal_list_total_columns_set(list, 0);
    gx_horizontal_list_selected_set(list, 0);
    if(select_count != 1)
    {
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
