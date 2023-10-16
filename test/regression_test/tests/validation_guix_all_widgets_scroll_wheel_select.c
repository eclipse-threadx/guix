/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_system.h"

TEST_PARAM test_parameter = {
    "guix_all_widgets_scroll_wheel_select", /* Test name */
    100, 117, 525, 336  /* Define the coordinates of the capture area.
                         In this test, we only need to capture the line
                         drawing area.  */
};

static VOID      control_thread_entry(ULONG);
int main(int argc, char ** argv)
{
    /* Parse the command line argument. */
    gx_validation_setup(argc, argv);

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
#ifdef win32_graphics_driver_setup_24xrgb  
#undef win32_graphics_driver_setup_24xrgb  
#endif
#define win32_graphics_driver_setup_24xrgb  gx_validation_graphics_driver_setup_24xrgb


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_guix_all_widgets.c"

typedef struct TEST_STRUCT{
INT select_index;
INT remove_style;
INT total_rows;
GX_CHAR *comment;
}TEST;

TEST test_list[]={
    {32, 0,             0, "set selected row to 32"},
    {0,  0,            30, "set total rows to 30"},
    {-1, 0,             0, "set selected row to -1"},
    {9,  GX_STYLE_WRAP, 0, "remove wrap style, set selected row to 9"},
    {5,  0,             0, "set selected row to 5"},
    {30, 0,             0, "set selected row to 30"},
    {0,  0,            29, "set total rows to 29"},
    {0,  0, 0, 0}, 
};

char *test_comment[1];

TX_SEMAPHORE semaphore_memory;

static VOID control_thread_entry(ULONG input)
{
INT                      frame_id = 1;
GX_NUMERIC_SCROLL_WHEEL *day_wheel;
TEST                    *test = test_list;

    selected_semaphore = &semaphore_memory;

    tx_semaphore_create(selected_semaphore, "test_semaphore", 0);

    /* Create scroll wheel screen dynamically. */
    gx_studio_named_widget_create("scroll_wheel_screen", GX_NULL, (GX_WIDGET **)&pScrollWheelScreen);

    /* Toggle from button screen to scroll wheel screen. */
    ToggleScreen((GX_WINDOW *)pScrollWheelScreen, pButtonScreen);

    /* Search 3 wheel widget. */
    gx_widget_find(pScrollWheelScreen, ID_DAY_WHEEL, GX_SEARCH_DEPTH_INFINITE, &day_wheel);

    while(test->comment)
    {
        test_comment[0] = test->comment;
        gx_validation_current_frame_id_get(&frame_id);
        gx_validation_capture_frames(frame_id, 40, test_comment, 1, 1000);

        if(test->remove_style)
        {
            gx_widget_style_remove((GX_WIDGET *)day_wheel, GX_STYLE_WRAP);
        }

        if(test->total_rows)
        {
            gx_scroll_wheel_total_rows_set((GX_SCROLL_WHEEL *)day_wheel, test->total_rows);
        }
        else
        {
            gx_scroll_wheel_selected_set(day_wheel, test->select_index);
        }
        tx_semaphore_get(selected_semaphore, TX_WAIT_FOREVER);
        test++;
    }

    tx_semaphore_delete(selected_semaphore);
    selected_semaphore = GX_NULL;

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}

