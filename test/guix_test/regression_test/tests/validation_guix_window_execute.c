/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_menu.h"
#include "gx_system.h"
#include "tx_thread.h"

TEST_PARAM test_parameter = {
    "guix_window_execute", /* Test name */
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

typedef struct TEST_STRUCT{
    INT   event_type;
    INT   xpos;
    INT   ypos;
    char *comment;
}TEST;

#define USER_EVENT_EXECUTE_WINDOW GX_FIRST_USER_EVENT
#define TEST_TIMER_ID 1024
TEST test_list[]={
{USER_EVENT_EXECUTE_WINDOW, 0,   0, "execute window"},
{GX_EVENT_TIMER,            0,   0, "start a timer and return 1"},
{USER_EVENT_EXECUTE_WINDOW, 0,   0, "execute window"},
{GX_EVENT_HIDE,             0,   0, "send hide event"},
{USER_EVENT_EXECUTE_WINDOW, 0,   0, "execute window"},
{GX_EVENT_CLOSE,            0,   0, "send close event"},
{USER_EVENT_EXECUTE_WINDOW, 0,   0, "execute window"},
{0,                         0,   0, "send event 0"},
{GX_EVENT_PEN_DOWN,         0,   0, "pen down on (0, 0)"},
{GX_EVENT_PEN_DRAG,         0,   0, "pen drag on (0, 0)"},
{GX_EVENT_PEN_UP,           0,   0, "pen up on (0, 0)"},
{GX_EVENT_PEN_DOWN,       370, 310, "pen down on (370, 310)"},
{GX_EVENT_PEN_DRAG,       370, 310, "pen drag on (370, 310)"},
{GX_EVENT_PEN_UP,         370, 370, "pen up on (370, 310)"},
{0, 0, 0, 0}
};

GX_BOOL set_return_code = GX_TRUE;
GX_BOOL initiate_tx_queue = GX_FALSE;

VOID test_popup_modal_draw(GX_WINDOW *window)
{
    gx_window_draw(window);

    if(initiate_tx_queue)
    {
        /* Delete the event queue.  */
        _tx_thread_preempt_disable = 1;
        initiate_tx_queue = GX_FALSE;
    }
}

UINT popup_modal_event_process(GX_WINDOW *window, GX_EVENT *event_ptr)
{
ULONG return_code;

    switch(event_ptr->gx_event_type)
    {
        case GX_EVENT_TIMER:
            if(event_ptr->gx_event_payload.gx_event_timer_id == TEST_TIMER_ID)
            {
                gx_system_timer_stop((GX_WIDGET *)window, TEST_TIMER_ID);
                return 1;
            }
            break;

        case USER_EVENT_EXECUTE_WINDOW:
            gx_widget_attach(&button_screen, &popup_modal);
            if(set_return_code)
            {
                gx_window_execute((GX_WINDOW *)&popup_modal, &return_code);
            }
            else
            {
                gx_window_execute((GX_WINDOW *)&popup_modal, GX_NULL);
            }
            _tx_thread_preempt_disable = 0;
            break;

        default:
            return gx_window_event_process(window, event_ptr);
    }
    return 0;
}

static VOID control_thread_entry(ULONG input)
{
INT        frame_id = 1;
GX_EVENT   my_event;
TEST      *entry = test_list;

    gx_widget_event_process_set((GX_WIDGET *)&popup_modal, popup_modal_event_process);
    gx_widget_draw_set(&popup_modal, test_popup_modal_draw);

    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_display_handle = 1;

    while(entry->comment)
    {
        /* Simulate click on text button to execute window. */
        switch(entry->event_type)
        {
        case GX_EVENT_TIMER:
            gx_system_timer_start((GX_WIDGET *)&popup_modal, TEST_TIMER_ID, 1, 1);
            tx_thread_sleep(2);
            break;
        
        case GX_EVENT_PEN_DOWN:
        case GX_EVENT_PEN_UP:
        case GX_EVENT_PEN_DRAG:
            my_event.gx_event_type = entry->event_type;
            my_event.gx_event_target = GX_NULL;
            my_event.gx_event_sender = 0;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_x = entry->xpos;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_y = entry->ypos;
            gx_system_event_send(&my_event);
            break;
        
        default:
            my_event.gx_event_type = entry->event_type;
            my_event.gx_event_target = (GX_WIDGET *)&popup_modal;
            my_event.gx_event_sender = 0;
            gx_system_event_send(&my_event);
            break;
        }

        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(entry->comment);
        gx_validation_screen_refresh();
        entry++;
    }

    initiate_tx_queue = GX_TRUE;

    gx_validation_screen_refresh();
    if(_gx_system_last_error != GX_SYSTEM_EVENT_RECEIVE_ERROR)
    {
        exit(1);
    }
    
    initiate_tx_queue = GX_TRUE;
    set_return_code = GX_FALSE;

    /* Execute window. */
    my_event.gx_event_type = USER_EVENT_EXECUTE_WINDOW;
    my_event.gx_event_target = (GX_WIDGET *)&popup_modal;
    my_event.gx_event_sender = 0;
    gx_system_event_send(&my_event);

    gx_validation_screen_refresh();
    if(_gx_system_last_error != GX_SYSTEM_EVENT_RECEIVE_ERROR)
    {
        exit(2);
    }

    /* execute window. */
    my_event.gx_event_type = USER_EVENT_EXECUTE_WINDOW;
    my_event.gx_event_target = (GX_WIDGET *)&popup_modal;
    my_event.gx_event_sender = 0;
    gx_system_event_send(&my_event);
    gx_system_event_send(&my_event);

    /* terminate executetion. */
    my_event.gx_event_type = GX_EVENT_TERMINATE;
    my_event.gx_event_target = (GX_WIDGET *)&popup_modal;
    my_event.gx_event_sender = 0;
    gx_system_event_send(&my_event);

    tx_thread_sleep(5);

    gx_validation_end();

    exit(0);
}

