/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_system.h"

TEST_PARAM test_parameter = {
    "guix_system_free_view_get_no_output", /* Test name */
    0, 0, 0, 0  /* Define the coordinates of the capture area. */
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
INT        failed_tests = 0;
GX_WINDOW  window[GX_MAX_VIEWS / 2];
int        width;
int        height;
GX_RECTANGLE size;
int        widget_width;
int        widget_height;
int        x_count;
int        y_count;
int        x, y;
int        index = 0;
GX_EVENT   my_event;

    gx_widget_detach(&button_screen);
    gx_widget_width_get(root, (GX_VALUE *)&width);
    gx_widget_height_get(root, (GX_VALUE *)&height);

    width -= 20;
    height -= 20;

    y_count = 4;
    x_count = GX_MAX_VIEWS / 2 / y_count;

    widget_width = width / x_count;
    widget_height = height / y_count;

    for(y = 0; y < y_count; y++)
    {
        size.gx_rectangle_top = 10 + y * widget_height;
        size.gx_rectangle_bottom = size.gx_rectangle_top + widget_height - 10;
        for(x = 0; x < x_count; x++)
        {
            size.gx_rectangle_left = 10 + y + x * widget_width;
            size.gx_rectangle_right = size.gx_rectangle_left + widget_width - 10;
            
            gx_window_create(&window[index], "test_window", root, 0, 1024 + index, &size);      
            index++;
        }
    }

    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_type = GX_EVENT_SHOW;
    my_event.gx_event_target = (GX_WIDGET *)root;
    gx_system_event_send(&my_event);

    EXPECT_EQ(GX_SYSTEM_OUT_OF_VIEWS, _gx_system_last_error);

    if(!failed_tests)
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
