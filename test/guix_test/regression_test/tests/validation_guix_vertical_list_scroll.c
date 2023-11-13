/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_vertical_list_scroll", /* Test name */
    70, 85, 620, 330  /* Define the coordinates of the capture area.
                         In this test, we only need to capture the pixelmap slider
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
       on the pixelmap drawing example. These operations simulate 
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

#include "demo_guix_vertical_list_32bpp.c"
typedef struct TEST_STRUCT{
    INT event_type;
    GX_VALUE xpos;
    GX_VALUE ypos;
    GX_CHAR *comment;
}TEST;

#define XPOS 480
#define YPOS 160

TEST test_list[]={
    {GX_EVENT_PEN_DOWN, XPOS, YPOS,      "pen down"},
    {GX_EVENT_PEN_DRAG, XPOS, YPOS + 10, "drag down"},
    {GX_EVENT_PEN_UP,   XPOS, YPOS + 10, "pen up"},
    {GX_EVENT_PEN_DOWN, XPOS, YPOS,      "pen down"},
    {GX_EVENT_PEN_DRAG, XPOS, YPOS - 10, "drag up"},
    {GX_EVENT_PEN_UP,   XPOS, YPOS - 10, "pen up"},
    {0, 0, 0, 0}
};

char test_comment[256];

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
INT               frame_id = 1;
GX_EVENT          my_event;
GX_SCROLL_INFO   *info;
TEST             *test;
GX_VERTICAL_LIST *list = &window.window_vertical_list_scroll;
int               index;

    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_target = (GX_WIDGET *)&window.window_vertical_list_scroll;

    /* Test horizontal list previous and next item select handler. */
    sprintf(test_comment, "select next item");
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment(test_comment);
    my_event.gx_event_type = GX_EVENT_FOCUS_NEXT;
    gx_system_event_send(&my_event);
    
    sprintf(test_comment, "select previous item");
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment(test_comment);
    my_event.gx_event_type = GX_EVENT_FOCUS_PREVIOUS;
    gx_system_event_send(&my_event);
   
    /* Test horizontal list scroll. */
    info = &window.window_vertical_scroll.gx_scrollbar_info;
    my_event.gx_event_target = (GX_WIDGET *)&window.window_vertical_scroll;
 
    sprintf(test_comment, "down end button clicked");
    while(info->gx_scroll_value < info->gx_scroll_maximum - info->gx_scroll_visible + 1)
    {
        my_event.gx_event_type = GX_SIGNAL(GX_ID_SCROLL_DOWN_RIGHT, GX_EVENT_CLICKED);
        my_event.gx_event_sender = GX_ID_SCROLL_DOWN_RIGHT;
        gx_system_event_send(&my_event);

        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(test_comment);
        gx_system_dirty_mark((GX_WIDGET *)&window.window_vertical_list_scroll);
        gx_validation_screen_refresh();
    }

    sprintf(test_comment, "up end button clicked");
    while(info->gx_scroll_value > info->gx_scroll_minimum)
    {
        my_event.gx_event_type = GX_SIGNAL(GX_ID_SCROLL_UP_LEFT, GX_EVENT_CLICKED);
        my_event.gx_event_sender = GX_ID_SCROLL_UP_LEFT;
        gx_system_event_send(&my_event);

        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(test_comment);
        gx_system_dirty_mark((GX_WIDGET *)&window.window_vertical_list_scroll);
        gx_validation_screen_refresh();
    }

    /* Test button list scroll. */
    test = test_list;
    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_display_handle = 1;

    while(test->event_type)
    {
        my_event.gx_event_type = test->event_type;
        my_event.gx_event_payload.gx_event_pointdata.gx_point_x = test->xpos;
        my_event.gx_event_payload.gx_event_pointdata.gx_point_y = test->ypos;
        gx_system_event_send(&my_event);
        tx_thread_sleep(11);

        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(test->comment);
        gx_validation_screen_refresh();

        test++;
    }
   
    for(index = 0; index < 4; index++)
    {
        switch(index)
        {
        case 0:
        case 1:
            if(index == 0)
            {
                gx_widget_border_style_set(list, GX_STYLE_BORDER_THICK);
                sprintf(test_comment, "set thick border");
            }
            else
            {
                gx_widget_hide(list);
                sprintf(test_comment, "hide widget");
            }
            my_event.gx_event_target = (GX_WIDGET *)list;
            my_event.gx_event_payload.gx_event_intdata[1] = 0;
            my_event.gx_event_payload.gx_event_intdata[0] = 100;
            my_event.gx_event_type = GX_EVENT_VERTICAL_SCROLL;
            gx_system_event_send(&my_event);
    
            strcat(test_comment, ", scroll up by 100");
            break;

        case 2:
            gx_widget_show(list);
            sprintf(test_comment, "show widget");
            break;

        case 3:
            gx_widget_style_remove(list, GX_STYLE_WRAP | GX_STYLE_TRANSPARENT);
            gx_widget_border_style_set(list, GX_STYLE_BORDER_THIN);
            gx_vertical_list_total_rows_set(list, 0);
            my_event.gx_event_target = (GX_WIDGET *)list;
            my_event.gx_event_type = GX_EVENT_VERTICAL_SCROLL;
            my_event.gx_event_payload.gx_event_intdata[1] = 100;
            my_event.gx_event_payload.gx_event_intdata[0] = 0;
            gx_system_event_send(&my_event);
            sprintf(test_comment, "set total rows = 0, send vertical scroll event");
            break;
        }

        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(test_comment);
        gx_validation_screen_refresh();
    }

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
