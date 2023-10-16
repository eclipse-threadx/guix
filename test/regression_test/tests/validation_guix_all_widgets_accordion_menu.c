/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_menu.h"
#include "gx_system.h"

TEST_PARAM test_parameter = {
    "guix_all_widgets_accordion_menu", /* Test name */
    18, 100, 450, 450  /* Define the coordinates of the capture area.
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

typedef struct{
    GX_ACCORDION_MENU *owner;
    GX_WIDGET *menu;
}menu_info;

menu_info menu_list[] = {
    {&menu_screen.menu_screen_one_level_accordion, (GX_WIDGET *)&menu_screen.menu_screen_ola_menu_4},
    {&menu_screen.menu_screen_one_level_accordion, (GX_WIDGET *)&menu_screen.menu_screen_ola_menu_3},
    {&menu_screen.menu_screen_one_level_accordion, (GX_WIDGET *)&menu_screen.menu_screen_ola_menu_2},
    {&menu_screen.menu_screen_one_level_accordion, (GX_WIDGET *)&menu_screen.menu_screen_ola_menu_1},
    {&menu_screen.menu_screen_one_level_accordion_2, (GX_WIDGET *)&menu_screen.menu_screen_ola_2_menu_1},
    {&menu_screen.menu_screen_one_level_accordion_2, (GX_WIDGET *)&menu_screen.menu_screen_ola_2_menu_3},
    {&menu_screen.menu_screen_ola_2_menu_2, (GX_WIDGET *)&menu_screen.menu_screen_ola_2_menu_2_1},
    {&menu_screen.menu_screen_ola_2_menu_2, (GX_WIDGET *)&menu_screen.menu_screen_ola_2_menu_2_2},
    {&menu_screen.menu_screen_multi_level_accordion, (GX_WIDGET *)&menu_screen.menu_screen_mla_menu_1},
    {&menu_screen.menu_screen_mla_menu_1_accordion, (GX_WIDGET *)&menu_screen.menu_screen_mla_menu_1_1},
    {&menu_screen.menu_screen_mla_menu_1_accordion, (GX_WIDGET *)&menu_screen.menu_screen_mla_menu_1_2},
    {&menu_screen.menu_screen_multi_level_accordion, (GX_WIDGET *)&menu_screen.menu_screen_mla_menu_2},
    {&menu_screen.menu_screen_mla_menu_2_accordion, (GX_WIDGET *)&menu_screen.menu_screen_mla_menu_2_1},
    {&menu_screen.menu_screen_mla_menu_2_1_accordion, (GX_WIDGET *)&menu_screen.menu_screen_mla_menu_2_1_1},
    {&menu_screen.menu_screen_mla_menu_2_1_accordion, (GX_WIDGET *)&menu_screen.menu_screen_mla_menu_2_1_2},
    {&menu_screen.menu_screen_mla_menu_2_accordion, (GX_WIDGET *)&menu_screen.menu_screen_mla_menu_2_2},
    {&menu_screen.menu_screen_mla_menu_2_2_accordion, (GX_WIDGET *)&menu_screen.menu_screen_mla_menu_2_2_1},
    {&menu_screen.menu_screen_mla_menu_2_2_accordion, (GX_WIDGET *)&menu_screen.menu_screen_mla_menu_2_2_2},
    {&menu_screen.menu_screen_multi_level_accordion, (GX_WIDGET *)&menu_screen.menu_screen_mla_menu_3},
    {GX_NULL, GX_NULL},
};

char test_comment[256];

static VOID control_thread_entry(ULONG input)
{
INT           frame_id = 1;
GX_EVENT      my_event;
GX_RECTANGLE *size;
menu_info          *list;
GX_WIDGET          *menu;
GX_ACCORDION_MENU  *accordion;
#ifndef GX_DISABLE_ERROR_CHECKING
GX_ACCORDION_MENU   test_widget_create;
    
    /* Test accordion menu create. */
    memset(&test_widget_create, 0, sizeof(GX_ACCORDION_MENU));
    gx_accordion_menu_create(&test_widget_create, "", GX_NULL, GX_NULL, GX_NULL, &menu_screen.gx_widget_size);
    gx_accordion_menu_create(GX_NULL, "", GX_NULL, GX_NULL, GX_NULL, &menu_screen.gx_widget_size);
    gx_accordion_menu_create(&test_widget_create, "", GX_NULL, GX_NULL, GX_NULL, GX_NULL);
#endif
    gx_widget_style_add((GX_WIDGET *)&menu_screen.menu_screen_ola_menu_1, GX_STYLE_MENU_EXPANDED);
    gx_widget_style_add((GX_WIDGET *)&menu_screen.menu_screen_ola_menu_2, GX_STYLE_MENU_EXPANDED);
        
    /* Toggle from menu screen to text screen. */
    ToggleScreen((GX_WINDOW *)&menu_screen, pButtonScreen);

    sprintf(test_comment, "Add EXPANDED style for ola_menu_1 and ola_menu_2");
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();

    gx_widget_style_remove((GX_WIDGET *)&menu_screen.menu_screen_ola_menu_1, GX_STYLE_MENU_EXPANDED);
    gx_widget_style_remove((GX_WIDGET *)&menu_screen.menu_screen_ola_menu_2, GX_STYLE_MENU_EXPANDED);
    gx_accordion_menu_position(&menu_screen.menu_screen_one_level_accordion);

    sprintf(test_comment, "Remove EXPANDED style from ola_menu_1 and ola_menu_2");
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();

    /* Init event structure. */
    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_display_handle = 1;

    list = menu_list;

    /* Click menu in the menu list. */
    while(list->owner)
    {
        accordion = list->owner;
        menu = list->menu;

        size = &menu->gx_widget_size;

        GX_ENTER_CRITICAL

        /* Simulate pen down event. */
        my_event.gx_event_type = GX_EVENT_PEN_DOWN;
        my_event.gx_event_payload.gx_event_pointdata.gx_point_x = (size->gx_rectangle_left +
                                                                   size->gx_rectangle_right) / 2;
        my_event.gx_event_payload.gx_event_pointdata.gx_point_y = (size->gx_rectangle_top +
                                                                   size->gx_rectangle_bottom) / 2;
        gx_system_event_send(&my_event);
    
        /* Simulate pen up event. */
        my_event.gx_event_type = GX_EVENT_PEN_UP;
        gx_system_event_send(&my_event);

        GX_EXIT_CRITICAL

        tx_thread_sleep(5);

        while((accordion->gx_accordion_menu_animation_status & GX_MENU_ANIMATION_CLOSE) ||
              (accordion->gx_accordion_menu_animation_status & GX_MENU_ANIMATION_OPEN))
        {
            tx_thread_sleep(1);
        }

        gx_validation_set_frame_id(frame_id++);
        sprintf(test_comment, "click on \"%s\"", menu->gx_widget_name);
        gx_validation_set_frame_comment(test_comment);
        gx_validation_screen_refresh();

        list++;
    }

    gx_validation_end();

    exit(0);
}

