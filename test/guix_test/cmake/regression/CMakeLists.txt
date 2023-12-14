cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)
cmake_policy(SET CMP0057 NEW)

project(regression_test LANGUAGES C)

# Set directories
get_filename_component(ROOT_DIR ${CMAKE_CURRENT_LIST_DIR}/../../../../ ABSOLUTE)

# Demos of different type of build cases
set(NO_UTF8_DEMOS
    ml_text_input_8bpp
    all_widgets_1bpp
    all_widgets_4bpp
    all_widgets_332rgb
    all_widgets_16bpp
    all_widgets_4444argb
    multi_line_text_view_clip_32bpp
    multi_line_text_view_clip_16bpp
    single_line_text_input_clip_32bpp
    single_line_text_input_clip_16bpp
    multi_line_text_view_font_32bpp
    multi_line_text_view_font_16bpp
    all_widgets_8bpp
    glyph_draw_8bpp_screen
    glyph_draw_16bpp_screen
    glyph_draw_32bpp_screen
    multi_line_text_view_font_8bpp
    multi_line_text_view_clip_8bpp
    single_line_text_input_clip_8bpp
    synergy_glyph_draw_16bpp
    synergy_glyph_draw_32bpp
    text_rotation)
set(EXTENDED_UNICODE_DEMOS extended_unicode standalone_binres_load_extended_unicode_32bpp)
set(OTHER_DEMOS
    all_widgets
    all_widgets_32argb
    glyph_draw_multi_lang_8bpp
    glyph_draw_multi_lang_16bpp
    glyph_draw_multi_lang_32bpp
    ml_text_view_32bpp
    ml_text_view_16bpp
    ml_text_view_8bpp
    multi_themes_8bpp_palette
    ml_text_view_vcenter
    multi_themes_16bpp
    multi_themes_16bpp_synergy
    ml_text_input_32bpp
    ml_text_input_8bpp
    ml_text_input_16bpp
    polygon_16bpp
    polygon_1bpp
    all_widgets_1bpp
    all_widgets_4bpp
    all_widgets_332rgb
    all_widgets_8bpp_rotated_ccw
    all_widgets_8bpp_rotated_ccw_binary
    all_widgets_8bpp_rotated_cw
    all_widgets_8bpp_rotated_cw_binary
    all_widgets_synergy_8bpp_rotated_ccw
    all_widgets_synergy_8bpp_rotated_ccw_binary
    all_widgets_synergy_8bpp_rotated_cw
    all_widgets_synergy_8bpp_rotated_cw_binary
    all_widgets_synergy_8bpp_rotated_cw_dave2d_disabled
    all_widgets_24xrgb_rotated_ccw
    all_widgets_24xrgb_rotated_ccw_binary
    all_widgets_24xrgb_rotated_cw
    all_widgets_24xrgb_rotated_cw_binary
    all_widgets_32argb_rotated_cw
    all_widgets_32argb_rotated_ccw
    all_widgets_synergy_24xrgb_rotated_ccw
    all_widgets_synergy_24xrgb_rotated_ccw_binary
    all_widgets_synergy_24xrgb_rotated_cw
    all_widgets_synergy_24xrgb_rotated_cw_binary
    all_widgets_synergy_24xrgb_rotated_cw_dave2d_disabled
    all_widgets_synergy_16bpp_flip
    all_widgets_16bpp
    all_widgets_16bpp_rotated
    all_widgets_16bpp_rotated_270
    all_widgets_16bpp_rotated_cw_binary
    all_widgets_16bpp_rotated_ccw_binary
    all_widgets_4444argb
    binres_theme_static_language
    drop_list
    pixelmap_8bpp
    pixelmap_16bpp
    pixelmaps_4444argb
    pixelmaps_565rgb
    pixelmap_32bpp
    pixelmaps_32bpp
    pixelmap_clip_32bpp
    pixelmap_clip_16bpp
    sprite_16bpp
    multi_line_text_view_clip_32bpp
    multi_line_text_view_clip_16bpp
    single_line_text_input_clip_32bpp
    single_line_text_input_clip_16bpp
    multi_line_text_view_font_32bpp
    multi_line_text_view_font_16bpp
    all_widgets_8bpp
    glyph_draw_8bpp_screen
    glyph_draw_16bpp_screen
    glyph_draw_32bpp_screen
    multi_line_text_view_font_8bpp
    multi_line_text_view_clip_8bpp
    single_line_text_input_clip_8bpp
    line_1bpp
    line_8bpp
    pixelmap_resize_32bpp
    pixelmap_resize_16bpp
    pixelmap_resize_8bpp
    vertical_list_32bpp
    horizontal_list_32bpp
    synergy_4444argb_32bpp
    synergy_4444argb_16bpp
    generic_4444argb_16bpp
    generic_16bpp_32bppdriver
    synergy_32bpp_16bppdriver
    synergy_565rgb
    synergy_32argb
    standalone_binres_load_16bpp
    standalone_binres_load_8bpp
    all_widgets_synergy_565rgb
    all_widgets_synergy
    jpeg_draw_32bpp
    jpeg_draw_16bpp
    alphamap_16bpp
    alphamap_16bppdriver
    alphamap_32bppdriver
    palette_32bpp
    palette_16bpp
    sl_text_input_16bpp
    synergy_glyph_draw_16bpp
    synergy_glyph_draw_32bpp
    color_convert_32bpp
    color_convert_16bpp
    color_convert_8bpp
    graphics_32bpp_synergy
    graphics_565rgb_synergy
    graphics_32bpp
    graphics_palette
    png_draw_16bpp
    png_draw_32bpp
    menu
    radial_progress_bar
    runtime_allocate_16bpp
    all_widgets_execute
    all_widgets_1555xrgb
    text_rotation
    text_draw
    screen_drag_animation
    screen_drag_animation_with_canvas
    screen_drag_animation_with_block_move
    line_chart
    non_linear_animation
    widgets_with_private_string
    system_screen_stack
    template
    widget_find
    arabic_glyph_shaping
    farsi_glyph_shaping
    thai_glyph_shaping
    focus_management
    rich_text_view_16bpp
    all_widgets_565bgr
    glyph_draw
    dynamic_menu
    helloworld
    lines
    progress_bar
    two_displays
    bidi_text_line_break_static
    generic_scroll_wheel)
set(MOUSE_SUPPORT_DEMOS all_widgets_1bpp all_widgets_4bpp all_widgets_8bpp
                        pixelmap_16bpp pixelmap_32bpp)
set(FONT_KERNING_SUPPORT_DEMOS kerning_glyph_draw_32bpp_screen
                               kerning_glyph_in_string_table)
set(DYNAMIC_BIDI_TEXT_DEMOS bidi_text_draw_32bpp bidi_text_line_break_runtime)
set(_5_4_0_COMPATIBLE_DEMOS all_widgets_defined_5_4_0_compatibility)
set(SYNERGY_FONT_SUPPORT_DEMOS
    synergy_text_rotation
    all_widgets_synergy_16bpp_rotated_ccw
    all_widgets_synergy_16bpp_rotated_ccw_binary
    all_widgets_synergy_16bpp_rotated_cw
    all_widgets_synergy_16bpp_rotated_cw_binary)
set(THAI_GLYPH_SHAPING_SUPPORT_DEMOS thai_glyph_shaping_runtime)
set(PALETTE_MODE_AA_TEXT_COLORS_16_DEMOS
    glyph_draw_8bpp_screen_text_colors_16
    ml_text_view_vcenter)
set(PARTIAL_CANVAS_SUPPORT_DEMOS partial_frame_buffer_16bpp)

# Set directories for demos
set(EXAMPLE_INTERNAL_DIR ${ROOT_DIR}/test/example_internal)
foreach(
  demo
  ${NO_UTF8_DEMOS};${EXTENDED_UNICODE_DEMOS};${OTHER_DEMOS};${MOUSE_SUPPORT_DEMOS};${FONT_KERNING_SUPPORT_DEMOS};${DYNAMIC_BIDI_TEXT_DEMOS};
  ${_5_4_0_COMPATIBLE_DEMOS};
  ${SYNERGY_FONT_SUPPORT_DEMOS};
  ${THAI_GLYPH_SHAPING_SUPPORT_DEMOS};
  ${PALETTE_MODE_AA_TEXT_COLORS_16_DEMOS};
  ${PARTIAL_CANVAS_SUPPORT_DEMOS};)
  set(${demo}_SOURCE_DIRECTORY ${EXAMPLE_INTERNAL_DIR}/${demo})
endforeach()

# demos of examples
set(DEMO_IN_EXAMPLES
    demo_guix_calculator
    demo_guix_car_infotainment
    demo_guix_home_automation
    demo_guix_smart_watch
    demo_guix_industrial
    demo_guix_washing_machine
    demo_guix_medical
    demo_guix_widget_types
    demo_guix_keyboard
    demo_guix_shapes
    demo_guix_simple
    demo_guix_sprite
    demo_guix_thermostat)

# Set directories for demos
set(EXAMPLE_DIR ${ROOT_DIR}/samples)
foreach(demo ${DEMO_IN_EXAMPLES})
  set(${demo}_SOURCE_DIRECTORY ${EXAMPLE_DIR}/${demo})
endforeach()

# demos of tutorials
set(DEMO_IN_TUTORIALS
    demo_guix_binres
    demo_guix_binres_standalone
    demo_guix_circular_gauge
    demo_guix_drop_list
    demo_guix_menu
    demo_guix_ml_text_input
    demo_guix_radial_slider
    demo_guix_scroll_wheel
    demo_guix_scrollbar
    demo_guix_transitions
    demo_guix_vertical_list
    demo_guix_widget_animation)

# Set directories for demos
set(TUTORIALS_DIR ${ROOT_DIR}/tutorials)
foreach(demo ${DEMO_IN_TUTORIALS})
  set(${demo}_SOURCE_DIRECTORY ${TUTORIALS_DIR}/${demo})
endforeach()

# set demos of screenshot_to_bitmap
set(DEMO_SCREENSHOT_TO_BITMAP
    screenshot_to_bitmap_16bpp
    screenshot_to_bitmap_32bpp
    screenshot_to_bitmap_monochrome
    screenshot_to_bitmap_4444argb
    screenshot_to_bitmap_1555xrgb
    screenshot_to_bitmap_332rgb
    screenshot_to_bitmap_8bit_palette
    screenshot_to_bitmap_4bpp)

# Set directories for demos
set(SCREENSHOT_TO_BITMAP_DIR ${ROOT_DIR}/test/example_internal/screenshot_to_bitmap)
foreach(demo ${DEMO_SCREENSHOT_TO_BITMAP})
  set(${demo}_SOURCE_DIRECTORY ${SCREENSHOT_TO_BITMAP_DIR}/${demo})
endforeach()

# file list and regression tests for each demo
set(all_widgets_FILE_LIST
    all_widgets_resources.c
    all_widgets_specifications.c
    demo_guix_all_widgets.c
    gauge_screen.c
    sprite_screen.c
    scroll_wheel_screen.c
    rotate_screen.c)
