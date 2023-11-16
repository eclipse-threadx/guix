/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_system.h"

TEST_PARAM test_parameter = {
    "guix_deprecated_system_api_no_output", /* Test name */
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

#define TEST_LANGUAGE_TABLE_SIZE 2
#define TEST_STRING_TABLE_SIZE   3

extern GX_CONST GX_UBYTE *Primary_English_string_table[];

GX_CONST GX_UBYTE test_string_1[]="test string 1";
GX_CONST GX_UBYTE test_string_2[]="test string 2";

GX_CONST GX_UBYTE *test_string_table[]=
{
GX_NULL,
test_string_1,
test_string_2
};

GX_CONST GX_UBYTE **test_language_table[TEST_LANGUAGE_TABLE_SIZE] =
{
test_string_table,
test_string_table
};

extern GX_CONST GX_UBYTE *Primary_English_string_table[];

static VOID control_thread_entry(ULONG input)
{
UINT        failed_tests = 0;
UINT        status;
GX_DISPLAY *display = root->gx_window_root_canvas->gx_canvas_display;
GX_CHAR  ***language_table;
GX_CHAR   **string_table;
GX_UBYTE    language_count;
UINT        string_count;
GX_CONST GX_CHAR    *string;

    /* Test _gx_system_language_table_set. */
    _gx_system_language_table_set((GX_CHAR ***)test_language_table, TEST_LANGUAGE_TABLE_SIZE, TEST_STRING_TABLE_SIZE);

    /* Test _gx_system_language_table_get. */
    status = _gx_system_language_table_get(&language_table, &language_count, &string_count);
    EXPECT_EQ(GX_SUCCESS, status);
    EXPECT_EQ(TEST_LANGUAGE_TABLE_SIZE, language_count);
    EXPECT_EQ(TEST_STRING_TABLE_SIZE, string_count);

    /* Test _gx_system_active_language_set. */
    status = _gx_system_active_language_set(1);
    EXPECT_EQ(GX_SUCCESS, status);
    EXPECT_EQ(1, display->gx_display_active_language);

    /* Test _gx_system_string_get. */
    status = _gx_system_string_get(1, &string);
    EXPECT_EQ(GX_SUCCESS, status);
    if(strcmp(string, (char *)test_string_1) != 0)
    {
        failed_tests++;
    }

    /* Test _gx_system_string_table_get. */
    status = _gx_system_string_table_get(0, &string_table, &string_count);
    EXPECT_EQ(GX_SUCCESS, status);
    EXPECT_EQ(TEST_STRING_TABLE_SIZE, string_count);

    _gx_system_display_created_list = GX_NULL;
    status = _gx_system_language_table_set((GX_CHAR ***)test_language_table, TEST_LANGUAGE_TABLE_SIZE, TEST_STRING_TABLE_SIZE);
    EXPECT_EQ(GX_FAILURE, status);

    status = _gx_system_string_get(1, &string);
    EXPECT_EQ(GX_FAILURE, status);

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
