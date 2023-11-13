/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_rich_text_view_font", /* Test name */
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

static char text_invalid_font_0[] = "<f 1000>Invalid Font ID</f>";
static char text_invalid_font_1[] = "</b>bold <f 1000>Invalid Font ID</f>text</b>";
static char text_invalid_font_2[] = "<f 1";
static char text_invalid_font_3[] = "<f 12";
static char text_invalid_font_4[] = "<f .>Invalid Font ID</f>";
static char text_invalid_font_5[] = "<f a>Invalid Font ID</f>";

GX_STRING test_string_list[]={
{text_invalid_font_0, sizeof(text_invalid_font_0) - 1},
{text_invalid_font_1, sizeof(text_invalid_font_1) - 1},
{text_invalid_font_2, sizeof(text_invalid_font_2) - 1},
{text_invalid_font_3, sizeof(text_invalid_font_3) - 1},
{text_invalid_font_4, sizeof(text_invalid_font_4) - 1},
{text_invalid_font_5, sizeof(text_invalid_font_4) - 1}
};

char comment[256];

static VOID control_thread_entry(ULONG input)
{
INT  frame_id = 1;
UINT index;
GX_RICH_TEXT_FONTS fonts;
GX_RICH_TEXT_VIEW *rich_view = &main_screen.main_screen_rich_text_view;
GX_MULTI_LINE_TEXT_VIEW *ml_view = (GX_MULTI_LINE_TEXT_VIEW *)rich_view;

    /* Detach rich text view scrollbar. */
    gx_widget_detach(&main_screen.main_screen_vertical_scroll);

    for(index = 0; index < sizeof(test_string_list)/sizeof(GX_STRING); index++)
    {
         gx_multi_line_text_view_text_set_ext(ml_view, &test_string_list[index]);
         gx_validation_set_frame_id(frame_id++);
         gx_validation_set_frame_comment("test font tag contains invalid font id.");
         gx_validation_screen_refresh();   
    }

    memset(&fonts, 0, sizeof(GX_RICH_TEXT_FONTS));
    gx_multi_line_text_view_text_id_set((GX_MULTI_LINE_TEXT_VIEW *)rich_view, GX_STRING_ID_RICH_TEXT_1);
    gx_rich_text_view_fonts_set(rich_view, &fonts);
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Set all fonts to default system font.");
    gx_validation_screen_refresh();

    /* Test invalid font. */
    fonts.gx_rich_text_fonts_normal_id = 1000;
    gx_rich_text_view_fonts_set(rich_view, &fonts);
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Set invalid normal font id.");
    gx_validation_screen_refresh();

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
