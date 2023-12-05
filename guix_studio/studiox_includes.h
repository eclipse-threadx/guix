
#ifndef _STUDIOX_INCLUDES
#define _STUDIOX_INCLUDES

#include "stdafx.h"
#include "studiox.h"

#define REPORT_MEMORY_LEAKS

#if defined REPORT_MEMORY_LEAKS
#include "stdlib.h"
#include "crtdbg.h"
//#define new DEBUG_NEW
//#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

#ifndef GX_FONT_KERNING_SUPPORT
#define GX_FONT_KERNING_SUPPORT
#endif // !GX_FONT_KERNING_SUPPORT


#include "studiox.h"
#include "studiox_constants.h"
#include "gx_api.h"
#include "scrollhelper.h"
#include "gx_win32_display_driver.h"
extern "C"{
#include "studiox_screen_driver.h"
}

typedef struct {
    CString pathname;
    int     pathtype;
} PATHINFO;

typedef struct {
    CString name;
    int  val;
} STRING_VAL_PAIR;
#include "xml_writer.h"
#include "xml_reader.h"
#include "string_table.h"
#include "StudioXProject.h"
#include "widget_writer.h"
#include "widget_reader.h"

#include "widget_service_provider.h"
#include "button_service_provider.h"
#include "text_button_service_provider.h"
#include "mlt_button_service_provider.h"
#include "checkbox_service_provider.h"
#include "radial_progress_bar_service_provider.h"
#include "radial_slider_service_provider.h"
#include "radio_button_service_provider.h"
#include "icon_button_service_provider.h"
#include "pixelmap_button_service_provider.h"
#include "slider_service_provider.h"
#include "progress_bar_service_provider.h"
#include "sprite_service_provider.h"
#include "prompt_service_provider.h"
#include "numeric_prompt_service_provider.h"
#include "pixelmap_prompt_service_provider.h"
#include "numeric_pixelmap_prompt_service_provider.h"
#include "pixelmap_slider_service_provider.h"
#include "icon_service_provider.h"
#include "window_service_provider.h"
#include "line_chart_service_provider.h"
#include "vertical_list_service_provider.h"
#include "drop_list_service_provider.h"
#include "horizontal_list_service_provider.h"
#include "hscroll_service_provider.h"
#include "vscroll_service_provider.h"
#include "ml_text_view_service_provider.h"
#include "rich_text_view_service_provider.h"
#include "ml_text_input_service_provider.h"
#include "text_input_service_provider.h"
#include "circular_gauge_service_provider.h"
#include "template_service_provider.h"
#include "scroll_wheel_service_provider.h"
#include "text_scroll_wheel_service_provider.h"
#include "string_scroll_wheel_service_provider.h"
#include "numeric_scroll_wheel_service_provider.h"
#include "generic_scroll_wheel_service_provider.h"
#include "menu_service_provider.h"
#include "accordion_menu_service_provider.h"
#include "tree_view_service_provider.h"

#include "widget_factory.h"

#include "resource_item.h"
#include "resource_tree.h"

#include "view_header.h"
#include "project_view.h"
#include "resource_view.h"
#include "target_screen.h"
#include "target_view.h"
#include "properties_win.h"
#include "left_panel_frame.h"
#include "right_panel_frame.h"

#include "studiox_util.h"

#include "express_dialog.h"
#include "MainFrm.h"
#include "NewProjectDlg.h"
#include "ProjectConfigDlg.h"
#include "configure_theme_dlg.h"
#include "RecentProjectDlg.h"
#include "font_path_dialog.h"
#include "resource_export_dlg.h"
#include "message_dialog.h"
#include "import_project_dlg.h"
#include "copy_paste_engine.h"

#include "screen_flow.h"
#include "screen_flow_edit_dlg.h"
#include "trigger_edit_dlg.h"
#include "trigger_list_edit_dlg.h"

#include "studio_source_writer.h"
#include "resource_gen.h"
#include "image_reader.h"
#include "screen_generator.h"

#include "CommandInfo.h"
#include "MacroRecordDlg.h"
#include "MacroPlaybackDlg.h"
#include "palette_creater.h"
#include "synergy_settings_dlg.h"
#include "grid_setting_dlg.h"
#include "binary_resource_gen.h"
#include "options_dialog.h"
#include "string_scroll_wheel_edit_dlg.h"
#include "screen_flow.h"
#include "screen_flow_edit_dlg.h"
#include "trigger_action_edit_dlg.h"
#include "trigger_action_select_dlg.h"
#include "trigger_edit_dlg.h"
#include "trigger_list_edit_dlg.h"
#include "easing_function_select_dlg.h"

#include "csv_read_write.h"
#include "xliff_read_write.h"
#include "canonical_normalize.h"

#include "accessibility_props_service.h"
#include "express_combobox.h"
#include "color_box_button.h"
#include "rich_text_edit_dlg.h"
#endif