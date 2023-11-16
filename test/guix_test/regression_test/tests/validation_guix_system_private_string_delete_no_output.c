/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_system_private_string_delete_no_output", /* Test name */
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
int             failed_tests = 0;
GX_PROMPT      *prompt = &button_screen.button_screen_title_1;
GX_PROMPT      *prompt_2 = &button_screen.button_screen_button_label_1;
GX_TEXT_BUTTON *button = &button_screen.button_screen_text_button_1;
GX_CHECKBOX    *checkbox = &button_screen.button_screen_checkbox;
GX_STRING       string;
  
    /* Test private prompt string delete when private string is not empty. */
    gx_prompt_text_set_ext(prompt, GX_NULL);
    gx_widget_style_add(prompt, GX_STYLE_TEXT_COPY);

    string.gx_string_ptr = "test";
    string.gx_string_length = 4;
    gx_prompt_text_set_ext(prompt, &string);
    if(prompt->gx_prompt_string.gx_string_ptr == GX_NULL)
    {
        failed_tests++;
    }

    gx_widget_delete(prompt);
    if(prompt->gx_prompt_string.gx_string_ptr != GX_NULL)
    {
        failed_tests++;
    }
 
    /* Test private text button string delete when private string is not empty. */
    gx_text_button_text_set_ext(button, GX_NULL);
    gx_widget_style_add(button, GX_STYLE_TEXT_COPY);
    gx_text_button_text_set_ext(button, &string);
    if(button->gx_text_button_string.gx_string_ptr == GX_NULL)
    {
        failed_tests++;
    }

    gx_widget_delete(button);
    if(button->gx_text_button_string.gx_string_ptr != GX_NULL)
    {
        failed_tests++;
    }

    /* Test private prompt string delete when private string is empty. */
    gx_prompt_text_set_ext(prompt_2, GX_NULL);
    gx_widget_style_add(prompt_2, GX_STYLE_TEXT_COPY);
    gx_widget_delete(prompt_2);

    /* Test private checkbox string delete when private string is empty. */
    gx_text_button_text_set_ext((GX_TEXT_BUTTON *)checkbox, GX_NULL);
    gx_widget_style_add(checkbox, GX_STYLE_TEXT_COPY);
    gx_widget_delete(checkbox); 
 
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
