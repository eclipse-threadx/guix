/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_system.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_ml_text_input_api_select", /* Test name */
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

#define APP_EVENT_SELECT GX_FIRST_USER_EVENT

char test_comment[256];

typedef struct TEST_STRUCT{
INT event_type;
GX_VALUE key_value;
GX_VALUE start_index;
GX_VALUE end_index;
INT      scroll_value;
GX_CHAR *comment;
}TEST;

TEST select_center_all_visible[]={
{GX_EVENT_PEN_DOWN,        0,  66,  266,  0,   "pen down on (66, 266)"},
{GX_EVENT_VERTICAL_SCROLL, 0,   0,   0,   578, "scroll up"},
{APP_EVENT_SELECT,         0, 106, 113,   0, "mark previous" },
{0,                        0,   0,   0,   0, ""}
};

TEST select_center_all_invisible[]={
{GX_EVENT_PEN_DOWN,        0,  66,  266,  0,   "pen down on (66, 266)"},
{GX_EVENT_VERTICAL_SCROLL, 0,   0,   0,   578, "scroll up"},
{APP_EVENT_SELECT,         0,  84, 141,   0, "mark previous" },
{0,                        0,   0,   0,   0, ""}
};

TEST select_top_one_invisible[]={
{GX_EVENT_PEN_DOWN,        0,  66,  266,  0,   "pen down on (66, 266)"},
{GX_EVENT_VERTICAL_SCROLL, 0,   0,   0,   578, "scroll up"},
{APP_EVENT_SELECT,         0,  84, 106,   0, "mark previous" },
{0,                        0,   0,   0,   0, ""}
};

TEST select_top_all_invisible[]={
{GX_EVENT_PEN_DOWN,        0,  66,  266,  0,   "pen down on (66, 266)"},
{GX_EVENT_VERTICAL_SCROLL, 0,   0,   0,   578, "scroll up"},
{APP_EVENT_SELECT,         0,  76,  84,   0, "mark previous" },
{0,                        0,   0,   0,   0, ""}
};

TEST select_bottom_one_invisible[]={
{GX_EVENT_PEN_DOWN,        0,  66,  266,  0,   "pen down on (66, 266)"},
{GX_EVENT_VERTICAL_SCROLL, 0,   0,   0,   578, "scroll up"},
{APP_EVENT_SELECT,         0, 106, 141,   0, "mark previous" },
{0,                        0,   0,   0,   0, ""}
};

TEST select_bottom_all_invisible[]={
{GX_EVENT_PEN_DOWN,        0,  66,  266,  0,   "pen down on (66, 266)"},
{GX_EVENT_VERTICAL_SCROLL, 0,   0,   0,   578, "scroll up"},
{APP_EVENT_SELECT,         0, 141, 149,   0, "mark previous" },
{0,                        0,   0,   0,   0, ""}
};

typedef struct CASE_STRUCT{
TEST *test_list;
GX_CHAR *comment;
}CASE;

CASE case_list[]={
{select_center_all_visible, "select center all visible"},
{select_center_all_invisible, "select center all invisible"},
{select_top_one_invisible, "select top one invisible"},
{select_top_all_invisible, "select top all invisible"},
{select_bottom_one_invisible, "select_bottom_one_invisible"},
{select_bottom_all_invisible, "select_bottom_all_invisible"},
{0, ""}
};

TEST test_list[]={
{GX_EVENT_KEY_DOWN, GX_KEY_HOME,       0, 0, 0, "home key down"},
{GX_EVENT_KEY_DOWN, GX_KEY_END,        0, 0, 0, "end key down"},
{GX_EVENT_KEY_DOWN, GX_KEY_LEFT_ARROW, 0, 0, 0, "left arrow down"},
{GX_EVENT_KEY_DOWN, GX_KEY_RIGHT_ARROW,0, 0, 0, "right arrow down"},
{GX_EVENT_KEY_DOWN, GX_KEY_BACKSPACE,  0, 0, 0, "backspace"},
{GX_EVENT_KEY_DOWN, GX_KEY_DELETE,     0, 0, 0, "delete"},
{GX_EVENT_KEY_DOWN, 'I',               0, 0, 0, "insert character I"},
{GX_EVENT_CUT,      0,                 0, 0, 0, "cut"},
{GX_EVENT_PASTE,    0,                 0, 0, 0, "paste"},
{GX_EVENT_MARK_PREVIOUS, 0,            0, 0, 0, "mark previous"},
{GX_EVENT_MARK_NEXT,     0,            0, 0, 0, "mark next"},
{GX_EVENT_MARK_UP,       0,            0, 0, 0, "mark up"},
{GX_EVENT_MARK_DOWN,     0,            0, 0, 0, "mark down"},
{GX_EVENT_MARK_HOME,     0,            0, 0, 0, "mark home"},
{GX_EVENT_MARK_END,      0,            0, 0, 0, "mark end"},
{0,                 0,                 0, 0, 0, ""}
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

void run_case(GX_MULTI_LINE_TEXT_INPUT *input, CASE *case_entry, int frame_id, int select_inverse)
{
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
        if((test + 1)->event_type == 0)
        {
            gx_validation_set_frame_id(frame_id++);
            gx_validation_set_frame_comment(case_entry->comment);
        }

        my_event.gx_event_type = test->event_type;
        my_event.gx_event_target = (GX_WIDGET *)input;

        switch(test->event_type)
        {
        case GX_EVENT_PEN_DOWN:
            my_event.gx_event_payload.gx_event_pointdata.gx_point_x = test->start_index;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_y = test->end_index;
            my_event.gx_event_target = 0;
            break;

        case GX_EVENT_VERTICAL_SCROLL:
            my_event.gx_event_payload.gx_event_intdata[0] = test->scroll_value;
            my_event.gx_event_payload.gx_event_intdata[1] = scroll->gx_scrollbar_info.gx_scroll_value;
            break;
        case APP_EVENT_SELECT:
            if(select_inverse)
            {
                gx_multi_line_text_input_text_select(input, test->end_index, test->start_index);
            }
            else
            {
                gx_multi_line_text_input_text_select(input, test->start_index, test->end_index);
            }
            break;
        }
        gx_system_event_send(&my_event);
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
INT                       select_inverse;

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
                for(select_inverse = 0; select_inverse <= 1; select_inverse ++)
                {
                    run_case(text_input, case_entry, frame_id, select_inverse);

                    my_event.gx_event_type = test->event_type;
                    my_event.gx_event_payload.gx_event_ushortdata[0] = test->key_value;
                    gx_system_event_send(&my_event);

                    gx_validation_current_frame_id_get(&frame_id);
                    gx_validation_set_frame_id(frame_id++);
                    sprintf(test_comment, "%s, select_inverse:%d", test->comment, select_inverse);
                    gx_validation_set_frame_comment(test_comment);
                    gx_validation_screen_refresh();
                }

                test++;
            }

            case_entry++;
        }
    }

    gx_validation_end();

    exit(0);
}
