/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_display_driver_simple_line_alpha_draw", /* Test name */
    25, 14, 344, 333  /* Define the coordinates of the capture area.
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

#include "demo_guix_lines.c"
/* Define the angles values for the line. */
static int angle_value[] = {0, 1, 23, 75, 89, 90, 135, 244, 345};
static GX_RECTANGLE size_list[] = 
{
    {25, 14, 344, 333},
    {25, 14, 185, 174},
    {25, 14, 160, 150}
};

char test_comment[256];

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{

int angle_index, size_index;
int frame_id = 1;

    brush_alpha = 155;
    anti_aliased = GX_FALSE;
    rounded = GX_FALSE;
    line_width = 1;
    line_color = GX_COLOR_ID_RED;
    
    for(size_index = 0; size_index < (int)(sizeof(size_list)/ sizeof(GX_RECTANGLE)); size_index++)
    {
        gx_widget_resize(pLineWin, &size_list[size_index]);
        for(angle_index = 0; angle_index < (INT)(sizeof(angle_value) / sizeof(int)); angle_index++)
        {
            /* Inform the validation system 
            (1) Frame ID, which identifies a specific test configuration;
            (2) Start recording frame on the next toggle operation.
            */
            gx_validation_set_frame_id(frame_id);

            /* Set the line angle value */
            line_angle = angle_value[angle_index];
                                
            sprintf(test_comment, "width 1, angle %d, brush_alpha: 155", line_angle);
             
            gx_validation_set_frame_comment(test_comment);
             
            /* Mark the window "dirty" */
            gx_system_dirty_mark(pLineWin);

            /* Force a screen refresh. */
            gx_validation_screen_refresh();

            /* Increment frame ID */
            frame_id ++;
        }
    }
    
    gx_widget_resize(pLineWin, &size_list[0]);
    gx_validation_set_frame_id(frame_id);

    /* Set the line angle value */
    line_angle = 30;
    root->gx_window_root_canvas->gx_canvas_display->gx_display_driver_pixel_blend = GX_NULL;
                                    
    sprintf(test_comment, "Set pixel blend function to GX_NULL. width 1, angle %d, brush_alpha: 155", line_angle);
                 
    gx_validation_set_frame_comment(test_comment);
                 
    /* Mark the window "dirty" */
    gx_system_dirty_mark(pLineWin);

    /* Force a screen refresh. */
    gx_validation_screen_refresh();
            
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}





