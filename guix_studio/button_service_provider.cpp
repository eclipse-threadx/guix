
#include "studiox_includes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

button_service_provider::button_service_provider()
{
}

CString button_service_provider::GetVarDeclaration()
{
    return CString("GX_BUTTON_MEMBERS_DECLARE");
}

CString button_service_provider::GetCreateFromDefFunctionName()
{
    return CString("gx_studio_button_create");
}

CString button_service_provider::GetCreateFromDefFunction(int version)
{
    CString out;
    MakeCreatePreamble("button", version, out);

    out += "{\n"
        "    UINT status;\n"
        "    GX_BUTTON *button = (GX_BUTTON *) control_block;\n"
        "    status = gx_button_create(button, info->widget_name, parent, info->style, info->widget_id, &info->size);\n"
        "    return status;\n"
        "}\n";
    return out;
}

widget_info *button_service_provider::CreateNewInstance(GX_WIDGET *parent)
{
    GX_RECTANGLE size;
    gx_utility_rectangle_define(&size, 0, 0, 79, 23);
    gx_utility_rectangle_center(&size, &parent->gx_widget_size);
    
    GX_BUTTON *button = new GX_BUTTON;
    gx_button_create(button, "button", parent,
        GX_STYLE_ENABLED|GX_STYLE_BORDER_RAISED, 0, &size);
    widget_info *info = InitWidgetInfo((GX_WIDGET *) button);
    return info;
}

void button_service_provider::ReadFromProject(xml_reader &reader, studiox_project *project, int display, widget_info *info, ULONG valid_styles)
{
    valid_styles |=  GX_STYLE_BUTTON_PUSHED|GX_STYLE_BUTTON_TOGGLE|GX_STYLE_BUTTON_RADIO|GX_STYLE_BUTTON_EVENT_ON_PUSH|GX_STYLE_BUTTON_REPEAT;
    widget_service_provider::ReadFromProject(reader, project, display, info, valid_styles);
}


GX_WIDGET *button_service_provider::GenerateFromInfo(GX_WIDGET *parent, widget_info *info)
{
    GX_BUTTON *button = new GX_BUTTON;
    gx_button_create(button,
        (CHAR *) info->app_name.GetString(),
        parent,
        info->style,
        0,
        &info->size);

    gx_widget_fill_color_set((GX_WIDGET *) button,
        info->color_id[NORMAL_FILL_COLOR_INDEX], info->color_id[SELECTED_FILL_COLOR_INDEX], info->color_id[DISABLED_FILL_COLOR_INDEX]);
    return ((GX_WIDGET *) button);
}
        
