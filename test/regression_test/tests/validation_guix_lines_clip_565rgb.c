/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

GX_WINDOW pCoverWin;
# define ID_WINDOW_COVER 18
INT cover_delta =0;
TEST_PARAM test_parameter = {
    "guix_lines_clip_565rgb", /* Test name */
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
#define win32_graphics_driver_setup_24xrgb  gx_validation_graphics_driver_setup_565rgb


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_guix_lines.c"
/* Define the angles values for the line. */
static int angle_value[] = {0, 1, 23, 30, 45, 60, 75, 89, 90, 135};

/* Define the width values for the line.*/
static int width_value[] = {1, 2, 3, 5, 10};

/* Define the cover coordinate values for the line.*/
static int cover_value[] = {50, 100, 5, 145, -300};

/* Define the anti-alias settings. */
static int aa_value[] = {GX_TRUE, GX_FALSE};

/* Define the line end settings. */
static int rounded_value[] = {GX_TRUE, GX_FALSE};

char test_comment[256];

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{

int angle_index, width_index, aa_index, rounded_index, cover_index;
int frame_id = 1;
GX_EVENT my_event;
GX_RECTANGLE    size;

    gx_utility_rectangle_define(&size, 25, 14, 34, 333);
    gx_window_create(&pCoverWin, "cover_window", (GX_WIDGET *)pMainWin, 0,ID_WINDOW_COVER,  &size);
    gx_widget_fill_color_set(&pCoverWin, pCoverWin.gx_widget_normal_fill_color, pCoverWin.gx_widget_normal_fill_color,
                             pCoverWin.gx_widget_normal_fill_color);
    gx_widget_status_add(&pCoverWin, GX_STATUS_MOVABLE);
    for(aa_index = 0; aa_index < 2; aa_index++)
    {
        anti_aliased = aa_value[aa_index];
        for(rounded_index = 0; rounded_index < 2; rounded_index++)
        {
            rounded =rounded_value[rounded_index];
            for(width_index = 0; width_index < (INT)(sizeof(width_value) / sizeof(int)); width_index++)
            {
                line_width = width_value[width_index];
                for(angle_index = 0; angle_index < (INT)(sizeof(angle_value) / sizeof(int)); angle_index++)
                {
                    /* Set the line angle value */
                    line_angle =angle_value[angle_index]; 
                    
                    /* Mark the window "dirty" */
                    gx_system_dirty_mark(pLineWin);

                    /* Force a screen refresh. */
                    gx_validation_screen_refresh();
                                       
                    for(cover_index = 0; cover_index < (INT)(sizeof(cover_value) / sizeof(int)); cover_index++)
                    {
                        /* Inform the validation system 
                           (1) Frame ID, which identifies a specific test configuration;
                           (2) Start recording frame on the next toggle operation.
                        */
                        /* Set the cover window coordinate shift. */
                        cover_delta = cover_value[cover_index];                     
                        memset(&my_event, 0, sizeof(GX_EVENT));
                        my_event.gx_event_display_handle = 1;
                        /* Create the pen down event. */
                        my_event.gx_event_target = (GX_WIDGET *)&pCoverWin;
                        my_event.gx_event_type = GX_EVENT_PEN_DOWN;
                        my_event.gx_event_payload.gx_event_pointdata.gx_point_x = pCoverWin.gx_widget_size.gx_rectangle_left + 5;
                        my_event.gx_event_payload.gx_event_pointdata.gx_point_y = pCoverWin.gx_widget_size.gx_rectangle_top + 10;
                        gx_system_event_send(&my_event);
                        /* Create the pen drag event. */
                        my_event.gx_event_payload.gx_event_pointdata.gx_point_x = pCoverWin.gx_widget_size.gx_rectangle_left+ 5 + cover_delta;
                        my_event.gx_event_type = GX_EVENT_PEN_DRAG;
                        gx_system_event_send(&my_event);
                        my_event.gx_event_type = GX_EVENT_PEN_UP;
                        gx_system_event_send(&my_event);

                        gx_validation_set_frame_id(frame_id);
                        
                        sprintf(test_comment, "width %d angle %d cover %d ", line_width, line_angle, cover_delta);
                        if(anti_aliased) 
                            strcat(test_comment, "anti-aliased ");
                        if(rounded)
                            strcat(test_comment, "round end ");
                                                
                        gx_validation_set_frame_comment(test_comment);                        
                        /* Increment frame ID */
                        frame_id ++;
                    }
                }    
            }
        }
    }
    
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}





