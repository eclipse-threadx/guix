/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_synergy_glyph_draw_16bpp", /* Test name */
     0, 0, 640, 480      /* Define the coordinates of the capture area.
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

    /* Create a dedicated thread to perform various operations
       on the pixelmap drawing example. These operations simulate 
       user input. */
    gx_validation_control_thread_create(control_thread_entry);

    /* Termiante the test if it runs for more than 100 ticks */
    /* This function is not implemented yet. */
    gx_validation_watchdog_create(100);

    /* Call the actual line example routine. */
    gx_validation_application_define(first_unused_memory);

}


/* Replace the default graphics driver with the validation driver. */
#ifdef win32_dave2d_graphics_driver_setup_565rgb 
#undef win32_dave2d_graphics_driver_setup_565rgb  
#endif
#define win32_dave2d_graphics_driver_setup_565rgb  gx_validation_dave2d_graphics_driver_setup_565rgb


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_synergy_glyph_draw_16bpp.c"

int xpos_value[]={0, 360, 420};
int ypos_value[]={0, 270, 330};

char test_comment[256];

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{

int xpos_i, ypos_i;
int frame_id = 1;
GX_EVENT my_event;

    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_display_handle = 1;

    my_event.gx_event_payload.gx_event_pointdata.gx_point_y = 447;
    for(xpos_i = 0; xpos_i < (int)(sizeof(xpos_value)/sizeof(int)); xpos_i++)
    {
        my_event.gx_event_payload.gx_event_pointdata.gx_point_x = xpos_value[xpos_i]; 
        my_event.gx_event_type = GX_EVENT_PEN_DOWN;
        gx_system_event_send(&my_event);
        my_event.gx_event_type = GX_EVENT_PEN_UP;
        gx_system_event_send(&my_event);

        /* Inform the validation system 
        (1) Frame ID, which identifies a specific test configuration;
        (2) Start recording frame on the next toggle operation.
        */
        gx_validation_set_frame_id(frame_id);

        /* Set the vertical slider value */

        gx_validation_set_frame_comment(test_comment);

        /* Mark the window "dirty" */
        gx_system_dirty_mark((GX_WIDGET **) &pMainWin);

        /* Force a screen refresh. */
        gx_validation_screen_refresh();

        /* Increment frame ID */
        frame_id ++;
                
    }
       
    my_event.gx_event_payload.gx_event_pointdata.gx_point_x = 592; 
    for(ypos_i = 0; ypos_i < (int)(sizeof(ypos_value)/sizeof(int)); ypos_i++)
    {
        my_event.gx_event_payload.gx_event_pointdata.gx_point_y = ypos_value[ypos_i];
        my_event.gx_event_type = GX_EVENT_PEN_DOWN;
        gx_system_event_send(&my_event);
        my_event.gx_event_type = GX_EVENT_PEN_UP;
        gx_system_event_send(&my_event);

        /* Inform the validation system
        (1) Frame ID, which identifies a specific test configuration;
        (2) Start recording frame on the next toggle operation.
        */
        gx_validation_set_frame_id(frame_id);

        /* Set the vertical slider value */

        gx_validation_set_frame_comment(test_comment);

        /* Mark the window "dirty" */
        gx_system_dirty_mark((GX_WIDGET **) &pMainWin);

        /* Force a screen refresh. */
        gx_validation_screen_refresh();

        /* Increment frame ID */
        frame_id ++;

    }
   
    
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}


