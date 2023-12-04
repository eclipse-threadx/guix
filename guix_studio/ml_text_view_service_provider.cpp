
#include "studiox_includes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

ml_text_view_service_provider::ml_text_view_service_provider()
{
}

CString ml_text_view_service_provider::GetVarDeclaration()
{
    return CString("GX_MULTI_LINE_TEXT_VIEW_MEMBERS_DECLARE");
}

CString ml_text_view_service_provider::DeclarePropertiesStruct()
{
CString out;

    if (project_lib_version() >= GX_WHITESPACE_IS_UBYTE)
    {
        out = (""
            "typedef struct\n"
            "{\n"
            "    GX_RESOURCE_ID string_id;\n"
            "    GX_RESOURCE_ID font_id;\n"
            "    GX_RESOURCE_ID normal_text_color_id;\n"
            "    GX_RESOURCE_ID selected_text_color_id;\n"
            "    GX_RESOURCE_ID disabled_text_color_id;\n"
            "    GX_UBYTE       whitespace;\n"
            "    GX_BYTE        line_space;\n"
            "} GX_ML_TEXT_VIEW_PROPERTIES;\n\n");
    }
    else if (project_lib_version() > 50401)
    {
        out = (""
            "typedef struct\n"
            "{\n"
            "    GX_RESOURCE_ID string_id;\n"
            "    GX_RESOURCE_ID font_id;\n"
            "    GX_RESOURCE_ID normal_text_color_id;\n"
            "    GX_RESOURCE_ID selected_text_color_id;\n"
            "    GX_RESOURCE_ID disabled_text_color_id;\n"
            "    GX_BYTE        whitespace;\n"
            "    GX_BYTE        line_space;\n"
            "} GX_ML_TEXT_VIEW_PROPERTIES;\n\n");
    }
    else if (project_lib_version() >= 50303)
    {
        out = (""
            "typedef struct\n"
            "{\n"
            "    GX_RESOURCE_ID string_id;\n"
            "    GX_RESOURCE_ID font_id;\n"
            "    GX_RESOURCE_ID normal_text_color_id;\n"
            "    GX_RESOURCE_ID selected_text_color_id;\n"
            "    GX_BYTE        whitespace;\n"
            "    GX_BYTE        line_space;\n"
            "} GX_ML_TEXT_VIEW_PROPERTIES;\n\n");
    }
    else
    {
        out = (""
            "typedef struct\n"
            "{\n"
            "    GX_RESOURCE_ID string_id;\n"
            "    GX_RESOURCE_ID font_id;\n"
            "    GX_RESOURCE_ID normal_text_color_id;\n"
            "    GX_RESOURCE_ID selected_text_color_id;\n"
            "} GX_ML_TEXT_VIEW_PROPERTIES;\n\n");
    }
    return out;
}

CString ml_text_view_service_provider::WriteExtendedProperties(screen_generator *gen, CString &prefix, widget_info *info)
{
    CString out;
    CString propname = prefix + info->app_name;

    if (project_lib_version() > 50401)
    {
        out.Format(_T("GX_ML_TEXT_VIEW_PROPERTIES %s_properties =\n")
            _T("{\n")
            _T("    %s, /* string id */\n")
            _T("    %s, /* font id */\n")
            _T("    %s, /* normal text color */\n")
            _T("    %s, /* selected text color */\n")
            _T("    %s, /* disabled text color */\n")
            _T("    %d, /* whitespace */\n")
            _T("    %d  /* line_space */\n};\n"),
            propname,
            gen->GetStringIdName(info->string_id[0], info),
            gen->GetFontIdName(info->font_id[0]),
            gen->GetColorIdName(info->color_id[NORMAL_TEXT_COLOR_INDEX]),
            gen->GetColorIdName(info->color_id[SELECTED_TEXT_COLOR_INDEX]),
            gen->GetColorIdName(info->color_id[DISABLED_TEXT_COLOR_INDEX]),
            info->ewi.text_info.whitespace,
            info->ewi.text_info.line_space);
    }
    else if (project_lib_version() >= 50303)
    {
        out.Format(_T("GX_ML_TEXT_VIEW_PROPERTIES %s_properties =\n")
            _T("{\n")
            _T("    %s, /* string id */\n")
            _T("    %s, /* font id */\n")
            _T("    %s, /* normal text color */\n")
            _T("    %s, /* selected text color */\n")
            _T("    %d, /* whitespace */\n")
            _T("    %d  /* line_space */\n};\n"),
            propname,
            gen->GetStringIdName(info->string_id[0]),
            gen->GetFontIdName(info->font_id[0]),
            gen->GetColorIdName(info->color_id[NORMAL_TEXT_COLOR_INDEX]),
            gen->GetColorIdName(info->color_id[SELECTED_TEXT_COLOR_INDEX]),
            info->ewi.text_info.whitespace,
            info->ewi.text_info.line_space);
    }
    else
    {
        out.Format(_T("GX_ML_TEXT_VIEW_PROPERTIES %s_properties =\n")
            _T("{\n")
            _T("    %s, /* string id */\n")
            _T("    %s, /* font id */\n")
            _T("    %s, /* normal text color */\n")
            _T("    %s  /* selected text color */\n};\n"),
            propname,
            gen->GetStringIdName(info->string_id[0]),
            gen->GetFontIdName(info->font_id[0]),
            gen->GetColorIdName(info->color_id[NORMAL_TEXT_COLOR_INDEX]),
            gen->GetColorIdName(info->color_id[SELECTED_TEXT_COLOR_INDEX]));
    }
    return out;
}

