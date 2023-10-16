/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_utility_utf8_string_character", /* Test name */
    55, 241, 306, 395  /* Define the coordinates of the capture area.
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
GX_CHAR text[] ={0x20, 0x20, 0x20}; 

GX_CHAR *string_table[2] =
{
    GX_NULL,
    text
};

GX_CHAR **language_table[1] = 
{
    string_table,
};

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int                        frame_id = 1;
GX_MULTI_LINE_TEXT_VIEW   *ml_text_view = &text_screen.text_screen_text_view_1;
GX_DISPLAY *display = root->gx_window_root_canvas->gx_canvas_display;

    sprintf(test_comment, "This is the original text screen");
    ToggleScreen((GX_WINDOW *)&text_screen, (GX_WINDOW *)&button_screen);
    gx_system_timer_stop(pTextScreen, NUMBER_ANIMATION_TIMER);
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();
    
    gx_multi_line_text_view_font_set(ml_text_view, GX_FONT_ID_CJK);
    gx_validation_set_frame_id(frame_id++);
    sprintf(test_comment, "Set font id of multi line text view to simkai.");
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();

    gx_multi_line_text_view_text_id_set(ml_text_view, GX_STRING_ID_STRING_92);
    sprintf(test_comment, "Set Chinese text to text view.");
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();

    gx_multi_line_text_view_text_id_set(ml_text_view, GX_STRING_ID_STRING_93);
    sprintf(test_comment, "Set Latin text to text view.");
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();
    
    gx_display_language_table_set(display, language_table, 1, 2);
    
    gx_multi_line_text_view_text_id_set(ml_text_view, 1);
    sprintf(test_comment, "Set invalid text to text view.");
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();
    
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
