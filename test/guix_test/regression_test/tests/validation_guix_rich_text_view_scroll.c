/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_rich_text_view_scroll", /* Test name */
    99, 94, 295, 357  /* Define the coordinates of the capture area.
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

#include "demo_guix_rich_text_view_16bpp.c"

char comment[256];

char test_string[] = "1\r2\r3\r4\r5\r6\r7\r8\r9\r10\r11\r\n12\n13\r14\r15\r16\r17\r";

static VOID control_thread_entry(ULONG input)
{
INT  frame_id = 1;
GX_RICH_TEXT_VIEW *rich_view = &main_screen.main_screen_rich_text_view;
GX_MULTI_LINE_TEXT_VIEW *ml_view = (GX_MULTI_LINE_TEXT_VIEW *)rich_view;
GX_EVENT myevent;
GX_RICH_TEXT_FONTS fonts;
GX_STRING string;
INT       index;

    memset(&myevent, 0, sizeof(GX_EVENT));
    myevent.gx_event_payload.gx_event_intdata[1] = 0;
    myevent.gx_event_payload.gx_event_intdata[0] = 100;
    myevent.gx_event_type = GX_EVENT_VERTICAL_SCROLL;
    myevent.gx_event_display_handle = 1;
    myevent.gx_event_target = (GX_WIDGET *)ml_view;

    /* Set test string. */
    gx_multi_line_text_view_text_id_set(ml_view, GX_STRING_ID_RICH_TEXT_4);

    index = 0;
    while(index >= 0)
    {
        switch(index)
        {
        case 0:
            gx_system_event_send(&myevent);
            sprintf(comment, "Scroll down.");
            break;

        case 1:
            gx_multi_line_text_view_whitespace_set(ml_view, 0);
            sprintf(comment, "Swt whitespace to 0.");
            break;

        case 2:
            myevent.gx_event_payload.gx_event_intdata[1] = 100;
            myevent.gx_event_payload.gx_event_intdata[0] = 0;
            gx_system_event_send(&myevent);
            sprintf(comment, "Scroll up");
            break;

        case 3:
            string.gx_string_ptr = test_string;
            string.gx_string_length = sizeof(test_string) - 1;
            gx_multi_line_text_view_text_set_ext(ml_view, &string);
            sprintf(comment, "Set test string.");
            break;

        case 4:
            memset(&fonts, 0, sizeof(GX_RICH_TEXT_FONTS));
            fonts.gx_rich_text_fonts_normal_id = 1000;
            gx_rich_text_view_fonts_set(rich_view, &fonts);
 
            myevent.gx_event_payload.gx_event_intdata[1] = 0;
            myevent.gx_event_payload.gx_event_intdata[0] = 100;
   
            gx_system_event_send(&myevent);

            sprintf(comment, "Set invalid font and scroll down.");
            break;

         default:
            index = -1;
            continue;
        }

        index++;
        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(comment);
        gx_validation_screen_refresh();
    }

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
