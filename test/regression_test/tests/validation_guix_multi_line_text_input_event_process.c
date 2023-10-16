/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_multi_line_text_input_event_process", /* Test name */
    321, 242, 572, 394  /* Define the coordinates of the capture area.
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
#ifdef win32_graphics_driver_setup_24xrgb  
#undef win32_graphics_driver_setup_24xrgb  
#endif
#define win32_graphics_driver_setup_24xrgb  gx_validation_graphics_driver_setup_24xrgb


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"
#include "demo_guix_all_widgets.c"

char test_comment[255];

typedef struct TEST_STRUCT{
    USHORT         key_value;
    GX_CHAR       *comment;
}TEST;

TEST test_list[] ={
    {GX_KEY_BACKSPACE, "Backspace key"},
    {GX_KEY_DELETE, "delete key"},
    {GX_KEY_LEFT_ARROW, "left arrow key"},
    {GX_KEY_RIGHT_ARROW, "right arrow key"},
    {GX_KEY_UP_ARROW, "up arrow key"},
    {GX_KEY_DOWN_ARROW, "down arrow key"},
    {GX_KEY_CARRIAGE_RETURN, "carriage return key"},
    {GX_KEY_LINE_FEED, "line feed key"},
    {GX_KEY_SPACE, "space key"},
    {GX_KEY_HOME, "home key"},
    {GX_KEY_END, "end key"},
    {GX_KEY_PAGE_UP, "page up key"},  /* we don't support page up and down key for current. If this event is supported in future, this test should be updated. */
    {GX_KEY_PAGE_DOWN, "page down key"}
};

static char test_text[] = "It is universally\n acknowledged that\n trust is\n one of the\n most valuable\n assets for\n interpersonal communication. \nHowever, it is \nnot uncommon to have\n misunderstanding and generation\n gap between teachers and students. with the current trend\n of communication becoming necessary and\n indispensable in this ever-changing modern society, building trust is of great\n significance.In order to set up the credibility between teachers and students, on the one hand, as    teachers, we should sincerely deal with the students’ problems and difficulties, comprehending their necessities. On the other hand, as students, it is necessary that more understanding and respect should be given. Becoming good friends to have the transpositional consideration each other can build a harmonious atmosphere.";

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
GX_EVENT my_event;
int  index;
GX_MULTI_LINE_TEXT_INPUT *text_input;
int frame_id = 1;

    text_input = &text_screen.text_screen_text_input_2;
    /* Remove the cursor blink flag. */
    gx_multi_line_text_input_style_remove(text_input, GX_STYLE_CURSOR_BLINK);
    gx_multi_line_text_input_style_add(text_input, GX_STYLE_CURSOR_ALWAYS);
    gx_multi_line_text_input_text_set(text_input, test_text);
    
    gx_widget_detach(pButtonScreen);
    gx_widget_attach(root, pTextScreen);
    
    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_display_handle = 1;
    my_event.gx_event_type = GX_EVENT_PEN_DOWN;
    my_event.gx_event_payload.gx_event_pointdata.gx_point_x = text_input->gx_widget_size.gx_rectangle_left + 10;
    my_event.gx_event_payload.gx_event_pointdata.gx_point_y = text_input->gx_widget_size.gx_rectangle_top + 10;
        
    gx_validation_set_frame_id(frame_id++);
    sprintf(test_comment, "Toggle to text screen. Set text to ml input.");
    gx_validation_set_frame_comment(test_comment);
    /* Force a screen refresh. */
    gx_validation_screen_refresh();
    
    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_display_handle = 1;
    my_event.gx_event_type = GX_EVENT_KEY_DOWN;
    my_event.gx_event_target = (GX_WIDGET *)text_input;
    for(index = 0; index < (int)(sizeof(test_list)/sizeof(TEST)); index++)
    {
            /* Send pen down event to gain input focus. */

        my_event.gx_event_payload.gx_event_ushortdata[0] = test_list[index].key_value;

        gx_system_event_send(&my_event);
        
        gx_validation_set_frame_id(frame_id++);
        strcpy(test_comment, test_list[index].comment);
        gx_validation_set_frame_comment(test_comment);
        /* Force a screen refresh. */
        gx_validation_screen_refresh();
    }
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();
    exit(0);
}
