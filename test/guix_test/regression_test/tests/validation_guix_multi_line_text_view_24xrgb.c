/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_multi_line_text_view_24xrgb", /* Test name */
    46, 232, 316, 404  /* Define the coordinates of the capture area.
                         In this test, we only need to capture the multi 
                         line text view drawing area.  */
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
       on the multi line text view example. These operations simulate 
       user input. */
    gx_validation_control_thread_create(control_thread_entry);

    /* Terminate the test if it runs for more than 100 ticks */
    /* This function is not implemented yet. */
    gx_validation_watchdog_create(100);

    /* Call the actual multi line text view routine. */
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

/* Define the text to display in the multi_line_text_view. */
static char text_1[] = "";
static char text_2[] = "1234";
static char text_3[] = "1234 abcd ";
static char text_4[] = "0123456789abcdefghijklmnopqrstuvwxyz";
static char text_5[] = "0123456789 abcdefghijklmnopqrstuvwxyz ";
static char text_6[] = "0123456789 abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz";
static char text_7[] = "0123456789abcdefghijklmnopqrstuvwxyz0123456789abcdefghijklmnopqrstuvwxyz0123456789abcdefghijklmnopqrstuvwxyz0123456789abcdefghijklmnopqrstuvwxyz0123456789abcdefghijklmnopqrstuvwxyz0123456789abcdefghijklmnopqrstuvwxyz0123456789abcdefghijklmnopqrstuvwxyz";
static char text_8[] = "0123456789 abcdefghijklmnopqrstuvwxyz 0123456789 abcdefghijklmnopqrstuvwxyz 0123456789 abcdefghijklmnopqrstuvwxyz 0123456789 abcdefghijklmnopqrstuvwxyz 0123456789 abcdefghijklmnopqrstuvwxyz 0123456789 abcdefghijklmnopqrstuvwxyz 0123456789 abcdefghijklmnopqrstuvwxyz ";

/* Define the text array.*/
static char *text_array[] = {text_1, text_2, text_3, text_4, text_5, text_6, text_7, text_8};

char test_comment[256];

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int text_index;
int frame_id = 1;
TEXT_SCREEN_CONTROL_BLOCK *text_screen;

    text_screen = (TEXT_SCREEN_CONTROL_BLOCK *)pTextScreen;

    /* Toggle button_screen to text_screen. */
    ToggleScreen(pTextScreen, pButtonScreen);

    gx_multi_line_text_view_text_color_set(&text_screen -> text_screen_text_view_1, GX_COLOR_ID_ORANGE, GX_COLOR_ID_ORANGE, GX_COLOR_ID_ORANGE);
    for(text_index = 0; text_index < 8; text_index++)
    {
        /* Inform the validation system 
        (1) Frame ID, which identifies a specific test configuration;
        (2) Start recording frame on the next toggle operation.
        */
        gx_validation_set_frame_id(frame_id);
        switch(text_index)
        {
        case 0:
            sprintf(test_comment, "this is an empty string");
            break;
        case 1:
            sprintf(test_comment, "a word shorter than the widget width");
            break;
        case 2:
            sprintf(test_comment, "serval words shorter than the widget width");
            break;
        case 3:
            sprintf(test_comment, "a word longer than the widget width");
            break;
        case 4:
            sprintf(test_comment, "serval words longer than the widget width");
            break;
        case 5:
            sprintf(test_comment, "serval words the second word longer than the widget width");
            break;
        case 6:
            sprintf(test_comment, "a word longer than the visible area");
            break;
        case 7:
            sprintf(test_comment, "serval words longer than the visible area");
            break;
        }
        gx_validation_set_frame_comment(test_comment);

        /* Set the text to be displayed in the multi line text view window. */
        gx_multi_line_text_view_text_set(&text_screen -> text_screen_text_view_1, text_array[text_index]);

        /* Mark the window "dirty". */
        gx_system_dirty_mark(pTextScreen);

        /* Force a screen refresh. */
        gx_validation_screen_refresh();

        /* Increment frame ID. */
        frame_id ++;
    }

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}





