/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_string_scroll_wheel_string_list_set_no_output", /* Test name */
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
    /* Create a dedicated thread to perform various operations
       on the pixelmap drawing example. These operations simulate 
       user input. */
    gx_validation_control_thread_create(control_thread_entry);

    gx_validation_application_define(first_unused_memory);
    
    /* Termiante the test if it runs for more than 100 ticks */
    /* This function is not implemented yet. */
    gx_validation_watchdog_create(100);
}

#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"
#include "demo_guix_all_widgets.c"

#define TEST_STRING_LIST_SIZE 2
GX_CONST GX_STRING test_string_list_1[TEST_STRING_LIST_SIZE]={
{GX_NULL, 0},
{"test", 5}
};

GX_CONST GX_STRING test_string_list_2[TEST_STRING_LIST_SIZE]={
{"string1", 7},
{"string2", 6}
};

GX_CONST GX_STRING test_valid_string_list[TEST_STRING_LIST_SIZE]={
{"string 1", sizeof("string 1") - 1},
{"string 2", sizeof("string 2") - 1}
};

GX_CONST GX_CHAR *test_valid_old_string_list[TEST_STRING_LIST_SIZE]={
"old string 1",
"old string 2"
};

GX_RESOURCE_ID test_string_id_list[TEST_STRING_LIST_SIZE] = {
1,
2
};

VOID *test_memory_allocate(ULONG size)
{
    return GX_NULL;
}

static VOID control_thread_entry(ULONG input)
{
int                    failed_tests = 0;
UINT                   status;
GX_STRING_SCROLL_WHEEL wheel;
GX_RECTANGLE           size;

    /* Call with Null pointer widget. */
    status = gx_string_scroll_wheel_string_list_set_ext(GX_NULL, GX_NULL, 0);
    EXPECT_EQ(GX_PTR_ERROR, status);

    /* Call with invalid widget. */
    memset(&wheel, 0, sizeof(GX_STRING_SCROLL_WHEEL));
    status = gx_string_scroll_wheel_string_list_set_ext(&wheel, test_string_list_1, TEST_STRING_LIST_SIZE);
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    /* Create string scroll wheel for testing. */
    gx_utility_rectangle_define(&size, 0, 0, 100, 100);
    status = gx_string_scroll_wheel_create_ext(&wheel, "test_wheel", NULL, 0, GX_NULL, GX_STYLE_ENABLED, 1024, &size);
    EXPECT_EQ(GX_SUCCESS, status);

    /* Set string list that contains invalid string. */
    status = gx_string_scroll_wheel_string_list_set_ext(&wheel, test_string_list_1, TEST_STRING_LIST_SIZE);
    EXPECT_EQ(GX_INVALID_STRING_LENGTH, status);

    /* Set string list that contains invalid string. */
    status = gx_string_scroll_wheel_string_list_set_ext(&wheel, test_string_list_2, TEST_STRING_LIST_SIZE);
    EXPECT_EQ(GX_INVALID_STRING_LENGTH, status);

    /* Set string list with invalid string list count. */
    status = gx_string_scroll_wheel_string_list_set_ext(&wheel, test_string_list_2, -1);
    EXPECT_EQ(GX_INVALID_VALUE, status);

    /* Set null pointer string list. */
    status = gx_string_scroll_wheel_string_list_set_ext(&wheel, GX_NULL, 0);
    EXPECT_EQ(GX_SUCCESS, status);

    /* Added text copy style. */
    gx_widget_style_add(&wheel, GX_STYLE_TEXT_COPY);

    /* Set new version string list. */
    status = gx_string_scroll_wheel_string_list_set_ext(&wheel, test_valid_string_list, TEST_STRING_LIST_SIZE);
    EXPECT_EQ(GX_SUCCESS, status);

    /* Set memory allocator and free funciton to NULL. */
    gx_system_memory_allocator_set(GX_NULL, GX_NULL);

#ifdef GX_ENABLE_DEPRECATED_STRING_API
    /* Set old version string list. */
    status = gx_string_scroll_wheel_string_list_set(&wheel, test_valid_old_string_list, TEST_STRING_LIST_SIZE);
    EXPECT_EQ(GX_SYSTEM_MEMORY_ERROR, status);
#endif

    /* Set string id list. */
    status = gx_string_scroll_wheel_string_id_list_set(&wheel, test_string_id_list, TEST_STRING_LIST_SIZE);
    EXPECT_EQ(GX_SYSTEM_MEMORY_ERROR, status);

#ifdef GX_ENABLE_DEPRECATED_STRING_API
    /* Set valid memory allocator and free function, and set old version string list. */
    gx_system_memory_allocator_set(rotate_memory_allocate, rotate_memory_free);
    status = gx_string_scroll_wheel_string_list_set(&wheel, test_valid_old_string_list, TEST_STRING_LIST_SIZE);
    EXPECT_EQ(GX_SUCCESS, status);

    /* Set memory allocator and free function to NULL, and set new version string list. */
    gx_system_memory_allocator_set(GX_NULL, GX_NULL);
    status = gx_string_scroll_wheel_string_list_set_ext(&wheel, test_valid_string_list, TEST_STRING_LIST_SIZE);
    EXPECT_EQ(GX_SYSTEM_MEMORY_ERROR, status);

    /* Set string id list. */
    status = gx_string_scroll_wheel_string_id_list_set(&wheel, test_string_id_list, TEST_STRING_LIST_SIZE);
    EXPECT_EQ(GX_SYSTEM_MEMORY_ERROR, status);
#endif 

    gx_system_memory_allocator_set(test_memory_allocate, rotate_memory_free);
    status = gx_string_scroll_wheel_string_list_set_ext(&wheel, test_valid_string_list, TEST_STRING_LIST_SIZE);
    EXPECT_EQ(GX_SYSTEM_MEMORY_ERROR, status);

    /* Set valid memory allocate and free function. */
    gx_system_memory_allocator_set(rotate_memory_allocate, rotate_memory_free);

    /* Set new version string list. */
    status = gx_string_scroll_wheel_string_list_set_ext(&wheel, test_valid_string_list, TEST_STRING_LIST_SIZE);
    EXPECT_EQ(GX_SUCCESS, status);

    /* Delete string scroll wheel widget. */
    status = gx_widget_delete(&wheel);
    EXPECT_EQ(GX_SUCCESS, status);

    /* Test if string list been freed. */
    if(wheel.gx_string_scroll_wheel_string_list != GX_NULL)
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
