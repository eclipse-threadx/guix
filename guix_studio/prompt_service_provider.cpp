
#include "studiox_includes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

prompt_service_provider::prompt_service_provider()
{
}

CString prompt_service_provider::GetVarDeclaration()
{
    return CString("GX_PROMPT_MEMBERS_DECLARE");
}

CString prompt_service_provider::DeclarePropertiesStructContent()
{
    CString out;

    if (project_lib_version() > 50401)
    {
        out = ("    GX_RESOURCE_ID string_id;\n"
            "    GX_RESOURCE_ID font_id;\n"
            "    GX_RESOURCE_ID normal_text_color_id;\n"
            "    GX_RESOURCE_ID selected_text_color_id;\n"
            "    GX_RESOURCE_ID disabled_text_color_id;");
    }
    else
    {
        out = ("    GX_RESOURCE_ID string_id;\n"
            "    GX_RESOURCE_ID font_id;\n"
            "    GX_RESOURCE_ID normal_text_color_id;\n"
            "    GX_RESOURCE_ID selected_text_color_id;");
    }

    return out;
}

CString prompt_service_provider::DeclarePropertiesStruct()
{
    CString out;

    out.Format(_T("typedef struct\n")
        _T("{\n")
        _T("%s\n")
        _T("} GX_PROMPT_PROPERTIES;\n\n"),
        DeclarePropertiesStructContent());

    return out;
}

CString prompt_service_provider::WriteExtendedPropertiesContent(screen_generator *gen, widget_info *info, GX_BOOL add_last_comma)
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

    if (project_lib_version() <= 50401)
    {
        out.Format(_T("    %s, /* string id */\n")
            _T("    %s, /* font id */\n")
            _T("    %s, /* normal text color */\n")
            _T("    %s%s /* selected text color */"),
            gen->GetStringIdName(info->string_id[0]),
            gen->GetFontIdName(info->font_id[0]),
            gen->GetColorIdName(info->color_id[NORMAL_TEXT_COLOR_INDEX]),
            gen->GetColorIdName(info->color_id[SELECTED_TEXT_COLOR_INDEX]), last_delimiter);
    }
    else
    {
        out.Format(_T("    %s, /* string id */\n")
            _T("    %s, /* font id */\n")
            _T("    %s, /* normal text color */\n")
            _T("    %s,  /* selected text color */\n")
            _T("    %s%s /* disabled text color */"),
            gen->GetStringIdName(info->string_id[0]),
            gen->GetFontIdName(info->font_id[0]),
            gen->GetColorIdName(info->color_id[NORMAL_TEXT_COLOR_INDEX]),
            gen->GetColorIdName(info->color_id[SELECTED_TEXT_COLOR_INDEX]),
            gen->GetColorIdName(info->color_id[DISABLED_TEXT_COLOR_INDEX]), last_delimiter);
    }

    return out;
}

CString prompt_service_provider::WriteExtendedProperties(screen_generator *gen, CString &prefix, widget_info *info)
{
    CString out;
    CString propname = prefix + info->app_name;

    out.Format(_T("GX_PROMPT_PROPERTIES %s_properties =\n")
        _T("{\n")
        _T("%s\n")
        _T("};\n"),
        propname,
        WriteExtendedPropertiesContent(gen, info));
    return out;
}

CString prompt_service_provider::GetCreateFromDefFunctionName()
{
    return CString("gx_studio_prompt_create");
}

CString prompt_service_provider::GetCreateFromDefFunction(int version)
{
    CString out;
    MakeCreatePreamble("prompt", version, out);

    out += "{\n"
        "    UINT status;\n"
        "    GX_PROMPT *prompt = (GX_PROMPT *) control_block;\n"
        "    GX_PROMPT_PROPERTIES *props = (GX_PROMPT_PROPERTIES *) info->properties;\n"
        "    status = gx_prompt_create(prompt, info->widget_name, parent, props->string_id, info->style, info->widget_id, &info->size);\n"
        "    if (status == GX_SUCCESS)\n"
        "    {\n"
        "        gx_prompt_font_set(prompt, props->font_id);\n";

        if (project_lib_version() > 50401)
        {
            out += "#if defined(GUIX_5_4_0_COMPATIBILITY)\n"
            "        gx_prompt_text_color_set(prompt, props->normal_text_color_id, props->selected_text_color_id);\n"
            "#else\n"
            "        gx_prompt_text_color_set(prompt, props->normal_text_color_id, props->selected_text_color_id, props->disabled_text_color_id);\n"
            "#endif\n";
        }
        else
        {
            out += "        gx_prompt_text_color_set(prompt, props->normal_text_color_id, props->selected_text_color_id);\n";
        }
       
    out += "    }\n"
        "    return status;\n"
        "}\n";

    return out;
}

