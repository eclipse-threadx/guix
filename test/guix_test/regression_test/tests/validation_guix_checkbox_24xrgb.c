/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_checkbox_24xrgb", /* Test name */
    145, 315, 258, 355  /* Define the coordinates of the capture area.
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

#include "demo_guix_all_widgets.c"
/* Define the xpos values for the mouse. */
static int xpos_value[] = {145, 148, 150, 159, 167, 182, 191, 200, 214, 233, 252, 257};

/* Define the ypos values for the mouse.*/
static int ypos_value[] = {315, 320, 328, 332, 339, 349, 354};

char test_comment[256];


static VOID control_thread_entry(ULONG input)
{

int xpos_index, ypos_index;
int frame_id = 1;
GX_EVENT my_event;
GX_CHECKBOX *checkbox;

	gx_widget_find(pButtonScreen, ID_CHECKBOX, 0, &checkbox);
	gx_checkbox_pixelmap_set(checkbox, GX_PIXELMAP_ID_CHECKBOX_OFF, GX_PIXELMAP_ID_CHECKBOX_ON, GX_PIXELMAP_ID_CHECKBOX_OFF, GX_PIXELMAP_ID_CHECKBOX_ON);
	
    for(xpos_index = 0; xpos_index < (INT)(sizeof(xpos_value) / sizeof(int)); xpos_index++)
    {
        for(ypos_index = 0; ypos_index < (INT)(sizeof(ypos_value) / sizeof(int)); ypos_index++)
        {
            gx_validation_set_frame_comment(test_comment);
            memset(&my_event, 0, sizeof(GX_EVENT));

            my_event.gx_event_type = GX_EVENT_PEN_DOWN;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_x = xpos_value[xpos_index];
            my_event.gx_event_payload.gx_event_pointdata.gx_point_y = ypos_value[ypos_index];
            my_event.gx_event_display_handle = 1;
            gx_system_event_send(&my_event);

            gx_validation_set_frame_id(frame_id);
            sprintf(test_comment, "xpos %d ypos %d ", xpos_value[xpos_index], ypos_value[ypos_index]);
            if(xpos_value[xpos_index] < 150 || xpos_value[xpos_index] > 253 || ypos_value[ypos_index] < 320 || ypos_value[ypos_index] > 350)
            {
                strcat(test_comment, "outside ");
            }
            else
            {
                strcat(test_comment, "inside ");
            }
            
            if(!(checkbox -> gx_widget_style & GX_STYLE_BUTTON_PUSHED))
            {
                strcat(test_comment, "click_off ");
            }
            else
            {
                strcat(test_comment, "click_on ");
            }

            /* Mark the window "dirty" */
            gx_system_dirty_mark(pButtonScreen);

            /* Force a screen refresh. */
            gx_validation_screen_refresh();

            /* Increment frame ID */
            frame_id ++;

        }
    }


    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}





