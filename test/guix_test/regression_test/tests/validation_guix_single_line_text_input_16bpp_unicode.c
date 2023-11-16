/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_single_line_text_input_16bpp_unicode", /* Test name */
    5, 5, 630, 470  /* Define the coordinates of the capture area.
                         In this test, we only need to capture the pixelmap
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
#ifdef win32_graphics_driver_setup_565rgb
#undef win32_graphics_driver_setup_565rgb  
#endif
#define win32_graphics_driver_setup_565rgb  gx_validation_graphics_driver_setup_565rgb


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"
#include "demo_guix_sl_text_input_16bpp.c"

/* 汉语是世界上使用人口普查最多的语言，人类同汉语有着密切的联系 */
static USHORT input_buffer[] = { 0x6c49, 0x8bed, 0x662f, 0x4e16, 0x754c, 0x4e0a, 0x4f7f, 0x7528, 0x4eba, 0x53e3, 0x666e, 0x67e5, 0x6700, 0x591a, 0x7684, 0x8bed, 0x8a00, 0xff0c, 0x4eba, 0x7c7b, 0x540c, 0x6c49, 0x8bed, 0x6709, 0x7740, 0x5bc6, 0x5207, 0x7684, 0x8054, 0x7cfb, 0x3002};

char test_comment[255];

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
GX_SINGLE_LINE_TEXT_INPUT *text_input_ptr;
GX_EVENT                   my_event;
int                        string_size;
int                        key_index;
int                        frame_id = 1;

    text_input_ptr = &pMainScreen->window_sl_text_input;

    gx_single_line_text_input_style_remove(text_input_ptr, GX_STYLE_CURSOR_BLINK);
    gx_single_line_text_input_style_add(text_input_ptr, GX_STYLE_CURSOR_ALWAYS);
    gx_display_language_table_set_ext(root->gx_window_root_canvas->gx_canvas_display, display_1_language_table, DISPLAY_1_LANGUAGE_TABLE_SIZE, DISPLAY_1_STRING_TABLE_SIZE);
    gx_system_active_language_set(LANGUAGE_CHINESE);
    font_depth = 3;
    gx_prompt_font_set((GX_PROMPT *)text_input_ptr, font[font_depth][font_size]);
    /* Set the border style for the widget.  */
    gx_widget_border_style_set(text_input_ptr, GX_STYLE_BORDER_THICK);
    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_display_handle = 1;
    my_event.gx_event_type = GX_EVENT_PEN_DOWN;
    my_event.gx_event_payload.gx_event_pointdata.gx_point_x = 150;
    my_event.gx_event_payload.gx_event_pointdata.gx_point_y = 100;
    gx_system_focus_claim((GX_WIDGET *) text_input_ptr);
    /* Initialize text input buffer.  */
    string_size = sizeof(input_buffer)/sizeof(USHORT);

    for(key_index = 0; key_index < string_size; key_index++)
    {
        memset(&my_event, 0, sizeof(GX_EVENT));
        my_event.gx_event_payload.gx_event_ushortdata[0] = input_buffer[key_index];
        my_event.gx_event_type = GX_EVENT_KEY_DOWN;
        my_event.gx_event_display_handle = 1;
        gx_system_event_send(&my_event);

        gx_validation_set_frame_id(frame_id++);
        sprintf(test_comment, "insert Chinese character ");
        gx_validation_set_frame_comment(test_comment);
        /* Force a screen refresh. */
        gx_validation_screen_refresh();
    }
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
