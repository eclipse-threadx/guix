/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_menu.h"
#include "gx_system.h"

TEST_PARAM test_parameter = {
    "guix_tree_view_scroll", /* Test name */
    460, 100, 628, 355  /* Define the coordinates of the capture area.
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

char test_comment[256];

static VOID control_thread_entry(ULONG input)
{
INT           frame_id = 1;
GX_EVENT      my_event;
GX_TREE_VIEW *tree_view;
INT           index;

    tree_view = &menu_screen.menu_screen_tree_view;

    /* Toggle from menu screen to text screen. */
    ToggleScreen((GX_WINDOW *)&menu_screen, pButtonScreen);

    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_target = (GX_WIDGET *)tree_view;
    for(index = 0; index < 5; index++)
    {
        switch(index)
        {
        case 0:
            gx_widget_style_add(tree_view, GX_STYLE_TRANSPARENT);
            sprintf(test_comment, "add GX_STYLE_TRANSPARENT style");
            break;


        case 1:
        case 4:
            if(index == 4)
            {
                /* Detach vertial scroll bar. */
                gx_widget_detach(&menu_screen.menu_screen_tree_vscroll);
                sprintf(test_comment, "detach vertical scroll bar and scroll down by 100");
            }
            else
            {
                sprintf(test_comment, "scroll down by 100");
            }
            my_event.gx_event_type = GX_EVENT_VERTICAL_SCROLL;
            my_event.gx_event_payload.gx_event_intdata[1] = 100;
            my_event.gx_event_payload.gx_event_intdata[0] = 0;
            gx_system_event_send(&my_event);
            break;

        case 2:
            gx_widget_style_remove(tree_view, GX_STYLE_TRANSPARENT);
            gx_widget_border_style_set(tree_view, GX_STYLE_BORDER_THICK);
            sprintf(test_comment, "remove transparent style, set thick border");
            break;

        case 3:
            my_event.gx_event_type = GX_EVENT_HORIZONTAL_SCROLL;
            my_event.gx_event_payload.gx_event_intdata[1] = 100;
            my_event.gx_event_payload.gx_event_intdata[0] = 0;
            gx_system_event_send(&my_event);
            sprintf(test_comment, "scroll right by 100");
            break;
        }
        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(test_comment);
        gx_validation_screen_refresh();
    }
    gx_validation_end();

    exit(0);
}

