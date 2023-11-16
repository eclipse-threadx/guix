/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_generic_scroll_wheel", /* Test name */
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
        case GX_SIGNAL(ID_SCROLL_WHEEL, GX_EVENT_LIST_SELECT):
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

void set_row_height(int height)
{
    gx_generic_scroll_wheel_row_height_set(&main_screen.main_screen_scroll_wheel, height);
    gx_generic_scroll_wheel_row_height_set(&main_screen.main_screen_scroll_wheel_nowrap, height);
    gx_generic_scroll_wheel_row_height_set(&main_screen.main_screen_scroll_wheel_wrap, height);
}

void set_total_rows(int total_rows)
{
    gx_generic_scroll_wheel_total_rows_set(&main_screen.main_screen_scroll_wheel, total_rows);
    gx_generic_scroll_wheel_total_rows_set(&main_screen.main_screen_scroll_wheel_nowrap, total_rows);
    gx_generic_scroll_wheel_total_rows_set(&main_screen.main_screen_scroll_wheel_wrap, total_rows);
}

void set_selected_row(int row, TX_SEMAPHORE *p_semaphore)
{
    gx_scroll_wheel_selected_set(&main_screen.main_screen_scroll_wheel, row);
    gx_scroll_wheel_selected_set(&main_screen.main_screen_scroll_wheel_nowrap, row);
    gx_scroll_wheel_selected_set(&main_screen.main_screen_scroll_wheel_wrap, row);

    if(p_semaphore)
    {
        tx_semaphore_get(p_sel_semaphore, TX_WAIT_FOREVER);
        tx_semaphore_get(p_sel_semaphore, TX_WAIT_FOREVER);
        tx_semaphore_get(p_sel_semaphore, TX_WAIT_FOREVER);
    }
}

static VOID control_thread_entry(ULONG input)
{
INT     frame_id = 1;
INT     index = 0;
GX_BOOL end_loop = GX_FALSE;
GX_GENERIC_SCROLL_WHEEL *wheel = &main_screen.main_screen_scroll_wheel;
GX_GENERIC_SCROLL_WHEEL *wheel_wrap = &main_screen.main_screen_scroll_wheel_wrap;
GX_SCROLLBAR vsb;

    p_sel_semaphore = &sel_semaphore;
    tx_semaphore_create(p_sel_semaphore, "test_semaphore", 0);

    gx_widget_event_process_set(&main_screen, test_main_screen_event_process);

    while(!end_loop)
    {
        switch(index)
        {
        case 0:
            /* Set total rows to 0.  */
            set_total_rows(0);
            sprintf(test_comment, "Set total rows to 0.");
            break;

        case 1:
            /* Set total rows to 6.  */
            set_total_rows(6);
            sprintf(test_comment, "Set total rows to 6.");
            break;

        case 2:
            /* Set total rows to 1.  */
            set_total_rows(1);
            sprintf(test_comment, "Set total rows to 1.");
            break;

        case 3:
            /* Set total rows to 3.  */
            set_total_rows(3);
            sprintf(test_comment, "Set total rows to 3.");
            break;

        case 4:
            /* Set total rows to 8.  */
            set_total_rows(8);
            sprintf(test_comment, "Set total rows to 8.");
            break;

        case 5:
            /* Set selected row to 2.  */
            set_selected_row(2, p_sel_semaphore);
            sprintf(test_comment, "Set selected row to 2.");
            break;

        case 6:
            set_selected_row(-1, p_sel_semaphore);
            sprintf(test_comment, "Set selected row to -1.");
            break;

        case 7:
            set_total_rows(0);
            sprintf(test_comment, "Set total rows to 0.");
            break;

        case 8:
            set_selected_row(1, GX_NULL);
            sprintf(test_comment, "Set selected row to 1.");
            break;

        case 9:
            set_total_rows(8);
            sprintf(test_comment, "Set total rows to 8.");
            break;

        case 10:
            set_selected_row(9, p_sel_semaphore);
            sprintf(test_comment, "Set selected row to 9.");
            break;

        case 11:
            set_total_rows(4);
            sprintf(test_comment, "Set total rows to 4.");
            break;

        case 12:
            set_row_height(30);
            sprintf(test_comment, "Set row height to 30.");
            break;

        case 13:
            gx_scroll_wheel_gradient_alpha_set(wheel, 0, 0);
            gx_scroll_wheel_selected_background_set(wheel, 0);
            sprintf(test_comment, "Remove gradient alpha and selected background for the first generic scroll wheel");
            break;

        case 14:
            gx_widget_hide(wheel);
            gx_generic_scroll_wheel_total_rows_set(wheel, 0);
            gx_widget_show(wheel);
            sprintf(test_comment, "hide first wheel, set total rows to 0, show wheel.");
            break;

        case 15:
            gx_widget_hide(wheel);
            gx_generic_scroll_wheel_total_rows_set(wheel, 8);
            gx_generic_scroll_wheel_children_position(wheel);
            gx_widget_show(wheel);
            sprintf(test_comment, "hide first wheel, set total rows to 8, position children, show wheel.");
            break;

        case 16:
            memset(&vsb, 0, sizeof(GX_SCROLLBAR));
            gx_vertical_scrollbar_create(&vsb, "vertical scroll bar", wheel_wrap, GX_NULL, GX_STYLE_ENABLED);
            sprintf(test_comment, "add vertical scroll bar to the third wheel");
            break;

        case 17:
            set_total_rows(6);
            sprintf(test_comment, "Set total rows to 6.");
            break;

        case 18:
            set_row_height(45);
            sprintf(test_comment, "Set row height to 45.");
            break;

        case 19:
            set_total_rows(8);
            sprintf(test_comment, "set total rows to 8.");
            break;

        case 20:
            set_selected_row(7, p_sel_semaphore);
            sprintf(test_comment, "Set selected row to 7.");
            break;

        case 21:
            gx_widget_back_move(&vsb, GX_NULL);
            set_selected_row(0, p_sel_semaphore);
            sprintf(test_comment, "back move vertical scrolbar, set selected row to 0");
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

