/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
   "guix_menu_32bpp", /* Test name */
    0 , 0, 240, 320  /* Define the coordinates of the capture area.
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

#include "demo_guix_menu.c"

char test_comment[256];

static GX_POINT click_pos[]={
    {15, 90},/* To bookself screen.  */
    {95, 45},/* To book screen.  */
    {120, 160},/* Popup toolbar.  */
    {8, 8},/* Back to booself screen.  */
    {11, 7},/* Back to main screen.  */
    {15, 90},
    {90, 45},
    {120, 160},
    {150, 10},/* Back to home screen directly. */
};

static char comments[9][30]={
"To bookself screen",
"To book screen",
"Popup toolbar.",
"Back to bookself screen.",
"Back to main screen.",
"To bookself screen.",
"To book screen",
"Popup toolbar.",
"Back to home screen directly."
};

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
GX_EVENT my_event;
int      frame_id = 1;
int      loop = 0;
int      index = 0;

    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_display_handle = 1;

    for(loop = 0; loop < 4; loop++)
    {
        for(index = 0; index < (int)(sizeof(click_pos)/sizeof(GX_POINT)); index++)
        {   
            my_event.gx_event_type = GX_EVENT_PEN_DOWN;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_x = click_pos[index].gx_point_x;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_y = click_pos[index].gx_point_y;
            gx_system_event_send(&my_event);

            my_event.gx_event_type = GX_EVENT_PEN_UP;
            gx_system_event_send(&my_event);

            sprintf(test_comment, "%s", comments[index]);

            gx_validation_set_frame_id(frame_id++);
            gx_validation_set_frame_comment(test_comment);

            /* Force a screen refresh. */
            gx_validation_screen_refresh();
        }

    }   
     /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);

}
