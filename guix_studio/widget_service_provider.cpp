
#include "studiox_includes.h"
extern "C"{
#include "gx_utility.h"
}

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

typedef struct {
    USHORT widget_type;
    char *event_process_name;
} event_process_entry;

event_process_entry default_event_process_table [] = {
    {GX_TYPE_WIDGET,                    "gx_widget_event_process"},
    {GX_TYPE_BUTTON,                    "gx_button_event_process"},
    {GX_TYPE_TEXT_BUTTON,               "gx_button_event_process"},
    {GX_TYPE_MULTI_LINE_TEXT_BUTTON,    "gx_multi_line_text_button_event_process"},
    {GX_TYPE_RADIO_BUTTON,              "gx_button_event_process"},
    {GX_TYPE_CHECKBOX,                  "gx_checkbox_event_process"},
    {GX_TYPE_PIXELMAP_BUTTON,           "gx_pixelmap_button_event_process"},
    {GX_TYPE_SHADOW_BUTTON,             "gx_button_event_process"},
    {GX_TYPE_ICON_BUTTON,               "gx_button_event_process"},
    {GX_TYPE_SPIN_BUTTON,               "gx_button_event_process"},
    {GX_TYPE_ICON,                      "gx_icon_event_process"},
    {GX_TYPE_SPRITE,                    "gx_sprite_event_process"},
    {GX_TYPE_CIRCULAR_GAUGE,            "gx_circular_gauge_event_process"},
    {GX_TYPE_SLIDER,                    "gx_slider_event_process"},
    {GX_TYPE_PIXELMAP_SLIDER,           "gx_pixelmap_slider_event_process"},
    {GX_TYPE_VERTICAL_SCROLL,           "gx_scrollbar_event_process"},
    {GX_TYPE_HORIZONTAL_SCROLL,         "gx_scrollbar_event_process"},
    {GX_TYPE_PROGRESS_BAR,              "gx_progress_bar_event_process"},
    {GX_TYPE_RADIAL_PROGRESS_BAR,       "gx_radial_progress_bar_event_process"},
    {GX_TYPE_PROMPT,                    "gx_widget_event_process"},
    {GX_TYPE_NUMERIC_PROMPT,            "gx_widget_event_process"},
    {GX_TYPE_PIXELMAP_PROMPT,           "gx_widget_event_process"},
    {GX_TYPE_NUMERIC_PIXELMAP_PROMPT,   "gx_widget_event_process"},
    {GX_TYPE_SINGLE_LINE_TEXT_INPUT,    "gx_single_line_text_input_event_process"},
    {GX_TYPE_PIXELMAP_TEXT_INPUT,       "gx_single_line_text_input_event_process"},
    {GX_TYPE_DROP_LIST,                 "gx_drop_list_event_process"},
    {GX_TYPE_MENU_LIST,                 "gx_widget_event_process"},
    {GX_TYPE_MENU,                      "gx_menu_event_process"},
    {GX_TYPE_ACCORDION_MENU,            "gx_accordion_menu_event_process"},
    {GX_TYPE_WINDOW,                    "gx_window_event_process"},
    {GX_TYPE_ROOT_WINDOW,               "gx_window_event_process"},
    {GX_TYPE_VERTICAL_LIST,             "gx_vertical_list_event_process"},
    {GX_TYPE_HORIZONTAL_LIST,           "gx_horizontal_list_event_process"},
    {GX_TYPE_POPUP_LIST,                "gx_popup_list_event_process"},
    {GX_TYPE_MULTI_LINE_TEXT_VIEW,      "gx_multi_line_text_view_event_process"},
    {GX_TYPE_MULTI_LINE_TEXT_INPUT,     "gx_multi_line_text_input_event_process"},
    {GX_TYPE_LINE_CHART,                "gx_window_event_process"},
    {GX_TYPE_DIALOG,                    "gx_window_event_process"},
    {GX_TYPE_KEYBOARD,                  "gx_window_event_process"},
    {GX_TYPE_SCROLL_WHEEL,              "gx_scroll_wheel_event_process"},
    {GX_TYPE_TEXT_SCROLL_WHEEL,         "gx_scroll_wheel_event_process"},
    {GX_TYPE_STRING_SCROLL_WHEEL,       "gx_scroll_wheel_event_process"},
    {GX_TYPE_NUMERIC_SCROLL_WHEEL,      "gx_scroll_wheel_event_process"},
    {GX_TYPE_GENERIC_SCROLL_WHEEL,      "gx_generic_scroll_wheel_event_process"},
    {GX_TYPE_TREE_VIEW,                 "gx_tree_view_event_process"},
    {0, NULL}
};


///////////////////////////////////////////////////////////////////////////////
widget_service_provider::widget_service_provider()
{
}

