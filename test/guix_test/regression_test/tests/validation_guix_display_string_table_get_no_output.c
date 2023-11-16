/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_display_string_table_get_no_output", /* Test name */
    0, 0, 0, 0  /* Define the coordinates of the capture area.*/
};

static VOID      control_thread_entry(ULONG);

int main(int argc, char ** argv)
{
  tx_kernel_enter();
  return(0);
}

VOID tx_application_define(void *first_unused_memory)
{

    /* Create a dedicated thread to perform various operations
       on the line drawing example. These operations simulate 
       user input. */
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

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */

static VOID control_thread_entry(ULONG input)
{
INT         failed_tests = 0;
UINT        status;
#ifdef GX_ENABLE_DEPRECATED_STRING_API
GX_CHAR   **table = GX_NULL;
#endif
GX_STRING  *table_ext = GX_NULL;
UINT        table_size;
GX_DISPLAY *display;
GX_DISPLAY  new_display;
    
    display = root->gx_window_root_canvas->gx_canvas_display;

#ifdef GX_ENABLE_DEPRECATED_STRING_API
    status = gx_display_string_table_get(display, LANGUAGE_ENGLISH, &table, &table_size);
    EXPECT_EQ(GX_SUCCESS, status);
    EXPECT_EQ(table_size, display->gx_display_string_table_size);
    
    status = gx_display_create(&new_display, "new_display", gx_validation_display_driver_setup_24xrgb, 10, 10);
    EXPECT_EQ(GX_SUCCESS, status);
 
    new_display.gx_display_language_table_size = 2;
    status = gx_display_string_table_get(&new_display, LANGUAGE_ENGLISH, &table, &table_size);
    EXPECT_EQ(GX_SUCCESS, status);
    EXPECT_EQ(GX_NULL, table_size);
#endif

    /* Test _gx_display_string_table_get_ext. */
    status = gx_display_string_table_get_ext(GX_NULL, LANGUAGE_ENGLISH, &table_ext, &table_size);
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_display_string_table_get_ext(display, 10, &table_ext, &table_size);
    EXPECT_EQ(GX_NOT_FOUND, status);

    status = gx_display_string_table_get_ext(display, LANGUAGE_ENGLISH, &table_ext, &table_size);
    if((table_ext == GX_NULL) || (table_size != display -> gx_display_string_table_size))
    {
        failed_tests++;
    }

    gx_display_string_table_get_ext(display, LANGUAGE_ENGLISH, GX_NULL, GX_NULL);

    display->gx_display_language_table = GX_NULL;

    gx_display_string_table_get_ext(display, LANGUAGE_ENGLISH, &table_ext, &table_size);

    if(table_ext != GX_NULL || table_size != display->gx_display_string_table_size)
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

