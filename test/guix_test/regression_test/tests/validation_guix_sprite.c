/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_system.h"

TEST_PARAM test_parameter = {
    "guix_sprite", /* Test name */
    98, 76,215, 177  /* Define the coordinates of the capture area.
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

#include "gx_validation_wrapper.h"
#include "demo_guix_all_widgets.c"

char test_comment[255];

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int        frame_id = 1;
char      *comment[1];
GX_SPRITE *sprite = &sprite_screen.sprite_screen_sprite_1;

    comment[0] = &test_comment[0];
 
    /* Removed auto start style. */
    gx_widget_style_remove(sprite, GX_STYLE_SPRITE_AUTO);

    /* Added loop style . */
    gx_widget_style_add(sprite, GX_STYLE_SPRITE_LOOP);

    /* Toggle to sprite screen. */
    ToggleScreen((GX_WINDOW *)&sprite_screen, (GX_WINDOW *)&button_screen);
    
    /* set frame list, and capture two frame*/
    sprintf(test_comment, "Start sprite and start frame capture. ");
    gx_validation_capture_frames(frame_id, sprite -> gx_sprite_frame_count + 1, comment, 0, 1000);

    gx_sprite_start(sprite, 0);
    gx_validation_capture_frames_wait(); 

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
