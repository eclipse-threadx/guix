
#include "studiox_includes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

accordion_menu_service_provider::accordion_menu_service_provider()
{
}

CString accordion_menu_service_provider::GetCreateFromDefFunctionName()
{
    return CString("gx_studio_accordion_menu_create");
}

CString accordion_menu_service_provider::GetCreateFromDefFunction(int version)
{
    CString out;
    MakeCreatePreamble("accordion_menu", version, out);

    out += "{\n"
        "    UINT status;\n"
        "    GX_ACCORDION_MENU *accordion = (GX_ACCORDION_MENU *) control_block;\n"
        "    status = gx_accordion_menu_create(accordion, info->widget_name,\n"
        "                                      parent, info->style, info->widget_id, &info->size);\n"
        "    return status;\n"
        "}\n";
    return out;
}

widget_info *accordion_menu_service_provider::CreateNewInstance(GX_WIDGET *parent)
{
    GX_RECTANGLE size;
    int parent_size;

    gx_utility_rectangle_define(&size, 0, 0, 79, 23);

    parent_size = (parent->gx_widget_size.gx_rectangle_right -
        parent->gx_widget_size.gx_rectangle_left) / 2;
    size.gx_rectangle_right = size.gx_rectangle_left + parent_size;

    parent_size = (parent->gx_widget_size.gx_rectangle_bottom -
        parent->gx_widget_size.gx_rectangle_top) / 2;
    size.gx_rectangle_bottom = size.gx_rectangle_top + parent_size;

    gx_utility_rectangle_center(&size, &parent->gx_widget_size);

    GX_ACCORDION_MENU *accordion = new GX_ACCORDION_MENU;

    gx_accordion_menu_create(accordion, "accordion", parent, GX_STYLE_ENABLED | GX_STYLE_BORDER_THIN, 0, &size);
    widget_info *info = InitWidgetInfo((GX_WIDGET *)accordion);

    return info;
}

GX_WIDGET *accordion_menu_service_provider::GenerateFromInfo(GX_WIDGET *parent, widget_info *info)
{
    GX_ACCORDION_MENU *accordion = new GX_ACCORDION_MENU;
    gx_accordion_menu_create(accordion,
        (CHAR *)info->app_name.GetString(),
        parent,
        info->style, 0,
        &info->size);

    gx_widget_fill_color_set((GX_WIDGET *)accordion,
        info->color_id[NORMAL_FILL_COLOR_INDEX], info->color_id[SELECTED_FILL_COLOR_INDEX], info->color_id[DISABLED_FILL_COLOR_INDEX]);

    return (GX_WIDGET *)accordion;
}