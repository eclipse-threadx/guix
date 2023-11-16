/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_icon_button_24xrgb", /* Test name */
    229, 386, 252, 414  /* Define the coordinates of the capture area.
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

static GX_VALUE x_point[] = {228, 229, 235, 252, 253};
static GX_VALUE y_point[] = {385, 386, 399, 409, 410};
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
GX_ICON_BUTTON *pButton;

    gx_widget_find(pButtonScreen, ID_ICON_BUTTON, 0, (GX_WIDGET **) &pButton);

    /* add transparent style */
    gx_widget_style_add(pButton, GX_STYLE_TRANSPARENT);
    gx_validation_set_frame_id(frame_id);
    sprintf(test_comment, "add transparent style");
    gx_validation_set_frame_comment(test_comment);
    /* Force a screen refresh. */
    gx_validation_screen_refresh();


    frame_id++;

   
    /* send pen down event.  */
    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_type = GX_EVENT_PEN_DOWN;
    my_event.gx_event_payload.gx_event_pointdata.gx_point_x = 235;
    my_event.gx_event_payload.gx_event_pointdata.gx_point_y = 399;
    my_event.gx_event_display_handle = 1;
    gx_system_event_send(&my_event);
    gx_validation_set_frame_id(frame_id);
    sprintf(test_comment, "send pen down event");
    gx_validation_set_frame_comment(test_comment);
     /* Force a screen refresh. */
    gx_validation_screen_refresh();

    frame_id ++;

    my_event.gx_event_type = GX_EVENT_PEN_UP;
    my_event.gx_event_display_handle = 1;
    gx_system_event_send(&my_event);
    gx_validation_set_frame_id(frame_id);
    sprintf(test_comment, "send pen up event");
    gx_validation_set_frame_comment(test_comment);
    /* Force a screen refresh. */
    gx_validation_screen_refresh();


    frame_id ++;

    /* remove transparent style.  */
    gx_widget_style_remove(pButton, GX_STYLE_TRANSPARENT);
    gx_validation_set_frame_id(frame_id);
    sprintf(test_comment, "remove transparent style");
    gx_validation_set_frame_comment(test_comment);
    /* Force a screen refresh. */
    gx_validation_screen_refresh();


    frame_id++;

    my_event.gx_event_type = GX_EVENT_PEN_DOWN;
    my_event.gx_event_display_handle = 1;
    gx_system_event_send(&my_event);
    gx_validation_set_frame_id(frame_id);
    sprintf(test_comment, "send pen down event");
    gx_validation_set_frame_comment(test_comment);
    /* Force a screen refresh. */
    gx_validation_screen_refresh();


    frame_id ++;

    my_event.gx_event_type = GX_EVENT_PEN_UP;
    my_event.gx_event_display_handle = 1;
    gx_system_event_send(&my_event);
    gx_validation_set_frame_id(frame_id);
    sprintf(test_comment, "send pen up event");
    gx_validation_set_frame_comment(test_comment);
    /* Force a screen refresh. */
    gx_validation_screen_refresh();


    frame_id ++;
    /* remove background pixelmap.  */
    gx_icon_button_pixelmap_set(pButton, GX_NULL);
    gx_validation_set_frame_id(frame_id);
    sprintf(test_comment, "remove background map");
    gx_validation_set_frame_comment(test_comment);
    /* Force a screen refresh. */
    gx_validation_screen_refresh();


    frame_id++;

    /* set new background colors. */
    gx_widget_fill_color_set(pButton, GX_COLOR_ID_NEXT_BUTTON_UPPER, GX_COLOR_ID_NEXT_BUTTON_LOWER, GX_COLOR_ID_NEXT_BUTTON_UPPER);
    gx_validation_set_frame_id(frame_id);
    sprintf(test_comment, "set new background colors");
    gx_validation_set_frame_comment(test_comment);
    /* Force a screen refresh. */
    gx_validation_screen_refresh();


    frame_id++;

    /* send pen down event.  */
    my_event.gx_event_type = GX_EVENT_PEN_DOWN;
    my_event.gx_event_display_handle = 1;
    gx_system_event_send(&my_event);
    gx_validation_set_frame_id(frame_id);
    sprintf(test_comment, "send pen down event");
    gx_validation_set_frame_comment(test_comment);
    /* Force a screen refresh. */
    gx_validation_screen_refresh();


    frame_id ++;

    my_event.gx_event_type = GX_EVENT_PEN_UP;
    my_event.gx_event_display_handle = 1;
    gx_system_event_send(&my_event);
    gx_validation_set_frame_id(frame_id);
    sprintf(test_comment, "send pen up event");
    gx_validation_set_frame_comment(test_comment);
    /* Force a screen refresh. */
    gx_validation_screen_refresh();


    frame_id ++;

    /* set background pixelmap for the icon button. */
    gx_icon_button_pixelmap_set(pButton, GX_PIXELMAP_ID_SAVE_ICON);
    gx_validation_set_frame_id(frame_id);
    sprintf(test_comment, "set background pixelmap");
    gx_validation_set_frame_comment(test_comment);
    /* Force a screen refresh. */
    gx_validation_screen_refresh();

    frame_id ++;

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


                if ((x_point[x_index] >= 229) && (x_point[x_index] <= 252) && 
                    (y_point[y_index] >= 386) && (y_point[y_index] <= 409))
                {

                    if (my_event.gx_event_type == GX_EVENT_PEN_DOWN)
                    {
                        sprintf(test_comment, "send pen down event");
                    }
                    else if (my_event.gx_event_type == GX_EVENT_PEN_UP)
                    {
                        sprintf(test_comment, "send pen up event");
                    }
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
