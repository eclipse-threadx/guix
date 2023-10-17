                        Express Logic's GUIX  

1. Revision History

For installation and usage please see the readme_guix.txt file included
in your distribution. For port-specific version information, please refer
to the bottom of readme_guix.txt. The GUIX generic code version information 
is as follows:

03-01-2017  GUIX generic code version 5.3.3.

            The following files are removed from version 5.3.3:

            gx_display_driver_8bpp_pixel_blend.c
            gx_display_driver_16bpp_horizontal_line_draw.c (renamed to gx_display_driver_565rgb_horizontal_pixelmap_line_draw.c)            
            gxe_multi_line_text_view_scroll.c

            

            The following files are added in version 5.3.3:

            gx_animation_drag_disable.c
            gx_animation_drag_enable.c
            gx_animation_drag_event_process.c
            gx_animation_drag_tracking.c
            gx_animation_drag_tracking_start.c
            gx_animation_slide_landling.c
            gx_animation_slide_landing_start.c

            gx_binres_language_table_read.c
            gx_binres_reader.h
            gx_binres_theme_read.c

            gx_canvas_hardware_layer_bind.c
            gx_canvas_hide.c
            gx_canvas_rotated_text_draw.c
            gx_canvas_show.c

            gx_display_driver_332rgb_pixel_blend.c
            gx_display_driver_4444argb_canvas_blend.c
            gx_display_driver_565rgb_horizontal_pixelmap_line_draw.c (renamed from gx_display_driver_16bpp_horizontal_line_draw.c)
            gx_display_theme_install.c
            gx_drop_list.h
            gx_drop_list_background_draw.c
            gx_multi_line_text_view_line_space_set.c
            gx_multi_line_text_view_whitespace_set.c
            gx_system_private_string.c
            gx_touch_driver_generic_resistive.c
            gx_utility_alphamap_create.c
            gx_utility_string_to_alphamap.c

            gxe_animation_drag_disable.c
            gxe_animation_drag_enable.c
            gxe_binres_language_table_read.c
            gxe_binres_theme_read.c
            gxe_canvas_hardware_layer_bind.c
            gxe_canvas_hide.c
            gxe_canvas_rotated_text_draw.c
            gxe_canvas_show.c
            gxe_display_theme_install.c
            gxe_multi_line_text_view_draw.c
            gxe_multi_line_text_view_line_space_set.c
            gxe_multi_line_text_view_scroll_info_get.c
            gxe_multi_line_text_view_whitespace_set.c
            gxe_progress_bar_text_draw.c
            gxe_single_line_text_input_text_alignment_set.c
            gxe_system_canvas_refresh.c
            gxe_utility_pixelmap_simple_rotate.c
            gxe_utility_string_to_alphamap.c

            
            The following files are modified in version 5.3.3:

            gx_animation_canvas_define.c                                Added error checking logic.
            gx_animation_stop.c                                         Improved internal logic.            
            gx_animation_update.c                                       Improved internal logic.
            gx_canvas_composite_create.c                                Fixed an issue where the blend function was called before it was set
            gx_canvas_glyphs_draw.c                                     Added support for unicode longer than two bytes.
            gx_circular_gauge_angle_increment_calculate.c               Timer is set on the gauge or draw the gauge only if it is visible.
            gx_display_driver_4444argb_horizontal_pixelmap_line_draw.c  Added support for alpha channel.
            gx_display_driver_4444argb_pixelmap_draw.c                  Added support for alpha channel.
            gx_display_driver_8bpp_canvas_copy.c                        Fixed memory size computation.
            gx_display_driver_generic_arc_fill.c                        Added logic to support only native format for pixelmap
            gx_display_driver_generic_circle_fill.c                     Added logic to support only native format for pixelmap
            gx_display_driver_generic_ellipse_fill.c                    Added logic to support only native format for pixelmap
            gx_display_driver_generic_simple_pie_fill.c                 Added logic to support only native format for pixelmap
            gx_drop_list_create.c                                       Initialized internal data.
            gx_drop_list_draw.c                                         Made separate calls to background draw and children draw.
            gx_drop_list_event_process.c                                Added drop list button event.
            gx_drop_list_pixelmap_set.c                                 Improved internal logic.
            gx_horizontal_list_event_process.c                          Improved internal logic.
            gx_horizontal_list_scroll_info_get.c                        Set scroll max to scroll_visible when max is smaller than visible,
                                                                        thus avoided scrolling with a big value.
            gx_horizontal_list_selected_set.c                           Improved internal logic.
            gx_horizontal_list_total_columns_set.c                      Reset scrollbar when list count is changed.
            gx_icon_background_draw.c                                   Added pixelmap alignment flag.
            gx_multi_line_text_input_buffer_clear.c                     Improved internal logic.
            gx_pixelmap_slider_pixelmap_update.c                        Added support for GX_EVENT_PEN_UP
            gx_popup_list_event_process.c                               Added support for GX_EVENT_PEN_UP
            gx_scrollbar_event_process.c                                Improved internal logic.
            gx_scrollbar_thumb_position_calculate.c                     Improved internal logic.
            gx_scrollbar_value_calculate.c                              Improved internal logic.
            gx_slider_create.c                                          Fixed how the needle height is computed.
            gx_system_canvas_refresh.c                                  Add proper return values.
            gx_system_event_dispatch.c                                  Improved internal logic.
            gx_system_initialize.c                                      Initialized theme table. 
            gx_system_rtos_bind.c                                       Updated proper function name.
            gx_system_string_width_get.c                                Use proper string types.
	    gx_text_input_cursor_draw.c			  		Change logic to determine cursor start and end points.
            gx_utility_utf8_string_character_get.c                      Apply "const" qualifier to un-modified input parameters
            gx_vertical_list_event_process.c				Change to logic handling PEN_DRAG events
	    gx_vertical_list_scroll_info_get.c				Modified to fix scroll information calculation
	    gx_vertical_list_selected_set				Improve logic for calculating page index
            gx_vertical_list_total_rows_set.c				Add logic to reset scrollbar if needed
	    gx_window_root_delete.c					Fix bug in root window delete logic


            The following files are added for binary resource support:

            gx_binres_reader.h
            gx_binres_language_table_read.c
            gx_binres_theme_read.c
            gxe_binres_language_table_read.c
            gxe_binres_theme_read.c

 	    The following files are added to suppport swipe/drag style animations:

	    gx_animation_drag_enable.c
            gx_animation_drag_disable.c
            gx_animation_drag_event_process.c
            gx_animation_drag_tracking.c
            gx_animation_drag_tracking_start.c
            gx_animation_slide_landing.c
            gx_animation_slide_landing_start.c
       	    gxe_animation_drag_enable.c
            gxe_animation_drag_disable.c
      

            The following files are changed to use whitespace and line_space properties in multi-line text input widget:

            gx_multi_line_text_input_backspace.c
            gx_multi_line_text_input_char_insert.c
            gx_multi_line_text_input_cursor_pos_calculate.c
            gx_multi_line_text_input_cursor_pos_update.c
            gx_multi_line_text_input_delete.c
            gx_multi_line_text_input_down_arrow.c
            gx_multi_line_text_input_draw.c
            gx_multi_line_text_input_end.c
            gx_multi_line_text_input_home.c
            gx_multi_line_text_input_left_arrow.c
            gx_multi_line_text_input_up_arrow.c
            gx_multi_line_text_view_scroll.c
            gx_multi_line_text_view_string_total_rows_compute.c
            gx_multi_line_text_view_visible_row_compute.c
            gx_widget_border_style_set.c


            The following files are change to add support for GX_STYLE_TEXT_COPY:
            gx_api.h
            gx_button.h
            gx_checkbox_draw.c
            gx_multi_line_text_button_line_pointers_set.c
            gx_multi_line_text_button_text_draw.c
            gx_multi_line_text_view_line_space_set.c
            gx_multi_line_text_view_whitespace_set.c
            gx_multi_line_text_view_text_id_set.c
            gx_multi_line_text_view_text_set.c
            gx_prompt.h
            gx_prompt_create.c
            gx_prompt_text_draw.c
            gx_prompt_text_get.c
            gx_prompt_text_id_set.c
            gx_prompt_text_set.c
            gx_radio_button_draw.c
            gx_system.h
            gx_system_string_get.c
   	    gx_text_button_text_draw.c
            gx_text_button_text_get.c
	    gx_text_button_text_id_set.c
	    gx_text_button_text_set.c
            gx_widget.h
            gx_widget_delete.c
            gx_widget_text_blend.c
            gx_widget_text_draw.c
            gxe_prompt_text_id_set.c
            gxe_prompt_text_set.c

            The following files are change to initialize newly added draw functions.

            gx_canvas_rotated_text_draw.c   
            gx_display_driver_24xrgb_setup.c
            gx_display_driver_32argb_setup.c
            gx_display_driver_332rgb_setup.c
            gx_display_driver_4444argb_horizontal_pixelmap_line_draw.c
            gx_display_driver_4444argb_pixelmap_draw.c
            gx_display_driver_4444argb_setup.c
            gx_display_driver_565rgb_setup.c
            gx_display_driver_8bit_palette_setup.c
            gx_display_driver_monochrome_setup.c
            gx_display_theme_install.c
            gx_system_private_string.c
            gx_utility_alphamap_create.c
            gxe_canvas_rotated_text_draw.c
            gxe_multi_line_text_view_scroll_info_get.c
            gxe_multi_line_text_view_whitespace_set.c
            gxe_utility_string_to_alphamap.c

            The following files are modified or added to support hardware graphics layers.

            gx_api.h
            gx_canvas.h
            gx_canvas_alpha_set.c
            gx_canvas_create.c
            gx_canvas_delete.c
            gx_canvas_offset_set.c
            
            The following files are modified to improve anti-alias draw logic:
            
            gx_canvas_arc_draw.c
            gx_canvas_circle_draw.c
            gx_canvas_ellipse_draw.c
            gx_canvas_line_draw.c
            gx_canvas_pie_draw.c

            
            The following files are modified to support signed center of rotation values:

	    gx_utility_32argb_pixelmap_rotate.c
            gx_utility_332rgb_pixelmap_rotate.c
            gx_utility_565rgb_pixelmap_rotate.c
            gx_utility_8bpp_pixelmap_rotate.c
            gx_utility_pixelmap_rotate.c
            gx_utility_pixelmap_simple_rotate.c


            The following files are added to support 4444 a:r:g:b graphics format:

            gx_utility_4444argb_pixelmap_resize.c
            gx_utility_4444argb_pixelmap_rotate.c


            The following header files are changed for new APIs, and to clean up data type.

            gx_api.h
            gx_button.h
            gx_canvas.h
            gx_display.h
            gx_image_reader.h
            gx_multi_line_text_view.h
            gx_progress_bar.h
            gx_prompt.h
            gx_single_line_text_input.h
            gx_system.h
            gx_utility.h
            gx_widget.h
            gx_window.h



