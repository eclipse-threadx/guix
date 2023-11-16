/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_error_checking_api_compile_no_output", /* Test name */
    0, 0, 0, 0  /* Define the coordinates of the capture area.*/
};

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
static VOID      control_thread_entry(ULONG);

VOID tx_application_define(void *first_unused_memory)
{

    /* Create a dedicated thread to perform various operations
       on the line drawing example. These operations simulate 
       user input. */
    gx_validation_control_thread_create(control_thread_entry);
}

static VOID control_thread_entry(ULONG input)
{
GX_POINT pos;

    /* accordion menu */
    gx_accordion_menu_create(GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    gx_accordion_menu_event_process(GX_NULL, GX_NULL);
    gx_accordion_menu_position(GX_NULL);
    
    /* animation */
    gx_animation_canvas_define(GX_NULL, GX_NULL);
    gx_animation_create(GX_NULL);
    gx_animation_drag_disable(GX_NULL, GX_NULL);
    gx_animation_drag_enable(GX_NULL, GX_NULL, GX_NULL);
    gx_animation_landing_speed_set(GX_NULL, GX_NULL);
    gx_animation_start(GX_NULL, GX_NULL);
    gx_animation_stop(GX_NULL);
    
    /* binres */
    gx_binres_language_table_load(GX_NULL, GX_NULL);
    gx_binres_theme_load(GX_NULL, GX_NULL, GX_NULL);
    
    /* brush */
    gx_brush_default(GX_NULL);
    gx_brush_define(GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    
    /* button */
    gx_button_create(GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    gx_button_deselect(GX_NULL, GX_TRUE);
    gx_button_event_process(GX_NULL, GX_NULL);
    gx_button_select(GX_NULL);
    
    /* canvas */
    gx_canvas_alpha_set(GX_NULL, GX_NULL);
    gx_canvas_arc_draw(GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    gx_canvas_block_move(GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    gx_canvas_circle_draw(GX_NULL, GX_NULL, GX_NULL);
    gx_canvas_create(GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    gx_canvas_delete(GX_NULL);
    gx_canvas_drawing_complete(GX_NULL, GX_NULL);
    gx_canvas_drawing_initiate(GX_NULL, GX_NULL, GX_NULL);
    gx_canvas_ellipse_draw(GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    gx_canvas_hardware_layer_bind(GX_NULL, GX_NULL);
    gx_canvas_hide(GX_NULL);
    gx_canvas_line_draw(GX_NULL, GX_NULL, GX_NULL, GX_NULL);
#if defined (GX_MOUSE_SUPPORT)
    gx_canvas_mouse_define(GX_NULL, GX_NULL);
    gx_canvas_mouse_hide(GX_NULL);
    gx_canvas_mouse_show(GX_NULL);
#endif
    gx_canvas_offset_set(GX_NULL, GX_NULL, GX_NULL);
    gx_canvas_pie_draw(GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    gx_canvas_pixelmap_blend(GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    gx_canvas_pixelmap_draw(GX_NULL, GX_NULL, GX_NULL);
    gx_canvas_pixelmap_get(GX_NULL);
    gx_canvas_pixelmap_rotate(GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    gx_canvas_pixelmap_tile(GX_NULL, GX_NULL);
    gx_canvas_polygon_draw(GX_NULL, GX_NULL);
    gx_canvas_rectangle_draw(GX_NULL);
    gx_canvas_rotated_text_draw(GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    gx_canvas_shift(GX_NULL, GX_NULL, GX_NULL);
    gx_canvas_show(GX_NULL);
    gx_canvas_text_draw(GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    
    /* checkbox */
    gx_checkbox_create(GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    gx_checkbox_event_process(GX_NULL, GX_NULL);
    gx_checkbox_pixelmap_set(GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    gx_checkbox_select(GX_NULL);
    
    /* circular gauge */
    gx_circular_gauge_angle_get(GX_NULL, GX_NULL);
    gx_circular_gauge_angle_set(GX_NULL, GX_NULL);
    gx_circular_gauge_animation_set(GX_NULL, GX_NULL, GX_NULL);
    gx_circular_gauge_create(GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    gx_circular_gauge_event_process(GX_NULL, GX_NULL);
    
    /* context brush */
    gx_context_brush_default(GX_NULL);
    gx_context_brush_define(GX_NULL, GX_NULL, GX_NULL);
    gx_context_brush_get(GX_NULL);
    gx_context_brush_pattern_set(GX_NULL);
    gx_context_brush_set(GX_NULL);
    gx_context_brush_style_set(GX_NULL);
    gx_context_brush_width_set(GX_NULL);
    gx_context_color_get(GX_NULL, GX_NULL);
    gx_context_fill_color_set(GX_NULL);
    gx_context_font_get(GX_NULL, GX_NULL);
    gx_context_font_set(GX_NULL);
    gx_context_line_color_set(GX_NULL);
    gx_context_pixelmap_get(GX_NULL, GX_NULL);
    gx_context_pixelmap_set(GX_NULL);
    gx_context_raw_brush_define(GX_NULL, GX_NULL, GX_NULL);
    gx_context_raw_fill_color_set(GX_NULL);
    gx_context_raw_line_color_set(GX_NULL);
    
    /* display */
    gx_display_active_language_set(GX_NULL, GX_NULL);
    gx_display_color_set(GX_NULL, GX_NULL, GX_NULL);
    gx_display_color_table_set(GX_NULL, GX_NULL, GX_NULL);
    gx_display_create(GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    gx_display_delete(GX_NULL, GX_NULL);
    gx_display_font_table_set(GX_NULL, GX_NULL, GX_NULL);
    gx_display_language_table_get(GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    gx_display_language_table_set(GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    gx_display_pixelmap_table_set(GX_NULL, GX_NULL, GX_NULL);
    gx_display_string_get(GX_NULL, GX_NULL, GX_NULL);
    gx_display_string_table_get(GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    gx_display_theme_install(GX_NULL, GX_NULL);
    
    /* drop list */
    gx_drop_list_close(GX_NULL);
    gx_drop_list_create(GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    gx_drop_list_event_process(GX_NULL, GX_NULL);
    gx_drop_list_open(GX_NULL);
    gx_drop_list_pixelmap_set(GX_NULL, GX_NULL);
    gx_drop_list_popup_get(GX_NULL, GX_NULL);
    
    /* horizontal list */
    gx_horizontal_list_children_position(GX_NULL);
    gx_horizontal_list_create(GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    gx_horizontal_list_event_process(GX_NULL, GX_NULL);
    gx_horizontal_list_page_index_set(GX_NULL, GX_NULL);
    gx_horizontal_list_selected_index_get(GX_NULL, GX_NULL);
    gx_horizontal_list_selected_set(GX_NULL, GX_NULL);
    gx_horizontal_list_selected_widget_get(GX_NULL, GX_NULL);
    gx_horizontal_list_total_columns_set(GX_NULL, GX_NULL);
    
    /* horizontal scrollbar */
    gx_horizontal_scrollbar_create(GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    
    /* icon */
    gx_icon_button_create(GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    gx_icon_button_pixelmap_set(GX_NULL, GX_NULL);
    gx_icon_create(GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    gx_icon_event_process(GX_NULL, GX_NULL);
    gx_icon_pixelmap_set(GX_NULL, GX_NULL, GX_NULL);
    
    /* image reader */
    gx_image_reader_create(GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    gx_image_reader_palette_set(GX_NULL, GX_NULL, GX_NULL);
    gx_image_reader_start(GX_NULL, GX_NULL);
    
    /* line chart */
    gx_line_chart_create(GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    gx_line_chart_update(GX_NULL, GX_NULL, GX_NULL);
    gx_line_chart_y_scale_calculate(GX_NULL, GX_NULL);
    
    /* menu */
    gx_menu_create(GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    gx_menu_insert(GX_NULL, GX_NULL);
    gx_menu_remove(GX_NULL, GX_NULL);
    gx_menu_text_offset_set(GX_NULL, GX_NULL, GX_NULL);
    
    /* multi line text button */
    gx_multi_line_text_button_create(GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    gx_multi_line_text_button_event_process(GX_NULL, GX_NULL);
    gx_multi_line_text_button_text_id_set(GX_NULL, GX_NULL);
    gx_multi_line_text_button_text_set(GX_NULL, GX_NULL);
    
    /* multi line text input */
    gx_multi_line_text_input_backspace(GX_NULL);
    gx_multi_line_text_input_buffer_clear(GX_NULL);
    gx_multi_line_text_input_buffer_get(GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    gx_multi_line_text_input_char_insert(GX_NULL, GX_NULL, 0);
    gx_multi_line_text_input_create(GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    gx_multi_line_text_input_delete(GX_NULL);
    gx_multi_line_text_input_down_arrow(GX_NULL);
    gx_multi_line_text_input_end(GX_NULL);
    gx_multi_line_text_input_event_process(GX_NULL, GX_NULL);
    gx_multi_line_text_input_fill_color_set(GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    gx_multi_line_text_input_home(GX_NULL);
    gx_multi_line_text_input_left_arrow(GX_NULL);
    gx_multi_line_text_input_right_arrow(GX_NULL);
    gx_multi_line_text_input_style_add(GX_NULL, GX_NULL);
    gx_multi_line_text_input_style_remove(GX_NULL, GX_NULL);
    gx_multi_line_text_input_style_set(GX_NULL, GX_NULL);
    gx_multi_line_text_input_text_color_set(GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    gx_multi_line_text_input_text_select(GX_NULL, GX_NULL, GX_NULL);
    gx_multi_line_text_input_text_set(GX_NULL, GX_NULL);
    gx_multi_line_text_input_up_arrow(GX_NULL);

    /* multi line text view */
    gx_multi_line_text_view_create(GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    gx_multi_line_text_view_event_process(GX_NULL, GX_NULL);
    gx_multi_line_text_view_font_set(GX_NULL, GX_NULL);
    gx_multi_line_text_view_line_space_set(GX_NULL, GX_NULL);
    gx_multi_line_text_view_scroll_info_get(GX_NULL, GX_NULL, GX_NULL);
    gx_multi_line_text_view_text_color_set(GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    gx_multi_line_text_view_text_id_set(GX_NULL, GX_NULL);
    gx_multi_line_text_view_text_set(GX_NULL, GX_NULL);
    gx_multi_line_text_view_whitespace_set(GX_NULL, GX_NULL);
    
    /* numeric pixelmap prompt */
    gx_numeric_pixelmap_prompt_create(GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    gx_numeric_pixelmap_prompt_format_function_set(GX_NULL, GX_NULL);
    gx_numeric_pixelmap_prompt_value_set(GX_NULL, GX_NULL);
    
    /* numeric prompt */
    gx_numeric_prompt_create(GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    gx_numeric_prompt_format_function_set(GX_NULL, GX_NULL);
    gx_numeric_prompt_value_set(GX_NULL, GX_NULL);
    
    /* numeric scroll wheel */
    gx_numeric_scroll_wheel_create(GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    gx_numeric_scroll_wheel_range_set(GX_NULL, GX_NULL, GX_NULL);
    
    /* pixelmap button */
    gx_pixelmap_button_create(GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    gx_pixelmap_button_event_process(GX_NULL, GX_NULL);
    gx_pixelmap_button_pixelmap_set(GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    
    /* pixelmap prompt */
    gx_pixelmap_prompt_create(GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    gx_pixelmap_prompt_pixelmap_set(GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    
    /* pixelmap slider*/
    gx_pixelmap_slider_create(GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    gx_pixelmap_slider_event_process(GX_NULL, GX_NULL);
    gx_pixelmap_slider_pixelmap_set(GX_NULL, GX_NULL);
    
    /* progress bar */ 
    gx_progress_bar_create(GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    gx_progress_bar_event_process(GX_NULL, GX_NULL);
    gx_progress_bar_font_set(GX_NULL, GX_NULL);
    gx_progress_bar_info_set(GX_NULL, GX_NULL);
    gx_progress_bar_pixelmap_set(GX_NULL, GX_NULL);
    gx_progress_bar_range_set(GX_NULL, GX_NULL, GX_NULL);
    gx_progress_bar_text_color_set(GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    gx_progress_bar_value_set(GX_NULL, GX_NULL);
    
    /* prompt */
    gx_prompt_create(GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    gx_prompt_font_set(GX_NULL, GX_NULL);
    gx_prompt_text_color_set(GX_NULL, GX_NULL, GX_NULL, GX_NULL);
#ifdef GX_ENABLE_DEPRECATED_STRING_API
    gx_prompt_text_get(GX_NULL, GX_NULL);
#endif
    gx_prompt_text_get_ext(GX_NULL, GX_NULL);
    gx_prompt_text_id_set(GX_NULL, GX_NULL);
    gx_prompt_text_set(GX_NULL, GX_NULL);
    
    /* radial progress bar */ 
    gx_radial_progress_bar_anchor_set(GX_NULL, GX_NULL);
    gx_radial_progress_bar_create(GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    gx_radial_progress_bar_event_process(GX_NULL, GX_NULL);
    gx_radial_progress_bar_font_set(GX_NULL, GX_NULL);
    gx_radial_progress_bar_info_set(GX_NULL, GX_NULL);
    gx_radial_progress_bar_text_color_set(GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    gx_radial_progress_bar_value_set(GX_NULL, GX_NULL);
    
    /* radio button */
    gx_radio_button_create(GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    gx_radio_button_pixelmap_set(GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL);
   
    /* radial slider */
    gx_radial_slider_anchor_angles_set(GX_NULL, GX_NULL, 0);
    gx_radial_slider_angle_set(GX_NULL, 0);
    gx_radial_slider_animation_set(GX_NULL, 0, 0, 0, GX_NULL);
    gx_radial_slider_animation_start(GX_NULL, 0);
    gx_radial_slider_create(GX_NULL, GX_NULL, GX_NULL, GX_NULL, 0, 0, GX_NULL);
    gx_radial_slider_event_process(GX_NULL, GX_NULL);
    gx_radial_slider_info_get(GX_NULL, GX_NULL);
    gx_radial_slider_info_set(GX_NULL, GX_NULL);
    gx_radial_slider_pixelmap_set(GX_NULL, 0, 0);
 
    /* screen stack */
    gx_screen_stack_create(GX_NULL, GX_NULL, GX_NULL);
    gx_screen_stack_pop(GX_NULL);
    gx_screen_stack_push(GX_NULL, GX_NULL, GX_NULL);
    gx_screen_stack_reset(GX_NULL);
    
    /* scroll thumb */
    gx_scroll_thumb_create(GX_NULL, GX_NULL, GX_NULL);
    gx_scroll_thumb_event_process(GX_NULL, GX_NULL);
    
    /* scrollbar */
    gx_scrollbar_event_process(GX_NULL, GX_NULL);
    gx_scrollbar_limit_check(GX_NULL);
    gx_scrollbar_reset(GX_NULL, GX_NULL);
    
    /* scroll wheel */
    gx_scroll_wheel_create(GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    gx_scroll_wheel_event_process(GX_NULL, GX_NULL);
    gx_scroll_wheel_gradient_alpha_set(GX_NULL, GX_NULL, GX_NULL);
    gx_scroll_wheel_row_height_set(GX_NULL, GX_NULL);
    gx_scroll_wheel_selected_background_set(GX_NULL, GX_NULL);
    gx_scroll_wheel_selected_get(GX_NULL, GX_NULL);
    gx_scroll_wheel_selected_set(GX_NULL, GX_NULL);
    gx_scroll_wheel_speed_set(GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    gx_scroll_wheel_total_rows_set(GX_NULL, GX_NULL);
    
    /* single line text input */
    gx_single_line_text_input_backspace(GX_NULL);
    gx_single_line_text_input_buffer_clear(GX_NULL);
    gx_single_line_text_input_buffer_get(GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    gx_single_line_text_input_character_delete(GX_NULL);
    gx_single_line_text_input_character_insert(GX_NULL, GX_NULL, GX_NULL);
    gx_single_line_text_input_create(GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    gx_single_line_text_input_draw_position_get(GX_NULL, GX_NULL, GX_NULL);
    gx_single_line_text_input_end(GX_NULL);
    gx_single_line_text_input_event_process(GX_NULL, GX_NULL);
    gx_single_line_text_input_fill_color_set(GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    gx_single_line_text_input_home(GX_NULL);
    gx_single_line_text_input_left_arrow(GX_NULL);
    gx_single_line_text_input_position_get(GX_NULL, GX_NULL);
    gx_single_line_text_input_right_arrow(GX_NULL);
    gx_single_line_text_input_style_add(GX_NULL, GX_NULL);
    gx_single_line_text_input_style_remove(GX_NULL, GX_NULL);
    gx_single_line_text_input_style_set(GX_NULL, GX_NULL);
    gx_single_line_text_input_text_color_set(GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    gx_single_line_text_input_text_select(GX_NULL, GX_NULL, GX_NULL);
    gx_single_line_text_input_text_set(GX_NULL, GX_NULL);

    /* slider */
    gx_slider_create(GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    gx_slider_event_process(GX_NULL, GX_NULL);
    gx_slider_info_set(GX_NULL, GX_NULL);
    gx_slider_needle_position_get(GX_NULL, GX_NULL, GX_NULL);
    gx_slider_travel_get(GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    gx_slider_value_calculate(GX_NULL, GX_NULL, GX_NULL);
    gx_slider_value_set(GX_NULL, GX_NULL, GX_NULL);
    
    /* sprite */
    gx_sprite_create(GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    gx_sprite_current_frame_set(GX_NULL, GX_NULL);
    gx_sprite_frame_list_set(GX_NULL, GX_NULL, GX_NULL);
    gx_sprite_start(GX_NULL, GX_NULL);
    gx_sprite_stop(GX_NULL);
    
    /* string scroll wheel */
    gx_string_scroll_wheel_create(GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    gx_string_scroll_wheel_string_list_set(GX_NULL, GX_NULL, GX_NULL);
    gx_string_scroll_wheel_string_id_list_set(GX_NULL, GX_NULL, GX_NULL);
    
    /* system language */
    gx_system_active_language_set(GX_NULL);
    
    /* system animation */
    gx_system_animation_get(GX_NULL);
    gx_system_animation_free(GX_NULL);
    
    /* system */
    gx_system_canvas_refresh();
    gx_system_dirty_mark(GX_NULL);
    gx_system_dirty_partial_add(GX_NULL, GX_NULL);
    gx_system_draw_context_get(GX_NULL);
    gx_system_event_fold(GX_NULL);
    gx_system_event_send(GX_NULL);
    gx_system_focus_claim(GX_NULL);
    gx_system_initialize();
    gx_system_language_table_get(GX_NULL, GX_NULL, GX_NULL);
    gx_system_language_table_set(GX_NULL, GX_NULL, GX_NULL);
    gx_system_memory_allocator_set(GX_NULL, GX_NULL);
    gx_system_pen_configure(GX_NULL);
    gx_system_scroll_appearance_get(GX_NULL, GX_NULL);
    gx_system_scroll_appearance_set(GX_NULL, GX_NULL);
    gx_system_string_get(GX_NULL, GX_NULL);
    gx_system_string_table_get(GX_NULL, GX_NULL, GX_NULL);
    gx_system_string_width_get(GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    gx_system_timer_start(GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    gx_system_timer_stop(GX_NULL, GX_NULL);
    gx_system_version_string_get(GX_NULL);
    gx_system_widget_find(GX_NULL, GX_NULL, GX_NULL);
    
    /* text button */
    gx_text_button_create(GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    gx_text_button_font_set(GX_NULL, GX_NULL);
    gx_text_button_text_color_set(GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    gx_text_button_text_get(GX_NULL, GX_NULL);
    gx_text_button_text_id_set(GX_NULL, GX_NULL);
    gx_text_button_text_set(GX_NULL, GX_NULL);
    
    /* text input cursor */
    gx_text_input_cursor_blink_interval_set(GX_NULL, GX_NULL);
    gx_text_input_cursor_height_set(GX_NULL, GX_NULL);
    gx_text_input_cursor_width_set(GX_NULL, GX_NULL);
    
    /* text scroll wheel */
    gx_text_scroll_wheel_callback_set(GX_NULL, GX_NULL);
    gx_text_scroll_wheel_font_set(GX_NULL, GX_NULL, GX_NULL);
    gx_text_scroll_wheel_text_color_set(GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    gx_text_scroll_wheel_create(GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    
    /* utility gradient */
    gx_utility_gradient_create(GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    gx_utility_gradient_delete(GX_NULL);
    
    /* tree view */
    gx_tree_view_create(GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    gx_tree_view_event_process(GX_NULL, GX_NULL);
    gx_tree_view_indentation_set(GX_NULL, GX_NULL);
    gx_tree_view_position(GX_NULL);
    gx_tree_view_root_line_color_set(GX_NULL, GX_NULL);
    gx_tree_view_root_pixelmap_set(GX_NULL, GX_NULL, GX_NULL);
    gx_tree_view_selected_get(GX_NULL, GX_NULL);
    gx_tree_view_selected_set(GX_NULL, GX_NULL);
    
    /* utility */
    gx_utility_canvas_to_bmp(GX_NULL, GX_NULL, GX_NULL);
    gx_utility_ltoa(GX_NULL, GX_NULL, GX_NULL);
    gx_utility_math_acos(GX_NULL);
    gx_utility_math_asin(GX_NULL);
    gx_utility_math_cos(GX_NULL);
    gx_utility_math_sin(GX_NULL);
    gx_utility_math_sqrt(GX_NULL);
    gx_utility_pixelmap_resize(GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    gx_utility_pixelmap_rotate(GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    gx_utility_pixelmap_simple_rotate(GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    gx_utility_rectangle_center(GX_NULL, GX_NULL);
    gx_utility_rectangle_center_find(GX_NULL, GX_NULL);
    gx_utility_rectangle_combine(GX_NULL, GX_NULL);
    gx_utility_rectangle_compare(GX_NULL, GX_NULL);
    gx_utility_rectangle_define(GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    gx_utility_rectangle_overlap_detect(GX_NULL, GX_NULL, GX_NULL);
    gx_utility_rectangle_point_detect(GX_NULL, pos);
    gx_utility_rectangle_resize(GX_NULL, GX_NULL);
    gx_utility_rectangle_shift(GX_NULL, GX_NULL, GX_NULL);
    gx_utility_string_to_alphamap(GX_NULL, GX_NULL, GX_NULL);
    
    /* vertical list */
    gx_vertical_list_children_position(GX_NULL);
    gx_vertical_list_create(GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    gx_vertical_list_event_process(GX_NULL, GX_NULL);
    gx_vertical_list_page_index_set(GX_NULL, GX_NULL);
    gx_vertical_list_selected_index_get(GX_NULL, GX_NULL);
    gx_vertical_list_selected_set(GX_NULL, GX_NULL);
    gx_vertical_list_selected_widget_get(GX_NULL, GX_NULL);
    gx_vertical_list_total_rows_set(GX_NULL, GX_NULL);
    
    /* vertical scrollbar */
    gx_vertical_scrollbar_create(GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    
    /* widget */
    gx_widget_allocate(GX_NULL, GX_NULL);
    gx_widget_attach(GX_NULL, GX_NULL);
    gx_widget_back_attach(GX_NULL, GX_NULL);
    gx_widget_back_move(GX_NULL, GX_NULL);
    gx_widget_block_move(GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    gx_widget_border_style_set(GX_NULL, GX_NULL);
    gx_widget_border_width_get(GX_NULL, GX_NULL);
    gx_widget_canvas_get(GX_NULL, GX_NULL);
    gx_widget_child_detect(GX_NULL, GX_NULL, GX_NULL);
    gx_widget_color_get(GX_NULL, GX_NULL, GX_NULL);
    gx_widget_client_get(GX_NULL, GX_NULL, GX_NULL);
    gx_widget_create(GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    gx_widget_delete(GX_NULL);
    gx_widget_detach(GX_NULL);
    gx_widget_draw_set(GX_NULL, GX_NULL);
    gx_widget_event_generate(GX_NULL, GX_NULL, GX_NULL);
    gx_widget_event_process(GX_NULL, GX_NULL);
    gx_widget_event_process_set(GX_NULL, GX_NULL);
    gx_widget_event_to_parent(GX_NULL, GX_NULL);
    gx_widget_fill_color_set(GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    gx_widget_find(GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    gx_widget_focus_next(GX_NULL);
    gx_widget_focus_previous(GX_NULL);
    gx_widget_font_get(GX_NULL, GX_NULL, GX_NULL);
    gx_widget_free(GX_NULL);
    gx_widget_front_move(GX_NULL, GX_NULL);
    gx_widget_height_get(GX_NULL, GX_NULL);
    gx_widget_hide(GX_NULL);
    gx_widget_pixelmap_get(GX_NULL, GX_NULL, GX_NULL);
    gx_widget_resize(GX_NULL, GX_NULL);
    gx_widget_shift(GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    gx_widget_show(GX_NULL);
    gx_widget_status_add(GX_NULL, GX_NULL);
    gx_widget_status_get(GX_NULL, GX_NULL);
    gx_widget_status_remove(GX_NULL, GX_NULL);
    gx_widget_status_test(GX_NULL, GX_NULL, GX_NULL);
    gx_widget_style_add(GX_NULL, GX_NULL);
    gx_widget_style_get(GX_NULL, GX_NULL);
    gx_widget_style_remove(GX_NULL, GX_NULL);
    gx_widget_style_set(GX_NULL, GX_NULL);
    gx_widget_text_blend(GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    gx_widget_type_find(GX_NULL, GX_NULL, GX_NULL);
    gx_widget_width_get(GX_NULL, GX_NULL);
    
    /* window */
    gx_window_client_height_get(GX_NULL, GX_NULL);
    gx_window_client_scroll(GX_NULL, GX_NULL, GX_NULL);
    gx_window_client_width_get(GX_NULL, GX_NULL);
    gx_window_close(GX_NULL);
    gx_window_create(GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    gx_window_event_process(GX_NULL, GX_NULL);
    gx_window_execute(GX_NULL, GX_NULL);
    gx_window_root_create(GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL);
    gx_window_root_delete(GX_NULL);
    gx_window_root_event_process(GX_NULL, GX_NULL);
    gx_window_root_find(GX_NULL, GX_NULL);
    gx_window_scroll_info_get(GX_NULL, GX_NULL, GX_NULL);
    gx_window_scrollbar_find(GX_NULL, GX_NULL, GX_NULL);
    gx_window_wallpaper_get(GX_NULL, GX_NULL);
    gx_window_wallpaper_set(GX_NULL, GX_NULL, GX_NULL);

    gx_validation_print_test_result(TEST_SUCCESS);
    exit(0);

}
#endif

#include "gx_validation_wrapper.h"
#include "demo_guix_all_widgets.c" //This test is added in all_widgets reg tests list, so it should include this file

