/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_sl_input_select_copy_paste", /* Test name */
    114, 69, 538, 119  /* Define the coordinates of the capture area.
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
static USHORT chinese_text[] = { 0x6c49, 0x8bed, 0x662f, 0x4e16, 0x754c, 0x4e0a, 0x4f7f, 0x7528, 0x4eba, 0x53e3, 0x666e, 0x67e5, 0x6700, 0x591a, 0x7684, 0x8bed, 0x8a00, 0x4eba, 0x7c7b, 0x540c, 0x6c49, 0x8bed, 0x6709, 0x7740, 0x5bc6, 0x5207, 0x7684, 0x8054, 0x7cfb};

char test_comment[255];

typedef struct TEST_STRUCT{
INT event_type;
INT key_value;
GX_VALUE point_x;
GX_VALUE point_y;
INT repeat_times;
GX_CHAR *comment;
}TEST;

TEST test_list[]={
{GX_EVENT_MARK_PREVIOUS, 0, 0, 0, 29, "mark previous"},
{GX_EVENT_MARK_NEXT, 0, 0, 0, 1, "mark next"},
{GX_EVENT_KEY_DOWN, GX_KEY_RIGHT_ARROW, 0, 0, 1, "right arrow key down"},/* right key down */
{GX_EVENT_MARK_PREVIOUS, 0, 0, 0, 29, "mark previous"},
{GX_EVENT_KEY_DOWN, GX_KEY_LEFT_ARROW, 0, 0, 1, "left arrow key down"},/* left key down. */
{GX_EVENT_MARK_NEXT, 0, 0, 0, 29, "mark next"},
{GX_EVENT_KEY_DOWN, GX_KEY_LEFT_ARROW, 0, 0, 1, "left arrow key down"},/* left key down. */
{GX_EVENT_MARK_NEXT, 0, 0, 0, 29, "mark next"},
{GX_EVENT_KEY_DOWN, GX_KEY_RIGHT_ARROW, 0, 0, 1, "right arrow key down"},/* right key down.*/
{GX_EVENT_MARK_PREVIOUS, 0, 0, 0, 10, "mark previous"},
{GX_EVENT_PEN_DOWN, 0, 533, 94, 1, "pen down(533, 94)"},/* pen down*/
{GX_EVENT_KEY_DOWN, GX_KEY_HOME, 0, 0, 1, "home key down"},
{GX_EVENT_MARK_NEXT, 0, 0, 0, 29, "mark next"},
{GX_EVENT_CUT, 0, 0, 0, 1, "cut"},
{GX_EVENT_PASTE, 0, 0, 0, 1, "paste"},
{GX_EVENT_PEN_DOWN, 0, 533, 94, 1, "pen down(533, 94)"},/* pen down*/
{GX_EVENT_PEN_UP, 0, 533, 94, 1, "pen up(533, 94)"},
{GX_EVENT_MARK_PREVIOUS, 0, 0, 0, 9, "mark previous"},
{GX_EVENT_CUT, 0, 0, 0, 1, "cut"},
{GX_EVENT_PASTE, 0, 0, 0, 1, "paste"},
{GX_EVENT_MARK_PREVIOUS, 0, 0, 0, 2, "mark previous"},
{GX_EVENT_COPY, 0, 0, 0, 1, "copy"},
{GX_EVENT_KEY_DOWN, GX_KEY_LEFT_ARROW, 0, 0, 2, "left arrow key down"},
{GX_EVENT_PASTE, 0, 0, 0, 1, "paste"},
{GX_EVENT_MARK_PREVIOUS, 0, 0, 0, 27, "mark previous"},
{GX_EVENT_MARK_NEXT, 0, 0, 0, 25, "mark next"},
{GX_EVENT_KEY_DOWN, GX_KEY_BACKSPACE, 0, 0, 1, "backspace key down"},
{GX_EVENT_PASTE, 0, 0, 0, 1, "paste"},
{GX_EVENT_MARK_PREVIOUS, 0, 0, 0, 27, "mark previous"},
{GX_EVENT_MARK_NEXT, 0, 0, 0, 25, "mark next"},
{GX_EVENT_KEY_DOWN, GX_KEY_DELETE, 0, 0, 1, "delete key down"},
{GX_EVENT_PASTE, 0, 0, 0, 1, "paste"},
{GX_EVENT_MARK_PREVIOUS, 0, 0, 0, 27, "mark previous"},
{GX_EVENT_KEY_DOWN, GX_KEY_END, 0, 0, 1, "end key down"},
{GX_EVENT_MARK_PREVIOUS, 0, 0, 0, 1, "mark previous"},
{GX_EVENT_KEY_DOWN, GX_KEY_END, 0, 0, 1, "end key down"},
{GX_EVENT_MARK_PREVIOUS, 0, 0, 0, 1, "mark previous"},
{GX_EVENT_KEY_DOWN, GX_KEY_HOME, 0, 0, 1, "home key down"},
{GX_EVENT_MARK_PREVIOUS, 0, 0, 0, 1, "mark previous"},
{GX_EVENT_KEY_DOWN, GX_KEY_HOME, 0, 0, 1, "home key down"},
{GX_EVENT_MARK_NEXT, 0, 0, 0, 27, "mark next"},
{GX_EVENT_MARK_PREVIOUS, 0, 0, 0, 25, "mark previous"},
{GX_EVENT_KEY_DOWN, GX_EVENT_DELETE, 0, 0, 1, "delete key down"},
{GX_EVENT_PASTE, 0, 0, 0, 1, "paste"},
{GX_EVENT_KEY_DOWN, GX_KEY_HOME, 0, 0, 1, "home key down"},
{GX_EVENT_MARK_NEXT, 0, 0, 0, 27, "mark next"},
{GX_EVENT_KEY_DOWN, GX_KEY_BACKSPACE, 0, 0, 1, "backspace key down"},
{0, 0, 0, 0, 0, GX_NULL}
};

