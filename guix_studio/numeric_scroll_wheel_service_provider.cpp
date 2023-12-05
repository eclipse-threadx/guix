
#include "studiox_includes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
numeric_scroll_wheel_service_provider::numeric_scroll_wheel_service_provider()
{
}


///////////////////////////////////////////////////////////////////////////////
CString numeric_scroll_wheel_service_provider::GetVarDeclaration()
{
    return CString("GX_STRING_SCROLL_WHEEL_MEMBERS_DECLARE");
}

///////////////////////////////////////////////////////////////////////////////
CString numeric_scroll_wheel_service_provider::DeclarePropertiesStruct()
{
    CString out;
    CString callback;

    if (project_lib_version() >= GX_VERSION_STRING_LENGTH_FIX)
    {
        callback = "UINT (*callback)(struct GX_NUMERIC_SCROLL_WHEEL_STRUCT *, INT, GX_STRING *)";
    }
    else
    {
        callback = "GX_CONST GX_CHAR * (*callback)(struct GX_NUMERIC_SCROLL_WHEEL_STRUCT *, INT)";
    }

    out.Format(
        _T("typedef struct\n")
        _T("{\n")
        _T("%s\n")
        _T("    %s;\n")
        _T("    int                 start_val;\n")
        _T("    int                 end_val;\n")
        _T("} GX_NUMERIC_SCROLL_WHEEL_PROPERTIES;\n\n"),
        text_scroll_wheel_service_provider::DeclarePropertiesStructContent(),
        callback);

    return out;
}

///////////////////////////////////////////////////////////////////////////////
CString numeric_scroll_wheel_service_provider::WriteExtendedProperties(screen_generator *gen, CString &prefix, widget_info *info)
{
    CString out;
    CString propname = prefix + info->app_name;
    CString callback;

    if (info->callback_func.IsEmpty())
    {
        callback = "GX_NULL";
    }
    else
    {
        callback = info->callback_func;
    }

        out.Format(_T("GX_NUMERIC_SCROLL_WHEEL_PROPERTIES %s_properties =\n")
            _T("{\n")
            _T("%s\n")
            _T("    %s, /* format callback */\n")
            _T("    %d, /* start val */\n")
            _T("    %d, /* end val */\n")
            _T("};\n"),
            propname,
            text_scroll_wheel_service_provider::WriteExtendedPropertiesContent(gen, info, TRUE),
            callback,
            info->ewi.numeric_scroll_wheel.start_val,
            info->ewi.numeric_scroll_wheel.end_val);

    return out;
}


///////////////////////////////////////////////////////////////////////////////
CString numeric_scroll_wheel_service_provider::GetCreateFromDefFunctionName()
{
    return CString("gx_studio_numeric_scroll_wheel_create");
}

///////////////////////////////////////////////////////////////////////////////
CString numeric_scroll_wheel_service_provider::GetCreateFromDefFunction(int version)
{
    CString out;
    MakeCreatePreamble("numeric_scroll_wheel", version, out);

        out += "{\n"
            "    UINT status;\n"
            "    GX_NUMERIC_SCROLL_WHEEL *wheel = (GX_NUMERIC_SCROLL_WHEEL *) control_block;\n"
            "    GX_NUMERIC_SCROLL_WHEEL_PROPERTIES *props = (GX_NUMERIC_SCROLL_WHEEL_PROPERTIES *) info->properties;\n"
            "    status = gx_numeric_scroll_wheel_create(wheel, info->widget_name, parent, props->start_val, props->end_val, \n"
            "                                    info->style, info->widget_id, &info->size);\n"
            "    if (status == GX_SUCCESS)\n"
            "    {\n"
            "        if (props->wallpaper_id)\n"
            "        {\n"
            "            gx_window_wallpaper_set((GX_WINDOW *) wheel, props->wallpaper_id, info->style & GX_STYLE_TILE_WALLPAPER);\n"
            "        }\n"
            "        if(props->selected_background)\n"
            "        {\n"
            "            gx_scroll_wheel_selected_background_set((GX_SCROLL_WHEEL *)wheel, props->selected_background);\n"
            "        }\n\n"
            "        if (props->total_rows)\n"
            "        {\n"
            "            gx_scroll_wheel_total_rows_set((GX_SCROLL_WHEEL *)wheel, props->total_rows);\n"
            "        }\n\n"
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
widget_info *numeric_scroll_wheel_service_provider::CreateNewInstance(GX_WIDGET *parent)
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

    GX_NUMERIC_SCROLL_WHEEL *wheel = new GX_NUMERIC_SCROLL_WHEEL;
    gx_numeric_scroll_wheel_create(wheel, "numeric_scroll_wheel", parent, 0, 10,
        GX_STYLE_ENABLED | GX_STYLE_BORDER_THIN | GX_STYLE_TEXT_CENTER | GX_STYLE_WRAP, 0, &size);

    gx_widget_style_remove((GX_WIDGET *)wheel, GX_STYLE_DRAW_SELECTED);

    widget_info *info = InitWidgetInfo((GX_WIDGET *)wheel);
    info->pixelmap_id[WALLPAPER_PIXELMAP_INDEX] = 0;
    info->ewi.numeric_scroll_wheel.base.start_alpha = wheel->gx_scroll_wheel_gradient.gx_gradient_alpha_start;
    info->ewi.numeric_scroll_wheel.base.end_alpha = wheel->gx_scroll_wheel_gradient.gx_gradient_alpha_end;
    info->ewi.numeric_scroll_wheel.base.row_height = wheel->gx_scroll_wheel_row_height;
    info->ewi.numeric_scroll_wheel.base.selected_row = wheel->gx_scroll_wheel_selected_row;
    info->ewi.numeric_scroll_wheel.base.total_rows = wheel->gx_scroll_wheel_total_rows;
    info->ewi.numeric_scroll_wheel.start_val = wheel->gx_numeric_scroll_wheel_start_val;
    info->ewi.numeric_scroll_wheel.end_val = wheel->gx_numeric_scroll_wheel_end_val;
    info->color_id[NORMAL_TEXT_COLOR_INDEX] = wheel->gx_text_scroll_wheel_normal_text_color;
    info->color_id[SELECTED_TEXT_COLOR_INDEX] = wheel->gx_text_scroll_wheel_selected_text_color;
    info->color_id[DISABLED_TEXT_COLOR_INDEX] = wheel->gx_text_scroll_wheel_disabled_text_color;
    info->font_id[NORMAL_FONT_INDEX] = wheel->gx_text_scroll_wheel_normal_font;
    info->font_id[SELECTED_FONT_INDEX] = wheel->gx_text_scroll_wheel_selected_font;

    return info;
}

