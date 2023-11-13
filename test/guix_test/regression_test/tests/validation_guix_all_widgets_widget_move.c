/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_all_widgets_widget_move", /* Test name */
    38, 25, 582, 444  /* Define the coordinates of the capture area.
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

char test_comment[255];
typedef struct WIDGET_LIST_STRUCT
{
    GX_WIDGET *widget;
    GX_RECTANGLE size;
} WIDGET_LIST;

static WIDGET_LIST widget_list[] =
{
    {(GX_WIDGET *)&text_screen.text_screen_prompt_1, {58, 83, 366, 107}},
    {(GX_WIDGET *)&text_screen.text_screen_prompt_3, {95, 92, 403, 117}},
    {(GX_WIDGET *)&text_screen.text_screen_prompt_2, {138, 100, 446, 144}},
    {(GX_WIDGET *)&text_screen.text_screen_prompt_4, {171, 125, 479, 161}},
    {(GX_WIDGET *)&text_screen.text_screen_text_input_1, {202, 146, 510, 182}},
    {(GX_WIDGET *)&text_screen.text_screen_text_view_1, {262, 171, 513, 324}},
    {(GX_WIDGET *)&text_screen.text_screen_text_input_2, {318, 233, 570, 386}}
};

static VOID control_thread_entry(ULONG input)
{
int       frame_id = 1;
UINT      index;
GX_BOOL   moved = GX_FALSE;

    ToggleScreen(pTextScreen, pButtonScreen);
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Original text screen.");
    gx_validation_screen_refresh();   

    gx_single_line_text_input_style_remove(&text_screen.text_screen_text_input_1, GX_STYLE_CURSOR_BLINK);
    gx_multi_line_text_input_style_remove(&text_screen.text_screen_text_input_2, GX_STYLE_CURSOR_BLINK);
    
    for(index = 0; index < sizeof(widget_list)/sizeof(WIDGET_LIST); index++)
    {
        gx_system_focus_claim(pTextScreen);
        
        gx_widget_resize(widget_list[index].widget, &widget_list[index].size);        
    }
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Resize widgets.");
    gx_validation_screen_refresh();   
    
    for(index = 0; index < sizeof(widget_list)/sizeof(WIDGET_LIST); index++)
    {
        gx_system_focus_claim(pTextScreen);
        
        gx_widget_front_move(widget_list[index].widget, &moved);
        gx_validation_set_frame_id(frame_id++);
        sprintf(test_comment, "Move widget_%d front.", index);
        gx_validation_set_frame_comment(test_comment);
        gx_validation_screen_refresh();           
    }
    
    for(index = sizeof(widget_list)/sizeof(WIDGET_LIST); index > 0; )
    {
        index--;
        gx_system_focus_claim(pTextScreen);
        
        gx_widget_back_move(widget_list[index].widget, &moved);
        gx_validation_set_frame_id(frame_id++);
        sprintf(test_comment, "Move widget_%d back.", index);
        gx_validation_set_frame_comment(test_comment);
        gx_validation_screen_refresh();           
    }
    
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
