/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_prompt_event_process_no_output", /* Test name */
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

VOID numeric_value_format(GX_NUMERIC_PROMPT *prompt, INT value)
{
    memset(prompt->gx_numeric_prompt_buffer, '0', GX_NUMERIC_PROMPT_BUFFER_SIZE);
}

static VOID control_thread_entry(ULONG input)
{
int        failed_tests = 0;
UINT       status;
GX_PROMPT  test_prompt;
GX_PROMPT *prompt = &button_screen.button_screen_title_1;
GX_STRING  string;
GX_CHAR    string_buffer[] = "test string";
GX_EVENT   my_event;

    status = gx_prompt_event_process(GX_NULL, &my_event);
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_prompt_event_process(&test_prompt, GX_NULL);
    EXPECT_EQ(GX_PTR_ERROR, status);

    memset(&test_prompt, 0, sizeof(GX_PROMPT));
    status = gx_prompt_event_process(&test_prompt, &my_event);
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    gx_widget_hide(&button_screen); 

    gx_widget_style_add(prompt, GX_STYLE_TEXT_COPY);

    string.gx_string_ptr = string_buffer;
    string.gx_string_length = sizeof(string_buffer) - 1;

    gx_prompt_text_set_ext(prompt, &string);

    gx_system_memory_allocator_set(GX_NULL, GX_NULL);
    
    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_type = GX_EVENT_DELETE;
    status = gx_prompt_event_process(prompt, &my_event);
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
