/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_all_widgets_ml_text_button", /* Test name */
    117, 176, 253, 245  /* Define the coordinates of the capture area.
                         In this test, we only need to capture the line
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

typedef struct TEST_STRUCT{
    INT remove_style;
    INT add_style;
    GX_CHAR *comments;
}TEST;

TEST test_list[]={
    {GX_STYLE_TEXT_ALIGNMENT_MASK, GX_STYLE_TEXT_RIGHT,  "text right alignment"},
    {GX_STYLE_TEXT_ALIGNMENT_MASK, GX_STYLE_TEXT_CENTER, "text center alignment"},
    {GX_STYLE_TEXT_ALIGNMENT_MASK, GX_STYLE_TEXT_LEFT,   "text left alignment"},
    {0, 0, 0}
};

GX_CHAR test_string[]={"test\rmulti\nline\r\nbutton\rtest"};
GX_CHAR test_comment[256];

static VOID control_thread_entry(ULONG input)
{
int              frame_id = 1;
GX_MULTI_LINE_TEXT_BUTTON  *button;
TEST            *test;
int              index;
GX_STRING        string;

    button = &button_screen.button_screen_multi_line_button_1;
    gx_widget_style_add((GX_WIDGET *)button, GX_STYLE_TEXT_COPY);
    gx_multi_line_text_button_text_set(button, test_string);
   
    test = test_list;
    while(test->comments)
    {
        gx_widget_style_remove((GX_WIDGET *)button, test->remove_style);
        gx_widget_style_add((GX_WIDGET *)button, test->add_style);
        
        for(index = 0; index < 2; index++)
        {
            sprintf(test_comment, "%s", test->comments);

            if(index)
            {
                gx_widget_style_add((GX_WIDGET *)button, GX_STYLE_BUTTON_PUSHED);
                strcat(test_comment, "button pushed");
            }
            else
            {
                gx_widget_style_remove((GX_WIDGET *)button, GX_STYLE_BUTTON_PUSHED);
            }
            gx_validation_set_frame_id(frame_id++);
            gx_validation_set_frame_comment(test_comment);
            gx_validation_screen_refresh();
        }
        test++;
    }
    sprintf(test_comment, "Set multi line text button text id.");
    gx_multi_line_text_button_text_id_set(button, GX_STRING_ID_STRING_39);
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();

    gx_multi_line_text_button_text_set(button, "");
    gx_validation_set_frame_id(frame_id++);
    sprintf(test_comment, "set null string to multi line text button");
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();

    string.gx_string_ptr = test_string;
    string.gx_string_length = strnlen(test_string, sizeof(test_string));
    gx_multi_line_text_button_text_set_ext(button, &string);
    gx_validation_set_frame_id(frame_id++);
    sprintf(test_comment, "set text button text");
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();

    gx_text_button_font_set((GX_TEXT_BUTTON *)button, 1000);
    gx_validation_set_frame_id(frame_id++);
    sprintf(test_comment, "set font to an invalid font id");
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