typedef struct ALIGNMENT_STRUCT{
INT value;
GX_CHAR *text;
}ALIGNMENT;

ALIGNMENT alignment_style_list[]={
{GX_STYLE_TEXT_LEFT, "left alignment"},
{GX_STYLE_TEXT_RIGHT, "right alignment"},
{GX_STYLE_TEXT_CENTER, "center alignment"},
{0, 0}
};

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
GX_SINGLE_LINE_TEXT_INPUT *text_input_ptr;
GX_EVENT                   my_event;
int                        text_size;
int                        index;
int                        frame_id = 1;
TEST                      *test;
ALIGNMENT                 *alignment = alignment_style_list;
int                        repeat;

    memset(&my_event, 0, sizeof(GX_EVENT));
    text_input_ptr = &pMainScreen->window_sl_text_input;

    /* Remove cursor blink style. */
    gx_single_line_text_input_style_remove(text_input_ptr, GX_STYLE_CURSOR_BLINK);
    gx_display_language_table_set_ext(root->gx_window_root_canvas->gx_canvas_display, display_1_language_table, DISPLAY_1_LANGUAGE_TABLE_SIZE, DISPLAY_1_STRING_TABLE_SIZE);
    gx_system_active_language_set(LANGUAGE_CHINESE);
    font_depth = 3;
    gx_prompt_font_set((GX_PROMPT *)text_input_ptr, font[font_depth][font_size]);

    while(alignment->value)
    {
        gx_single_line_text_input_buffer_clear(text_input_ptr);
        gx_widget_style_remove((GX_WIDGET *)text_input_ptr, GX_STYLE_TEXT_ALIGNMENT_MASK);
        gx_widget_style_add((GX_WIDGET *)text_input_ptr, alignment->value);
        
        gx_validation_set_frame_id(frame_id++);
        sprintf(test_comment, "clear input buffer, set %s", alignment->text);
        gx_validation_set_frame_comment(test_comment);
        gx_validation_screen_refresh();
        alignment++;

        test = test_list;

         /* Simulate a click event. */
        my_event.gx_event_display_handle = 1;
        my_event.gx_event_type = GX_EVENT_PEN_DOWN;
        my_event.gx_event_payload.gx_event_pointdata.gx_point_x = 150;
        my_event.gx_event_payload.gx_event_pointdata.gx_point_y = 100;
        gx_system_event_send(&my_event);

        my_event.gx_event_type = GX_EVENT_PEN_UP;
        gx_system_event_send(&my_event);

        /* Initialize text input buffer.  */
        text_size = sizeof(chinese_text)/sizeof(USHORT);

        for(index = 0; index < text_size; index++)
        {
            my_event.gx_event_payload.gx_event_ushortdata[0] = chinese_text[index];
            my_event.gx_event_type = GX_EVENT_KEY_DOWN;
            my_event.gx_event_display_handle = 1;
            gx_system_event_send(&my_event);

            gx_validation_set_frame_id(frame_id++);
            sprintf(test_comment, "insert Chinese character %x", chinese_text[index]);
            gx_validation_set_frame_comment(test_comment);

            /* Force a screen refresh. */
            gx_validation_screen_refresh();
        }

        while(test->event_type)
        {
        

            my_event.gx_event_type = test->event_type;
            my_event.gx_event_target = (GX_WIDGET *)text_input_ptr;
            if(test->event_type == GX_EVENT_KEY_DOWN)
            {
                my_event.gx_event_payload.gx_event_ushortdata[0] = test->key_value;
            }
            else
            {
                my_event.gx_event_payload.gx_event_pointdata.gx_point_x = test->point_x;
                my_event.gx_event_payload.gx_event_pointdata.gx_point_y = test->point_y;
            }

            repeat = test->repeat_times;
            while(repeat)
            {
                gx_validation_set_frame_id(frame_id++);
                gx_validation_set_frame_comment(test->comment);

                gx_system_event_send(&my_event);
                tx_thread_sleep(10);
                repeat--;
            }
            test++;
        }
    }

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
