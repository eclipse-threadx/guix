/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_widget_back_move", /* Test name */
    73, 7, 575, 475  /* Define the coordinates of the capture area.
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

static VOID control_thread_entry(ULONG input)
{
int       frame_id = 1;
int       index = 0;


GX_RECTANGLE size[] = {{117, 100, 253, 134},
                             {183, 118, 320, 150},
                             {230, 131, 366, 200}};
                             
GX_WIDGET *list[] = {(GX_WIDGET *)&button_screen.button_screen_base_button_1,
                            (GX_WIDGET *)&button_screen.button_screen_text_button_1,
                            (GX_WIDGET *)&button_screen.button_screen_multi_line_button_1};


    for(index = 0; index < (int)(sizeof(size)/sizeof(GX_RECTANGLE)); index++)
    {
        gx_widget_resize(list[index], &size[index]);
    }

    gx_validation_set_frame_id(frame_id++); 
    gx_validation_set_frame_comment("Resize buttons.");
    /* Force a screen refresh. */
    gx_validation_screen_refresh();    /* Signal the end of the test case.  Verify the output. */
    
    gx_widget_back_move(list[1], 0);
    gx_validation_set_frame_id(frame_id++); 
    gx_validation_set_frame_comment("Move text button back.");
    /* Force a screen refresh. */
    gx_validation_screen_refresh();    /* Signal the end of the test case.  Verify the output. */
    
    gx_widget_back_move(list[1], 0);
    gx_validation_set_frame_id(frame_id++); 
    gx_validation_set_frame_comment("Move text button back gain.");
    /* Force a screen refresh. */
    gx_validation_screen_refresh();    /* Signal the end of the test case.  Verify the output. */
    
    gx_validation_end();

    exit(0);
}
