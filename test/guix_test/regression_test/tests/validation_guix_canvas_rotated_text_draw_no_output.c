/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_canvas_rotated_text_draw_no_output", /* Test name */
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
int       failed_tests = 0;
UINT      status;
GX_STRING string;
#ifdef GX_ENABLE_DEPRECATED_STRING_API
GX_CHAR   invalid_string[GX_MAX_STRING_LENGTH + 1];
#endif

    /* Initiate drawing context. */
    gx_canvas_drawing_initiate(root->gx_window_root_canvas, &button_screen, &button_screen.gx_widget_size);

#ifdef GX_ENABLE_DEPRECATED_STRING_API
    memset(invalid_string, 'x', GX_MAX_STRING_LENGTH + 1);
    status = gx_canvas_rotated_text_draw(invalid_string, 0, 0, 90);
    EXPECT_EQ(GX_INVALID_STRING_LENGTH, status);
#endif

    status = gx_canvas_rotated_text_draw_ext(GX_NULL, 0, 0, 90);
    EXPECT_EQ(GX_PTR_ERROR, status);

    memset(&string, 0, sizeof(GX_STRING));
    status = gx_canvas_rotated_text_draw_ext(&string, 0, 0, 90);
    EXPECT_EQ(GX_PTR_ERROR, status);

    string.gx_string_ptr = "test";
    string.gx_string_length = 5;
    status = gx_canvas_rotated_text_draw_ext(&string, 0, 0, 90);
    EXPECT_EQ(GX_INVALID_STRING_LENGTH, status);

    string.gx_string_ptr = "test";
    string.gx_string_length = 3;
    status = gx_canvas_rotated_text_draw_ext(&string, 0, 0, 90);
    EXPECT_EQ(GX_INVALID_STRING_LENGTH, status);

    string.gx_string_ptr = "test";
    string.gx_string_length = 4;

    gx_system_memory_allocator_set(rotate_memory_allocate, GX_NULL);
    status = gx_canvas_rotated_text_draw_ext(&string, 0, 0, 90);
    EXPECT_EQ(GX_SYSTEM_MEMORY_ERROR, status);

    gx_system_memory_allocator_set(GX_NULL, rotate_memory_free);
    status = gx_canvas_rotated_text_draw_ext(&string, 0, 0, 90);
    EXPECT_EQ(GX_SYSTEM_MEMORY_ERROR, status);

    /* Complete drawing context. */
    gx_canvas_drawing_complete(root->gx_window_root_canvas, GX_FALSE);

    status = gx_canvas_rotated_text_draw_ext(&string, 0, 0, 90);
    EXPECT_EQ(GX_INVALID_CONTEXT, status);

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
