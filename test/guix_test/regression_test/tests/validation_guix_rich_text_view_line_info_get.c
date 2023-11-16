/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_rich_text_view_line_info_get", /* Test name */
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

static GX_UBYTE string_0[] = {0xfc, 0x00};
static char string_1[] = "<align left>A                                              B</align>";

GX_STRING test_string_list[]={
{(GX_CHAR *)string_0, sizeof(string_0) - 1},
{string_1, sizeof(string_1) - 1},
};

char comment[256];

static VOID control_thread_entry(ULONG input)
{
INT  frame_id = 1;
UINT index;
GX_RICH_TEXT_VIEW *rich_view = &main_screen.main_screen_rich_text_view;
GX_RECTANGLE size;

    /* Detach rich text view scrollbar. */
    gx_widget_detach(&main_screen.main_screen_vertical_scroll);

    for(index = 0; index < sizeof(test_string_list)/sizeof(GX_STRING); index++)
    {
         gx_multi_line_text_view_text_set_ext((GX_MULTI_LINE_TEXT_VIEW *)rich_view, &test_string_list[index]);
         gx_validation_set_frame_id(frame_id++);
         gx_validation_set_frame_comment("test line break");
         gx_validation_screen_refresh(); 
    }

    size = rich_view->gx_widget_size;
    size.gx_rectangle_right = size.gx_rectangle_left + 15;

    gx_widget_resize(rich_view, &size);
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Resize widget.");
    gx_validation_screen_refresh(); 

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
