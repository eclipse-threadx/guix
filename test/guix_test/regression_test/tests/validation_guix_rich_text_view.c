/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_rich_text_view", /* Test name */
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

typedef struct TEXT_INFO_STRUCT{
GX_RESOURCE_ID text_id;
char id_name[30];
}TEXT_INFO;

TEXT_INFO text_info_list[]={
{GX_STRING_ID_RICH_TEXT_1, "GX_STRING_ID_RICH_TEXT_1"},
{GX_STRING_ID_RICH_TEXT_2, "GX_STRING_ID_RICH_TEXT_2"},
{GX_STRING_ID_RICH_TEXT_3, "GX_STRING_ID_RICH_TEXT_3"},
{GX_STRING_ID_RICH_TEXT_4, "GX_STRING_ID_RICH_TEXT_4"},
{GX_STRING_ID_RICH_TEXT_5, "GX_STRING_ID_RICH_TEXT_5"},
{GX_STRING_ID_RICH_TEXT_LEFT_ALIGN, "GX_STRING_ID_RICH_TEXT_LEFT_ALIGN"},
{GX_STRING_ID_RICH_TEXT_CENTER_ALIGN, "GX_STRING_ID_RICH_TEXT_CENTER_ALIGN"},
{GX_STRING_ID_RICH_TEXT_RIGHT_ALIGN, "GX_STRING_ID_RICH_TEXT_RIGHT_ALIGN"},
{GX_STRING_ID_RICH_TEXT_BOLD_ITALIC, "GX_STRING_ID_RICH_TEXT_BOLD_ITALIC"},
{GX_STRING_ID_RICH_TEXT_ITALIC_BOLD, "GX_STRING_ID_RICH_TEXT_ITALIC_BOLD"},
{GX_STRING_ID_RICH_TEXT_TAG_TEST_1, "GX_STRING_ID_RICH_TEXT_TAG_TEST_1"},
{GX_STRING_ID_RICH_TEXT_TAG_TEST_2, "GX_STRING_ID_RICH_TEXT_TAG_TEST_2"},
{GX_STRING_ID_RICH_TEXT_TAG_TEST_3, "GX_STRING_ID_RICH_TEXT_TAG_TEST_3"},
{GX_STRING_ID_RICH_TEXT_TAG_TEST_4, "GX_STRING_ID_RICH_TEXT_TAG_TEST_4"},
{GX_STRING_ID_RICH_TEXT_TAG_TEST_5, "GX_STRING_ID_RICH_TEXT_TAG_TEST_5"},
{GX_STRING_ID_RICH_TEXT_TAG_TEST_6, "GX_STRING_ID_RICH_TEXT_TAG_TEST_6"},
{GX_STRING_ID_RICH_TEXT_TAG_TEST_7, "GX_STRING_ID_RICH_TEXT_TAG_TEST_7"},
{GX_STRING_ID_RICH_TEXT_TAG_TEST_8, "GX_STRING_ID_RICH_TEXT_TAG_TEST_8"},
{GX_STRING_ID_RICH_TEXT_TAG_TEST_9, "GX_STRING_ID_RICH_TEXT_TAG_TEST_9"},
{GX_STRING_ID_RICH_TEXT_TAG_TEST_10, "GX_STRING_ID_RICH_TEXT_TAG_TEST_10"},
{GX_STRING_ID_RICH_TEXT_TAG_TEST_11, "GX_STRING_ID_RICH_TEXT_TAG_TEST_11"},
{GX_STRING_ID_RICH_TEXT_TAG_TEST_12, "GX_STRING_ID_RICH_TEXT_TAG_TEST_12"},
{GX_STRING_ID_RICH_TEXT_TAG_TEST_13, "GX_STRING_ID_RICH_TEXT_TAG_TEST_13"},
{GX_STRING_ID_RICH_TEXT_TAG_TEST_14, "GX_STRING_ID_RICH_TEXT_TAG_TEST_14"},
{GX_STRING_ID_RICH_TEXT_TAG_TEST_15, "GX_STRING_ID_RICH_TEXT_TAG_TEST_15"},
{GX_STRING_ID_RICH_TEXT_TAG_TEST_16, "GX_STRING_ID_RICH_TEXT_TAG_TEST_16"},
{GX_STRING_ID_RICH_TEXT_TAG_TEST_17, "GX_STRING_ID_RICH_TEXT_TAG_TEST_17"},
{GX_STRING_ID_RICH_TEXT_TAG_TEST_18, "GX_STRING_ID_RICH_TEXT_TAG_TEST_18"},
{GX_STRING_ID_RICH_TEXT_TAG_TEST_19, "GX_STRING_ID_RICH_TEXT_TAG_TEST_19"},
{GX_STRING_ID_RICH_TEXT_TAG_TEST_20, "GX_STRING_ID_RICH_TEXT_TAG_TEST_2-"},
{GX_STRING_ID_RICH_TEXT_LINE_BREAK_TEST_1, "GX_STRING_ID_RICH_TEXT_LINE_BREAK_TEST_1"},
{GX_STRING_ID_RICH_TEXT_LINE_BREAK_TEST_2, "GX_STRING_ID_RICH_TEXT_LINE_BREAK_TEST_2"},
{GX_STRING_ID_RICH_TEXT_UTF8_TEST, "GX_STRING_ID_RICH_TEXT_UTF8_TEST"},
{0, ""}
};

