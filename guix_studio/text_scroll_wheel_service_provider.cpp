
#include "studiox_includes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

text_scroll_wheel_service_provider::text_scroll_wheel_service_provider()
{
}

CString text_scroll_wheel_service_provider::GetVarDeclaration()
{
    return CString("GX_TEXT_SCROLL_WHEEL_MEMBERS_DECLARE");
}

CString text_scroll_wheel_service_provider::DeclarePropertiesStructContent()
{
    CString out;

    if (project_lib_version() > 50401)
    {
        out =
            "    int                 total_rows;\n"
            "    int                 selected_row;\n"
            "    GX_VALUE            row_height;\n"
            "    GX_UBYTE            start_alpha;\n"
            "    GX_UBYTE            end_alpha;\n"
            "    GX_RESOURCE_ID      normal_font;\n"
            "    GX_RESOURCE_ID      selected_font;\n"
            "    GX_RESOURCE_ID      normal_text_color;\n"
            "    GX_RESOURCE_ID      selected_text_color;\n"
            "    GX_RESOURCE_ID      disabled_text_color;\n"
            "    GX_RESOURCE_ID      wallpaper_id;\n"
            "    GX_RESOURCE_ID      selected_background;";
    }
    else
    {
        out = 
            "    int                 total_rows;\n"
            "    int                 selected_row;\n"
            "    GX_VALUE            row_height;\n"
            "    GX_UBYTE            start_alpha;\n"
            "    GX_UBYTE            end_alpha;\n"
            "    GX_RESOURCE_ID      normal_font;\n"
            "    GX_RESOURCE_ID      selected_font;\n"
            "    GX_RESOURCE_ID      normal_text_color;\n"
            "    GX_RESOURCE_ID      selected_text_color;\n"
            "    GX_RESOURCE_ID      wallpaper_id;\n"
            "    GX_RESOURCE_ID      selected_background;";
    }

    return out;
}

CString text_scroll_wheel_service_provider::DeclarePropertiesStruct()
{
    CString out;
    out.Format(_T("typedef struct\n")
        _T("{\n")
        _T("%s\n")
        _T("} GX_TEXT_SCROLL_WHEEL_PROPERTIES;\n\n"),
        DeclarePropertiesStructContent());
    return out;
}

CString text_scroll_wheel_service_provider::WriteExtendedPropertiesContent(screen_generator *gen, widget_info *info, BOOL add_last_comma)
{
    CString out;
    CString last_delimiter;

    if (add_last_comma)
    {
        last_delimiter = ",";
    }
    else
    {
        last_delimiter = " ";
    }

    if (project_lib_version() > 50401)
    {
        out.Format(
            _T("    %d, /* total rows */\n")
            _T("    %d, /* selected row */\n")
            _T("    %d, /* row height */\n")
            _T("    %d, /* start alpha */\n")
            _T("    %d, /* end alpha */\n")
            _T("    %s, /* normal font id */\n")
            _T("    %s, /* selected font id */\n")
            _T("    %s, /* normal text color id */\n")
            _T("    %s, /* selected text color id */\n")
            _T("    %s, /* disabled text color id */\n")
            _T("    %s, /* wallpaper id */\n")
            _T("    %s%s /* selected background */"),
            info->ewi.scroll_wheel.total_rows,
            info->ewi.scroll_wheel.selected_row,
            info->ewi.scroll_wheel.row_height,
            info->ewi.scroll_wheel.start_alpha,
            info->ewi.scroll_wheel.end_alpha,
            gen->GetFontIdName(info->font_id[NORMAL_FONT_INDEX]),
            gen->GetFontIdName(info->font_id[SELECTED_FONT_INDEX]),
            gen->GetColorIdName(info->color_id[NORMAL_TEXT_COLOR_INDEX]),
            gen->GetColorIdName(info->color_id[SELECTED_TEXT_COLOR_INDEX]),
            gen->GetColorIdName(info->color_id[DISABLED_TEXT_COLOR_INDEX]),
            gen->GetPixelmapIdName(info->pixelmap_id[WALLPAPER_PIXELMAP_INDEX]),
            gen->GetPixelmapIdName(info->pixelmap_id[SCROLL_WHEEL_PIXELMAP_INDEX]), last_delimiter);
    }
    else
    {
        out.Format(
            _T("    %d, /* total rows */\n")
            _T("    %d, /* selected row */\n")
            _T("    %d, /* row height */\n")
            _T("    %d, /* start alpha */\n")
            _T("    %d, /* end alpha */\n")
            _T("    %s, /* normal font id */\n")
            _T("    %s, /* selected font id */\n")
            _T("    %s, /* normal text color id */\n")
            _T("    %s, /* selected text color id */\n")
            _T("    %s, /* wallpaper id */\n")
            _T("    %s%s /* selected background */"),
            info->ewi.scroll_wheel.total_rows,
            info->ewi.scroll_wheel.selected_row,
            info->ewi.scroll_wheel.row_height,
            info->ewi.scroll_wheel.start_alpha,
            info->ewi.scroll_wheel.end_alpha,
            gen->GetFontIdName(info->font_id[NORMAL_FONT_INDEX]),
            gen->GetFontIdName(info->font_id[SELECTED_FONT_INDEX]),
            gen->GetColorIdName(info->color_id[NORMAL_TEXT_COLOR_INDEX]),
            gen->GetColorIdName(info->color_id[SELECTED_TEXT_COLOR_INDEX]),
            gen->GetPixelmapIdName(info->pixelmap_id[WALLPAPER_PIXELMAP_INDEX]),
            gen->GetPixelmapIdName(info->pixelmap_id[SCROLL_WHEEL_PIXELMAP_INDEX]), last_delimiter);
    }

    return out;
}