///////////////////////////////////////////////////////////////////////////////
GX_WIDGET *numeric_scroll_wheel_service_provider::GenerateFromInfo(GX_WIDGET *parent, widget_info *info)
{
    GX_NUMERIC_SCROLL_WHEEL *wheel = new GX_NUMERIC_SCROLL_WHEEL;

    // Create scroll wheel widget.
    gx_numeric_scroll_wheel_create(wheel,
        (CHAR *)info->app_name.GetString(),
        parent,
        info->ewi.numeric_scroll_wheel.start_val,
        info->ewi.numeric_scroll_wheel.end_val,
        info->style,
        0,
        &info->size);

    // Set fill color.
    gx_widget_fill_color_set(wheel,
        info->color_id[NORMAL_FILL_COLOR_INDEX],
        info->color_id[SELECTED_FILL_COLOR_INDEX], info->color_id[DISABLED_FILL_COLOR_INDEX]);

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
    gx_text_scroll_wheel_font_set((GX_TEXT_SCROLL_WHEEL *)wheel, info->font_id[0], info->font_id[1]);
    gx_text_scroll_wheel_text_color_set((GX_TEXT_SCROLL_WHEEL *)wheel, info->color_id[NORMAL_TEXT_COLOR_INDEX],
        info->color_id[SELECTED_TEXT_COLOR_INDEX], info->color_id[DISABLED_TEXT_COLOR_INDEX]);
    gx_scroll_wheel_gradient_alpha_set((GX_SCROLL_WHEEL *)wheel, info->ewi.numeric_scroll_wheel.base.start_alpha, info->ewi.numeric_scroll_wheel.base.end_alpha);
    gx_scroll_wheel_row_height_set((GX_SCROLL_WHEEL *) wheel, info->ewi.numeric_scroll_wheel.base.row_height);
    gx_scroll_wheel_selected_set((GX_SCROLL_WHEEL *)wheel, info->ewi.numeric_scroll_wheel.base.selected_row);

    if (info->ewi.numeric_scroll_wheel.base.total_rows > 0)
    {
        gx_scroll_wheel_total_rows_set((GX_SCROLL_WHEEL *)wheel, info->ewi.numeric_scroll_wheel.base.total_rows);
    }

    return (GX_WIDGET *)wheel;
}

///////////////////////////////////////////////////////////////////////////////
void numeric_scroll_wheel_service_provider::SaveToProject(xml_writer &writer, studiox_project *project, int display, widget_info *info)
{
    text_scroll_wheel_service_provider::SaveToProject(writer, project, display, info);

    writer.WriteInt("start_val", info->ewi.numeric_scroll_wheel.start_val);
    writer.WriteInt("end_val", info->ewi.numeric_scroll_wheel.end_val);
}

///////////////////////////////////////////////////////////////////////////////
void numeric_scroll_wheel_service_provider::ReadFromProject(xml_reader &reader, studiox_project *project, int display, widget_info *info, ULONG valid_styles)
{
    text_scroll_wheel_service_provider::ReadFromProject(reader, project, display, info, valid_styles);

    reader.ReadInt("start_val", info->ewi.numeric_scroll_wheel.start_val);
    reader.ReadInt("end_val", info->ewi.numeric_scroll_wheel.end_val);
}

///////////////////////////////////////////////////////////////////////////////
void numeric_scroll_wheel_service_provider::AssignValueRange(widget_info *info, int start_val, int end_val)
{
    info->ewi.numeric_scroll_wheel.start_val = start_val;
    info->ewi.numeric_scroll_wheel.end_val = end_val;

    GX_NUMERIC_SCROLL_WHEEL *wheel = (GX_NUMERIC_SCROLL_WHEEL *) (info->widget);

    if (wheel)
    {
        gx_numeric_scroll_wheel_range_set(wheel, start_val, end_val);
    }
}
