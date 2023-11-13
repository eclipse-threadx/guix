/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_all_widgets_checkbox_draw", /* Test name */
    145, 315, 258, 355  /* Define the coordinates of the capture area.
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
    {GX_STYLE_TEXT_ALIGNMENT_MASK, GX_STYLE_TEXT_RIGHT,     "text right alignment"},
    {GX_STYLE_TEXT_ALIGNMENT_MASK, GX_STYLE_TEXT_CENTER,    "text center alignment"},
    {GX_STYLE_TEXT_ALIGNMENT_MASK, GX_STYLE_TEXT_LEFT,      "text left alignment"},
    {0,                            GX_STYLE_ENABLED,        "Enabled"},
    {0,                            GX_STYLE_BUTTON_PUSHED,  "Pushed"},
    {GX_STYLE_BUTTON_PUSHED,       0,                       "Not Pushed"},
    {GX_STYLE_ENABLED,             0,                       "Not Enabled"},
    {0,                            GX_STYLE_BUTTON_PUSHED,  "Pushed"},
    {GX_STYLE_BUTTON_PUSHED,       0,                       "Not Pushed"},
    {GX_STYLE_TRANSPARENT,         0,                       "Remove Transparent"},
    {0, 0, 0}
};

static char test_comment[255];

static VOID control_thread_entry(ULONG input)
{
int          frame_id = 1;
GX_CHECKBOX *checkbox;
TEST        *test;
static GX_CHAR  text[]="test";
GX_STRING       string;
INT             index;

    gx_widget_find(pButtonScreen, ID_CHECKBOX, 0, &checkbox);

    for(index = 0; index < 2; index++)
    {
        if(index)
        {
            gx_checkbox_pixelmap_set(checkbox, GX_NULL, GX_NULL, GX_NULL, GX_NULL);

            string.gx_string_ptr = text;
            string.gx_string_length = sizeof(text) - 1;
            gx_text_button_text_set_ext((GX_TEXT_BUTTON *)checkbox, &string);
        }
        else
        {
            gx_checkbox_pixelmap_set(checkbox, GX_PIXELMAP_ID_CHECKBOX_OFF, GX_PIXELMAP_ID_CHECKBOX_ON,
                                     GX_PIXELMAP_ID_RADIO_OFF, GX_PIXELMAP_ID_RADIO_ON);
        }
        
        test = test_list;
        while(test->comments)
        {
            
            gx_widget_style_remove((GX_WIDGET *)checkbox, test->remove_style);
            gx_widget_style_add((GX_WIDGET *)checkbox, test->add_style);
            
            gx_validation_set_frame_id(frame_id++);
            gx_validation_set_frame_comment(test->comments);
            gx_system_dirty_mark((GX_WIDGET *)checkbox);
            gx_validation_screen_refresh();
            test++;
        }
    }

    gx_text_button_text_set((GX_TEXT_BUTTON *)checkbox, GX_NULL);
    sprintf(test_comment, "set checkbox text to null");
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
