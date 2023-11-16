/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_system_private_string_copy_no_output", /* Test name */
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
int                    failed_tests = 0;
GX_PROMPT             *prompt = &button_screen.button_screen_title_1;
GX_STRING_SCROLL_WHEEL wheel;
GX_STRING              string;
GX_CHAR               *buffer;
UINT                   status;
GX_RECTANGLE           size;
GX_STRING              string_list_ext[1];
GX_CONST GX_CHAR      *string_list[1];
INT                    string_count;
INT                    string_length;

    gx_widget_style_add(prompt, GX_STYLE_TEXT_COPY);

    /* Test _gx_system_private_string_copy. */

    /* Test string_length > (USHORT)-1. */
    string_length = (USHORT)-1 + 1;
    buffer = malloc(string_length + 1);
    memset(buffer, 'x', string_length);
    buffer[string_length] = '\0';
    
    string.gx_string_length = string_length;
    string.gx_string_ptr = buffer;
    status = gx_prompt_text_set_ext(prompt, &string);
    EXPECT_EQ(GX_MATH_OVERFLOW, status);

    /* Test string_length > (USHORT)-1 - 3. */
    string.gx_string_length = string_length -  3;
    buffer[string.gx_string_length] = '\0';
    status = gx_prompt_text_set_ext(prompt, &string);
    EXPECT_EQ(GX_MATH_OVERFLOW, status);

#if defined(GX_ENABLE_DEPRECATED_STRING_API)
    /* Create string scroll wheel for testing. */
    memset(&wheel, 0, sizeof(GX_STRING_SCROLL_WHEEL));
    gx_utility_rectangle_define(&size, 0, 0, 100, 100);
    status = gx_string_scroll_wheel_create(&wheel, "wheel", GX_NULL, 0, GX_NULL, GX_STYLE_TEXT_COPY, 0, &size);
    EXPECT_EQ(GX_SUCCESS, status);

    /* Test _gx_system_private_string_list_copy. */

    /* Test string_count > (USHORT)-1. */
    string_count = (USHORT)-1 + 1;
    status = gx_string_scroll_wheel_string_list_set(&wheel, string_list, string_count);
    EXPECT_EQ(GX_MATH_OVERFLOW, status);

    /* Test string_count * sizeof(GX_CHAR *) > (USHORT)-1. */
    string_count = (string_count + 3) / 4;
    status = gx_string_scroll_wheel_string_list_set(&wheel, string_list, string_count);
    EXPECT_EQ(GX_MATH_OVERFLOW, status);

    /* Test string length > (USHORT)-1. */
    string_list[0] = (GX_CONST GX_CHAR *)buffer;
    memset(buffer, 'x', string_length);
    buffer[string_length] = '\0';
    status = gx_string_scroll_wheel_string_list_set(&wheel, string_list, 1);
    EXPECT_EQ(GX_MATH_OVERFLOW, status);

    /* Test string length + sizeof(GX_CHAR *) * string_count+ 2 > (USHORT)-1. */
    buffer[string_length - 6] = '\0';
    status = gx_string_scroll_wheel_string_list_set(&wheel, string_list, 1);
    EXPECT_EQ(GX_MATH_OVERFLOW, status);

    /* Test string length + sizeof(GX_CHAR *) * string_count + 2 + 1 > (USHORT)-1. */
    buffer[string_length - 1 - 6] = '\0';
    status = gx_string_scroll_wheel_string_list_set(&wheel, string_list, 1);
    EXPECT_EQ(GX_MATH_OVERFLOW, status);
#endif

    /* Create string scroll wheel for testing. */
    memset(&wheel, 0, sizeof(GX_STRING_SCROLL_WHEEL));
    gx_utility_rectangle_define(&size, 0, 0, 100, 100);
    status = gx_string_scroll_wheel_create_ext(&wheel, "wheel", GX_NULL, 0, GX_NULL, GX_STYLE_TEXT_COPY, 0, &size);
    EXPECT_EQ(GX_SUCCESS, status);

    /* Test string length > (USHORT)-1. */
    memset(buffer, 'x', string_length);
    buffer[string_length] = '\0';
    string_list_ext[0].gx_string_length = string_length;
    string_list_ext[0].gx_string_ptr = (GX_CONST GX_CHAR *)buffer;
    status = gx_string_scroll_wheel_string_list_set_ext(&wheel, (GX_CONST GX_STRING *)string_list_ext, 1);
    EXPECT_EQ(GX_MATH_OVERFLOW, status);

    /* Test string length + sizeof(GX_STRING) * string_count > (USHORT)-1. */
    string_list_ext[0].gx_string_length = string_length - sizeof(GX_STRING);
    buffer[string_list_ext[0].gx_string_length] = '\0';
    status = gx_string_scroll_wheel_string_list_set_ext(&wheel, (GX_CONST GX_STRING *)string_list_ext, 1);
    EXPECT_EQ(GX_MATH_OVERFLOW, status);   
    
    /* Test string length + sizeof(GX_STRING) * string_count + 1 > (USHORT)-1. */
    string_list_ext[0].gx_string_length = string_length - sizeof(GX_STRING) - 1;
    buffer[string_list_ext[0].gx_string_length] = '\0';
    status = gx_string_scroll_wheel_string_list_set_ext(&wheel, (GX_CONST GX_STRING *)string_list_ext, 1);
    EXPECT_EQ(GX_MATH_OVERFLOW, status);

    free(buffer);
 
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
