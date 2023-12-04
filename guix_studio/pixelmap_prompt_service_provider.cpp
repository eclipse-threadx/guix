
#include "studiox_includes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

pixelmap_prompt_service_provider::pixelmap_prompt_service_provider()
{
}

CString pixelmap_prompt_service_provider::GetVarDeclaration()
{
    return CString("GX_PIXELMAP_PROMPT_MEMBERS_DECLARE");
}

CString pixelmap_prompt_service_provider::DeclarePropertiesStructContent()
{
    CString out;

    out.Format(_T("%s\n")
        _T("    GX_RESOURCE_ID left_map_id;\n")
        _T("    GX_RESOURCE_ID fill_map_id;\n")
        _T("    GX_RESOURCE_ID right_map_id;\n")
        _T("    GX_RESOURCE_ID selected_left_map_id;\n")
        _T("    GX_RESOURCE_ID selected_fill_map_id;\n")
        _T("    GX_RESOURCE_ID selected_right_map_id;"),
        prompt_service_provider::DeclarePropertiesStructContent());

    return out;
}

CString pixelmap_prompt_service_provider::DeclarePropertiesStruct()
{
    CString out;
    out.Format(_T("typedef struct\n")
            _T("{\n")
            _T("%s\n")
            _T("} GX_PIXELMAP_PROMPT_PROPERTIES;\n\n"),
            DeclarePropertiesStructContent());

    return out;
}

CString pixelmap_prompt_service_provider::WriteExtendedPropertiesContent(screen_generator *gen, widget_info *info, BOOL add_last_comma)
{
    CString out;
    CString last_delimiter;

    if (add_last_comma)
    {
        last_delimiter = _T(",");
    }
    else
    {
        last_delimiter = _T(" ");
    }

    out.Format(_T("%s\n")
        _T("    %s, /* left pixelmap id */\n")
        _T("    %s, /* fill pixelmap id */\n")
        _T("    %s, /* right pixelmap id*/\n")
        _T("    %s, /* selected left pixelmap id */\n")
        _T("    %s, /* selected fill pixelmap id */\n")
        _T("    %s%s /* selected right pixelmap id*/"),
        prompt_service_provider::WriteExtendedPropertiesContent(gen, info, TRUE),
        gen->GetPixelmapIdName(info->pixelmap_id[LEFT_PIXELMAP_INDEX]),
        gen->GetPixelmapIdName(info->pixelmap_id[FILL_PIXELMAP_INDEX]),
        gen->GetPixelmapIdName(info->pixelmap_id[RIGHT_PIXELMAP_INDEX]),
        gen->GetPixelmapIdName(info->pixelmap_id[SELECTED_LEFT_PIXELMAP_INDEX]),
        gen->GetPixelmapIdName(info->pixelmap_id[SELECTED_FILL_PIXELMAP_INDEX]),
        gen->GetPixelmapIdName(info->pixelmap_id[SELECTED_RIGHT_PIXELMAP_INDEX]), last_delimiter);
    return out;
}

CString pixelmap_prompt_service_provider::WriteExtendedProperties(screen_generator *gen, CString &prefix, widget_info *info)
{
    CString out;
    CString propname = prefix + info->app_name;
    out.Format(_T("GX_PIXELMAP_PROMPT_PROPERTIES %s_properties =\n")
        _T("{\n")
        _T("%s\n")
        _T("};\n"),
        propname,
        WriteExtendedPropertiesContent(gen, info));
    return out;
}

CString pixelmap_prompt_service_provider::GetCreateFromDefFunctionName()
{
    return CString("gx_studio_pixelmap_prompt_create");
}

