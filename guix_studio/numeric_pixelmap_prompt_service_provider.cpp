
#include "studiox_includes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

numeric_pixelmap_prompt_service_provider::numeric_pixelmap_prompt_service_provider()
{
}

CString numeric_pixelmap_prompt_service_provider::GetVarDeclaration()
{
    return CString("GX_NUMERIC_PIXELMAP_PROMPT_MEMBERS_DECLARE");
}

CString numeric_pixelmap_prompt_service_provider::DeclarePropertiesStruct()
{
    CString out;
    
    out.Format(_T("typedef struct\n")
        _T("{\n")
        _T("%s\n")
        _T("    VOID (*format_func)(GX_NUMERIC_PIXELMAP_PROMPT *, INT);\n")
        _T("    INT            numeric_prompt_value;\n")
        _T("} GX_NUMERIC_PIXELMAP_PROMPT_PROPERTIES;\n\n"),
        pixelmap_prompt_service_provider::DeclarePropertiesStructContent());
    return out;
}

CString numeric_pixelmap_prompt_service_provider::WriteExtendedProperties(screen_generator *gen, CString &prefix, widget_info *info)
{
    CString out;
    CString propname = prefix + info->app_name;
    CString format_func;

    if (info->format_func.IsEmpty())
    {
        format_func = "GX_NULL";
    }
    else
    {
        format_func = info->format_func;
    }

    out.Format(_T("GX_NUMERIC_PIXELMAP_PROMPT_PROPERTIES %s_properties =\n")
        _T("{\n")
        _T("%s\n")
        _T("    %s, /* format function */\n")
        _T("    %d  /* numeric prompt value */\n};\n"),
        propname,
        pixelmap_prompt_service_provider::WriteExtendedPropertiesContent(gen, info, TRUE),
        format_func,
        info->ewi.numeric_prompt_value);
    return out;
}

CString numeric_pixelmap_prompt_service_provider::GetCreateFromDefFunctionName()
{
    return CString("gx_studio_numeric_pixelmap_prompt_create");
}

CString numeric_pixelmap_prompt_service_provider::GetCreateFromDefFunction(int version)
{
    CString out;
    MakeCreatePreamble("numeric_pixelmap_prompt", version, out);

    out += "{\n"
        "    UINT status;\n"
        "    GX_NUMERIC_PIXELMAP_PROMPT *pix_prompt = (GX_NUMERIC_PIXELMAP_PROMPT *) control_block;\n"
        "    GX_PROMPT *prompt = (GX_PROMPT *) pix_prompt;\n"
        "    GX_NUMERIC_PIXELMAP_PROMPT_PROPERTIES *props = (GX_NUMERIC_PIXELMAP_PROMPT_PROPERTIES *) info->properties;\n"
        "    status = gx_numeric_pixelmap_prompt_create(pix_prompt, info->widget_name, parent,\n"
        "               props->string_id,\n"
        "               props->fill_map_id,\n"
        "               info->style, info->widget_id, &info->size);\n"
        "\n"
        "    if (status == GX_SUCCESS)\n"
        "    {\n"
        "        if(!props->string_id)\n"
        "        {\n"
        "            gx_numeric_pixelmap_prompt_value_set(pix_prompt, props->numeric_prompt_value);\n"
        "        }\n"
        "        if(props->format_func)\n"
        "        {\n"
        "            gx_numeric_pixelmap_prompt_format_function_set(pix_prompt, props->format_func);\n"
        "        }\n"
        "        gx_pixelmap_prompt_pixelmap_set((GX_PIXELMAP_PROMPT *)pix_prompt,\n"
        "                                        props->left_map_id,\n"
        "                                        props->fill_map_id,\n"
        "                                        props->right_map_id,\n"
        "                                        props->selected_left_map_id,\n"
        "                                        props->selected_fill_map_id,\n"
        "                                        props->selected_right_map_id);\n"
        "        gx_prompt_font_set(prompt, props->font_id);\n";

    if (version > 50401)
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
    out +=     "    }\n"
        "    return status;\n"
        "}\n";

    return out;
}

