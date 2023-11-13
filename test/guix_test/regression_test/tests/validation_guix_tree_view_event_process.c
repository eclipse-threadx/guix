/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_menu.h"
#include "gx_system.h"

TEST_PARAM test_parameter = {
    "guix_tree_view_event_process", /* Test name */
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

void empty_tree_view(GX_TREE_VIEW *tree)
{
GX_WIDGET *child;
GX_WIDGET *next;

    child = tree->gx_widget_first_child;

    while(child)
    {
        next = child->gx_widget_next;

        gx_widget_detach(child);

        child = next;
    }
}

static VOID control_thread_entry(ULONG input)
{
INT           frame_id = 1;
GX_EVENT      my_event;
GX_MENU_LIST  menu_list;
GX_BUTTON     button;
GX_MENU       menu;
GX_RECTANGLE  size;
GX_TREE_VIEW *tree_view;
int           index;

    tree_view = &menu_screen.menu_screen_tree_view;

    gx_utility_rectangle_define(&size, 486, 247, 565, 270);

    memset(&button, 0, sizeof(GX_BUTTON));
    gx_button_create(&button, "button", tree_view, GX_STYLE_ENABLED, 2014, &size);

    gx_utility_rectangle_define(&size, 486, 271, 565, 294);

    /* Added GX_MENU_LIST type item to tree view. */
    memset(&menu_list, 0, sizeof(GX_MENU_LIST));
    gx_widget_create((GX_WIDGET *)&menu_list, "menu_list", tree_view, 0, 1025, &size);
    menu_list.gx_widget_type = GX_TYPE_MENU_LIST;
 
    memset(&menu, 0, sizeof(GX_MENU));
    gx_utility_rectangle_define(&size, 486, 295, 565, 315);
    gx_menu_create(&menu, "menu", tree_view, 0, 0, GX_STYLE_ENABLED | GX_STYLE_MENU_EXPANDED, 1026, &size);

    gx_widget_style_remove(tree_view, GX_STYLE_TREE_VIEW_SHOW_ROOT_LINES);

    /* Toggle from menu screen to text screen. */
    ToggleScreen((GX_WINDOW *)&menu_screen, pButtonScreen);

    my_event.gx_event_target = (GX_WIDGET *)tree_view;
    for(index = 0; index < 12; index++)
    {
        switch(index)
        {
        case 0:
        case 1:
        case 2:
        case 3:
            if(index == 2)
            {
                gx_widget_hide(tree_view);
                sprintf(test_comment, "hide widget and click on expand icon of menu 2");
            }
            else if(index == 3)
            {
                gx_widget_show(tree_view);
                gx_tree_view_root_pixelmap_set(tree_view, 0, 0);
                sprintf(test_comment, "show widget, set root pixelmaps null, click on expan icon of menu 3");
            }
            else
            {

                sprintf(test_comment, "click on expand icon of menu 3");
            }
            my_event.gx_event_type = GX_EVENT_PEN_DOWN;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_x = 474;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_y = 162;
            gx_system_event_send(&my_event);

            my_event.gx_event_type = GX_EVENT_PEN_UP;
            gx_system_event_send(&my_event);
            break;

        case 4:
            my_event.gx_event_type = GX_EVENT_PEN_DOWN;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_x = 491;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_y = 102;
            gx_system_event_send(&my_event);

            my_event.gx_event_type = GX_EVENT_PEN_UP;
            gx_system_event_send(&my_event);
            sprintf(test_comment, "click on tree view border");
            break;

        case 5:
            my_event.gx_event_type = GX_EVENT_PEN_DOWN;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_x = 505;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_y = 232;
            gx_system_event_send(&my_event);

            my_event.gx_event_type = GX_EVENT_PEN_UP;
            gx_system_event_send(&my_event);
            sprintf(test_comment, "click on GX_BUTTON type item");
            break;

        case 6:
            my_event.gx_event_type = GX_EVENT_PEN_DOWN;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_x = 505;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_y = 260;
            gx_system_event_send(&my_event);

            my_event.gx_event_type = GX_EVENT_PEN_UP;
            gx_system_event_send(&my_event);
            sprintf(test_comment, "click on GX_MENU type item");
            break;

        case 7:
            gx_widget_detach(&button);
            gx_widget_attach(tree_view, &button);
            gx_widget_style_add(tree_view, GX_STYLE_TREE_VIEW_SHOW_ROOT_LINES);
            gx_tree_view_position(tree_view);
            sprintf(test_comment, "detach and attach test_button, show root lines");
            break;

        case 8:
            gx_widget_hide(tree_view);
            gx_tree_view_position(tree_view);
            sprintf(test_comment, "Hide tree view and repostion");
            break;

        case 9:
            gx_widget_back_attach(tree_view, &menu_list);
            gx_widget_show(tree_view);
            sprintf(test_comment, "back attach GX_MENU_LIST type item, and show tree view");
            break;

        case 10:
            my_event.gx_event_type = GX_EVENT_PEN_DOWN;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_x = 505;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_y = 260;
            gx_system_event_send(&my_event);

            my_event.gx_event_type = GX_EVENT_PEN_UP;
            gx_system_event_send(&my_event);
            sprintf(test_comment, "click on GX_BUTTON type item");
            break;

        case 11:
            empty_tree_view(tree_view);
            my_event.gx_event_type = GX_EVENT_PEN_DOWN;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_x = 505;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_y = 260;
            gx_system_event_send(&my_event);

            my_event.gx_event_type = GX_EVENT_PEN_UP;
            gx_system_event_send(&my_event);
            sprintf(test_comment, "click on point(505, 260)");
            break;

        }
        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(test_comment);
        gx_validation_screen_refresh();
    }
    gx_validation_end();

    exit(0);
}

