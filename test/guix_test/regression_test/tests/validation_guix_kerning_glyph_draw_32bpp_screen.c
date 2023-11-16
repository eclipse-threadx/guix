/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_kerning_glyph_draw_32bpp_screen", /* Test name */
    20, 45, 600, 460  /* Define the coordinates of the capture area.
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

#include "demo_kerning_glyph_draw_32bpp_screen.c"

/*NOTICE: Number of xpos_value and ypos_valus should be same.*/
/* Define the xpos values for the mouse. */
static int xpos_value[] = {350, 450, 585, 585, 100, 100, 100, 100, 100, 100};
/* Define the ypos values for the mouse.*/
static int ypos_value[] = {415, 415, 250, 360, 70, 120, 170, 220, 270, 320};

static int Mouse_event[] = {GX_EVENT_PEN_DOWN, GX_EVENT_PEN_UP};

char test_comment[256];

static VOID control_thread_entry(ULONG input)
{

int   frame_id = 1;
int xpos_index, Mouse_index;
GX_EVENT my_event;

    memset(&my_event, 0, sizeof(GX_EVENT));

    for(xpos_index = 0; xpos_index < (INT)(sizeof(xpos_value) / sizeof(int)); xpos_index++)
    {
       for(Mouse_index = 0; Mouse_index < 2; Mouse_index++)
       {
           /* Inform the validation system 
           (1) Frame ID, which identifies a specific test configuration;
           (2) Start recording frame on the next toggle operation.
           */
           gx_validation_set_frame_id(frame_id);

           sprintf(test_comment, "xpos %d ypos %d ", xpos_value[xpos_index], ypos_value[xpos_index]);
           if(Mouse_event[Mouse_index] == GX_EVENT_PEN_DOWN)
           {
               strcat(test_comment, "mouse down ");
           }
           else
           {
               strcat(test_comment, "mouse up ");
           }

           memset(&my_event, 0, sizeof(GX_EVENT));

           my_event.gx_event_type = Mouse_event[Mouse_index];
           my_event.gx_event_payload.gx_event_pointdata.gx_point_x = xpos_value[xpos_index];
           my_event.gx_event_payload.gx_event_pointdata.gx_point_y = ypos_value[xpos_index];
           my_event.gx_event_display_handle = 1;
           gx_system_event_send(&my_event);
           
           gx_validation_set_frame_id(frame_id);
           gx_validation_set_frame_comment(test_comment);
           gx_validation_screen_refresh();
           frame_id++;
       }
    }


   /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