set(all_widgets_REG_TESTS
    guix_image_reader_png_decode_no_output
    guix_error_checking_api_coverage_no_output
    guix_error_checking_api_compile_no_output # used for API compile, to link
                                              # every function.
    guix_gxe_api_error_checking_coverage_no_output
    guix_horizontal_gradient_no_output
    guix_all_widgets_canvas_copy_blend
    guix_all_widgets_display_color_set
    guix_all_widgets_horizontal_32bpp
    guix_all_widgets_progress_bar_32bpp
    guix_binres_language_table_load_no_output
    guix_button_24xrgb
    guix_button_deselect_no_output
    guix_canvas_drawing_complete
    guix_canvas_hardware_layer
    guix_canvas_pixelmap_blend
    guix_canvas_pixelmap_get_no_output
    guix_canvas_rotated_text_draw
    guix_canvas_rotated_text_draw_no_output
    guix_canvas_text_draw
    guix_canvas_text_draw_no_output
    guix_checkbox_24xrgb
    guix_circular_gauge_animation_set
    guix_context_brush_define_no_output
    guix_display_create_no_output
    guix_display_delete_no_output
    guix_display_driver_32bpp_block_move
    guix_display_language_table_set_no_output
    guix_display_string_table_get_no_output
    guix_display_string_get_no_output
    guix_display_active_language_set_no_output
    guix_horizontal_list_event_process_pen_up
    guix_horizontal_list_selected_widget_get_no_output
    guix_horizontal_scrollbar_create
    guix_icon_24xrgb
    guix_icon_button_24xrgb
    guix_multi_line_text_input_character_insert_24xrgb
    guix_multi_line_text_input_cursor_draw_24xrgb
    guix_multi_line_text_input_text_select
    guix_pixelmap_button_24xrgb
    guix_pixelmap_slider_24xrgb
    guix_pixelmap_slider_h_24xrgb
    guix_prompt_24xrgb
    guix_radio_button_24xrgb
    guix_screen_stack
    guix_scroll_wheel_callback_set_no_output
    guix_scrollbar_24xrgb
    guix_scrollbar_event_process
    guix_scrollbar_reset
    guix_scrollbar_thumb_24xrgb
    guix_single_line_partial_dirty_24xrgb
    guix_single_line_text_input_24xrgb
    guix_single_line_text_input_24xrgb_center_aligned
    guix_single_line_text_input_buffer_clear
    guix_single_line_text_input_create
    guix_single_line_text_input_fill_color_set
    guix_single_line_text_input_property_24xrgb
    guix_single_line_text_input_text_color_set
    guix_single_line_text_input_text_select
    guix_slider_24xrgb
    guix_slider_h_24xrgb
    guix_slider_needle_position_get
    guix_sprite
    guix_string_scroll_wheel_text_get
    guix_string_scroll_wheel_string_list_set_ext
    guix_text_scroll_wheel_callback_set
    guix_text_button_24xrgb
    guix_text_input_cursor_height_set
    guix_tree_view_root_pixelmap_set
    guix_tree_view_event_process
    guix_tree_view_show_event_process
    guix_tree_view_scroll
    guix_menu_event_process_no_output
    guix_menu_text_offset_set
    guix_menu_text_alignment
    guix_menu_position
    guix_ml_button_text_set_no_output
    guix_ml_input_char_insert_no_output
    guix_ml_input_text_set_no_output
    guix_ml_input_no_output
    guix_ml_view_text_set_no_output
    guix_ml_view_no_output
    guix_multi_line_text_view_24xrgb
    guix_multi_line_text_input_down_arrow
    guix_multi_line_text_input_event_process
    guix_multi_line_text_input_text_color_set
    guix_multi_line_text_input_fill_color_set
    guix_multi_line_text_input_left_arrow
    guix_multi_line_text_input_up_arrow
    guix_numeric_pixelmap_prompt
    guix_numeric_prompt
    guix_numeric_prompt_value_set_no_output
    guix_numeric_pixelmap_prompt_value_set_no_output
    guix_drop_list_24xrgb
    guix_drop_list_open
    guix_all_widgets_gx_system_string_width_get_no_output
    guix_all_widgets_scroll_wheel_32bpp
    guix_all_widgets_scroll_wheel_flick_event_process
    guix_all_widgets_scroll_wheel_select
    guix_scroll_wheel_timer_event_process
    guix_scroll_wheel_flick_event_process
    guix_scroll_wheel_scroll
    guix_scroll_wheel_gradient_create_no_output
    guix_text_scroll_wheel_round_draw
    guix_text_scroll_wheel_flat_draw
    guix_text_scroll_wheel_draw_with_alpha
    guix_all_widgets_accordion_menu
    guix_accordion_menu_draw
    guix_accordion_menu_pen_down_event_process
    guix_accordion_menu_pen_up_event_process
    guix_accordion_menu_position
    guix_accordion_menu_shift_no_output
    guix_all_widgets_tree_view
    guix_all_widgets_rotate_screen
    guix_all_widgets_block_move
    guix_all_widgets_gauge_animation
    guix_all_widgets_button_repeat
    guix_all_widgets_button_toggle
    guix_all_widgets_button_event_process
    guix_all_widgets_checkbox_draw
    guix_all_widgets_drop_list_background_draw
    guix_all_widgets_drop_list_event_process
    guix_all_widgets_drop_list_pixelmap_set
    guix_all_widgets_radio_button_draw
    guix_all_widgets_icon_alignment
    guix_all_widgets_icon_button_alignment
    guix_all_widgets_pixelmap_button_alignment
    guix_all_widgets_root_window_operation
    guix_all_widgets_sl_text_input_buffer_get
    guix_all_widgets_slider_create
    guix_all_widgets_sprite_frame
    guix_sprite_start
    guix_all_widgets_text_button_text_get
    guix_all_widgets_ml_text_button
    guix_all_widgets_multi_line_text_input_style_add
    guix_all_widgets_multi_line_text_view_scroll
    guix_all_widgets_widget_border_style_set
    guix_all_widgets_widget_focus
    guix_all_widgets_ml_input_cursor_animation
    guix_all_widgets_progress_bar_background_draw
    guix_all_widgets_progress_bar_info_set
    guix_all_widgets_progress_bar_value_set
    guix_all_widgets_root_window_move
    guix_all_widgets_scroll_wheel_event_process
    guix_all_widgets_single_line_text_input_style_add
    guix_all_widgets_single_line_text_input_mark
    guix_all_widgets_system_view_fold
    guix_all_widgets_fold_drag_event
    guix_all_widgets_utility_vertical_gradient_create
    guix_all_widgets_widget_color_get
    guix_all_widgets_widget_event_process
    guix_all_widgets_widget_move
    guix_all_widgets_widget_style_set
    guix_all_widgets_widget_text_blend
    guix_canvas_delete_no_output
    guix_circular_gauge_angle_increment_calculate
    guix_context_string_get_no_output
    guix_display_language_table_get_no_output
    guix_string_scroll_wheel_create_no_output
    guix_string_scroll_wheel_event_process_no_output
    guix_string_scroll_wheel_string_list_set_no_output
    guix_system_canvas_refresh
    guix_system_clipboard_put
    guix_system_dirty_list_remove
    guix_system_view_fold_no_output
    guix_system_thread_entry_no_output
    guix_system_event_dispatch_no_output
    guix_system_event_fold_no_output
    guix_system_event_fold_pen_drag_no_output
    guix_system_event_remove_no_output
    guix_system_focus_claim_no_output
    guix_system_free_view_get_no_output
    guix_system_lock_no_output
    guix_system_no_output
    guix_system_private_string
    guix_system_private_string_copy_no_output
    guix_system_private_string_list_copy
    guix_system_private_string_delete_no_output
    guix_system_timer_update_no_output
    guix_system_screen_stack_no_output
    guix_system_string_width_get_no_output
    guix_system_version_string_get_no_output
    guix_text_button_event_process_no_output
    guix_text_button_text_get_no_output
    guix_utility_utf8_string_character
    guix_utility_math_sqrt_no_output
    guix_text_button_text_set_no_output
    guix_text_scroll_wheel_event_process_no_output
    guix_widget_back_attach
    guix_widget_back_link
    guix_widget_back_move
    guix_widget_block_move
    guix_widget_border_draw
    guix_widget_canvas_get_no_output
    guix_widget_children_draw
    guix_widget_child_focus_assign_no_output
    guix_widget_delete_no_output
    guix_widget_link
    guix_widget_no_output
    guix_widget_font_get_no_output
    guix_widget_pixelmap_get
    guix_widget_style_set
    guix_widget_transparent_pixelmap_detect_no_output
    guix_widget_find_no_output
    guix_widget_string_get_no_output
    guix_widget_text_blend_no_output
    guix_widget_text_draw
    guix_window_client_scroll
    guix_window_client_scroll_api
    guix_window_no_output
    guix_window_event_process_no_output
    guix_tree_view_selected_set
    guix_tree_view_position
    guix_circular_gauge_angle_set
    guix_circular_gauge_dirty_mark
    guix_circular_gauge_32bpp
    guix_circular_gauge_needle_rotate_callback
    guix_all_widgets_radial_slider
    guix_all_widgets_radial_slider_property
    guix_all_widgets_radial_slider_needle_offset
    guix_radial_slider_anchor_angle_calculate
    guix_radial_slider_angle_calculate
    guix_radial_slider_event_process
    guix_pixelmap_prompt_background_draw
    guix_pixelmap_slider_pixelmap_set
    guix_pixelmap_slider_event_process
    guix_prompt_event_process_no_output
    guix_prompt_text_get_no_output
    guix_prompt_text_set_no_output
    guix_circular_gauge_event_process
    guix_root_window_focus_no_output
    guix_scroll_thumb_event_process
    guix_scrollbar_value_calculate
    guix_scrollbar_value_set
    guix_sl_input_text_set_no_output
    guix_sl_input_no_output
    guix_display_driver_32bpp_pixelmap_rotate
    guix_utility_32bpp_pixelmap_rotate
    guix_utility_string_to_alphamap
    guix_string_to_alphamap_no_output
    guix_deprecated_system_api_no_output)

set(all_widgets_32argb_FILE_LIST
    all_widgets_32argb_resources.c
    all_widgets_32argb_specifications.c
    demo_guix_all_widgets_32argb.c
    gauge_screen.c
    sprite_screen.c
    scroll_wheel_screen.c
    rotate_screen.c)

set(all_widgets_32argb_REG_TESTS guix_all_widgets_32argb_pixelmap_draw)

set(all_widgets_16bpp_FILE_LIST
    all_widgets_16bpp_resources.c
    all_widgets_16bpp_specifications.c
    demo_guix_all_widgets_16bpp.c
    rotate_screen.c
    shapes_screen.c
    scroll_wheel_screen.c
    lines_screen.c
    gauge_screen.c)

set(all_widgets_16bpp_REG_TESTS
    guix_all_widgets_canvas_copy_blend_565rgb
    guix_all_widgets_h_pixelmap_slider_16bpp
    guix_all_widgets_h_slider_16bpp
    guix_all_widgets_ml_text_input_16bpp
    guix_all_widgets_ml_text_input_cursor_draw_16bpp
    guix_all_widgets_ml_text_view_16bpp
    guix_all_widgets_scrollbar_16bpp
    guix_all_widgets_sl_text_input_16bpp
    guix_all_widgets_sl_text_input_property_16bpp
    guix_all_widgets_v_pixelmap_slider_16bpp
    guix_all_widgets_v_slider_16bpp
    guix_all_widgets_16bpp_shapes_screen_pie
    guix_all_widgets_565rgb_rotate_screen
    guix_all_widgets_565rgb_pixelmap_rotate
    guix_all_widgets_16bpp_block_move
    guix_lines_565rgb
    guix_565rgb_simple_line_draw
    guix_all_widgets_16bpp_canvas_animation
    guix_all_widgets_16bpp_canvas_offset_set
    guix_all_widgets_16bpp_fill_text_color_test
    guix_display_driver_alised_wide_line_draw
    guix_display_driver_aliased_fixed_point_line_draw
    guix_display_driver_aliased_filled_circle_draw
    guix_display_driver_wide_line_fill
    guix_display_driver_16bpp_block_move
    guix_canvas_pixelmap_rotate
    guix_canvas_hide
    guix_animation_complete
    guix_animation_complete_push_stack
    guix_animation_delete_no_output
    guix_animation_update
    guix_animation_stop
    guix_16bpp_horizontal_pixelmap_line_draw
    guix_all_widgets_16bpp_generic_shape_fill
    guix_display_driver_16bpp_pixelmap_rotate
    guix_utility_16bpp_pixelmap_rotate
    guix_circular_gauge_16bpp
    guix_all_widgets_16bpp_sprite)

set(all_widgets_8bpp_rotated_ccw_FILE_LIST
    all_widgets_8bpp_rotated_ccw_resources.c
    all_widgets_8bpp_rotated_ccw_specifications.c
    demo_guix_all_widgets_8bpp_rotated_ccw.c
    gauge_screen.c
    lines_screen.c
    resize_screen.c
    rotate_screen.c
    scroll_wheel_screen.c
    shapes_screen.c)

set(all_widgets_8bpp_rotated_ccw_REG_TESTS
    guix_all_widgets_8bpp_rotated_ccw
    guix_all_widgets_8bpp_rotated_ccw_block_move
    guix_all_widgets_8bpp_rotated_ccw_canvas_copy
    guix_all_widgets_8bpp_rotated_ccw_line_draw
    guix_all_widgets_8bpp_rotated_ccw_pixelmap_draw
    guix_all_widgets_8bpp_rotated_ccw_pixelmap_fill
    guix_all_widgets_8bpp_rotated_ccw_resize_screen
    guix_all_widgets_8bpp_rotated_ccw_rotate_screen)

set(all_widgets_8bpp_rotated_ccw_binary_FILE_LIST
    all_widgets_8bpp_rotated_ccw_binary_specifications.c
    demo_guix_all_widgets_8bpp_rotated_ccw_binary.c
    gauge_screen.c
    lines_screen.c
    resize_screen.c
    rotate_screen.c
    scroll_wheel_screen.c
    shapes_screen.c)

set(all_widgets_8bpp_rotated_ccw_binary_REG_TESTS
    guix_all_widgets_8bpp_rotated_ccw_binary)

set(all_widgets_8bpp_rotated_cw_FILE_LIST
    all_widgets_8bpp_rotated_cw_resources.c
    all_widgets_8bpp_rotated_cw_specifications.c
    demo_guix_all_widgets_8bpp_rotated_cw.c
    gauge_screen.c
    lines_screen.c
    resize_screen.c
    rotate_screen.c
    scroll_wheel_screen.c
    shapes_screen.c)

set(all_widgets_8bpp_rotated_cw_REG_TESTS
    guix_all_widgets_8bpp_rotated_cw
    guix_all_widgets_8bpp_rotated_cw_block_move
    guix_all_widgets_8bpp_rotated_cw_canvas_copy
    guix_all_widgets_8bpp_rotated_cw_line_draw
    guix_all_widgets_8bpp_rotated_cw_pixelmap_draw
    guix_all_widgets_8bpp_rotated_cw_pixelmap_fill
    guix_all_widgets_8bpp_rotated_cw_resize_screen
    guix_all_widgets_8bpp_rotated_cw_rotate_screen
    guix_all_widgets_8bpp_rotated_cw_vertical_list
    guix_all_widgets_8bpp_rotated_cw_glyph_draw
    guix_all_widgets_8bpp_rotated_cw_horizontal_pixelmap_line_draw)

set(all_widgets_8bpp_rotated_cw_binary_FILE_LIST
    all_widgets_8bpp_rotated_cw_binary_specifications.c
    demo_guix_all_widgets_8bpp_rotated_cw_binary.c
    gauge_screen.c
    lines_screen.c
    resize_screen.c
    rotate_screen.c
    scroll_wheel_screen.c
    shapes_screen.c)

set(all_widgets_8bpp_rotated_cw_binary_REG_TESTS
    guix_all_widgets_8bpp_rotated_cw_binary)

set(all_widgets_synergy_8bpp_rotated_ccw_FILE_LIST
    all_widgets_synergy_8bpp_rotated_ccw_resources.c
    all_widgets_synergy_8bpp_rotated_ccw_specifications.c
    demo_guix_all_widgets_synergy_8bpp_rotated_ccw.c
    gauge_screen.c
    lines_screen.c
    resize_screen.c
    rotate_screen.c
    scroll_wheel_screen.c
    shapes_screen.c)

set(all_widgets_synergy_8bpp_rotated_ccw_REG_TESTS
    guix_all_widgets_synergy_8bpp_rotated_ccw)

set(all_widgets_synergy_8bpp_rotated_ccw_binary_FILE_LIST
    all_widgets_synergy_8bpp_rotated_ccw_binary_specifications.c
    demo_guix_all_widgets_synergy_8bpp_rotated_ccw_binary.c
    gauge_screen.c
    lines_screen.c
    resize_screen.c
    rotate_screen.c
    scroll_wheel_screen.c
    shapes_screen.c)

set(all_widgets_synergy_8bpp_rotated_ccw_binary_REG_TESTS
    guix_all_widgets_synergy_8bpp_rotated_ccw_binary
    guix_all_widgets_synergy_8bpp_rotated_ccw_binary_sprite)

set(all_widgets_synergy_8bpp_rotated_cw_FILE_LIST
    all_widgets_synergy_8bpp_rotated_cw_resources.c
    all_widgets_synergy_8bpp_rotated_cw_specifications.c
    demo_guix_all_widgets_synergy_8bpp_rotated_cw.c
    gauge_screen.c
    lines_screen.c
    resize_screen.c
    rotate_screen.c
    scroll_wheel_screen.c
    shapes_screen.c)

