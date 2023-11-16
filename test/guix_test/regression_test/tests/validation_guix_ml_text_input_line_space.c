/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_system.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_ml_text_input_line_space", /* Test name */
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

GX_CHAR test_string[]="why do the birds\rgo on singging\rwhy do the seas rush the shore";

char test_comment[256];

typedef struct TEST_STRUCT{
INT event_type;
INT key_value;
GX_VALUE point_x;
GX_VALUE point_y;
GX_CHAR *comment;
}TEST;

TEST test_list[]={
{GX_EVENT_PEN_DOWN,                     0,   43,  133, "pen down on (43, 133)"},
{GX_EVENT_PEN_UP,                       0,   43,  133, "pen up on (43, 133)"},
{GX_EVENT_KEY_DOWN,                   'T',    0,    0, "insert character 'T'"},
{GX_EVENT_KEY_DOWN,     GX_KEY_LEFT_ARROW,    0,    0, "left arrow down"},
{GX_EVENT_KEY_DOWN,      GX_KEY_BACKSPACE,    0,    0, "backspace"},
{GX_EVENT_KEY_DOWN,     GX_KEY_LEFT_ARROW,    0,    0, "left arrow"},
{GX_EVENT_KEY_DOWN,         GX_KEY_DELETE,    0,    0, "delete"},
{GX_EVENT_KEY_DOWN,     GX_KEY_DOWN_ARROW,    0,    0, "down arrow"},
{GX_EVENT_KEY_DOWN,     GX_KEY_DOWN_ARROW,    0,    0, "down arrow"},
{GX_EVENT_KEY_DOWN,       GX_KEY_UP_ARROW,    0,    0, "up arrow"},
{GX_EVENT_MARK_DOWN,                    0,    0,    0, "mark down"},
{GX_EVENT_MARK_UP,                      0,    0,    0, "mark up"},
{GX_EVENT_MARK_PREVIOUS,                0,    0,    0, "mark previous"},
{GX_EVENT_MARK_PREVIOUS,                0,    0,    0, "mark previous"},
{GX_EVENT_MARK_NEXT,                    0,    0,    0, "mark next"},
{GX_EVENT_PEN_DOWN,                     0,   43,  133, "pen down on (43, 133)"},
{0,                                     0,    0,    0, ""}
};

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int                       frame_id = 1;
GX_MULTI_LINE_TEXT_INPUT *text_input;
GX_EVENT                  my_event;
TEST                     *test;
GX_FONT                  *font;
int                       line_height;

    text_input = &((WINDOW_CONTROL_BLOCK *)pMainScreen)->window_ml_text_input;

    gx_widget_font_get((GX_WIDGET *)text_input, text_input->gx_multi_line_text_view_font_id, &font);
    line_height = font->gx_font_line_height;
    
     /* Remove the cursor blink flag. */
    gx_multi_line_text_input_style_remove(text_input, GX_STYLE_CURSOR_BLINK | GX_STYLE_CURSOR_ALWAYS);

    gx_multi_line_text_view_line_space_set((GX_MULTI_LINE_TEXT_VIEW *)text_input, -line_height);

    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("set line height to negative of font height");
    gx_validation_screen_refresh();

    /* Assign focus to text input. */
    gx_system_focus_claim((GX_WIDGET *)text_input);

    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_display_handle = 1;

    gx_multi_line_text_input_text_set(text_input, test_string);

    test = test_list;
  
    while(test->event_type)
    {
        my_event.gx_event_type = test->event_type;
        my_event.gx_event_target = (GX_WIDGET *)text_input;
  
        if(test->event_type == GX_EVENT_PEN_DOWN ||
           test->event_type == GX_EVENT_PEN_UP)
        {
            my_event.gx_event_payload.gx_event_pointdata.gx_point_x = test->point_x;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_y = test->point_y;
        }
        else
        {
            my_event.gx_event_payload.gx_event_ushortdata[0] = test->key_value;
        }
 
        gx_system_event_send(&my_event);
        
        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(test->comment);
        gx_validation_screen_refresh();

        test++;
    }

    gx_multi_line_text_view_line_space_set((GX_MULTI_LINE_TEXT_VIEW *)text_input, -2);
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("set line space -2");
    gx_validation_screen_refresh();

    gx_validation_end();

    exit(0);
}
