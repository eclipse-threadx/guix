
#include "studiox_includes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

mlt_button_service_provider::mlt_button_service_provider()
{
}

CString mlt_button_service_provider::GetVarDeclaration()
{
    return CString("GX_MULTI_LINE_TEXT_BUTTON_MEMBERS_DECLARE");
}

CString mlt_button_service_provider::DeclarePropertiesStruct()
{
    CString out;
    out.Format(_T("typedef struct\n")
    _T("{\n")
    _T("%s\n")
    _T("} GX_ML_TEXT_BUTTON_PROPERTIES;\n\n"),
    text_button_service_provider::DeclaraPropertiesStructContent());
    return out;
}

CString mlt_button_service_provider::WriteExtendedProperties(screen_generator *gen, CString &prefix, widget_info *info)
{
    CString out;
    CString propname = prefix + info->app_name;
    out.Format(_T("GX_ML_TEXT_BUTTON_PROPERTIES %s_properties =\n")
        _T("{\n")
        _T("%s\n")
        _T("};\n"),
        propname,
        text_button_service_provider::WriteExtendedPropertiesContent(gen, info));

    return out;
}


CString mlt_button_service_provider::GetCreateFromDefFunctionName()
{
    return CString("gx_studio_multi_line_text_button_create");
}

CString mlt_button_service_provider::GetCreateFromDefFunction(int version)
{
    CString out;
    MakeCreatePreamble("multi_line_text_button", version, out);

    out += "{\n"
        "    UINT status;\n"
        "    GX_MULTI_LINE_TEXT_BUTTON *button = (GX_MULTI_LINE_TEXT_BUTTON *) control_block;\n"
        "    GX_ML_TEXT_BUTTON_PROPERTIES *props = (GX_ML_TEXT_BUTTON_PROPERTIES *) info->properties;\n"
        "    status = gx_multi_line_text_button_create(button, info->widget_name, parent, props->string_id, info->style, info->widget_id, &info->size);\n"
        "    if (status == GX_SUCCESS)\n"
        "    {\n"
        "        gx_text_button_font_set((GX_TEXT_BUTTON *) button, props->font_id);\n";

    if (project_lib_version() > 50401)
    {
        out += "#if defined(GUIX_5_4_0_COMPATIBILITY)\n"
        "        gx_text_button_text_color_set((GX_TEXT_BUTTON *) button, props->normal_text_color_id, props->selected_text_color_id);\n"
        "#else\n"
        "        gx_text_button_text_color_set((GX_TEXT_BUTTON *) button, props->normal_text_color_id, props->selected_text_color_id, props->disabled_text_color_id);\n"
        "#endif\n";
    }
    else
    {
        out += "        gx_text_button_text_color_set((GX_TEXT_BUTTON *) button, props->normal_text_color_id, props->selected_text_color_id);\n";
    }

    out += "    }\n"
    "    return status;\n"
    "}\n";
    return out;
}

widget_info *mlt_button_service_provider::CreateNewInstance(GX_WIDGET *parent)
{
    GX_RECTANGLE size;
    gx_utility_rectangle_define(&size, 0, 0, 79, 63);
    gx_utility_rectangle_center(&size, &parent->gx_widget_size);
    
    GX_MULTI_LINE_TEXT_BUTTON *button = new GX_MULTI_LINE_TEXT_BUTTON;
    GX_RESOURCE_ID string_id = GetStringId("multi\rline\rbutton");

    gx_multi_line_text_button_create(button, "multi_line_button", parent,
        string_id, GX_STYLE_ENABLED|GX_STYLE_TEXT_CENTER|GX_STYLE_BORDER_RAISED, 0, &size);

    widget_info *info = InitWidgetInfo((GX_WIDGET *) button);

    info->string_id[0]  = string_id;
    info->color_id[NORMAL_TEXT_COLOR_INDEX] = button->gx_text_button_normal_text_color;
    info->color_id[SELECTED_TEXT_COLOR_INDEX] = button->gx_text_button_selected_text_color;
    info->color_id[DISABLED_TEXT_COLOR_INDEX] = button->gx_text_button_disabled_text_color;
    info->font_id[0] = button->gx_text_button_font_id;

    return info;
}

GX_WIDGET *mlt_button_service_provider::GenerateFromInfo(GX_WIDGET *parent, widget_info *info)
{
    GX_MULTI_LINE_TEXT_BUTTON *button = new GX_MULTI_LINE_TEXT_BUTTON;
    gx_multi_line_text_button_create(button,
        (CHAR *) info->app_name.GetString(),
        parent,
        info->string_id[0],
        info->style,
        0,
        &info->size);

    gx_widget_fill_color_set((GX_TEXT_BUTTON *)button,
        info->color_id[NORMAL_FILL_COLOR_INDEX],
        info->color_id[SELECTED_FILL_COLOR_INDEX],
        info->color_id[DISABLED_FILL_COLOR_INDEX]);
    gx_text_button_text_color_set((GX_TEXT_BUTTON *) button,
        info->color_id[NORMAL_TEXT_COLOR_INDEX],
        info->color_id[SELECTED_TEXT_COLOR_INDEX],
        info->color_id[DISABLED_TEXT_COLOR_INDEX]);
    gx_text_button_font_set((GX_TEXT_BUTTON *) button, info->font_id[0]);
    return ((GX_WIDGET *) button);
}
        
        
void mlt_button_service_provider::AssignText(widget_info *info, int index,
    GX_RESOURCE_ID string_id)
{
    GX_WIDGET *widget = info->widget;

    if (widget)
    {
        GX_MULTI_LINE_TEXT_BUTTON *pb = (GX_MULTI_LINE_TEXT_BUTTON *)widget;

        gx_multi_line_text_button_text_id_set(pb, string_id);
    }
    info->string_id[0] = string_id;
}

