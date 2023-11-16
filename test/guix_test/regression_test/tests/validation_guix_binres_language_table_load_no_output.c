/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_system.h"

TEST_PARAM test_parameter = {
    "guix_binres_language_table_load_no_output", /* Test name */
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

static VOID control_thread_entry(ULONG input)
{
int         failed_tests = 0;
UINT        status;
GX_STRING **language_table;
GX_UBYTE   *root_address = (GX_UBYTE *)0x6000000;

    status = gx_binres_language_table_load_ext(GX_NULL, &language_table);
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_binres_language_table_load_ext(root_address, GX_NULL);
    EXPECT_EQ(GX_PTR_ERROR, status);

    gx_system_memory_allocator_set(rotate_memory_allocate, GX_NULL);
    status = gx_binres_language_table_load_ext(root_address, &language_table);
    EXPECT_EQ(GX_SYSTEM_MEMORY_ERROR, status);

    gx_system_memory_allocator_set(GX_NULL, rotate_memory_free);
    status = gx_binres_language_table_load_ext(root_address, &language_table);
    EXPECT_EQ(GX_SYSTEM_MEMORY_ERROR, status);

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