06-15-2016  GUIX generic code version 5.3.2.  

            The following file has been removed from GUIX 5.3.0 release:

            gxe_progress_bar_fill_color_set.c

            GUIX 5.3.2 includes the following modifications:

            gx_animation_complete.c                                             Fixed the offset for positioning the target.
            gx_animation_update.c                                               Fixed the calculation for steps used in animation
            gx_api.h                                                            Added new features and APIs.
            gx_button.h                                                         Added new features and APIs.
            gx_canvas_pixelmap_draw.c                                           Added compilation flag for software decoder feature.
            gx_circular_gauge.h                                                 Added new features and APIs.
            gx_circular_gauge_draw.c                                            Split background draw logic into a separate function.
            gx_circular_gauge_event_process.c                                   Initialized the needle rectangle during the first time the needle is shown.
            gx_circular_gauge_needle_dirty_mark.c                               Moved the rectangle calculation logic into its own function.
            gx_display_driver_24xrgb_pixelmap_blend.c                           Fixed internal logic in pixelmap width caculation
            gx_horizontal_scrollbar_create.c                                    Use scrollbar_appearance thumb button color when creating end buttons.
            gx_icon.h                                                           Added new features and APIs.
            gx_icon_draw.c                                                      Split background draw into a separate file.
            gx_multi_line_text_button_draw.c                                    Moved the text drawing logic into its own function.
            gx_multi_line_text_button_line_pointers_set.c                       Validated pointer value.
            gx_multi_line_text_input_event_process. c                           Added handler for GX_EVENT_SHOW event.
            gx_multi_line_text_view_text_set.c                                  Initialized text ID.
            gx_prompt.h                                                         Added new features and APIs.
            gx_prompt_text_draw.c                                               Modified function return type.
            gx_scroll_thumb_create.c                                            Use gx_scroll_thumb_color when creating thumb button.
            gx_scroll_thumb_draw.c                                              Fixed internal logic in drawing scroll thumb.
            gx_scrollbar_draw.c                                                 Improved scrollbar drawing, supported no border style.
                                                                                  added logic to draw up and down pixelmap icons.
            gx_scrollbar_event_process.c                                        Added logic to compute client area with regard to the parent border width.
            gx_scrollbar_size_update.c                                          Changed logic in computing size of a scroll button.
            gx_scrollbar_thumb_position_calculate.c                             Modified to correctly apply thumb width and height parameters from 
                                                                                  scrollbar_appearance structure.
            gx_single_line_text_input.h                                         Added new features and APIs.
            gx_system_focus_claim.c                                             Added logic to initialize local variable.
            gx_system_initialize.c                                              Added logic to initialize local variable.
            gx_text_button_draw.c                                               Moved the text draw logic into a separate function.
            gx_vertical_scrollbar_create.c                                      Set scrollbar appearance to normal fill color.
            gx_widget_resize.c                                                  Called widget's event process to handle resize for radial progress bar.
            gx_widget_show.c                                                    Initialized gx_event_target.
            gx_window_client_scroll.c                                           Improved scrolling, added support for transparent scrollbar background.      
            gxe_circular_gauge_create.c                                         Added error check for the size of the control block.
            gxe_image_reader_create.c                                           Added compilation flag for software decoder feature.
            gxe_image_reader_palette_set.c                                      Added compilation flag for software decoder feature.
            gxe_image_reader_start.c                                            Added compilation flag for software decoder feature.
            gxe_multi_line_text_button_draw.c                                   Allowed the scroll widget not to have a parent.


            The following files are modifed for comments and style. 

            gx_display_driver_565rgb_jpeg_draw.c
            gx_image_reader_jpeg_decode.c
            gx_image_reader_pixel_read_callback_set.c
            gx_image_reader_png_decode.c
            gx_sprite_draw.c
            gx_utility_16bpp_pixelmap_resize.c
            gx_utility_4444argb_pixelmap_resize.c
            gx_utility_4444argb_pixelmap_rotate.c
            gx_utility_565rgb_pixelmap_rotate.c
            gx_utility_8bpp_pixelmap_resize.c
            gx_vertical_list_scroll_info_get.c

            Files added to release 5.3.2

            gx_circular_gauge_background_draw.c
            gx_circular_gauge_needle_rectangle_calculate.c
            gx_icon_background_draw.c
            gx_line_chart.h
            gx_line_chart_axis_draw.c
            gx_line_chart_create.c
            gx_line_chart_data_draw.c
            gx_line_chart_draw.c
            gx_line_chart_update.c
            gx_line_chart_y_scale_calculate.c
            gx_multi_line_text_button_text_draw.c
            gx_radial_progress_bar.h
            gx_radial_progress_bar_anchor_set.c
            gx_radial_progress_bar_background_draw.c
            gx_radial_progress_bar_create.c
            gx_radial_progress_bar_draw.c
            gx_radial_progress_bar_event_process.c
            gx_radial_progress_bar_font_set.c
            gx_radial_progress_bar_info_set.c
            gx_radial_progress_bar_resize.c
            gx_radial_progress_bar_size_update.c
            gx_radial_progress_bar_text_color_set.c
            gx_radial_progress_bar_text_draw.c
            gx_radial_progress_bar_value_calculate.c
            gx_radial_progress_bar_value_set.c
            gx_text_button_text_draw.c
            gxe_circular_gauge_background_draw.c
            gxe_circular_gauge_draw.c
            gxe_icon_background_draw.c
            gxe_line_chart_axis_draw.c
            gxe_line_chart_create.c
            gxe_line_chart_data_draw.c
            gxe_line_chart_draw.c
            gxe_line_chart_update.c
            gxe_line_chart_y_scale_calculate.c
            gxe_multi_line_text_button_text_draw.c
            gxe_prompt_text_draw.c
            gxe_radial_progress_bar_anchor_set.c
            gxe_radial_progress_bar_background_draw.c
            gxe_radial_progress_bar_create.c
            gxe_radial_progress_bar_draw.c
            gxe_radial_progress_bar_event_process.c
            gxe_radial_progress_bar_font_set.c
            gxe_radial_progress_bar_info_set.c
            gxe_radial_progress_bar_text_color_set.c
            gxe_radial_progress_bar_text_draw.c
            gxe_radial_progress_bar_value_set.c
            gxe_text_button_text_draw.c


