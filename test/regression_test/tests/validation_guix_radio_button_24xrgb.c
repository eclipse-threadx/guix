/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_radio_button_24xrgb", /* Test name */
    115, 351, 262, 384  /* Define the coordinates of the capture area.
                         In this test, we only need to capture the text button
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

    /* Call the actual text button routine. */
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

/* Define the x coordinate of mouse click. */
static int x_value[] = {124, 125, 126, 251, 252, 253};

/* Define the y coordinate of mouse click.*/
static int y_value[] = {355, 358, 359, 380, 381, 382};

char test_comment[256];

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int x_index, y_index;
int x_coordinate, y_coordinate;
int frame_id = 1;
GX_EVENT my_event;
GX_RADIO_BUTTON *radio_button;

    gx_widget_find(pButtonScreen, ID_RADIO_BUTTON, 0, &radio_button);

    gx_radio_button_pixelmap_set(radio_button, GX_PIXELMAP_ID_RADIO_OFF, 
                                 GX_PIXELMAP_ID_RADIO_ON, GX_PIXELMAP_ID_RADIO_OFF, GX_PIXELMAP_ID_RADIO_ON);

    for(x_index = 0; x_index < (INT)(sizeof(x_value) / sizeof(int)); x_index++)
    {
        x_coordinate = x_value[x_index];
        for(y_index = 0; y_index < (INT)(sizeof(y_value) / sizeof(int)); y_index++)
        {
            y_coordinate = y_value[y_index];

            memset(&my_event, 0, sizeof(GX_EVENT));
            my_event.gx_event_type = GX_EVENT_PEN_DOWN;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_x = x_coordinate;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_y = y_coordinate;
            my_event.gx_event_display_handle = 1;
            gx_system_event_send(&my_event);

            /* Inform the validation system 
            (1) Frame ID, which identifies a specific test configuration;
            (2) Start recording frame on the next toggle operation.
            */
            gx_validation_set_frame_id(frame_id);

            sprintf(test_comment, "x_coordinate %d y_coordinate %d ", x_coordinate, y_coordinate);
            if((x_coordinate >= 115) && (x_coordinate <= 252) && (y_coordinate >= 356) && (y_coordinate <= 379))
                strcat(test_comment, "inside ");
            else
                strcat(test_comment, "outside ");

            strcat(test_comment, "pen down ");
            
            gx_validation_set_frame_comment(test_comment);

            /* Mark the window "dirty" */
            gx_system_dirty_mark(pButtonScreen);

            /* Force a screen refresh. */
            gx_validation_screen_refresh();

            /* Increment frame ID */
            frame_id ++;
        }
    }

    gx_text_button_text_id_set((GX_TEXT_BUTTON *)radio_button, 0);
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Set text id to 0.");
    gx_validation_screen_refresh();

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}





