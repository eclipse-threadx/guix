/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_vertical_list_set_32bpp", /* Test name */
   70, 85, 440, 330  /* Define the coordinates of the capture area.
                         In this test, we only need to capture the pixelmap slider
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
#ifdef win32_graphics_driver_setup_24xrgb  
#undef win32_graphics_driver_setup_24xrgb  
#endif
#define win32_graphics_driver_setup_24xrgb  gx_validation_graphics_driver_setup_24xrgb


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_guix_vertical_list_32bpp.c"

char     test_comment[256];
int      lines_array[]={18,26,1,10,0,5,7,0,15,25,19};
GX_POINT remove_button_point = {500, 395};


/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int      frame_id = 1;
int      index;

    for(index = 0 ; index < (int)(sizeof(lines_array)/sizeof(int));index++)
    {
        list_index = lines_array[index];

        gx_vertical_list_selected_set(&(pMainScreen)->window_vertical_list_scroll, list_index);
        gx_vertical_list_selected_set(&(pMainScreen)->window_vertical_list_wrap, list_index);

        sprintf(test_comment, "Set selected index to %d", list_index);
        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(test_comment);
        gx_validation_screen_refresh();
    }
 
    gx_vertical_list_selected_set(&window.window_vertical_list_scroll, 1);
    gx_vertical_list_selected_set(&window.window_vertical_list_wrap, 1);
    gx_vertical_list_page_index_set(&window.window_vertical_list_scroll, 5);
    gx_vertical_list_page_index_set(&window.window_vertical_list_wrap, 5);

    sprintf(test_comment, "Set selected index to 1, set page index to 5");
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();

    for(index = 0; index < 21; index++)
    {
#if defined(GX_DISABLE_ERROR_CHECKING)
        if(index < 20)
        {
            remove_last_index();
        }
#else
        remove_last_index();
#endif
        sprintf(test_comment, "remove last line");
        gx_validation_set_frame_id(frame_id);
        gx_validation_set_frame_comment(test_comment);
    
        /* Force a screen refresh. */
        gx_validation_screen_refresh();
    
        /* Increment frame ID */
        frame_id ++;
    }

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
