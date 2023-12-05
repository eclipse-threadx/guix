
#include "studiox_includes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
string_scroll_wheel_service_provider::string_scroll_wheel_service_provider()
{
}

///////////////////////////////////////////////////////////////////////////////
CString string_scroll_wheel_service_provider::GetVarDeclaration()
{
    return CString("GX_STRING_SCROLL_WHEEL_MEMBERS_DECLARE");
}

///////////////////////////////////////////////////////////////////////////////
CString string_scroll_wheel_service_provider::DeclarePropertiesStruct()
{
    CString out("");

    if (project_lib_version() >= 50400)
    {
        CString callback;

        if (project_lib_version() >= GX_VERSION_STRING_LENGTH_FIX)
        {
            callback = "UINT (*callback)(struct GX_STRING_SCROLL_WHEEL_STRUCT *, INT, GX_STRING *)";
        }
        else
        {
            callback = "GX_CONST GX_CHAR * (*callback)(struct GX_STRING_SCROLL_WHEEL_STRUCT *, INT)";
        }

        out.Format(
                 _T("typedef struct\n")
                 _T("{\n")
                 _T("%s\n")
                 _T("    GX_CONST GX_RESOURCE_ID  *string_id_list;\n")
                 _T("    %s;\n")
                 _T("} GX_STRING_SCROLL_WHEEL_PROPERTIES;\n\n"),
                 text_scroll_wheel_service_provider::DeclarePropertiesStructContent(),
                 callback);
    }
    else
    {
        out.Format(
                _T("typedef struct\n")
                _T("{\n")
                _T("%s\n")
                _T("    GX_CONST GX_CHAR  **string_list;\n")
                _T("    GX_CONST GX_CHAR *(*callback)(struct GX_STRING_SCROLL_WHEEL_STRUCT *, INT);\n")
                _T("} GX_STRING_SCROLL_WHEEL_PROPERTIES;\n\n"),
                text_scroll_wheel_service_provider::DeclarePropertiesStructContent());
    }

    return out;
}

///////////////////////////////////////////////////////////////////////////////
CString string_scroll_wheel_service_provider::WriteExtendedProperties(screen_generator *gen, CString &prefix, widget_info *info)
{
    CString out;
    CString propname = prefix + info->app_name;
    CString string_list;
    CString temp;
    CString callback;
    studiox_project *project = GetOpenProject();

    if (info->callback_func.IsEmpty() && project)
    {
        callback = "GX_NULL";

        if (info->ewi.string_scroll_wheel.string_id_list)
        {
            CString id_name;

            if (project->mHeader.guix_version < 50400)
            {
                //old version of cold generation
                CString string;
                string_list.Format(_T("%s_string_list"), propname);
                out.Format(_T("GX_CONST CHAR *%s[]={\n"), string_list);

                for (int index = 0; index < info->ewi.string_scroll_wheel.base.total_rows; index++)
                {
                    string = gen->GetString(info->ewi.string_scroll_wheel.string_id_list[index]);

                    if (index == info->ewi.string_scroll_wheel.base.total_rows - 1)
                    {
                        temp.Format(_T("    \"%s\"\n"), string);
                    }
                    else
                    {
                        temp.Format(_T("    \"%s\",\n"), string);
                    }
                    out.Append(temp);
                }
                out.Append(_T("};\n"));
            }
            else
            {
                string_list.Format(_T("%s_string_id_list"), propname);
                out.Format(_T("GX_CONST GX_RESOURCE_ID %s[]={\n"), string_list);

                for (int index = 0; index < info->ewi.string_scroll_wheel.base.total_rows; index++)
                {
                    id_name = gen->GetStringIdName(info->ewi.string_scroll_wheel.string_id_list[index]);
                    if (index == info->ewi.string_scroll_wheel.base.total_rows - 1)
                    {
                        temp.Format(_T("    %s\n"), id_name);
                    }
                    else
                    {
                        temp.Format(_T("    %s,\n"), id_name);
                    }
                    out.Append(temp);
                }
                out.Append(_T("};\n"));
            }
        }
        else
        {
            string_list = "GX_NULL";
        }
    }
    else
    {
        callback = info->callback_func;
        string_list = "GX_NULL";
    }

        temp.Format(_T("GX_STRING_SCROLL_WHEEL_PROPERTIES %s_properties =\n")
            _T("{\n")
            _T("%s\n")
            _T("    %s, /* string list */\n")
        _T("    %s  /* callback */\n")
            _T("\n};\n"),
            propname,
            text_scroll_wheel_service_provider::WriteExtendedPropertiesContent(gen, info, TRUE),
            string_list,
            callback);

    out.Append(temp);
    return out;
}

