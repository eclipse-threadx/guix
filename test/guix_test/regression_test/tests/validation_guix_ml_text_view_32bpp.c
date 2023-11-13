/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_system.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_ml_text_view_32bpp", /* Test name */
    20, 101, 264, 424  /* Define the coordinates of the capture area.
                         In this test, we only need to capture the multi 
                         line text view drawing area.  */
};

static VOID      control_thread_entry(ULONG);
int main(int argc, char ** argv)
{
#ifdef GX_UTF8_SUPPORT
    /* Parse the command line argument. */
    gx_validation_setup(argc, argv);
#else
    printf("guix_ml_text_view_32bpp: Skip Testing!\n");
#endif
    /* Start ThreadX system */
    tx_kernel_enter(); 
    return(0);
}

VOID tx_application_define(void *first_unused_memory)
{

    /* Create a dedicated thread to perform various operations
       on the multi line text view example. These operations simulate 
       user input. */
    gx_validation_control_thread_create(control_thread_entry);

    /* Terminate the test if it runs for more than 100 ticks */
    /* This function is not implemented yet. */
    gx_validation_watchdog_create(100);

    /* Call the actual multi line text view routine. */
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

#include "demo_guix_ml_text_view_32bpp.c"

char test_comment[256];
char ft_depth[3][5]={"1bpp", "4bpp", "8bpp"};
char ft_size[2][6]={"18pix", "24pix"};

char text_b[] = "text B";

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
#ifdef GX_UTF8_SUPPORT
int ypos;
int frame_id = 1;
int font_index, font_size_index, text_index;
GX_EVENT scroll_event;
GX_MULTI_LINE_TEXT_VIEW *view;
char pathname[255];
int  pathlen;

    memset(&scroll_event, 0, sizeof(GX_EVENT));

    view = &pMainScreen->window_text_view;

    gx_validation_extract_path(__FILE__, pathname, &pathlen);
    strcat(pathname, "../../../example_internal/ml_text_view_32bpp/readme_guix_generic.txt");

    PopulateMLTexts(pathname);
    gx_validation_set_frame_id(frame_id++);
    sprintf(test_comment, "Set multi line text view text.");
    gx_validation_screen_refresh();

    /* multi line text view scroll down. */
    for(ypos = 131; ypos < 388; ypos += 1)
    {
        /* Inform the validation system 
        (1) Frame ID, which identifies a specific test configuration;
        (2) Start recording frame on the next toggle operation.
        */
        GX_ENTER_CRITICAL
        scroll_event.gx_event_display_handle = 1;
        scroll_event.gx_event_type = GX_EVENT_PEN_DOWN;
        scroll_event.gx_event_payload.gx_event_pointdata.gx_point_y = ypos;
        scroll_event.gx_event_payload.gx_event_pointdata.gx_point_x = 245;
        gx_system_event_send(&scroll_event);

        scroll_event.gx_event_type = GX_EVENT_PEN_DRAG;
        scroll_event.gx_event_payload.gx_event_pointdata.gx_point_y = ypos + 1;
        gx_system_event_send(&scroll_event);

        scroll_event.gx_event_type = GX_EVENT_PEN_UP;
        scroll_event.gx_event_payload.gx_event_pointdata.gx_point_y = ypos + 1;
        gx_system_event_send(&scroll_event);
        GX_EXIT_CRITICAL

        gx_validation_set_frame_id(frame_id);
        sprintf(test_comment, "scroll down by 1");
        gx_validation_set_frame_comment(test_comment);
        gx_validation_screen_refresh();

        /* Increment frame ID. */
        frame_id ++;
    }

    /* multi line text view scroll up */
    for(ypos = 388; ypos >= 131; ypos -= 10)
    {
        /* Inform the validation system 
        (1) Frame ID, which identifies a specific test configuration;
        (2) Start recording frame on the next toggle operation.
        */
        GX_ENTER_CRITICAL
        scroll_event.gx_event_display_handle = 1;
        scroll_event.gx_event_type = GX_EVENT_PEN_DOWN;
        scroll_event.gx_event_payload.gx_event_pointdata.gx_point_y = ypos;
        scroll_event.gx_event_payload.gx_event_pointdata.gx_point_x = 245;
        gx_system_event_send(&scroll_event);

        scroll_event.gx_event_type = GX_EVENT_PEN_DRAG;
        scroll_event.gx_event_payload.gx_event_pointdata.gx_point_y = ypos - 10;
        gx_system_event_send(&scroll_event);

        scroll_event.gx_event_type = GX_EVENT_PEN_UP;
        scroll_event.gx_event_payload.gx_event_pointdata.gx_point_y = ypos - 10;
        gx_system_event_send(&scroll_event);
        GX_EXIT_CRITICAL

        gx_validation_set_frame_id(frame_id++);
        sprintf(test_comment, "scroll up by 5");
        gx_validation_set_frame_comment(test_comment);
        gx_validation_screen_refresh();
    }

    gx_multi_line_text_view_text_set(view, text_b);
    gx_validation_set_frame_id(frame_id ++);
    sprintf(test_comment, "set multi line text view text");
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();
 
    gx_widget_hide((GX_WIDGET *)view);
    gx_validation_set_frame_id(frame_id ++);
    sprintf(test_comment, "hide multi line text view");
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();
 
    gx_widget_show((GX_WIDGET *)view);
    gx_validation_set_frame_id(frame_id ++);
    sprintf(test_comment, "show multi line text view");
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();

    /* Remove scrollbar. */   
    gx_validation_set_frame_id(frame_id++);
    sprintf(test_comment, "remove scrollbar.");
    gx_validation_set_frame_comment(test_comment);
    scroll_event.gx_event_type = GX_EVENT_PEN_DOWN;
    scroll_event.gx_event_payload.gx_event_pointdata.gx_point_y = 112;
    scroll_event.gx_event_payload.gx_event_pointdata.gx_point_x = 290;
    gx_system_event_send(&scroll_event);

    for(text_index = 0; text_index < 2; text_index++)
    {
         if(text_index == 1)
         {
             gx_multi_line_text_view_text_id_set(view, GX_STRING_ID_STRING_UTF8);
         }

         for(font_index = 0; font_index < 3; font_index ++)
         {
             for(font_size_index = 0; font_size_index < 2; font_size_index++)
             {
                 sprintf(test_comment, "font: %s, font size: %s, ", ft_depth[font_index], ft_size[font_size_index]);
                 if(text_index == 1)
                 {
                      strcat(test_comment, "utf8 text");
                 }
                 else
                 {
                      strcat(test_comment, "ascii text");
                 }
                 
                 gx_validation_set_frame_comment(test_comment);
                 gx_validation_set_frame_id(frame_id++);
                 gx_multi_line_text_view_font_set(view, font[font_index][font_size_index]);
                
                 /* Force a screen refresh. */
                 gx_validation_screen_refresh();
             }
         }
    }
          
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();
#endif
    exit(0);
}