04-05-2016  GUIX generic code version 5.3 Service Pack 1 (SP1).

            SP1 includes the following modifications:

            gx_api.h                                                           Added new APIs
            gx_display.h                                                       Added new APIs
            gx_multi_line_text_input.h                                         Added new APIs
            gx_multi_line_text_view.h                                          Added new APIs
            gx_progress_bar.h                                                  Removed unused function prototypes
            gx_single_line_text_input.h                                        Added new APIs
            gx_user.h                                                          Modified comments      
            gx_utility.h                                                       Added new APIs
            
            gx_canvas_pie_draw.c                                               Fix for drawing with brush_width > radius / 2
            gx_display_driver_16bpp_pixelmap_blend.c                           Fix bug when pixlemap clipped to area smaller than pixelmap
            gx_display_driver_16bpp_pixelmap_rotate.c                          Added a helper function to handle simple rotation with alpha channel. 
            gx_display_driver_8bpp_horizontal_pixelmap_line_draw.c             Added support for pixelmap with alpha channel.
            gx_display_driver_8bpp_pixelmap_rotate.c                           Added support for pixelmap with alpha channel.
            gx_display_driver_generic_aliased_wide_arc_draw.c                  Changed brush style when drawing round end to prevent image fill.
            gx_display_driver_generic_wide_arc_draw.c                          Changed brush style when drawing round end to prevent image fill.


            Files added to support 8-bit 332RGB format:

            gx_display_driver_332rgb_native_color_get.c
            gx_display_driver_332rgb_pixelmap_draw.c
            gx_display_driver_332rgb_setup.c
            gx_utility_332rgb_pixelmap_rotate.c


            Files modified to support left and right text alignment in single-line text input/view widget:

            gx_single_line_text_input_backspace.c 
            gx_single_line_text_input_buffer_clear.c 
            gx_single_line_text_input_buffer_get.c 
            gx_single_line_text_input_character_delete.c 
            gx_single_line_text_input_character_insert.c 
            gx_single_line_text_input_create.c 
            gx_single_line_text_input_draw.c 
            gx_single_line_text_input_end.c 
            gx_single_line_text_input_event_process.c 
            gx_single_line_text_input_home.c 
            gx_single_line_text_input_keydown_process.c 
            gx_single_line_text_input_left_arrow.c 
            gx_single_line_text_input_position_get.c 
            gx_single_line_text_input_position_update.c 
            gx_single_line_text_input_right_arrow.c 
            gx_single_line_text_input_style_add.c 
            gx_single_line_text_input_style_remove.c 
            gx_single_line_text_input_style_set.c
            gxe_single_line_text_input_text_alignment_set.c


            Files modified to support left, center, and right text alignment in multi-line text input/view widget:

            gx_multi_line_text_input_backspace.c 
            gx_multi_line_text_input_buffer_clear.c 
            gx_multi_line_text_input_cursor_pos_calculate.c 
            gx_multi_line_text_input_cursor_pos_update.c 
            gx_multi_line_text_input_left_arrow.c 
            gx_multi_line_text_input_style_add.c 
            gx_multi_line_text_input_style_remove.c 
            gx_multi_line_text_input_style_set.c 
            gx_multi_line_text_view_draw.c 
            gx_multi_line_text_view_string_total_rows_compute.c 
            gxe_multi_line_text_input_buffer_clear.c


            The following files are modified to update comments.
            
            gxe_single_line_text_input_backspace.c 
            gxe_single_line_text_input_buffer_clear.c 
            gxe_single_line_text_input_buffer_get.c 
            gxe_single_line_text_input_character_delete.c 
            gxe_single_line_text_input_character_insert.c 
            gxe_single_line_text_input_create.c 
            gxe_single_line_text_input_draw.c 
            gxe_single_line_text_input_end.c 
            gxe_single_line_text_input_event_process.c 
            gxe_single_line_text_input_home.c   
            gxe_single_line_text_input_left_arrow.c 
            gxe_single_line_text_input_position_get.c 
            gxe_single_line_text_input_right_arrow.c 
            gxe_single_line_text_input_style_add.c 
            gxe_single_line_text_input_style_remove.c 
            gxe_single_line_text_input_style_set.c
            gxe_single_line_text_input_text_alignment_set.c


