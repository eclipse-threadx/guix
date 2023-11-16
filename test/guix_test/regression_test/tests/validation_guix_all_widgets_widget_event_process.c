/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_all_widgets_widget_event_process", /* Test name */
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


static VOID control_thread_entry(ULONG input)
{
int       frame_id = 1;
GX_EVENT  my_event;
GX_SINGLE_LINE_TEXT_INPUT *sl_text_input = GX_NULL;
GX_MULTI_LINE_TEXT_INPUT  *ml_text_input = GX_NULL;
GX_MULTI_LINE_TEXT_VIEW  *ml_text_view = GX_NULL;

    ToggleScreen(pTextScreen, pButtonScreen);
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Original text screen.");
    gx_validation_screen_refresh();   
    
    gx_system_widget_find(ID_SL_INPUT, 2, (GX_WIDGET **)&sl_text_input);
    gx_system_widget_find(ID_ML_INPUT, 2, (GX_WIDGET **)&ml_text_input);
    gx_system_widget_find(ID_ML_VIEW, 2, (GX_WIDGET **)&ml_text_view);

    if (sl_text_input &&  ml_text_input && ml_text_view)
    {
        gx_widget_fill_color_set(sl_text_input, GX_COLOR_ID_TEXT_INPUT_FILL, GX_COLOR_ID_BLUE, GX_COLOR_ID_SHINE);
        gx_widget_fill_color_set(ml_text_input, GX_COLOR_ID_TEXT_INPUT_FILL, GX_COLOR_ID_BLUE, GX_COLOR_ID_SHINE);
        gx_widget_fill_color_set(ml_text_view, GX_COLOR_ID_TEXT_INPUT_FILL, GX_COLOR_ID_BLUE, GX_COLOR_ID_SHINE);
        
        gx_system_focus_claim(pTextScreen);
        gx_system_focus_claim(sl_text_input);
        gx_single_line_text_input_style_remove(sl_text_input, GX_STYLE_CURSOR_BLINK);
        gx_single_line_text_input_style_add(sl_text_input, GX_STYLE_CURSOR_ALWAYS);
        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment("Claim single line text input gain focus.");
        gx_validation_screen_refresh();   
        
        memset(&my_event, 0, sizeof(GX_EVENT));
        my_event.gx_event_type = GX_EVENT_KEY_DOWN;
        my_event.gx_event_display_handle = 1;
        my_event.gx_event_payload.gx_event_ushortdata[0] = GX_KEY_SELECT;
        gx_system_event_send(&my_event);
        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment("Send event KEY_DOWN and GX_KEY_SELECT on current widget.");
        gx_validation_screen_refresh();   
        
        my_event.gx_event_type = GX_EVENT_KEY_UP;
        my_event.gx_event_display_handle = 1;
        my_event.gx_event_payload.gx_event_ushortdata[0] = GX_KEY_SELECT;
        gx_system_event_send(&my_event);
        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment("Send event GX_KEY_SELECT and KEY_UP on current widget.");
        gx_validation_screen_refresh();  
    
        my_event.gx_event_type = GX_EVENT_KEY_DOWN;
        my_event.gx_event_display_handle = 1;
        my_event.gx_event_payload.gx_event_ushortdata[0] = GX_KEY_NEXT;
        gx_system_event_send(&my_event);
        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment("Send event GX_KEY_NEXT on current widget.");
        gx_validation_screen_refresh();   
        
        my_event.gx_event_type = GX_EVENT_KEY_DOWN;
        my_event.gx_event_display_handle = 1;
        my_event.gx_event_payload.gx_event_ushortdata[0] = GX_KEY_PREVIOUS;
        gx_system_event_send(&my_event);
        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment("Send event GX_KEY_PREVIOUS on current widget.");
        gx_validation_screen_refresh();       


        my_event.gx_event_type = GX_EVENT_KEY_DOWN;
        my_event.gx_event_display_handle = 1;
        my_event.gx_event_payload.gx_event_ushortdata[0] = GX_KEY_CONTROL;
        gx_system_event_send(&my_event);
        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment("Send event GX_KEY_CONTROL on current widget.");
        gx_validation_screen_refresh();    

        gx_system_focus_claim(sl_text_input);
        my_event.gx_event_type = GX_EVENT_INPUT_RELEASE;
        my_event.gx_event_display_handle = 1;
        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment("Send GX_EVENT_INPUT_RELEASE to sl_text_input.");
        gx_validation_screen_refresh();  
        
        memset(&my_event, 0, sizeof(GX_EVENT));
        my_event.gx_event_payload.gx_event_pointdata.gx_point_x = 60;
        my_event.gx_event_payload.gx_event_pointdata.gx_point_y = 200;
        my_event.gx_event_type = GX_EVENT_PEN_DOWN;
        my_event.gx_event_display_handle = 1;
        gx_system_event_send(&my_event);
        gx_system_focus_claim(sl_text_input);
        gx_widget_hide(sl_text_input);
        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment("Hide sl_text_input.");
        gx_validation_screen_refresh(); 
        
        memset(&my_event, 0, sizeof(GX_EVENT));
        my_event.gx_event_type = GX_EVENT_INPUT_RELEASE;
        my_event.gx_event_display_handle = 1;
        my_event.gx_event_target = (GX_WIDGET *)sl_text_input;
        gx_system_event_send(&my_event);
        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment("Send GX_EVENT_INPUT_RELEASE event to sl_text_input.");
        gx_validation_screen_refresh(); 

        gx_widget_status_add(sl_text_input, GX_STATUS_OWNS_INPUT);
        memset(&my_event, 0, sizeof(GX_EVENT));
        my_event.gx_event_type = GX_EVENT_INPUT_RELEASE;
        my_event.gx_event_display_handle = 1;
        my_event.gx_event_target = (GX_WIDGET *)sl_text_input;
        gx_system_event_send(&my_event);
        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment("Send GX_EVENT_INPUT_RELEASE event to sl_text_input with GX_STATUS_OWNS_INPUT.");
        gx_validation_screen_refresh(); 

    }    
    
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
