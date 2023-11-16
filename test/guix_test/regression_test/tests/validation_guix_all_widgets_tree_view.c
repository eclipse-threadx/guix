/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_menu.h"
#include "gx_system.h"

TEST_PARAM test_parameter = {
    "guix_all_widgets_tree_view", /* Test name */
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

enum action{
    CLICK_ITEM_ROOT = 1,
    CLICK_ITEM,
    SCROLL_DOWN,
    SCROLL_RIGHT,
    SCROLL_LEFT,
};

typedef struct{
    int        action;
    GX_WIDGET *menu;
}menu_info;

menu_info menu_list[] = {
    {CLICK_ITEM_ROOT, (GX_WIDGET *)&menu_screen.menu_screen_tree_menu_2},
    {CLICK_ITEM_ROOT, (GX_WIDGET *)&menu_screen.menu_screen_tree_menu_1},
    {CLICK_ITEM_ROOT, (GX_WIDGET *)&menu_screen.menu_screen_tree_menu_2},
    {CLICK_ITEM_ROOT, (GX_WIDGET *)&menu_screen.menu_screen_tree_menu_3},
    {CLICK_ITEM_ROOT, (GX_WIDGET *)&menu_screen.menu_screen_tree_menu_2_2},
    {CLICK_ITEM_ROOT, (GX_WIDGET *)&menu_screen.menu_screen_tree_menu_2_2_1},
    {CLICK_ITEM, (GX_WIDGET *)&menu_screen.menu_screen_tree_menu_1_1},
    {CLICK_ITEM, (GX_WIDGET *)&menu_screen.menu_screen_tree_menu_2_1},
    {SCROLL_DOWN, GX_NULL},
    {SCROLL_RIGHT, GX_NULL},
    {SCROLL_LEFT, GX_NULL},
    {CLICK_ITEM_ROOT, (GX_WIDGET *)&menu_screen.menu_screen_tree_menu_3},
    {CLICK_ITEM_ROOT, (GX_WIDGET *)&menu_screen.menu_screen_tree_menu_2_2_1},
    {CLICK_ITEM_ROOT, (GX_WIDGET *)&menu_screen.menu_screen_tree_menu_2_2},
    {CLICK_ITEM_ROOT, (GX_WIDGET *)&menu_screen.menu_screen_tree_menu_2},
    {GX_NULL, GX_NULL},
};

char test_comment[256];

static VOID control_thread_entry(ULONG input)
{
INT           frame_id = 1;
GX_EVENT      my_event;
GX_RECTANGLE *size;
menu_info    *list;
GX_WIDGET    *menu;
GX_SCROLLBAR *scroll;
INT           indentation = 30;

    /* Toggle from menu screen to text screen. */
    ToggleScreen((GX_WINDOW *)&menu_screen, pButtonScreen);

    /* Select a tree view menu. */
    gx_tree_view_selected_set(&menu_screen.menu_screen_tree_view, (GX_WIDGET *)&menu_screen.menu_screen_tree_menu_2_1_1);
    gx_validation_set_frame_id(frame_id++);
    sprintf(test_comment, "Select menu 2.1.1");
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();

    gx_tree_view_indentation_set(&menu_screen.menu_screen_tree_view, indentation);
    gx_validation_set_frame_id(frame_id++);
    sprintf(test_comment, "set indentation  to %d", indentation);
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();

    /* Init event structure. */
    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_display_handle = 1;

    list = menu_list;

    /* Click menu in the menu list. */
    while(list->action)
    {
        menu = list->menu;

        size = &menu->gx_widget_size;

        if(list->action == SCROLL_DOWN)
        {
            gx_window_scrollbar_find((GX_WINDOW *)&menu_screen.menu_screen_tree_view, GX_TYPE_VERTICAL_SCROLL, &scroll);

            if(scroll && (scroll->gx_widget_status & GX_STATUS_VISIBLE))
            {
                my_event.gx_event_payload.gx_event_intdata[1] = scroll->gx_scrollbar_info.gx_scroll_value;
                my_event.gx_event_payload.gx_event_intdata[0] = scroll->gx_scrollbar_info.gx_scroll_maximum;
                my_event.gx_event_type = GX_EVENT_VERTICAL_SCROLL;
                my_event.gx_event_target = (GX_WIDGET *)&menu_screen.menu_screen_tree_view;
                gx_system_event_send(&my_event);
                sprintf(test_comment, "scroll vertical scrollbar down");
            }
        }
        else if(list->action == SCROLL_RIGHT ||
                list->action == SCROLL_LEFT)
        {
            gx_window_scrollbar_find((GX_WINDOW *)&menu_screen.menu_screen_tree_view, GX_TYPE_HORIZONTAL_SCROLL, &scroll);

            if(scroll && (scroll->gx_widget_status & GX_STATUS_VISIBLE))
            {
                my_event.gx_event_payload.gx_event_intdata[1] = scroll->gx_scrollbar_info.gx_scroll_value;
                if(list->action == SCROLL_RIGHT)
                {
                    my_event.gx_event_payload.gx_event_intdata[0] = scroll->gx_scrollbar_info.gx_scroll_maximum;
                    sprintf(test_comment, "scroll horizontal scrollbar right");
                }
                else
                {
                    my_event.gx_event_payload.gx_event_intdata[0] = scroll->gx_scrollbar_info.gx_scroll_minimum;
                    sprintf(test_comment, "scroll horizontal scrollbar left");
                }
                my_event.gx_event_type = GX_EVENT_HORIZONTAL_SCROLL;
                my_event.gx_event_target = (GX_WIDGET *)&menu_screen.menu_screen_tree_view;
                gx_system_event_send(&my_event);
            }
        }
        else
        {
            GX_ENTER_CRITICAL

            /* Simulate pen down event. */
            my_event.gx_event_type = GX_EVENT_PEN_DOWN;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_y = (size->gx_rectangle_top +
                                                                       size->gx_rectangle_bottom) / 2;

            if(list->action == CLICK_ITEM_ROOT)
            {
                my_event.gx_event_payload.gx_event_pointdata.gx_point_x = size->gx_rectangle_left - (indentation/2);
                sprintf(test_comment, "click on \"%s\" menu root", menu->gx_widget_name);
            }
            else
            {
                my_event.gx_event_payload.gx_event_pointdata.gx_point_x = size->gx_rectangle_left + 1;
                sprintf(test_comment, "click on \"%s\" menu", menu->gx_widget_name);
            }
            my_event.gx_event_target = GX_NULL;
            gx_system_event_send(&my_event);
    
            /* Simulate pen up event. */
            my_event.gx_event_type = GX_EVENT_PEN_UP;
            gx_system_event_send(&my_event);

            GX_EXIT_CRITICAL
        }

        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(test_comment);
        gx_validation_screen_refresh();

        list++;
    }

    gx_tree_view_root_line_color_set(&menu_screen.menu_screen_tree_view, GX_COLOR_ID_ORANGE);
    gx_validation_set_frame_id(frame_id++);
    sprintf(test_comment, "set root line color to orange");
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();

    gx_widget_style_remove(&menu_screen.menu_screen_tree_view, GX_STYLE_TREE_VIEW_SHOW_ROOT_LINES);
    gx_validation_set_frame_id(frame_id++);
    sprintf(test_comment, "remove root lines");
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();

    gx_validation_end();

    exit(0);
}

