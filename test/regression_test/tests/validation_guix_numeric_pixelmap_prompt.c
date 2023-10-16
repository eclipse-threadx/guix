 /* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_numeric_pixelmap_prompt", /* Test name */
    402, 130, 541, 210   /* Define the coordinates of the capture area.
                         In this test, we only need to capture the promt area. */
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
GX_POINT button_value = {200 , 250};

static VOID control_thread_entry(ULONG input)
{
UINT     frame_id = 1;
int      value;


    /* Toggle to text screen. */
    ToggleScreen(pTextScreen, pButtonScreen);

    /* Stop number animation timer. */
    gx_system_timer_stop(pTextScreen, NUMBER_ANIMATION_TIMER);

    for( value = -10000 ; value< 10000 ; value += 1234)
    {

        /* Inform the validation system 
        (1) Frame ID, which identifies a specific test configuration;
        (2) Start recording frame on the next toggle operation.*/
        gx_validation_set_frame_id(frame_id);

        sprintf(test_comment, "Set prompt value: %d", value);
        gx_validation_set_frame_comment(test_comment);

        /* Set numeric prompt value. */
        gx_numeric_pixelmap_prompt_value_set(&text_screen.text_screen_numeric_pixelmap_prompt, value);
             
        /* Force a screen refresh. */
        gx_validation_screen_refresh();
           
        /* Increment frame ID */
        frame_id ++;
       
    }    
    
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
