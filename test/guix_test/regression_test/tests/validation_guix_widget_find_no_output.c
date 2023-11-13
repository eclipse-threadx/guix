/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_widget.h"
#include "gx_system.h"

TEST_PARAM test_parameter = {
    "guix_widget_find_no_output", /* Test name */
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
INT        failed_tests = 0;
GX_WIDGET *find;

    /* Test _gx_widget_text_draw. */
    gx_widget_find(&button_screen, ID_TEXT_BUTTON, GX_SEARCH_DEPTH_INFINITE, &find);
    EXPECT_EQ((GX_WIDGET *)&button_screen.button_screen_text_button_1, find);

    gx_widget_find(&window_screen, ID_NESTED_WINDOW_1, 0, &find);
    EXPECT_EQ(GX_NULL, find);

    gx_widget_find(&window_screen, ID_NESTED_WINDOW_1, GX_SEARCH_DEPTH_INFINITE, &find);
    EXPECT_EQ((GX_WIDGET *)&window_screen.window_screen_window_6, find);

    gx_widget_find(&window_screen, ID_NESTED_WINDOW_2, 1, &find);
    EXPECT_EQ(GX_NULL, find);

    gx_widget_find(&window_screen, ID_NESTED_WINDOW_2, GX_SEARCH_DEPTH_INFINITE, &find);
    EXPECT_EQ((GX_WIDGET *)&window_screen.window_screen_window_8, find);

    gx_widget_find(&window_screen, ID_BUTTON_1, GX_SEARCH_DEPTH_INFINITE, &find);
    EXPECT_EQ((GX_WIDGET *)&window_screen.window_screen_button_1, find);

    gx_widget_find(&window_screen, ID_BUTTON_2, 0, &find);
    EXPECT_EQ(GX_NULL, find);

    gx_widget_find(&window_screen, ID_BUTTON_4, GX_SEARCH_DEPTH_INFINITE, &find);
    EXPECT_EQ((GX_WIDGET *)&window_screen.window_screen_button_4, find);

    gx_widget_find(&window_screen, ID_DROP_LIST_BUTTON, GX_SEARCH_DEPTH_INFINITE, &find);
    EXPECT_EQ((GX_WIDGET *)&window_screen.window_screen_pixelmap_button, find);

    gx_widget_find(&window_screen, GX_ID_VERTICAL_SCROLL, 1, &find);
    EXPECT_EQ((GX_WIDGET *)&window_screen.window_screen_vertical_scroll_1, find);
    
    gx_widget_find(&window_screen, GX_ID_HORIZONTAL_SCROLL, 1, &find);
    EXPECT_EQ((GX_WIDGET *)&window_screen.window_screen_hscroll_1, find);

    gx_widget_style_add(&menu_screen.menu_screen_ola_2_menu_2_1, GX_STYLE_MENU_EXPANDED);
    ToggleScreen((GX_WINDOW *)&menu_screen, (GX_WINDOW *)&button_screen);

    gx_widget_find(&menu_screen, ID_MENU_2_1_PROMPT, GX_SEARCH_DEPTH_INFINITE, &find);
    EXPECT_EQ((GX_WIDGET *)&menu_screen.menu_screen_prompt_9, find);

    gx_widget_find(&menu_screen, ID_TEXT_BUTTON, GX_SEARCH_DEPTH_INFINITE, &find);
    EXPECT_EQ(GX_NULL, find);
    
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
