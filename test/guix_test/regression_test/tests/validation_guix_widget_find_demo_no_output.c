/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_widget.h"
#include "gx_system.h"

TEST_PARAM test_parameter = {
    "guix_widget_find_demo_no_output", /* Test name */
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
#include "demo_widget_find.c"

typedef struct TEST_STRUCT{
    GX_WIDGET *widget;
    INT        widget_id;
}TEST;

TEST test_list[]={
    {(GX_WIDGET *)&main_screen.main_screen_window_1,       ID_1},
    {(GX_WIDGET *)&main_screen.main_screen_window_1_1,     ID_1_1},
    {(GX_WIDGET *)&main_screen.main_screen_window_1_1_1,   ID_1_1_1},
    {(GX_WIDGET *)&main_screen.main_screen_prompt_1_1_1_1, ID_1_1_1_1},
    {(GX_WIDGET *)&main_screen.main_screen_prompt_1_1_1_2, ID_1_1_1_2},
    {(GX_WIDGET *)&main_screen.main_screen_prompt_1_1_1_3, ID_1_1_1_3},
    {(GX_WIDGET *)&main_screen.main_screen_window_2,       ID_2},
    {(GX_WIDGET *)&main_screen.main_screen_window_2_1,     ID_2_1},
    {(GX_WIDGET *)&main_screen.main_screen_window_2_1_1,   ID_2_1_1},
    {(GX_WIDGET *)&main_screen.main_screen_prompt_2_1_1_1, ID_2_1_1_1},
    {(GX_WIDGET *)&main_screen.main_screen_prompt_2_1_1_2, ID_2_1_1_2},
    {(GX_WIDGET *)&main_screen.main_screen_window_2_1_2,   ID_2_1_2},
    {(GX_WIDGET *)&main_screen.main_screen_window_4,       ID_4},
    {(GX_WIDGET *)&main_screen.main_screen_prompt_4_1,     ID_4_1},
    {(GX_WIDGET *)&main_screen.main_screen_prompt_4_2,     ID_4_2},
    {GX_NULL, 0}
};

static VOID control_thread_entry(ULONG input)
{
INT        failed_tests = 0;
GX_WIDGET *find;
TEST      *test = test_list;

    while(test->widget)
    {
        /* Test _gx_widget_text_draw. */
        gx_widget_find(&main_screen, test->widget_id, GX_SEARCH_DEPTH_INFINITE, &find);
        if(test->widget != find)
        {
            printf("\nERROR! File: %s Line: %d\n", __FILE__, __LINE__); 
            printf("Wrong widget founded!");                            
            failed_tests++;                                             
        }
        test++;
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
