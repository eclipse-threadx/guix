/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_menu.h"
#include "gx_system.h"

TEST_PARAM test_parameter = {
    "guix_all_widgets_execute", /* Test name */
    0, 0, 639, 479  /* Define the coordinates of the capture area.
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
#include "demo_guix_all_widgets_execute.c"

char *test_comment[1];

typedef struct SCREEN_INFO_STRUCT{
    GX_RESOURCE_ID id;
    char          *name;
    int            animation_steps;
}SCREEN_INFO;

SCREEN_INFO screen_info[]={
    {ID_BUTTON_SCREEN, "button_screen", 21},
    {ID_WINDOW_SCREEN, "window_screen", 21},
    {ID_INDICATOR_SCREEN, "indicator_screen", 37},
    {ID_TEXT_SCREEN, "text_screen", 1},
    {ID_GAUGE_SCREEN, "gauge_screen", 11},
    {ID_SPRITE_SCREEN, "sprite_screen", 1},
    {ID_SCROLL_WHEEL_SCREEN, "scroll_wheel_screen", 21},
    {ID_MENU_SCREEN, "menu", 21},
    {0, "", 0}
};

static VOID control_thread_entry(ULONG input)
{
INT        frame_id;
GX_EVENT   my_event;
GX_WIDGET *parent;
SCREEN_INFO *entry = screen_info;

    memset(&my_event, 0, sizeof(GX_EVENT));

    test_comment[0] = (char *)malloc(256);

    while(entry->id)
    {
        gx_widget_find((GX_WIDGET *)root, entry->id, GX_SEARCH_DEPTH_INFINITE, &parent);

        if(entry->id == ID_BUTTON_SCREEN)
        {
            /* Test window execute. */
            gx_validation_current_frame_id_get(&frame_id);
            frame_id++;
            gx_validation_set_frame_id(frame_id);
            sprintf(test_comment[0], "Execute Window");
            gx_validation_set_frame_comment(test_comment[0]);

            /* Simulate click on text button to execute window. */
            my_event.gx_event_type = GX_SIGNAL(ID_TEXT_BUTTON, GX_EVENT_CLICKED);
            my_event.gx_event_target = parent;
            my_event.gx_event_sender = ID_TEXT_BUTTON;

            gx_system_event_send(&my_event);

            gx_validation_current_frame_id_get(&frame_id);
            frame_id++;
            gx_validation_set_frame_id(frame_id);
            sprintf(test_comment[0], "Stop Execute Window");
            gx_validation_set_frame_comment(test_comment[0]);

            /* Simulate click on ok button to stop window execute. */ 
            my_event.gx_event_type = GX_SIGNAL(IDB_OK, GX_EVENT_CLICKED);
            my_event.gx_event_target = (GX_WIDGET *)&popup_modal;
            my_event.gx_event_sender = IDB_OK;
            gx_system_event_send(&my_event);
        }

        /* Search next button. */
GX_ENTER_CRITICAL
        my_event.gx_event_type = GX_SIGNAL(IDB_NEXT, GX_EVENT_CLICKED);
        my_event.gx_event_target = parent;
        my_event.gx_event_sender = IDB_NEXT;

        sprintf(test_comment[0], "Switch from %s to next", entry->name);

        gx_validation_current_frame_id_get(&frame_id);
        frame_id ++;

        /* start_id, expected_frames, comments, num_comments, max_time. */
        gx_validation_capture_frames(frame_id, entry->animation_steps, (char **)test_comment, 1, 1000);
        gx_system_event_send(&my_event);
GX_EXIT_CRITICAL
        gx_validation_capture_frames_wait();

        tx_thread_sleep(2);
        entry++;
    }

    free(test_comment[0]);
    gx_validation_end();

    exit(0);
}