CString ml_text_view_service_provider::GetCreateFromDefFunctionName()
{
    return CString("gx_studio_multi_line_text_view_create");
}

CString ml_text_view_service_provider::GetCreateFromDefFunction(int version)
{
    CString out;
    MakeCreatePreamble("multi_line_text_view", version, out);

    if (project_lib_version() > 50401)
    {
        out += "{\n"
            "    UINT status;\n"
            "    GX_MULTI_LINE_TEXT_VIEW *view = (GX_MULTI_LINE_TEXT_VIEW *) control_block;\n"
            "    GX_ML_TEXT_VIEW_PROPERTIES *props = (GX_ML_TEXT_VIEW_PROPERTIES *) info->properties;\n"
            "    status = gx_multi_line_text_view_create(view, info->widget_name, parent, props->string_id, info->style, info->widget_id, &info->size);\n"
            "    if (status == GX_SUCCESS)\n"
            "    {\n"
            "        gx_multi_line_text_view_font_set(view, props->font_id);\n"
            "#if defined(GUIX_5_4_0_COMPATIBILITY)\n"
            "        gx_multi_line_text_view_text_color_set(view, props->normal_text_color_id, props->selected_text_color_id);\n"
            "#else\n"
            "        gx_multi_line_text_view_text_color_set(view, props->normal_text_color_id, props->selected_text_color_id, props->disabled_text_color_id);\n"
            "#endif\n"
            "        gx_multi_line_text_view_whitespace_set(view, props->whitespace);\n"
            "        gx_multi_line_text_view_line_space_set(view, props->line_space);\n"
            "    }\n"
            "    return status;\n"
            "}\n";
    }
    else if (project_lib_version() >= 50303)
    {
        out += "{\n"
            "    UINT status;\n"
            "    GX_MULTI_LINE_TEXT_VIEW *view = (GX_MULTI_LINE_TEXT_VIEW *) control_block;\n"
            "    GX_ML_TEXT_VIEW_PROPERTIES *props = (GX_ML_TEXT_VIEW_PROPERTIES *) info->properties;\n"
            "    status = gx_multi_line_text_view_create(view, info->widget_name, parent, props->string_id, info->style, info->widget_id, &info->size);\n"
            "    if (status == GX_SUCCESS)\n"
            "    {\n"
            "        gx_multi_line_text_view_font_set(view, props->font_id);\n"
            "        gx_multi_line_text_view_text_color_set(view, props->normal_text_color_id, props->selected_text_color_id);\n"
            "        gx_multi_line_text_view_whitespace_set(view, props->whitespace);\n"
            "        gx_multi_line_text_view_line_space_set(view, props->line_space);\n"
            "    }\n"
            "    return status;\n"
            "}\n";
    }
    else
    {
        out += "{\n"
            "    UINT status;\n"
            "    GX_MULTI_LINE_TEXT_VIEW *view = (GX_MULTI_LINE_TEXT_VIEW *) control_block;\n"
            "    GX_ML_TEXT_VIEW_PROPERTIES *props = (GX_ML_TEXT_VIEW_PROPERTIES *) info->properties;\n"
            "    status = gx_multi_line_text_view_create(view, info->widget_name, parent, props->string_id, info->style, info->widget_id, &info->size);\n"
            "    if (status == GX_SUCCESS)\n"
            "    {\n"
            "        gx_multi_line_text_view_font_set(view, props->font_id);\n"
            "        gx_multi_line_text_view_text_color_set(view, props->normal_text_color_id, props->selected_text_color_id);\n"
            "    }\n"
            "    return status;\n"
            "}\n";
    }

    return out;
}