02/22/2016 GUIX generic code version 5.3.0

Added gx_window_execute API for modal windows.
Added gx_window_close API for modal windows.
Added support for wide borders on circle, ellipse, arc, pie, polygon, and rectangle.
Fixed bug in displaying 4bpp and 1bpp fonts on Synergy target.
Fixed bug in drawing pattern lines on Synergy target.
Added support for 24bpp xrgb output format on Synergy target.
Fixed logic of simple canvas to eliminate need for root window on simple canvas.
Improved documentation of brush styles.
Added support for runtime software JPG decoder.
Added support for runtime software PNG decoder.
Fixed compiler warnings.

8/19/2015 GUIX generic code version 5.2.6

Added file gx_system_event_remove.c
Added file gx_window_execute.c
Added file gxe_window_execute.c
Modified gx_animation_complete.c to target event to parent.
Modified gx_multi_line_text_view_draw.c
Added event handler for GX_EVENT_LANGUAGE_CHANGE to gx_multi_line_text_view_event_process.c
Modifed gx_multi_line_text_view_scroll_info_get.c
Modified gx_multi_line_text_view_text_id_set to use gx_window_scrollbar_find function.
Modified gx_system_event_dispatch to return event processing return code to caller.
Modifed gx_system_language_table_set to target events to root windows.
Added gx_generic_event_purge function to example gx_system_rtos_bind.c
Modifed gx_widget_delete() to call gx_system_event_remove()
Modified gx_widget_event_generate to send targetted event through event queue.
Change event_type parameter to USHORT in gxe_widget_event_generate