set(all_widgets_synergy_8bpp_rotated_cw_REG_TESTS
    guix_all_widgets_synergy_8bpp_rotated_cw
    guix_all_widgets_synergy_8bpp_rotated_cw_vertical_list)

set(all_widgets_synergy_8bpp_rotated_cw_binary_FILE_LIST
    all_widgets_synergy_8bpp_rotated_cw_binary_specifications.c
    demo_guix_all_widgets_synergy_8bpp_rotated_cw_binary.c
    gauge_screen.c
    lines_screen.c
    resize_screen.c
    rotate_screen.c
    scroll_wheel_screen.c
    shapes_screen.c)

set(all_widgets_synergy_8bpp_rotated_cw_binary_REG_TESTS
    guix_all_widgets_synergy_8bpp_rotated_cw_binary)

set(all_widgets_synergy_8bpp_rotated_cw_dave2d_disabled_FILE_LIST
    all_widgets_synergy_8bpp_rotated_cw_dave2d_disabled_resources.c
    all_widgets_synergy_8bpp_rotated_cw_dave2d_disabled_specifications.c
    demo_guix_all_widgets_synergy_8bpp_rotated_cw_dave2d_disabled.c
    gauge_screen.c
    lines_screen.c
    resize_screen.c
    rotate_screen.c
    scroll_wheel_screen.c
    shapes_screen.c)

set(all_widgets_synergy_8bpp_rotated_cw_dave2d_disabled_REG_TESTS
    guix_all_widgets_synergy_8bpp_rotated_cw_dave2d_disabled)

set(all_widgets_24xrgb_rotated_ccw_FILE_LIST
    all_widgets_24xrgb_rotated_ccw_resources.c
    all_widgets_24xrgb_rotated_ccw_specifications.c
    demo_guix_all_widgets_24xrgb_rotated_ccw.c
    gauge_screen.c
    lines_screen.c
    resize_screen.c
    rotate_screen.c
    scroll_wheel_screen.c
    shapes_screen.c)

set(all_widgets_24xrgb_rotated_ccw_REG_TESTS
    guix_all_widgets_24xrgb_rotated_ccw_block_move
    guix_all_widgets_24xrgb_rotated_ccw_canvas_copy_blend
    guix_all_widgets_24xrgb_rotated_ccw_glyph_draw
    guix_all_widgets_24xrgb_rotated_ccw_line_draw
    guix_all_widgets_24xrgb_rotated_ccw_pixelmap_draw
    guix_all_widgets_24xrgb_rotated_ccw_resize_screen
    guix_all_widgets_24xrgb_rotated_ccw_rotate_screen
    guix_all_widgets_24xrgb_rotated_ccw_shapes_screen
    guix_all_widgets_24xrgb_rotated_ccw_vertical_list
    guix_all_widgets_24xrgb_rotated_ccw)

set(all_widgets_24xrgb_rotated_ccw_binary_FILE_LIST
    all_widgets_24xrgb_rotated_ccw_binary_specifications.c
    demo_guix_all_widgets_24xrgb_rotated_ccw_binary.c
    gauge_screen.c
    lines_screen.c
    resize_screen.c
    rotate_screen.c
    scroll_wheel_screen.c
    shapes_screen.c)


set(all_widgets_24xrgb_rotated_ccw_binary_REG_TESTS
    guix_all_widgets_24xrgb_rotated_ccw_binary)

set(all_widgets_24xrgb_rotated_cw_FILE_LIST
    all_widgets_24xrgb_rotated_cw_resources.c
    all_widgets_24xrgb_rotated_cw_specifications.c
    demo_guix_all_widgets_24xrgb_rotated_cw.c
    gauge_screen.c
    lines_screen.c
    resize_screen.c
    rotate_screen.c
    scroll_wheel_screen.c
    shapes_screen.c)

set(all_widgets_24xrgb_rotated_cw_REG_TESTS
    guix_all_widgets_24xrgb_rotated_cw_block_move
    guix_all_widgets_24xrgb_rotated_cw_canvas_copy_blend
    guix_all_widgets_24xrgb_rotated_cw_glyph_draw
    guix_all_widgets_24xrgb_rotated_cw_line_draw
    guix_all_widgets_24xrgb_rotated_cw_pixelmap_draw
    guix_all_widgets_24xrgb_rotated_cw_resize_screen
    guix_all_widgets_24xrgb_rotated_cw_rotate_screen
    guix_all_widgets_24xrgb_rotated_cw_shapes_screen
    guix_all_widgets_24xrgb_rotated_cw_vertical_list
    guix_all_widgets_24xrgb_rotated_cw
    guix_all_widgets_24xrgb_rotated_cw_horizontal_pixelmap_line_draw
    guix_all_widgets_24xrgb_rotated_cw_sprite)

set(all_widgets_24xrgb_rotated_cw_binary_FILE_LIST
    all_widgets_24xrgb_rotated_cw_binary_specifications.c
    demo_guix_all_widgets_24xrgb_rotated_cw_binary.c
    gauge_screen.c
    lines_screen.c
    resize_screen.c
    rotate_screen.c
    scroll_wheel_screen.c
    shapes_screen.c)

set(all_widgets_24xrgb_rotated_cw_binary_REG_TESTS
    guix_all_widgets_24xrgb_rotated_cw_binary)

set(all_widgets_synergy_24xrgb_rotated_ccw_FILE_LIST
    all_widgets_synergy_24xrgb_rotated_ccw_resources.c
    all_widgets_synergy_24xrgb_rotated_ccw_specifications.c
    demo_guix_all_widgets_synergy_24xrgb_rotated_ccw.c
    gauge_screen.c
    lines_screen.c
    resize_screen.c
    rotate_screen.c
    scroll_wheel_screen.c
    shapes_screen.c)

set(all_widgets_synergy_24xrgb_rotated_ccw_REG_TESTS
    guix_all_widgets_synergy_24xrgb_rotated_ccw)

set(all_widgets_synergy_24xrgb_rotated_ccw_binary_FILE_LIST
    all_widgets_synergy_24xrgb_rotated_ccw_binary_specifications.c
    demo_guix_all_widgets_synergy_24xrgb_rotated_ccw_binary.c
    gauge_screen.c
    lines_screen.c
    resize_screen.c
    rotate_screen.c
    scroll_wheel_screen.c
    shapes_screen.c)

set(all_widgets_synergy_24xrgb_rotated_ccw_binary_REG_TESTS
    guix_all_widgets_synergy_24xrgb_rotated_ccw_binary)

set(all_widgets_synergy_24xrgb_rotated_cw_FILE_LIST
    all_widgets_synergy_24xrgb_rotated_cw_resources.c
    all_widgets_synergy_24xrgb_rotated_cw_specifications.c
    demo_guix_all_widgets_synergy_24xrgb_rotated_cw.c
    gauge_screen.c
    lines_screen.c
    resize_screen.c
    rotate_screen.c
    scroll_wheel_screen.c
    shapes_screen.c)

set(all_widgets_synergy_24xrgb_rotated_cw_REG_TESTS
    guix_all_widgets_synergy_24xrgb_rotated_cw_glyph_draw
    guix_all_widgets_synergy_24xrgb_rotated_cw_pixelmap_draw
    guix_all_widgets_synergy_24xrgb_rotated_cw_vertical_list
    guix_all_widgets_synergy_24xrgb_rotated_cw)

set(all_widgets_synergy_24xrgb_rotated_cw_binary_FILE_LIST
    all_widgets_synergy_24xrgb_rotated_cw_binary_specifications.c
    demo_guix_all_widgets_synergy_24xrgb_rotated_cw_binary.c
    gauge_screen.c
    lines_screen.c
    resize_screen.c
    rotate_screen.c
    scroll_wheel_screen.c
    shapes_screen.c)

set(all_widgets_synergy_24xrgb_rotated_cw_binary_REG_TESTS
    guix_all_widgets_synergy_24xrgb_rotated_cw_binary)

set(all_widgets_synergy_24xrgb_rotated_cw_dave2d_disabled_FILE_LIST
    all_widgets_synergy_24xrgb_rotated_cw_dave2d_disabled_resources.c
    all_widgets_synergy_24xrgb_rotated_cw_dave2d_disabled_specifications.c
    demo_guix_all_widgets_synergy_24xrgb_rotated_cw_dave2d_disabled.c
    gauge_screen.c
    lines_screen.c
    resize_screen.c
    rotate_screen.c
    scroll_wheel_screen.c
    shapes_screen.c)

set(all_widgets_synergy_24xrgb_rotated_cw_dave2d_disabled_REG_TESTS
    guix_all_widgets_synergy_24xrgb_rotated_cw_dave2d_disabled)

set(all_widgets_synergy_16bpp_flip_FILE_LIST
    all_widgets_synergy_16bpp_flip_specifications.c
    all_widgets_synergy_16bpp_flip_resources.c
    demo_guix_all_widgets_synergy_16bpp_flip.c
    sprite_screen.c)

set(all_widgets_synergy_16bpp_flip_REG_TESTS
    guix_all_widgets_synergy_16bpp_flip)

set(all_widgets_32argb_rotated_cw_FILE_LIST
    all_widgets_32argb_rotated_cw_resources.c
    all_widgets_32argb_rotated_cw_specifications.c
    demo_guix_all_widgets_32argb_rotated_cw.c
    gauge_screen.c
    lines_screen.c
    resize_screen.c
    rotate_screen.c
    scroll_wheel_screen.c
    shapes_screen.c)

set(all_widgets_32argb_rotated_cw_REG_TESTS
    guix_all_widgets_32argb_rotated_cw_canvas_copy_blend
    guix_all_widgets_32argb_rotated_cw_glyph_draw
    guix_all_widgets_32argb_rotated_cw_line_draw
    guix_all_widgets_32argb_rotated_cw_pixelmap_draw
    guix_all_widgets_32argb_rotated_cw_resize_screen
    guix_all_widgets_32argb_rotated_cw_rotate_screen
    guix_all_widgets_32argb_rotated_cw_shapes_screen
    guix_all_widgets_32argb_rotated_cw)

set(all_widgets_32argb_rotated_ccw_FILE_LIST
    all_widgets_32argb_rotated_ccw_resources.c
    all_widgets_32argb_rotated_ccw_specifications.c
    demo_guix_all_widgets_32argb_rotated_ccw.c
    gauge_screen.c
    lines_screen.c
    resize_screen.c
    rotate_screen.c
    scroll_wheel_screen.c
    shapes_screen.c)

set(all_widgets_32argb_rotated_ccw_REG_TESTS
    guix_all_widgets_32argb_rotated_ccw)

set(all_widgets_16bpp_rotated_FILE_LIST
    all_widgets_16bpp_rotated_resources.c
    all_widgets_16bpp_rotated_specifications.c
    demo_guix_all_widgets_16bpp_rotated.c
    gauge_screen.c
    lines_screen.c
    resize_screen.c
    rotate_screen.c
    scroll_wheel_screen.c
    shapes_screen.c)

set(all_widgets_16bpp_rotated_REG_TESTS
    guix_all_widgets_16bpp_rotated_canvas_copy_blend
    guix_all_widgets_16bpp_rotated_rotate_screen
    guix_all_widgets_16bpp_rotated_resize_screen
    guix_all_widgets_16bpp_rotated_pixelmap_draw
    guix_all_widgets_16bpp_rotated_shapes_screen
    guix_all_widgets_16bpp_rotated_line_draw
    guix_all_widgets_16bpp_rotated_block_move
    guix_all_widgets_16bpp_rotated_glyph_draw
    guix_all_widgets_16bpp_rotated_arc_fill
    guix_all_widgets_16bpp_rotated_circle_fill
    guix_all_widgets_16bpp_rotated_ellipse_fill
    guix_all_widgets_16bpp_rotated_pie_fill
    guix_all_widgets_16bpp_rotated_polygon_fill
    guix_all_widgets_16bpp_rotated_wide_line_fill
    guix_all_widgets_16bpp_rotated_scroll_wheel_screen
    guix_all_widgets_16bpp_rotated_vertical_list
    guix_all_widgets_16bpp_rotated_horizontal_pixelmap_line_draw
    guix_all_widgets_16bpp_rotated_sprite)

set(all_widgets_16bpp_rotated_270_FILE_LIST
    all_widgets_16bpp_rotated_270_resources.c
    all_widgets_16bpp_rotated_270_specifications.c
    demo_guix_all_widgets_16bpp_rotated_270.c
    gauge_screen.c
    lines_screen.c
    resize_screen.c
    rotate_screen.c
    scroll_wheel_screen.c
    shapes_screen.c)

set(all_widgets_16bpp_rotated_270_REG_TESTS
    guix_all_widgets_16bpp_rotated_270_canvas_copy_blend
    guix_all_widgets_16bpp_rotated_270_rotate_screen
    guix_all_widgets_16bpp_rotated_270_resize_screen
    guix_all_widgets_16bpp_rotated_270_pixelmap_draw
    guix_all_widgets_16bpp_rotated_270_shapes_screen
    guix_all_widgets_16bpp_rotated_270_line_draw
    guix_all_widgets_16bpp_rotated_270_block_move)

set(all_widgets_16bpp_rotated_cw_binary_FILE_LIST
    all_widgets_16bpp_rotated_cw_binary_specifications.c
    demo_guix_all_widgets_16bpp_rotated_cw_binary.c
    gauge_screen.c
    lines_screen.c
    resize_screen.c
    rotate_screen.c
    scroll_wheel_screen.c
    shapes_screen.c)

set(all_widgets_16bpp_rotated_cw_binary_REG_TESTS
    guix_all_widgets_16bpp_rotated_cw_binary
    guix_all_widgets_16bpp_rotated_cw_binary_rotate_screen
    guix_all_widgets_16bpp_rotated_cw_binary_pixelmap_draw)

set(all_widgets_16bpp_rotated_ccw_binary_FILE_LIST
    all_widgets_16bpp_rotated_ccw_binary_specifications.c
    demo_guix_all_widgets_16bpp_rotated_ccw_binary.c
    gauge_screen.c
    lines_screen.c
    resize_screen.c
    rotate_screen.c
    scroll_wheel_screen.c
    shapes_screen.c)