///////////////////////////////////////////////////////////////////////////////
CString widget_service_provider::GetVarDeclaration()
{
    return CString("GX_WIDGET_MEMBERS_DECLARE");
}

///////////////////////////////////////////////////////////////////////////////
CString widget_service_provider::GetDefaultEventProcess(int widget_type)
{
    event_process_entry *entry = default_event_process_table;
    while(entry->widget_type)
    {
        if (entry->widget_type == widget_type)
        {
            return CString(entry->event_process_name);
        }
        entry++;
    }
    return CString("GX_NULL");
}

///////////////////////////////////////////////////////////////////////////////
CString widget_service_provider::GetCreateFromDefFunctionName()
{
    return CString("gx_studio_widget_create");
}

void widget_service_provider::MakeCreatePreamble(char *name, int version, CString &out)
{
    CString string(name);

    if (version > 50)
    {
        out.Format(_T("\nUINT gx_studio_%s_create(GX_CONST GX_STUDIO_WIDGET *info, GX_WIDGET *control_block, GX_WIDGET *parent)\n"), string);
    }
    else
    {
        out.Format(_T("\nUINT gx_studio_%s_create(GX_STUDIO_WIDGET *info, GX_WIDGET *control_block, GX_WIDGET *parent)\n"), string);
    }
}

///////////////////////////////////////////////////////////////////////////////
CString widget_service_provider::GetCreateFromDefFunction(int version)
{
    CString out;
    MakeCreatePreamble("widget", version, out);

    out += "{\n"
        "    UINT status;\n"
        "    GX_WIDGET *widget = (GX_WIDGET *) control_block;\n"
        "    status = gx_widget_create(widget, info->widget_name, parent, info->style, info->widget_id, &info->size);\n"
        "    if (status == GX_SUCCESS)\n"
        "    {\n";

        if (project_lib_version() > 50401)
        {
            "#if defined(GUIX_5_4_0_COMPATIBILITY)\n"
            "        gx_widget_fill_color_set(widget, info->normal_fill_color_id, info->selected_fill_color_id);\n"
            "#else\n"
            "        gx_widget_fill_color_set(widget, info->normal_fill_color_id, info->selected_fill_color_id, info->disabled_fill_color_id);\n"
            "#endif\n";
        }
        else
        {
            out += "        gx_widget_fill_color_set(widget, info->normal_fill_color_id, info->selected_fill_color_id);\n";
        }

    out += "    }\n"
        "    return status;\n"
        "}\n";

    return out;
}

///////////////////////////////////////////////////////////////////////////////
CString widget_service_provider::DeclarePropertiesStruct()
{
// GX_BOOL accepts_focus;\n"
    CString out;

    if (project_lib_version() > 50400)
    {
        out = ""
            "typedef struct GX_STUDIO_WIDGET_STRUCT\n"
            "{\n"
            "   GX_CHAR *widget_name;\n"
            "   USHORT  widget_type;\n"
            "   USHORT  widget_id;\n"
            "   #if defined(GX_WIDGET_USER_DATA)\n"
            "   INT   user_data;\n"
            "   #endif\n"
            "   ULONG style;\n"
            "   ULONG status;\n"
            "   ULONG control_block_size;\n"
            "   GX_RESOURCE_ID normal_fill_color_id;\n"
            "   GX_RESOURCE_ID selected_fill_color_id;\n"
            "   GX_RESOURCE_ID disabled_fill_color_id;\n";
    }
    else if (project_lib_version() > 50205)
    {
        out = ""
            "typedef struct GX_STUDIO_WIDGET_STRUCT\n"
            "{\n"
            "   GX_CHAR *widget_name;\n"
            "   USHORT  widget_type;\n"
            "   USHORT  widget_id;\n"
            "   #if defined(GX_WIDGET_USER_DATA)\n"
            "   INT   user_data;\n"
            "   #endif\n"
            "   ULONG style;\n"
            "   ULONG status;\n"
            "   ULONG control_block_size;\n"
            "   GX_RESOURCE_ID normal_fill_color_id;\n"
            "   GX_RESOURCE_ID selected_fill_color_id;\n";

    }
    else
    {
        out = ""
            "typedef struct GX_STUDIO_WIDGET_STRUCT\n"
            "{\n"
            "   GX_CHAR *widget_name;\n"
            "   USHORT  widget_type;\n"
            "   USHORT  widget_id;\n"
            "   ULONG style;\n"
            "   ULONG status;\n"
            "   ULONG control_block_size;\n"
            "   GX_RESOURCE_ID normal_fill_color_id;\n"
            "   GX_RESOURCE_ID selected_fill_color_id;\n";
    }

    if (project_lib_version() < 50100)
    {
        out +=     "   UINT (*create_function) (struct GX_STUDIO_WIDGET_STRUCT *, GX_WIDGET *, GX_WIDGET *);\n";
    }
    else
    {
        out +=     "   UINT (*create_function) (GX_CONST struct GX_STUDIO_WIDGET_STRUCT *, GX_WIDGET *, GX_WIDGET *);\n";
    }
    out += "   void (*draw_function) (GX_WIDGET *);\n"
    "   UINT (*event_function) (GX_WIDGET *, GX_EVENT *);\n"
    "   GX_RECTANGLE size;\n";

    if (project_lib_version() < 50100)
    {
        out +=     "   struct GX_STUDIO_WIDGET_STRUCT *next_widget;\n"
        "   struct GX_STUDIO_WIDGET_STRUCT *child_widget;\n";
    }
    else
    {
        out +=     "   GX_CONST struct GX_STUDIO_WIDGET_STRUCT *next_widget;\n"
        "   GX_CONST struct GX_STUDIO_WIDGET_STRUCT *child_widget;\n";
    }
    out += "   ULONG control_block_offset;\n"
           "   GX_CONST void *properties;\n"
           "} GX_STUDIO_WIDGET;\n\n";

    out += "typedef struct\n"
           "{\n"
           "    GX_CONST GX_STUDIO_WIDGET *widget_information;\n"
           "    GX_WIDGET        *widget;\n"
           "} GX_STUDIO_WIDGET_ENTRY;\n\n";

    return out;
}
 