widget_info *ml_text_view_service_provider::CreateNewInstance(GX_WIDGET *parent)
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

    GX_MULTI_LINE_TEXT_VIEW *view = new GX_MULTI_LINE_TEXT_VIEW;
    GX_RESOURCE_ID string_id = GetStringId("text_view");

    gx_multi_line_text_view_create(view, "text_view", parent, string_id,
        GX_STYLE_ENABLED|GX_STYLE_BORDER_THIN|GX_STYLE_TEXT_CENTER, 0, &size);
    gx_widget_style_remove(view, GX_STYLE_DRAW_SELECTED);
    widget_info *info = InitWidgetInfo((GX_WIDGET *) view);

    info->font_id[0] = view->gx_multi_line_text_view_font_id;
    info->color_id[NORMAL_TEXT_COLOR_INDEX] = view->gx_multi_line_text_view_normal_text_color;
    info->color_id[SELECTED_TEXT_COLOR_INDEX] = view->gx_multi_line_text_view_selected_text_color;
    info->color_id[DISABLED_TEXT_COLOR_INDEX] = view->gx_multi_line_text_view_disabled_text_color;
    info->string_id[0] = string_id;
    info->ewi.text_info.whitespace = 0;
    info->ewi.text_info.line_space = 0;
    return info;
}
GX_WIDGET *ml_text_view_service_provider::GenerateFromInfo(GX_WIDGET *parent, widget_info *info)
{
    GX_MULTI_LINE_TEXT_VIEW *view = new GX_MULTI_LINE_TEXT_VIEW;
    gx_multi_line_text_view_create(view,
        (CHAR *) info->app_name.GetString(),
        parent,
        info->string_id[0],
        info->style, 0,
        &info->size);

    gx_widget_fill_color_set(view,
        info->color_id[NORMAL_FILL_COLOR_INDEX],
        info->color_id[SELECTED_FILL_COLOR_INDEX],
        info->color_id[DISABLED_FILL_COLOR_INDEX]);

    gx_multi_line_text_view_text_color_set(view,
        info->color_id[NORMAL_TEXT_COLOR_INDEX],
        info->color_id[SELECTED_TEXT_COLOR_INDEX],
        info->color_id[DISABLED_TEXT_COLOR_INDEX]);
    gx_multi_line_text_view_font_set(view, info->font_id[0]);

    gx_multi_line_text_view_whitespace_set(view, info->ewi.text_info.whitespace);
    gx_multi_line_text_view_line_space_set(view, info->ewi.text_info.line_space);

    return ((GX_WIDGET *) view);
}
        
void ml_text_view_service_provider::SaveToProject(xml_writer &writer, studiox_project *project, int display, widget_info *info)
{
    widget_service_provider::SaveToProject(writer, project, display, info);
    WriteStringId(writer, project, display, "string_id", info->string_id[0]);
    WriteFontId(writer, project, display, "font_id", info->font_id[0]);
    WriteColorId(writer, project, display, "normal_text_color", info->color_id[NORMAL_TEXT_COLOR_INDEX]);
    WriteColorId(writer, project, display, "selected_text_color", info->color_id[SELECTED_TEXT_COLOR_INDEX]);
    WriteColorId(writer, project, display, "disabled_fill_color", info->color_id[DISABLED_FILL_COLOR_INDEX]);
    WriteColorId(writer, project, display, "disabled_text_color", info->color_id[DISABLED_TEXT_COLOR_INDEX]);
    writer.WriteInt("whitespace", info->ewi.text_info.whitespace);
    writer.WriteInt("line_space", info->ewi.text_info.line_space);
}

void ml_text_view_service_provider::ReadFromProject(xml_reader &reader, studiox_project *project, int display, widget_info *info, ULONG valid_styles)
{
    valid_styles |= GX_STYLE_TEXT_ALIGNMENT_MASK|GX_STYLE_TEXT_COPY|GX_STYLE_VALIGN_CENTER;
    widget_service_provider::ReadFromProject(reader, project, display, info, valid_styles);
    info->string_id[0] = ReadStringId(reader, project, display, "string_id");
    if (info->string_id[0] < 0 || info->string_id[0] > 100000)
    {
        info->string_id[0] = 0;
    }
    info->font_id[0] = ReadFontId(reader, project, display, "font_id");
    info->color_id[NORMAL_TEXT_COLOR_INDEX] = ReadColorId(reader, project, display, "normal_text_color");

    if (project->mHeader.project_version > 54)
    {
        info->color_id[SELECTED_TEXT_COLOR_INDEX] = ReadColorId(reader, project, display, "selected_text_color");
        info->color_id[DISABLED_TEXT_COLOR_INDEX] = ReadColorId(reader, project, display, "disabled_text_color");
    }
    else
    {
        info->color_id[SELECTED_FILL_COLOR_INDEX] = info->color_id[NORMAL_FILL_COLOR_INDEX];
        info->color_id[SELECTED_TEXT_COLOR_INDEX] = info->color_id[NORMAL_TEXT_COLOR_INDEX];
        info->color_id[DISABLED_TEXT_COLOR_INDEX] = info->color_id[NORMAL_TEXT_COLOR_INDEX];
    }
    reader.ReadInt("whitespace", info->ewi.text_info.whitespace, 0);
    reader.ReadInt("line_space", info->ewi.text_info.line_space, 0);
}
        
void ml_text_view_service_provider::AssignText(widget_info *info, int index,
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

void ml_text_view_service_provider::AssignFont(widget_info *info, int index, GX_RESOURCE_ID font_id)
{
    info->font_id[0] = font_id;
    GX_WIDGET *widget = info->widget;

    if (widget)
    {
        GX_MULTI_LINE_TEXT_VIEW *pb = (GX_MULTI_LINE_TEXT_VIEW *)widget;
        gx_multi_line_text_view_font_set(pb, font_id);
    }
}

void ml_text_view_service_provider::AssignColor(widget_info *info, int index, GX_RESOURCE_ID color_id)
{
    GX_MULTI_LINE_TEXT_VIEW *view = (GX_MULTI_LINE_TEXT_VIEW *) (info->widget);

    switch(index)
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