/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_system_clipboard_put", /* Test name */
    56, 196, 363, 231  /* Define the coordinates of the capture area.
                         In this test, we only need to capture the pixelmap
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
#include "demo_guix_all_widgets.c"

char test_comment[255];

VOID *test_memory_allocate(ULONG size)
{
    return GX_NULL;
}

VOID test_memory_free(VOID *mem)
{
}

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int                        frame_id = 1;
GX_SINGLE_LINE_TEXT_INPUT *sl_line_text_input;
GX_EVENT                   my_event;
INT                        index;

    gx_widget_detach(pButtonScreen);
    gx_widget_attach(root, pTextScreen);

    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_display_handle = 1;
    
    sl_line_text_input = &text_screen.text_screen_text_input_1;

    gx_single_line_text_input_style_remove(sl_line_text_input, GX_STYLE_CURSOR_BLINK);
    gx_single_line_text_input_text_set(sl_line_text_input, "This is single line text.");

    /* Simulate pen down and pen up event on single line input to gain focus. */
    my_event.gx_event_payload.gx_event_pointdata.gx_point_x = sl_line_text_input->gx_widget_size.gx_rectangle_right - 10;
    my_event.gx_event_payload.gx_event_pointdata.gx_point_y = sl_line_text_input->gx_widget_size.gx_rectangle_bottom - 10;
    my_event.gx_event_type = GX_EVENT_PEN_DOWN;
    gx_system_event_send(&my_event);

    my_event.gx_event_type = GX_EVENT_PEN_UP;
    gx_system_event_send(&my_event);
      
    my_event.gx_event_target = (GX_WIDGET *)sl_line_text_input;

    for(index = 0; index < 8; index++)
    {
        switch(index)
        {
        case 0:
            /* Send mark home event. */
            my_event.gx_event_type = GX_EVENT_MARK_HOME;
            gx_system_event_send(&my_event);
            
            sprintf(test_comment, "Send event mark home.");
            break;

        case 1: 
            /* Send cut event. */     
            my_event.gx_event_type = GX_EVENT_CUT;
            gx_system_event_send(&my_event);
           
            sprintf(test_comment, "Send event cut.");
            break;
 
        case 2: 
            /* Send paste event. */
            my_event.gx_event_type = GX_EVENT_PASTE;
            gx_system_event_send(&my_event);

            sprintf(test_comment, "Send event paste.");
            break;

        case 3:
            /* Send system allocator and free NULL. */
            gx_system_memory_allocator_set(GX_NULL, GX_NULL);

            /* Send home event. */
            my_event.gx_event_payload.gx_event_ushortdata[0] = GX_KEY_HOME;
            my_event.gx_event_type = GX_EVENT_KEY_DOWN;
            gx_system_event_send(&my_event);

            /* Send end event. */
            my_event.gx_event_type = GX_EVENT_MARK_END;
            gx_system_event_send(&my_event);
            sprintf(test_comment, "set system allocate function to null. And select all text.");
            break;

        case 4: 
            /* Send cut event. */
            my_event.gx_event_type = GX_EVENT_CUT;
            gx_system_event_send(&my_event);
            sprintf(test_comment, "Send event cut.");
            break;

        case 5:
            /* Send paste event. */ 
            my_event.gx_event_type = GX_EVENT_PASTE;
            gx_system_event_send(&my_event);
            sprintf(test_comment, "Send event paste.");
            break;  
  
        case 6:
            /* Reset system memory allocator and free function. */
            gx_system_memory_allocator_set(test_memory_allocate, test_memory_free);
            gx_single_line_text_input_text_set(sl_line_text_input, "test");
            sprintf(test_comment, "set text \"test\"");
            break;

        case 7:
            gx_single_line_text_input_text_select(sl_line_text_input, 0, 1);
            my_event.gx_event_type = GX_EVENT_CUT;
            gx_system_event_send(&my_event);

            my_event.gx_event_type = GX_EVENT_PASTE;
            gx_system_event_send(&my_event);
            sprintf(test_comment, "set text[0, 1], send cut event, send paste event.");
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