set(all_widgets_16bpp_rotated_ccw_binary_REG_TESTS
    guix_all_widgets_16bpp_rotated_ccw_binary
    guix_all_widgets_16bpp_rotated_ccw_binary_rotate_screen
    guix_all_widgets_16bpp_rotated_ccw_binary_pixelmap_draw)

set(all_widgets_565bgr_FILE_LIST
    all_widgets_565bgr_resources.c
    all_widgets_565bgr_specifications.c
    demo_guix_all_widgets_565bgr.c
    rotate_screen.c
    resize_screen.c
    shapes_screen.c
    lines_screen.c
    gauge_screen.c)

set(all_widgets_565bgr_REG_TESTS
    guix_all_widgets_565bgr guix_all_widgets_565bgr_gauge_screen
    guix_all_widgets_565bgr_resize_screen guix_all_widgets_565bgr_rotate_screen)

set(glyph_draw_FILE_LIST glyph_draw_resources.c glyph_draw_specifications.c
                         demo_guix_glyph_draw.c)

set(glyph_draw_REG_TESTS guix_glyph_draw)

set(all_widgets_1555xrgb_FILE_LIST
    all_widgets_1555xrgb_resources.c
    all_widgets_1555xrgb_specifications.c
    demo_guix_all_widgets_1555xrgb.c
    rotate_screen.c
    shapes_screen.c
    scroll_wheel_screen.c
    image_convert_screen.c
    resize_screen.c)

set(all_widgets_1555xrgb_REG_TESTS
    guix_all_widgets_1555xrgb_canvas_copy_blend
    guix_all_widgets_1555xrgb_shapes_screen
    guix_all_widgets_1555xrgb_rotate_screen
    guix_all_widgets_1555xrgb_image_convert_screen
    guix_all_widgets_1555xrgb_resize_screen
    guix_all_widgets_1555xrgb_jpeg_draw
    guix_all_widgets_1555xrgb_png_draw
    guix_all_widgets_1555xrgb_pixelmap_draw
    guix_display_driver_1555xrgb_pixelmap_rotate
    guix_utility_1555xrgb_pixelmap_resize
    guix_utility_1555xrgb_pixelmap_rotate)
set(all_widgets_1bpp_FILE_LIST
    all_widgets_1bpp_resources.c
    all_widgets_1bpp_specifications.c
    demo_guix_all_widgets_1bpp.c
    rotate_screen.c
    shapes_screen.c
    sprite_screen.c
    scroll_wheel_screen.c
    image_convert_screen.c
    resize_screen.c)

set(all_widgets_1bpp_REG_TESTS
    guix_all_widgets_1bpp_text_screen
    guix_all_widgets_1bpp_pic_screen
    guix_all_widgets_1bpp_button_screen
    guix_all_widgets_1bpp_sliders_screen
    guix_all_widgets_1bpp_rotate_screen
    guix_all_widgets_1bpp_shapes_screen_polygon
    guix_all_widgets_1bpp_image_convert_screen
    guix_all_widgets_1bpp_block_move
    guix_all_widgets_1bpp_resize_screen
    guix_all_widgets_1bpp_canvas_copy
    guix_canvas_ellipse_draw
    guix_canvas_pie_draw
    guix_canvas_arc_draw
    guix_display_driver_1bpp_pixelmap_rotate
    guix_utility_1bpp_pixelmap_resize
    guix_utility_1bpp_pixelmap_rotate
    guix_1bpp_glyph_draw
    guix_1bpp_pixelmap_draw
    guix_1bpp_pixelmap_line_draw
    guix_1bpp_horizontal_pixelmap_line_draw
    guix_image_reader_colorspace_convert_1bpp)

set(all_widgets_332rgb_FILE_LIST
    all_widgets_332rgb_resources.c
    all_widgets_332rgb_specifications.c
    demo_all_widgets_332rgb.c
    scroll_wheel_screen.c
    rotate_screen.c
    shapes_screen.c
    sprite_screen.c
    resize_screen.c
    line_screen.c)
set(all_widgets_332rgb_REG_TESTS
    guix_all_widgets_332rgb_button_screen
    guix_all_widgets_332rgb_pixelmap_screen
    guix_all_widgets_332rgb_rotate_screen
    guix_all_widgets_332rgb_shapes_screen_circles
    guix_all_widgets_332rgb_shapes_screen_ellipse
    guix_all_widgets_332rgb_sprite_screen
    guix_all_widgets_332rgb_window_screen
    guix_all_widgets_332rgb_resize_screen
    guix_all_widgets_332rgb_line_screen
    guix_332rgb_simple_line_alpha_draw
    guix_all_widgets_332rgb_pixelmap_blend
    guix_utility_332rgb_pixelmap_resize
    guix_332rgb_glyph_1bit_draw
    guix_332rgb_glyph_4bit_draw
    guix_332rgb_horizontal_pixelmap_line_draw
    guix_332rgb_pixelmap_blend
    guix_display_driver_332rgb_pixelmap_rotate
    guix_utility_332rgb_pixelmap_rotate)

set(all_widgets_4444argb_FILE_LIST
    all_widgets_4444argb_resources.c
    all_widgets_4444argb_specifications.c
    demo_guix_all_widgets_4444argb.c
    rotate_screen.c
    shapes_screen.c
    sprite_screen.c
    scroll_wheel_screen.c
    image_convert_screen.c
    resize_screen.c)
set(all_widgets_4444argb_REG_TESTS
    guix_all_widgets_canvas_copy_blend_4444argb
    guix_all_widgets_4444argb_shapes_screen_arc
    guix_all_widgets_4444argb_rotate_screen
    guix_all_widgets_4444argb_image_convert_screen
    guix_all_widgets_4444argb_resize_screen
    guix_utility_4444argb_pixelmap_resize
    guix_utility_4444argb_pixelmap_rotate
    guix_display_driver_4444argb_pixelmap_rotate
    guix_4444argb_horizontal_pixelmap_line_draw)

set(all_widgets_4bpp_FILE_LIST
    all_widgets_4bpp_resources.c
    all_widgets_4bpp_specifications.c
    demo_guix_all_widgets_4bpp.c
    rotate_screen.c
    shapes_screen.c
    sprite_screen.c
    line_screen.c
    scroll_wheel_screen.c
    image_convert_screen.c
    resize_screen.c)
set(all_widgets_4bpp_REG_TESTS
    guix_all_widgets_4bpp_button_screen
    guix_all_widgets_4bpp_rotate_screen
    guix_all_widgets_4bpp_shapes_screen_ellipse
    guix_all_widgets_4bpp_sliders_screen
    guix_all_widgets_4bpp_sprite_screen
    guix_all_widgets_4bpp_window_screen
    guix_all_widgets_4bpp_horizontal_pixelmap_line_draw
    guix_all_widgets_4bpp_image_convert_screen
    guix_all_widgets_4bpp_block_move
    guix_all_widgets_4bpp_resize_screen
    guix_all_widgets_4bpp_text_draw
    guix_all_widgets_4bpp_line_screen
    guix_all_widgets_4bpp_generic_arc_fill
    guix_4bpp_simple_line_draw
    guix_4bpp_pixelmap_draw
    guix_4bpp_canvas_copy
    guix_4bpp_glyph_1bit_draw
    guix_4bpp_glyph_4bit_draw
    guix_4bpp_horizontal_pixelmap_line_draw
    guix_4bpp_pixelmap_rotate
    guix_display_driver_4bpp_pixelmap_rotate
    guix_utility_4bpp_pixelmap_resize
    guix_utility_4bpp_pixelmap_rotate
    guix_image_reader_colorspace_convert_4bpp
    guix_all_widgets_4bpp_sprite)

set(all_widgets_8bpp_FILE_LIST
    all_widgets_8bpp_resources.c all_widgets_8bpp_specifications.c
    demo_guix_all_widgets_8bpp.c scroll_wheel_screen.c rotate_screen.c)
set(all_widgets_8bpp_REG_TESTS
    guix_all_widgets_canvas_copy_8bit_palette
    guix_all_widgets_scrollbar_8bpp
    guix_all_widgets_8bpp_rotate_screen
    guix_all_widgets_8bpp_block_move
    guix_display_driver_8bpp_block_move
    guix_display_driver_8bpp_pixelmap_rotate
    guix_utility_8bpp_pixelmap_rotate
    guix_8bit_palette_glyph_1bit_draw)
set(all_widgets_execute_FILE_LIST
    all_widgets_execute_resources.c all_widgets_execute_specifications.c
    demo_guix_all_widgets_execute.c gauge_screen.c scroll_wheel_screen.c
    sprite_screen.c)

set(all_widgets_execute_REG_TESTS guix_all_widgets_execute guix_window_execute)

set(text_rotation_FILE_LIST text_rotation_resources.c
                            text_rotation_specifications.c demo_text_rotation.c)

set(text_rotation_REG_TESTS guix_text_rotation
                            guix_utility_1bpp_string_to_alphamap)

set(text_draw_FILE_LIST
    text_draw_resources.c
    text_draw_specifications.c
    demo_text_draw.c)

set(text_draw_REG_TESTS
    guix_text_draw)

set(alphamap_16bppdriver_FILE_LIST
    alphamap_16bppdriver_resources.c alphamap_16bppdriver_specifications.c
    demo_alphamap_16bppdriver.c)
set(alphamap_16bppdriver_REG_TESTS guix_alphamap_16bppdriver)

set(alphamap_16bpp_FILE_LIST demo_alphamap_16bpp.c alphamap_16bpp_resources.c
                             alphamap_16bpp_specifications.c)
set(alphamap_16bpp_REG_TESTS guix_alphamap_brush_alpha
                             guix_generic_alphamap_draw)

set(alphamap_32bppdriver_FILE_LIST
    alphamap_32bppdriver_resources.c alphamap_32bppdriver_specifications.c
    demo_alphamap_32bppdriver.c)
set(alphamap_32bppdriver_REG_TESTS guix_alphamap_32bppdriver)

set(binres_theme_static_language_FILE_LIST
demo_guix_binres_theme_static_language.c
binres_theme_static_language_resources.c
binres_theme_static_language_specifications.c
)

set(binres_theme_static_language_REG_TESTS guix_binres_theme_static_language)

set(drop_list_FILE_LIST
    demo_guix_drop_list.c guix_drop_list_resources.c guix_drop_list_specifications.c)

set(drop_list_REG_TESTS guix_drop_list)

set(canvas_text_view_FILE_LIST
    demo_guix_canvas_text_view.c canvas_text_view_resources.c
    canvas_text_view_specifications.c)

set(color_convert_16bpp_FILE_LIST
    color_convert_16bpp_resources.c color_convert_16bpp_specifications.c
    demo_guix_color_convert_16bpp.c)
set(color_convert_16bpp_REG_TESTS
    guix_color_convert_16bpp
    guix_image_reader_pixel_write_callback_set_no_output)

set(color_convert_32bpp_FILE_LIST
    color_convert_32bpp_resources.c color_convert_32bpp_specifications.c
    demo_guix_color_convert_32bpp.c)
set(color_convert_32bpp_REG_TESTS
    guix_color_convert_32bpp guix_image_reader_image_decode
    guix_image_reader_rle_encode_no_output guix_image_reader_start_no_output)

set(color_convert_8bpp_FILE_LIST
    color_convert_8bpp_resources.c color_convert_8bpp_specifications.c
    demo_guix_color_convert_8bpp.c)
set(color_convert_8bpp_REG_TESTS guix_color_convert_8bpp
                                 guix_image_reader_nearest_palette_color_get)

set(extended_unicode_FILE_LIST extended_unicode.c extended_unicode_resources.c
                               extended_unicode_specifications.c)
set(extended_unicode_REG_TESTS guix_extended_unicode)

set(generic_16bpp_32bppdriver_FILE_LIST
    demo_generic_16bpp_32bppdriver.c generic_16bpp_32bppdriver_resources.c
    generic_16bpp_32bppdriver_specifications.c)
set(generic_16bpp_32bppdriver_REG_TESTS guix_generic_16bpp_32bppdriver)

set(generic_4444argb_16bpp_FILE_LIST
    demo_generic_4444argb_16bpp.c generic_4444argb_16bpp_resources.c
    generic_4444argb_16bpp_specifications.c)
set(generic_4444argb_16bpp_REG_TESTS guix_generic_4444argb_16bpp)

set(glyph_draw_16bpp_screen_FILE_LIST
    demo_glyph_draw_16bpp.c glyph_draw_16bpp_screen_resources.c
    glyph_draw_16bpp_screen_specifications.c)
set(glyph_draw_16bpp_screen_REG_TESTS guix_glyph_draw_16bpp_screen
                                      guix_glyph_blend_16bpp_screen)

set(glyph_draw_32bpp_screen_FILE_LIST
    demo_glyph_draw_32bpp.c glyph_draw_32bpp_screen_resources.c
    glyph_draw_32bpp_screen_specifications.c)
set(glyph_draw_32bpp_screen_REG_TESTS guix_glyph_draw_32bpp_screen
                                      guix_glyph_blend_32bpp_screen)

set(glyph_draw_8bpp_screen_FILE_LIST
    demo_glyph_draw_8bpp.c glyph_draw_8bpp_screen_resources.c
    glyph_draw_8bpp_screen_specifications.c)
set(glyph_draw_8bpp_screen_REG_TESTS
    guix_glyph_draw_8bpp_screen_aa_text_colors_8)

set(glyph_draw_8bpp_screen_text_colors_16_FILE_LIST
    demo_glyph_draw_8bpp_screen_text_colors_16.c
    glyph_draw_8bpp_screen_text_colors_16_resources.c
    glyph_draw_8bpp_screen_text_colors_16_specifications.c)
set(glyph_draw_8bpp_screen_text_colors_16_REG_TESTS
    guix_glyph_draw_8bpp_screen_aa_text_colors_16)

set(glyph_draw_multi_lang_16bpp_FILE_LIST
    demo_glyph_draw_multi_lang_16bpp.c glyph_draw_multi_lang_16bpp_resources.c
    glyph_draw_multi_lang_16bpp_specifications.c)
set(glyph_draw_multi_lang_16bpp_REG_TESTS guix_glyph_draw_multi_lang_16bpp)

set(glyph_draw_multi_lang_32bpp_FILE_LIST
    demo_glyph_draw_multi_lang_32bpp.c glyph_draw_multi_lang_32bpp_resources.c
    glyph_draw_multi_lang_32bpp_specifications.c)
