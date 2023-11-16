/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_all_widgets_sprite_frame", /* Test name */
    41, 31, 597, 442  /* Define the coordinates of the capture area.
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

GX_SPRITE_FRAME frame_list[3] =
{
    {
        GX_PIXELMAP_ID_CHECKBOX_ON,          /* pixelmap id                    */
        0,                                   /* x offset                       */
        0,                                   /* y offset                       */
        5,                                   /* frame delay                    */
        GX_SPRITE_BACKGROUND_NO_ACTION,      /* background operation           */
        255                                  /* alpha value                    */
    },
    {
        GX_PIXELMAP_ID_CHECKBOX_OFF,          /* pixelmap id                    */
        0,                                   /* x offset                       */
        0,                                   /* y offset                       */
        5,                                   /* frame delay                    */
        GX_SPRITE_BACKGROUND_NO_ACTION,      /* background operation           */
        255                                  /* alpha value                    */
    },
    {
        GX_PIXELMAP_ID_RADIO_OFF,          /* pixelmap id                    */
        0,                                   /* x offset                       */
        0,                                   /* y offset                       */
        5,                                   /* frame delay                    */
        GX_SPRITE_BACKGROUND_NO_ACTION,      /* background operation           */
        255                                  /* alpha value                    */
    }
};

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int                frame_id = 1;
char *comment[1];
GX_SPRITE   *sprite = &sprite_screen.sprite_screen_sprite_1;
GX_EVENT    my_event;

    comment[0] = &test_comment[0];
    sprintf(test_comment, "toggle to sprite screen, and capture 5 frame.");
    
    gx_validation_capture_frames(frame_id, 5, comment, 0, 1000);
 
    ToggleScreen((GX_WINDOW *)&sprite_screen, (GX_WINDOW *)&button_screen);
    
    gx_validation_capture_frames_wait(); 
    
    /*Stop sprite */
    sprintf(test_comment, "Stop sprite.");
    gx_validation_capture_frames(frame_id, 1, comment, 0, 1000);

    gx_sprite_stop(sprite);
    gx_validation_capture_frames_wait(); 
        
    /* set frame index and capture one frame*/
    sprintf(test_comment, "Set frame index and capture 5 frame.");
    gx_validation_capture_frames(frame_id, 5, comment, 0, 1000);
    gx_sprite_start(sprite, 0);
    gx_sprite_current_frame_set(sprite, 1);
    gx_validation_capture_frames_wait(); 
    
    /* set frame list, and capture two frame*/
    sprintf(test_comment, "Set frame list and capture 5 frame.");
    gx_validation_capture_frames(frame_id, 2, comment, 0, 1000);

    gx_sprite_frame_list_set(sprite, frame_list, 3);
    gx_sprite_start(sprite, 0);
    gx_validation_capture_frames_wait(); 

    /* Send invalid timer event to the sprite. */
    memset(&my_event, 0, sizeof(my_event));
    my_event.gx_event_target = (GX_WIDGET *)sprite;
    my_event.gx_event_type = GX_EVENT_TIMER;
    gx_system_event_send(&my_event);
    
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