8/4/2015 GUIX generic code version 5.2.5

Added GX_CIRCULAR_GAUGE widget, resulting in the following new source files:

	- gx_circular_gauge_angle_get.c
	- gx_circular_gauge_angle_increment_calculate.c
	- gx_circular_guage_angle_set.c
	- gx_circular_gauge_animation_set.c
	- gx_circular_gauge_create.c
	- gx_circular_gauge_draw.c
	- gx_circular_gauge_event_process.c
	- gx_circular_gauge_needle_dirty_mark.c
	- gx_circular_gauge_needle_rotate.c
	- gxe_circular_gauge_angle_get.c
	- gxe_circular_gauge_angle_set.c
	- gxe_circular_gauge_animation_set.c
	- gxe_circular_gauge_create.c

Added gx_display_driver_generic_alphamap_draw.c
Added Renesas Synergy simulation driver for desktop emulation of hardware capabilities.
Added pixelmap_resize functions as part of gx_utility component.
Made gx_vertical_list_page_index_set an API function, added error checking version.
Made gx_horizontal_list_page_index_set an API function, added error checking version.
Fixed bugs in maintaining status and scroll position of vertical and horizontal list when the list is hidden and re-displayed.
Modify gx_animation_start to insure widget is visible.
Modified logic of gx_button_select.c when calling gx_widget_front_move.
Modified logic to claim focus when window is initially displayed.
Modified logic to update views when window is shifted.

