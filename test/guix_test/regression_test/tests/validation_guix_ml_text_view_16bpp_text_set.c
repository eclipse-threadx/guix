/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_ml_text_view_16bpp_text_set", /* Test name */
    50, 119, 224, 317  /* Define the coordinates of the capture area.
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
#ifdef win32_graphics_driver_setup_565rgb  
#undef win32_graphics_driver_setup_565rgb  
#endif
#define win32_graphics_driver_setup_565rgb  gx_validation_graphics_driver_setup_565rgb


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_guix_ml_text_view_16bpp.c"

GX_CHAR test_text[]="\r33\r32\r31\r30\r29\r28\r27\r26\r25\r24\r23\r22\r21\r20\r19\r18\r17\r16\r15\r14\r13\r12\r11\r10\r9\r8\r7\r6\r5\r4\r3\r2\r1";
char test_comment[256];

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
INT             frame_id = 1;
GX_EVENT        my_event;
GX_SCROLL_INFO *info;
GX_MULTI_LINE_TEXT_VIEW *view;
INT index;

    view = &main_screen.main_screen_text_view;

    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_target = (GX_WIDGET *)&main_screen.main_screen_vertical_scroll_1;

    info = &main_screen.main_screen_vertical_scroll_1.gx_scrollbar_info;
 
    sprintf(test_comment, "down end button clicked");
    while(info->gx_scroll_value < info->gx_scroll_maximum - info->gx_scroll_visible + 1)
    {
        my_event.gx_event_type = GX_SIGNAL(GX_ID_SCROLL_DOWN_RIGHT, GX_EVENT_CLICKED);
        my_event.gx_event_sender = GX_ID_SCROLL_DOWN_RIGHT;
        gx_system_event_send(&my_event);

        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(test_comment);
        gx_validation_screen_refresh();
    }

    for(index = 0; index < 4; index++)
    {
        switch(index)
        {
        case 0:
            sprintf(test_comment, "Set NULL text");
            gx_multi_line_text_view_text_set(view, GX_NULL);
            break;
        case 1:
            sprintf(test_comment, "Added GX_STYLE_TEXT_COPY style, set new text");
            gx_widget_style_add(view, GX_STYLE_TEXT_COPY);
            gx_multi_line_text_view_text_set(view, test_text);
            break;
        case 2:
            sprintf(test_comment, "Set text id to 0");
            gx_multi_line_text_view_text_id_set(view, 0);
            break;
        case 3:
            sprintf(test_comment, "Set new text id");
            gx_multi_line_text_view_text_id_set(view, GX_STRING_ID_ML_VIEW_TEXT);
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