CString text_scroll_wheel_service_provider::WriteExtendedProperties(screen_generator *gen, CString &prefix, widget_info *info)
{
    CString out;
    CString propname = prefix + info->app_name;

    out.Format(_T("GX_TEXT_SCROLL_WHEEL_PROPERTIES %s_properties =\n")
        _T("{\n")
        _T("%s\n")
        _T("};\n"),
        propname,
        WriteExtendedPropertiesContent(gen, info, TRUE));

    return out;
}


CString text_scroll_wheel_service_provider::GetCreateFromDefFunctionName()
{
    return CString("gx_studio_text_scroll_wheel_create");
}

CString text_scroll_wheel_service_provider::GetCreateFromDefFunction(int version)
{
    CString out;
    MakeCreatePreamble("text_scroll_wheel", version, out);

    out += "{\n"
        "    UINT status;\n"
        "    GX_TEXT_SCROLL_WHEEL *wheel = (GX_TEXT_SCROLL_WHEEL *) control_block;\n"
        "    GX_TEXT_SCROLL_WHEEL_PROPERTIES *props = (GX_TEXT_SCROLL_WHEEL_PROPERTIES *) info->properties;\n"
        "    status = gx_text_scroll_wheel_create(wheel, info->widget_name, parent, props->total_rows, props->string_list, props->callback, \n"
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
        "        }\n"
        "        gx_scroll_wheel_selected_set((GX_SCROLL_WHEEL *)wheel, props->selected_row);\n"
        "        gx_scroll_wheel_gradient_alpha_set((GX_SCROLL_WHEEL *)wheel, props->start_alpha, props->end_alpha);\n"
        "        gx_scroll_wheel_row_height_set((GX_SCROLL_WHEEL *)wheel, props->row_height);\n"
        "        gx_text_scroll_wheel_font_set(wheel, props->normal_font, props->selected_font);\n";

        if (project_lib_version() > 50401)
        {
            out += "#if defined(GUIX_5_4_0_COMPATIBILITY)\n"
            "        gx_text_scroll_wheel_text_color_set(wheel, props->normal_text_color, props->selected_text_color);\n"
            "#else\n"
            "        gx_text_scroll_wheel_text_color_set(wheel, props->normal_text_color, props->selected_text_color, props->disabled_text_color);\n"
            "#endif\n";
        }
        else
        {
            out += "        gx_text_scroll_wheel_text_color_set(wheel, props->normal_text_color, props->selected_text_color);\n";
        }

    out += "    }\n"
        "    return status;\n"
        "}\n";
    return out;
}