///////////////////////////////////////////////////////////////////////////////
CString string_scroll_wheel_service_provider::GetCreateFromDefFunctionName()
{
    return CString("gx_studio_string_scroll_wheel_create");
}

///////////////////////////////////////////////////////////////////////////////
CString string_scroll_wheel_service_provider::GetCreateFromDefFunction(int version)
{
    CString out;
    MakeCreatePreamble("string_scroll_wheel", version, out);

    studiox_project *project = GetOpenProject();

    if (!project)
    {
        return out;
    }

    out +=     "{\n"
               "    UINT status;\n"
               "    GX_STRING_SCROLL_WHEEL *wheel = (GX_STRING_SCROLL_WHEEL *) control_block;\n"
               "    GX_STRING_SCROLL_WHEEL_PROPERTIES *props = (GX_STRING_SCROLL_WHEEL_PROPERTIES *) info->properties;\n";

    if (project_lib_version() >= 50400)
    {
        if (project_lib_version() >= GX_VERSION_STRING_LENGTH_FIX)
        {
            out += "    status = gx_string_scroll_wheel_create_ext(wheel, info->widget_name, parent, props->total_rows, GX_NULL, \n"
                   "                                               info->style, info->widget_id, &info->size);\n";
        }
        else
        {
            out += "    status = gx_string_scroll_wheel_create(wheel, info->widget_name, parent, props->total_rows, GX_NULL, \n"
                   "                                    info->style, info->widget_id, &info->size);\n";
        }
        out += "    if (status == GX_SUCCESS)\n"
               "    {\n"
               "        if(props->string_id_list)\n"
               "        {\n"
               "            gx_string_scroll_wheel_string_id_list_set(wheel, props->string_id_list, props->total_rows);\n"
               "        }\n";
    }
    else
    {
        out += "    status = gx_string_scroll_wheel_create(wheel, info->widget_name, parent, props->total_rows, props->string_list, \n"
               "                                    info->style, info->widget_id, &info->size);\n"
               "    if (status == GX_SUCCESS)\n"
               "    {\n";
    }

    out +=     "        if (props->wallpaper_id)\n"
               "        {\n"
               "            gx_window_wallpaper_set((GX_WINDOW *) wheel, props->wallpaper_id, info->style & GX_STYLE_TILE_WALLPAPER);\n"
               "        }\n"
               "        if(props->selected_background)\n"
               "        {\n"
               "            gx_scroll_wheel_selected_background_set((GX_SCROLL_WHEEL *)wheel, props->selected_background);\n"
               "        }\n"
               "        gx_scroll_wheel_selected_set((GX_SCROLL_WHEEL *)wheel, props->selected_row);\n"
               "        gx_scroll_wheel_gradient_alpha_set((GX_SCROLL_WHEEL *)wheel, props->start_alpha, props->end_alpha);\n"
               "        gx_scroll_wheel_row_height_set((GX_SCROLL_WHEEL *)wheel, props->row_height);\n"
               "        gx_text_scroll_wheel_font_set((GX_TEXT_SCROLL_WHEEL *)wheel, props->normal_font, props->selected_font);\n";

    if (project_lib_version() > 50401)
    {
        out += "#if defined(GUIX_5_4_0_COMPATIBILITY)\n"
               "        gx_text_scroll_wheel_text_color_set((GX_TEXT_SCROLL_WHEEL *)wheel, props->normal_text_color, props->selected_text_color);\n"
               "#else\n"
               "        gx_text_scroll_wheel_text_color_set((GX_TEXT_SCROLL_WHEEL *)wheel, props->normal_text_color, props->selected_text_color, props->disabled_text_color);\n"
               "#endif\n";
    }
    else
    {
        out += "        gx_text_scroll_wheel_text_color_set((GX_TEXT_SCROLL_WHEEL *)wheel, props->normal_text_color, props->selected_text_color);\n";
    }

    out +=     "        if(props->callback)\n"
               "        {\n";
    if (project_lib_version() >= GX_VERSION_STRING_LENGTH_FIX)
    {
        out += "            gx_text_scroll_wheel_callback_set_ext((GX_TEXT_SCROLL_WHEEL *)wheel, (UINT (*)(GX_TEXT_SCROLL_WHEEL*, INT, GX_STRING *))props->callback);\n";
    }
    else
    {
        out += "            gx_text_scroll_wheel_callback_set((GX_TEXT_SCROLL_WHEEL *)wheel, (GX_CONST GX_CHAR *(*)(GX_TEXT_SCROLL_WHEEL*, INT))props->callback);\n";
    }

    out +=     "        }\n"
               "    }\n"
               "    return status;\n"
               "}\n";

    return out;
}

