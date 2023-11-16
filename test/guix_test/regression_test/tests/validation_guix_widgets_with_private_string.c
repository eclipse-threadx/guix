/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_widgets_with_private_string", /* Test name */
    30, 30, 610, 450  /* Define the coordinates of the capture area.
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
    /* Call the actual line example routine. */
    gx_validation_application_define(first_unused_memory);
    
    /* Termiante the test if it runs for more than 100 ticks */
    /* This function is not implemented yet. */
    gx_validation_watchdog_create(100);

    /* Create a dedicated thread to perform various operations
       on the line drawing example. These operations simulate 
       user input. */
    gx_validation_control_thread_create(control_thread_entry);



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

#include "demo_guix_widgets_with_private_string.c"

char test_comment[255];

static GX_STRING_SCROLL_WHEEL *string_scroll_wheel_runtime = 0;
static GX_STRING_SCROLL_WHEEL *string_scroll_wheel = 0;
static GX_MULTI_LINE_TEXT_VIEW *multi_line_text_view= 0;
static GX_PROMPT *prompt = 0;
static GX_PIXELMAP_PROMPT *pixelmap_prompt= 0;
static GX_TEXT_BUTTON *text_button = 0;
static GX_RADIO_BUTTON *radio_button = 0;
static GX_CHECKBOX *checkbox = 0;
static GX_MULTI_LINE_TEXT_BUTTON *multi_line_text_button = 0;

typedef struct test_content_struct
{
    GX_WIDGET *widget;
    GX_CHAR *comment;
}
TEST_CONTENT;