4/15/2015 GUIX generic code version 5.2.4

Added gx_utility_32argb_pixelmap_rotate.c
Added gx_utility_565rgb_pixelmap_rotate.c
Added gx_utility_8bpp_pixelmap_rotate.c
Added gx_utility_pixelmap_rotate.c
Added gx_utility_pixelmap_simple_rotate.c
Added gx_multi_line_text_input_buffer_get.c
Added gx_single_line_text_input_buffer_get.c
Added win32_display_driver_565bgr.c
Modified gx_brush_default() to initialize alpha value to 0xff
Fixed bug in gx_display_driver_32argb_setup.c
Modified gx_display_driver_generic_glyph_8bpp_draw.c to support glyph blending in addition to anti-aliasing.
Fixed bug in function _gx_vertical_list_total_rows_set(), not skipping non-client children.
Modified function _gx_system_dirty_entry_shift() to ensure GX_EXIT_CRITICAL is invoked for all paths.


03/02/2015 GUIX generic code version 5.2.3

Added arc drawing API functions and support functions.
Added circle drawing API functions and support functions.
Added pie drawing API functions and support functions.
Add ellipse draw API functions and support functions.
Add pixelmap filling of polygons, arcs, and circles.
Changed gx_vertical_list_selected_get to gx_vertical_list_selected_widget_get
Added gx_vertical_list_selected_index_get API.
Changed gx_horizontal_list_selected_get to gx_horizontal_list_selected_widget_get
Added gx_horizontal_list_selected_index_get API.
Added gx_widget_focus_next API.
Added gx_widget_focus_previous API.
Added gx_widget_nav_order_initialize() to set default next/previous focus navigation order.
Removed obsolete gx_display_driver_generic_polygon_vertex_offset.c
Removed obsolete gx_display_driver_generic_quadrangle_fill.c
Rename gx_first_client_child_get.c to gx_widget_first_client_child_get.c
Rename gx_last_child_child_get.t to gx_widget_last_client_child_get.c
Added logic to gx_window to move focus to first client child when initially displayed.
Added file gx_widget_nav_order_initialize.c
Added file gx_widget_next_client_child_get.c
Added file gx_widget_child_focus_assign.c


1/28/2015 GUIX generic code version 5.2.2

Added new files gx_generic_rtos_bind.c and gx_generic_rtos_bind.h, an os abstraction layer to allow GUIX to be easily ported to operating systems other than ThreadX.
Added pre-processor definition GX_DISABLE_THREADX_BINDING.
Update GUIX User guide to describe os porting requirements.

Changed gx_single_line_text_input_cursor_style_add to gx_single_line_text_input_style_add
Changed gx_single_line_text_input_cursor_style_remove to gx_single_line_text_input_style_remove
Changed gx_single_line_text_input_cursor_style_set to gx_single_line_text_input_style_set