///////////////////////////////////////////////////////////////////////////////
widget_info *string_scroll_wheel_service_provider::CreateNewInstance(GX_WIDGET *parent)
{
    int parent_size;

    GX_RECTANGLE size = parent->gx_widget_size;
    gx_utility_rectangle_define(&size, 0, 0, 79, 23);
    parent_size = (parent->gx_widget_size.gx_rectangle_right -
        parent->gx_widget_size.gx_rectangle_left) / 4;
    size.gx_rectangle_right = size.gx_rectangle_left + parent_size;

    parent_size = (parent->gx_widget_size.gx_rectangle_bottom -
        parent->gx_widget_size.gx_rectangle_top) / 2;
    size.gx_rectangle_bottom = size.gx_rectangle_top + parent_size;

    gx_utility_rectangle_center(&size, &parent->gx_widget_size);

    GX_STRING_SCROLL_WHEEL *wheel = new GX_STRING_SCROLL_WHEEL;
    gx_string_scroll_wheel_create_ext(wheel, "string_scroll_wheel", parent, 0, GX_NULL,
        GX_STYLE_ENABLED | GX_STYLE_BORDER_THIN | GX_STYLE_TEXT_CENTER, 0, &size);

    gx_widget_style_remove((GX_WIDGET *)wheel, GX_STYLE_DRAW_SELECTED);

    widget_info *info = InitWidgetInfo((GX_WIDGET *)wheel);
    info->pixelmap_id[WALLPAPER_PIXELMAP_INDEX] = 0;
    info->ewi.string_scroll_wheel.base.start_alpha = wheel->gx_scroll_wheel_gradient.gx_gradient_alpha_start;
    info->ewi.string_scroll_wheel.base.end_alpha = wheel->gx_scroll_wheel_gradient.gx_gradient_alpha_end;
    info->ewi.string_scroll_wheel.base.row_height = wheel->gx_scroll_wheel_row_height;
    info->ewi.string_scroll_wheel.base.selected_row = wheel->gx_scroll_wheel_selected_row;
    info->ewi.string_scroll_wheel.base.total_rows = 0;
    info->ewi.string_scroll_wheel.string_id_list = GX_NULL;
    info->color_id[NORMAL_TEXT_COLOR_INDEX] = wheel->gx_text_scroll_wheel_normal_text_color;
    info->color_id[SELECTED_TEXT_COLOR_INDEX] = wheel->gx_text_scroll_wheel_selected_text_color;
    info->color_id[DISABLED_TEXT_COLOR_INDEX] = wheel->gx_text_scroll_wheel_disabled_text_color;
    info->font_id[NORMAL_FONT_INDEX] = wheel->gx_text_scroll_wheel_normal_font;
    info->font_id[SELECTED_FONT_INDEX] = wheel->gx_text_scroll_wheel_selected_font;

    return info;
}