widget_info *prompt_service_provider::CreateNewInstance(GX_WIDGET *parent)
{
    GX_RECTANGLE size;
    gx_utility_rectangle_define(&size, 0, 0, 79, 23);
    gx_utility_rectangle_center(&size, &parent->gx_widget_size);
    
    GX_PROMPT *prompt = new GX_PROMPT;
    GX_RESOURCE_ID string_id = GetStringId("prompt");

    gx_prompt_create(prompt, "prompt", parent, string_id,
        GX_STYLE_ENABLED|GX_STYLE_BORDER_THIN|GX_STYLE_TEXT_CENTER, 0, &size);
    widget_info *info = InitWidgetInfo((GX_WIDGET *) prompt);

    info->font_id[0] = prompt->gx_prompt_font_id;
    info->color_id[NORMAL_TEXT_COLOR_INDEX] = prompt->gx_prompt_normal_text_color;
    info->color_id[SELECTED_TEXT_COLOR_INDEX] = prompt->gx_prompt_selected_text_color;
    info->color_id[DISABLED_TEXT_COLOR_INDEX] = prompt->gx_prompt_disabled_text_color;
    info->string_id[0] = string_id;
    return info;
}

GX_WIDGET *prompt_service_provider::GenerateFromInfo(GX_WIDGET *parent, widget_info *info)
{
    GX_PROMPT *prompt = new GX_PROMPT;
    gx_prompt_create(prompt,
        (CHAR *) info->app_name.GetString(),
        parent,
        info->string_id[0],      // this should be text id
        info->style, 0,
        &info->size);

    gx_widget_fill_color_set(prompt,
        info->color_id[NORMAL_FILL_COLOR_INDEX],
        info->color_id[SELECTED_FILL_COLOR_INDEX],
        info->color_id[DISABLED_FILL_COLOR_INDEX]);
    gx_prompt_text_color_set(prompt,
        info->color_id[NORMAL_TEXT_COLOR_INDEX],
        info->color_id[SELECTED_TEXT_COLOR_INDEX],
        info->color_id[DISABLED_TEXT_COLOR_INDEX]);
    gx_prompt_font_set(prompt, info->font_id[0]);

    return ((GX_WIDGET *) prompt);
}
        
void prompt_service_provider::SaveToProject(xml_writer &writer, studiox_project *project, int display, widget_info *info)
{
    widget_service_provider::SaveToProject(writer, project, display, info);
    WriteStringId(writer, project, display, "string_id", info->string_id[0]);
    WriteFontId(writer, project, display, "font_id", info->font_id[0]);
    WriteColorId(writer, project, display, "normal_text_color", info->color_id[NORMAL_TEXT_COLOR_INDEX]);
    WriteColorId(writer, project, display, "selected_text_color", info->color_id[SELECTED_TEXT_COLOR_INDEX]);
    WriteColorId(writer, project, display, "disabled_text_color", info->color_id[DISABLED_TEXT_COLOR_INDEX]);
}

void prompt_service_provider::ReadFromProject(xml_reader &reader, studiox_project *project, int display, widget_info *info, ULONG valid_styles)
{
    valid_styles |= GX_STYLE_TEXT_ALIGNMENT_MASK|GX_STYLE_TEXT_COPY;
    widget_service_provider::ReadFromProject(reader, project, display, info, valid_styles);
    info->string_id[0] = ReadStringId(reader, project, display, "string_id");
    if (info->string_id[0] < 0 || info->string_id[0] > 100000)
    {
        info->string_id[0] = 0;
    }
    info->font_id[0] = ReadFontId(reader, project, display, "font_id");
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
}
        
void prompt_service_provider::AssignText(widget_info *info, int index,
    GX_RESOURCE_ID string_id)
{
    GX_WIDGET *widget = info->widget;

    if (widget)
    {
        GX_PROMPT *prompt = (GX_PROMPT *)widget;
        gx_prompt_text_id_set(prompt, string_id);
    }

    info->string_id[0] = string_id;
}

void prompt_service_provider::AssignFont(widget_info *info, int index, 
    GX_RESOURCE_ID font_id)
{
    GX_WIDGET *widget = info->widget;

    if (widget)
    {
        GX_PROMPT *prompt = (GX_PROMPT *)widget;
        gx_prompt_font_set(prompt, font_id);
    }

    info->font_id[0] = font_id;
}

void prompt_service_provider::AssignColor(widget_info *info, int index, GX_RESOURCE_ID color_id)
{
    GX_PROMPT *prompt = (GX_PROMPT *) (info->widget);

    switch(index)
    {
    case NORMAL_TEXT_COLOR_INDEX:
        if (prompt)
        {
            gx_prompt_text_color_set(prompt,
                color_id,
                prompt->gx_prompt_selected_text_color,
                prompt->gx_prompt_disabled_text_color);
        }

        info->color_id[NORMAL_TEXT_COLOR_INDEX] = color_id;
        break;
        
    case SELECTED_TEXT_COLOR_INDEX:
        if (prompt)
        {
            gx_prompt_text_color_set(prompt,
                prompt->gx_prompt_normal_text_color,
                color_id,
                prompt->gx_prompt_disabled_text_color);
        }

        info->color_id[SELECTED_TEXT_COLOR_INDEX] = color_id;
        break;

    case DISABLED_TEXT_COLOR_INDEX:
        if (prompt)
        {
            gx_prompt_text_color_set(prompt,
                prompt->gx_prompt_normal_text_color,
                prompt->gx_prompt_selected_text_color,
                color_id);
        }

        info->color_id[DISABLED_TEXT_COLOR_INDEX] = color_id;
        break;

    default:
        widget_service_provider::AssignColor(info, index, color_id);
    }
}

void prompt_service_provider::Autosize(widget_info *info)
{
    CheckResizeToText(info);
}
