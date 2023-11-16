/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_horizontal_list_select", /* Test name */
    359, 356, 626, 467  /* Define the coordinates of the capture area.
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

#include "demo_guix_horizontal_list_32bpp.c"

char test_comment[256];

#define ITEM_0_X 410
#define ITEM_1_X 485
#define ITEM_2_X 571

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
INT                 frame_id = 1;
GX_HORIZONTAL_LIST *list = &window.window_horizontal_list_1;
GX_WIDGET          *widget = (GX_WIDGET *)list;
GX_RECTANGLE        size;
GX_EVENT            my_event;
INT                 index;


    memset(&my_event, 0, sizeof(my_event));
    my_event.gx_event_display_handle = 1;

    size = widget->gx_widget_size;

    for(index = 0; index < 11; index++)
    {
        gx_horizontal_list_selected_set(list, -1);
        gx_numeric_prompt_value_set(&window.window_horizontal_list_1_selected_prompt, 0);
        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment("set selected -1");
        gx_validation_screen_refresh();

        switch(index)
        {
        case 0:
            my_event.gx_event_type = GX_EVENT_PEN_DOWN;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_x = size.gx_rectangle_right - 5;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_y = size.gx_rectangle_top + 5;
            gx_system_event_send(&my_event);

            my_event.gx_event_type = GX_EVENT_PEN_UP;
            gx_system_event_send(&my_event);
            sprintf(test_comment, "click on horizontal list without hit a list item");
            break;

        case 1:
            my_event.gx_event_type = GX_EVENT_PEN_DOWN;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_x = ITEM_1_X;
            gx_system_event_send(&my_event);
            
            my_event.gx_event_type = GX_EVENT_PEN_UP;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_x = size.gx_rectangle_right - 5;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_y = size.gx_rectangle_top + 5;
            gx_system_event_send(&my_event);
            sprintf(test_comment, "pen down on item 2(button), pen up and not hover any list item");
            break;

        case 2:
            my_event.gx_event_type = GX_EVENT_PEN_DOWN;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_x = ITEM_1_X;
            gx_system_event_send(&my_event);

            my_event.gx_event_type = GX_EVENT_PEN_UP;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_x = ITEM_2_X;
            gx_system_event_send(&my_event);
            sprintf(test_comment, "pen down on item 2(button), pen up on item 2(prompt)");
            break;

        case 3:
            my_event.gx_event_type = GX_EVENT_PEN_DOWN;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_x = ITEM_2_X;
            gx_system_event_send(&my_event);

            my_event.gx_event_type = GX_EVENT_PEN_UP;
            gx_system_event_send(&my_event);
            sprintf(test_comment, "click on item 3(prompt)");
            break;
        case 4:
            my_event.gx_event_type = GX_EVENT_PEN_DOWN;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_x = ITEM_1_X;
            gx_system_event_send(&my_event);

            my_event.gx_event_type = GX_EVENT_PEN_UP;
            gx_system_event_send(&my_event);
            sprintf(test_comment, "click on item 2(button)");
            break;

        case 5:
            list->gx_horizontal_list_snap_back_distance = 10;
            my_event.gx_event_type = GX_EVENT_PEN_DOWN;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_x = ITEM_0_X;
            gx_system_event_send(&my_event);

            my_event.gx_event_type = GX_EVENT_PEN_UP;
            gx_system_event_send(&my_event);
            sprintf(test_comment, "click on item 1(checkbox)");
            break;
 
        case 6:
            my_event.gx_event_type = GX_EVENT_PEN_DOWN;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_x = size.gx_rectangle_right - 5;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_y = size.gx_rectangle_top + 5;
            gx_system_event_send(&my_event);

            my_event.gx_event_type = GX_EVENT_PEN_UP;
            gx_system_event_send(&my_event);
            sprintf(test_comment, "click on horizontal list without hit a list item");
            break;

        case 7:
            /* Resize to make children width > widget width. */
            size = widget->gx_widget_size;
            size.gx_rectangle_right = size.gx_rectangle_left +228;
            gx_widget_resize(widget, &size);
            sprintf(test_comment, "resize to make children width > widget width");
            break;

        case 8:
            list->gx_horizontal_list_snap_back_distance = 10;
            my_event.gx_event_type = GX_EVENT_PEN_DOWN;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_x = ITEM_0_X;
            gx_system_event_send(&my_event);

            my_event.gx_event_type = GX_EVENT_PEN_UP;
            gx_system_event_send(&my_event);
            sprintf(test_comment, "click on item 1(checkbox)");
            break;
 
        case 9:
            my_event.gx_event_type = GX_EVENT_PEN_DOWN;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_x = ITEM_1_X;
            gx_system_event_send(&my_event);

            my_event.gx_event_type = GX_EVENT_PEN_DRAG;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_x = ITEM_1_X + 5;
            gx_system_event_send(&my_event);
            sprintf(test_comment, "click on item 2(button) and drag right");
            break;

         case 10:
            my_event.gx_event_type = GX_EVENT_PEN_DOWN;
            gx_system_event_send(&my_event);
            
            my_event.gx_event_type = GX_EVENT_PEN_UP;
            gx_system_event_send(&my_event);
            sprintf(test_comment, "click on item 2(button)");
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
