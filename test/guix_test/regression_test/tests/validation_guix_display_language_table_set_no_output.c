/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_system.h"

TEST_PARAM test_parameter = {
    "guix_display_language_table_set_no_output", /* Test name */
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

extern GX_FONT _gx_system_font_8bpp;

#define STRING_TABLE_SIZE 2
#define LANGUAGE_TABLE_SIZE 1

GX_CONST GX_CHAR string[]="test";
GX_CONST GX_STRING string_table_1[STRING_TABLE_SIZE]={
{GX_NULL, 0},
{string, sizeof(string) - 2}
};

GX_CONST GX_STRING *language_table_1[LANGUAGE_TABLE_SIZE]={
string_table_1
};

GX_CONST GX_STRING string_table_2[STRING_TABLE_SIZE]={
{GX_NULL, 0},
{string, sizeof(string)}
};

GX_CONST GX_STRING *language_table_2[LANGUAGE_TABLE_SIZE]={
string_table_2
};

GX_CONST GX_STRING string_table_3[STRING_TABLE_SIZE]={
{GX_NULL, 0},
{string, sizeof(string) - 1}
};

GX_CONST GX_STRING *language_table_3[LANGUAGE_TABLE_SIZE]={
string_table_3
};

static VOID control_thread_entry(ULONG input)
{
int         failed_tests = 0;
UINT        status;
GX_DISPLAY *display = root->gx_window_root_canvas->gx_canvas_display;

    status = gx_display_language_table_set_ext(GX_NULL, language_table_1, LANGUAGE_TABLE_SIZE, STRING_TABLE_SIZE);
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_display_language_table_set_ext(display, GX_NULL, LANGUAGE_TABLE_SIZE, STRING_TABLE_SIZE);
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_display_language_table_set_ext(display, language_table_1, LANGUAGE_TABLE_SIZE, STRING_TABLE_SIZE);
    EXPECT_EQ(GX_INVALID_STRING_LENGTH, status);

    status = gx_display_language_table_set_ext(display, language_table_2, LANGUAGE_TABLE_SIZE, STRING_TABLE_SIZE);
    EXPECT_EQ(GX_INVALID_STRING_LENGTH, status);

    status = gx_display_language_table_set_ext(display, GX_NULL, 0, 0);
    EXPECT_EQ(GX_SUCCESS, status);

    status = gx_display_language_table_set_ext(display, language_table_3, LANGUAGE_TABLE_SIZE, STRING_TABLE_SIZE);
    EXPECT_EQ(GX_SUCCESS, status);

    gx_widget_hide(root);
    root->gx_window_root_canvas = GX_NULL;
    status = gx_display_language_table_set_ext(display, GX_NULL, 0, 0);
    EXPECT_EQ(GX_SUCCESS, status);

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