///////////////////////////////////////////////////////////////////////////////
widget_info *widget_service_provider::InitWidgetInfo(GX_WIDGET *child)
{
    widget_info *info = new widget_info(child->gx_widget_type);
    info->widget = child;
    info->app_name = CString(child->gx_widget_name);
    widget_factory::CreateUniqueAppNames(info);
        
    info->style = child->gx_widget_style;
    info->size = child->gx_widget_size;
    if (child->gx_widget_status & GX_STATUS_ACCEPTS_FOCUS)
    {
        info->accepts_focus = TRUE;
    }
    else
    {
        info->accepts_focus = FALSE;
    }
    info->color_id[NORMAL_FILL_COLOR_INDEX] = child->gx_widget_normal_fill_color;
    info->color_id[SELECTED_FILL_COLOR_INDEX] = child->gx_widget_selected_fill_color;
    info->color_id[DISABLED_FILL_COLOR_INDEX] = child->gx_widget_disabled_fill_color;
    return info;
}

///////////////////////////////////////////////////////////////////////////////
GX_RESOURCE_ID widget_service_provider::GetStringId(char *string)
{
    GX_RESOURCE_ID string_id = 0;
    studiox_project *project = GetOpenProject();

    if (project)
    {
        CString str(string);

        int display = GetProjectView()->GetActiveDisplay();
        string_table *table = project->mDisplays[display].stable;

        if (table)
        {
            int string_index = table->CheckAddString(str);
            CString id_name = table->GetStringId(string_index);
            string_id = table->GetResourceId(id_name);
        }
    }
    return string_id;
}

