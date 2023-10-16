/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_widget.h"
#include "gx_system.h"

TEST_PARAM test_parameter = {
    "guix_widget_no_output", /* Test name */
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


static VOID control_thread_entry(ULONG input)
{
INT        failed_tests = 0;
UINT       status;
INT        index;
GX_COLOR   color;
GX_FONT   *font;
GX_WIDGET *widget;
GX_WIDGET  parent;
GX_EVENT   my_event;

    memset(&my_event, 0, sizeof(GX_EVENT));

    /* Test _gx_widget_client_index_get. */
    index = _gx_widget_client_index_get((GX_WIDGET *)&button_screen.button_screen_base_button_1, (GX_WIDGET *)&button_screen.button_screen_text_button_1);
    EXPECT_EQ(0, index);

    /* Test _gx_widget_last_client_child_get. */
    widget = _gx_widget_last_client_child_get((GX_WIDGET *)&button_screen.button_screen_base_button_1);
    EXPECT_EQ(GX_NULL, widget);

    /* Test _gx_widget_clipping_update. */
    _gx_widget_clipping_update(GX_NULL);

    /* Test _gx_widget_event_process. */
    widget = (GX_WIDGET *)&button_screen.button_screen_base_button_1;

    for(index = 0; index < 2; index++)
    {
        if(index == 0)
        {
            gx_widget_style_remove(widget, GX_STYLE_ENABLED);
        }
        else
        {
            gx_widget_style_add(widget, GX_STYLE_ENABLED);
        }
    
        gx_widget_style_remove(widget, GX_STYLE_BUTTON_REPEAT);
        my_event.gx_event_target = widget;
        my_event.gx_event_type = GX_EVENT_KEY_DOWN;
        my_event.gx_event_payload.gx_event_ushortdata[0] = GX_KEY_SELECT;
        gx_system_event_send(&my_event);

        my_event.gx_event_type = GX_EVENT_DESELECT;
        gx_system_event_send(&my_event);
    }
    gx_widget_status_add(widget, GX_STATUS_NOTIFY_ON_GAIN_FOCUS);
    gx_system_focus_claim((GX_WIDGET *)&button_screen.button_screen_text_button_1);
    gx_system_focus_claim(widget);

    EXPECT_EQ(1, button_focus_gain_count);
    EXPECT_EQ(1, button_click_count);

    /* Test _gx_widget_color_get. */
    gx_widget_detach(widget);
    widget->gx_widget_status |= GX_STATUS_VISIBLE;
    status = gx_widget_color_get(widget, 0, &color);
    EXPECT_EQ(GX_INVALID_CANVAS, status);

    status = gx_widget_font_get(widget, 0, &font);
    EXPECT_EQ(GX_INVALID_CANVAS, status);

    /* Test _gx_widget_find. */
    status = _gx_widget_find(GX_NULL, 0, 0, &widget);
    EXPECT_EQ(GX_NOT_FOUND, status);

    /* Test _gx_widget_focus_next. */
    status = _gx_widget_focus_next(GX_NULL);
    EXPECT_EQ(GX_FAILURE, status);

    /* Test _gx_widget_focus_previous. */
    status = _gx_widget_focus_previous(GX_NULL);
    EXPECT_EQ(GX_FAILURE, status);

    /* Test _gx_widget_front_move. */
    ToggleScreen((GX_WINDOW *)&window_screen, (GX_WINDOW *)&button_screen);
    widget = (GX_WIDGET *)&window_screen.window_screen_scroll_frame_1;
    gx_widget_front_move(widget, GX_NULL);
    status = widget->gx_widget_status & GX_STATUS_HAS_FOCUS;
    EXPECT_EQ(0, status);

    gx_widget_show(&button_screen);
    gx_system_focus_claim(&button_screen);
    widget = (GX_WIDGET *)&window_screen.window_screen_window_5;
    gx_widget_style_add(widget, GX_STYLE_ENABLED);
    gx_widget_status_add(widget, GX_STATUS_ACCEPTS_FOCUS);
    gx_widget_front_move(widget, GX_NULL);
    status = widget->gx_widget_status & GX_STATUS_HAS_FOCUS;
    EXPECT_EQ(0, status);

    /* Test _gx_widget_text_draw. */
    gx_widget_text_draw(GX_NULL, 0, 0, GX_NULL, 0, 0);
    gx_widget_text_draw_ext(GX_NULL, 0, 0, GX_NULL, 0, 0);

#ifndef GX_DISABLE_ERROR_CHECKING

    parent.gx_widget_type = 0;

    /* Test gx_widget_first_child_get. */
    status = gx_widget_first_child_get(GX_NULL, &widget);
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_widget_first_child_get((GX_WIDGET *)&button_screen, GX_NULL);
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_widget_first_child_get((GX_WIDGET *)&parent, &widget);
    EXPECT_EQ(GX_INVALID_WIDGET, status);


    /* Test gx_widget_last_child_get. */
    status = gx_widget_last_child_get(GX_NULL, &widget);
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_widget_last_child_get((GX_WIDGET *)&button_screen, GX_NULL);
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_widget_last_child_get((GX_WIDGET *)&parent, &widget);
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    /* Test gx_widget_next_sibling_get. */
    status = gx_widget_next_sibling_get(GX_NULL, &widget);
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_widget_next_sibling_get((GX_WIDGET *)&button_screen, GX_NULL);
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_widget_next_sibling_get((GX_WIDGET *)&parent, &widget);
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    /* Test gx_widget_parent_get. */
    status = gx_widget_parent_get(GX_NULL, &widget);
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_widget_parent_get((GX_WIDGET *)&button_screen, GX_NULL);
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_widget_parent_get((GX_WIDGET *)&parent, &widget);
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    /* Test gx_widget_previous_sibling_get. */
    status = gx_widget_previous_sibling_get(GX_NULL, &widget);
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_widget_previous_sibling_get((GX_WIDGET *)&button_screen, GX_NULL);
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_widget_previous_sibling_get((GX_WIDGET *)&parent, &widget);
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    /* Test gx_widget_top_visible_child_find. */
    status = gx_widget_top_visible_child_find(GX_NULL, &widget);
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_widget_top_visible_child_find((GX_WIDGET *)&button_screen, GX_NULL);
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_widget_top_visible_child_find(&parent, &widget);
    EXPECT_EQ(GX_INVALID_WIDGET, status);
#endif

    status =  gx_widget_top_visible_child_find((GX_WIDGET *)&button_screen, &widget);
    EXPECT_EQ(GX_SUCCESS, status);

    gx_widget_detach(&window_screen.window_screen_window_4);
    widget = _gx_widget_first_visible_client_child_get(&window_screen.window_screen_scroll_frame_1);
    EXPECT_EQ(widget, GX_NULL);

    widget = _gx_widget_last_visible_client_child_get(&window_screen.window_screen_scroll_frame_1);
    EXPECT_EQ(widget, GX_NULL);

    widget = window_screen.window_screen_scroll_frame_1.gx_widget_first_child;
    widget = _gx_widget_next_visible_client_child_get(widget);
    EXPECT_EQ(widget, GX_NULL);

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
