/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_private_string_delete_no_output", /* Test name */
    0, 0, 0, 0  /* Define the coordinates of the capture area.
                         In this test, we only need to capture the line
                         drawing area.  */
};

static VOID      control_thread_entry(ULONG);
int main(int argc, char ** argv)
{
    /* Start ThreadX system */
    tx_kernel_enter(); 
    return(0);
}

VOID tx_application_define(void *first_unused_memory)
{
    /* Call the actual line example routine. */
    gx_validation_application_define(first_unused_memory);
    
    /* Termiante the test if it runs for more than 100 ticks */
    /* This function is not implemented yet. */
    gx_validation_watchdog_create(100);

    /* Create a dedicated thread to perform various operations
       on the line drawing example. These operations simulate 
       user input. */
    gx_validation_control_thread_create(control_thread_entry);
}


/* Replace the default graphics driver with the validation driver. */
#ifdef win32_graphics_driver_setup_24xrgb  
#undef win32_graphics_driver_setup_24xrgb  
#endif
#define win32_graphics_driver_setup_24xrgb  gx_validation_graphics_driver_setup_24xrgb


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"
#include "demo_guix_widgets_with_private_string.c"

CHAR test_string_1[] = "string 1";
CHAR test_string_2[] = "string 2";
CHAR test_string_3[] = "string 3";

GX_CONST GX_STRING test_string_list[3]={
   {test_string_1, sizeof(test_string_1) - 1},
   {test_string_2, sizeof(test_string_2) - 1},
   {test_string_3, sizeof(test_string_3) - 1}
};

static VOID control_thread_entry(ULONG input)
{
GX_WIDGET *child;
GX_WIDGET *next;
GX_PROMPT *prompt;
GX_TEXT_BUTTON *text_button;
GX_MULTI_LINE_TEXT_VIEW *view;
GX_STRING_SCROLL_WHEEL *string_wheel;
INT failed_tests = 0;

    child = main_screen.gx_widget_first_child;

    while(child)
    {
        next = child->gx_widget_next;

        switch(child->gx_widget_type)
        {
        case GX_TYPE_PROMPT:
        case GX_TYPE_PIXELMAP_PROMPT:
        case GX_TYPE_MENU:
        case GX_TYPE_TREE_VIEW:
            if(child->gx_widget_type == GX_TYPE_TREE_VIEW)
            {
                prompt = (GX_PROMPT *)child->gx_widget_first_child;
            }
            else
            {
                prompt = (GX_PROMPT *)child;
            }
            gx_prompt_text_set_ext(prompt, &test_string_list[0]);
            if(!prompt->gx_prompt_string.gx_string_ptr)
            {
                PRINT_ERROR("Prompt text set failed!")
            }

            gx_widget_delete(child);
            if(prompt->gx_prompt_string.gx_string_ptr)
            {
                PRINT_ERROR("Prompt private string delete failed!")
            }
            break;

        case GX_TYPE_TEXT_BUTTON:
        case GX_TYPE_CHECKBOX:
        case GX_TYPE_RADIO_BUTTON:
        case GX_TYPE_MULTI_LINE_TEXT_BUTTON:
            text_button = (GX_TEXT_BUTTON *)child;
            gx_text_button_text_set_ext(text_button, &test_string_list[0]);
            if(!text_button->gx_text_button_string.gx_string_ptr)
            {
                PRINT_ERROR("Text button text set failed!")
            }

            gx_widget_delete(child);
            if(text_button->gx_text_button_string.gx_string_ptr)
            {
                PRINT_ERROR("Text button private string delete failed!")
            }
            
            break;

        case GX_TYPE_MULTI_LINE_TEXT_VIEW:
        case GX_TYPE_RICH_TEXT_VIEW:
            view = (GX_MULTI_LINE_TEXT_VIEW *)child;
            gx_multi_line_text_view_text_set_ext(view, &test_string_list[0]);
            if(!view->gx_multi_line_text_view_text.gx_string_ptr)
            {
                PRINT_ERROR("Multi line text view text set failed!")
            }

            gx_widget_delete(child);

            if(view->gx_multi_line_text_view_text.gx_string_ptr)
            {
                PRINT_ERROR("Multi line text view private string delete faile!")
            }
            break;

        case GX_TYPE_STRING_SCROLL_WHEEL:
            string_wheel = (GX_STRING_SCROLL_WHEEL *)child;
            gx_string_scroll_wheel_string_list_set_ext(string_wheel, &test_string_list[0], 3);
            if(!string_wheel->gx_string_scroll_wheel_string_list)
            {
                PRINT_ERROR("String scroll wheel text set failed!");
            }

            gx_widget_delete(child);
            if(string_wheel->gx_string_scroll_wheel_string_list)
            {
                PRINT_ERROR("String scroll wheel private string delete failed!")
            }
            break;
        }
        child = next;
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