set(glyph_draw_multi_lang_32bpp_REG_TESTS guix_glyph_draw_multi_lang_32bpp)

set(glyph_draw_multi_lang_8bpp_FILE_LIST
    demo_glyph_draw_multi_lang_8bpp.c glyph_draw_multi_lang_8bpp_resources.c
    glyph_draw_multi_lang_8bpp_specifications.c)
set(glyph_draw_multi_lang_8bpp_REG_TESTS guix_glyph_draw_multi_lang_8bpp)

set(graphics_32bpp_FILE_LIST
    demo_guix_graphics_32bpp.c graphics_32bpp_resources.c
    graphics_32bpp_specifications.c)
set(graphics_32bpp_REG_TESTS
    guix_canvas_polygon_draw
    guix_graphics_arc_32bpp
    guix_graphics_arc_draw
    guix_graphics_circle_32bpp
    guix_graphics_ellipse_32bpp
    guix_graphics_pie_32bpp
    guix_graphics_pie_draw
    guix_graphics_polygon_32bpp
    guix_graphics_rectangle_draw
    guix_generic_ellipse_fill
    guix_generic_wide_arc_draw
    guix_generic_aliased_wide_ellipse_draw
    guix_generic_wide_ellipse_draw
    guix_generic_shape_fill
    guix_generic_polygon_fill
    guix_invalid_shape_draw
    guix_32bpp_horizontal_pixelmap_line_draw
    guix_canvas_circle_draw
    guix_shape_fill)

set(graphics_32bpp_synergy_FILE_LIST
    demo_guix_graphics_32bpp_synergy.c graphics_32bpp_synergy_resources.c
    graphics_32bpp_synergy_specifications.c)

set(graphics_565rgb_synergy_FILE_LIST
    demo_guix_graphics_565rgb_synergy.c graphics_565rgb_synergy_resources.c
    graphics_565rgb_synergy_specifications.c)

set(graphics_palette_FILE_LIST
    demo_guix_graphics_palette.c graphics_palette_resources.c
    graphics_palette_specifications.c)
set(graphics_palette_REG_TESTS
    guix_graphics_palette_arc guix_graphics_palette_circle
    guix_graphics_palette_ellipse guix_graphics_palette_pie
    guix_graphics_palette_polygon guix_8bpp_horizontal_pixelmap_line_draw)

set(helloworld_FILE_LIST helloworld.c)

set(horizontal_list_32bpp_FILE_LIST
    demo_guix_horizontal_list_32bpp.c horizontal_list_32bpp_resources.c
    horizontal_list_32bpp_specifications.c)
set(horizontal_list_32bpp_REG_TESTS
    guix_horizontal_list_drag_32bpp
    guix_horizontal_list_set_32bpp
    guix_horizontal_list_animation
    guix_horizontal_list_page_index_set
    guix_horizontal_list_scroll
    guix_horizontal_list_scroll_with_large_value
    guix_horizontal_list_select
    guix_horizontal_list_selected_set
    guix_horizontal_list_total_columns_set
    guix_horizontal_list_event_process
    guix_horizontal_list_wrap
    guix_horizontal_list_wrap_1
    guix_horizontal_list_children_position_no_output)

set(jpeg_draw_32bpp_FILE_LIST
    demo_guix_jpeg_draw_32bpp.c jpeg_draw_32bpp_resources.c
    jpeg_draw_32bpp_specifications.c restart_interval_jpeg_map.c)
set(jpeg_draw_32bpp_REG_TESTS guix_jpeg_draw_32bpp
                              guix_image_reader_jpeg_decode_no_output)

set(kerning_glyph_draw_32bpp_screen_FILE_LIST
    demo_kerning_glyph_draw_32bpp_screen.c
    kerning_glyph_draw_32bpp_screen_resources.c
    kerning_glyph_draw_32bpp_screen_specifications.c)

set(kerning_glyph_draw_32bpp_screen_REG_TESTS
    guix_kerning_glyph_draw_32bpp_screen
    guix_kerning_glyph_draw_32bpp_screen_binary_source)

set(kerning_glyph_in_string_table_FILE_LIST
    demo_kerning_glyph_in_string_table.c
    kerning_glyph_in_string_table_resources.c
    kerning_glyph_in_string_table_specifications.c)

set(kerning_glyph_in_string_table_REG_TESTS guix_kerning_glyph_in_string_table)

set(jpeg_draw_16bpp_FILE_LIST
    demo_guix_jpeg_draw_16bpp.c jpeg_draw_16bpp_resources.c
    jpeg_draw_16bpp_specifications.c)
set(jpeg_draw_16bpp_REG_TESTS guix_jpeg_draw_16bpp)

set(keyboard_FILE_LIST guix_keyboard_resources.c guix_keyboard_specifications.c
                       demo_guix_keyboard.c)

set(line_1bpp_FILE_LIST resource.c demo_line_1bpp.c)

set(line_1bpp_REG_TESTS guix_line_1bpp guix_1bpp_simple_line_draw
                        guix_1bpp_horizontal_line_draw)

set(line_8bpp_FILE_LIST demo_guix_line_8bpp.c lines_8bpp_resources.c
                        lines_8bpp_specifications.c)

set(line_8bpp_REG_TESTS
    guix_line_8bpp guix_8bpp_simple_line_draw
    guix_display_driver_8bpp_vertical_line_draw
    guix_display_driver_8bpp_horizontal_line_draw)

set(lines_FILE_LIST demo_guix_lines.c lines_resources.c lines_specifications.c)
set(lines_REG_TESTS
    guix_lines_24xrgb
    guix_lines_clip_24xrgb
    guix_lines_clip_565rgb
    guix_display_driver_simple_line_alpha_draw
    guix_display_driver_aliased_line_draw
    guix_32bpp_simple_line_draw
    guix_generic_line_draw)

set(line_chart_FILE_LIST demo_guix_chart.c line_chart_resources.c
                         line_chart_specifications.c)

set(line_chart_REG_TESTS guix_line_chart)

set(menu_FILE_LIST demo_guix_menu.c menu_resources.c menu_specifications.c)
set(menu_REG_TESTS guix_menu_32bpp)

set(ml_text_input_16bpp_FILE_LIST
    demo_guix_ml_text_input_16bpp.c ml_text_input_16bpp_resources.c
    ml_text_input_16bpp_specifications.c)
set(ml_text_input_16bpp_REG_TESTS
    guix_ml_text_input_16bpp
    guix_ml_text_input_char_insert
    guix_ml_text_input_create
    guix_ml_text_input_backspace_16bpp
    guix_ml_text_input_delete
    guix_ml_text_input_left_right_arrow_16bpp
    guix_ml_text_input_block_move
    guix_ml_text_input_select_copy_paste
    guix_ml_text_input_api_select
    guix_ml_text_input_cache
    guix_ml_text_input_drag_select
    guix_ml_text_input_key_down_process
    guix_ml_text_input_key_api
    guix_ml_text_input_property
    guix_ml_text_input_line_break
    guix_ml_text_input_text_set
    guix_ml_text_input_event_process
    guix_ml_text_input_invalid_font
    guix_ml_text_input_copy
    guix_ml_text_input_paste
    guix_ml_text_input_mark_next
    guix_ml_text_input_mark_previous
    guix_ml_text_input_api_select_invisible
    guix_ml_text_input_cursor_pos_update
    guix_ml_text_input_line_space
    guix_ml_text_input_left_arrow)

set(ml_text_input_32bpp_FILE_LIST
    demo_guix_ml_text_input_32bpp.c ml_text_input_32bpp_resources.c
    ml_text_input_32bpp_specifications.c)
set(ml_text_input_32bpp_REG_TESTS guix_ml_text_input_32bpp)

set(ml_text_input_8bpp_FILE_LIST
    demo_guix_ml_text_input_8bpp.c ml_text_input_8bpp_resources.c
    ml_text_input_8bpp_specifications.c)

set(ml_text_view_16bpp_FILE_LIST
    demo_guix_ml_text_view_16bpp.c ml_text_view_16bpp_resources.c
    ml_text_view_16bpp_specifications.c)
set(ml_text_view_16bpp_REG_TESTS
    guix_ml_text_view_align_center_16bpp guix_ml_text_view_align_right_16bpp
    guix_ml_text_view_16bpp_text_set guix_ml_text_view_display_info_get
    guix_ml_text_view_event_process guix_ml_text_view_scroll)

set(ml_text_view_32bpp_FILE_LIST
    demo_guix_ml_text_view_32bpp.c ml_text_view_32bpp_resources.c
    ml_text_view_32bpp_specifications.c)
set(ml_text_view_32bpp_REG_TESTS guix_ml_text_view_32bpp
                                 guix_ml_text_view_cache_32bpp)

set(ml_text_view_8bpp_FILE_LIST
    demo_guix_ml_text_view_8bpp.c ml_text_view_8bpp_resources.c
    ml_text_view_8bpp_specifications.c)

set(multi_line_text_view_clip_16bpp_FILE_LIST
    demo_guix_multi_line_text_view_clip_16bpp.c
    multi_line_text_view_clip_16bpp_resources.c
    multi_line_text_view_clip_16bpp_specifications.c)

set(multi_line_text_view_clip_8bpp_FILE_LIST
    demo_guix_multi_line_text_view_clip_8bpp.c
    multi_line_text_view_clip_8bpp_resources.c
    multi_line_text_view_clip_8bpp_specifications.c)

set(multi_line_text_view_clip_32bpp_FILE_LIST
    demo_guix_multi_line_text_view_clip_32bpp.c
    multi_line_text_view_clip_32bpp_resources.c
    multi_line_text_view_clip_32bpp_specifications.c)
set(multi_line_text_view_clip_32bpp_REG_TESTS
    guix_multi_line_text_view_clip_24xrgb)

set(multi_line_text_view_font_16bpp_FILE_LIST
    demo_guix_multi_line_text_view_font_16bpp.c
    multi_line_text_view_font_16bpp_resources.c
    multi_line_text_view_font_16bpp_specifications.c)
set(multi_line_text_view_font_16bpp_REG_TESTS guix_ml_text_view_font_16bpp)

set(multi_line_text_view_font_8bpp_FILE_LIST
    demo_guix_multi_line_text_view_font_8bpp.c
    multi_line_text_view_font_8bpp_resources.c
    multi_line_text_view_font_8bpp_specifications.c)
set(multi_line_text_view_font_8bpp_REG_TESTS guix_ml_text_view_font_8bpp)

set(multi_line_text_view_font_32bpp_FILE_LIST
    demo_guix_multi_line_text_view_font_32bpp.c
    multi_line_text_view_font_32bpp_resources.c
    multi_line_text_view_font_32bpp_specifications.c)
set(multi_line_text_view_font_32bpp_REG_TESTS
    guix_multi_line_text_view_font_24xrgb
    guix_multi_line_text_view_visible_row_compute)

set(ml_text_view_vcenter_FILE_LIST
    demo_guix_ml_text_view_vcenter.c ml_text_view_vcenter_resources.c
    ml_text_view_vcenter_specifications.c)
set(ml_text_view_vcenter_REG_TESTS guix_ml_text_view_vcenter)

set(multi_themes_16bpp_FILE_LIST
    demo_guix_multi_themes_16bpp.c multi_themes_16bpp_resources.c
    multi_themes_16bpp_specifications.c)
set(multi_themes_16bpp_REG_TESTS
    guix_multi_themes_16bpp guix_binres_language_info_get_no_output
    guix_binres_language_table_load guix_binres_language_table_load_use_old_api
    guix_binres_theme_load)

set(multi_themes_16bpp_synergy_FILE_LIST
    demo_guix_multi_themes_16bpp_synergy.c
    multi_themes_16bpp_synergy_resources.c
    multi_themes_16bpp_synergy_specifications.c)
set(multi_themes_16bpp_synergy_REG_TESTS guix_multi_themes_16bpp_synergy
                                         guix_binres_theme_load_synergy)

set(multi_themes_8bpp_palette_FILE_LIST
    demo_guix_multi_themes_8bpp_palette.c multi_themes_8bpp_palette_resources.c
    multi_themes_8bpp_palette_specifications.c)
set(multi_themes_8bpp_palette_REG_TESTS
    guix_multi_themes_8bpp_palette_theme_1
    guix_multi_themes_8bpp_palette_theme_2
    guix_multi_themes_8bpp_palette_theme_3
    guix_multi_themes_8bpp_palette_theme_4
    guix_multi_themes_8bpp_palette_theme_5
    guix_binres_theme_load_8bpp)

set(palette_16bpp_FILE_LIST demo_guix_palette_16bpp.c palette_16bpp_resources.c
                            palette_16bpp_specifications.c)
set(palette_16bpp_REG_TESTS guix_palette_16bpp)

set(palette_32bpp_FILE_LIST demo_guix_palette_32bpp.c palette_32bpp_resources.c
                            palette_32bpp_specifications.c)
set(palette_32bpp_REG_TESTS guix_palette_32bpp)

set(pixelmap_16bpp_FILE_LIST
    demo_guix_pixelmap_16bpp.c pixelmap_16bpp_resources.c
    pixelmap_16bpp_specifications.c)
set(pixelmap_16bpp_REG_TESTS guix_pixelmap_565rgb)

set(pixelmap_8bpp_FILE_LIST demo_guix_pixelmap_8bpp.c pixelmap_8bpp_resources.c
                            pixelmap_8bpp_specifications.c)

set(pixelmap_8bpp_REG_TESTS guix_8bpp_pixelmap_draw)

set(pixelmaps_4444argb_FILE_LIST
    demo_guix_pixelmaps_4444argb.c pixelmaps_4444argb_resources.c
    pixelmaps_4444argb_specifications.c)
set(pixelmaps_4444argb_REG_TESTS guix_pixelmaps_4444argb
                                 guix_4444argb_pixelmap_draw)

set(pixelmaps_565rgb_FILE_LIST
    demo_guix_pixelmaps_565rgb.c pixelmaps_565rgb_resources.c
    pixelmaps_565rgb_specifications.c)
set(pixelmaps_565rgb_REG_TESTS guix_pixelmaps_565rgb
                               guix_565rgb_palette_pixelmap_draw)

set(pixelmap_32bpp_FILE_LIST
    demo_guix_pixelmap_32bpp.c pixelmap_32bpp_resources.c
    pixelmap_32bpp_specifications.c)