typedef struct align_info_struct{
GX_RESOURCE_ID style;
char *comment;
}align_info;

align_info align_info_list[]=
{
{GX_STYLE_TEXT_LEFT, "left alignment"},
{GX_STYLE_TEXT_CENTER, "center alignment"},
{GX_STYLE_TEXT_RIGHT, "right alignment"}
};

char comment[256];

static VOID control_thread_entry(ULONG input)
{
INT  frame_id = 1;
UINT alignment;
GX_RICH_TEXT_VIEW *rich_view = &main_screen.main_screen_rich_text_view;
GX_STRING string;
GX_RICH_TEXT_FONTS fonts;
TEXT_INFO *info = text_info_list;

    while(info->text_id)
    {
        gx_multi_line_text_view_text_id_set((GX_MULTI_LINE_TEXT_VIEW *)rich_view, info->text_id);

        if(info->text_id == GX_STRING_ID_RICH_TEXT_UTF8_TEST)
        {
            /* Set fonts to test utf8 string. */
            fonts.gx_rich_text_fonts_normal_id = GX_FONT_ID_CJK;
            fonts.gx_rich_text_fonts_bold_id = GX_FONT_ID_CJK_BOLD;
            fonts.gx_rich_text_fonts_italic_id = GX_FONT_ID_CJK_ITALIC;
            fonts.gx_rich_text_fonts_bold_italic_id = GX_FONT_ID_CJK_BOLD_ITALIC;
            gx_rich_text_view_fonts_set(rich_view, &fonts);
        }

        for(alignment = 0; alignment < sizeof(align_info_list)/sizeof(align_info); alignment++)
        {
            gx_widget_style_remove(rich_view, GX_STYLE_TEXT_ALIGNMENT_MASK);
            gx_widget_style_add(rich_view, align_info_list[alignment].style);

            gx_widget_string_get_ext(rich_view, info->text_id, &string);
            sprintf(comment, "%s, test string id: %s", align_info_list[alignment].comment, info->id_name);
            gx_validation_set_frame_id(frame_id++);
            gx_validation_set_frame_comment(comment);
            gx_validation_screen_refresh();
        }

        info++;
    }

    gx_multi_line_text_view_text_id_set((GX_MULTI_LINE_TEXT_VIEW *)rich_view, 0);
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("set string id to 0");
    gx_validation_screen_refresh();

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