CString pixelmap_prompt_service_provider::GetCreateFromDefFunction(int version)
{
    CString out;
    MakeCreatePreamble("pixelmap_prompt", version, out);

    if (version < 51)
    {
        out += "{\n"
        "    UINT status;\n"
        "    GX_PIXELMAP_PROMPT *pix_prompt = (GX_PIXELMAP_PROMPT *) control_block;\n"
        "    GX_PROMPT *prompt = (GX_PROMPT *) pix_prompt;\n"
        "    GX_PIXELMAP_PROMPT_PROPERTIES *props = (GX_PIXELMAP_PROMPT_PROPERTIES *) info->properties;\n"   
        "    status = gx_pixelmap_prompt_create(pix_prompt, info->widget_name, parent,\n"
        "               props->string_id,\n"
        "               props->left_map_id,\n"
        "               props->fill_map_id,\n"
        "               props->right_map_id,\n"
        "               info->style, info->widget_id, &info->size);\n"
        "\n"
        "    if (status == GX_SUCCESS)\n"
        "    {\n"
        "        gx_prompt_font_set(prompt, props->font_id);\n"
        "        gx_prompt_text_color_set(prompt, props->normal_text_color_id, props->selected_text_color_id);\n"
        "    }\n"
        "    return status;\n"
        "}\n";
    }
    else
    {
        out += "{\n"
        "    UINT status;\n"
        "    GX_PIXELMAP_PROMPT *pix_prompt = (GX_PIXELMAP_PROMPT *) control_block;\n"
        "    GX_PROMPT *prompt = (GX_PROMPT *) pix_prompt;\n"
        "    GX_PIXELMAP_PROMPT_PROPERTIES *props = (GX_PIXELMAP_PROMPT_PROPERTIES *) info->properties;\n"   
        "    status = gx_pixelmap_prompt_create(pix_prompt, info->widget_name, parent,\n"
        "               props->string_id,\n"
        "               props->fill_map_id,\n"
        "               info->style, info->widget_id, &info->size);\n"
        "\n"
        "    if (status == GX_SUCCESS)\n"
        "    {\n"
        "        gx_pixelmap_prompt_pixelmap_set(pix_prompt,\n"
        "                                        props->left_map_id,\n"
        "                                        props->fill_map_id,\n"
        "                                        props->right_map_id,\n"
        "                                        props->selected_left_map_id,\n"
        "                                        props->selected_fill_map_id,\n"
        "                                        props->selected_right_map_id);\n"
        "        gx_prompt_font_set(prompt, props->font_id);\n";
        if (version <= 50401)
        {
            out += "        gx_prompt_text_color_set(prompt, props->normal_text_color_id, props->selected_text_color_id);\n";
        }
        else
        {
            out += "#if defined(GUIX_5_4_0_COMPATIBILITY)\n"
            "        gx_prompt_text_color_set(prompt, props->normal_text_color_id, props->selected_text_color_id);\n"
            "#else\n"
            "        gx_prompt_text_color_set(prompt, props->normal_text_color_id, props->selected_text_color_id, props->disabled_text_color_id);\n"
            "#endif\n";
        }
        out += "    }\n"
        "    return status;\n"
        "}\n";
    }
    return out;
}

widget_info *pixelmap_prompt_service_provider::CreateNewInstance(GX_WIDGET *parent)
{
    GX_RECTANGLE size;
    gx_utility_rectangle_define(&size, 0, 0, 79, 23);
    gx_utility_rectangle_center(&size, &parent->gx_widget_size);
    
    GX_PIXELMAP_PROMPT *prompt = new GX_PIXELMAP_PROMPT;
    GX_RESOURCE_ID string_id = GetStringId("prompt");

    gx_pixelmap_prompt_create(prompt, "prompt", parent,
        string_id, 0, 
        GX_STYLE_ENABLED|GX_STYLE_BORDER_THIN|GX_STYLE_TEXT_CENTER, 0, &size);

    widget_info *info = InitWidgetInfo((GX_WIDGET *) prompt);
    info->pixelmap_id[FILL_PIXELMAP_INDEX] = 0;
    info->pixelmap_id[LEFT_PIXELMAP_INDEX] = 0;
    info->pixelmap_id[RIGHT_PIXELMAP_INDEX] = 0;
    info->pixelmap_id[SELECTED_FILL_PIXELMAP_INDEX] = 0;
    info->pixelmap_id[SELECTED_LEFT_PIXELMAP_INDEX] = 0;
    info->pixelmap_id[SELECTED_RIGHT_PIXELMAP_INDEX] = 0;
    info->string_id[0] = string_id;
    info->font_id[0] = prompt->gx_prompt_font_id;
    info->color_id[NORMAL_TEXT_COLOR_INDEX] = prompt->gx_prompt_normal_text_color;
    info->color_id[SELECTED_TEXT_COLOR_INDEX] = prompt->gx_prompt_selected_text_color;
    info->color_id[DISABLED_TEXT_COLOR_INDEX] = prompt->gx_prompt_disabled_text_color;
    return info;
}

GX_WIDGET *pixelmap_prompt_service_provider::GenerateFromInfo(GX_WIDGET *parent, widget_info *info)
{
    GX_PIXELMAP_PROMPT *prompt = new GX_PIXELMAP_PROMPT;

    gx_pixelmap_prompt_create(prompt,
        (CHAR *) info->app_name.GetString(),
        parent,
        info->string_id[0],
        info->pixelmap_id[FILL_PIXELMAP_INDEX],
        info->style, 0, &info->size);

    gx_pixelmap_prompt_pixelmap_set(prompt,
        info->pixelmap_id[LEFT_PIXELMAP_INDEX],
        info->pixelmap_id[FILL_PIXELMAP_INDEX],
        info->pixelmap_id[RIGHT_PIXELMAP_INDEX],
        info->pixelmap_id[SELECTED_LEFT_PIXELMAP_INDEX],
        info->pixelmap_id[SELECTED_FILL_PIXELMAP_INDEX],
        info->pixelmap_id[SELECTED_RIGHT_PIXELMAP_INDEX]);

    gx_widget_fill_color_set((GX_PROMPT *) prompt,
        info->color_id[NORMAL_FILL_COLOR_INDEX],
        info->color_id[SELECTED_FILL_COLOR_INDEX],
        info->color_id[DISABLED_FILL_COLOR_INDEX]);

    gx_prompt_text_color_set((GX_PROMPT *) prompt,
        info->color_id[NORMAL_TEXT_COLOR_INDEX],
        info->color_id[SELECTED_TEXT_COLOR_INDEX],
        info->color_id[DISABLED_TEXT_COLOR_INDEX]);
    gx_prompt_font_set((GX_PROMPT *) prompt, info->font_id[0]);

    return ((GX_WIDGET *) prompt);
}
        
