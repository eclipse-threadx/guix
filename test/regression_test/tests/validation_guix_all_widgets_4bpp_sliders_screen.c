/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_all_widgets_4bpp_sliders_screen", /* Test name */
    146, 53, 466, 387  /* Define the coordinates of the capture area.
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
#ifdef win32_graphics_driver_setup_4bpp_grayscale
#undef win32_graphics_driver_setup_4bpp_grayscale
#endif
#define win32_graphics_driver_setup_4bpp_grayscale  gx_validation_graphics_driver_setup_4bpp_grayscale


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_guix_all_widgets_4bpp.c"
/* Define the xpos values for the mouse. */
static int xpos = 150;
static int xend = 450;
/* Define the ypos values for the mouse.*/
static int ypos = 150;
static int yend = 330;

char test_comment[256];
extern ULONG palette_4bpp_grayscale[16];

static VOID control_thread_entry(ULONG input)
{

int increament = 30;
int x, y;
int frame_id = 1;
GX_EVENT my_event;
   
    gx_validation_write_palette((ULONG *)palette_4bpp_grayscale, 16);
    memset(&my_event, 0, sizeof(GX_EVENT));
  
    for(y = ypos; y <= yend; y += increament)
    {
        for(x = xpos; x <= xend ; x += increament)
        {
            /* Inform the validation system 
	    (1) Frame ID, which identifies a specific test configuration;
	    (2) Start recording frame on the next toggle operation.
	    */
	    
	    ToggleScreen(pIndicatorScreen, pSpriteScreen);
	    my_event.gx_event_payload.gx_event_pointdata.gx_point_x = x;
	    my_event.gx_event_payload.gx_event_pointdata.gx_point_y = y;
	    my_event.gx_event_type = GX_EVENT_PEN_DOWN;

	    my_event.gx_event_display_handle = 1;
	    gx_system_event_send(&my_event);
	    my_event.gx_event_type = GX_EVENT_PEN_UP;
	    gx_system_event_send(&my_event);


	    /* Inform the validation system 
	    (1) Frame ID, which identifies a specific test configuration;
	    (2) Start recording frame on the next toggle operation.
	    */
	    gx_validation_set_frame_id(frame_id);
	    sprintf(test_comment, "pen pos x=%d, y= %d ",x, y);
	    gx_validation_set_frame_comment(test_comment);
            gx_validation_screen_refresh();

            /* Increment frame ID */
	    frame_id ++;

	    ToggleScreen(pSpriteScreen, pIndicatorScreen);
        }
    }


    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}





