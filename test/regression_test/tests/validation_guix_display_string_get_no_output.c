/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_system.h"

TEST_PARAM test_parameter = {
    "guix_display_string_get_no_output", /* Test name */
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

#ifdef GX_ENABLE_DEPRECATED_STRING_API

#define TEST_STRING_ID_INVALID 1
#define TEST_STRING_ID_NULL    2
#define TEST_STRING_ID_VALID   3
#define TEST_STRING_TABLE_SIZE 4

#define TEST_LANGUAGE_ID_1       0
#define TEST_LANGUAGE_ID_2       1
#define TEST_LANGUAGE_TABLE_SIZE 2

GX_CHAR  test_string_invalid[GX_MAX_STRING_LENGTH + 1];
GX_CHAR *test_string_table_1[]={
GX_NULL,
"string 1",
"string 2",
"string 3"
};

GX_STRING test_string_table_ext_1[]={
{GX_NULL, 0},
{"string 1", sizeof("string 1") - 1},
{"string 2", sizeof("string 2") - 1},
{"string 3", sizeof("string 3") - 1}
};

GX_CHAR *test_string_table_2[]={
GX_NULL,
test_string_invalid,
GX_NULL,
"table 2 string 3"
};

GX_STRING test_string_table_ext_2[]={
{GX_NULL, 0},
{"table 2 string 1", sizeof("table 2 string 1") - 1},
{GX_NULL, 0},
{"table 2 string 3", sizeof("table 2 string 3") - 1}
};

GX_CHAR **test_language_table[]={
test_string_table_1,
test_string_table_2
};

GX_STRING *test_language_table_ext[]={
test_string_table_ext_1,
test_string_table_ext_2
};
#endif

static VOID control_thread_entry(ULONG input)
{
int         failed_tests = 0;
UINT        status;
GX_STRING   string;
GX_DISPLAY *display = root->gx_window_root_canvas->gx_canvas_display;

    /* Test invalid pointers. */
    status = gx_display_string_get_ext(GX_NULL, 0, &string);
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_display_string_get_ext(display, 0, GX_NULL);
    EXPECT_EQ(GX_PTR_ERROR, status);

    /* Test invalid resource id. */
    status = gx_display_string_get_ext(display, 1024, &string);
    EXPECT_EQ(GX_INVALID_RESOURCE_ID, status);

    status = gx_display_string_get_ext(display, 0, &string);
    EXPECT_EQ(GX_SUCCESS, status);

#ifdef GX_ENABLE_DEPRECATED_STRING_API
    /* Test invalid pointers. */
    status = gx_display_string_get(0, 0, 0);
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_display_string_get(display, 0, 0);
    EXPECT_EQ(GX_PTR_ERROR, status);
   
    status = gx_system_string_get(0, 0);
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_system_string_get(0, &string.gx_string_ptr);
    EXPECT_EQ(GX_SUCCESS, status);
 
    /* Test invalid resource id*/
    status = gx_display_string_get(display, 1024, &string.gx_string_ptr);
    EXPECT_EQ(GX_INVALID_RESOURCE_ID, status);
 
    status = gx_display_string_get(display, 2, &string.gx_string_ptr);
    EXPECT_EQ(GX_SUCCESS, status);

    memset(test_string_invalid, 'x', GX_MAX_STRING_LENGTH + 1);

    /* Set new version language table. */
    status = gx_display_language_table_set_ext(display, (GX_CONST GX_STRING **)test_language_table_ext, TEST_LANGUAGE_TABLE_SIZE, TEST_STRING_TABLE_SIZE);
    EXPECT_EQ(GX_SUCCESS, status);

    /* Set active language. */
    status = gx_system_active_language_set(TEST_LANGUAGE_ID_2);
    EXPECT_EQ(GX_SUCCESS, status);

    /* Test get a string that is null pointer in active string table. */
    status = gx_display_string_get(display, TEST_STRING_ID_NULL, &string.gx_string_ptr);
    if(strcmp(string.gx_string_ptr, test_string_table_ext_1[TEST_STRING_ID_NULL].gx_string_ptr) != 0)
    {
        failed_tests++;
    }
  
    /* Test get a string from active string table. */
    status = gx_display_string_get(display, TEST_STRING_ID_VALID, &string.gx_string_ptr);
    if(strcmp(string.gx_string_ptr, test_string_table_ext_2[TEST_STRING_ID_VALID].gx_string_ptr) != 0)
    {
        failed_tests++;
    }
 
    /* Set old version language table that contains invalid string. */
    status = gx_display_language_table_set(display, (GX_CHAR ***)test_language_table, TEST_LANGUAGE_TABLE_SIZE, TEST_STRING_TABLE_SIZE);
    EXPECT_EQ(GX_SUCCESS, status);

    /* Test invalid string with extended API. */
    status = gx_display_string_get_ext(display, TEST_STRING_ID_INVALID, &string);
    EXPECT_EQ(GX_INVALID_STRING_LENGTH, status);

    /* Test get a string that is null pointer in active string table. */
    status = gx_display_string_get(display, TEST_STRING_ID_NULL, &string.gx_string_ptr);
    if(strcmp(string.gx_string_ptr, test_string_table_1[TEST_STRING_ID_NULL]) != 0)
    {
        failed_tests++;
    }

    /* Set deprecated language table to NULL. */
    display->gx_display_language_table_deprecated = GX_NULL;
    status = gx_display_string_get(display, TEST_STRING_ID_VALID, &string.gx_string_ptr);
    if(string.gx_string_ptr != GX_NULL)
    {
        failed_tests++;
    }
#endif

    /* Set langauge table to null, and call display string get. */
    display->gx_display_language_table = GX_NULL;
    status = gx_display_string_get_ext(display, 1, &string);
    if(string.gx_string_ptr != GX_NULL)
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
