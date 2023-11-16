/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_system.h"

TEST_PARAM test_parameter = {
    "guix_all_widgets_progress_bar_background_draw", /* Test name */
    270, 220, 440, 270  /* Define the coordinates of the capture area.
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
#include "demo_guix_all_widgets.c"

char test_comment[255];



/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int                        frame_id = 1;
GX_PROGRESS_BAR           *progress_bar;
static GX_PROGRESS_BAR_INFO progress_bar_info;
GX_RECTANGLE               size;

    progress_bar = &indicator_screen.indicator_screen_progress_bar_1;
    progress_bar_info = progress_bar->gx_progress_bar_info;
    gx_widget_detach(pButtonScreen);
    gx_widget_attach(root, pIndicatorScreen);

    /* gx_progress_bar_info_max_val == min_val, return directly. */
    progress_bar_info.gx_progress_bar_info_max_val = 0;
    progress_bar_info.gx_progress_bar_info_min_val = 0;
    
    gx_progress_bar_info_set(progress_bar, &progress_bar_info);
    sprintf(test_comment, "Set progress bar info, progress_bar_info_max_value: %d, progress_bar_min_val: %d.",
            progress_bar_info.gx_progress_bar_info_max_val, progress_bar_info.gx_progress_bar_info_min_val);
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();

    /* fill_map == NULL. */
    gx_progress_bar_pixelmap_set(progress_bar, -1);
    sprintf(test_comment, "Set invalid pixelmap id -1 to progress_bar.");
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();

    /* Fill color to several segments. */
    progress_bar_info.gx_progress_bar_info_max_val = 100;
    progress_bar_info.gx_progress_bar_info_current_val = 100;
    progress_bar_info.gx_progress_bar_info_min_val = 0;
    
    gx_progress_bar_info_set(progress_bar, &progress_bar_info);
    gx_progress_bar_pixelmap_set(progress_bar, 0);
    size =  progress_bar -> gx_widget_size;
    size.gx_rectangle_bottom = 100;
    size.gx_rectangle_top = 0;
    gx_widget_resize(progress_bar, &size);
    gx_widget_style_add(progress_bar, GX_STYLE_PROGRESS_SEGMENTED_FILL | GX_STYLE_PROGRESS_VERTICAL);
    
    sprintf(test_comment, "Set progress bar info : max_val: %d, current_val: %d, min_val: %d, ",
            progress_bar_info.gx_progress_bar_info_max_val, progress_bar_info.gx_progress_bar_info_current_val,  progress_bar_info.gx_progress_bar_info_min_val);
           
    strcat(test_comment, "set progress bar pixelmap id: 0,");
    strcat(test_comment, "Resize progress_bar widget size,");   
    strcat(test_comment, "Add style GX_STYLE_PROGRESS_SEGMENTED_FILL and GX_STYLE_PROGRESS_VERTICAL to progress bar.");   
            
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();

    gx_progress_bar_pixelmap_set(progress_bar, 1000);
    gx_widget_status_add(progress_bar, GX_STATUS_VISIBLE);

    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("invalid fill_pixelmap");
    gx_validation_screen_refresh();

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