///////////////////////////////////////////////////////////////////////////////
void widget_service_provider::GetStringText(char *put, int size, widget_info *info)
{
    CString val;
    studiox_project *project = GetOpenProject();

    if (!project || !put || size <= 1)
    {
        return;
    }
    *put = 0;

    int display = project->GetDisplayIndex(info);

    if (display < 0)
    {
        display = GetProjectView()->GetActiveDisplay();
    }

    if (display >= 0)
    {
        string_table *table = project->mDisplays[display].stable;

        if (table)
        {
            CString id_name = table->GetResourceIdName(info->string_id[0]);
            val = table->GetString(id_name);
            int char_count = val.GetLength();

            if (char_count)
            {
                // The maximun UTF8 character is 6 bytes, calculate the maximun utf8 buffer size needed for the string.
                int buffer_size = char_count * 6 + 1;
                char *utf8buf = new char[buffer_size];
                strcpy_s(utf8buf, buffer_size, CT2A(val.GetString(), CP_UTF8));

                int len = strlen(utf8buf);

                if (len < size)
                {
                    strcpy_s(put, size, utf8buf);
                }
                else
                {
                    GX_STRING   string;
                    INT                   index;
                    INT                   glyph_len = 0;
                    index = 0;
                    string.gx_string_ptr = utf8buf;
                    string.gx_string_length = len;

                    /* Calculate the maximum bytes that could be set. */
                    while (index + glyph_len < size)
                    {
                        index += glyph_len;
                        _gx_utility_utf8_string_character_get(&string, GX_NULL, (UINT *)&glyph_len);
                    }
                    size = index;

                    strncpy(put, utf8buf, size);
                    put[size] = 0;
                }

                delete[] utf8buf;
            }
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
widget_info *widget_service_provider::CreateNewInstance(GX_WIDGET *parent)
{
    GX_RECTANGLE size;
    gx_utility_rectangle_define(&size, 0, 0, 79, 23);
    gx_utility_rectangle_center(&size, &parent->gx_widget_size);
    
    GX_WIDGET *widget = new GX_WIDGET;
    gx_widget_create(widget, "widget", parent,
        GX_STYLE_ENABLED, 0, &size);
    widget_info *info = InitWidgetInfo(widget);
    return info;
}

///////////////////////////////////////////////////////////////////////////////
GX_WIDGET *widget_service_provider::GenerateFromInfo(GX_WIDGET *parent, widget_info *info)
{
    GX_WIDGET *widget = new GX_WIDGET;
   
    gx_widget_create(widget,
        (CHAR *) info->app_name.GetString(),
        parent,
        info->style,
        0,
        &info->size);

    gx_widget_fill_color_set(widget,
        info->color_id[NORMAL_FILL_COLOR_INDEX],
        info->color_id[SELECTED_FILL_COLOR_INDEX],
        info->color_id[DISABLED_FILL_COLOR_INDEX]);
    return widget;
}
        
///////////////////////////////////////////////////////////////////////////////
void widget_service_provider::SaveToProject(xml_writer &writer, studiox_project *project, int display, widget_info *info)
{
    GX_WIDGET *widget = info->widget;

    writer.WriteString("type", GetShortName());
    writer.WriteString("app_name", info->app_name);
    writer.WriteRect("size", info->size);
    writer.WriteUnsigned("style", info->style);
    writer.WriteInt("allocation", info->allocation);
    writer.WriteBool("accepts_focus", info->accepts_focus);

    WriteColorId(writer, project, display, "normal_fill_color", info->color_id[NORMAL_FILL_COLOR_INDEX]); 
    WriteColorId(writer, project, display, "selected_fill_color", info->color_id[SELECTED_FILL_COLOR_INDEX]);
    WriteColorId(writer, project, display, "disabled_fill_color", info->color_id[DISABLED_FILL_COLOR_INDEX]);

    if (!info->event_func.IsEmpty())
    {
        writer.WriteString("event_handler", info->event_func);
    }
    if (!info->draw_func.IsEmpty())
    {
        writer.WriteString("draw_func", info->draw_func);
    }
    if (!info->id_name.IsEmpty())
    {
        writer.WriteString("id_name", info->id_name);
    }
    if (!info->custom_name.IsEmpty())
    {
        writer.WriteString("custom_name", info->custom_name);
    }
    if (!info->user_data.IsEmpty())
    {
        writer.WriteString("user_data", info->user_data);
    }

    writer.WriteBool("template", info->is_template);
    writer.WriteBool("visible_at_startup", info->visible_at_startup);
}

///////////////////////////////////////////////////////////////////////////////
#define GLOBAL_STYLES GX_STYLE_BORDER_MASK|GX_STYLE_TRANSPARENT|GX_STYLE_DRAW_SELECTED|GX_STYLE_ENABLED
void widget_service_provider::ReadFromProject(xml_reader &reader, studiox_project *project, int display, widget_info *info, ULONG valid_styles)
{
    GX_WIDGET *widget = info->widget;
    info->base_name = GetShortName();
    reader.ReadString("app_name", info->app_name);
    reader.ReadRect("size", info->size);
    reader.ReadUnsigned("style", info->style);
    reader.ReadInt("allocation", info->allocation);

    valid_styles |= GLOBAL_STYLES;

    if ((info->style & valid_styles) != info->style)
    {
        ErrorMsg("Project contains invalid widget styles. Resetting to default.");
        info->style &= valid_styles;
    }

    if (!reader.ReadBool("accepts_focus", info->accepts_focus))
    {
        info->accepts_focus = TRUE; 
    }

    info->color_id[NORMAL_FILL_COLOR_INDEX] = ReadColorId(reader, project, display, "normal_fill_color");
    info->color_id[SELECTED_FILL_COLOR_INDEX] = ReadColorId(reader, project, display, "selected_fill_color");

    if (project->mHeader.project_version > 54)
    {
        info->color_id[DISABLED_FILL_COLOR_INDEX] = ReadColorId(reader, project, display, "disabled_fill_color");
    }
    else
    {
        info->color_id[DISABLED_FILL_COLOR_INDEX] = info->color_id[NORMAL_FILL_COLOR_INDEX];
    }
    
    reader.ReadString("event_handler", info->event_func);
    reader.ReadString("draw_func", info->draw_func);
    reader.ReadString("id_name", info->id_name);
    reader.ReadString("custom_name", info->custom_name);
    reader.ReadString("user_data", info->user_data);

    reader.ReadBool("template", info->is_template);
    reader.ReadBool("visible_at_startup", info->visible_at_startup);
}
        
///////////////////////////////////////////////////////////////////////////////
void widget_service_provider::AssignColor(widget_info *info, int index, GX_RESOURCE_ID color_id)
{
    int save_id;
    GX_WIDGET *widget = info->widget;

    switch(index)
    {
    case NORMAL_FILL_COLOR_INDEX:
        if (widget)
        {
            save_id = widget->gx_widget_selected_fill_color;
            gx_widget_fill_color_set(widget, color_id, save_id, widget->gx_widget_disabled_fill_color);
        }

        info->color_id[NORMAL_FILL_COLOR_INDEX] = color_id;
        break;

    case SELECTED_FILL_COLOR_INDEX:
        if (widget)
        {
            save_id = widget->gx_widget_normal_fill_color;
            gx_widget_fill_color_set(widget, save_id, color_id, widget->gx_widget_disabled_fill_color);
        }

        info->color_id[SELECTED_FILL_COLOR_INDEX] = color_id;
        break;

    case DISABLED_FILL_COLOR_INDEX:
        if (widget)
        {
            gx_widget_fill_color_set(widget, widget->gx_widget_normal_fill_color, widget->gx_widget_selected_fill_color, color_id);
        }

        info->color_id[DISABLED_FILL_COLOR_INDEX] = color_id;
        break;
    }
}

///////////////////////////////////////////////////////////////////////////////
void widget_service_provider::GetTextDimension(int base_type, GX_CONST GX_CHAR *text,
    GX_FONT *normal_font, GX_FONT *selected_font, INT &max_width, INT &max_height)
{
    GX_VALUE text_width = 0;
    int text_height = 0;
    GX_STRING string;

    if (base_type == GX_TYPE_MULTI_LINE_TEXT_BUTTON)
    {
        // handle multi line text button
        GX_CONST GX_CHAR *buffer = text;
        GX_CONST GX_CHAR *line = NULL;
        int line_index = 0;
        int byte_count = 0;

        while (buffer && (*buffer))
        {
            if ((*buffer == GX_KEY_CARRIAGE_RETURN) || (*buffer == GX_KEY_LINE_FEED))
            {
                if ((*buffer == GX_KEY_CARRIAGE_RETURN) && (*(buffer + 1) == GX_KEY_LINE_FEED))
                {
                    buffer += 2;
                }
                else
                {
                    buffer++;
                }
                line_index++;

                if (line_index >= GX_MULTI_LINE_TEXT_BUTTON_MAX_LINES)
                {
                    break;
                }
                else
                {
                    string.gx_string_ptr = line;
                    string.gx_string_length = byte_count;

                    _gx_system_string_width_get_ext(normal_font, &string, &text_width);
                    if (text_width > max_width)
                    {
                        max_width = text_width;
                    }

                    text_height += normal_font->gx_font_line_height;

                    line = NULL;
                    byte_count = 0;
                }
            }
            else
            {
                if (line == GX_NULL)
                {
                    line = buffer;
                }
                byte_count++;
                buffer++;
            }
        }

        if (line)
        {
            string.gx_string_ptr = line;
            string.gx_string_length = byte_count;

            _gx_system_string_width_get_ext(normal_font, &string, &text_width);
            if (text_width > max_width)
            {
                max_width = text_width;
            }

            text_height += normal_font->gx_font_line_height;
        }

        if (text_height > max_height)
        {
            max_height = text_height;
        }
    }
    else
    {
        string.gx_string_ptr = text;
        string.gx_string_length = strlen(text);
        _gx_system_string_width_get_ext(normal_font, &string, &text_width);
        text_height = normal_font->gx_font_line_height;

        if (text_width > max_width)
        {
            max_width = text_width;
        }

        if (text_height > max_height)
        {
            max_height = text_height;
        }

        if (selected_font)
        {
            _gx_system_string_width_get_ext(selected_font, &string, &text_width);
            text_height = selected_font->gx_font_line_height;

            if (text_width > max_width)
            {
                max_width = text_width;
            }

            if (text_height > max_height)
            {
                max_height = text_height;
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
/* KGM- this should be change to create an instance of a widget (at least once),
   and retrieve the border width for each border style (one time) and save this
   information to an static array. Otherwise the GUIX libary and Studio can
   conflict about the border width for each style.

   This is a quick fix to get things working for now.

   TECH_DEBT
*/
int widget_service_provider::GetBorderWidth(DWORD style)
{
    int width = 0;

    switch (style & GX_STYLE_BORDER_MASK)
    {
    case GX_STYLE_BORDER_RAISED:
    case GX_STYLE_BORDER_RECESSED:
        width = 2;
        break;

    case GX_STYLE_BORDER_THIN:
        width = 1;
        break;

    case GX_STYLE_BORDER_THICK:
        width = 5;
        break;

    case GX_STYLE_BORDER_NONE:
    default:
        width = 0;
        break;
    }
    return width;
}

///////////////////////////////////////////////////////////////////////////////
void widget_service_provider::GetTextDrawDimension(widget_info *info, GX_DISPLAY *display, INT &draw_width, INT &draw_height)
{
    GX_VALUE border_width;

    //pick up widget border
    border_width = GetBorderWidth(info->style);

    draw_width = (info->size.gx_rectangle_right - info->size.gx_rectangle_left + 1);
    draw_width -= (border_width << 1);
    draw_height = (info->size.gx_rectangle_bottom - info->size.gx_rectangle_top + 1);
    draw_height -= (border_width << 1);
    
    switch(info->basetype)
    {
    case GX_TYPE_STRING_SCROLL_WHEEL:
        draw_height = info->ewi.string_scroll_wheel.base.row_height;
        break;

    case GX_TYPE_RADIO_BUTTON:
    case GX_TYPE_CHECKBOX:
        //for radio button and check box, removed pixemap draw area from client area
        if (!(info->style & GX_STYLE_TEXT_CENTER))
        {
            GX_PIXELMAP *map = NULL;
            UINT id;

            if (info->basetype == GX_TYPE_RADIO_BUTTON)
            {
                id = info->pixelmap_id[RADIO_ON_PIXELMAP_INDEX];
                if (!id)
                {
                    id = GX_PIXELMAP_RADIO_ON_ID;
                }
            }
            else
            {
                id = info->pixelmap_id[CHECKED_PIXELMAP_INDEX];
                if (!id)
                {
                    id = GX_PIXELMAP_CHECKBOX_ON_ID;
                }
            }

            if (id < display->gx_display_pixelmap_table_size)
            {
                map = display->gx_display_pixelmap_table[id];
            }

            if (map)
            {
                draw_width -= map->gx_pixelmap_width * 3 / 2;
            }
        }
        break;

    case GX_TYPE_MENU:
        draw_width -= info->ewi.menu.text_x_offset;
        draw_height -= info->ewi.menu.text_y_offset;
        break;

    default:
        break;
    }

    if (draw_width < 0)
    {
        draw_width = 0;
    }

    if (draw_height < 0)
    {
        draw_height = 0;
    }
}

///////////////////////////////////////////////////////////////////////////////
GX_RECTANGLE widget_service_provider::CalculateTextFitSize(widget_info *info, int display_index)
{
    studiox_project *project = GetOpenProject();
    project_view *pview = GetProjectView();
    GX_RECTANGLE newsize = info->size;

    if (!project || !pview)
    {
        return newsize;
    }

    GX_DISPLAY *gx_display = get_target_view_display();
    if (!gx_display)
    {
        return newsize;
    }

    display_info *display = &project->mDisplays[display_index];
    string_table *stable = display->stable;
    int index;
    GX_RESOURCE_ID *id_list;
    int id_count;
    UINT normal_font_id = info->font_id[NORMAL_FONT_INDEX];
    UINT selected_font_id = info->font_id[SELECTED_FONT_INDEX];
    GX_FONT *normal_font = NULL;
    GX_FONT *selected_font = NULL;
    int text_draw_width;
    int text_draw_height;

    if (normal_font_id < gx_display->gx_display_font_table_size)
    {
        //pick up normal font
        normal_font = gx_display->gx_display_font_table[normal_font_id];
    }

    if (!normal_font)
    {
        return newsize;
    }

    //calculate client width and height for text drawing
    if (info->basetype == GX_TYPE_STRING_SCROLL_WHEEL)
    {
        // string scroll wheel haver selected font
        id_list = info->ewi.string_scroll_wheel.string_id_list;
        id_count = info->ewi.string_scroll_wheel.base.total_rows;

        if (selected_font_id < gx_display->gx_display_font_table_size)
        {
            selected_font = gx_display->gx_display_font_table[selected_font_id];
        }

        if (!selected_font)
        {
            return newsize;
        }
    }
    else
    {
        id_list = info->string_id;
        id_count = NUM_WIDGET_STRINGS;
    }

    if (!id_list)
    {
        return newsize;
    }

    GetTextDrawDimension(info, gx_display, text_draw_width, text_draw_height);

    CString string;
    int language_count;
    int language_index;

    //calculate maximum text width and height
    if ((info->basetype == GX_TYPE_NUMERIC_PIXELMAP_PROMPT) ||
        (info->basetype == GX_TYPE_NUMERIC_PROMPT))
    {
        string.Format(_T("%d"), info->ewi.numeric_prompt_value);
        id_count = 1;
        language_count = 1;
    }
    else
    {
        language_count = stable->CountLanguages();
    }

    int text_width = 0;
    int text_height = 0;
    CString id_name;

    for (language_index = 0; language_index < language_count; language_index++)
    {
        //calculate text dimension for one language
        for (index = 0; index < id_count; index++)
        {
            if (id_list[index])
            {
                id_name = stable->GetResourceIdName(id_list[index]);
                string = stable->GetString(id_name, language_index);
                language_count = stable->CountLanguages();
            }

            if (!string.IsEmpty())
            {
                int char_count;
                char *utf8buf;

                // The maximun UTF8 character is 6 bytes, calculate the maximun utf8 buffer size needed for the string.
                char_count = string.GetLength() * 6 + 1;
                utf8buf = new char[char_count];
                strcpy_s(utf8buf, char_count, CT2A(string.GetString(), CP_UTF8));

                GetTextDimension(info->basetype, utf8buf, normal_font, selected_font, text_width, text_height);

                delete[]utf8buf;
            }

            string.Empty();
        }
    }

    newsize.gx_rectangle_right += (text_width - text_draw_width);
    newsize.gx_rectangle_bottom += (text_height - text_draw_height);

    return newsize;
}

///////////////////////////////////////////////////////////////////////////////
BOOL widget_service_provider::CheckResizeToText(widget_info *info, INT resize_mode)
{
    studiox_project *project = GetOpenProject();

    if (!project || !info)
    {
        return FALSE;
    }

    int display = project->GetDisplayIndex(info);

    if (display < 0)
    {
        return FALSE;
    }

    GX_RECTANGLE newsize = info->size;
    GX_RECTANGLE fitsize = CalculateTextFitSize(info, display);
    GX_VALUE border_width;

    //pick up widget border
    gx_widget_border_width_get(info->widget, &border_width);

    if (border_width)
    {
        fitsize.gx_rectangle_right += 2;
        fitsize.gx_rectangle_bottom += 2;
    }

    if (resize_mode == RESIZE_MODE_ALL ||
        resize_mode == RESIZE_MODE_WIDTH)
    {
        newsize.gx_rectangle_right = fitsize.gx_rectangle_right;
    }

    if (resize_mode == RESIZE_MODE_ALL ||
        resize_mode == RESIZE_MODE_HEIGHT)
    {
        newsize.gx_rectangle_bottom = fitsize.gx_rectangle_bottom;
    }

    if (!gx_utility_rectangle_compare(&newsize, &info->widget->gx_widget_size))
    {
        widget_factory::MoveWidget(info, newsize);
        GetTargetScreen()->WidgetWasMoved(info);
        GetPropsWin()->WidgetWasMoved();

        return TRUE;
    }

    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
BOOL widget_service_provider::CheckResizeToPixelmap(widget_info *info,
         int resize_mode, BOOL DontShrink)
{
    GX_PIXELMAP *map = NULL;
    GX_VALUE border_width;
    int map_index;

    int max_map_width = -1;
    int max_map_height = -1;
    studiox_project *project = GetOpenProject();

    if (!project || !info)
    {
        return FALSE;
    }

    int display = project->GetDisplayIndex(info);
    int active_theme = project->mDisplays[display].active_theme;

    if (display >= 0)
    {
        for (map_index = 0; map_index < NUM_WIDGET_PIXELMAPS; map_index++)
        {
            if (info->pixelmap_id[map_index])
            {
                res_info *resource = project->FindResource(display, active_theme, RES_TYPE_PIXELMAP, info->pixelmap_id[map_index]);
                if (resource)
                {
                    map = resource->GetPixelmap();
                    if (map)
                    {
                        if (map->gx_pixelmap_width > max_map_width)
                        {
                            max_map_width = map->gx_pixelmap_width;
                        }
                        if (map->gx_pixelmap_height > max_map_height)
                        {
                            max_map_height = map->gx_pixelmap_height;
                        }
                    }
                }
            }
        }
    }
    
    if (max_map_height <= 0 || max_map_width <= 0)
    {
        return FALSE;
    }

    // check to see if new size if any different

    GX_RECTANGLE newsize = info->size;
    //gx_widget_border_width_get(info->widget, &border_width);
    border_width = GetBorderWidth(info->style);

    if (resize_mode == RESIZE_MODE_ALL ||
        resize_mode == RESIZE_MODE_WIDTH)
    {
        newsize.gx_rectangle_right = newsize.gx_rectangle_left + max_map_width + (border_width * 2) - 1;
    }

    if (resize_mode == RESIZE_MODE_ALL ||
        resize_mode == RESIZE_MODE_HEIGHT)
    {
        newsize.gx_rectangle_bottom = newsize.gx_rectangle_top + max_map_height + (border_width * 2) - 1;
    }

    if (DontShrink)
    {
        if (newsize.gx_rectangle_bottom < info->size.gx_rectangle_bottom)
        {
            return FALSE;
        }
    }

    if (!gx_utility_rectangle_compare(&newsize, &info->widget->gx_widget_size))
    {
        if (info->widget)
        {
            gx_widget_resize(info->widget, &newsize);
        }
        info->size = newsize;
        GetPropsWin()->WidgetWasMoved();
        GetTargetScreen()->WidgetWasMoved(info);
        return TRUE;
    }
    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
void widget_service_provider::WriteStringId(xml_writer &writer, studiox_project *project, int display, char *tag, GX_RESOURCE_ID resid)
{
    string_table *table = project->mDisplays[display].stable;
    CString idName;

    if (!table)
    {
        return;
    }

    idName = table->GetResourceIdName(resid);

    if (!idName.IsEmpty())
    {
        writer.WriteString(tag, idName);
    }
}

///////////////////////////////////////////////////////////////////////////////
void widget_service_provider::WriteFontId(xml_writer &writer, studiox_project *project, int display, char *tag, GX_RESOURCE_ID resid)
{
    CString FontName;

    project->GetResourceName(display, RES_TYPE_FONT, resid, FontName);

    if (!FontName.IsEmpty())
    {
        writer.WriteString(tag, FontName);
    }
}

///////////////////////////////////////////////////////////////////////////////
void widget_service_provider::WritePixelmapId(xml_writer &writer, studiox_project *project, int display, char *tag, GX_RESOURCE_ID resid)
{
    CString PixName;

    project->GetResourceName(display, RES_TYPE_PIXELMAP, resid, PixName);

    if (!PixName.IsEmpty())
    {
        writer.WriteString(tag, PixName);
    }
}

///////////////////////////////////////////////////////////////////////////////
void widget_service_provider::WriteColorId(xml_writer &writer, studiox_project *project, int display, char *tag, GX_RESOURCE_ID resid)
{
    CString ColorName;

    project->GetResourceName(display, RES_TYPE_COLOR, resid, ColorName);

    if (!ColorName.IsEmpty())
    {
        writer.WriteString(tag, ColorName);
    }
}

///////////////////////////////////////////////////////////////////////////////
GX_RESOURCE_ID widget_service_provider::ReadStringId(xml_reader &reader, studiox_project *project, int display, char *tag)
{
    ULONG res_id = 0;
    CString IdName;

    if (project->mHeader.project_version <= 52)
    {
        reader.ReadUnsigned(tag, res_id, 0);
    }
    else
    {
        string_table *table = project->mDisplays[display].stable;
        
        if (table)
        {
            reader.ReadString(tag, IdName);
            res_id = table->GetResourceId(IdName);
        }
    }
    return res_id;
}

///////////////////////////////////////////////////////////////////////////////
GX_RESOURCE_ID widget_service_provider::ReadFontId(xml_reader &reader, studiox_project *project, int display, char *tag)
{
    ULONG res_id = 0;
    CString IdName;

    if (project->mHeader.project_version <= 52)
    {
        reader.ReadUnsigned(tag, res_id, 0);

        // sanity check
        if (res_id > 10000)
        {
            res_id = 0;
        }
    }
    else
    {
        reader.ReadString(tag, IdName);
        res_id = project->GetResourceId(display, RES_TYPE_FONT, IdName);        
    }
    return res_id;
}

///////////////////////////////////////////////////////////////////////////////
GX_RESOURCE_ID widget_service_provider::ReadPixelmapId(xml_reader &reader, studiox_project *project, int display, char *tag)
{
    ULONG res_id = 0;
    CString IdName;

    if (project->mHeader.project_version <= 52)
    {
        reader.ReadUnsigned(tag, res_id, 0);

        // sanity check
        if (res_id > 10000)
        {
            res_id = 0;
        }
    }
    else
    {
        reader.ReadString(tag, IdName);
        res_id = project->GetResourceId(display, RES_TYPE_PIXELMAP, IdName);        
    }
    return res_id;

}

///////////////////////////////////////////////////////////////////////////////
GX_RESOURCE_ID widget_service_provider::ReadColorId(xml_reader &reader, studiox_project *project, int display, char *tag)
{
    ULONG res_id = 0;
    CString IdName;

    if (project->mHeader.project_version <= 52)
    {
        reader.ReadUnsigned(tag, res_id, 0);

        // sanity check
        if (res_id > 10000)
        {
            res_id = 0;
        }
    }
    else
    {
        reader.ReadString(tag, IdName);
        res_id = project->GetResourceId(display, RES_TYPE_COLOR, IdName);        
    }
    return res_id;

}

