/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_system.h"

TEST_PARAM test_parameter = {
    "guix_system_no_output", /* Test name */
    0, 0, 0, 0  /* Define the coordinates of the capture area.*/
};

int main(int argc, char ** argv)
{
    /* Start ThreadX system */
    tx_kernel_enter(); 
    return(0);
}

static VOID      control_thread_entry(ULONG);

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

#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"
#include "demo_guix_all_widgets.c"

#define TEST_STRING_SIZE 0xccd
char test_string[TEST_STRING_SIZE + 1];

int flick_count = 0;
UINT test_event_handler(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    switch(event_ptr->gx_event_type)
    {
    case GX_EVENT_VERTICAL_FLICK:
        flick_count++;
        break;
    }
    return gx_window_event_process(window, event_ptr);
}

static VOID control_thread_entry(ULONG input)
{
UINT                     status;
GX_EVENT                 my_event;
GX_PEN_CONFIGURATION     configure;
int                      index;
GX_MULTI_LINE_TEXT_VIEW  view;
GX_FONT                 *font;
GX_VALUE                 width;
GX_STRING                string;
int                      failed_tests = 0;

    memset(&my_event, 0, sizeof(GX_EVENT));

    /* Test gx_system_input_release. */
    _gx_system_input_capture((GX_WIDGET *)&button_screen);
    status = _gx_system_input_release((GX_WIDGET *)&window_screen);
    EXPECT_EQ(GX_NO_CHANGE, status);
    window_screen.gx_widget_status |= GX_STATUS_OWNS_INPUT;
    status = _gx_system_input_release((GX_WIDGET *)&window_screen);
    EXPECT_EQ(GX_PTR_ERROR, status);

    /* Test _gx_system_pen_flick_test. */
    configure.gx_pen_configuration_min_drag_dist = GX_FIXED_VAL_MAKE(10);
    configure.gx_pen_configuration_max_pen_speed_ticks = 10;
    gx_system_pen_configure(&configure);

    gx_widget_event_process_set(&button_screen, test_event_handler);
    gx_widget_style_add(&button_screen, GX_STYLE_ENABLED);

    for(index = 0; index < 2; index++)
    {
        my_event.gx_event_display_handle = 1;
        my_event.gx_event_type = GX_EVENT_PEN_DOWN;
        my_event.gx_event_payload.gx_event_pointdata.gx_point_x = 80;
        my_event.gx_event_payload.gx_event_pointdata.gx_point_y = 10;
        gx_system_event_send(&my_event);
       
        tx_thread_sleep(2); 
        my_event.gx_event_type = GX_EVENT_PEN_DRAG;
        if(index == 0)
        {
            my_event.gx_event_payload.gx_event_pointdata.gx_point_y += 40;
        }
        else
        {
            my_event.gx_event_payload.gx_event_pointdata.gx_point_y += 9;
        }

        gx_system_event_send(&my_event);

        my_event.gx_event_type = GX_EVENT_PEN_UP;
        gx_system_event_send(&my_event);
        EXPECT_EQ(1, flick_count);
    }

    /* Test _gx_system_private_string_delete. */
    memset(&view, 0, sizeof(GX_MULTI_LINE_TEXT_VIEW));
    gx_multi_line_text_view_create(&view, "test_view", &text_screen, 0, GX_STYLE_TEXT_COPY, 0, &text_screen.gx_widget_size);
    
    gx_multi_line_text_view_text_set(&view, GX_NULL);
    gx_multi_line_text_view_text_set(&view, "test_string");
    view.gx_multi_line_text_view_text_id = 1;
    gx_widget_delete((GX_WIDGET *)&view);
    if(view.gx_multi_line_text_view_text.gx_string_ptr == GX_NULL)
    {
        failed_tests++;
        PRINT_ERROR("");

    }

    gx_multi_line_text_view_create(&view, "text_view", &text_screen, 0, GX_STYLE_TEXT_COPY, 0, &text_screen.gx_widget_size);
    gx_widget_delete((GX_WIDGET *)&view);
    EXPECT_EQ(GX_NULL, view.gx_multi_line_text_view_text.gx_string_ptr);

    gx_system_memory_allocator_set(GX_NULL, GX_NULL);
    
    /* Test _gx_system_dirty_mark. */
    status = _gx_system_dirty_mark(GX_NULL);
    EXPECT_EQ(GX_PTR_ERROR, status);

    /* Test _gx_system_dirty_partial_add. */
    status = _gx_system_dirty_partial_add(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_PTR_ERROR, status);

    /* Test _gx_system_dirty_entry_shift. */
    _gx_system_dirty_entry_shift(GX_NULL, 0, 0);
    gx_widget_status_add(&window_screen, GX_STATUS_VISIBLE | GX_STATUS_DIRTY);
    _gx_system_dirty_entry_shift((GX_WIDGET *)&window_screen, 10, 10);
    EXPECT_EQ(0, _gx_system_lock_nesting);

    /* Test gx_system_string_width_get. */
    memset(test_string, 'A', TEST_STRING_SIZE);
    test_string[TEST_STRING_SIZE] = '\0';
    gx_widget_font_get(&button_screen, GX_FONT_ID_SYSTEM, &font);

    string.gx_string_ptr = test_string;
    string.gx_string_length = TEST_STRING_SIZE;
    gx_system_string_width_get_ext(font, &string, &width);
    EXPECT_EQ(0x7fff, width);

    if(!failed_tests)
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
