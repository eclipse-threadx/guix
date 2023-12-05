
#include "studiox_includes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
rich_text_view_service_provider::rich_text_view_service_provider()
{
}

///////////////////////////////////////////////////////////////////////////////
CString rich_text_view_service_provider::GetVarDeclaration()
{
    return CString("GX_RICH_TEXT_VIEW_MEMBERS_DECLARE");
}

///////////////////////////////////////////////////////////////////////////////
CString rich_text_view_service_provider::DeclarePropertiesStruct()
{
    CString out;

    out = (""
        "typedef struct\n"
        "{\n"
        "    GX_RESOURCE_ID      string_id;\n"
        "    GX_RESOURCE_ID      normal_text_color_id;\n"
        "    GX_RESOURCE_ID      selected_text_color_id;\n"
        "    GX_RESOURCE_ID      disabled_text_color_id;\n"
        "    GX_UBYTE            whitespace;\n"
        "    GX_BYTE             line_space;\n"
        "    GX_RICH_TEXT_FONTS *fonts;\n"
        "} GX_RICH_TEXT_VIEW_PROPERTIES;\n\n");

    return out;
}

///////////////////////////////////////////////////////////////////////////////
CString rich_text_view_service_provider::WriteExtendedProperties(screen_generator *gen, CString &prefix, widget_info *info)
{
    CString out;
    CString temp;
    CString propname = prefix + info->app_name;

    // generate rich text font
    out.Format(_T("GX_RICH_TEXT_FONTS %s_fonts =\n")
        _T("{\n")
        _T("    %s, /* normal font. */\n")
        _T("    %s, /* bold font. */\n")
        _T("    %s, /* italic font. */\n")
        _T("    %s  /* bold italic font. */\n")
        _T("};\n\n"),
        propname,
        gen->GetFontIdName(info->font_id[RICH_TEXT_FONT_NORMAL]),
        gen->GetFontIdName(info->font_id[RICH_TEXT_FONT_BOLD]),
        gen->GetFontIdName(info->font_id[RICH_TEXT_FONT_ITALIC]),
        gen->GetFontIdName(info->font_id[RICH_TEXT_FONT_BOLD_ITALIC]));

    temp.Format(_T("GX_RICH_TEXT_VIEW_PROPERTIES %s_properties =\n")
        _T("{\n")
        _T("    %s, /* string id */\n")
        _T("    %s, /* normal text color */\n")
        _T("    %s, /* selected text color */\n")
        _T("    %s, /* disabled text color */\n")
        _T("    %d, /* whitespace */\n")
        _T("    %d, /* line_space */\n")
        _T("    &%s_fonts, /* fonts */\n};\n"),
        propname,
        gen->GetStringIdName(info->string_id[0]),
        gen->GetColorIdName(info->color_id[NORMAL_TEXT_COLOR_INDEX]),
        gen->GetColorIdName(info->color_id[SELECTED_TEXT_COLOR_INDEX]),
        gen->GetColorIdName(info->color_id[DISABLED_TEXT_COLOR_INDEX]),
        info->ewi.text_info.whitespace,
        info->ewi.text_info.line_space,
        propname);

    out += temp;

    return out;
}

///////////////////////////////////////////////////////////////////////////////
CString rich_text_view_service_provider::GetCreateFromDefFunctionName()
{
    return CString("gx_studio_rich_text_view_create");
}

///////////////////////////////////////////////////////////////////////////////
CString rich_text_view_service_provider::GetCreateFromDefFunction(int version)
{
    CString out;
    MakeCreatePreamble("rich_text_view", version, out);

    out += "{\n"
        "    UINT status;\n"
        "    GX_RICH_TEXT_VIEW *view = (GX_RICH_TEXT_VIEW *) control_block;\n"
        "    GX_RICH_TEXT_VIEW_PROPERTIES *props = (GX_RICH_TEXT_VIEW_PROPERTIES *) info->properties;\n"
        "    status = gx_rich_text_view_create(view, info->widget_name, parent, props->string_id, props->fonts, info->style, info->widget_id, &info->size);\n"
        "    if (status == GX_SUCCESS)\n"
        "    {\n"
        "        gx_multi_line_text_view_text_color_set((GX_MULTI_LINE_TEXT_VIEW *)view, props->normal_text_color_id, props->selected_text_color_id, props->disabled_text_color_id);\n"
        "        gx_multi_line_text_view_whitespace_set((GX_MULTI_LINE_TEXT_VIEW *)view, props->whitespace);\n"
        "        gx_multi_line_text_view_line_space_set((GX_MULTI_LINE_TEXT_VIEW *)view, props->line_space);\n"
        "    }\n"
        "    return status;\n"
        "}\n";

    return out;
}