static VOID control_thread_entry(ULONG input)
{
int       frame_id = 1;
int       index;

    gx_widget_find(pScrollWheelScreen, ID_MONTH_WHEEL_RUNTIME, GX_SEARCH_DEPTH_INFINITE, &string_scroll_wheel_runtime);
    gx_widget_find(pScrollWheelScreen, ID_MONTH_WHEEL, GX_SEARCH_DEPTH_INFINITE, &string_scroll_wheel);
    gx_widget_find(pScrollWheelScreen, ID_MULTI_LINE_TEXT_VIEW, GX_SEARCH_DEPTH_INFINITE, &multi_line_text_view);
    gx_widget_find(pScrollWheelScreen, ID_PROMPT, GX_SEARCH_DEPTH_INFINITE, &prompt);
    gx_widget_find(pScrollWheelScreen, ID_PIXELMAP_PROMPT, GX_SEARCH_DEPTH_INFINITE, &pixelmap_prompt);
    gx_widget_find(pScrollWheelScreen, ID_TEXT_BUTTON, GX_SEARCH_DEPTH_INFINITE, &text_button);
    gx_widget_find(pScrollWheelScreen, ID_RADIO_BUTTON, GX_SEARCH_DEPTH_INFINITE, &radio_button);
    gx_widget_find(pScrollWheelScreen, ID_CHECKBOX, GX_SEARCH_DEPTH_INFINITE, &checkbox);
    gx_widget_find(pScrollWheelScreen, ID_MULTI_LINE_TEXT_BUTTON, GX_SEARCH_DEPTH_INFINITE, &multi_line_text_button);
   
   {
        TEST_CONTENT test_widget_list[] = 
        {
        {(GX_WIDGET *)string_scroll_wheel_runtime, "Delete string_scroll_wheel_runtime"},
        {(GX_WIDGET *)string_scroll_wheel, "Delete string_scroll_wheel"},
        {(GX_WIDGET *)multi_line_text_view, "Delete multi_line_text_view"},
        {(GX_WIDGET *)prompt, "Delete prompt"},
        {(GX_WIDGET *)pixelmap_prompt, "Delete pixelmap_prompt"},
        {(GX_WIDGET *)text_button, "Delete text_button"},
        {(GX_WIDGET *)radio_button, "Delete radio_button"},
        {(GX_WIDGET *)checkbox, "Delete checkbox"},
        {(GX_WIDGET *)multi_line_text_button, "Delete multi_line_text_button"}
        };
        /* call text set function to test private string copy */    
        gx_string_scroll_wheel_string_list_set(string_scroll_wheel, GX_NULL, 0); //set string list to 0
        gx_validation_set_frame_id(frame_id++); 
        gx_validation_set_frame_comment("Set string list to null for string scroll wheel.");
        gx_system_dirty_mark(pScrollWheelScreen);
        /* Force a screen refresh. */
        gx_validation_screen_refresh();
        
        gx_string_scroll_wheel_string_list_set(string_scroll_wheel_runtime, GX_NULL, 0); //set string list to 0
        gx_validation_set_frame_id(frame_id++); 
        gx_validation_set_frame_comment("Set string list to null for string scroll wheel runtime.");
        gx_system_dirty_mark(pScrollWheelScreen);
        /* Force a screen refresh. */
        gx_validation_screen_refresh();

                
        gx_multi_line_text_view_text_set(multi_line_text_view, 0);
        gx_validation_set_frame_id(frame_id++); 
        gx_validation_set_frame_comment("Set string to null for multi_line_text_view.");
        gx_system_dirty_mark(pScrollWheelScreen);
        /* Force a screen refresh. */
        gx_validation_screen_refresh();
        
        gx_multi_line_text_view_text_set(multi_line_text_view, "this is multi line text view");
        gx_validation_set_frame_id(frame_id++); 
        gx_validation_set_frame_comment("Set string to \"this is multi line text view\" for multi_line_text_view.");
        gx_system_dirty_mark(pScrollWheelScreen);
        /* Force a screen refresh. */
        gx_validation_screen_refresh();
                
        gx_prompt_text_set(prompt, 0);
        gx_validation_set_frame_id(frame_id++); 
        gx_validation_set_frame_comment("Set text null for prompt.");
        gx_system_dirty_mark(pScrollWheelScreen);
        /* Force a screen refresh. */
        gx_validation_screen_refresh();
        
        gx_prompt_text_set(prompt, "private string");
        gx_validation_set_frame_id(frame_id++); 
        gx_validation_set_frame_comment("Set text \"private string\"  for prompt.");
        gx_system_dirty_mark(pScrollWheelScreen);
        /* Force a screen refresh. */
        gx_validation_screen_refresh();
        
        gx_prompt_text_set(prompt, "This is private string prompt");
        gx_validation_set_frame_id(frame_id++); 
        gx_validation_set_frame_comment("Set text \"This is private string prompt\"  for prompt.");
        gx_system_dirty_mark(pScrollWheelScreen);
        /* Force a screen refresh. */
        gx_validation_screen_refresh();
        
        gx_text_button_text_set(text_button, 0);
        gx_validation_set_frame_id(frame_id++); 
        gx_validation_set_frame_comment("Set text to null for text_button.");
        gx_system_dirty_mark(pScrollWheelScreen);
        /* Force a screen refresh. */
        gx_validation_screen_refresh();
        
        gx_text_button_text_set(text_button, "text button");
        gx_validation_set_frame_id(frame_id++); 
        gx_validation_set_frame_comment("Set text to \"text button\" for text_button.");
        gx_system_dirty_mark(pScrollWheelScreen);
        /* Force a screen refresh. */
        gx_validation_screen_refresh();
        
        for(index = 0; index < (int)(sizeof(test_widget_list)/sizeof(TEST_CONTENT)); index++)
        {
            /* Delete widget to test private string delete */
            gx_widget_delete(test_widget_list[index].widget);
            gx_validation_set_frame_id(frame_id++);
            strcpy(test_comment, test_widget_list[index].comment);
            gx_validation_set_frame_comment(test_comment);
            gx_system_dirty_mark(pScrollWheelScreen);
            /* Force a screen refresh. */
            gx_validation_screen_refresh();
        }    
    }
    /* Add following test later. */
    /* Set allocate and delete function to null to test. */
    gx_system_memory_allocator_set(GX_NULL, GX_NULL);
    
    /* call text set function to test private string copy when no system allocate function */    
    
    /* call widget delete function to test private string copy when no system free function */    
    
    
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