widget_info *text_scroll_wheel_service_provider::CreateNewInstance(GX_WIDGET *parent)
{
    int parent_size;

    GX_RECTANGLE size = parent->gx_widget_size;
    gx_utility_rectangle_define(&size, 0, 0, 79, 23);
    parent_size = (parent->gx_widget_size.gx_rectangle_right -
                   parent->gx_widget_size.gx_rectangle_left) / 2;
    size.gx_rectangle_right = size.gx_rectangle_left + parent_size;

    parent_size = (parent->gx_widget_size.gx_rectangle_bottom -
                   parent->gx_widget_size.gx_rectangle_top) / 2;
    size.gx_rectangle_bottom = size.gx_rectangle_top + parent_size;

    gx_utility_rectangle_center(&size, &parent->gx_widget_size);
    
    GX_TEXT_SCROLL_WHEEL *wheel = new GX_TEXT_SCROLL_WHEEL;
    gx_text_scroll_wheel_create(wheel, "text_scroll_wheel", parent, 0,
        GX_STYLE_ENABLED | GX_STYLE_BORDER_THIN | GX_STYLE_TEXT_CENTER, 0, &size);

    widget_info *info = InitWidgetInfo((GX_WIDGET *) wheel);
    info->pixelmap_id[WALLPAPER_PIXELMAP_INDEX] = 0;
    info->ewi.scroll_wheel.start_alpha = wheel->gx_scroll_wheel_gradient.gx_gradient_alpha_start;
    info->ewi.scroll_wheel.end_alpha = wheel->gx_scroll_wheel_gradient.gx_gradient_alpha_end;
    info->ewi.scroll_wheel.row_height = wheel->gx_scroll_wheel_row_height;
    info->ewi.scroll_wheel.selected_row = wheel->gx_scroll_wheel_selected_row;
    info->ewi.scroll_wheel.total_rows = 0;
    info->color_id[NORMAL_TEXT_COLOR_INDEX] = wheel->gx_text_scroll_wheel_normal_text_color;
    info->color_id[SELECTED_TEXT_COLOR_INDEX] = wheel->gx_text_scroll_wheel_selected_text_color;
    info->color_id[DISABLED_TEXT_COLOR_INDEX] = wheel->gx_text_scroll_wheel_disabled_text_color;
    info->font_id[NORMAL_FONT_INDEX] = wheel->gx_text_scroll_wheel_normal_font;
    info->font_id[SELECTED_FONT_INDEX] = wheel->gx_text_scroll_wheel_selected_font;

    return info;
}

GX_WIDGET *text_scroll_wheel_service_provider::GenerateFromInfo(GX_WIDGET *parent, widget_info *info)
{
    GX_TEXT_SCROLL_WHEEL *wheel = new GX_TEXT_SCROLL_WHEEL;

    // Create scroll wheel widget.
    gx_text_scroll_wheel_create(wheel,
        (CHAR *)info->app_name.GetString(),
        parent,
        info->ewi.scroll_wheel.total_rows,
        info->style,
        0,
        &info->size);

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
    gx_text_scroll_wheel_text_color_set(wheel, info->color_id[NORMAL_TEXT_COLOR_INDEX], info->color_id[SELECTED_TEXT_COLOR_INDEX],
        info->color_id[DISABLED_TEXT_COLOR_INDEX]);
    gx_scroll_wheel_gradient_alpha_set(wheel, info->ewi.scroll_wheel.start_alpha, info->ewi.scroll_wheel.end_alpha);
    gx_scroll_wheel_row_height_set(wheel, info->ewi.scroll_wheel.row_height);
    gx_scroll_wheel_selected_set(wheel, info->ewi.scroll_wheel.selected_row);

    return (GX_WIDGET *)wheel;
}

void text_scroll_wheel_service_provider::SaveToProject(xml_writer &writer, studiox_project *project, int display, widget_info *info)
{
    scroll_wheel_service_provider::SaveToProject(writer, project, display, info);

    WriteColorId(writer, project, display, "normal_text_color", info->color_id[NORMAL_TEXT_COLOR_INDEX]);
    WriteColorId(writer, project, display, "selected_text_color", info->color_id[SELECTED_TEXT_COLOR_INDEX]);
    WriteColorId(writer, project, display, "disabled_text_color", info->color_id[DISABLED_TEXT_COLOR_INDEX]);
    WriteFontId(writer, project, display, "normal_font_id", info->font_id[NORMAL_FONT_INDEX]);
    WriteFontId(writer, project, display, "selected_font_id", info->font_id[SELECTED_FONT_INDEX]);
    writer.WriteString("callback", info->callback_func);
}

