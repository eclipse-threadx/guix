/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_system.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_ml_text_input_select_copy_paste", /* Test name */
    30, 120, 245, 330  /* Define the coordinates of the capture area.
                         In this test, we only need to capture the multi 
                         line text input drawing area.  */
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
       on the multi line text input example. These operations simulate 
       user input. */
    gx_validation_control_thread_create(control_thread_entry);

    /* Terminate the test if it runs for more than 100 ticks */
    /* This function is not implemented yet. */
    gx_validation_watchdog_create(100);

    /* Call the actual multi line text input routine. */
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

#include "demo_guix_ml_text_input_16bpp.c"

GX_CHAR test_string[]=
"0 do you remember the day we used to be\r\r1\r2\r3\r4ABCDEFGHIJKLMN\r5\r6\r7\r8\r9\r\r\
0\r1\r2\r3\r4\r5OPQRSTUVWXYZ\r6\r7\r8\r9\r\
0\r1\r2\r3\r4\r5abcdefghijklmn\r6\r7\r8\r9\r0\r\
31\r32\r33\r34\r35\r36\r37ABCDEFG\r38abcdefghijk\r39ABCDEF\r40";

char test_comment[256];

typedef struct TEST_STRUCT{
INT event_type;
GX_VALUE key_value;
GX_VALUE point_x;
GX_VALUE point_y;
INT      scroll_value;
INT      repeat_times;
GX_CHAR *comment;
}TEST;

TEST mark_left[]={
{GX_EVENT_PEN_DOWN,     0, 70, 266, 0, 1, "pen down on (70, 266)"},
{GX_EVENT_MARK_PREVIOUS,0, 0,  0,   0, 3, "mark previous" },
{0,                     0, 0,  0,   0, 0, ""}
};

TEST mark_left_to_previous_line[]={
{GX_EVENT_PEN_DOWN,     0, 70, 266, 0, 1, "pen down on (70, 266)"},
{GX_EVENT_MARK_PREVIOUS,0, 0,  0,   0, 8, "mark previous" },
{0,                     0, 0,  0,   0, 0, ""}
};

TEST mark_left_to_invisible_line[]={
{GX_EVENT_PEN_DOWN,     0, 62, 163, 0, 1, "pen down on (62, 163)"},
{GX_EVENT_MARK_PREVIOUS,0, 0,  0,   0, 9, "mark previous" },
{0,                     0, 0,  0,   0, 0, ""}
};

TEST mark_right[]={
{GX_EVENT_PEN_DOWN,     0, 70, 266, 0, 1, "pen down on (70, 266)"},
{GX_EVENT_MARK_NEXT,    0, 0,  0,   0, 3, "mark next" },
{0,                     0, 0,  0,   0, 0, ""}
};

TEST mark_right_to_next_line[]={
{GX_EVENT_PEN_DOWN,     0, 70, 266, 0, 1, "pen down on (70, 266)"},
{GX_EVENT_MARK_NEXT,    0, 0,  0,   0, 11, "mark next" },
{0,                     0, 0,  0,   0, 0, ""}
};

TEST mark_right_to_invisible_line[]={
{GX_EVENT_VERTICAL_SCROLL, 0, 0, 0,125, 1, "scroll up"},
{GX_EVENT_PEN_DOWN,     0, 189, 263, 0, 1, "pen down on (189, 263)"},
{GX_EVENT_MARK_NEXT,    0, 0,  0,   0, 7, "mark next" },
{0,                     0, 0,  0,   0, 0, ""}
};

TEST mark_up[]={
{GX_EVENT_PEN_DOWN,     0, 70, 266, 0, 1, "pen down on (70, 266)"},
{GX_EVENT_MARK_UP,      0, 0,  0,   0, 1, "mark up" },
{0,                     0, 0,  0,   0, 0, ""}
};

TEST mark_up_to_invisible_line[]={
{GX_EVENT_PEN_DOWN,     0, 62, 163, 0, 1, "pen down on (62, 163)"},
{GX_EVENT_MARK_UP,      0, 0,  0,   0, 3, "mark up" },
{0,                     0, 0,  0,   0, 0, ""}
};

TEST mark_down[]={
{GX_EVENT_VERTICAL_SCROLL, 0, 0, 0, 125, 1, "scroll up"},
{GX_EVENT_PEN_DOWN,     0, 189, 263, 0, 1, "pen down on (189, 263)"},
{GX_EVENT_MARK_DOWN,    0, 0,  0,    0, 1, "mark down" },
{0,                     0, 0,  0,    0, 0, ""}
};

TEST mark_down_to_invisible_line[]={
{GX_EVENT_VERTICAL_SCROLL, 0, 0, 0, 125, 1, "scroll up"},
{GX_EVENT_PEN_DOWN,     0, 189, 263, 0, 1, "pen down on (189, 263)"},
{GX_EVENT_MARK_DOWN,    0, 0,  0,    0, 4, "mark down" },
{0,                     0, 0,  0,    0, 0, ""}
};

TEST mark_home[]={
{GX_EVENT_PEN_DOWN,     0, 70, 266, 0, 1, "pen down on (70, 266)"},
{GX_EVENT_MARK_HOME,    0, 0, 0, 0, 1, "home key down"},
{0,                     0, 0,  0,    0, 0, ""}
};

TEST mark_end[]={
{GX_EVENT_PEN_DOWN,     0, 70, 266, 0, 1, "pen down on (70, 266)"},
{GX_EVENT_MARK_END,     0, 0, 0, 0, 1, "end key down"},
{0,                     0, 0,  0,    0, 0, ""}
};

typedef struct CASE_STRUCT{
TEST *test_list;
GX_CHAR *comment;
}CASE;