1/16/2015  GUIX generic code version 5.2.1:

removed  gx_canvas_polygon_convex_fill.c   
removed  gx_canvas_polygon_scan_line_fill.
removed  gx_display_driver_16bpp_chart_quadrangle_fill.c 
removed  gx_display_driver_16bpp_quadrangle_fill.c
removed gx_display_driver_1bpp_quadrangle_fill.c
removed gx_display_driver_1bpp_chart_quadrangle_fill.c
removed gx_display_driver_32bpp_chart_quadrangle_fill.c 
removed gx_display_driver_32bpp_quadrangle_fill.c
removed gx_display_driver_8bpp_chart_quadrangle_fill.c
removed gx_display_driver_8bpp_quadrangle_fill.c
removed gx_multi_line_text_input_cursor_draw.c 
removed gx_multi_line_text_input_cursor_flag_clear.c
removed gx_multi_line_text_input_cursor_flag_set.c
removed gx_multi_line_text_input_cursor_rectangle_define
removed gx_multi_line_text_view_char_index_set.c 
removed gx_single_line_text_input_cursor_dirty_mark.c
removed gx_single_line_text_input_cursor_flag_clear.c
removed gx_single_line_text_input_cursor_flag_set.c
removed gx_single_line_text_input_dirty_mark.c 
removed gxe_multi_line_text_input_cursor_flag_clear.c 
removed gxe_multi_line_text_input_cursor_flag_set.c 
removed gxe_multi_line_text_view_char_index_set.c 
removed gxe_single_line_text_input_cursor_flag_clear.c 
removed gxe_single_line_text_input_cursor_flag_set.c 

added gx_display_driver_generic_polygon_convex_fill.c 
added gx_display_driver_generic_polygon_fill.c 
added gx_display_driver_generic_polygon_vertex_offset.c
added gx_display_driver_generic_quadrangle_fill.c 
added gx_multi_line_text_button_create.c 
added gx_multi_line_text_button_draw.c 
added gx_multi_line_text_button_event_process.c 
added gx_multi_line_text_button_line_pointers_set.c 
added gx_multi_line_text_button_text_id_set.c 
added gx_multi_line_text_button_text_set.c 
added gx_multi_line_text_input_cursor_style_add.c 
added gx_multi_line_text_input_cursor_style_remove.c 
added gx_multi_line_text_input_cursor_style_set.c
added gx_single_line_text_input_cursor_style_add.c
added gx_single_line_text_input_cursor_style_remove.c
added gx_single_line_text_input_cursor_style_set.c
added gx_text_input_cursor.h
added gx_text_input_cursor_blink_interval_set.c
added gx_text_input_cursor_dirty_rectangle_get.c
added gx_text_input_cursor_draw.c
added gxe_multi_line_text_button_create.c
added gxe_multi_line_text_button_draw.c
added gxe_multi_line_text_button_event_process.c
added gxe_multi_line_text_button_text_id_set.c
added gxe_multi_line_text_button_text_set.c
added gxe_multi_line_text_input_cursor_style_add.c
added gxe_multi_line_text_input_cursor_style_remove.c
added gxe_multi_line_text_input_cursor_style_set.c
added gxe_single_line_text_input_cursor_style_add.c
added gxe_single_line_text_input_cursor_style_remove.c
added gxe_single_line_text_input_cursor_style_set.c
added gxe_text_input_cursor_blink_interval_set.c
added gxe_text_input_cursor_height_set.c
added gxe_text_input_cursor_width_set.c

modified gx_api.h: Added new GX_MULTI_LINE_TEXT_BUTTON widget, modified text input cursor definition API functions.

modified gx_button.h to add the new multi-line text input button function prototypes.

modified gx_canvas.h to remove the obsolete canvas polygon functions

modified gx_canvas_drawing_complete.c to notify display driver of drawing complete. This is used to trigger the execution of a display list for accelerated display drivers.

modified gx_canvas_drawing_initiate.c to notify the display driver of drawing initiation.

modified gx_canvas_rectangle_draw.c to correctly set the context clipping rectangle.

modified gx_display.h to remove the obsolete quadrangle drawing function prototypes.

modified gx_display_driver_16bpp_polygon_draw.c to implement new polygon drawing algorithm.
modified gx_display_driver_1bpp_polygon_draw.c to implement new polygon drawing algorithm.
modified gx_display_driver_8bpp_polygon_draw.c to implement new polygon drawing algorithm.
modified gx_display_driver_32bpp_polygon_draw.c to implement new polygon drawing algorithm.