///////////////////////////////////////////////////////////////////////////////
GX_WIDGET *string_scroll_wheel_service_provider::GenerateFromInfo(GX_WIDGET *parent, widget_info *info)
{
    GX_STRING_SCROLL_WHEEL *wheel = new GX_STRING_SCROLL_WHEEL;

    // Create scroll wheel widget.
    gx_string_scroll_wheel_create_ext(wheel,
        (CHAR *)info->app_name.GetString(),
        parent,
        info->ewi.string_scroll_wheel.base.total_rows,
        GX_NULL,
        info->style,
        0,
        &info->size);

    wheel->gx_string_scroll_wheel_string_id_list = info->ewi.string_scroll_wheel.string_id_list;

    // Set fill color.
    gx_widget_fill_color_set(wheel,
        info->color_id[NORMAL_FILL_COLOR_INDEX],
        info->color_id[SELECTED_FILL_COLOR_INDEX],
        info->color_id[DISABLED_FILL_COLOR_INDEX]);

    // Set wallpaper.
    if (info->pixelmap_id[WALLPAPER_PIXELMAP_INDEX] > 0)
    {
        BOOL tile = FALSE;

        if (info->style & GX_STYLE_TILE_WALLPAPER)
        {
            tile = TRUE;
        }
        gx_window_wallpaper_set((GX_WINDOW *)wheel, info->pixelmap_id[WALLPAPER_PIXELMAP_INDEX], tile);
    }

    // Set selected row background.
    if (info->pixelmap_id[SCROLL_WHEEL_PIXELMAP_INDEX] > 0)
    {
        gx_scroll_wheel_selected_background_set((GX_SCROLL_WHEEL *)wheel, info->pixelmap_id[SCROLL_WHEEL_PIXELMAP_INDEX]);
    }

    // Set scroll wheel properties.
    gx_text_scroll_wheel_font_set(wheel, info->font_id[0], info->font_id[1]);
    gx_text_scroll_wheel_text_color_set(wheel, info->color_id[NORMAL_TEXT_COLOR_INDEX],
        info->color_id[SELECTED_TEXT_COLOR_INDEX], info->color_id[DISABLED_TEXT_COLOR_INDEX]);
    gx_scroll_wheel_gradient_alpha_set(wheel, info->ewi.string_scroll_wheel.base.start_alpha, info->ewi.string_scroll_wheel.base.end_alpha);
    gx_scroll_wheel_row_height_set(wheel, info->ewi.string_scroll_wheel.base.row_height);
    gx_scroll_wheel_selected_set(wheel, info->ewi.string_scroll_wheel.base.selected_row);

    return (GX_WIDGET *)wheel;
}

///////////////////////////////////////////////////////////////////////////////
void string_scroll_wheel_service_provider::SaveToProject(xml_writer &writer, studiox_project *project, int display, widget_info *info)
{
    text_scroll_wheel_service_provider::SaveToProject(writer, project, display, info);

    if (info->ewi.string_scroll_wheel.string_id_list)
    {
        writer.OpenTag("string_list");
        CString id_name;
        string_table *table = project->mDisplays[display].stable;

        //generate string id list
        for (int index = 0; index < info->ewi.string_scroll_wheel.base.total_rows; index++)
        {
            id_name = table->GetResourceIdName(info->ewi.string_scroll_wheel.string_id_list[index]);
            writer.WriteString("string_id", id_name, TRUE);
        }

        writer.CloseTag("string_list");
    }
}

///////////////////////////////////////////////////////////////////////////////
void string_scroll_wheel_service_provider::ReadFromProject(xml_reader &reader, studiox_project *project, int display, widget_info *info, ULONG valid_styles)
{
    text_scroll_wheel_service_provider::ReadFromProject(reader, project, display, info, valid_styles);

    if (reader.EnterSection("string_list"))
    {
        CString text;
        CArray<GX_RESOURCE_ID> string_list;
        string_table *table = project->mDisplays[display].stable;
        CString id_name;
        int res_id;
        int index;

        if (!table)
        {
            table = new string_table;
            table->Initialize(1, 1);
            project->mDisplays[display].stable = table;
        }

        if (project->mHeader.project_version < 54)
        {
            //read plain string and add it to string table
            for (index = 0; index < info->ewi.string_scroll_wheel.base.total_rows; index++)
            {
                reader.ReadString("val", text);
                res_id = table->CheckAddString(text);
                id_name = table->GetStringId(res_id);
                res_id = table->GetResourceId(id_name);
                string_list.Add(res_id);
            }

            table->Sort();
        }
        else
        {
            //read string id name
            for (index = 0; index < info->ewi.string_scroll_wheel.base.total_rows; index++)
            {
                reader.ReadString("string_id", id_name);
                res_id = table->GetResourceId(id_name);
                string_list.Add(res_id);
            }
        }

        if (info->ewi.string_scroll_wheel.base.total_rows)
        {
            CreateStringIdList(info, string_list);
        }

        reader.CloseSection(TRUE, TRUE);
    }
}