CASE case_list[]={
{mark_left, "mark left"},
{mark_left_to_previous_line, "mark left to previous line"},
{mark_left_to_invisible_line, "mark left to invisible line"},
{mark_right, "mark right"},
{mark_right_to_next_line, "mark right to next line"},
{mark_right_to_invisible_line, "mark right to invisible line"},
{mark_up, "mark up"},
{mark_up_to_invisible_line, "mark up to invisible line"},
{mark_down, "mark down"},
{mark_down_to_invisible_line, "mark down to invisible line"},
{mark_home, "mark home"},
{mark_end, "mark end"},
{0, ""}
};

TEST test_list[]={
{GX_EVENT_KEY_DOWN, GX_KEY_HOME,       0, 0, 0, 1, "home key down"},
{GX_EVENT_KEY_DOWN, GX_KEY_END,        0, 0, 0, 1, "end key down"},
{GX_EVENT_KEY_DOWN, GX_KEY_LEFT_ARROW, 0, 0, 0, 1, "left arrow down"},
{GX_EVENT_KEY_DOWN, GX_KEY_RIGHT_ARROW,0, 0, 0, 1, "right arrow down"},
{GX_EVENT_KEY_DOWN, GX_KEY_BACKSPACE,  0, 0, 0, 1, "backspace"},
{GX_EVENT_KEY_DOWN, GX_KEY_DELETE,     0, 0, 0, 1, "delete"},
{GX_EVENT_KEY_DOWN, 'I',               0, 0, 0, 1, "insert character I"},
{GX_EVENT_CUT,      0,                 0, 0, 0, 1, "cut"},
{GX_EVENT_PASTE,    0,                 0, 0, 0, 1, "paste"},
{0,                 0,                 0, 0, 0, 0, ""}
};

typedef struct ALIGNMENT_STRUCT{
INT value;
GX_CHAR *text;
}ALIGNMENT;

ALIGNMENT alignment_style_list[]={
{GX_STYLE_TEXT_LEFT, "left alignment"},
{GX_STYLE_TEXT_RIGHT, "right alignment"},
{GX_STYLE_TEXT_CENTER, "center alignment"},
{0, ""}
};

void run_case(GX_MULTI_LINE_TEXT_INPUT *input, CASE *case_entry, int frame_id)
{
INT repeat_times;
GX_EVENT my_event;
TEST *test;
GX_SCROLLBAR *scroll;

    gx_window_scrollbar_find((GX_WINDOW *)input, GX_TYPE_VERTICAL_SCROLL, &scroll);

    test = case_entry->test_list;
    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_display_handle = 1;
    
    gx_multi_line_text_input_text_set(input, test_string);

    while(test->event_type)
    {
        repeat_times = test->repeat_times;

        while(repeat_times)
        {
            if((repeat_times == 1) && ((test + 1)->event_type == 0))
            {
                gx_validation_set_frame_id(frame_id++);
                gx_validation_set_frame_comment(case_entry->comment);
            }

            my_event.gx_event_type = test->event_type;
            my_event.gx_event_target = (GX_WIDGET *)input;

            switch(test->event_type)
            {
            case GX_EVENT_KEY_DOWN:
                my_event.gx_event_payload.gx_event_ushortdata[0] = test->key_value;
                break;
            case GX_EVENT_VERTICAL_SCROLL:
                my_event.gx_event_payload.gx_event_intdata[0] = test->scroll_value;
                my_event.gx_event_payload.gx_event_intdata[1] = scroll->gx_scrollbar_info.gx_scroll_value;
                break;
            case GX_EVENT_PEN_DOWN:
                my_event.gx_event_payload.gx_event_pointdata.gx_point_x = test->point_x;
                my_event.gx_event_payload.gx_event_pointdata.gx_point_y = test->point_y;
                my_event.gx_event_target = 0;
                break;
            }
            gx_system_event_send(&my_event);
            repeat_times--;
        }
        test++;
    }
}

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int                       frame_id = 1;
GX_MULTI_LINE_TEXT_INPUT *text_input;
GX_EVENT                  my_event;
ALIGNMENT                *alignment = alignment_style_list;
CASE                     *case_entry;
TEST                     *test;

    text_input = &((WINDOW_CONTROL_BLOCK *)pMainScreen)->window_ml_text_input;

    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_display_handle = 1;
    my_event.gx_event_target = (GX_WIDGET *)text_input;
    
     /* Remove the cursor blink flag. */
    gx_multi_line_text_input_style_remove(text_input, GX_STYLE_CURSOR_BLINK);
    gx_multi_line_text_view_whitespace_set((GX_MULTI_LINE_TEXT_VIEW *)text_input, 1);
    while(alignment->value)
    {
        gx_widget_style_remove((GX_WIDGET *)text_input, GX_STYLE_TEXT_ALIGNMENT_MASK);
        gx_widget_style_add((GX_WIDGET *)text_input, alignment->value);

        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(alignment->text);
        gx_validation_screen_refresh();
        alignment++;

        case_entry = case_list;

        while(case_entry->test_list)
        {
            test = test_list;
            while(test->event_type)
            {
                run_case(text_input, case_entry, frame_id);

                gx_validation_current_frame_id_get(&frame_id);
                gx_validation_set_frame_id(frame_id++);
                gx_validation_set_frame_comment(test->comment);
                
                my_event.gx_event_type = test->event_type;
                my_event.gx_event_payload.gx_event_ushortdata[0] = test->key_value;
                gx_system_event_send(&my_event);

                test++;
            }

            case_entry++;
        }
    }

    gx_validation_end();

    exit(0);
}
