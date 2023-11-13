/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_radial_progress_bar_resize", /* Test name */
    0, 0, 300, 350  /* Define the coordinates of the capture area.
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

#define REGRESSION_TEST

#include "gx_validation_wrapper.h"
#include "demo_guix_radial_progress_bar.c"

char test_comment[255];

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int          frame_id = 1;
int          index;
GX_RECTANGLE size;
GX_EVENT     my_event;
GX_RADIAL_PROGRESS_BAR_INFO info;

    radial_progress_bar_alpha = 128;
    gx_validation_screen_refresh();

    gx_widget_style_remove(progress_bar, GX_STYLE_TRANSPARENT);

    memset(&my_event, 0, sizeof(GX_EVENT));

    for(index = 0; index < 7; index++)
    {
        switch(index)
        {
        case 0:
            gx_utility_rectangle_define(&size, 63, 119, 280, 339);
            gx_widget_resize(progress_bar, &size);
            sprintf(test_comment, "resize width");
            break;

        case 1:
            gx_utility_rectangle_define(&size, 63, 119, 280, 330);
            gx_widget_resize(progress_bar, &size);
            sprintf(test_comment, "resize height");
            break;
        
        case 2:
            gx_widget_hide(progress_bar);
            my_event.gx_event_type = GX_EVENT_RESIZED;
            my_event.gx_event_target = (GX_WIDGET *)progress_bar;
            gx_system_event_send(&my_event);
            sprintf(test_comment, "hide widget and send resize event");
            break;

        case 3:
            gx_radial_progress_bar_info_set(progress_bar, &progress_bar->gx_radial_progress_bar_info);
            sprintf(test_comment, "set progress bar info");
            break;

        case 4:
            gx_widget_show(progress_bar);
            info = progress_bar->gx_radial_progress_bar_info;
            info.gx_radial_progress_bar_info_xcenter -= 2;
            info.gx_radial_progress_bar_info_radius -= 2;
            gx_radial_progress_bar_info_set(progress_bar, &info);
            sprintf(test_comment, "show widget and set progress bar info");
            break;

        case 5:
            info.gx_radial_progress_bar_info_ycenter += 2;
            gx_radial_progress_bar_info_set(progress_bar, &info);
            sprintf(test_comment, "show widget and set progress bar info");
            break;

        case 6:
            gx_system_memory_allocator_set(GX_NULL, GX_NULL);
            gx_utility_rectangle_define(&size, 63, 110, 280, 330);
            gx_widget_resize(progress_bar, &size);
            sprintf(test_comment, "set memort allocator null and resize");
            break;
        }
        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(test_comment);
        gx_validation_screen_refresh();
    }
     
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