set(pixelmap_32bpp_REG_TESTS guix_pixelmap_24xrgb)

set(pixelmaps_32bpp_FILE_LIST
    demo_guix_pixelmaps_32bpp.c pixelmaps_32bpp_resources.c
    pixelmaps_32bpp_specifications.c)

set(pixelmaps_32bpp_REG_TESTS
    guix_pixelmaps_32bpp guix_display_driver_generic_glyph_4bit_draw
    guix_32bpp_pixelmap_draw)

set(pixelmaps_565rgb_FILE_LIST
    demo_guix_pixelmaps_565rgb.c pixelmaps_565rgb_resources.c
    pixelmaps_565rgb_specifications.c)

set(pixelmap_clip_16bpp_FILE_LIST
    demo_guix_pixelmap_clip_16bpp.c pixelmap_clip_16bpp_resources.c
    pixelmap_clip_16bpp_specifications.c)
set(pixelmap_clip_16bpp_REG_TESTS guix_pixelmap_clip_565rgb)

set(pixelmap_clip_32bpp_FILE_LIST
    demo_guix_pixelmap_clip_32bpp.c pixelmap_clip_32bpp_resources.c
    pixelmap_clip_32bpp_specifications.c)
set(pixelmap_clip_32bpp_REG_TESTS guix_pixelmap_clip_24xrgb)

set(pixelmap_resize_16bpp_FILE_LIST
    demo_guix_pixelmap_resize_16bpp.c pixelmap_resize_16bpp_resources.c
    pixelmap_resize_16bpp_specifications.c)
set(pixelmap_resize_16bpp_REG_TESTS guix_pixelmap_resize_16bpp
                                    guix_utility_16bpp_pixelmap_resize)

set(pixelmap_resize_32bpp_FILE_LIST
    demo_guix_pixelmap_resize_32bpp.c pixelmap_resize_32bpp_resources.c
    pixelmap_resize_32bpp_specifications.c)
set(pixelmap_resize_32bpp_REG_TESTS guix_pixelmap_resize_32bpp
                                    guix_utility_32argb_pixelmap_resize)

set(pixelmap_resize_8bpp_FILE_LIST
    demo_guix_pixelmap_resize_8bpp.c pixelmap_resize_8bpp_resources.c
    pixelmap_resize_8bpp_specifications.c)
set(pixelmap_resize_8bpp_REG_TESTS guix_pixelmap_resize_8bpp)

set(png_draw_16bpp_FILE_LIST
    demo_guix_png_draw_16bpp.c png_draw_16bpp_resources.c
    png_draw_16bpp_specifications.c)
set(png_draw_16bpp_REG_TESTS guix_png_draw_16bpp)

set(png_draw_32bpp_FILE_LIST
    demo_guix_png_draw_32bpp.c png_draw_32bpp_resources.c
    png_draw_32bpp_specifications.c)
set(png_draw_32bpp_REG_TESTS guix_png_draw_32bpp)

set(polygon_1bpp_FILE_LIST demo_guix_polygon_1bpp.c)

set(polygon_16bpp_FILE_LIST demo_guix_polygon_16bpp.c polygon_16bpp_resources.c
                            polygon_16bpp_specifications.c)

set(radial_progress_bar_FILE_LIST
    demo_guix_radial_progress_bar.c radial_progress_bar_resources.c
    radial_progress_bar_specifications.c)
set(radial_progress_bar_REG_TESTS
    guix_radial_progress_bar
    guix_radial_progress_bar_alpha_blend
    guix_radial_progress_bar_background_draw
    guix_radial_progress_bar_event_process
    guix_radial_progress_bar_resize
    guix_radial_progress_bar_text_draw
    guix_radial_progress_bar_value_calculate)

set(runtime_allocate_16bpp_FILE_LIST
    demo_runtime_allocate_16bpp.c runtime_allocate_16bpp_resources.c
    runtime_allocate_16bpp_specifications.c)
set(runtime_allocate_16bpp_REG_TESTS guix_runtime_allocate_16bpp)

set(singleline_input_32bpp_FILE_LIST demo_guix_singleline_input_32bpp.c)

set(single_line_text_input_clip_16bpp_FILE_LIST
    demo_guix_single_line_text_input_clip_16bpp.c
    single_line_text_input_clip_16bpp_resources.c
    single_line_text_input_clip_16bpp_specifications.c)

set(sl_text_input_16bpp_FILE_LIST
    demo_guix_sl_text_input_16bpp.c
	sl_text_input_16bpp_resources.c
    sl_text_input_16bpp_specifications.c)
	
set(sl_text_input_16bpp_REG_TESTS
    guix_single_line_text_input_16bpp_unicode
    guix_sl_input_select_copy_paste
    guix_sl_input_api_select
    guix_sl_input_gain_lose_focus
    guix_sl_input_cursor
    guix_sl_input_drag_select
    guix_sl_input_text_set
    guix_sl_input_backspace
    guix_sl_input_insert
    guix_sl_input_draw
    guix_sl_input_event_process
    guix_sl_input_left_arrow
    guix_sl_input_mark_home
    guix_sl_input_style_remove
    guix_sl_input_text_select
    guix_sl_input_input_focus_release)

set(sl_text_clip_8bpp_FILE_LIST
    demo_guix_single_line_text_input_clip_8bpp.c sl_text_clip_8bpp_resources.c
    sl_text_clip_8bpp_specifications.c)

set(single_line_text_input_clip_32bpp_FILE_LIST
    demo_guix_single_line_text_input_clip_32bpp.c
    single_line_text_input_clip_32bpp_resources.c
    single_line_text_input_clip_32bpp_specifications.c)
set(single_line_text_input_clip_32bpp_REG_TESTS
    guix_single_line_text_input_clip_32xrgb)

set(sprite_16bpp_FILE_LIST demo_guix_sprite_16bpp.c sprite_16bpp_resources.c
                           sprite_16bpp_specifications.c)
set(sprite_16bpp_REG_TESTS guix_sprite_565rgb)

set(synergy_32argb_FILE_LIST demo_synergy_32argb.c synergy_32argb_resources.c
                             synergy_32argb_specifications.c)
set(synergy_32argb_REG_TESTS guix_synergy_32argb)

set(standalone_binres_load_16bpp_FILE_LIST
    demo_guix_standalone_binres_load_16bpp.c
    standalone_binres_load_16bpp_resources.c
    standalone_binres_load_16bpp_specifications.c)

set(standalone_binres_load_16bpp_REG_TESTS
    guix_standalone_binres_load_16bpp)

set(standalone_binres_load_8bpp_FILE_LIST
    demo_guix_standalone_binres_load_8bpp.c
    standalone_binres_load_8bpp_resources.c
    standalone_binres_load_8bpp_specifications.c)

set(standalone_binres_load_8bpp_REG_TESTS
    guix_standalone_binres_load_8bpp)

set(standalone_binres_load_extended_unicode_32bpp_FILE_LIST
    demo_guix_standalone_binres_load_extended_unicode_32bpp.c
    standalone_binres_load_extended_unicode_32bpp_resources.c
    standalone_binres_load_extended_unicode_32bpp_specifications.c)

set(standalone_binres_load_extended_unicode_32bpp_REG_TESTS
    guix_standalone_binres_load_extended_unicode_32bpp)
    
set(all_widgets_synergy_565rgb_FILE_LIST
    demo_guix_all_widgets_synergy_565rgb.c
    all_widgets_synergy_565rgb_resources.c
    all_widgets_synergy_565rgb_specifications.c
    sprite_screen.c)

set(all_widgets_synergy_565rgb_REG_TESTS guix_all_widgets_synergy_565rgb_glyph_draw)

set(all_widgets_synergy_FILE_LIST
    demo_guix_all_widgets_synergy.c
    all_widgets_synergy_resources.c
    all_widgets_synergy_specifications.c
    gauge_screen.c
    scroll_wheel_screen.c
    sprite_screen.c)

set(all_widgets_synergy_REG_TESTS guix_all_widgets_synergy_glyph_draw)

set(synergy_32bpp_16bppdriver_FILE_LIST
    demo_synergy_32bpp_16bppdriver.c synergy_32bpp_16bppdriver_resources.c
    synergy_32bpp_16bppdriver_specifications.c)
set(synergy_32bpp_16bppdriver_REG_TESTS guix_synergy_32bpp_16bppdriver)

set(synergy_4444argb_16bpp_FILE_LIST
    demo_synergy_4444argb_16bpp.c synergy_4444argb_16bpp_resources.c
    synergy_4444argb_16bpp_specifications.c)
set(synergy_4444argb_16bpp_REG_TESTS guix_synergy_4444argb_16bpp)

set(synergy_4444argb_32bpp_FILE_LIST
    demo_synergy_4444argb_32bpp.c synergy_4444argb_32bpp_resources.c
    synergy_4444argb_32bpp_specifications.c)
set(synergy_4444argb_32bpp_REG_TESTS guix_synergy_4444argb_32bpp)

set(synergy_565rgb_FILE_LIST demo_synergy_565rgb.c synergy_565rgb_resources.c
                             synergy_565rgb_specifications.c)
set(synergy_565rgb_REG_TESTS guix_synergy_565rgb)

set(synergy_glyph_draw_16bpp_FILE_LIST
    demo_synergy_glyph_draw_16bpp.c synergy_glyph_draw_16bpp_screen_resources.c
    synergy_glyph_draw_16bpp_screen_specifications.c)
set(synergy_glyph_draw_16bpp_REG_TESTS guix_synergy_glyph_draw_16bpp
                                       guix_canvas_glyphs_draw)

set(synergy_glyph_draw_32bpp_FILE_LIST
    demo_synergy_glyph_draw_32bpp.c synergy_glyph_draw_32bpp_screen_resources.c
    synergy_glyph_draw_32bpp_screen_specifications.c)
set(synergy_glyph_draw_32bpp_REG_TESTS guix_synergy_glyph_draw_32bpp)

set(screen_drag_animation_FILE_LIST
    demo_screen_drag_animation.c screen_drag_animation_resources.c
    screen_drag_animation_specifications.c)

set(screen_drag_animation_REG_TESTS
    guix_horizontal_screen_drag_animation
    guix_horizontal_screen_drag_animation_with_easing_func
    guix_vertical_screen_drag_animation
    guix_vertical_screen_drag_animation_with_easing_func
    guix_animation_drag_event_process
    guix_animation_drag_flick_event_process
    guix_animation_drag_tracking
    guix_animation_drag_tracking_start)

set(screen_drag_animation_with_canvas_FILE_LIST
    demo_screen_drag_animation_with_canvas.c
    screen_drag_animation_with_canvas_resources.c
    screen_drag_animation_with_canvas_specifications.c)

set(screen_drag_animation_with_canvas_REG_TESTS
    guix_horizontal_screen_drag_animation_with_canvas
    guix_vertical_screen_drag_animation_with_canvas)

set(screen_drag_animation_with_block_move_FILE_LIST
    demo_screen_drag_animation_with_block_move.c
    screen_drag_animation_with_block_move_resources.c
    screen_drag_animation_with_block_move_specifications.c)

set(screen_drag_animation_with_block_move_REG_TESTS
    guix_horizontal_screen_drag_animation_with_block_move
    guix_vertical_screen_drag_animation_with_block_move)

set(screenshot_to_bitmap_16bpp_FILE_LIST
    demo_guix_screenshot_to_bitmap_16bpp.c
    screenshot_to_bitmap_16bpp_resources.c
    screenshot_to_bitmap_16bpp_specifications.c)

set(screenshot_to_bitmap_16bpp_REG_TESTS
    guix_utility_canvas_to_bmp_16bpp_no_output)

set(screenshot_to_bitmap_32bpp_FILE_LIST
    demo_guix_screenshot_to_bitmap_32bpp.c
    screenshot_to_bitmap_32bpp_resources.c
    screenshot_to_bitmap_32bpp_specifications.c)

set(screenshot_to_bitmap_32bpp_REG_TESTS
    guix_utility_canvas_to_bmp_32bpp_no_output guix_utility_no_output)

set(screenshot_to_bitmap_4bpp_FILE_LIST
    demo_guix_screenshot_to_bitmap_4bpp.c screenshot_to_bitmap_4bpp_resources.c
    screenshot_to_bitmap_4bpp_specifications.c)

set(screenshot_to_bitmap_4bpp_REG_TESTS
    guix_utility_canvas_to_bmp_4bpp_no_output)

set(screenshot_to_bitmap_8bit_palette_FILE_LIST
    demo_guix_screenshot_to_bitmap_8bit_palette.c
    screenshot_to_bitmap_8bit_palette_resources.c
    screenshot_to_bitmap_8bit_palette_specifications.c)

set(screenshot_to_bitmap_8bit_palette_REG_TESTS
    guix_utility_canvas_to_bmp_8bit_palette_no_output)

set(screenshot_to_bitmap_332rgb_FILE_LIST
    demo_guix_screenshot_to_bitmap_332rgb.c
    screenshot_to_bitmap_332rgb_resources.c
    screenshot_to_bitmap_332rgb_specifications.c)

set(screenshot_to_bitmap_332rgb_REG_TESTS
    guix_utility_canvas_to_bmp_332rgb_no_output)

set(screenshot_to_bitmap_1555xrgb_FILE_LIST
    demo_guix_screenshot_to_bitmap_1555xrgb.c
    screenshot_to_bitmap_1555xrgb_resources.c
    screenshot_to_bitmap_1555xrgb_specifications.c)

set(screenshot_to_bitmap_1555xrgb_REG_TESTS
    guix_utility_canvas_to_bmp_1555xrgb_no_output)

set(screenshot_to_bitmap_4444argb_FILE_LIST
    demo_guix_screenshot_to_bitmap_4444argb.c
    screenshot_to_bitmap_4444argb_resources.c
    screenshot_to_bitmap_4444argb_specifications.c)

set(screenshot_to_bitmap_4444argb_REG_TESTS
    guix_utility_canvas_to_bmp_4444argb_no_output)

set(screenshot_to_bitmap_monochrome_FILE_LIST
    demo_guix_screenshot_to_bitmap_monochrome.c
    screenshot_to_bitmap_monochrome_resources.c
    screenshot_to_bitmap_monochrome_specifications.c)

set(screenshot_to_bitmap_monochrome_REG_TESTS
    guix_utility_canvas_to_bmp_monochrome_no_output)

