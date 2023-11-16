/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

#define REGRESSION_TEST_MODE

TEST_PARAM test_parameter = {
    "guix_dynamic_bidi_screen", /* Test name */
    0, 0, 639, 479  /* Define the coordinates of the capture area.
                         In this test, we only need to capture the line
                         drawing area.  */
};

static VOID      control_thread_entry(ULONG);
int main(int argc, char ** argv)
{
    /* Parse the command line argument. */
    gx_validation_setup(argc, argv);

    /* Start ThreadX system */
    tx_kernel_enter(); 
    return(0);
}

VOID tx_application_define(void *first_unused_memory)
{

    /* Create a dedicated thread to perform various operations
       on the line drawing example. These operations simulate 
       user input. */
    gx_validation_control_thread_create(control_thread_entry);

    /* Termiante the test if it runs for more than 100 ticks */
    /* This function is not implemented yet. */
    gx_validation_watchdog_create(100);

    /* Call the actual line example routine. */
    gx_validation_application_define(first_unused_memory);

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

#include "demo_bidi_text_draw_32bpp.c"

extern GX_CHAR file_name[];

char test_comment[256];

GX_CONST GX_RESOURCE_ID test_string_id_list[3]={
GX_STRING_ID_ONE_LINE_TEXT_2,
GX_STRING_ID_ONE_LINE_TEXT_2,
GX_STRING_ID_ONE_LINE_TEXT_2
};

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
int   frame_id = 1;

    gx_system_bidi_text_disable();

    gx_widget_attach(root, (GX_WIDGET *)&dynamic_bidi_screen);
    gx_widget_hide((GX_WIDGET *)&main_screen);

    gx_numeric_prompt_value_set(&dynamic_bidi_screen.dynamic_bidi_screen_numeric_prompt, 1);
    gx_numeric_pixelmap_prompt_value_set(&dynamic_bidi_screen.dynamic_bidi_screen_numeric_pixelmap_prompt, 1);
    
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Show dynamic bidi screen.");
    gx_validation_screen_refresh();

    gx_system_bidi_text_enable();
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Enable dynamic bidi text draw.");
    gx_validation_screen_refresh();

    gx_prompt_text_id_set(&dynamic_bidi_screen.dynamic_bidi_screen_prompt, GX_STRING_ID_ONE_LINE_TEXT_2);
    gx_prompt_text_id_set((GX_PROMPT *)&dynamic_bidi_screen.dynamic_bidi_screen_pixelmap_prompt, GX_STRING_ID_ONE_LINE_TEXT_2);
    gx_prompt_text_id_set((GX_PROMPT *)&dynamic_bidi_screen.dynamic_bidi_screen_menu, GX_STRING_ID_ONE_LINE_TEXT_2);
    gx_prompt_text_id_set((GX_PROMPT *)&dynamic_bidi_screen.dynamic_bidi_screen_menu_1, GX_STRING_ID_ONE_LINE_TEXT_2);
    gx_numeric_prompt_value_set(&dynamic_bidi_screen.dynamic_bidi_screen_numeric_prompt, 5);
    gx_numeric_pixelmap_prompt_value_set(&dynamic_bidi_screen.dynamic_bidi_screen_numeric_pixelmap_prompt, 5);
    gx_text_button_text_id_set(&dynamic_bidi_screen.dynamic_bidi_screen_button, GX_STRING_ID_ONE_LINE_TEXT_2);
    gx_text_button_text_id_set((GX_TEXT_BUTTON *)&dynamic_bidi_screen.dynamic_bidi_screen_multi_line_text_button, GX_STRING_ID_MULTI_LINE_TEXT_VIEW_TEXT);
    gx_text_button_text_id_set((GX_TEXT_BUTTON *)&dynamic_bidi_screen.dynamic_bidi_screen_checkbox, GX_STRING_ID_ONE_LINE_TEXT_2);
    gx_text_button_text_id_set((GX_TEXT_BUTTON *)&dynamic_bidi_screen.dynamic_bidi_screen_radio_button, GX_STRING_ID_ONE_LINE_TEXT_2);
    gx_multi_line_text_view_text_id_set(&dynamic_bidi_screen.dynamic_bidi_screen_text_view, GX_STRING_ID_ONE_LINE_TEXT_2);
    gx_string_scroll_wheel_string_id_list_set(&dynamic_bidi_screen.dynamic_bidi_screen_string_scroll_wheel, test_string_id_list, 3);
    gx_numeric_scroll_wheel_range_set(&dynamic_bidi_screen.dynamic_bidi_screen_numeric_scroll_wheel, 10, 20);
 
    gx_system_bidi_text_enable();
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Reset text id.");
    gx_validation_screen_refresh();
   
    gx_prompt_text_set_ext(&dynamic_bidi_screen.dynamic_bidi_screen_prompt, &test_string_list[0]);
    gx_prompt_text_set_ext((GX_PROMPT *)&dynamic_bidi_screen.dynamic_bidi_screen_pixelmap_prompt, &test_string_list[0]);
    gx_prompt_text_set_ext((GX_PROMPT *)&dynamic_bidi_screen.dynamic_bidi_screen_menu, &test_string_list[0]);
    gx_prompt_text_set_ext((GX_PROMPT *)&dynamic_bidi_screen.dynamic_bidi_screen_menu_1, &test_string_list[0]);
    gx_text_button_text_set_ext(&dynamic_bidi_screen.dynamic_bidi_screen_button, &test_string_list[0]);
    gx_text_button_text_set_ext((GX_TEXT_BUTTON *)&dynamic_bidi_screen.dynamic_bidi_screen_multi_line_text_button, &test_string_list[0]);
    gx_text_button_text_set_ext((GX_TEXT_BUTTON *)&dynamic_bidi_screen.dynamic_bidi_screen_checkbox, &test_string_list[0]);
    gx_text_button_text_set_ext((GX_TEXT_BUTTON *)&dynamic_bidi_screen.dynamic_bidi_screen_radio_button, &test_string_list[0]);
    gx_multi_line_text_view_text_set_ext(&dynamic_bidi_screen.dynamic_bidi_screen_text_view, &test_string_list[0]);
    gx_string_scroll_wheel_string_list_set_ext(&dynamic_bidi_screen.dynamic_bidi_screen_string_scroll_wheel, test_string_list, 3);

    gx_system_bidi_text_enable();
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Reset text.");
    gx_validation_screen_refresh();

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
