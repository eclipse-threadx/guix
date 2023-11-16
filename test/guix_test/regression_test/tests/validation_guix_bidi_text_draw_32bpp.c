/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

#define REGRESSION_TEST_MODE

TEST_PARAM test_parameter = {
    "guix_bidi_text_draw_32bpp", /* Test name */
    0, 40, 639, 479  /* Define the coordinates of the capture area.
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

#include "demo_bidi_text_draw_32bpp.c"

extern GX_CHAR file_name[];

char test_comment[256];

static VOID control_thread_entry(ULONG input)
{
int   frame_id = 1;
int   text_index;
int   frame_offset;
GX_EVENT my_event;
GX_SCROLL_INFO *info;
char pathname[255];
int  pathlen;

    gx_validation_extract_path(__FILE__, pathname, &pathlen);
    
    sprintf(file_name,  "%s../../../example_internal/bidi_text_draw_32bpp/bidi_character_test_cases.txt", pathname);

    info = &((main_screen).main_screen_vertical_scroll).gx_scrollbar_info;
    
    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_target = (GX_WIDGET *)&main_screen.main_screen_vertical_scroll;

    if(bidi_character_test() == GX_SUCCESS)
    {
      
        sprintf(pathname + pathlen, "../../../example_internal/bidi_text_draw_32bpp/readme_guix_generic.txt");
        for(text_index = 0; text_index < 2; text_index++)
        {
            while(info->gx_scroll_value < info -> gx_scroll_maximum - info -> gx_scroll_visible + 1)
            {
                if(text_index)
                {
                    PopulateMLTexts(pathname);
                    sprintf(test_comment, "English Text");
                    frame_offset = 40;
                }
                else
                {
                    sprintf(test_comment, "BiDi Text");
                    frame_offset = 2;
                }

                /* Scroll down. */
                strcat(test_comment, ", Scroll down");
                
                my_event.gx_event_type = GX_SIGNAL(GX_ID_SCROLL_DOWN_RIGHT, GX_EVENT_CLICKED);
                my_event.gx_event_sender = GX_ID_SCROLL_DOWN_RIGHT;
                gx_system_event_send(&my_event);

                if((info->gx_scroll_value % frame_offset) == 0)
                {
                    gx_validation_set_frame_id(frame_id++);
                    gx_validation_set_frame_comment(test_comment);
                    gx_validation_screen_refresh();

                    gx_system_bidi_text_disable();
                    gx_validation_set_frame_id(frame_id++);
                    sprintf(test_comment, "Disable bidi text");
                    gx_validation_set_frame_comment(test_comment);
                    gx_system_dirty_mark((GX_WIDGET *)&main_screen);
                    gx_validation_screen_refresh();
                    gx_system_bidi_text_enable();
                }
            }

            while(info->gx_scroll_value > info->gx_scroll_minimum)
            {
                if(text_index)
                {

                    PopulateMLTexts(pathname);
                    sprintf(test_comment, "English Text");
                    frame_offset = 20;
                }
                else
                {
                    sprintf(test_comment, "BiDi Text");
                    frame_offset = 1;
                }

                /* Scroll up. */
                strcat(test_comment, ", Scroll up");

                my_event.gx_event_type = GX_SIGNAL(GX_ID_SCROLL_UP_LEFT, GX_EVENT_CLICKED);
                my_event.gx_event_sender = GX_ID_SCROLL_UP_LEFT;
                gx_system_event_send(&my_event);

                if((info->gx_scroll_value % frame_offset) == 0)
                {
                    gx_validation_set_frame_id(frame_id++);
                    gx_validation_set_frame_comment(test_comment);
                    gx_validation_screen_refresh();

                    gx_system_bidi_text_disable();
                    gx_validation_set_frame_id(frame_id++);
                    sprintf(test_comment, "Disable bidi text");
                    gx_validation_set_frame_comment(test_comment);
                    gx_system_dirty_mark((GX_WIDGET *)&main_screen);
                    gx_validation_screen_refresh();
                    gx_system_bidi_text_enable();
                }
            }
        }

        gx_system_bidi_text_disable();

        gx_system_memory_allocator_set(GX_NULL, GX_NULL);
        gx_validation_set_frame_id(frame_id++);
        sprintf(test_comment, "bidi text enabled, memory allocator/free function is not set");
        gx_validation_set_frame_comment(test_comment);
        gx_system_dirty_mark((GX_WIDGET *)&main_screen);
        gx_validation_screen_refresh();
    }

    gx_multi_line_text_view_text_id_set(&main_screen.main_screen_text_view, GX_STRING_ID_HEWBREW_SONG);
    gx_system_memory_allocator_set(memory_allocate, memory_free);
    gx_system_bidi_text_disable();
    gx_system_active_language_set(LANGUAGE_HEBREW);
    sprintf(test_comment, "bidi text disabled, set language to Hebrew(reordered bidi text)");
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();

    binres_root_address = load_binary_resource_data_to_ram();
    if(load_language_table(binres_root_address) == GX_SUCCESS)
    {
        sprintf(test_comment, "bidi text disabled, load binary language resource(reordered bidi text)");
        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(test_comment);
        gx_validation_screen_refresh();
    }

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