modified gx_display_driver_24xrgb_setup.c to remove obsolete function pointer initialization
modified gx_display_driver_32argb_setup.c to remove obsolete function pointer initialization
modified gx_display_driver_565rgb_setup.c to remove obsolete function pointer initialization
modified gx_display_driver_8bi_palette_setup.c to remove obsolete function pointer initialization
modified gx_display_driver_monochrome_setup.c to remove obsolete function pointer initialization

modified gx_multi_line_text_input.h to change prototypes for cursor control functions.
modified gx_multi_line_text_input_backspace.c to improve efficiency
modified gx_multi_line_text_input_char_insert.c to improve efficiency
modified gx_multi_line_text_input_char_remove.c to improve efficiency
modified gx_multi_line_text_input_create.c to make cursor control a member of text input control block.
modified gx_multi_line_text_input_cursor_pos_calculate.c to improve efficiency.
modified gx_multi_line_text_input_cursor_pos_update.c to improve efficiency.
modified gx_multi_line_text_input_delete.c to improve efficiency.
modified gx_multi_line_text_input_down_arrow.c to improve efficiency.
modfied gx_multi_line_text_input_draw to change cursor drawing.
modified gx_multi_line_text_input_end.c to improve efficiency.
modified gx_multi_line_text_input_event_process.c to support cursor drawing changes.
modified gx_multi_line_text_input_home.c to improve efficiency.
modified gx_multi_line_text_input_left_arrow.c to improve efficiency.
modified gx_multi_line_text_input_right_arrow.c to improve efficiency.
modified gx_multi_line_text_input_up_arrow.c to improve efficiency.
modified gx_multi_line_text_view.h to define size of char index buffer.
modified gx_multi_line_text_view_create to support new char index model
modified gx_multi_line_text_view_display_info_get.c to support UTF8 encoding
modified gx_multi_line_te4xt_view_draw.c to improve readability
modified gx_multi_line_text_view_font_set.c to correct scrollbar position when font is changed.
modified gx_multi_line_text_view_scroll.c to improve efficiency.
modified gx_multi_line_text_view_scroll_info_get.c to improve efficiency.
modified gx_multi_line_text_view_string_total_rows_compute.c to improve efficiency.
modified gx_multi_line_text_view_text_id_set.c to update scroll bar.
modified gx_multi_line_text_view_text_set.c to update scroll bar.
modified gx_single_line_text_input.h to accommodate new cursor control API
modified gx_single_line_text_input_backspace.c to improve efficiency
modified gx_single_line_text_input_character_delete.c to improve efficiency
modified gx_single_line_text_input_character_insert.c to improve efficiency
modified gx_single_line_text_input_create.c to improve efficiency
modified gx_single_line_text_input_draw to improve efficiency
modified gx_single_line_text_input_end to improve efficiency
modified gx_single_line_text_input_event_process improve efficiency
modified gx_single_line_text_input_home to improve efficiency
modified gx_single_line_text_input_left_arrow to improve efficiency
modified gx_single_line_text_input_position_get to improve efficiency
modified gx_single_line_text_input_right_arrow to improve efficiency

modified gx_system_active_language_set.c to send a broadcast GX_LANGUAGE_CHANGE event when the active language is modified.

modified gx_system_focus_claim.c  to prevent sending GX_EVENT_FOCUS_LOST to a root window.

modified gx_system_language_table_set.c to send a broadcast GX_LANGUAGE_CHANGE event when the language table is modified.

modified gx_system_string_viewable_length_get to handle UTF8 encoding
modified gx_system_string_width_get.c to improve header comments.
modified gx_text_button_text_id_set.c to remove whitespace.
modified gx_text_input_cursor_height_set.c to assign height to correct control  block member.
modified gx_widget_event_process.c to handle GX_EVENT_LANGUAGE_CHANGE and GX_EVENT_RESOURCE_CHANGE.
modified gx_widget_style_add.c to reflect transparent style in widget status variable.
modified gx_widget_style_remove.c to reflect transparent style in widget status variable.
modified gx_widget_text_draw.c to improve code efficiency.
modified gx_window_root_create.c to remove addition of GX_ENABLED style to improve drawing efficiency.
modified gxe_icon_draw.c to remove invalid test.
modified win32_display_driver.c to add WM_IME_CHAR window message handler.



11/24/2014  Initial GUIX generic code version 5.2. 


Copyright(c) 1996-2017 Express Logic, Inc.


Express Logic, Inc.
11423 West Bernardo Court
San Diego, CA  92127

www.expresslogic.com

