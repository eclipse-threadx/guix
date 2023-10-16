/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_all_widgets_widget_text_blend", /* Test name */
    73, 7, 576, 100  /* Define the coordinates of the capture area.
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

static UINT color_ids[] = {GX_COLOR_ID_BLUE, GX_COLOR_ID_WHITE, GX_COLOR_ID_BLACK};
static UINT font_ids[] = {GX_FONT_ID_SYSTEM, GX_FONT_ID_NORMAL_FONT, GX_FONT_ID_SELECTED_FONT};
static GX_UBYTE alpha_values[] = {0, 100, 180, 255};
static ULONG aligned[] = {GX_STYLE_TEXT_RIGHT, GX_STYLE_TEXT_LEFT, GX_STYLE_TEXT_CENTER};
static int       offset;
static UINT      font_index, alpha_index, aligned_index;

static GX_CONST GX_CHAR test_string[] = "widget text blend test.";
static void widget_text_draw_function(GX_PROMPT *prompt)
{
ULONG old_style;
GX_STRING string;

    old_style = prompt->gx_widget_style;
    old_style &= ~GX_STYLE_TEXT_CENTER;
    prompt->gx_widget_style |= aligned[aligned_index];
    gx_prompt_draw(prompt);

    if(aligned_index == 0)
    {
        gx_widget_text_blend((GX_WIDGET *)prompt, color_ids[font_index], font_ids[font_index], test_string, offset, offset, alpha_values[alpha_index]); 
    }
    else
    {
        string.gx_string_ptr = test_string;
        string.gx_string_length = sizeof(test_string) - 1;
        gx_widget_text_blend_ext((GX_WIDGET *)prompt, color_ids[font_index], font_ids[font_index], &string, offset, offset, alpha_values[alpha_index]);
    }

    prompt->gx_widget_style = old_style;
}

static VOID control_thread_entry(ULONG input)
{
int       frame_id = 1;
    
    gx_widget_draw_set((GX_WIDGET *)&button_screen.button_screen_title_1, widget_text_draw_function);
    
    for(aligned_index = 0; aligned_index < sizeof(aligned)/sizeof(ULONG); aligned_index++)
    {
        for(font_index = 0; font_index < sizeof(font_ids)/sizeof(UINT); font_index++)
        {
            for(alpha_index = 0; alpha_index < sizeof(alpha_values)/sizeof(GX_UBYTE); alpha_index++)
            {
                for(offset = -20; offset <= 30; offset += 10)
                {
                    sprintf(test_comment, "xoffset: %d, yoffset: %d, alpha: %d, ", offset, offset, alpha_values[alpha_index]);
                    
                    switch(font_index)
                    {
                        case 0:
                            strcat(test_comment, "font: GX_FONT_ID_SYSTEM, color:  GX_COLOR_ID_BLUE, ");
                            break;
                            
                        case 1:
                            strcat(test_comment, "font: GX_FONT_ID_NORMAL_FONT, color:  GX_COLOR_ID_WHITE, ");
                            break;                        
                            
                        case 2:
                            strcat(test_comment, "font: GX_FONT_ID_SELECTED_FONT, color:  GX_COLOR_ID_BLACK, ");
                            break;                        
                    }
                    
                    switch(aligned_index)
                    {
                        case 0:
                            strcat(test_comment, "aligned: right");
                            break;
                            
                        case 1:
                            strcat(test_comment, "aligned: left");
                            break;                        
                            
                        case 2:
                            strcat(test_comment, "aligned: center");
                            break;                        
                    }
                    
                    
                    gx_validation_set_frame_id(frame_id++);
                    gx_validation_set_frame_comment(test_comment);
                    gx_validation_screen_refresh();
                }
            }   
        }
    }

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
