/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_horizontal_list_scroll", /* Test name */
    50, 50, 290, 460  /* Define the coordinates of the capture area.
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

#include "demo_guix_horizontal_list_32bpp.c"
typedef struct TEST_STRUCT{
    INT event_type;
    GX_VALUE xpos;
    GX_VALUE ypos;
    GX_CHAR *comment;
}TEST;

#define XPOS 140
#define YPOS 360

TEST test_list[]={
    {GX_EVENT_PEN_DOWN, XPOS,      YPOS, "pen down"},
    {GX_EVENT_PEN_DRAG, XPOS + 10, YPOS, "drag right"},
    {GX_EVENT_PEN_UP,   XPOS,      YPOS, "pen up"},
    {GX_EVENT_PEN_DOWN, XPOS,      YPOS, "pen down"},
    {GX_EVENT_PEN_DRAG, XPOS - 10, YPOS, "drag left"},
    {GX_EVENT_PEN_UP,   XPOS - 10, YPOS, "pen up"},
    {0, 0, 0, 0}
};

char test_comment[256];

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
INT             frame_id = 1;
GX_EVENT        my_event;
GX_SCROLL_INFO *info;
TEST           *test;
GX_HORIZONTAL_LIST *scroll;

    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_target = (GX_WIDGET *)&window.window_horizontal_list_scroll;

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
    info = &window.window_hscroll_1.gx_scrollbar_info;
    my_event.gx_event_target = (GX_WIDGET *)&window.window_hscroll_1;
 
    sprintf(test_comment, "right end button clicked");
    while(info->gx_scroll_value < info->gx_scroll_maximum - info->gx_scroll_visible + 1)
    {
        my_event.gx_event_type = GX_SIGNAL(GX_ID_SCROLL_DOWN_RIGHT, GX_EVENT_CLICKED);
        my_event.gx_event_sender = GX_ID_SCROLL_DOWN_RIGHT;
        gx_system_event_send(&my_event);

        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(test_comment);
        gx_system_dirty_mark((GX_WIDGET *)&window.window_horizontal_list_scroll);
        gx_validation_screen_refresh();
    }

    /* Set thick border. */
    gx_widget_border_style_set(&window.window_horizontal_list_scroll, GX_STYLE_BORDER_THICK);
    gx_widget_hide(&window.window_horizontal_list_scroll);
    gx_widget_show(&window.window_horizontal_list_scroll);

    sprintf(test_comment, "left end button clicked");
    while(info->gx_scroll_value > info->gx_scroll_minimum)
    {
        my_event.gx_event_type = GX_SIGNAL(GX_ID_SCROLL_UP_LEFT, GX_EVENT_CLICKED);
        my_event.gx_event_sender = GX_ID_SCROLL_UP_LEFT;
        gx_system_event_send(&my_event);

        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(test_comment);
        gx_system_dirty_mark((GX_WIDGET *)&window.window_horizontal_list_scroll);
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
   
    gx_widget_hide(&window.window_horizontal_list_scroll);
    gx_horizontal_list_page_index_set(&window.window_horizontal_list_scroll, 7); 
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("hide horizontal list, and set page index to 7");
    gx_validation_screen_refresh();

    /* Set list column to 0 and send scroll event. */
    scroll = &window.window_horizontal_list_scroll;
    gx_widget_style_remove(scroll, (GX_STYLE_WRAP | GX_STYLE_TRANSPARENT));
    gx_widget_border_style_set(scroll, GX_STYLE_BORDER_THIN);
    gx_horizontal_list_total_columns_set(scroll, 0);
    gx_widget_show(scroll);
    my_event.gx_event_target = (GX_WIDGET *)scroll;
    my_event.gx_event_type = GX_EVENT_HORIZONTAL_SCROLL;
    my_event.gx_event_payload.gx_event_intdata[1] = 100;
    my_event.gx_event_payload.gx_event_intdata[0] = 0;
    gx_system_event_send(&my_event);

    gx_validation_set_frame_id(frame_id);
    gx_validation_set_frame_comment("set list column = 0, send horizontal scroll event");
    gx_validation_screen_refresh();

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
