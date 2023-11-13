/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_lines_24xrgb", /* Test name */
    25, 17, 344, 336  /* Define the coordinates of the capture area.
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
static int angle_value[] = {0, 1, 23, 30, 45, 60, 75, 89, 90, 135};

/* Define the width values for the line.*/
static int width_value[] = {1, 2, 3, 5, 10};

/* Define the anti-alias settings. */
static int aa_value[] = {GX_TRUE, GX_FALSE};

static int brush_alpha_value[] = {0, 155, 255};

/* Define the line end settings. */
static int rounded_value[] = {GX_TRUE, GX_FALSE};

char test_comment[256];

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{

int angle_index, width_index, aa_index, rounded_index, brush_alpha_index;
int frame_id = 1;
GX_RECTANGLE size;

    for(aa_index = 0; aa_index < 2; aa_index++)
    {
        anti_aliased = aa_value[aa_index];
        for(rounded_index = 0; rounded_index < 2; rounded_index++)
        {
            rounded = rounded_value[rounded_index];
            for(width_index = 0; width_index < (INT)(sizeof(width_value) / sizeof(int)); width_index++)
            {
                line_width = width_value[width_index];
		line_color = (GX_RESOURCE_ID)color_list[width_index].color_id;
                for(angle_index = 0; angle_index < (INT)(sizeof(angle_value) / sizeof(int)); angle_index++)
                {
                    for(brush_alpha_index = 0; brush_alpha_index < (INT)(sizeof(brush_alpha_value)/sizeof(INT)); brush_alpha_index++)
                    {
                        brush_alpha = brush_alpha_value[brush_alpha_index];
                        /* Inform the validation system 
                        (1) Frame ID, which identifies a specific test configuration;
                       (2) Start recording frame on the next toggle operation.
                        */
                        gx_validation_set_frame_id(frame_id);

                        /* Set the line angle value */
                        line_angle = angle_value[angle_index];
                    
                        sprintf(test_comment, "width %d angle %d brush_alpha: %d", line_width, line_angle, brush_alpha);
                        if(anti_aliased) 
                            strcat(test_comment, "anti-aliased ");
                        if(rounded)
                            strcat(test_comment, "round end ");
 
                        gx_validation_set_frame_comment(test_comment);
 
                        /* Mark the window "dirty" */
                        gx_system_dirty_mark(pLineWin);

                        /* Force a screen refresh. */
                        gx_validation_screen_refresh();

                        /* Increment frame ID */
                        frame_id ++;
                    }
                }
            }
        }
    }
    brush_alpha = 0xff; 

    size.gx_rectangle_left = pLineWin->gx_widget_size.gx_rectangle_left;
    size.gx_rectangle_top = pLineWin->gx_widget_size.gx_rectangle_top;
    size.gx_rectangle_right = pLineWin->gx_widget_size.gx_rectangle_left + 150;
    size.gx_rectangle_bottom = pLineWin->gx_widget_size.gx_rectangle_top + 150;
    gx_widget_resize(pLineWin, &size);

    for(width_index = 0; width_index < (INT)(sizeof(width_value) / sizeof(int)); width_index++)
    {
        line_width = width_value[width_index];
        line_color = (GX_RESOURCE_ID)color_list[width_index].color_id;
        for(line_angle = 0; line_angle < 360; line_angle+=30)
        {
            for(brush_alpha_index = 0; brush_alpha_index < (INT)(sizeof(brush_alpha_value)/sizeof(INT)); brush_alpha_index++)
            {
                brush_alpha = brush_alpha_value[brush_alpha_index];
         
                /* Inform the validation system 
               (1) Frame ID, which identifies a specific test configuration;
               (2) Start recording frame on the next toggle operation.
                */
                gx_validation_set_frame_id(frame_id);

                /* Set the line angle value */
               
                sprintf(test_comment, "width %d angle %d brush_alpha: %d", line_width, line_angle, brush_alpha);

                gx_validation_set_frame_comment(test_comment);

                /* Mark the window "dirty" */
                gx_system_dirty_mark(pLineWin);

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





