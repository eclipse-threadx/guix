/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_sl_input_api_select", /* Test name */
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

static GX_CHAR english_text[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrstuvwxyz";
static GX_CHAR test_comment[255];

typedef struct TEST_STRUCT{
INT event_type;
INT key_value;
INT start_index;
INT end_index;
INT repeat_times;
GX_CHAR *comment;
}TEST;

#define APP_EVENT_SELECT GX_FIRST_USER_EVENT

TEST select_center_all_visible[]={
{GX_EVENT_KEY_DOWN,   GX_KEY_END,    0,   0,  1, "end key down"},
{APP_EVENT_SELECT,                   0,  26, 36,  1, "select text[0, a]"},
{GX_EVENT_KEY_DOWN,   GX_KEY_UP_ARROW, 0, 0,  1,  "focus canvas refresh"},
{0,                                 0,    0,   0, 0, 0}
};

TEST select_center_all_invisible[]={
{GX_EVENT_KEY_DOWN,        GX_KEY_HOME,   0,  0,  1, "home key down"},
{GX_EVENT_KEY_DOWN, GX_KEY_RIGHT_ARROW,   0,  0, 44, "right key down"},
{APP_EVENT_SELECT,                   0,   2, 59,  1, "select text[C:x]"},
{GX_EVENT_KEY_DOWN,   GX_KEY_UP_ARROW, 0, 0,  1,  "focus canvas refresh"},
{0,                                 0,    0,   0, 0, 0}
};

TEST select_left_one_invisible[]={
{GX_EVENT_KEY_DOWN,         GX_KEY_END,   0,  0,   1, "end key down"},
{GX_EVENT_KEY_DOWN,  GX_KEY_RIGHT_ARROW,   0,  0,  34, "right key down"},
{APP_EVENT_SELECT,                    0,   2, 26,   1, "select text[C:0]"},
{GX_EVENT_KEY_DOWN,   GX_KEY_UP_ARROW, 0, 0,  1,  "focus canvas refresh"},
{0,                                 0,    0,   0, 0, 0}
};

TEST select_left_all_invisible[] = {
{GX_EVENT_KEY_DOWN,        GX_KEY_END,   0,  0,  1, "end key down"},
{GX_EVENT_KEY_DOWN, GX_KEY_RIGHT_ARROW,   0,  0, 44, "right key down"},
{APP_EVENT_SELECT,                   0,   2,  5,  1, "select text[C:F]"},
{GX_EVENT_KEY_DOWN,   GX_KEY_UP_ARROW, 0, 0,  1,  "focus canvas refresh"},
{0,                                 0,    0,   0, 0, 0}
};

TEST select_right_one_invisible[] = {
{GX_EVENT_KEY_DOWN,       GX_KEY_HOME,    0,   0, 1, "home key down"},
{ APP_EVENT_SELECT,                 0,   30,  59, 1, "select text[4:x]"},
{GX_EVENT_KEY_DOWN,   GX_KEY_UP_ARROW, 0, 0,  1,  "focus canvas refresh"},
{0,                                 0,    0,   0, 0, 0}
};

TEST select_right_all_invisible[]={
{GX_EVENT_KEY_DOWN,       GX_KEY_HOME,    0,   0, 1, "home key down"},
{APP_EVENT_SELECT,                  0,   51,  59, 1, "select text[p:x]"},
{GX_EVENT_KEY_DOWN,   GX_KEY_UP_ARROW, 0, 0,  1,  "focus canvas refresh"},
{0,                                 0,    0,   0, 0, 0}
};

TEST test_list[]={
{GX_EVENT_KEY_DOWN, GX_KEY_LEFT_ARROW,    0,   0,  1, "left key down"},
{GX_EVENT_KEY_DOWN,GX_KEY_RIGHT_ARROW,    0,   0,  1, "right key down"},
{GX_EVENT_KEY_DOWN,  GX_KEY_BACKSPACE,    0,   0,  1, "backspace"},
{GX_EVENT_KEY_DOWN,     GX_KEY_DELETE,    0,   0,  1, "delete"},
{GX_EVENT_MARK_PREVIOUS,            0,    0,   0,  1, "mark previous"},
{GX_EVENT_MARK_NEXT,                0,    0,   0,  1, "mark next"},
{GX_EVENT_MARK_HOME,                0,    0,   0,  1, "mark home"},
{GX_EVENT_MARK_END,                 0,    0,   0,  1, "mark end"},
{0,                                 0,    0,   0,  0, ""}
};

typedef struct CASE_STRUCT{
TEST *test_list;
GX_CHAR *comment;
}CASE;

CASE case_list[]={
{select_center_all_visible, "select center all visible"},
{select_center_all_invisible, "select center all invisible"},
{select_left_one_invisible, "select left one invisible"},
{select_left_all_invisible, "select left all invisible"},
{select_right_one_invisible, "select right one invisible"},
{select_right_all_invisible, "select right all invisible"},
{GX_NULL, ""}
};

void run_case(GX_SINGLE_LINE_TEXT_INPUT *text_input_ptr, CASE *case_entry, int frame_id, int select_inverse)
{
INT repeat_times;
GX_EVENT my_event;
TEST *test;
TEST *next_test;

    test = case_entry->test_list;
    next_test = case_entry->test_list;

    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_display_handle = 1;

    gx_single_line_text_input_text_set(text_input_ptr, english_text);
   
    while(test->event_type)
    {
        next_test++;
        repeat_times = test->repeat_times;

        while(repeat_times)
        {
            if((repeat_times == 1) && (next_test->event_type == 0))
            {
                gx_validation_set_frame_id(frame_id++);
                gx_validation_set_frame_comment(case_entry->comment);
            }

            if(test->event_type == APP_EVENT_SELECT)
            {
                if(select_inverse)
                {
                    gx_single_line_text_input_text_select(text_input_ptr, test->end_index, test->start_index);
                }
                else
                {
                    gx_single_line_text_input_text_select(text_input_ptr, test->start_index, test->end_index);
                }
            }
            else
            {
                my_event.gx_event_type = test->event_type;
                my_event.gx_event_payload.gx_event_ushortdata[0] = test->key_value;
                gx_system_event_send(&my_event);
            }

            repeat_times--;
        }
        test++;
    }
}

INT alignment_list[]={
GX_STYLE_TEXT_LEFT,
GX_STYLE_TEXT_RIGHT,
GX_STYLE_TEXT_CENTER
};

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
GX_SINGLE_LINE_TEXT_INPUT *text_input_ptr;
GX_EVENT                   my_event;
int                        frame_id = 1;
CASE                      *case_entry;
TEST                      *test;
INT                        select_inverse = 0;
INT                        index;

    text_input_ptr = &pMainScreen->window_sl_text_input;

    /* Remove cursor blink style. */
    gx_single_line_text_input_style_remove(text_input_ptr, GX_STYLE_CURSOR_BLINK);
    gx_text_input_cursor_width_set(&text_input_ptr->gx_single_line_text_input_cursor_instance, 3);

    memset(&my_event, 0, sizeof(GX_EVENT));

    /* Simulate a click event. */
    my_event.gx_event_display_handle = 1;
    my_event.gx_event_type = GX_EVENT_PEN_DOWN;
    my_event.gx_event_payload.gx_event_pointdata.gx_point_x = 150;
    my_event.gx_event_payload.gx_event_pointdata.gx_point_y = 100;
    gx_system_event_send(&my_event);

    my_event.gx_event_type = GX_EVENT_PEN_UP;
    gx_system_event_send(&my_event);

    for(index = 0; index < (int)(sizeof(alignment_list)/sizeof(INT)); index++)
    {
        gx_widget_style_remove((GX_WIDGET *)text_input_ptr, GX_STYLE_TEXT_ALIGNMENT_MASK);
        gx_widget_style_add((GX_WIDGET *)text_input_ptr, alignment_list[index]);

        gx_validation_set_frame_id(frame_id++);
        switch(alignment_list[index])
        {
        case GX_STYLE_TEXT_LEFT:
            sprintf(test_comment, "set left alignment");
            break;
        case GX_STYLE_TEXT_RIGHT:
            sprintf(test_comment, "set right alignment");
            break;
        case GX_STYLE_TEXT_CENTER:
            sprintf(test_comment, "set center alignment");
            break;
        }
        gx_validation_set_frame_comment(test_comment);
        gx_validation_screen_refresh();

        case_entry = case_list;

        while(case_entry->test_list)
        {
            test = test_list;

            while(test->event_type)
            {
                for(select_inverse = 0; select_inverse <= 1; select_inverse ++)
                {
                    run_case(text_input_ptr, case_entry, frame_id, select_inverse);

                    gx_validation_current_frame_id_get(&frame_id);
                    gx_validation_set_frame_id(frame_id++);
                    sprintf(test_comment, "%s, select inverse: %d", test->comment, select_inverse);
                    gx_validation_set_frame_comment(test_comment);

                    my_event.gx_event_type = test->event_type;
                    my_event.gx_event_payload.gx_event_ushortdata[0] = test->key_value;
                    gx_system_event_send(&my_event);
                }
                test++;
            }

            case_entry++;
        }
    }

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
