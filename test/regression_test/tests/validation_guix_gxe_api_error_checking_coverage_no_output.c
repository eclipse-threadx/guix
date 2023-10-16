/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_animation.h"
#include "gx_canvas.h"
#include "gx_system.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_gxe_api_error_checking_coverage_no_output", /* Test name */
    0, 0, 0, 0  /* Define the coordinates of the capture area.*/
};

static UINT failed_tests = 0;
extern volatile ULONG      _tx_thread_system_state;
extern volatile TX_THREAD  *_tx_thread_current_ptr;
extern TX_THREAD            _tx_timer_thread;

int main(int argc, char ** argv)
{
    /* Start ThreadX system */
    tx_kernel_enter(); 
    return(0);
}

#if defined (GX_DISABLE_ERROR_CHECKING)
VOID tx_application_define(void *first_unused_memory)
{
    exit(0);
}
#else
VOID tx_application_define(void *first_unused_memory)
{
UINT status;
GX_POINT pos;

    _tx_thread_current_ptr = &_tx_timer_thread;

    _tx_thread_system_state = 1;
    status = gx_accordion_menu_create(GX_NULL, GX_NULL, GX_NULL, 0, 0, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_accordion_menu_create(GX_NULL, GX_NULL, GX_NULL, 0, 0, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_accordion_menu_event_process(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_accordion_menu_event_process(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_accordion_menu_position(GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_accordion_menu_position(GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_animation_drag_disable(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_animation_drag_disable(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_animation_stop(GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_animation_stop(GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_button_create(GX_NULL, GX_NULL, GX_NULL, 0, 0, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_button_create(GX_NULL, GX_NULL, GX_NULL, 0, 0, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_button_deselect(GX_NULL, GX_TRUE);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_button_deselect(GX_NULL, GX_TRUE);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_button_event_process(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_button_event_process(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_canvas_alpha_set(GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_canvas_alpha_set(GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_canvas_arc_draw(0, 0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_canvas_arc_draw(0, 0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_canvas_block_move(GX_NULL, 0, 0, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_canvas_block_move(GX_NULL, 0, 0, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_canvas_circle_draw(0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_canvas_circle_draw(0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_canvas_create(GX_NULL, GX_NULL, GX_NULL, 0, 0, 0, GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_canvas_create(GX_NULL, GX_NULL, GX_NULL, 0, 0, 0, GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_canvas_delete(GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_canvas_delete(GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_canvas_drawing_complete(GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_canvas_drawing_complete(GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_canvas_drawing_initiate(GX_NULL, GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_canvas_drawing_initiate(GX_NULL, GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status =  gx_canvas_ellipse_draw(0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status =  gx_canvas_ellipse_draw(0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_canvas_hardware_layer_bind(GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_canvas_hardware_layer_bind(GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_canvas_hide(GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status =  gx_canvas_hide(GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_canvas_line_draw(0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status =  gx_canvas_line_draw(0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    /* gx_canvas_memory_define. */
    _tx_thread_system_state = 1;
    status = gx_canvas_memory_define(GX_NULL, GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_canvas_memory_define(GX_NULL, GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

#if defined (GX_MOUSE_SUPPORT)
    _tx_thread_system_state = 1;
    status = gx_canvas_mouse_define(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_canvas_mouse_define(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_canvas_mouse_hide(GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_canvas_mouse_hide(GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_canvas_mouse_show(GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_canvas_mouse_show(GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
#endif

    _tx_thread_system_state = 1;
    status = gx_canvas_offset_set(GX_NULL, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_canvas_offset_set(GX_NULL, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_canvas_pie_draw(0, 0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_canvas_pie_draw(0, 0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_canvas_pixel_draw(pos);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_canvas_pixel_draw(pos);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_canvas_pixelmap_blend(0, 0, GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_canvas_pixelmap_blend(0, 0, GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_canvas_pixelmap_draw(0, 0, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_canvas_pixelmap_draw(0, 0, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_canvas_pixelmap_rotate(0, 0, GX_NULL, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_canvas_pixelmap_rotate(0, 0, GX_NULL, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_canvas_pixelmap_tile(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_canvas_pixelmap_tile(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_canvas_polygon_draw(GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_canvas_polygon_draw(GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_canvas_rectangle_draw(GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_canvas_rectangle_draw(GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

#ifdef GX_ENABLE_DEPRECATED_STRING_API
    _tx_thread_system_state = 1;
    status = gx_canvas_rotated_text_draw(GX_NULL, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_canvas_rotated_text_draw(GX_NULL, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
#endif
    _tx_thread_system_state = 1;
    status = gx_canvas_rotated_text_draw_ext(GX_NULL, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_canvas_rotated_text_draw_ext(GX_NULL, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_canvas_shift(GX_NULL, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_canvas_shift(GX_NULL, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_canvas_show(GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_canvas_show(GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

#ifdef GX_ENABLE_DEPRECATED_STRING_API
    _tx_thread_system_state = 1;
    status = gx_canvas_text_draw(0, 0, GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_canvas_text_draw(0, 0, GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
#endif
    _tx_thread_system_state = 1;
    status = gx_canvas_text_draw_ext(0, 0, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_canvas_text_draw_ext(0, 0, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_checkbox_create(GX_NULL, GX_NULL, GX_NULL, 0, 0, 0, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_checkbox_create(GX_NULL, GX_NULL, GX_NULL, 0, 0, 0, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_checkbox_event_process(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_checkbox_event_process(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_checkbox_select(GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_checkbox_select(GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_circular_gauge_angle_set(GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_circular_gauge_angle_set(GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_circular_gauge_animation_set(GX_NULL, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_circular_gauge_animation_set(GX_NULL, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_circular_gauge_create(GX_NULL, GX_NULL, GX_NULL, GX_NULL, 0, 0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_circular_gauge_create(GX_NULL, GX_NULL, GX_NULL, GX_NULL, 0, 0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_circular_gauge_event_process(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_circular_gauge_event_process(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

#ifdef GX_ENABLE_DEPRECATED_STRING_API
    _tx_thread_system_state = 1;
    status = gx_context_string_get(0, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_context_string_get(0, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
#endif
    _tx_thread_system_state = 1;
    status = gx_context_string_get_ext(0, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_context_string_get_ext(0, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_display_active_language_set(GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_display_active_language_set(GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_display_color_set(GX_NULL, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_display_color_set(GX_NULL, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_display_color_table_set(GX_NULL, GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_display_color_table_set(GX_NULL, GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_display_create(GX_NULL, GX_NULL, GX_NULL, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_display_create(GX_NULL, GX_NULL, GX_NULL, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_display_delete(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_display_delete(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_display_font_table_set(GX_NULL, GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_display_font_table_set(GX_NULL, GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    
#ifdef GX_ENABLE_DEPRECATED_STRING_API
    _tx_thread_system_state = 1;
    status = gx_display_language_table_get(GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_display_language_table_get(GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_display_language_table_set(GX_NULL, GX_NULL, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_display_language_table_set(GX_NULL, GX_NULL, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
#endif
     _tx_thread_system_state = 1;
    status = gx_display_language_table_get_ext(GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_display_language_table_get_ext(GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_display_language_table_set_ext(GX_NULL, GX_NULL, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_display_language_table_set_ext(GX_NULL, GX_NULL, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_display_pixelmap_table_set(GX_NULL, GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_display_pixelmap_table_set(GX_NULL, GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

#ifdef GX_ENABLE_DEPRECATED_STRING_API
    _tx_thread_system_state = 1;
    status = gx_display_string_get(GX_NULL, 0, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_display_string_get(GX_NULL, 0, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_display_string_table_get(GX_NULL, 0, GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_display_string_table_get(GX_NULL, 0, GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
#endif
     _tx_thread_system_state = 1;
    status = gx_display_string_get_ext(GX_NULL, 0, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_display_string_get_ext(GX_NULL, 0, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_display_string_table_get_ext(GX_NULL, 0, GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_display_string_table_get_ext(GX_NULL, 0, GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_display_theme_install(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_display_theme_install(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_drop_list_close(GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_drop_list_close(GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_drop_list_create(GX_NULL, GX_NULL, GX_NULL, 0, 0, GX_NULL, 0, 0, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_drop_list_create(GX_NULL, GX_NULL, GX_NULL, 0, 0, GX_NULL, 0, 0, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_drop_list_event_process(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_drop_list_event_process(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_drop_list_open(GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_drop_list_open(GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_drop_list_pixelmap_set(GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_drop_list_pixelmap_set(GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_horizontal_list_children_position(GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_horizontal_list_children_position(GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_horizontal_list_create(GX_NULL, GX_NULL, GX_NULL, 0, GX_NULL, 0, 0, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_horizontal_list_create(GX_NULL, GX_NULL, GX_NULL, 0, GX_NULL, 0, 0, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_horizontal_list_event_process(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_horizontal_list_event_process(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_horizontal_list_page_index_set(GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_horizontal_list_page_index_set(GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_horizontal_list_selected_set(GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_horizontal_list_selected_set(GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_horizontal_list_total_columns_set(GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_horizontal_list_total_columns_set(GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_horizontal_scrollbar_create(GX_NULL, GX_NULL, GX_NULL, GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_horizontal_scrollbar_create(GX_NULL, GX_NULL, GX_NULL, GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_scrollbar_value_set(GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_scrollbar_value_set(GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_icon_button_create(GX_NULL, GX_NULL, GX_NULL, 0, 0, 0, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_icon_button_create(GX_NULL, GX_NULL, GX_NULL, 0, 0, 0, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_icon_button_pixelmap_set(GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_icon_button_pixelmap_set(GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_icon_create(GX_NULL, GX_NULL, GX_NULL, 0, 0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_icon_create(GX_NULL, GX_NULL, GX_NULL, 0, 0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_icon_event_process(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_icon_event_process(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_icon_pixelmap_set(GX_NULL, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_icon_pixelmap_set(GX_NULL, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_image_reader_start(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_image_reader_start(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_line_chart_create(GX_NULL, GX_NULL, GX_NULL, GX_NULL, 0, 0, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_line_chart_create(GX_NULL, GX_NULL, GX_NULL, GX_NULL, 0, 0, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_line_chart_update(GX_NULL, GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_line_chart_update(GX_NULL, GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_menu_create(GX_NULL, GX_NULL, GX_NULL, 0, 0, 0, 0, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_menu_create(GX_NULL, GX_NULL, GX_NULL, 0, 0, 0, 0, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_menu_insert(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_menu_insert(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_menu_remove(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_menu_remove(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_menu_text_offset_set(GX_NULL, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_menu_text_offset_set(GX_NULL, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_menu_event_process(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_menu_event_process(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_multi_line_text_button_create(GX_NULL, GX_NULL, GX_NULL, 0, 0, 0, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_multi_line_text_button_create(GX_NULL, GX_NULL, GX_NULL, 0, 0, 0, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_multi_line_text_button_event_process(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_multi_line_text_button_event_process(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_multi_line_text_button_text_id_set(GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_multi_line_text_button_text_id_set(GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

#ifdef GX_ENABLE_DEPRECATED_STRING_API
    _tx_thread_system_state = 1;
    status = gx_multi_line_text_button_text_set(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_multi_line_text_button_text_set(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
#endif
    _tx_thread_system_state = 1;
    status = gx_multi_line_text_button_text_set_ext(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_multi_line_text_button_text_set_ext(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_multi_line_text_input_backspace(GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_multi_line_text_input_backspace(GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_multi_line_text_input_buffer_clear(GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_multi_line_text_input_buffer_clear(GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

#ifdef GX_ENABLE_DEPRECATED_STRING_API
    _tx_thread_system_state = 1;
    status = gx_multi_line_text_input_char_insert(GX_NULL, GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_multi_line_text_input_char_insert(GX_NULL, GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
#endif
    _tx_thread_system_state = 1;
    status = gx_multi_line_text_input_char_insert_ext(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_multi_line_text_input_char_insert_ext(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_multi_line_text_input_create(GX_NULL, GX_NULL, GX_NULL, GX_NULL, 0, 0, 0, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_multi_line_text_input_create(GX_NULL, GX_NULL, GX_NULL, GX_NULL, 0, 0, 0, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_multi_line_text_input_delete(GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_multi_line_text_input_delete(GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_multi_line_text_input_down_arrow(GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_multi_line_text_input_down_arrow(GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_multi_line_text_input_end(GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_multi_line_text_input_end(GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_multi_line_text_input_event_process(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_multi_line_text_input_event_process(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_multi_line_text_input_home(GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_multi_line_text_input_home(GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_multi_line_text_input_left_arrow(GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_multi_line_text_input_left_arrow(GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_multi_line_text_input_right_arrow(GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_multi_line_text_input_right_arrow(GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_multi_line_text_input_style_add(GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_multi_line_text_input_style_add(GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_multi_line_text_input_style_remove(GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_multi_line_text_input_style_remove(GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_multi_line_text_input_style_set(GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_multi_line_text_input_style_set(GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_multi_line_text_input_fill_color_set(GX_NULL, 0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_multi_line_text_input_fill_color_set(GX_NULL, 0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_multi_line_text_input_text_color_set(GX_NULL, 0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_multi_line_text_input_text_color_set(GX_NULL, 0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_multi_line_text_input_text_select(GX_NULL, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_multi_line_text_input_text_select(GX_NULL, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_multi_line_text_input_up_arrow(GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_multi_line_text_input_up_arrow(GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_multi_line_text_view_create(GX_NULL, GX_NULL, GX_NULL, 0, 0, 0, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_multi_line_text_view_create(GX_NULL, GX_NULL, GX_NULL, 0, 0, 0, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_multi_line_text_view_font_set(GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_multi_line_text_view_font_set(GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_multi_line_text_view_line_space_set(GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_multi_line_text_view_line_space_set(GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_multi_line_text_view_text_color_set(GX_NULL, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_multi_line_text_view_text_color_set(GX_NULL, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_multi_line_text_view_text_id_set(GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_multi_line_text_view_text_id_set(GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

#ifdef GX_ENABLE_DEPRECATED_STRING_API
    _tx_thread_system_state = 1;
    status = gx_multi_line_text_view_text_set(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_multi_line_text_view_text_set(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
#endif
   _tx_thread_system_state = 1;
    status = gx_multi_line_text_view_text_set_ext(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_multi_line_text_view_text_set_ext(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);


    _tx_thread_system_state = 1;
    status = gx_numeric_pixelmap_prompt_create(GX_NULL, GX_NULL, GX_NULL, 0, 0, 0, 0, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_numeric_pixelmap_prompt_create(GX_NULL, GX_NULL, GX_NULL, 0, 0, 0, 0, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_numeric_pixelmap_prompt_value_set(GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_numeric_pixelmap_prompt_value_set(GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_numeric_prompt_create(GX_NULL, GX_NULL, GX_NULL, 0, 0, 0, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_numeric_prompt_create(GX_NULL, GX_NULL, GX_NULL, 0, 0, 0, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_numeric_prompt_value_set(GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_numeric_prompt_value_set(GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_numeric_scroll_wheel_create(GX_NULL, GX_NULL, GX_NULL, 0, 0, 0, 0, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_numeric_scroll_wheel_create(GX_NULL, GX_NULL, GX_NULL, 0, 0, 0, 0, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_numeric_scroll_wheel_range_set(GX_NULL, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_numeric_scroll_wheel_range_set(GX_NULL, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_text_scroll_wheel_event_process(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_text_scroll_wheel_event_process(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_pixelmap_button_create(GX_NULL, GX_NULL, GX_NULL, 0, 0, 0, 0, 0, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_pixelmap_button_create(GX_NULL, GX_NULL, GX_NULL, 0, 0, 0, 0, 0, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_pixelmap_button_event_process(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_pixelmap_button_event_process(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_pixelmap_button_pixelmap_set(GX_NULL, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_pixelmap_button_pixelmap_set(GX_NULL, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_pixelmap_prompt_create(GX_NULL, GX_NULL, GX_NULL, 0, 0, 0, 0, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_pixelmap_prompt_create(GX_NULL, GX_NULL, GX_NULL, 0, 0, 0, 0, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_pixelmap_prompt_pixelmap_set(GX_NULL , 0, 0, 0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_pixelmap_prompt_pixelmap_set(GX_NULL , 0, 0, 0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_pixelmap_slider_create(GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, 0, 0, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_pixelmap_slider_create(GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, 0, 0, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_pixelmap_slider_event_process(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_pixelmap_slider_event_process(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_pixelmap_slider_pixelmap_set(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_pixelmap_slider_pixelmap_set(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_progress_bar_create(GX_NULL, GX_NULL, GX_NULL, GX_NULL, 0, 0, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_progress_bar_create(GX_NULL, GX_NULL, GX_NULL, GX_NULL, 0, 0, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_progress_bar_event_process(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_progress_bar_event_process(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_progress_bar_font_set(GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_progress_bar_font_set(GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_progress_bar_info_set(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_progress_bar_info_set(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_progress_bar_pixelmap_set(GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_progress_bar_pixelmap_set(GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_progress_bar_range_set(GX_NULL, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_progress_bar_range_set(GX_NULL, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_progress_bar_text_color_set(GX_NULL, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_progress_bar_text_color_set(GX_NULL, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_progress_bar_value_set(GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_progress_bar_value_set(GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_prompt_create(GX_NULL, GX_NULL, GX_NULL, 0, 0, 0, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_prompt_create(GX_NULL, GX_NULL, GX_NULL, 0, 0, 0, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_prompt_font_set(GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_prompt_font_set(GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_prompt_text_color_set(GX_NULL, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_prompt_text_color_set(GX_NULL, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_prompt_text_id_set(GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_prompt_text_id_set(GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

#ifdef GX_ENABLE_DEPRECATED_STRING_API
    _tx_thread_system_state = 1;
    status = gx_prompt_text_set(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_prompt_text_set(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
#endif
    _tx_thread_system_state = 1;
    status = gx_prompt_text_set_ext(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_prompt_text_set_ext(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_prompt_event_process(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_prompt_event_process(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_radial_progress_bar_anchor_set(GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_radial_progress_bar_anchor_set(GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_radial_progress_bar_create(GX_NULL, GX_NULL, GX_NULL, GX_NULL, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_radial_progress_bar_create(GX_NULL, GX_NULL, GX_NULL, GX_NULL, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_radial_progress_bar_event_process(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_radial_progress_bar_event_process(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_radial_progress_bar_font_set(GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_radial_progress_bar_font_set(GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_radial_progress_bar_info_set(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_radial_progress_bar_font_set(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_radial_progress_bar_text_color_set(GX_NULL, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_radial_progress_bar_text_color_set(GX_NULL, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_radial_progress_bar_value_set(GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_radial_progress_bar_value_set(GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_radial_slider_angle_set(GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_radial_slider_angle_set(GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_radial_slider_animation_set(GX_NULL, 0, 0, 0, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_radial_slider_animation_set(GX_NULL, 0, 0, 0, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_radial_slider_animation_start(GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_radial_slider_animation_start(GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_radial_slider_create(GX_NULL, GX_NULL, GX_NULL, GX_NULL, 0, 0, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_radial_slider_create(GX_NULL, GX_NULL, GX_NULL, GX_NULL, 0, 0, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_radial_slider_info_set(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_radial_slider_info_set(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_radial_slider_pixelmap_set(GX_NULL, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_radial_slider_pixelmap_set(GX_NULL, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_radio_button_create(GX_NULL, GX_NULL, GX_NULL, 0, 0, 0, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_radio_button_create(GX_NULL, GX_NULL, GX_NULL, 0, 0, 0, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_screen_stack_pop(GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_screen_stack_pop(GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_screen_stack_push(GX_NULL, GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_screen_stack_push(GX_NULL, GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_scroll_thumb_create(GX_NULL, GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_scroll_thumb_create(GX_NULL, GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_scroll_thumb_event_process(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_scroll_thumb_event_process(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_scrollbar_event_process(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_scrollbar_event_process(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_scrollbar_limit_check(GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_scrollbar_limit_check(GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_scrollbar_reset(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_scrollbar_reset(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_scroll_wheel_create(GX_NULL , GX_NULL , GX_NULL , 0, 0, 0, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_scroll_wheel_create(GX_NULL , GX_NULL , GX_NULL , 0, 0, 0, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_scroll_wheel_event_process(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_scroll_wheel_event_process(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_scroll_wheel_gradient_alpha_set(GX_NULL, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_scroll_wheel_gradient_alpha_set(GX_NULL, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_scroll_wheel_row_height_set(GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_scroll_wheel_row_height_set(GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_scroll_wheel_selected_background_set(GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_scroll_wheel_selected_background_set(GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_scroll_wheel_selected_get(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_scroll_wheel_selected_get(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_scroll_wheel_selected_set(GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_scroll_wheel_selected_set(GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_scroll_wheel_speed_set(GX_NULL, 0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_scroll_wheel_speed_set(GX_NULL, 0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_scroll_wheel_total_rows_set(GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_scroll_wheel_total_rows_set(GX_NULL, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_single_line_text_input_backspace(GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_single_line_text_input_backspace(GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_single_line_text_input_buffer_clear(GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_single_line_text_input_buffer_clear(GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_single_line_text_input_buffer_get(0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_single_line_text_input_buffer_get(0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_single_line_text_input_character_delete(0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_single_line_text_input_character_delete(0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_single_line_text_input_character_insert(0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_single_line_text_input_character_insert(0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_single_line_text_input_create(0, 0, 0, 0, 0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_single_line_text_input_create(0, 0, 0, 0, 0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_single_line_text_input_end(0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_single_line_text_input_end(0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_single_line_text_input_event_process(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_single_line_text_input_event_process(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_single_line_text_input_home(0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_single_line_text_input_home(0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_single_line_text_input_left_arrow(0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_single_line_text_input_left_arrow(0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_single_line_text_input_position_get(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_single_line_text_input_position_get(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_single_line_text_input_right_arrow(0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_single_line_text_input_right_arrow(0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_single_line_text_input_style_add(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_single_line_text_input_style_add(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_single_line_text_input_style_remove(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_single_line_text_input_style_remove(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_single_line_text_input_style_set(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_single_line_text_input_style_set(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_single_line_text_input_fill_color_set(0, 0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_single_line_text_input_fill_color_set(0, 0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_single_line_text_input_text_color_set(0, 0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_single_line_text_input_text_color_set(0, 0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_single_line_text_input_text_select(0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_single_line_text_input_text_select(0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

#ifdef GX_ENABLE_DEPRECATED_STRING_API
    _tx_thread_system_state = 1;
    status = gx_single_line_text_input_text_set(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_single_line_text_input_text_set(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
#endif
    _tx_thread_system_state = 1;
    status = gx_single_line_text_input_text_set_ext(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_single_line_text_input_text_set_ext(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_slider_create(0, 0, 0, 0, 0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_slider_create(0, 0, 0, 0, 0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_slider_event_process(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_slider_event_process(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_slider_info_set(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_slider_info_set(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_slider_needle_position_get(0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_slider_needle_position_get(0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_slider_travel_get(0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_slider_travel_get(0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_slider_value_calculate(0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_slider_value_calculate(0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_slider_value_set(0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_slider_value_set(0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_sprite_create(0, 0, 0, 0, 0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_sprite_create(0, 0, 0, 0, 0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_sprite_current_frame_set(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_sprite_current_frame_set(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_sprite_frame_list_set(0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_sprite_frame_list_set(0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_sprite_start(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = 0;
    _tx_thread_current_ptr = TX_NULL;
    status = gx_sprite_start(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_current_ptr = &_tx_timer_thread;
    status = gx_sprite_start(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_sprite_stop(0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = 0;
    _tx_thread_current_ptr = TX_NULL;
    status = gx_sprite_stop(0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_current_ptr = &_tx_timer_thread;
    status = gx_sprite_stop(0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

#ifdef GX_ENABLE_DEPRECATED_STRING_API
    _tx_thread_system_state = 1;
    status = gx_string_scroll_wheel_create(0, 0, 0, 0, 0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_string_scroll_wheel_create(0, 0, 0, 0, 0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
#endif
    _tx_thread_system_state = 1;
    status = gx_string_scroll_wheel_create_ext(0, 0, 0, 0, 0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_string_scroll_wheel_create_ext(0, 0, 0, 0, 0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_string_scroll_wheel_string_id_list_set(0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_string_scroll_wheel_string_id_list_set(0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

#ifdef GX_ENABLE_DEPRECATED_STRING_API
    _tx_thread_system_state = 1;
    status = gx_string_scroll_wheel_string_list_set(0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_string_scroll_wheel_string_list_set(0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
#endif
    _tx_thread_system_state = 1;
    status = gx_string_scroll_wheel_string_list_set_ext(0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_string_scroll_wheel_string_list_set_ext(0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_string_scroll_wheel_event_process(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_string_scroll_wheel_event_process(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_system_animation_get(0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_system_animation_get(0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_system_animation_free(0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_system_animation_free(0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_system_canvas_refresh();
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_system_canvas_refresh();
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_system_dirty_mark(0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_system_dirty_mark(0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_system_dirty_partial_add(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_system_dirty_partial_add(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_system_focus_claim(0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_system_focus_claim(0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_system_initialize();
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_system_initialize();
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_system_memory_allocator_set(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_system_memory_allocator_set(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_system_screen_stack_pop();
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_system_screen_stack_pop();
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_system_screen_stack_get(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_system_screen_stack_get(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_system_screen_stack_push(GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_system_screen_stack_push(GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_system_scroll_appearance_get(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_system_scroll_appearance_get(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_system_scroll_appearance_set(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_system_scroll_appearance_set(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_system_start();
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_system_start();
    EXPECT_EQ(GX_CALLER_ERROR, status);

#ifdef GX_ENABLE_DEPRECATED_STRING_API
    _tx_thread_system_state = 1;
    status = gx_system_string_width_get(0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_system_string_width_get(0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
#endif
    _tx_thread_system_state = 1;
    status = gx_system_string_width_get_ext(0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_system_string_width_get_ext(0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_system_timer_start(0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_system_timer_start(0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_system_timer_stop(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_system_timer_stop(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_text_button_create(0, 0, 0, 0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_text_button_create(0, 0, 0, 0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_text_button_font_set(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_text_button_font_set(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

#ifdef GX_ENABLE_DEPRECATED_STRING_API
    _tx_thread_system_state = 1;
    status = gx_text_button_text_get(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_text_button_text_get(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
#endif
    _tx_thread_system_state = 1;
    status = gx_text_button_text_get_ext(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_text_button_text_get_ext(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_text_button_text_id_set(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_text_button_text_id_set(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_text_button_event_process(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_text_button_event_process(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

#ifdef GX_ENABLE_DEPRECATED_STRING_API
    _tx_thread_system_state = 1;
    status = gx_text_button_text_set(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_text_button_text_set(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
#endif
    _tx_thread_system_state = 1;
    status = gx_text_button_text_set_ext(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_text_button_text_set_ext(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_text_scroll_wheel_font_set(0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_text_scroll_wheel_font_set(0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_text_scroll_wheel_text_color_set(0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_text_scroll_wheel_text_color_set(0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_text_scroll_wheel_create(0, 0, 0, 0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_text_scroll_wheel_create(0, 0, 0, 0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_tree_view_create(0, 0, 0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_tree_view_create(0, 0, 0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_tree_view_event_process(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_tree_view_event_process(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_tree_view_indentation_set(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_tree_view_indentation_set(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_tree_view_position(0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_tree_view_position(0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_tree_view_root_line_color_set(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_tree_view_root_line_color_set(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_tree_view_root_pixelmap_set(0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_tree_view_root_pixelmap_set(0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_tree_view_selected_set(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_tree_view_selected_set(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_utility_gradient_create(0, 0, 0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_utility_gradient_create(0, 0, 0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_utility_gradient_delete(0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_utility_gradient_delete(0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_utility_canvas_to_bmp(0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_utility_canvas_to_bmp(0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_vertical_list_children_position(0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_vertical_list_children_position(0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_vertical_list_create(0, 0, 0, 0, 0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_vertical_list_create(0, 0, 0, 0, 0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_vertical_list_event_process(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_vertical_list_event_process(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_vertical_list_page_index_set(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_vertical_list_page_index_set(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_vertical_list_selected_set(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_vertical_list_selected_set(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_vertical_list_total_rows_set(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_vertical_list_total_rows_set(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_vertical_scrollbar_create(0, 0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_vertical_scrollbar_create(0, 0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_widget_attach(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_widget_attach(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_widget_back_attach(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_widget_back_attach(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_widget_back_move(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_widget_back_move(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_widget_block_move(0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_widget_block_move(0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_widget_border_style_set(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_widget_border_style_set(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_widget_create(0, 0, 0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_widget_create(0, 0, 0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_widget_delete(0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_widget_delete(0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_widget_detach(0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_widget_detach(0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_widget_draw_set(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_widget_draw_set(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_widget_event_generate(0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_widget_event_generate(0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_widget_event_process(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_widget_event_process(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_widget_event_to_parent(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_widget_event_to_parent(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_widget_fill_color_set(0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_widget_fill_color_set(0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_widget_focus_next(0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_widget_focus_next(0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_widget_focus_previous(0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_widget_focus_previous(0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_widget_front_move(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_widget_front_move(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_widget_height_get(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_widget_height_get(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_widget_hide(0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_widget_hide(0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_widget_resize(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_widget_resize(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_widget_shift(0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_widget_shift(0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_widget_show(0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_widget_show(0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

#ifdef GX_ENABLE_DEPRECATED_STRING_API
    _tx_thread_system_state = 1;
    status = gx_widget_string_get(0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_widget_string_get(0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
#endif
    _tx_thread_system_state = 1;
    status = gx_widget_string_get_ext(0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_widget_string_get_ext(0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_widget_style_add(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_widget_style_add(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_widget_style_get(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_widget_style_get(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_widget_style_remove(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_widget_style_remove(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_widget_style_set(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_widget_style_set(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

#ifdef GX_ENABLE_DEPRECATED_STRING_API
    _tx_thread_system_state = 1;
    status = gx_widget_text_blend(0, 0, 0, 0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_widget_text_blend(0, 0, 0, 0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
#endif
    _tx_thread_system_state = 1;
    status = gx_widget_text_blend_ext(0, 0, 0, 0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_widget_text_blend_ext(0, 0, 0, 0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_widget_width_get(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_widget_width_get(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_window_client_scroll(0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_window_client_scroll(0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_window_client_width_get(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_window_client_width_get(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_window_close(0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_window_close(0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_window_create(0, 0, 0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_window_create(0, 0, 0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_window_event_process(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_window_event_process(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_window_execute(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_window_execute(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_window_root_create(0, 0, 0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_window_root_create(0, 0, 0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_window_root_delete(0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_window_root_delete(0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_window_root_event_process(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_window_root_event_process(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_window_wallpaper_set(0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_window_wallpaper_set(0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_multi_line_text_view_event_process(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_multi_line_text_view_event_process(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_radial_slider_event_process(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_radial_slider_event_process(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

#ifdef GX_ENABLE_DEPRECATED_STRING_API
    _tx_thread_system_state = 1;
    status = gx_multi_line_text_input_text_set(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_multi_line_text_input_text_set(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
#endif
    _tx_thread_system_state = 1;
    status = gx_multi_line_text_input_text_set_ext(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_multi_line_text_input_text_set_ext(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_multi_line_text_view_whitespace_set(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_multi_line_text_view_whitespace_set(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_radial_progress_bar_info_set(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_radial_progress_bar_info_set(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_radio_button_pixelmap_set(0, 0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_radio_button_pixelmap_set(0, 0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_sprite_stop(0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_sprite_stop(0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_text_button_text_color_set(0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_text_button_text_color_set(0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_generic_scroll_wheel_create(0, 0, 0, 0, 0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_generic_scroll_wheel_create(0, 0, 0, 0, 0, 0, 0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_generic_scroll_wheel_event_process(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_generic_scroll_wheel_event_process(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_generic_scroll_wheel_row_height_set(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_generic_scroll_wheel_row_height_set(0, 0);

    _tx_thread_system_state = 1;
    status = gx_generic_scroll_wheel_total_rows_set(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_generic_scroll_wheel_total_rows_set(0, 0);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_generic_scroll_wheel_children_position(GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    _tx_thread_system_state = -1;
    status = gx_generic_scroll_wheel_children_position(GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = 1;
    status = gx_animation_delete(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);

    _tx_thread_system_state = -1;
    status = gx_animation_delete(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_CALLER_ERROR, status);
    
    if (failed_tests)
    {
        gx_validation_print_test_result(TEST_FAIL);
    }
    else
    {
        gx_validation_print_test_result(TEST_SUCCESS);
    }

    exit(failed_tests);
}
#endif /* defined (GX_DISABLE_ERROR_CHECKING) */

#include "gx_validation_wrapper.h"
#include "demo_guix_all_widgets.c" //This test is added in all_widgets reg tests list, so it should include this file
