/* This is a small demo of the high-performance GUIX graphics framework. */


#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_system.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_all_widgets_system_view_fold", /* Test name */
    31, 69, 613, 400  /* Define the coordinates of the capture area.
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
       on the pixelmap drawing example. These operations simulate 
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


char test_comment[256];

GX_WIDGET *list[] =
{
    (GX_WIDGET *)&rotate_screen,
    (GX_WIDGET *)&sprite_screen,
    (GX_WIDGET *)&text_screen,
    (GX_WIDGET *)&window_screen,
    (GX_WIDGET *)&indicator_screen
};

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int frame_id = 1;
GX_RECTANGLE size;

int index;

    GX_ENTER_CRITICAL
    for(index = 0; index < (int)(sizeof(list)/sizeof(GX_WIDGET *));index++)
    {
        while (list[index]->gx_widget_first_child)
        {
            gx_widget_detach(list[index]->gx_widget_first_child); 
        }
        
        gx_widget_attach((GX_WIDGET *) root, list[index]);
    }

    size.gx_rectangle_left = 10;
    size.gx_rectangle_top = 5;
    size.gx_rectangle_right = 200;
    size.gx_rectangle_bottom = 220;
    gx_widget_resize((GX_WIDGET *)&button_screen, &size);
        
    size.gx_rectangle_left = 410;
    size.gx_rectangle_top = 5;
    size.gx_rectangle_right = 600;
    size.gx_rectangle_bottom = 220;
    gx_widget_resize((GX_WIDGET *)&rotate_screen, &size);
        
    size.gx_rectangle_left = 410;
    size.gx_rectangle_top = 220;
    size.gx_rectangle_right = 600;
    size.gx_rectangle_bottom = 450;
    gx_widget_resize((GX_WIDGET *)&sprite_screen, &size);
    
    size.gx_rectangle_left = 210;
    size.gx_rectangle_top = 5;
    size.gx_rectangle_right = 400;
    size.gx_rectangle_bottom = 220;
    gx_widget_resize((GX_WIDGET *)&text_screen, &size);

    
    size.gx_rectangle_left = 10;
    size.gx_rectangle_top = 220;
    size.gx_rectangle_right = 200;
    size.gx_rectangle_bottom = 450;
    gx_widget_resize((GX_WIDGET *)&window_screen, &size);

    size.gx_rectangle_left = 210;
    size.gx_rectangle_top = 220;
    size.gx_rectangle_right = 400;
    size.gx_rectangle_bottom = 450;
    gx_widget_resize((GX_WIDGET *)&indicator_screen, &size);
    GX_EXIT_CRITICAL
    
    gx_validation_set_frame_id(frame_id++);
    sprintf(test_comment, "Resize screens.");
    gx_validation_set_frame_comment(test_comment);
    /* Force a screen refresh. */
    gx_validation_screen_refresh();
    
    
    size.gx_rectangle_left = 10;
    size.gx_rectangle_top = 5;
    size.gx_rectangle_right = 200;
    size.gx_rectangle_bottom = 200;
    gx_widget_resize((GX_WIDGET *)&button_screen, &size);
        
    size.gx_rectangle_left = 410;
    size.gx_rectangle_top = 5;
    size.gx_rectangle_right = 600;
    size.gx_rectangle_bottom = 200;
    gx_widget_resize((GX_WIDGET *)&rotate_screen, &size);
        
    size.gx_rectangle_left = 410;
    size.gx_rectangle_top = 220;
    size.gx_rectangle_right = 600;
    size.gx_rectangle_bottom = 450;
    gx_widget_resize((GX_WIDGET *)&sprite_screen, &size);
    
    size.gx_rectangle_left = 210;
    size.gx_rectangle_top = 5;
    size.gx_rectangle_right = 400;
    size.gx_rectangle_bottom = 200;
    gx_widget_resize((GX_WIDGET *)&text_screen, &size);

    
    size.gx_rectangle_left = 10;
    size.gx_rectangle_top = 220;
    size.gx_rectangle_right = 200;
    size.gx_rectangle_bottom = 450;
    gx_widget_resize((GX_WIDGET *)&window_screen, &size);

    size.gx_rectangle_left = 210;
    size.gx_rectangle_top = 220;
    size.gx_rectangle_right = 400;
    size.gx_rectangle_bottom = 450;
    gx_widget_resize((GX_WIDGET *)&indicator_screen, &size);
    GX_EXIT_CRITICAL
    
    gx_validation_set_frame_id(frame_id++);
    sprintf(test_comment, "Resize screen.");
    gx_validation_set_frame_comment(test_comment);
    /* Force a screen refresh. */
    gx_validation_screen_refresh(); 
    
    
    
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}





