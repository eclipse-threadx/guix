/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_widget.h"
#include "gx_system.h"
#include "gx_rich_text_view.h"

TEST_PARAM test_parameter = {
    "guix_rich_text_view_no_output", /* Test name */
    0, 0, 0, 0  /* Define the coordinates of the capture area.*/
};

int main(int argc, char ** argv)
{
    /* Start ThreadX system */
    tx_kernel_enter(); 
    return(0);
}

static VOID      control_thread_entry(ULONG);

#if defined (GX_DISABLE_ERROR_CHECKING)
VOID tx_application_define(void *first_unused_memory)
{
    exit(0);
}
#else
VOID tx_application_define(void *first_unused_memory)
{
    gx_validation_application_define(first_unused_memory);
    
    /* Termiante the test if it runs for more than 100 ticks */
    /* This function is not implemented yet. */
    gx_validation_watchdog_create(100);

    /* Create a dedicated thread to perform various operations
       on the pixelmap drawing example. These operations simulate 
       user input. */
    gx_validation_control_thread_create(control_thread_entry);
}
#endif

#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"
#include "demo_guix_rich_text_view_16bpp.c"

extern volatile ULONG       _tx_thread_system_state;
extern volatile TX_THREAD  *_tx_thread_current_ptr;
extern TX_THREAD            _tx_timer_thread;


static VOID control_thread_entry(ULONG input)
{
INT                  failed_tests = 0;
UINT                 status;
GX_RICH_TEXT_VIEW    rich_view;
GX_RECTANGLE         size;
GX_RICH_TEXT_FONTS   fonts;
GX_STRING            string;
GX_RICH_TEXT_CONTEXT context;
GX_RICH_TEXT_LINE_INFO line_info;

    /* Initialize variables. */
    memset(&rich_view, 0, sizeof(GX_RICH_TEXT_VIEW));
    memset(&fonts, 0, sizeof(GX_RICH_TEXT_FONTS));
    gx_utility_rectangle_define(&size, 0, 0, 100, 100);

    /* gx_rich_text_view_create */
    status = gx_rich_text_view_create(GX_NULL, "", GX_NULL, 0, &fonts, 0, 0, &size);
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_rich_text_view_create(&rich_view, "", GX_NULL, 0, &fonts, 0, 0, GX_NULL);
    EXPECT_EQ(GX_PTR_ERROR, status);
 
    status = gx_rich_text_view_create(&rich_view, "", GX_NULL, 0, GX_NULL, 0, 0, &size);
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = _gxe_rich_text_view_create(&rich_view, "", GX_NULL, 0, &fonts, 0, 0, &size, 0);
    EXPECT_EQ(GX_INVALID_SIZE, status);

    status = gx_rich_text_view_create(&rich_view, "", GX_NULL, 0, &fonts, 0, 0, &size);
    EXPECT_EQ(GX_SUCCESS, status);

    status = gx_rich_text_view_create(&rich_view, "", GX_NULL, 0, &fonts, 0, 0, &size);
    EXPECT_EQ(GX_ALREADY_CREATED, status);

    /* gx_rich_text_view_fonts_set */
    status = gx_rich_text_view_fonts_set(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_rich_text_view_fonts_set(&rich_view, GX_NULL);
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_rich_text_view_fonts_set(&rich_view, &fonts);
    EXPECT_EQ(GX_SUCCESS, status);

    /* _gx_rich_text_view_context_peek. */
    _gx_rich_text_view_context_push(&context);
    status = _gx_rich_text_view_context_peek(GX_NULL);
    EXPECT_EQ(GX_FAILURE, status);

    status = _gx_rich_text_view_context_pop();
    EXPECT_EQ(GX_SUCCESS, status);

    status = _gx_rich_text_view_context_pop();
    EXPECT_EQ(GX_FAILURE, status);

    string.gx_string_ptr = "<";
    string.gx_string_length = 1;
    status = _gx_rich_text_view_tag_enter(GX_NULL, &string, GX_NULL, GX_NULL);
    EXPECT_EQ(GX_FAILURE, status);

    string.gx_string_ptr = "1";
    status = _gx_rich_text_view_tag_enter(GX_NULL, &string, GX_NULL, GX_NULL);
    EXPECT_EQ(GX_FAILURE, status);

    line_info.gx_rich_text_line_info_start_format.gx_rich_text_font_id = 100;
    status = _gx_rich_text_view_line_info_get(&main_screen.main_screen_rich_text_view, string,
                                              &line_info, 100);
    EXPECT_EQ(GX_INVALID_FONT, status);

    _tx_thread_current_ptr = &_tx_timer_thread;

    /* gx_rich_text_view_create */
    _tx_thread_system_state = 1;
    status = gx_rich_text_view_create(GX_NULL, GX_NULL, GX_NULL, 0, GX_NULL, 0, 0, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = -1;
    status = gx_rich_text_view_create(GX_NULL, GX_NULL, GX_NULL, 0, GX_NULL, 0, 0, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    
    /* gx_rich_text_view_fonts_set */
    _tx_thread_system_state = 1;
    status = gx_rich_text_view_fonts_set(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = -1;
    status = gx_rich_text_view_fonts_set(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    if(failed_tests == 0)
    {
        gx_validation_print_test_result(TEST_SUCCESS);
        exit(0);
    }
    else 
    {
        gx_validation_print_test_result(TEST_FAIL);
        exit(1);
    }
}