void pixelmap_prompt_service_provider::SaveToProject(xml_writer &writer, studiox_project *project, int display, widget_info *info)
{
    prompt_service_provider::SaveToProject(writer, project, display, info);
    WritePixelmapId(writer, project, display, "left_map_id", info->pixelmap_id[LEFT_PIXELMAP_INDEX]);
    WritePixelmapId(writer, project, display, "fill_map_id", info->pixelmap_id[FILL_PIXELMAP_INDEX]);
    WritePixelmapId(writer, project, display, "right_map_id", info->pixelmap_id[RIGHT_PIXELMAP_INDEX]);
    WritePixelmapId(writer, project, display, "selected_left_map_id", info->pixelmap_id[SELECTED_LEFT_PIXELMAP_INDEX]);
    WritePixelmapId(writer, project, display, "selected_fill_map_id", info->pixelmap_id[SELECTED_FILL_PIXELMAP_INDEX]);
    WritePixelmapId(writer, project, display, "selected_right_map_id", info->pixelmap_id[SELECTED_RIGHT_PIXELMAP_INDEX]);
}

void pixelmap_prompt_service_provider::ReadFromProject(xml_reader &reader, studiox_project *project, int display, widget_info *info, ULONG valid_styles)
{
    valid_styles |= GX_PIXELMAP_HALIGN_MASK|GX_PIXELMAP_VALIGN_MASK;
    prompt_service_provider::ReadFromProject(reader, project, display, info, valid_styles);
    info->pixelmap_id[LEFT_PIXELMAP_INDEX] = ReadPixelmapId(reader, project, display, "left_map_id");
    info->pixelmap_id[FILL_PIXELMAP_INDEX] = ReadPixelmapId(reader, project, display, "fill_map_id");
    info->pixelmap_id[RIGHT_PIXELMAP_INDEX] = ReadPixelmapId(reader, project, display, "right_map_id");
    info->pixelmap_id[SELECTED_LEFT_PIXELMAP_INDEX] = ReadPixelmapId(reader, project, display, "selected_left_map_id");
    info->pixelmap_id[SELECTED_FILL_PIXELMAP_INDEX] = ReadPixelmapId(reader, project, display, "selected_fill_map_id");
    info->pixelmap_id[SELECTED_RIGHT_PIXELMAP_INDEX] = ReadPixelmapId(reader, project, display, "selected_right_map_id");
}
        
void pixelmap_prompt_service_provider::AssignPixelmap(widget_info *info,
    int index, GX_RESOURCE_ID pixmap_id)
{
    GX_WIDGET *widget = info->widget;

    switch(index)
    {
    case LEFT_PIXELMAP_INDEX:
        info->pixelmap_id[LEFT_PIXELMAP_INDEX] = pixmap_id;
        break;

    case FILL_PIXELMAP_INDEX:
        info->pixelmap_id[FILL_PIXELMAP_INDEX] = pixmap_id;
        break;

    case RIGHT_PIXELMAP_INDEX:
        info->pixelmap_id[RIGHT_PIXELMAP_INDEX] = pixmap_id;
        break;

    case SELECTED_LEFT_PIXELMAP_INDEX:
        info->pixelmap_id[SELECTED_LEFT_PIXELMAP_INDEX] = pixmap_id;
        break;

    case SELECTED_FILL_PIXELMAP_INDEX:
        info->pixelmap_id[SELECTED_FILL_PIXELMAP_INDEX] = pixmap_id;
        break;

    case SELECTED_RIGHT_PIXELMAP_INDEX:
        info->pixelmap_id[SELECTED_RIGHT_PIXELMAP_INDEX] = pixmap_id;
        break;
    }

    if (widget)
    {
        GX_PIXELMAP_PROMPT *prompt = (GX_PIXELMAP_PROMPT *)widget;

        gx_pixelmap_prompt_pixelmap_set(prompt,
            info->pixelmap_id[LEFT_PIXELMAP_INDEX],
            info->pixelmap_id[FILL_PIXELMAP_INDEX],
            info->pixelmap_id[RIGHT_PIXELMAP_INDEX],
            info->pixelmap_id[SELECTED_LEFT_PIXELMAP_INDEX],
            info->pixelmap_id[SELECTED_FILL_PIXELMAP_INDEX],
            info->pixelmap_id[SELECTED_RIGHT_PIXELMAP_INDEX]);
    }
}

void pixelmap_prompt_service_provider::Autosize(widget_info *info)
{
    CheckResizeToPixelmap(info, RESIZE_MODE_HEIGHT);
}