///////////////////////////////////////////////////////////////////////////////
widget_info *rich_text_view_service_provider::CreateNewInstance(GX_WIDGET *parent)
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

    GX_RICH_TEXT_VIEW *view = new GX_RICH_TEXT_VIEW;
    GX_RESOURCE_ID string_id = GetStringId("rich_text_view");
    GX_RICH_TEXT_FONTS fonts;

    fonts.gx_rich_text_fonts_normal_id = GX_FONT_ID_DEFAULT;
    fonts.gx_rich_text_fonts_bold_id = GX_FONT_ID_DEFAULT;
    fonts.gx_rich_text_fonts_italic_id = GX_FONT_ID_DEFAULT;
    fonts.gx_rich_text_fonts_bold_italic_id = GX_FONT_ID_DEFAULT;

    gx_rich_text_view_create(view, "rich_text_view", parent, string_id, &fonts,
        GX_STYLE_ENABLED | GX_STYLE_BORDER_THIN | GX_STYLE_TEXT_CENTER, 0, &size);
    gx_widget_style_remove(view, GX_STYLE_DRAW_SELECTED);
    widget_info *info = InitWidgetInfo((GX_WIDGET *)view);

    info->color_id[NORMAL_TEXT_COLOR_INDEX] = view->gx_multi_line_text_view_normal_text_color;
    info->color_id[SELECTED_TEXT_COLOR_INDEX] = view->gx_multi_line_text_view_selected_text_color;
    info->color_id[DISABLED_TEXT_COLOR_INDEX] = view->gx_multi_line_text_view_disabled_text_color;
    info->string_id[0] = string_id;
    info->font_id[RICH_TEXT_FONT_NORMAL] = fonts.gx_rich_text_fonts_normal_id;
    info->font_id[RICH_TEXT_FONT_BOLD] = fonts.gx_rich_text_fonts_bold_id;
    info->font_id[RICH_TEXT_FONT_ITALIC] = fonts.gx_rich_text_fonts_italic_id;
    info->font_id[RICH_TEXT_FONT_BOLD_ITALIC] = fonts.gx_rich_text_fonts_bold_italic_id;
    info->ewi.text_info.whitespace = 0;
    info->ewi.text_info.line_space = 0;
    return info;
}

///////////////////////////////////////////////////////////////////////////////
GX_WIDGET *rich_text_view_service_provider::GenerateFromInfo(GX_WIDGET *parent, widget_info *info)
{
    GX_RICH_TEXT_VIEW *view = new GX_RICH_TEXT_VIEW;
    GX_RICH_TEXT_FONTS fonts;

    fonts.gx_rich_text_fonts_normal_id = info->font_id[RICH_TEXT_FONT_NORMAL];
    fonts.gx_rich_text_fonts_bold_id = info->font_id[RICH_TEXT_FONT_BOLD];
    fonts.gx_rich_text_fonts_italic_id = info->font_id[RICH_TEXT_FONT_ITALIC];
    fonts.gx_rich_text_fonts_bold_italic_id = info->font_id[RICH_TEXT_FONT_BOLD_ITALIC];

    gx_rich_text_view_create(view,
        (CHAR *)info->app_name.GetString(),
        parent,
        info->string_id[0],
        &fonts,
        info->style, 0,
        &info->size);

    gx_widget_fill_color_set(view,
        info->color_id[NORMAL_FILL_COLOR_INDEX],
        info->color_id[SELECTED_FILL_COLOR_INDEX],
        info->color_id[DISABLED_FILL_COLOR_INDEX]);

    gx_multi_line_text_view_text_color_set((GX_MULTI_LINE_TEXT_VIEW *)view,
        info->color_id[NORMAL_TEXT_COLOR_INDEX],
        info->color_id[SELECTED_TEXT_COLOR_INDEX],
        info->color_id[DISABLED_TEXT_COLOR_INDEX]);

    gx_multi_line_text_view_whitespace_set((GX_MULTI_LINE_TEXT_VIEW *)view, info->ewi.text_info.whitespace);
    gx_multi_line_text_view_line_space_set((GX_MULTI_LINE_TEXT_VIEW *)view, info->ewi.text_info.line_space);

    return ((GX_WIDGET *)view);
}

///////////////////////////////////////////////////////////////////////////////
void rich_text_view_service_provider::SaveToProject(xml_writer &writer, studiox_project *project, int display, widget_info *info)
{
    widget_service_provider::SaveToProject(writer, project, display, info);
    WriteStringId(writer, project, display, "string_id", info->string_id[0]);

    WriteFontId(writer, project, display, "normal_font", info->font_id[RICH_TEXT_FONT_NORMAL]);
    WriteFontId(writer, project, display, "bold_font", info->font_id[RICH_TEXT_FONT_BOLD]);
    WriteFontId(writer, project, display, "italic_font", info->font_id[RICH_TEXT_FONT_ITALIC]);
    WriteFontId(writer, project, display, "bold_italic_font", info->font_id[RICH_TEXT_FONT_BOLD_ITALIC]);

    WriteColorId(writer, project, display, "normal_text_color", info->color_id[NORMAL_TEXT_COLOR_INDEX]);
    WriteColorId(writer, project, display, "selected_text_color", info->color_id[SELECTED_TEXT_COLOR_INDEX]);
    WriteColorId(writer, project, display, "disabled_fill_color", info->color_id[DISABLED_FILL_COLOR_INDEX]);
    WriteColorId(writer, project, display, "disabled_text_color", info->color_id[DISABLED_TEXT_COLOR_INDEX]);
    writer.WriteInt("whitespace", info->ewi.text_info.whitespace);
    writer.WriteInt("line_space", info->ewi.text_info.line_space);
}