///////////////////////////////////////////////////////////////////////////////
void string_scroll_wheel_service_provider::StringEditEnableDisable(widget_info *info, BOOL enabled)
{
    GX_STRING_SCROLL_WHEEL *wheel = (GX_STRING_SCROLL_WHEEL *) (info->widget);

    if ((!enabled) && info->ewi.string_scroll_wheel.string_id_list)
    {
        delete info->ewi.string_scroll_wheel.string_id_list;
        info->ewi.string_scroll_wheel.string_id_list = NULL;

        if (wheel)
        {
            wheel->gx_string_scroll_wheel_string_list = GX_NULL;
            gx_system_dirty_mark((GX_WIDGET *)wheel);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void string_scroll_wheel_service_provider::AssignScrollWheelInfo(widget_info *info, scroll_wheel_info *wheel_info)
{
    if ((info->ewi.string_scroll_wheel.base.total_rows != wheel_info->total_rows) &&
        (info->callback_func.IsEmpty()))
    {
        CArray<GX_RESOURCE_ID> list_array;

        // init list array with old string list
        InitStringIdListArray(info->ewi.string_scroll_wheel.string_id_list, info->ewi.string_scroll_wheel.base.total_rows, list_array);

        // delete old string list
        delete info->ewi.string_scroll_wheel.string_id_list;
        info->ewi.string_scroll_wheel.string_id_list = NULL;

        scroll_wheel_service_provider::AssignScrollWheelInfo(info, wheel_info);

        // create new string list
        CreateStringIdList(info, list_array);
    }
    else
    {
        scroll_wheel_service_provider::AssignScrollWheelInfo(info, wheel_info);
    }
}

///////////////////////////////////////////////////////////////////////////////
void string_scroll_wheel_service_provider::InitStringIdListArray(GX_RESOURCE_ID *string_list, int total_rows, CArray<GX_RESOURCE_ID> &list_array)
{
    int index;
    CString string;

    for (index = 0; index < total_rows; index++)
    {
        if (string_list && string_list[index])
        {
            list_array.Add(string_list[index]);
        }
        else
        {
            list_array.Add(0);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void string_scroll_wheel_service_provider::CreateStringIdList(widget_info *info, CArray<GX_RESOURCE_ID> &list_array)
{
    int index;
    int total_rows;
    string_scroll_wheel_info *wheel_info = &info->ewi.string_scroll_wheel;

    if (wheel_info->string_id_list)
    {
        return;
    }

    // create new value list
    total_rows = wheel_info->base.total_rows;
    if (total_rows)
    {
        wheel_info->string_id_list = new GX_RESOURCE_ID[total_rows];
        memset(wheel_info->string_id_list, 0, sizeof(GX_RESOURCE_ID)*total_rows);

        if (total_rows > list_array.GetCount())
        {
            total_rows = list_array.GetCount();
        }

        for (index = 0; index < total_rows; index++)
        {
            wheel_info->string_id_list[index] = list_array.GetAt(index);
        }
    }

    if (info->widget)
    {
        GX_STRING_SCROLL_WHEEL *wheel = (GX_STRING_SCROLL_WHEEL *)info->widget;
        wheel->gx_string_scroll_wheel_string_id_list = (const GX_RESOURCE_ID *)wheel_info->string_id_list;
        gx_system_dirty_mark(info->widget);

        if (wheel_info->base.selected_row > wheel_info->base.total_rows)
        {
            wheel_info->base.selected_row = wheel_info->base.total_rows;
            GetPropsWin()->ResourcesChanged();
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void string_scroll_wheel_service_provider::AssignText(widget_info *info, int index, GX_RESOURCE_ID string_id)
{
    if (index < info->ewi.string_scroll_wheel.base.total_rows)
    {
        info->ewi.string_scroll_wheel.string_id_list[index] = string_id;
    }

    if (info->widget)
    {
        gx_system_dirty_mark(info->widget);
    }
}

///////////////////////////////////////////////////////////////////////////////
void string_scroll_wheel_service_provider::Autosize(widget_info *info)
{
    CheckResizeToText(info, RESIZE_MODE_WIDTH);

    window_service_provider::Autosize(info);
}