void text_scroll_wheel_service_provider::ReadFromProject(xml_reader &reader, studiox_project *project, int display, widget_info *info, ULONG valid_styles)
{
    valid_styles |= GX_STYLE_TEXT_SCROLL_WHEEL_ROUND;
    valid_styles |= GX_STYLE_TEXT_ALIGNMENT_MASK|GX_STYLE_TEXT_COPY;

    scroll_wheel_service_provider::ReadFromProject(reader, project, display, info, valid_styles);

    if (project->mDisplays[display].colorformat <= GX_COLOR_FORMAT_8BIT_PALETTE)
    {
        // Remove round style
        info->style &= (~GX_STYLE_TEXT_SCROLL_WHEEL_ROUND);
    }
    
    info->color_id[NORMAL_TEXT_COLOR_INDEX] = ReadColorId(reader, project, display, "normal_text_color");
    info->color_id[SELECTED_TEXT_COLOR_INDEX] = ReadColorId(reader, project, display, "selected_text_color");

    if (project->mHeader.project_version > 54)
    {
        info->color_id[DISABLED_TEXT_COLOR_INDEX] = ReadColorId(reader, project, display, "disabled_text_color");
    }
    else
    {
        info->color_id[DISABLED_TEXT_COLOR_INDEX] = info->color_id[NORMAL_TEXT_COLOR_INDEX];
    }

    info->font_id[NORMAL_FONT_INDEX] = ReadFontId(reader, project, display, "normal_font_id");
    info->font_id[SELECTED_FONT_INDEX] = ReadFontId(reader, project, display, "selected_font_id");
    reader.ReadString("callback", info->callback_func);
}

void text_scroll_wheel_service_provider::AssignColor(widget_info *info, int index, GX_RESOURCE_ID color_id)
{
    GX_TEXT_SCROLL_WHEEL *wheel = (GX_TEXT_SCROLL_WHEEL *) (info->widget);

    switch (index)
    {
    case NORMAL_TEXT_COLOR_INDEX:
        if (wheel)
        {
            gx_text_scroll_wheel_text_color_set(wheel, color_id, wheel->gx_text_scroll_wheel_selected_text_color,
                wheel->gx_text_scroll_wheel_disabled_text_color);
        }

        info->color_id[NORMAL_TEXT_COLOR_INDEX] = color_id;
        break;

    case SELECTED_TEXT_COLOR_INDEX:
        if (wheel)
        {
            gx_text_scroll_wheel_text_color_set(wheel, wheel->gx_text_scroll_wheel_normal_text_color, color_id,
                wheel->gx_text_scroll_wheel_disabled_text_color);
        }

        info->color_id[SELECTED_TEXT_COLOR_INDEX] = color_id;
        break;

    case DISABLED_TEXT_COLOR_INDEX:
        if (wheel)
        {
            gx_text_scroll_wheel_text_color_set(wheel, wheel->gx_text_scroll_wheel_normal_text_color,
                wheel->gx_text_scroll_wheel_selected_text_color, color_id);
        }

        info->color_id[DISABLED_TEXT_COLOR_INDEX] = color_id;
        break;

    default:
        widget_service_provider::AssignColor(info, index, color_id);
    }
}


void text_scroll_wheel_service_provider::AssignFont(widget_info *info, int index, GX_RESOURCE_ID font_id)
{
    GX_TEXT_SCROLL_WHEEL *wheel = (GX_TEXT_SCROLL_WHEEL *)(info->widget);

    switch (index)
    {
    case NORMAL_FONT_INDEX:
        if (wheel)
        {
            gx_text_scroll_wheel_font_set(wheel, font_id, wheel->gx_text_scroll_wheel_selected_font);
        }

        info->font_id[NORMAL_FONT_INDEX] = font_id;
        break;

    case SELECTED_FONT_INDEX:
        if (wheel)
        {
            gx_text_scroll_wheel_font_set(wheel, wheel->gx_text_scroll_wheel_normal_font, font_id);
        }

        info->font_id[SELECTED_FONT_INDEX] = font_id;
        break;
    }
}