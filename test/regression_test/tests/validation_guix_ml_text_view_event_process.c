/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_ml_text_view_event_process", /* Test name */
    50, 119, 224, 317  /* Define the coordinates of the capture area.
                         In this test, we only need to capture the pixelmap slider
                         drawing area.  */
};

char test_string[]="why do the birds go on singing, why do the sea rush to shore, don't they know it's the end of the word.";

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

char test_comment[256];

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
INT                      frame_id = 1;
GX_MULTI_LINE_TEXT_VIEW *view;
int                      index;
GX_EVENT                 my_event;
GX_RECTANGLE             size;

    view = &main_screen.main_screen_text_view;
 
    memset(&my_event, 0, sizeof(GX_EVENT));

    size = view->gx_widget_size;
    size.gx_rectangle_right = size.gx_rectangle_left + 100;

    for(index = 0; index < 8; index++)
    {
        switch(index)
        {
        case 0:
            gx_widget_hide((GX_WIDGET *)view);
            sprintf(test_comment, "hide multi line text view");
            break;
        case 1:
            gx_widget_border_style_set((GX_WIDGET *)view, GX_STYLE_BORDER_THICK);
            sprintf(test_comment, "set border style thick");
            break;

        case 2:
            gx_widget_show((GX_WIDGET *)view);
            sprintf(test_comment, "show multi line text view");
            break;
        case 3:
            gx_multi_line_text_view_text_id_set(view, 0);
            gx_system_active_language_set(1);
            sprintf(test_comment, "set text id 0 and change active language");
            break;

        case 4:
            gx_multi_line_text_view_text_set(view, test_string);
            sprintf(test_comment, "set text");
            break;

        case 5:
            gx_widget_hide((GX_WIDGET *)view);
            sprintf(test_comment, "hide widget");
            break;

        case 6:
            gx_widget_resize((GX_WIDGET *)view, &size);
            sprintf(test_comment, "resize");
            break;

        case 7:
            gx_widget_show((GX_WIDGET *)view);
            sprintf(test_comment, "show widget");
            break;
        }
        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(test_comment);
        gx_validation_screen_refresh();
    }

    gx_widget_hide((GX_WIDGET *)view);
 
    my_event.gx_event_display_handle = 1;
    my_event.gx_event_type = GX_EVENT_RESIZED;
    my_event.gx_event_target = (GX_WIDGET *)view;
    gx_system_event_send(&my_event);

    tx_thread_sleep(2);
    if(view->gx_multi_line_text_view_line_index_old == GX_TRUE)
    {
        gx_validation_end();
        exit(1);
    }
    my_event.gx_event_type = GX_EVENT_CLIENT_UPDATED;
    my_event.gx_event_target = (GX_WIDGET *)view;
    gx_system_event_send(&my_event);

    tx_thread_sleep(2);
    if(view->gx_multi_line_text_view_line_index_old == GX_TRUE)
    {
        gx_validation_end();
        exit(1);
    }
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
