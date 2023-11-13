/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_system.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_focus_management", /* Test name */
    0, 0, 639, 479  /* Define the coordinates of the capture area.
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

#include "demo_guix_focus_management.c"
char test_comments[256];

static VOID control_thread_entry(ULONG input)
{
int     frame_id = 1;
int     index;
GX_BOOL failed = GX_FALSE;

    for(index = 0; index < 6; index++)
    {
        switch(index)
        {
        case 0:
           sprintf(test_comments, "child window and prompt 3 has default focus");
           break;

        case 1:
           gx_widget_hide(&main_screen.main_screen_prompt_3);
           sprintf(test_comments, "hide prompt 3, focus should move to prompt 4");
           break;

        case 2:
           gx_widget_hide(&main_screen.main_screen_window);
           sprintf(test_comments, "hide child window, focus should move to prompt 1");
           break;

        case 3:
           gx_widget_show(&main_screen.main_screen_window);
           gx_system_focus_claim(&main_screen.main_screen_prompt_2);
           sprintf(test_comments, "show child window, set focus to prompt 2");
           break;

        case 4:
           gx_widget_hide(&main_screen.main_screen_prompt_2);
           sprintf(test_comments, "hide prompt 2, focus should move to prompt 4");
           break;

        case 5:
           gx_widget_hide(&main_screen.main_screen_prompt_4);
           sprintf(test_comments, "hide prompt 4, focus should move to parent window");
           if(_gx_system_focus_owner != (GX_WIDGET *)&main_screen.main_screen_window)
           {
               PRINT_ERROR("focus should move to parent window.");
               failed = GX_TRUE;
           }
           break;
        }

        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(test_comments);
        gx_validation_screen_refresh();
    }
    
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    if(failed)
    {
        exit(-1);
    }
    else
    {
        exit(0);
    }
}
