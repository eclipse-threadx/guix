/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_scroll_thumb_event_process", /* Test name */
    347, 43, 567, 187  /* Define the coordinates of the capture area.
                         In this test, we only need to capture the scrollbar
                         drawing area.  */
};

VOID _gx_scrollbar_thumb_position_calculate(GX_SCROLLBAR *scroll);
static VOID      control_thread_entry(ULONG);
int main(int argc, char ** argv)
{
    /* Parse the command scrollbar argument. */
    gx_validation_setup(argc, argv);

    /* Start ThreadX system */
    tx_kernel_enter(); 
    return(0);
}

VOID tx_application_define(void *first_unused_memory)
{

    /* Create a dedicated thread to perform various operations
       on the scrollbar drawing example. These operations simulate 
       user input. */
    gx_validation_control_thread_create(control_thread_entry);

    /* Termiante the test if it runs for more than 100 ticks */
    /* This function is not implemented yet. */
    gx_validation_watchdog_create(100);

    /* Call the actual scrollbar routine. */
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

typedef struct TEST_STRUCT{
int event_type;
int xpos;
int ypos;
char *comment;
}TEST;

TEST test_list[]=
{
{GX_EVENT_PEN_UP,   555, 73, "pen up on (555, 73)"},
{GX_EVENT_PEN_DOWN, 555, 73, "pen down on (555, 73)"},
{GX_EVENT_PEN_DRAG, 555, 73, "pen drag on (555, 73)"},
{GX_EVENT_PEN_UP,   555, 73, "pen up on (555, 73)"},
{GX_EVENT_PEN_DOWN, 402, 169, "pen down on (402, 169)"},
{GX_EVENT_PEN_DRAG, 402, 169, "pen drag on (402, 169)"},
{GX_EVENT_PEN_UP,   402, 169, "pen up on (402, 169)"},
{GX_EVENT_PEN_DOWN, 402, 169, "pen down on (402, 169)"},
{GX_EVENT_PEN_DRAG, 390, 169, "pen drag on (390, 169)"},
{GX_EVENT_PEN_DRAG, 380, 169, "pen drag on (380, 169)"},
{GX_EVENT_PEN_UP,   400, 169, "pen up on (400, 169)"},
{0,                   0,   0, ""}
};
/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int           frame_id = 1;
GX_EVENT      my_event;
TEST         *test = test_list;
GX_SCROLLBAR *scroll = &window_screen.window_screen_vertical_scroll_1;

    ToggleScreen(pWindowScreen, pButtonScreen);
    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_display_handle = 1;

    while(test->event_type)
    {
        my_event.gx_event_type = test->event_type;
        my_event.gx_event_payload.gx_event_pointdata.gx_point_x = test->xpos;
        my_event.gx_event_payload.gx_event_pointdata.gx_point_y = test->ypos;
        gx_system_event_send(&my_event);

        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(test->comment);
        gx_validation_screen_refresh();

        test++;
    }

    gx_widget_hide(scroll);
    scroll->gx_scrollbar_thumb.gx_scroll_thumb_pixelmap = GX_PIXELMAP_ID_I_ORANGEFILL_BOTTOM;
    gx_widget_show(scroll);
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("set transparent thumb pixelmap");
    gx_validation_screen_refresh();

    gx_widget_hide(scroll);
    scroll->gx_scrollbar_thumb.gx_scroll_thumb_pixelmap = 1024;
    gx_widget_show(scroll);
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("set thumb pixelmap an invalid id");
    gx_validation_screen_refresh();

    /* gx_scroll_maximum == gx_scroll_minmum */
    gx_widget_style_add((GX_WIDGET *)scroll, GX_SCROLLBAR_RELATIVE_THUMB);
    scroll -> gx_scrollbar_info.gx_scroll_maximum = scroll -> gx_scrollbar_info.gx_scroll_minimum;
    _gx_scrollbar_thumb_position_calculate(scroll);
    /* gx_scroll_maximum == gx_scroll_minmum - 1 */
    scroll -> gx_scrollbar_info.gx_scroll_maximum = scroll -> gx_scrollbar_info.gx_scroll_minimum - 1;
    _gx_scrollbar_thumb_position_calculate(scroll);

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
