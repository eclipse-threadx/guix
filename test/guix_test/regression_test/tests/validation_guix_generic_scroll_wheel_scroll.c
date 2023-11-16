/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_generic_scroll_wheel_scroll", /* Test name */
    36, 101, 602, 313    /* Define the coordinates of the capture area.
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
#ifdef win32_graphics_driver_setup_565rgb  
#undef win32_graphics_driver_setup_565rgb  
#endif
#define win32_graphics_driver_setup_565rgb  gx_validation_graphics_driver_setup_565rgb


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_guix_generic_scroll_wheel.c"

char test_comment[255];

TX_SEMAPHORE *p_sel_semaphore = GX_NULL;
TX_SEMAPHORE sel_semaphore;

UINT test_main_screen_event_process(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    switch(event_ptr->gx_event_type)
    {
        case GX_SIGNAL(ID_SCROLL_WHEEL_NOWRAP, GX_EVENT_LIST_SELECT):
        case GX_SIGNAL(ID_SCROLL_WHEEL_WRAP, GX_EVENT_LIST_SELECT):
            if(p_sel_semaphore)
            {
                tx_semaphore_put(p_sel_semaphore);
            }
            break;

        default:
            return main_screen_event_process(window, event_ptr);
    }

    return 0;
}

static VOID control_thread_entry(ULONG input)
{
INT     frame_id = 1;
INT     index = 0;
GX_BOOL end_loop = GX_FALSE;
GX_GENERIC_SCROLL_WHEEL *wheel = &main_screen.main_screen_scroll_wheel_nowrap;
GX_GENERIC_SCROLL_WHEEL *wheel_wrap = &main_screen.main_screen_scroll_wheel_wrap;
GX_EVENT my_event;
GX_RECTANGLE *size;
GX_WIDGET *child;

    memset(&my_event, 0, sizeof(GX_EVENT));

    p_sel_semaphore = &sel_semaphore;
    tx_semaphore_create(p_sel_semaphore, "test_semaphore", 0);

    gx_widget_event_process_set(&main_screen, test_main_screen_event_process);

    while(!end_loop)
    {
        switch(index)
        {
        case 0:
            size = &wheel->gx_widget_size;
            my_event.gx_event_target = (GX_WIDGET *)wheel;
            my_event.gx_event_type = GX_EVENT_PEN_DOWN;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_x = (size->gx_rectangle_left + size->gx_rectangle_right) >> 1;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_y = (size->gx_rectangle_top + size->gx_rectangle_bottom) >> 1;
            gx_system_event_send(&my_event);
            
            my_event.gx_event_type = GX_EVENT_PEN_DRAG;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_y += 10;
            gx_system_event_send(&my_event);

            sprintf(test_comment, "Drag down by 10.");

            tx_thread_sleep(15);
            break;

        case 1:
            my_event.gx_event_type = GX_EVENT_PEN_UP;
            gx_system_event_send(&my_event);
            tx_semaphore_get(p_sel_semaphore, TX_WAIT_FOREVER);

            sprintf(test_comment, "Send pen up event.");
            break;

        case 2:
            gx_scroll_wheel_selected_set(wheel, wheel->gx_scroll_wheel_total_rows - 1);
            tx_semaphore_get(p_sel_semaphore, TX_WAIT_FOREVER);
            sprintf(test_comment, "Set last row selected.");
            break;

        case 3:
            my_event.gx_event_type = GX_EVENT_PEN_DOWN;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_x = (size->gx_rectangle_left + size->gx_rectangle_right) >> 1;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_y = (size->gx_rectangle_top + size->gx_rectangle_bottom) >> 1;
            gx_system_event_send(&my_event);

            my_event.gx_event_type = GX_EVENT_PEN_DRAG;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_y -= 10;
            gx_system_event_send(&my_event);

            sprintf(test_comment, "Drag up by 10.");

            tx_thread_sleep(15);
            break;

        case 4:
            my_event.gx_event_type = GX_EVENT_PEN_UP;
            gx_system_event_send(&my_event);
            tx_semaphore_get(p_sel_semaphore, TX_WAIT_FOREVER);

            sprintf(test_comment, "Send pen up event.");
            break;

        case 5:
            size = &wheel_wrap->gx_widget_size;
            my_event.gx_event_target = (GX_WIDGET *)wheel_wrap;
            my_event.gx_event_type = GX_EVENT_PEN_DOWN;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_x = (size->gx_rectangle_left + size->gx_rectangle_right) >> 1;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_y = (size->gx_rectangle_top + size->gx_rectangle_bottom) >> 1;
            gx_system_event_send(&my_event);

            my_event.gx_event_type = GX_EVENT_PEN_DRAG;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_y += 45;
            gx_system_event_send(&my_event);
            tx_thread_sleep(15);

            my_event.gx_event_type = GX_EVENT_PEN_UP;
            gx_system_event_send(&my_event);
            tx_semaphore_get(p_sel_semaphore, TX_WAIT_FOREVER);

            sprintf(test_comment, "Drag down by 45");
            break;

        case 6:
            my_event.gx_event_target = (GX_WIDGET *)wheel_wrap;
            my_event.gx_event_type = GX_EVENT_PEN_DOWN;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_x = (size->gx_rectangle_left + size->gx_rectangle_right) >> 1;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_y = (size->gx_rectangle_top + size->gx_rectangle_bottom) >> 1;
            gx_system_event_send(&my_event);

            my_event.gx_event_type = GX_EVENT_PEN_DRAG;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_y -= 45;
            gx_system_event_send(&my_event);
            tx_thread_sleep(15);

            my_event.gx_event_type = GX_EVENT_PEN_UP;
            gx_system_event_send(&my_event);
            tx_semaphore_get(p_sel_semaphore, TX_WAIT_FOREVER);

            sprintf(test_comment, "Drag up by 45");
            break;

        case 7:
            child = wheel_wrap->gx_widget_first_child;
            wheel_wrap->gx_widget_first_child = GX_NULL;
            my_event.gx_event_target = (GX_WIDGET *)wheel_wrap;
            my_event.gx_event_type = GX_EVENT_PEN_DOWN;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_x = (size->gx_rectangle_left + size->gx_rectangle_right) >> 1;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_y = (size->gx_rectangle_top + size->gx_rectangle_bottom) >> 1;
            gx_system_event_send(&my_event);

            my_event.gx_event_type = GX_EVENT_PEN_DRAG;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_y += 45;
            gx_system_event_send(&my_event);
            tx_thread_sleep(15);

            my_event.gx_event_type = GX_EVENT_PEN_UP;
            gx_system_event_send(&my_event);
            tx_semaphore_get(p_sel_semaphore, TX_WAIT_FOREVER);

            sprintf(test_comment, "Set first child of wrap wheel to GX_NULL, and drag down by 45");
            break;

        case 8:
            wheel_wrap->gx_widget_first_child = child;
            my_event.gx_event_target = (GX_WIDGET *)wheel_wrap;
            my_event.gx_event_type = GX_EVENT_PEN_DOWN;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_x = (size->gx_rectangle_left + size->gx_rectangle_right) >> 1;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_y = (size->gx_rectangle_top + size->gx_rectangle_bottom) >> 1;
            gx_system_event_send(&my_event);

            my_event.gx_event_type = GX_EVENT_PEN_DRAG;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_y += 45;
            gx_system_event_send(&my_event);
            tx_thread_sleep(15);

            my_event.gx_event_type = GX_EVENT_PEN_UP;
            gx_system_event_send(&my_event);
            tx_semaphore_get(p_sel_semaphore, TX_WAIT_FOREVER);

            sprintf(test_comment, "Restore first child of wrap wheel and drag down by 45");
            break;

        case 9:
            child =wheel_wrap->gx_widget_last_child;
            wheel_wrap->gx_widget_last_child = GX_NULL;
            my_event.gx_event_target = (GX_WIDGET *)wheel_wrap;
            my_event.gx_event_type = GX_EVENT_PEN_DOWN;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_x = (size->gx_rectangle_left + size->gx_rectangle_right) >> 1;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_y = (size->gx_rectangle_top + size->gx_rectangle_bottom) >> 1;
            gx_system_event_send(&my_event);

            my_event.gx_event_type = GX_EVENT_PEN_DRAG;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_y -= 45;
            gx_system_event_send(&my_event);
            tx_thread_sleep(15);

            my_event.gx_event_type = GX_EVENT_PEN_UP;
            gx_system_event_send(&my_event);
            tx_semaphore_get(p_sel_semaphore, TX_WAIT_FOREVER);

            sprintf(test_comment, "Set last child of wrap wheel to GX_NULL, and drag up by 45");
            break;

        case 10:
            wheel_wrap->gx_widget_last_child = child;
            gx_generic_scroll_wheel_total_rows_set(wheel_wrap, 8);

            my_event.gx_event_target = (GX_WIDGET *)wheel_wrap;
            my_event.gx_event_type = GX_EVENT_PEN_DOWN;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_x = (size->gx_rectangle_left + size->gx_rectangle_right) >> 1;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_y = (size->gx_rectangle_top + size->gx_rectangle_bottom) >> 1;
            gx_system_event_send(&my_event);

            my_event.gx_event_type = GX_EVENT_PEN_DRAG;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_y -= (wheel_wrap->gx_scroll_wheel_row_height/3);
            gx_system_event_send(&my_event);
            tx_thread_sleep(15);

            my_event.gx_event_type = GX_EVENT_PEN_UP;
            gx_system_event_send(&my_event);
            tx_semaphore_get(p_sel_semaphore, TX_WAIT_FOREVER);
            sprintf(test_comment, "Restore last child, drag up by 1/3 of row height.");
            break;

        case 11:
            wheel_wrap->gx_generic_scroll_wheel_callback = GX_NULL;
            my_event.gx_event_target = (GX_WIDGET *)wheel_wrap;
            my_event.gx_event_type = GX_EVENT_PEN_DOWN;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_x = (size->gx_rectangle_left + size->gx_rectangle_right) >> 1;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_y = (size->gx_rectangle_top + size->gx_rectangle_bottom) >> 1;
            gx_system_event_send(&my_event);

            my_event.gx_event_type = GX_EVENT_PEN_DRAG;
            my_event.gx_event_payload.gx_event_pointdata.gx_point_y -= 45;
            gx_system_event_send(&my_event);
            tx_thread_sleep(15);

            my_event.gx_event_type = GX_EVENT_PEN_UP;
            gx_system_event_send(&my_event);
            tx_semaphore_get(p_sel_semaphore, TX_WAIT_FOREVER);

            sprintf(test_comment, "Set wrap wheel callback to NULL, and drag up by 45");
            break;

        case 12:
            wheel->gx_scroll_wheel_row_height = 0;
            gx_generic_scroll_wheel_children_position(wheel);
            sprintf(test_comment, "Set wheel row height to 0 and call children position.");
            break;


        case 13:
            wheel->gx_scroll_wheel_row_height = 45;
            wheel->gx_scroll_wheel_total_rows = 0;
            gx_generic_scroll_wheel_children_position(wheel);
            sprintf(test_comment, "Set wheel total rows to 0 and call children position.");
            break;

        default:
            end_loop = GX_TRUE;
            break;
        }

        if(!end_loop)
        {
            gx_validation_set_frame_id(frame_id++);
            gx_validation_set_frame_comment(test_comment);
            gx_validation_screen_refresh();
            index++;
        }
    }

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}

