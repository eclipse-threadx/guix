/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_all_widgets_sl_text_input_property_16bpp", /* Test name */
    56, 196, 363, 231  /* Define the coordinates of the capture area.
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
#include "demo_guix_all_widgets_16bpp.c"

char test_comment[255];

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
GX_SINGLE_LINE_TEXT_INPUT *text_input_ptr;
int frame_id = 1;
GX_EVENT my_event;

    ToggleScreen(pTextScreen, pButtonScreen);
    text_input_ptr = &((TEXT_SCREEN_CONTROL_BLOCK *)pTextScreen) -> text_screen_text_input_1;

    strcat(text_input_ptr -> gx_single_line_text_input_buffer, "abc initial text");
    text_input_ptr -> gx_single_line_text_input_string_size = GX_STRLEN(text_input_ptr -> gx_single_line_text_input_buffer); 

    gx_widget_status_remove((GX_WIDGET *)text_input_ptr, GX_STATUS_CURSOR_SHOW);

    gx_validation_set_frame_id(frame_id);
    sprintf(test_comment, "set the background color for single line text input ");
    gx_validation_set_frame_comment(test_comment);

    /* Set the background color for the widget.  */
    gx_widget_fill_color_set(text_input_ptr, GX_COLOR_ID_SLIDER_GROOVE_TOP, GX_COLOR_ID_SLIDER_GROOVE_TOP, GX_COLOR_ID_SLIDER_GROOVE_TOP);

    /* Mark the window "dirty" */
    gx_system_dirty_mark(text_input_ptr);

    /* Force a screen refresh. */
    gx_validation_screen_refresh();

    /* Increment frame ID */
    frame_id ++;

    gx_validation_set_frame_id(frame_id);
    sprintf(test_comment, "add the transparent style for single line text input ");
    gx_validation_set_frame_comment(test_comment);

    /* Add the transparent style for the widget.  */
    gx_widget_style_add(text_input_ptr, GX_STYLE_TRANSPARENT);

    /* Mark the window "dirty" */
    gx_system_dirty_mark(text_input_ptr);

    /* Force a screen refresh. */
    gx_validation_screen_refresh();

    frame_id ++;

    gx_validation_set_frame_id(frame_id);
    sprintf(test_comment, "remove the transparent style for single line text input ");
    gx_validation_set_frame_comment(test_comment);

    /* Remove the transparent style for the widget.  */
    gx_widget_style_remove(text_input_ptr, GX_STYLE_TRANSPARENT);

    /* Mark the window "dirty" */
    gx_system_dirty_mark(text_input_ptr);

    /* Force a screen refresh. */
    gx_validation_screen_refresh();

    frame_id ++;

    gx_validation_set_frame_id(frame_id);
    sprintf(test_comment, "set the font for single line text input ");
    gx_validation_set_frame_comment(test_comment);

    /* Set the text font for the widget.  */
    gx_prompt_font_set((GX_PROMPT *)text_input_ptr, GX_FONT_ID_PROMPT);

    /* Mark the window "dirty" */
    gx_system_dirty_mark(text_input_ptr);

    /* Force a screen refresh. */
    gx_validation_screen_refresh();

    frame_id ++;

    gx_validation_set_frame_id(frame_id);
    sprintf(test_comment, "set the text color for single line text input ");
    gx_validation_set_frame_comment(test_comment);

    /* Set the text color fot the widget.  */
    gx_prompt_text_color_set((GX_PROMPT *)text_input_ptr, GX_COLOR_ID_CANVAS, GX_COLOR_ID_CANVAS, GX_COLOR_ID_CANVAS);

    /* Mark the window "dirty" */
    gx_system_dirty_mark(text_input_ptr);

    /* Force a screen refresh. */
    gx_validation_screen_refresh();

    frame_id ++;

    gx_validation_set_frame_id(frame_id);
    sprintf(test_comment, "set the border style for single line text input ");
    gx_validation_set_frame_comment(test_comment);

    /* Set the border style for the widget.  */
    gx_widget_border_style_set(text_input_ptr, GX_STYLE_BORDER_THICK);

    /* Mark the window "dirty" */
    gx_system_dirty_mark(text_input_ptr);

    /* Force a screen refresh. */
    gx_validation_screen_refresh();

    frame_id ++;

    gx_single_line_text_input_style_remove(text_input_ptr, GX_STYLE_CURSOR_BLINK);
    gx_single_line_text_input_style_add(text_input_ptr, GX_STYLE_CURSOR_ALWAYS);

    gx_validation_set_frame_id(frame_id);
    sprintf(test_comment, "change the cursor style to cursor always, click inside ");
    gx_validation_set_frame_comment(test_comment);

    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_payload.gx_event_pointdata.gx_point_x = 80;
    my_event.gx_event_payload.gx_event_pointdata.gx_point_y = 200;

    my_event.gx_event_type = GX_EVENT_PEN_DOWN;
    my_event.gx_event_display_handle = 1;
    gx_system_event_send(&my_event);

    my_event.gx_event_type = GX_EVENT_PEN_UP;
    gx_system_event_send(&my_event);

    frame_id ++;

    gx_validation_set_frame_id(frame_id);
    sprintf(test_comment, "change the cursor style to cursor always, click outside ");
    gx_validation_set_frame_comment(test_comment);

    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_payload.gx_event_pointdata.gx_point_x = 55;
    my_event.gx_event_payload.gx_event_pointdata.gx_point_y = 200;

    my_event.gx_event_type = GX_EVENT_PEN_DOWN;
    my_event.gx_event_display_handle = 1;
    gx_system_event_send(&my_event);

    my_event.gx_event_type = GX_EVENT_PEN_UP;
    gx_system_event_send(&my_event);

    frame_id ++;

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