set(vertical_list_32bpp_FILE_LIST
    demo_guix_vertical_list_32bpp.c vertical_list_32bpp_resources.c
    vertical_list_32bpp_specifications.c)
set(vertical_list_32bpp_REG_TESTS
    guix_vertical_list_set_32bpp
    guix_vertical_list_animation
    guix_vertical_list_page_index_set
    guix_vertical_list_scroll
    guix_vertical_list_scroll_with_large_value
    guix_vertical_list_select
    guix_vertical_list_selected_set
    guix_vertical_list_total_rows_set
    guix_vertical_list_drag
    guix_vertical_list_event_process
    guix_vertical_list_wrap
    guix_vertical_list_wrap_1)

set(widgets_with_private_string_FILE_LIST
    demo_guix_widgets_with_private_string.c
    widgets_with_private_string_resources.c
    widgets_with_private_string_specifications.c)
set(widgets_with_private_string_REG_TESTS guix_widgets_with_private_string
                                          guix_private_string_delete_no_output)

set(system_screen_stack_FILE_LIST
    demo_guix_system_screen_stack.c system_screen_stack_resources.c
    system_screen_stack_specifications.c)

set(system_screen_stack_REG_TESTS guix_system_screen_stack)

set(non_linear_animation_FILE_LIST
    demo_guix_non_linear_animation.c non_linear_animation_resources.c
    non_linear_animation_specifications.c)
set(non_linear_animation_REG_TESTS guix_non_linear_animation)

set(bidi_text_draw_32bpp_FILE_LIST
    demo_bidi_text_draw_32bpp.c bidi_text_draw_32bpp_resources.c
    bidi_text_draw_32bpp_specifications.c bidi_character_test.c)

set(bidi_text_draw_32bpp_REG_TESTS guix_bidi_text_draw_32bpp
                                   guix_dynamic_bidi_screen)

set(bidi_text_line_break_runtime_FILE_LIST
    demo_guix_bidi_text_line_break_runtime.c
    bidi_text_line_break_runtime_resources.c
    bidi_text_line_break_runtime_specifications.c)

set(bidi_text_line_break_runtime_REG_TESTS guix_bidi_text_line_break_runtime)

set(bidi_text_line_break_static_FILE_LIST
    demo_guix_bidi_text_line_break_static.c
    bidi_text_line_break_static_resources.c
    bidi_text_line_break_static_specifications.c)

set(bidi_text_line_break_static_REG_TESTS guix_bidi_text_line_break_static)

set(generic_scroll_wheel_FILE_LIST
    demo_guix_generic_scroll_wheel.c
    generic_scroll_wheel_resources.c
    generic_scroll_wheel_specifications.c)

set(generic_scroll_wheel_REG_TESTS
    guix_generic_scroll_wheel_no_output
    guix_generic_scroll_wheel
    guix_generic_scroll_wheel_scroll)
    
set(all_widgets_defined_5_4_0_compatibility_FILE_LIST
    demo_guix_all_widgets_defined_5_4_0_compatibility.c
    all_widgets_defined_5_4_0_compatibility_resources.c
    all_widgets_defined_5_4_0_compatibility_specifications.c
    gauge_screen.c
    math_screen.c
    rotate_screen.c
    scroll_wheel_screen.c
    sprite_screen.c)

set(all_widgets_defined_5_4_0_compatibility_REG_TESTS
    guix_all_widgets_defined_5_4_0_compatibility)

set(synergy_text_rotation_FILE_LIST
    demo_synergy_text_rotation.c synergy_text_rotation_resources.c
    synergy_text_rotation_specifications.c)

set(synergy_text_rotation_REG_TESTS guix_synergy_text_rotation)

set(template_FILE_LIST demo_guix_template.c template_resources.c
                       template_specifications.c)

set(all_widgets_synergy_16bpp_rotated_ccw_FILE_LIST
    demo_guix_all_widgets_synergy_16bpp_rotated_ccw.c
    all_widgets_synergy_16bpp_rotated_ccw_resources.c
    all_widgets_synergy_16bpp_rotated_ccw_specifications.c
    gauge_screen.c
    lines_screen.c
    resize_screen.c
    rotate_screen.c
    scroll_wheel_screen.c
    shapes_screen.c)

set(all_widgets_synergy_16bpp_rotated_ccw_REG_TESTS
    guix_all_widgets_synergy_16bpp_rotated_ccw_string_to_alphamap
    guix_all_widgets_synergy_16bpp_rotated_ccw_glyph_draw
    guix_all_widgets_synergy_16bpp_rotated_ccw_pixelmap_draw
    guix_all_widgets_synergy_16bpp_rotated_ccw_pixelmap_fill)

set(all_widgets_synergy_16bpp_rotated_cw_FILE_LIST
    demo_guix_all_widgets_synergy_16bpp_rotated_cw.c
    all_widgets_synergy_16bpp_rotated_cw_resources.c
    all_widgets_synergy_16bpp_rotated_cw_specifications.c
    gauge_screen.c
    lines_screen.c
    resize_screen.c
    rotate_screen.c
    scroll_wheel_screen.c
    shapes_screen.c)

set(all_widgets_synergy_16bpp_rotated_cw_REG_TESTS
    guix_all_widgets_synergy_16bpp_rotated_cw_string_to_alphamap
    guix_all_widgets_synergy_16bpp_rotated_cw_glyph_draw
    guix_all_widgets_synergy_16bpp_rotated_cw_pixelmap_draw
    guix_all_widgets_synergy_16bpp_rotated_cw_pixelmap_fill
    guix_all_widgets_synergy_16bpp_rotated_cw_vertical_list)

set(all_widgets_synergy_16bpp_rotated_ccw_binary_FILE_LIST
    demo_guix_all_widgets_synergy_16bpp_rotated_ccw_binary.c
    all_widgets_synergy_16bpp_rotated_ccw_binary_specifications.c
    gauge_screen.c
    lines_screen.c
    resize_screen.c
    rotate_screen.c
    scroll_wheel_screen.c
    shapes_screen.c)

set(all_widgets_synergy_16bpp_rotated_ccw_binary_REG_TESTS
    guix_all_widgets_synergy_16bpp_rotated_ccw_binary
    guix_all_widgets_synergy_16bpp_rotated_ccw_binary_glyph_draw
    guix_all_widgets_synergy_16bpp_rotated_ccw_binary_pixelmap_draw)

set(all_widgets_synergy_16bpp_rotated_cw_binary_FILE_LIST
    demo_guix_all_widgets_synergy_16bpp_rotated_cw_binary.c
    all_widgets_synergy_16bpp_rotated_cw_binary_specifications.c
    gauge_screen.c
    lines_screen.c
    resize_screen.c
    rotate_screen.c
    scroll_wheel_screen.c
    shapes_screen.c)

set(all_widgets_synergy_16bpp_rotated_cw_binary_REG_TESTS
    guix_all_widgets_synergy_16bpp_rotated_cw_binary
    guix_all_widgets_synergy_16bpp_rotated_cw_binary_glyph_draw
    guix_all_widgets_synergy_16bpp_rotated_cw_binary_pixelmap_draw)

set(template_REG_TESTS guix_template)

set(widget_find_FILE_LIST demo_widget_find.c widget_find_resources.c
                          widget_find_specifications.c)

set(widget_find_REG_TESTS guix_widget_find_demo_no_output
                          guix_widget_scroll_shift)

set(arabic_glyph_shaping_FILE_LIST
    demo_arabic_glyph_shaping.c arabic_glyph_shaping_resources.c
    arabic_glyph_shaping_specifications.c)

set(arabic_glyph_shaping_REG_TESTS guix_arabic_glyph_shaping)

set(farsi_glyph_shaping_FILE_LIST
    demo_farsi_glyph_shaping.c farsi_glyph_shaping_resources.c
    farsi_glyph_shaping_specifications.c)

set(farsi_glyph_shaping_REG_TESTS guix_farsi_glyph_shaping)

set(thai_glyph_shaping_FILE_LIST
    demo_thai_glyph_shaping.c thai_glyph_shaping_resources.c
    thai_glyph_shaping_specifications.c)

set(thai_glyph_shaping_REG_TESTS guix_thai_glyph_shaping)

set(focus_management_FILE_LIST
    demo_guix_focus_management.c focus_management_resources.c
    focus_management_specifications.c)

set(focus_management_REG_TESTS guix_focus_management)

set(rich_text_view_16bpp_FILE_LIST
    demo_guix_rich_text_view_16bpp.c rich_text_view_16bpp_resources.c
    rich_text_view_16bpp_specifications.c)

set(rich_text_view_16bpp_REG_TESTS
    guix_rich_text_view guix_rich_text_view_line_info_get
    guix_rich_text_view_font guix_rich_text_view_scroll
    guix_rich_text_view_no_output)

set(thai_glyph_shaping_runtime_FILE_LIST
    demo_thai_glyph_shaping_runtime.c thai_glyph_shaping_runtime_resources.c
    thai_glyph_shaping_runtime_specifications.c)

set(thai_glyph_shaping_runtime_REG_TESTS guix_thai_glyph_shaping_runtime)

set(partial_frame_buffer_16bpp_FILE_LIST
    demo_guix_partial_frame_buffer_16bpp.c
    partial_frame_buffer_16bpp_resources.c
    partial_frame_buffer_16bpp_specifications.c
    gauge_screen.c
    line_screen.c
    resize_screen.c
    rotate_screen.c
    scroll_wheel_screen.c
    shapes_screen.c
    sprite_screen.c)

set(partial_frame_buffer_16bpp_REG_TESTS
    guix_partial_frame_buffer_16bpp_pixelmap_draw
    guix_partial_frame_buffer_16bpp_glyph_draw
    guix_partial_frame_buffer_16bpp_resize_screen
    guix_partial_frame_buffer_16bpp_rotate_screen
    guix_partial_frame_buffer_16bpp_line_draw
    guix_partial_frame_buffer_16bpp_vertical_list
    guix_partial_frame_buffer_16bpp)

set(demo_guix_calculator_FILE_LIST
    demo_guix_calculator.c evaluate.c guix_calculator_resources.c
    guix_calculator_specifications.c)

set(demo_guix_car_infotainment_FILE_LIST
    demo_guix_car_infotainment.c
    demo_guix_car_infotainment_resources.c
    demo_guix_car_infotainment_specifications.c
    audio.c
    base.c
    climate.c
    phone.c
    vehicle.c
    video.c
    volume_screen_base.c)

set(demo_guix_home_automation_FILE_LIST
    demo_guix_home_automation.c
    demo_guix_home_automation_resources.c
    demo_guix_home_automation_specifications.c
    controller_base.c
    lights.c
    locks.c
    passcode_screen.c
    screen_base.c
    thermostat.c
    weather.c)

set(demo_guix_industrial_FILE_LIST
    demo_guix_industrial.c demo_guix_industrial_resources.c
    demo_guix_industrial_specifications.c complete_window.c sequence_window.c)

set(demo_guix_washing_machine_FILE_LIST
    demo_guix_washing_machine.c
    demo_guix_washing_machine_resources.c
    demo_guix_washing_machine_specifications.c
    mode_select.c
    temperature.c
    wash_cycle.c
    water_level.c)

set(dynamic_menu_FILE_LIST demo_guix_menu.c dynamic_menu_resources.c
                           dynamic_menu_specifications.c)

set(demo_guix_medical_FILE_LIST
    demo_guix_medical.c guix_medical_resources.c guix_medical_specifications.c
    schedule.c patients.c vitals.c)

set(demo_guix_smart_watch_FILE_LIST
    demo_guix_smart_watch.c
    guix_smart_watch_resources.c
    guix_smart_watch_specifications.c
    page_1_message_screen.c
    page_1_music_screen.c
    page_1_weather_screen.c
    page_1_weather_screen_template.c
    page_2_clock_3_screen.c
    page_2_clock_4_screen.c
    page_2_clock_5_screen.c
    page_2_clock_screen_template.c
    page_2_home_screen.c
    page_3_calories_screen.c
    page_3_ekg_screen.c
    page_3_fitness_screen.c
    page_3_run_screen.c
    page_3_stand_screen.c
    page_3_yoga_screen.c
    screen_template.c)

set(demo_guix_widget_types_FILE_LIST
    demo_guix_widget_types.c guix_widget_types_resources.c
    guix_widget_types_specifications.c guix_circular_gauge_screen.c
    guix_scroll_wheel_screen.c)

set(demo_guix_shapes_FILE_LIST
    demo_guix_shapes.c guix_shapes_resources.c
    guix_shapes_specifications.c text_rotation_window.c)

set(demo_guix_simple_FILE_LIST
    demo_guix_simple.c guix_simple_resources.c
    guix_simple_specifications.c)

set(demo_guix_sprite_FILE_LIST demo_guix_sprite.c guix_sprite_resources.c
                     guix_sprite_specifications.c)
set(demo_guix_sprite_REG_TESTS guix_sprite_24xrgb)

set(demo_guix_thermostat_FILE_LIST demo_guix_thermostat.c guix_thermostat_resources.c
                         guix_thermostat_specifications.c)

set(two_display_FILE_LIST
    demo_guix_two_display.c two_display_MAIN_DISPLAY_resources.c
    two_display_SECONDARY_resources.c two_display_specifications.c)

set(demo_guix_binres_FILE_LIST demo_guix_binres.c
                               demo_guix_binres_specifications.c)

set(demo_guix_binres_standalone_FILE_LIST
    demo_guix_binres_standalone.c
    demo_guix_binres_standalone_resources.c
    demo_guix_binres_standalone_specifications.c)

set(demo_guix_circular_gauge_FILE_LIST
    demo_guix_circular_gauge.c demo_guix_circular_gauge_resources.c
    demo_guix_circular_gauge_specifications.c)

set(demo_guix_drop_list_FILE_LIST
    demo_guix_drop_list.c demo_guix_drop_list_resources.c
    demo_guix_drop_list_specifications.c)

set(demo_guix_horizontal_list_FILE_LIST
    demo_guix_horizontal_list.c demo_guix_horizontal_list_resources.c
    demo_guix_horizontal_list_specifications.c)

set(demo_guix_menu_FILE_LIST demo_guix_menu.c demo_guix_menu_resources.c
                             demo_guix_menu_specifications.c)

