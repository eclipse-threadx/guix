/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_menu.h"
#include "gx_system.h"

TEST_PARAM test_parameter = {
    "guix_accordion_menu_pen_up_event_process", /* Test name */
    18, 100, 150, 350  /* Define the coordinates of the capture area.
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

typedef struct TEST_STRUCT{
int event_type;
int point_x;
int point_y;
GX_BOOL wait;
char *comment;
}TEST;

#define DETACH_MENU_2 GX_FIRST_USER_EVENT
TEST test_list[]={
{GX_EVENT_PEN_DOWN, 95, 150, 0, "pen down on menu_1"},
{GX_EVENT_PEN_UP,   95, 150,20, "pen up on menu_1"},
{GX_EVENT_PEN_DOWN, 95, 150, 0, "pen down on menu_1"},
{GX_EVENT_PEN_UP,   95, 150,20, "pen up on menu_1"},
{0, 0, 0, 0, ""}
};

static VOID control_thread_entry(ULONG input)
{
INT frame_id = 1;
GX_ACCORDION_MENU *accordion;
TEST *test = test_list;
GX_EVENT my_event;
GX_RECTANGLE size;
int  index;

    accordion = &menu_screen.menu_screen_one_level_accordion;

    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_display_handle = 1;

    /* Toggle from menu screen to text screen. */
    ToggleScreen((GX_WINDOW *)&menu_screen, pButtonScreen);

    gx_utility_rectangle_define(&size, 20, 155, 149, 158);
    gx_widget_resize((GX_WIDGET *)&menu_screen.menu_screen_prompt_25, &size);

    gx_utility_rectangle_define(&size, 20,  179, 149, 182);
    gx_widget_resize((GX_WIDGET *)&menu_screen.menu_screen_prompt_26, &size);

    /* Reposition accordion menu. */
    gx_accordion_menu_position(accordion);

    for(index = 0; index < 2; index++)
    {

        if(index == 0)
        {
            gx_widget_shift(&menu_screen.menu_screen_ola_menu_1.gx_menu_list, 0, 10, GX_FALSE);
        }
        else
        {
            gx_widget_shift(&menu_screen.menu_screen_ola_menu_1.gx_menu_list, 10, 0, GX_FALSE);
        }

        test = test_list;
        while(test->event_type)
        {
            my_event.gx_event_type = test->event_type;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_x = test->point_x;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_y = test->point_y;
            gx_system_event_send(&my_event);
            if(test->wait)
            {
                while((accordion->gx_accordion_menu_animation_status & GX_MENU_ANIMATION_CLOSE) ||
                      (accordion->gx_accordion_menu_animation_status & GX_MENU_ANIMATION_OPEN))
                {
                    tx_thread_sleep(5);
                }
            }

            gx_validation_set_frame_id(frame_id++);
            gx_validation_set_frame_comment(test->comment);
            gx_validation_screen_refresh();

            test++;
        }    
    }
    gx_validation_end();

    exit(0);
}

