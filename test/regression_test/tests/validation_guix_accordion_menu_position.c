/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_menu.h"
#include "gx_system.h"

TEST_PARAM test_parameter = {
    "guix_accordion_menu_position", /* Test name */
    19, 107, 150, 479  /* Define the coordinates of the capture area.
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
INT frame_id = 1;
INT index;
GX_ACCORDION_MENU *accordion = &menu_screen.menu_screen_one_level_accordion;

    for(index = 0; index < 7; index++)
    {

        switch(index)
        {
        case 0:
            /* Toggle from menu screen to text screen. */
            gx_widget_style_add(&menu_screen.menu_screen_ola_menu_1, GX_STYLE_MENU_EXPANDED);
            ToggleScreen((GX_WINDOW *)&menu_screen, pButtonScreen);
            sprintf(test_comment, "add GX_STYLE_MENU_EXPANED style to menu_1, and toggle to menu_screen");
            break;

        case 1:
            gx_widget_detach(&menu_screen.menu_screen_prompt_25);
            gx_widget_detach(&menu_screen.menu_screen_prompt_26);
            sprintf(test_comment, "dettach menu_1.item_1 and menu_1.item_2");
            break;

        case 2:
            gx_widget_style_add(&menu_screen.menu_screen_ola_menu_2, GX_STYLE_MENU_EXPANDED);
            sprintf(test_comment, "add expaned style to menu_2");
            break;
        
        case 3:
            gx_accordion_menu_position(accordion);
            sprintf(test_comment, "reposition accordion menu");
            break;

        case 4:
            gx_widget_style_remove(&menu_screen.menu_screen_ola_menu_2, GX_STYLE_MENU_EXPANDED);
            gx_widget_style_add(&menu_screen.menu_screen_ola_menu_4, GX_STYLE_MENU_EXPANDED);
            gx_accordion_menu_position(accordion);
            sprintf(test_comment, "reset expanded menu");
            break;

        case 5:
            gx_widget_attach(accordion, &menu_screen.menu_screen_ola_2_menu_2);
            gx_accordion_menu_position(accordion);
            sprintf(test_comment, "insert accordion menu as menu item");
            break;

        case 6:
            gx_widget_detach(&menu_screen.menu_screen_ola_2_menu_2_1);
            gx_widget_detach(&menu_screen.menu_screen_ola_2_menu_2_2);
            gx_accordion_menu_position(accordion);
            sprintf(test_comment, "dettach all children to inserted accordion and reposition");
            break;
        }

        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(test_comment);
        gx_validation_screen_refresh();
    }

    gx_validation_end();

    exit(0);
}