///////////////////////////////////////////////////////////////////////////////
void rich_text_view_service_provider::ReadFromProject(xml_reader &reader, studiox_project *project, int display, widget_info *info, ULONG valid_styles)
{
    valid_styles |= GX_STYLE_TEXT_ALIGNMENT_MASK | GX_STYLE_TEXT_COPY;

    widget_service_provider::ReadFromProject(reader, project, display, info, valid_styles);
    info->string_id[0] = ReadStringId(reader, project, display, "string_id");
    if (info->string_id[0] < 0 || info->string_id[0] > 100000)
    {
        info->string_id[0] = 0;
    }

    info->font_id[RICH_TEXT_FONT_NORMAL] = ReadFontId(reader, project, display, "normal_font");
    info->font_id[RICH_TEXT_FONT_BOLD] = ReadFontId(reader, project, display, "bold_font");
    info->font_id[RICH_TEXT_FONT_ITALIC ] = ReadFontId(reader, project, display, "italic_font");
    info->font_id[RICH_TEXT_FONT_BOLD_ITALIC] = ReadFontId(reader, project, display, "bold_italic_font");

    info->color_id[NORMAL_TEXT_COLOR_INDEX] = ReadColorId(reader, project, display, "normal_text_color");
    info->color_id[SELECTED_TEXT_COLOR_INDEX] = ReadColorId(reader, project, display, "selected_text_color");
    info->color_id[DISABLED_TEXT_COLOR_INDEX] = ReadColorId(reader, project, display, "disabled_text_color");
    reader.ReadInt("whitespace", info->ewi.text_info.whitespace, 0);
    reader.ReadInt("line_space", info->ewi.text_info.line_space, 0);
}


///////////////////////////////////////////////////////////////////////////////
void rich_text_view_service_provider::AssignFont(widget_info *info, int index, GX_RESOURCE_ID font_id)
{
    GX_WIDGET *widget = info->widget;

    info->font_id[index] = font_id;

    if (widget)
    {
        GX_RICH_TEXT_VIEW *view = (GX_RICH_TEXT_VIEW *)widget;

        GX_RICH_TEXT_FONTS fonts = view->gx_rich_text_view_fonts;
        
        switch (index)
        {
        case RICH_TEXT_FONT_NORMAL:
            fonts.gx_rich_text_fonts_normal_id = font_id;
            break;

        case RICH_TEXT_FONT_BOLD:
            fonts.gx_rich_text_fonts_bold_id = font_id;
            break;

        case RICH_TEXT_FONT_ITALIC:
            fonts.gx_rich_text_fonts_italic_id = font_id;
            break;
            
        case RICH_TEXT_FONT_BOLD_ITALIC:
            fonts.gx_rich_text_fonts_bold_italic_id = font_id;
            break;
        }
        gx_rich_text_view_fonts_set((GX_RICH_TEXT_VIEW *)widget, &fonts);
    }
}


///////////////////////////////////////////////////////////////////////////////
void rich_text_view_service_provider::AssignText(widget_info *info, int index,
    GX_RESOURCE_ID string_id)
{
    info->string_id[0] = string_id;
    GX_WIDGET *widget = info->widget;

    if (widget)
    {
        GX_MULTI_LINE_TEXT_VIEW *view = (GX_MULTI_LINE_TEXT_VIEW *)widget;
        gx_multi_line_text_view_text_id_set(view, string_id);
    }
}

///////////////////////////////////////////////////////////////////////////////
void rich_text_view_service_provider::AssignColor(widget_info *info, int index, GX_RESOURCE_ID color_id)
{
    GX_MULTI_LINE_TEXT_VIEW *view = (GX_MULTI_LINE_TEXT_VIEW *)(info->widget);

    switch (index)
    {
    case NORMAL_TEXT_COLOR_INDEX:
        if (view)
        {
            gx_multi_line_text_view_text_color_set(view, color_id, view->gx_multi_line_text_view_selected_text_color, view->gx_multi_line_text_view_disabled_text_color);
        }

        info->color_id[NORMAL_TEXT_COLOR_INDEX] = color_id;
        break;

    case SELECTED_TEXT_COLOR_INDEX:
        if (view)
        {
            gx_multi_line_text_view_text_color_set(view, view->gx_multi_line_text_view_normal_text_color, color_id, view->gx_multi_line_text_view_disabled_text_color);
        }

        info->color_id[SELECTED_TEXT_COLOR_INDEX] = color_id;
        break;

    case DISABLED_TEXT_COLOR_INDEX:
        if (view)
        {
            gx_multi_line_text_view_text_color_set(view, view->gx_multi_line_text_view_normal_text_color, view->gx_multi_line_text_view_selected_text_color, color_id);
        }
        info->color_id[DISABLED_TEXT_COLOR_INDEX] = color_id;
        break;

    default:
        window_service_provider::AssignColor(info, index, color_id);
    }
}