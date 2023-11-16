/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_glyph_blend_16bpp_screen", /* Test name */
    55, 45, 560, 340  /* Define the coordinates of the capture area.
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
#ifdef win32_graphics_driver_setup_565rgb  
#undef win32_graphics_driver_setup_565rgb  
#endif
#define win32_graphics_driver_setup_565rgb  gx_validation_graphics_driver_setup_565rgb


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_glyph_draw_16bpp.c"

int clip_width[] = {5, 10, 15, 20, 25, 30};

char test_comment[256];

static VOID control_thread_entry(ULONG input)
{
int             slider_value;
UINT            width_index;
int             frame_id = 1;
int             increment = 10;
GX_SLIDER_INFO *slider_info;
GX_CHAR         get_value[3];

    gx_widget_style_add((GX_WIDGET *)&window, GX_STYLE_USE_LOCAL_ALPHA | GX_STYLE_TRANSPARENT);
    window.gx_widget_alpha = 128;

    gx_widget_hide((GX_WIDGET *)root);
    gx_widget_show((GX_WIDGET *)root);

    for(width_index = 0; width_index < sizeof(clip_width)/sizeof(int); width_index ++)
    {
        /* Set vertical clip width. */
        gx_utility_ltoa(clip_width[width_index], get_value, 3);
        strcpy(pVClipInput->gx_single_line_text_input_buffer, get_value);
        pVClipInput -> gx_single_line_text_input_string_size = strlen(get_value);
        gx_widget_event_generate(pVClipInput, GX_EVENT_TEXT_EDITED, 0);

        /* Set horizontal clip height. */
        strcpy(pHClipInput->gx_single_line_text_input_buffer, get_value);
        pHClipInput -> gx_single_line_text_input_string_size = strlen(get_value);
        gx_widget_event_generate(pHClipInput, GX_EVENT_TEXT_EDITED, 0);

        /* Drag horizontal clip down. */
        slider_info = &pVerticalSlider -> gx_slider_info;
        for(slider_value = slider_info -> gx_slider_info_max_val; slider_value >= slider_info -> gx_slider_info_min_val; slider_value -= increment)
        {
            gx_slider_value_set(pVerticalSlider, slider_info, slider_value);
    
            sprintf(test_comment, "Clip width: %d, Drag horizontal clip down", clip_width[width_index]);
            gx_validation_set_frame_id(frame_id++);
            gx_validation_set_frame_comment(test_comment);
            gx_validation_screen_refresh();
        }
    
        /* Drag horizontal clip up. */
        for(slider_value = slider_info -> gx_slider_info_min_val; slider_value <= slider_info -> gx_slider_info_max_val; slider_value += increment)
        {
            gx_slider_value_set(pVerticalSlider, slider_info, slider_value);

            sprintf(test_comment, "Clip width: %d, Drag horizontal clip up", clip_width[width_index]);
            gx_validation_set_frame_id(frame_id++);
            gx_validation_set_frame_comment(test_comment);
            gx_validation_screen_refresh();
    
        }
    
        /* Drag vertical clip right. */
        slider_info = &pHorizontalSlider -> gx_slider_info;
        for(slider_value = slider_info -> gx_slider_info_min_val; slider_value <= slider_info -> gx_slider_info_max_val; slider_value += increment)
        {
            gx_slider_value_set(pHorizontalSlider, slider_info, slider_value);

            sprintf(test_comment, "Clip width: %d, Drag vertical clip right", clip_width[width_index]);    
            gx_validation_set_frame_id(frame_id++);
            gx_validation_set_frame_comment(test_comment);
            gx_validation_screen_refresh();
        }
    
        /* Drag vertical clip left. */
        for(slider_value = slider_info -> gx_slider_info_max_val; slider_value >= slider_info -> gx_slider_info_min_val; slider_value -= increment)
        {
            gx_slider_value_set(pHorizontalSlider, slider_info, slider_value);

            sprintf(test_comment, "Clip width: %d, Drag vertical clip left", clip_width[width_index]);
            gx_validation_set_frame_id(frame_id++);
            gx_validation_set_frame_comment(test_comment);
            gx_validation_screen_refresh();
        }
    }

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
