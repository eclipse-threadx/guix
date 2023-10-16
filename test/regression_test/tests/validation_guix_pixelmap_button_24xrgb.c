/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_pixelmap_button_24xrgb", /* Test name */
    130, 250, 255, 320  /* Define the coordinates of the capture area.
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

static GX_VALUE x_point[] = {140, 141, 165, 262, 263};
static GX_VALUE y_point[] = {250, 251, 261, 313, 314};
static ULONG event_type[] = {GX_EVENT_PEN_DOWN, GX_EVENT_PEN_UP};


char test_comment[256];

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int x_index, y_index, type_index;
GX_EVENT my_event;
int frame_id = 1;
GX_PIXELMAP_BUTTON *pButton;

    gx_widget_find(pButtonScreen, ID_PIXELMAP_BUTTON, 0, (GX_WIDGET **) &pButton);


    /* Remove the pixelmap button's pixelmap */
    gx_pixelmap_button_pixelmap_set(pButton, GX_NULL, GX_NULL, GX_NULL);

    /* set the pixelmap button to transparent */
    gx_widget_style_add(pButton, GX_STYLE_TRANSPARENT);

    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_type = GX_EVENT_PEN_DOWN;
    my_event.gx_event_payload.gx_event_pointdata.gx_point_x = 165;
    my_event.gx_event_payload.gx_event_pointdata.gx_point_y = 261;
    my_event.gx_event_display_handle = 1;
    gx_system_event_send(&my_event);
    gx_validation_set_frame_id(frame_id);
    sprintf(test_comment, "set the style to transparent for pixelmap button, pen down ");
    gx_validation_set_frame_comment(test_comment);
    /* Force a screen refresh. */
    gx_validation_screen_refresh();

    frame_id ++;
    my_event.gx_event_type = GX_EVENT_PEN_UP;
    my_event.gx_event_display_handle = 1;
    gx_system_event_send(&my_event);
    gx_validation_set_frame_id(frame_id);
    sprintf(test_comment, "set the style to transparent for pixelmap button, pen up ");
    gx_validation_set_frame_comment(test_comment);
    /* Force a screen refresh. */
    gx_validation_screen_refresh();


    frame_id ++;
    /* remove the transparent style from pixelmap button */
    gx_widget_style_remove(pButton, GX_STYLE_TRANSPARENT);

    my_event.gx_event_type = GX_EVENT_PEN_DOWN;
    my_event.gx_event_display_handle = 1;
    gx_system_event_send(&my_event);
    gx_validation_set_frame_id(frame_id);
    sprintf(test_comment, "remove the style to transparent for pixelmap button, pen down ");
    gx_validation_set_frame_comment(test_comment);
    /* Force a screen refresh. */
    gx_validation_screen_refresh();


    frame_id ++;
    my_event.gx_event_type = GX_EVENT_PEN_UP;
    my_event.gx_event_display_handle = 1;
    gx_system_event_send(&my_event);
    gx_validation_set_frame_id(frame_id);
    sprintf(test_comment, "remove the style to transparent for pixelmap button, pen up ");
    gx_validation_set_frame_comment(test_comment);
    /* Force a screen refresh. */
    gx_validation_screen_refresh();


    frame_id ++;
    /* Set the pixelmap button's background color. */
    gx_widget_fill_color_set(pButton, GX_COLOR_ID_NEXT_BUTTON_UPPER, GX_COLOR_ID_NEXT_BUTTON_LOWER, GX_COLOR_ID_NEXT_BUTTON_UPPER);

    my_event.gx_event_type = GX_EVENT_PEN_DOWN;
    my_event.gx_event_display_handle = 1;
    gx_system_event_send(&my_event);
    gx_validation_set_frame_id(frame_id);
    sprintf(test_comment, "set the background color for pixelmap button, pen down ");
    gx_validation_set_frame_comment(test_comment);
    /* Force a screen refresh. */
    gx_validation_screen_refresh();


    frame_id ++;
    my_event.gx_event_type = GX_EVENT_PEN_UP;
    my_event.gx_event_display_handle = 1;
    gx_system_event_send(&my_event);
    gx_validation_set_frame_id(frame_id);
    sprintf(test_comment, "set the background color for pixelmap button, pen up ");
    gx_validation_set_frame_comment(test_comment);
    /* Force a screen refresh. */
    gx_validation_screen_refresh();

    frame_id ++;

    gx_pixelmap_button_pixelmap_set(pButton, GX_PIXELMAP_ID_FISH, GX_PIXELMAP_ID_FISH,
                                    GX_PIXELMAP_ID_I_MEDICATIONSGREEN_LG);

    gx_widget_style_remove((GX_WIDGET *)pButton, GX_STYLE_ENABLED);
    sprintf(test_comment, "set button pixelmaps, and disable button");
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();

    /* Set the pixelmap for the pixelmap button. */
    gx_widget_style_remove((GX_WIDGET *)pButton, GX_STYLE_TRANSPARENT);
    gx_pixelmap_button_pixelmap_set(pButton, GX_PIXELMAP_ID_FISH, 
                                    GX_PIXELMAP_ID_I_PATIENTLIST_LG, GX_PIXELMAP_ID_I_MEDICATIONSGREEN_LG);
    gx_widget_style_add((GX_WIDGET *)pButton, GX_STYLE_ENABLED);
    sprintf(test_comment, "set button pixelmaps and enable button");
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();

    
    for (x_index = 0; x_index < (GX_VALUE)(sizeof(x_point) / sizeof(GX_VALUE)); x_index ++)
    {
        my_event.gx_event_payload.gx_event_pointdata.gx_point_x = x_point[x_index];

        for (y_index = 0; y_index < (GX_VALUE)(sizeof(y_point) / sizeof(GX_VALUE)); y_index ++)
        {
            my_event.gx_event_payload.gx_event_pointdata.gx_point_y = y_point[y_index];

            for (type_index = 0; type_index < (int)(sizeof(event_type) / sizeof(ULONG)); type_index ++)
            {
                my_event.gx_event_type = event_type[type_index];
                my_event.gx_event_display_handle = 1;


                sprintf(test_comment, "x point %d y point %d ", x_point[x_index], y_point[y_index]);

                if ((x_point[x_index] >= 141) && (x_point[x_index] <= 262) && 
                    (y_point[y_index] >= 251) && (y_point[y_index] <= 313))
                {
                    strcat(test_comment, "click inside the button ");

                    if (my_event.gx_event_type == GX_EVENT_PEN_DOWN)
                        strcat(test_comment, "button down medications pixelmap ");
                    else
                        strcat(test_comment, "button up fish pixelmap ");
                }
                else
                {
                    strcat(test_comment, "click outside the button ");

                    if (my_event.gx_event_type == GX_EVENT_PEN_DOWN)
                        strcat(test_comment, "button down fish pixelmap ");
                    else
                        strcat(test_comment, "button up fish pixelmap ");
                }

                gx_system_event_send(&my_event);
                gx_validation_set_frame_id(frame_id);
                gx_validation_set_frame_comment(test_comment);
                /* Force a screen refresh. */
                gx_validation_screen_refresh();
                /* Increment frame ID */
                frame_id ++;
            }
        }
    }
    
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}