widget_info *numeric_pixelmap_prompt_service_provider::CreateNewInstance(GX_WIDGET *parent)
{
    GX_RECTANGLE size;
    gx_utility_rectangle_define(&size, 0, 0, 79, 23);
    gx_utility_rectangle_center(&size, &parent->gx_widget_size);

    GX_NUMERIC_PIXELMAP_PROMPT *prompt = new GX_NUMERIC_PIXELMAP_PROMPT;

    gx_numeric_pixelmap_prompt_create(prompt, "numeric_pixelmap_prompt", parent, 0, 0,
        GX_STYLE_ENABLED | GX_STYLE_BORDER_THIN | GX_STYLE_TEXT_CENTER, 0, &size);

    widget_info *info = InitWidgetInfo((GX_WIDGET *)prompt);
    info->pixelmap_id[FILL_PIXELMAP_INDEX] = 0;
    info->pixelmap_id[LEFT_PIXELMAP_INDEX] = 0;
    info->pixelmap_id[RIGHT_PIXELMAP_INDEX] = 0;
    info->pixelmap_id[SELECTED_FILL_PIXELMAP_INDEX] = 0;
    info->pixelmap_id[SELECTED_LEFT_PIXELMAP_INDEX] = 0;
    info->pixelmap_id[SELECTED_RIGHT_PIXELMAP_INDEX] = 0;
    info->string_id[0] = 0;
    info->font_id[0] = prompt->gx_prompt_font_id;
    info->color_id[NORMAL_TEXT_COLOR_INDEX] = prompt->gx_prompt_normal_text_color;
    info->color_id[SELECTED_TEXT_COLOR_INDEX] = prompt->gx_prompt_selected_text_color;
    info->color_id[DISABLED_TEXT_COLOR_INDEX] = prompt->gx_prompt_disabled_text_color;
    info->ewi.numeric_prompt_value = 0;

    gx_numeric_pixelmap_prompt_value_set(prompt, info->ewi.numeric_prompt_value);
    return info;
}

GX_WIDGET *numeric_pixelmap_prompt_service_provider::GenerateFromInfo(GX_WIDGET *parent, widget_info *info)
{
    GX_NUMERIC_PIXELMAP_PROMPT *prompt = new GX_NUMERIC_PIXELMAP_PROMPT;

    gx_numeric_pixelmap_prompt_create(prompt,
        (CHAR *)info->app_name.GetString(),
        parent,
        info->string_id[0],
        info->pixelmap_id[FILL_PIXELMAP_INDEX],
        info->style, 0, &info->size);

    if (!info->string_id[0])
    {
        gx_numeric_pixelmap_prompt_value_set(prompt, info->ewi.numeric_prompt_value);
    }

    gx_pixelmap_prompt_pixelmap_set((GX_PIXELMAP_PROMPT *)prompt,
        info->pixelmap_id[LEFT_PIXELMAP_INDEX],
        info->pixelmap_id[FILL_PIXELMAP_INDEX],
        info->pixelmap_id[RIGHT_PIXELMAP_INDEX],
        info->pixelmap_id[SELECTED_LEFT_PIXELMAP_INDEX],
        info->pixelmap_id[SELECTED_FILL_PIXELMAP_INDEX],
        info->pixelmap_id[SELECTED_RIGHT_PIXELMAP_INDEX]);

    gx_widget_fill_color_set((GX_PROMPT *)prompt,
        info->color_id[NORMAL_FILL_COLOR_INDEX],
        info->color_id[SELECTED_FILL_COLOR_INDEX],
        info->color_id[DISABLED_FILL_COLOR_INDEX]);

    gx_prompt_text_color_set((GX_PROMPT *)prompt,
        info->color_id[NORMAL_TEXT_COLOR_INDEX],
        info->color_id[SELECTED_TEXT_COLOR_INDEX],
        info->color_id[DISABLED_TEXT_COLOR_INDEX]);
    gx_prompt_font_set((GX_PROMPT *)prompt, info->font_id[0]);

    return (GX_WIDGET *)prompt;
}

void numeric_pixelmap_prompt_service_provider::SaveToProject(xml_writer &writer, studiox_project *project, int display, widget_info *info)
{
    pixelmap_prompt_service_provider::SaveToProject(writer, project, display, info);

    /* Write format function. */
    writer.WriteString("format_func", info->format_func);

    /* Write numeric prompt value. */
    writer.WriteInt("numeric_prompt_value", info->ewi.numeric_prompt_value);
}

void numeric_pixelmap_prompt_service_provider::ReadFromProject(xml_reader &reader, studiox_project *project, int display, widget_info *info, ULONG valid_styles)
{
    pixelmap_prompt_service_provider::ReadFromProject(reader, project, display, info, valid_styles);

    /* Write format function. */
    reader.ReadString("format_func", info->format_func);

    /* Read prompt value. */
    reader.ReadInt("numeric_prompt_value", info->ewi.numeric_prompt_value, 0);
}

void numeric_pixelmap_prompt_service_provider::AssignNumericValue(widget_info *info, INT value)
{
    info->ewi.numeric_prompt_value = value;
    info->string_id[0] = 0;

    /* Set new numeric value. */
    gx_numeric_pixelmap_prompt_value_set((GX_NUMERIC_PIXELMAP_PROMPT *)info->widget, value);
}