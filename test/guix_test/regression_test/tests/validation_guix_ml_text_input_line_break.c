/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_system.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_ml_text_input_line_break", /* Test name */
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


char test_comment[256];
int  text_alignment[]={GX_STYLE_TEXT_LEFT, GX_STYLE_TEXT_CENTER, GX_STYLE_TEXT_RIGHT};
char test_string[] = "do you remember iiiii      why do the birds";

typedef struct TEST_STRUCT{
INT event_type;
INT key_value;
INT repeat_times;
GX_CHAR *comment;
}TEST;

TEST test_list[]={
{GX_EVENT_MARK_PREVIOUS, 0,                26, "mark previous"},
{GX_EVENT_KEY_DOWN,      GX_KEY_LEFT_ARROW,      1,  "left key down"},
{GX_EVENT_MARK_NEXT,     0,                25, "mark next"},
{GX_EVENT_KEY_DOWN,      GX_KEY_RIGHT_ARROW,     1,  "right key down"},
{GX_EVENT_MARK_UP,       0,                1,  "mark up"},
{GX_EVENT_KEY_DOWN,      GX_KEY_LEFT_ARROW,      1,  "left key down"},
{GX_EVENT_MARK_DOWN,     0,                1,  "mark down"},
{GX_EVENT_KEY_DOWN,      GX_KEY_LEFT_ARROW,      1,  "left key down"},
{GX_EVENT_KEY_DOWN,      GX_KEY_RIGHT_ARROW,     14, "right key down"},
{GX_EVENT_KEY_DOWN,      GX_KEY_LEFT_ARROW,      6,  "left key down"},
{GX_EVENT_KEY_DOWN,      GX_KEY_END,       1,  "end key down"},
{GX_EVENT_KEY_DOWN,      GX_KEY_LEFT_ARROW,      6,  "left key down"},
{GX_EVENT_KEY_DOWN,      GX_KEY_DELETE,    6,  "delete"},
{GX_EVENT_KEY_DOWN,      ' ',              1,  "Insert a space"},
{GX_EVENT_KEY_DOWN,      ' ',              1,  "Insert a space"},
{GX_EVENT_KEY_DOWN,      ' ',              1,  "Insert a space"},
{GX_EVENT_KEY_DOWN,      ' ',              1,  "Insert a space"},
{GX_EVENT_KEY_DOWN,      GX_KEY_BACKSPACE, 4,  "backspace"},
{GX_EVENT_KEY_DOWN,      ' ',              1,  "Insert a space"},
{GX_EVENT_KEY_DOWN,      ' ',              1,  "Insert a space"},
{GX_EVENT_KEY_DOWN,      ' ',              1,  "Insert a space"},
{GX_EVENT_KEY_DOWN,      ' ',              1,  "Insert a space"},
{GX_EVENT_KEY_DOWN,      GX_KEY_DOWN_ARROW,1,  "key down"},
{GX_EVENT_PEN_DOWN,      0,                1,  "pen down"},
{GX_EVENT_PEN_UP,        0,                1,  "pen up"},
{0,                      0,                0,  ""}
};

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int                       frame_id = 1;
int                       align_index;
int                       index;
WINDOW_CONTROL_BLOCK     *main_screen;
GX_EVENT                  my_event;
GX_MULTI_LINE_TEXT_INPUT *text_input;
TEST                     *test;
int                       repeat_times;

    main_screen = (WINDOW_CONTROL_BLOCK *)pMainScreen;
    text_input = &main_screen->window_ml_text_input;

    /* Remove the cursor blink flag. */
    gx_multi_line_text_input_style_remove(text_input, GX_STYLE_CURSOR_BLINK);

    for(align_index = 0; align_index < (int)(sizeof(text_alignment)/sizeof(int)); align_index++)
    {
        if(align_index > 0)
        {
            switch(text_alignment[align_index])
            {
            case GX_STYLE_TEXT_LEFT:
                sprintf(test_comment, "Set text alignment: left, clear input buffer.");
                break;

            case GX_STYLE_TEXT_CENTER:
                sprintf(test_comment, "Set text alignment: center");
                break;

            case GX_STYLE_TEXT_RIGHT:
                sprintf(test_comment, "Set text alignment: right");
                break;
            }

           /* Set text alignment.  */
            gx_widget_style_remove((GX_WIDGET *)text_input, text_alignment[align_index - 1]);
            gx_widget_style_add((GX_WIDGET *)text_input, text_alignment[align_index]);
            gx_multi_line_text_input_buffer_clear(text_input);

            gx_validation_set_frame_id(frame_id++);
            gx_validation_set_frame_comment(test_comment);
            gx_validation_screen_refresh();
        }

        /* Set frame id. */
        gx_validation_set_frame_id(frame_id++);
        sprintf(test_comment, "xpos 400 ypos 300 click inside");
        gx_validation_set_frame_comment(test_comment);
    
        /* Send pen down event to gain input focus. */
        memset(&my_event, 0, sizeof(GX_EVENT));
        my_event.gx_event_display_handle = 1;
        my_event.gx_event_type = GX_EVENT_PEN_DOWN;
        my_event.gx_event_payload.gx_event_pointdata.gx_point_x = 100;
        my_event.gx_event_payload.gx_event_pointdata.gx_point_y = 200;
        gx_system_event_send(&my_event);
    
        my_event.gx_event_type = GX_EVENT_PEN_UP;
        gx_system_event_send(&my_event);

        for(index = 0; index <= (int)(sizeof(test_string)/sizeof(char)); index++)
        {      
            /* Inform the validation system 
            (1) Frame ID, which identifies a specific test configuration;
            (2) Start recording frame on the next toggle operation.
            */
            gx_validation_set_frame_id(frame_id++);
            sprintf(test_comment, "Insert character '%c'", test_string[index]);
            gx_validation_set_frame_comment(test_comment);
    
            my_event.gx_event_type = GX_EVENT_KEY_DOWN;
            my_event.gx_event_payload.gx_event_ushortdata[0] = test_string[index];
            gx_system_event_send(&my_event);
        }

        test = test_list;
        while(test->event_type)
        {
            repeat_times = test->repeat_times;
            my_event.gx_event_type = test->event_type;
            if(test->event_type == GX_EVENT_PEN_DOWN ||
               test->event_type == GX_EVENT_PEN_UP)
            {
                my_event.gx_event_payload.gx_event_pointdata.gx_point_x = 218;
                my_event.gx_event_payload.gx_event_pointdata.gx_point_y = 132;
            }
            else
            {
                my_event.gx_event_payload.gx_event_ushortdata[0] = test->key_value;
            }

            while(repeat_times)
            {
                gx_validation_set_frame_id(frame_id++);
                gx_validation_set_frame_comment(test->comment);

                gx_system_event_send(&my_event);
                repeat_times--;
            }
            test++;
        }
    }

    gx_validation_end();

    exit(0);
}
