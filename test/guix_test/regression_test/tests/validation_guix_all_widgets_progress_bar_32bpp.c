 /* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_all_widgets_progress_bar_32bpp", /* Test name */
    270, 220, 450, 280   /* Define the coordinates of the capture area.
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

typedef struct PROPERTY_STRUCT{
GX_RESOURCE_ID text_color_id;
GX_RESOURCE_ID pixelmap_id;
GX_RESOURCE_ID font_id;
UINT            min_value;
UINT            max_value;
}PROPERTY;

PROPERTY property[] = {
{GX_COLOR_ID_BTN_BORDER, GX_NULL, GX_FONT_ID_SYSTEM, 0, 100},
{GX_COLOR_ID_WINDOW_BORDER, GX_PIXELMAP_ID_TFIELD_FILL_SMALL, GX_FONT_ID_TEXT_INPUT, 24, 88}
};

ULONG border_style[] = {GX_STYLE_BORDER_NONE, GX_STYLE_BORDER_THIN, GX_STYLE_BORDER_THICK, GX_STYLE_BORDER_RAISED, GX_STYLE_BORDER_RECESSED};

char test_comment[255];

static VOID control_thread_entry(ULONG input)
{
UINT frame_id = 1;
UINT index, property_index = 0, style_index = 0, border_index = 0, val;
GX_PROGRESS_BAR *progress_bar;

    /* Switch to indicator screen. */
    ToggleScreen(pIndicatorScreen, pButtonScreen);
 
    /* Get progress bar widget. */
    progress_bar = &((INDICATOR_SCREEN_CONTROL_BLOCK *)pIndicatorScreen) -> indicator_screen_progress_bar_1;

    for(index = 0; index < 2; index ++)
    {
         if(index > 0)
         {
             gx_widget_style_add(progress_bar, GX_STYLE_PROGRESS_VERTICAL);
         }
          
         for(property_index = 0; property_index < sizeof(property)/sizeof(PROPERTY); property_index ++)
         {
             /* Set progress bar range. */
             gx_progress_bar_range_set(progress_bar, property[property_index].min_value, property[property_index].max_value);
             
             /* Set text color. */
             gx_progress_bar_text_color_set(progress_bar, property[property_index].text_color_id,
                                            property[property_index].text_color_id, property[property_index].text_color_id);
     
             /* Set font. */
             gx_progress_bar_font_set(progress_bar, property[property_index].font_id);
     
             /* Set pixelmap fill. */
             gx_progress_bar_pixelmap_set(progress_bar, property[property_index].pixelmap_id);  
                          
             for(val = property[property_index].min_value; val < property[property_index].max_value; val += 10)
             {
                 /* Inform the validation system 
                    (1) Frame ID, which identifies a specific test configuration;
                    (2) Start recording frame on the next toggle operation.
                 */
                 /* Set progress bar value. */ 
                 gx_progress_bar_value_set(progress_bar, val);

                 sprintf(test_comment, "Value: %d, ", val);
            
                 if(index > 0)
                 {
                     strcat(test_comment, "Vertical, ");
                 }
                 else
                 {
                     strcat(test_comment, "Horizontal, ");
                 }

                 switch(style_index)
                 {
                 case 0:
                     if(progress_bar->gx_widget_style & GX_STYLE_PROGRESS_SEGMENTED_FILL)
                     {
                          gx_widget_style_remove(progress_bar, GX_STYLE_PROGRESS_SEGMENTED_FILL);
                          strcat(test_comment, "solid fill");
                     }
                     else
                     {
                          gx_widget_style_add(progress_bar, GX_STYLE_PROGRESS_SEGMENTED_FILL);
                          strcat(test_comment, "segment fill");
                     }
                     style_index ++;
                     break;
                 case 1:
                     /* Set text style*/
                     if(progress_bar->gx_widget_style & GX_STYLE_PROGRESS_TEXT_DRAW)
                     {
                          gx_widget_style_remove(progress_bar, GX_STYLE_PROGRESS_TEXT_DRAW);
                          strcat(test_comment, "no text");
                     }
                     else
                     {
                          gx_widget_style_add(progress_bar, GX_STYLE_PROGRESS_TEXT_DRAW);
                          strcat(test_comment, "draw text, ");
                           
                          if(progress_bar -> gx_widget_style & GX_STYLE_PROGRESS_PERCENT)
                          {
                               gx_widget_style_remove(progress_bar, GX_STYLE_PROGRESS_PERCENT);
                               strcat(test_comment, "no percent sign");
                          }
                          else
                          {
                               gx_widget_style_add(progress_bar, GX_STYLE_PROGRESS_PERCENT); 
                               strcat(test_comment, "with percent sign");
                          }
                     }
                     style_index ++;
                     break;
                 case 2:
                     /* Set border style. */
                     if(border_index >= sizeof(border_style)/sizeof(ULONG))
                     {
                          border_index = 0;
                     }
                     gx_widget_border_style_set(progress_bar, border_style[border_index]);
                     sprintf(test_comment, "%s border_style_id %ld", test_comment, border_style[border_index++]);
                     style_index = 0;
                     break;
                 } 
        
                 gx_validation_set_frame_id(frame_id);
 
                 gx_validation_set_frame_comment(test_comment);

                 /* Mark the window "dirty" */
                 gx_system_dirty_mark(progress_bar);
             
                 /* Force a screen refresh. */
                 gx_validation_screen_refresh();
             
                 /* Increment frame ID */
                 frame_id ++;
             }
         }
    }
    
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}