set(demo_guix_ml_text_input_FILE_LIST
    demo_guix_ml_text_input.c demo_guix_ml_text_input_resources.c
    demo_guix_ml_text_input_specifications.c)

set(demo_guix_radial_slider_FILE_LIST
    demo_guix_radial_slider.c demo_guix_radial_slider_resources.c
    demo_guix_radial_slider_specifications.c)

set(demo_guix_scroll_wheel_FILE_LIST
    demo_guix_scroll_wheel.c demo_guix_scroll_wheel_resources.c
    demo_guix_scroll_wheel_specifications.c)

set(demo_guix_scrollbar_FILE_LIST
    demo_guix_scrollbar.c demo_guix_scrollbar_resources.c
    demo_guix_scrollbar_specifications.c)

set(demo_guix_transitions_FILE_LIST
    demo_guix_transitions.c demo_guix_transitions_resources.c
    demo_guix_transitions_specifications.c)

set(demo_guix_vertical_list_FILE_LIST
    demo_guix_vertical_list.c demo_guix_vertical_list_resources.c
    demo_guix_vertical_list_specifications.c)

set(demo_guix_widget_animation_FILE_LIST
    demo_guix_widget_animation.c demo_guix_widget_animation_resources.c
    demo_guix_widget_animation_specifications.c)

# Display driver
set(SOURCE_DIR ${ROOT_DIR}/test/guix_test/display_driver)
set(DAVE2D_SIMULATION_SOURCE_DIR ${ROOT_DIR}/ports/win32/src/renesas)
set(display_driver_files
    # Linux
    #${SOURCE_DIR}/linux/linux_screen_driver_16bpp.c
    # Renesas
    ${DAVE2D_SIMULATION_SOURCE_DIR}/gx_dave2d_simulation_display_driver_8bit_palette_setup.c
    ${DAVE2D_SIMULATION_SOURCE_DIR}/gx_dave2d_simulation_display_driver_8bpp_glyph_1bit_draw.c
    ${DAVE2D_SIMULATION_SOURCE_DIR}/gx_dave2d_simulation_display_driver_8bpp_glyph_4bit_draw.c
    ${DAVE2D_SIMULATION_SOURCE_DIR}/gx_dave2d_simulation_display_driver_16bpp_compressed_glyph_1bit_draw.c
    ${DAVE2D_SIMULATION_SOURCE_DIR}/gx_dave2d_simulation_display_driver_16bpp_glyph_1bit_draw.c
    ${DAVE2D_SIMULATION_SOURCE_DIR}/gx_dave2d_simulation_display_driver_16bpp_pixelmap_draw.c
    ${DAVE2D_SIMULATION_SOURCE_DIR}/gx_dave2d_simulation_display_driver_16bpp_rotated_compressed_glyph_1bit_draw.c
    ${DAVE2D_SIMULATION_SOURCE_DIR}/gx_dave2d_simulation_display_driver_16bpp_rotated_glyph_1bit_draw.c
    ${DAVE2D_SIMULATION_SOURCE_DIR}/gx_dave2d_simulation_display_driver_16bpp_rotated_pixelmap_draw.c
    ${DAVE2D_SIMULATION_SOURCE_DIR}/gx_dave2d_simulation_display_driver_24xrgb_pixelmap_draw.c
    ${DAVE2D_SIMULATION_SOURCE_DIR}/gx_dave2d_simulation_display_driver_24xrgb_pixelmap_blend.c
    ${DAVE2D_SIMULATION_SOURCE_DIR}/gx_dave2d_simulation_display_driver_24xrgb_setup.c
    ${DAVE2D_SIMULATION_SOURCE_DIR}/gx_dave2d_simulation_display_driver_24xrgb_rotated_pixelmap_draw.c
    ${DAVE2D_SIMULATION_SOURCE_DIR}/gx_dave2d_simulation_display_driver_24xrgb_rotated_pixelmap_blend.c
    ${DAVE2D_SIMULATION_SOURCE_DIR}/gx_dave2d_simulation_display_driver_24xrgb_rotated_setup.c
    ${DAVE2D_SIMULATION_SOURCE_DIR}/gx_dave2d_simulation_display_driver_32bpp_compressed_glyph_1bit_draw.c
    ${DAVE2D_SIMULATION_SOURCE_DIR}/gx_dave2d_simulation_display_driver_32bpp_glyph_1bit_draw.c
    ${DAVE2D_SIMULATION_SOURCE_DIR}/gx_dave2d_simulation_display_driver_32bpp_horizontal_pixelmap_line_draw.c
    ${DAVE2D_SIMULATION_SOURCE_DIR}/gx_dave2d_simulation_display_driver_32bpp_rotated_compressed_glyph_1bit_draw.c
    ${DAVE2D_SIMULATION_SOURCE_DIR}/gx_dave2d_simulation_display_driver_32bpp_rotated_glyph_1bit_draw.c
    ${DAVE2D_SIMULATION_SOURCE_DIR}/gx_dave2d_simulation_display_driver_32bpp_rotated_horizontal_pixelmap_line_draw.c
    ${DAVE2D_SIMULATION_SOURCE_DIR}/gx_dave2d_simulation_display_driver_565rgb_horizontal_pixelmap_line_draw.c
    ${DAVE2D_SIMULATION_SOURCE_DIR}/gx_dave2d_simulation_display_driver_565rgb_rotated_horizontal_pixelmap_line_draw.c
    ${DAVE2D_SIMULATION_SOURCE_DIR}/gx_dave2d_simulation_display_driver_565rgb_rotated_setup.c
    ${DAVE2D_SIMULATION_SOURCE_DIR}/gx_dave2d_simulation_display_driver_565rgb_setup.c
    ${DAVE2D_SIMULATION_SOURCE_DIR}/gx_dave2d_simulation_display_driver_compressed_glyph_4bit_draw.c
    ${DAVE2D_SIMULATION_SOURCE_DIR}/gx_dave2d_simulation_display_driver_compressed_glyph_8bit_draw.c
    ${DAVE2D_SIMULATION_SOURCE_DIR}/gx_dave2d_simulation_display_driver_glyph_4bit_draw.c
    ${DAVE2D_SIMULATION_SOURCE_DIR}/gx_dave2d_simulation_display_driver_rotated_compressed_glyph_8bit_draw.c
    ${DAVE2D_SIMULATION_SOURCE_DIR}/gx_dave2d_simulation_display_driver_rotated_compressed_glyph_4bit_draw.c
    ${DAVE2D_SIMULATION_SOURCE_DIR}/gx_dave2d_simulation_display_driver_rotated_glyph_4bit_draw.c
    ${DAVE2D_SIMULATION_SOURCE_DIR}/gx_synergy_system_font_mono.c
    ${DAVE2D_SIMULATION_SOURCE_DIR}/gx_synergy_system_font_4bpp.c
    # Validation
    ${SOURCE_DIR}/gx_validation_display_driver_16bpp.c
    ${SOURCE_DIR}/gx_validation_display_driver_1555xrgb.c
    ${SOURCE_DIR}/gx_validation_display_driver_1bpp.c
    ${SOURCE_DIR}/gx_validation_display_driver_32bpp.c
    ${SOURCE_DIR}/gx_validation_display_driver_332rgb.c
    ${SOURCE_DIR}/gx_validation_display_driver_4444argb.c
    ${SOURCE_DIR}/gx_validation_display_driver_4bpp_grayscale.c
    ${SOURCE_DIR}/gx_validation_display_driver_8bpp.c
    ${SOURCE_DIR}/gx_validation_synergy_display_driver_16bpp.c
    ${SOURCE_DIR}/gx_validation_synergy_display_driver_32bpp.c)
add_library(display_driver ${display_driver_files})
target_link_libraries(display_driver PUBLIC azrtos::guix m)
target_include_directories(
  display_driver PUBLIC ${SOURCE_DIR}/linux ${ROOT_DIR}/ports/win32/src/renesas
                        ${SOURCE_DIR}/)

# Utility
set(SOURCE_DIR ${ROOT_DIR}/test/guix_test/regression_test)
set(test_utility_files
    ${SOURCE_DIR}/utility/gx_validation_compare_file_data.c
    ${SOURCE_DIR}/utility/gx_validation_display_driver_1bpp.c
    ${SOURCE_DIR}/utility/gx_validation_display_driver_4bpp.c
    ${SOURCE_DIR}/utility/gx_validation_display_driver_8bpp.c
    ${SOURCE_DIR}/utility/gx_validation_display_driver_16bpp.c
    ${SOURCE_DIR}/utility/gx_validation_display_driver_32bpp.c
    ${SOURCE_DIR}/utility/gx_validation_utility.c
    ${SOURCE_DIR}/utility/gx_validation_verify.c)
add_library(test_utility ${test_utility_files})
target_link_libraries(test_utility PUBLIC display_driver)
target_include_directories(test_utility PUBLIC ${SOURCE_DIR}/tests
                                               ${SOURCE_DIR}/utility)

if("-DGX_DISABLE_UTF8_SUPPORT" IN_LIST ${CMAKE_BUILD_TYPE})
  set(demos ${NO_UTF8_DEMOS})
elseif("-DGX_EXTENDED_UNICODE_SUPPORT" IN_LIST ${CMAKE_BUILD_TYPE})
  set(demos ${EXTENDED_UNICODE_DEMOS})
elseif("-DGX_MOUSE_SUPPORT" IN_LIST ${CMAKE_BUILD_TYPE})
  set(demos ${MOUSE_SUPPORT_DEMOS})
elseif("-DGX_FONT_KERNING_SUPPORT" IN_LIST ${CMAKE_BUILD_TYPE})
  set(demos ${FONT_KERNING_SUPPORT_DEMOS})
elseif("-DGX_DYNAMIC_BIDI_TEXT_SUPPORT" IN_LIST ${CMAKE_BUILD_TYPE})
  set(demos ${DYNAMIC_BIDI_TEXT_DEMOS})
elseif("-DGUIX_5_4_0_COMPATIBILITY" IN_LIST ${CMAKE_BUILD_TYPE})
  set(demos ${_5_4_0_COMPATIBLE_DEMOS})
elseif("-DGX_SYNERGY_FONT_FORMAT_SUPPORT" IN_LIST ${CMAKE_BUILD_TYPE})
  set(demos ${SYNERGY_FONT_SUPPORT_DEMOS})
elseif("-DGX_THAI_GLYPH_SHAPING_SUPPORT" IN_LIST ${CMAKE_BUILD_TYPE})
  set(demos ${THAI_GLYPH_SHAPING_SUPPORT_DEMOS})
elseif("-DGX_PALETTE_MODE_AA_TEXT_COLORS=16" IN_LIST ${CMAKE_BUILD_TYPE})
  set(demos ${PALETTE_MODE_AA_TEXT_COLORS_16_DEMOS})
elseif("-DGX_DISABLE_DEPRECATED_STRING_API" IN_LIST ${CMAKE_BUILD_TYPE})
  set(demos ${DEMO_IN_EXAMPLES} ${DEMO_IN_TUTORIALS})
elseif("-DGX_ENABLE_CANVAS_PARTIAL_FRAME_BUFFER" IN_LIST ${CMAKE_BUILD_TYPE})
  set(demos ${PARTIAL_CANVAS_SUPPORT_DEMOS})
else()
  set(demos ${OTHER_DEMOS} ${DEMO_IN_EXAMPLES} ${DEMO_IN_TUTORIALS}
            ${DEMO_SCREENSHOT_TO_BITMAP})
endif()

# Set regression test program's SOURCE as validation_*.c and related source
# files excluding demo_*
foreach(
  demo
  ${NO_UTF8_DEMOS};${EXTENDED_UNICODE_DEMOS};${MOUSE_SUPPORT_DEMOS};${FONT_KERNING_SUPPORT_DEMOS};${DYNAMIC_BIDI_TEXT_DEMOS};
  ${_5_4_0_COMPATIBLE_DEMOS};
  ${SYNERGY_FONT_SUPPORT_DEMOS};
  ${OTHER_DEMOS};
  ${DEMO_IN_EXAMPLES};
  ${DEMO_SCREENSHOT_TO_BITMAP};
  ${THAI_GLYPH_SHAPING_SUPPORT_DEMOS};
  ${PALETTE_MODE_AA_TEXT_COLORS_16_DEMOS};
  ${PARTIAL_CANVAS_SUPPORT_DEMOS};)
  foreach(test_case ${${demo}_REG_TESTS})
    set(${test_case}_SOURCE ${SOURCE_DIR}/tests/validation_${test_case}.c)
    foreach(file ${${demo}_FILE_LIST})

      # Exclude main function definition of extended_unicode demo Exclude main
      # function definition in demo which is not needed in regression tests
      if(NOT ${file} STREQUAL "extended_unicode.c" AND NOT ${file} MATCHES
                                                       "^demo_")
        list(APPEND ${test_case}_SOURCE ${${demo}_SOURCE_DIRECTORY}/${file})
      endif()
    endforeach()
  endforeach()
endforeach()

macro(create_binray binary_name)
  if(${binary_name}_SOURCE)
    set(file_list ${${binary_name}_SOURCE})
  elseif(${binary_name}_FILE_LIST)
    set(file_list "")
    foreach(file ${${binary_name}_FILE_LIST})
      list(APPEND file_list ${${binary_name}_SOURCE_DIRECTORY}/${file})
    endforeach()
  else()
    set(file_list "")
    aux_source_directory(${${binary_name}_SOURCE_DIRECTORY} file_list)
  endif()
  add_executable(${binary_name} ${file_list})
endmacro()

# Add test cases
foreach(demo ${demos})
  # Demo
  create_binray(${demo})
  target_link_libraries(${demo} PRIVATE display_driver)

  # Test cases
  foreach(test_case ${${demo}_REG_TESTS})
    message(STATUS ${test_case})
    create_binray(${test_case})
    target_include_directories(${test_case} PRIVATE ${${demo}_SOURCE_DIRECTORY})
    target_link_libraries(${test_case} PRIVATE test_utility)
    if(${test_case} STREQUAL "guix_*_no_output")
      add_test(${CMAKE_BUILD_TYPE}::${test_case} ${test_case})
    else()
      add_test(${CMAKE_BUILD_TYPE}::${test_case} ${test_case} -checksum -gpath
               ${ROOT_DIR}/test/guix_test/golden_files/)
    endif()
  endforeach()
endforeach()
