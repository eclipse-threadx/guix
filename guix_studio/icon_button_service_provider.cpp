
#include "studiox_includes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

icon_button_service_provider::icon_button_service_provider()
{
}

CString icon_button_service_provider::GetVarDeclaration()
{
    return CString("GX_ICON_BUTTON_MEMBERS_DECLARE");
}

CString icon_button_service_provider::DeclarePropertiesStruct()
{
    CString out(""
    "typedef struct\n"
    "{\n"
    "    GX_RESOURCE_ID pixelmap_id;\n"
    "} GX_ICON_BUTTON_PROPERTIES;\n\n");
    return out;
}

CString icon_button_service_provider::WriteExtendedProperties(screen_generator *gen, CString &prefix, widget_info *info)
{
    CString out;
    CString propname = prefix + info->app_name;
    out.Format(_T("GX_ICON_BUTTON_PROPERTIES %s_properties =\n")
        _T("{\n")
        _T("    %s  /* pixelmap id */\n};\n"),
        propname,
        gen->GetPixelmapIdName(info->pixelmap_id[NORMAL_PIXELMAP_INDEX])
        );
    return out;
}

CString icon_button_service_provider::GetCreateFromDefFunctionName()
{
    return CString("gx_studio_icon_button_create");
}

CString icon_button_service_provider::GetCreateFromDefFunction(int version)
{
    CString out;
    MakeCreatePreamble("icon_button", version, out);

    out += "{\n"
        "    UINT status;\n"
        "    GX_ICON_BUTTON *button = (GX_ICON_BUTTON *) control_block;\n"
        "    GX_ICON_BUTTON_PROPERTIES *props = (GX_ICON_BUTTON_PROPERTIES *) info->properties;\n"   
        "    status = gx_icon_button_create(button, info->widget_name, parent, props->pixelmap_id, info->style, info->widget_id, &info->size);\n"
         "    return status;\n"
        "}\n";

    return out;
}

widget_info *icon_button_service_provider::CreateNewInstance(GX_WIDGET *parent)
{
    GX_RECTANGLE size;
    gx_utility_rectangle_define(&size, 0, 0, 31, 31);
    gx_utility_rectangle_center(&size, &parent->gx_widget_size);
    
    GX_ICON_BUTTON *button = new GX_ICON_BUTTON;
    gx_icon_button_create(button, "icon_button", parent,
        0, GX_STYLE_BORDER_RAISED|GX_STYLE_ENABLED|GX_STYLE_HALIGN_CENTER|GX_STYLE_VALIGN_CENTER|GX_STYLE_TRANSPARENT, 0, &size);

    widget_info *info = InitWidgetInfo((GX_WIDGET *) button);
    return info;
}

GX_WIDGET *icon_button_service_provider::GenerateFromInfo(GX_WIDGET *parent, widget_info *info)
{
    GX_ICON_BUTTON *button = new GX_ICON_BUTTON;
    gx_icon_button_create(button,
        (CHAR *) info->app_name.GetString(),
        parent, info->pixelmap_id[0],
        info->style,
        0,
        &info->size);
    gx_widget_fill_color_set((GX_WIDGET *) button,
        info->color_id[NORMAL_FILL_COLOR_INDEX], info->color_id[SELECTED_FILL_COLOR_INDEX], info->color_id[DISABLED_FILL_COLOR_INDEX]);
    return ((GX_WIDGET *) button);
}
        
void icon_button_service_provider::SaveToProject(xml_writer &writer, studiox_project *project, int display, widget_info *info)
{
    button_service_provider::SaveToProject(writer, project, display, info);
    WritePixelmapId(writer, project, display, "pixelmap_id", info->pixelmap_id[NORMAL_PIXELMAP_INDEX]);
}

void icon_button_service_provider::ReadFromProject(xml_reader &reader, studiox_project *project, int display, widget_info *info, ULONG valid_styles)
{
    valid_styles |= GX_PIXELMAP_HALIGN_MASK|GX_PIXELMAP_VALIGN_MASK;
    button_service_provider::ReadFromProject(reader, project, display, info, valid_styles);
    info->pixelmap_id[NORMAL_PIXELMAP_INDEX] = ReadPixelmapId(reader, project, display, "pixelmap_id");
}
        

void icon_button_service_provider::AssignPixelmap(widget_info *info, int index,
    GX_RESOURCE_ID pixmap_id)
{
    info->pixelmap_id[index] = pixmap_id;
    GX_WIDGET *widget = info->widget;

    if (widget)
    {
        GX_ICON_BUTTON *button = (GX_ICON_BUTTON *)widget;
        gx_icon_button_pixelmap_set(button, pixmap_id);
    }
}

void icon_button_service_provider::Autosize(widget_info *info)
{
    CheckResizeToPixelmap(info, RESIZE_MODE_ALL);
}