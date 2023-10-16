/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_all_widgets_sl_text_input_16bpp", /* Test name */
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

static char input_buffer1[] = "";
static char input_buffer2[] = "a";
static char input_buffer3[] = "bc";
static char input_buffer4[] = "single line text input";
static char input_buffer5[] = "0123456789 testwq";

static char *input_buffer[] = {input_buffer1, input_buffer2, input_buffer3, input_buffer4, input_buffer5};

static GX_VALUE x_point[] = {55, 56, 280, 363, 364};
static GX_VALUE y_point[] = {195, 196, 200, 231, 232};

static USHORT key_value[] = {GX_KEY_HOME, GX_KEY_DELETE, GX_KEY_RIGHT_ARROW, GX_KEY_END, GX_KEY_BACKSPACE, GX_KEY_LEFT_ARROW, GX_KEY_SELECT, GX_KEY_SPACE, 'q', 'v'};

char test_comment[255];

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
GX_SINGLE_LINE_TEXT_INPUT *text_input_ptr;
GX_EVENT                   my_event;
int                        buffer_index;
int                        string_size;
int                        x_index;
int                        y_index;
int                        key_index;
int                        frame_id = 1;
int                        index;


    ToggleScreen(pTextScreen, pButtonScreen);
    text_input_ptr = &((TEXT_SCREEN_CONTROL_BLOCK *)pTextScreen) -> text_screen_text_input_1;

    gx_single_line_text_input_style_remove(text_input_ptr, GX_STYLE_CURSOR_BLINK);
    gx_single_line_text_input_style_add(text_input_ptr, GX_STYLE_CURSOR_ALWAYS);

    /* Set the border style for the widget.  */
    gx_widget_border_style_set(text_input_ptr, GX_STYLE_BORDER_THICK);

    /* Set text color. */
    gx_prompt_text_color_set((GX_PROMPT *)text_input_ptr, GX_COLOR_ID_BLACK, GX_COLOR_ID_BLACK, GX_COLOR_ID_BLACK);

    for(index = 0; index < 2; index++)
    {
        if(index == 1)
        {
            gx_single_line_text_input_style_remove(text_input_ptr, GX_STYLE_TEXT_LEFT);
            gx_single_line_text_input_style_add(text_input_ptr, GX_STYLE_TEXT_RIGHT);
            gx_single_line_text_input_buffer_clear(text_input_ptr);

            gx_validation_set_frame_id(frame_id++);
            sprintf(test_comment, "Set text alignment to \"right\"");
            gx_validation_set_frame_comment(test_comment);
            gx_validation_screen_refresh();
        }

        for (buffer_index = 0; buffer_index < 5; buffer_index ++)
        {

            string_size = GX_STRLEN(input_buffer[buffer_index]);

            for(key_index = 0; key_index < string_size; key_index++)
            {
                gx_validation_set_frame_id(frame_id++);
                sprintf(test_comment, "insert \'%c\'", input_buffer[buffer_index][key_index]);
                gx_validation_set_frame_comment(test_comment);

                memset(&my_event, 0, sizeof(GX_EVENT));
                my_event.gx_event_type = GX_EVENT_KEY_DOWN;
                my_event.gx_event_display_handle = 1;
                my_event.gx_event_payload.gx_event_ushortdata[0] = input_buffer[buffer_index][key_index];
                gx_system_event_send(&my_event);
            }

            for (x_index = 0; x_index < (INT)(sizeof(x_point) / sizeof(GX_VALUE)); x_index ++)
            {
                for (y_index = 0; y_index < (INT)(sizeof(y_point) / sizeof(GX_VALUE)); y_index ++)
                {
                    for (key_index = 0; key_index < (INT)(sizeof(key_value) / sizeof(USHORT)); key_index++)
                    {
                        if ((x_point[x_index] < 56) || (x_point[x_index] > 363) 
                            || (y_point[y_index] < 196) || (y_point[y_index] > 231))
                        {
                            sprintf(test_comment, "text input already has %d characters, x: %d, y: %d, click outside, lose focus ", 
                                    text_input_ptr -> gx_single_line_text_input_string_size, x_point[x_index], y_point[y_index]);

                        }
                        else
                        {
                            sprintf(test_comment, "text input already has %d characters, x: %d, y: %d, click inside, gain focus, ", 
                                    text_input_ptr -> gx_single_line_text_input_string_size, x_point[x_index], y_point[y_index]);
                        }

                        gx_validation_set_frame_id(frame_id++);
                        gx_validation_set_frame_comment(test_comment);

                        memset(&my_event, 0, sizeof(GX_EVENT));
                        my_event.gx_event_payload.gx_event_pointdata.gx_point_x = x_point[x_index];
                        my_event.gx_event_payload.gx_event_pointdata.gx_point_y = y_point[y_index];

                        my_event.gx_event_type = GX_EVENT_PEN_DOWN;
                        my_event.gx_event_display_handle = 1;
                        gx_system_event_send(&my_event);

                        my_event.gx_event_type = GX_EVENT_PEN_UP;
                        gx_system_event_send(&my_event);

                        switch(key_value[key_index])
                        {
                        case GX_KEY_HOME:
                            strcat(test_comment, "Home key ");
                            break;
                        case GX_KEY_END:
                            strcat(test_comment, "End key ");
                            break;
                        case GX_KEY_BACKSPACE:
                            strcat(test_comment, "Backspace key ");
                            break;
                        case GX_KEY_DELETE:
                            strcat(test_comment, "Delete key ");
                            break;
                        case GX_KEY_LEFT_ARROW:
                            strcat(test_comment, "Left Arrow key ");
                            break;
                        case GX_KEY_RIGHT_ARROW:
                            strcat(test_comment, "Right Arrow key ");
                            break;
                        case GX_KEY_SELECT:
                            strcat(test_comment, "Enter key ");
                            break;
                        case GX_KEY_SPACE:
                            strcat(test_comment, "Space key ");
                            break;
                        default:
                            strcat(test_comment, "insert a character ");
                            break;
                        }
                        gx_validation_set_frame_id(frame_id++);
                        gx_validation_set_frame_comment(test_comment);

                        my_event.gx_event_payload.gx_event_ushortdata[0] = key_value[key_index];
                        my_event.gx_event_display_handle = 1;
                        my_event.gx_event_type = GX_EVENT_KEY_DOWN;
                        gx_system_event_send(&my_event);
                    }
                }
            }
        }
    }

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
