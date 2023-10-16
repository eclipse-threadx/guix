/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_text_button_24xrgb", /* Test name */
    0, 0, 640, 480  /* Define the coordinates of the capture area.
                         In this test, we only need to capture the text button
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

    /* Call the actual text button routine. */
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

#ifndef TEST_MODE
#define TEST_MODE
#endif

#include "gx_validation_wrapper.h"

#include "demo_guix_all_widgets.c"
/* Define the text on the text button. */
static char test_text[]="test text";
char test_comment[256];

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int frame_id = 1;
GX_EVENT my_event;
GX_TEXT_BUTTON *text_button;

    memset(&my_event, 0, sizeof(GX_EVENT));

    gx_widget_find(pButtonScreen, ID_TEXT_BUTTON, 0, &text_button);
    
    /* Set text color. */
    gx_text_button_text_color_set(text_button, GX_COLOR_ID_BLUE, GX_COLOR_ID_ORANGE, GX_COLOR_ID_BLUE);

    gx_validation_set_frame_id(frame_id++);
    sprintf(test_comment, "set normal text color=blue, selected text color=orange");
    gx_validation_set_frame_comment(test_comment);

     /* Force a screen refresh. */
    gx_validation_screen_refresh();

    gx_text_button_font_set(text_button, GX_FONT_ID_NORMAL_FONT);

    gx_validation_set_frame_id(frame_id++);
    sprintf(test_comment, "set text font to normal font");
    gx_validation_set_frame_comment(test_comment);

    /* Force a screen refresh. */
    gx_validation_screen_refresh();

    /* set new button text.  */
    gx_text_button_text_set(text_button, test_text);

    gx_validation_set_frame_id(frame_id++);
    sprintf(test_comment, "set button text to \"%s\"", test_text);
    gx_validation_set_frame_comment(test_comment);

    /* Force a screen refresh. */
    gx_validation_screen_refresh();

    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_payload.gx_event_pointdata.gx_point_x = text_button->gx_widget_size.gx_rectangle_left + 1;
    my_event.gx_event_payload.gx_event_pointdata.gx_point_y = text_button->gx_widget_size.gx_rectangle_top + 1;
    my_event.gx_event_display_handle = 1;
    my_event.gx_event_type = GX_EVENT_PEN_DOWN;
    gx_system_event_send(&my_event);

    gx_validation_set_frame_id(frame_id++);
    sprintf(test_comment, "simulate pen down inside text button bounding box");
    gx_validation_set_frame_comment(test_comment);

    /* Force a screen refresh. */
    gx_validation_screen_refresh();

    my_event.gx_event_type = GX_EVENT_PEN_UP;
    gx_system_event_send(&my_event);
    gx_validation_set_frame_id(frame_id++);
    sprintf(test_comment, "simulate pen up inside text button bounding box");
    gx_validation_set_frame_comment(test_comment);

    /* Force a screen refresh. */
    gx_validation_screen_refresh();

    /* Close modal window. */
    my_event.gx_event_type = GX_SIGNAL(IDB_CANCEL, GX_EVENT_CLICKED);
    my_event.gx_event_target = (GX_WIDGET *)pPopupScreen;
    my_event.gx_event_sender = IDB_CANCEL;
    gx_system_event_send(&my_event);
    gx_validation_set_frame_id(frame_id++);
    sprintf(test_comment, "simulate click on cancel button to close modal dialog");
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();

    my_event.gx_event_type = GX_EVENT_PEN_DOWN;
    my_event.gx_event_payload.gx_event_pointdata.gx_point_x = text_button->gx_widget_size.gx_rectangle_left - 1;;
    my_event.gx_event_payload.gx_event_pointdata.gx_point_y = text_button->gx_widget_size.gx_rectangle_top - 1;;
    gx_system_event_send(&my_event);
    gx_validation_set_frame_id(frame_id++);
    sprintf(test_comment, "simulate pen down outside text button bounding box");
    gx_validation_set_frame_comment(test_comment);

    /* Force a screen refresh. */
    gx_validation_screen_refresh();

    my_event.gx_event_type = GX_EVENT_PEN_UP;
    gx_system_event_send(&my_event);
    gx_validation_set_frame_id(frame_id++);
    sprintf(test_comment, "simulate pen up event outside text button bounding box");
    gx_validation_set_frame_comment(test_comment);

    /* Force a screen refresh. */
    gx_validation_screen_refresh();
    
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
