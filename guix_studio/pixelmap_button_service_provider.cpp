
#include "studiox_includes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

pixelmap_button_service_provider::pixelmap_button_service_provider()
{
}

CString pixelmap_button_service_provider::GetVarDeclaration()
{
    return CString("GX_PIXELMAP_BUTTON_MEMBERS_DECLARE");
}

CString pixelmap_button_service_provider::DeclarePropertiesStruct()
{
    CString out(""
    "typedef struct\n"
    "{\n"
    "    GX_RESOURCE_ID normal_pixelmap_id;\n"
    "    GX_RESOURCE_ID selected_pixelmap_id;\n"
    "    GX_RESOURCE_ID disabled_pixelmap_id;\n"
    "} GX_PIXELMAP_BUTTON_PROPERTIES;\n\n");

    return out;
}

CString pixelmap_button_service_provider::WriteExtendedProperties(screen_generator *gen, CString &prefix, widget_info *info)
{
    CString out;
    CString propname = prefix + info->app_name;
    out.Format(_T("GX_PIXELMAP_BUTTON_PROPERTIES %s_properties =\n")
        _T("{\n")
        _T("    %s, /* normal pixelmap id */\n")
        _T("    %s, /* selected pixelmap id */\n")
        _T("    %s  /* disabled pixelmap id */\n")
        _T("};\n"),
        propname,
        gen->GetPixelmapIdName(info->pixelmap_id[NORMAL_PIXELMAP_INDEX]),
        gen->GetPixelmapIdName(info->pixelmap_id[SELECTED_PIXELMAP_INDEX]),
        gen->GetPixelmapIdName(info->pixelmap_id[DISABLED_PIXELMAP_INDEX]));
    return out;
}

CString pixelmap_button_service_provider::GetCreateFromDefFunctionName()
{
    return CString("gx_studio_pixelmap_button_create");
}

CString pixelmap_button_service_provider::GetCreateFromDefFunction(int version)
{
    CString out;
    MakeCreatePreamble("pixelmap_button", version, out);

    out += "{\n"
        "    UINT status;\n"
        "    GX_PIXELMAP_BUTTON *button = (GX_PIXELMAP_BUTTON *) control_block;\n"
        "    GX_PIXELMAP_BUTTON_PROPERTIES *props = (GX_PIXELMAP_BUTTON_PROPERTIES *) info->properties;\n"   
        "    status = gx_pixelmap_button_create(button, info->widget_name, parent,\n"
        "               props->normal_pixelmap_id,\n"
        "               props->selected_pixelmap_id,\n"
        "               props->disabled_pixelmap_id,\n"
        "               info->style, info->widget_id, &info->size);\n"
        "    return status;\n"
        "}\n";
    return out;
}

widget_info *pixelmap_button_service_provider::CreateNewInstance(GX_WIDGET *parent)
{
    GX_RECTANGLE size;
    gx_utility_rectangle_define(&size, 0, 0, 79, 23);
    gx_utility_rectangle_center(&size, &parent->gx_widget_size);
    
    GX_PIXELMAP_BUTTON *button = new GX_PIXELMAP_BUTTON;
    gx_pixelmap_button_create(button, "pixelmap_button", parent,
        0, 0, 0, GX_STYLE_ENABLED, 0, &size);
    widget_info *info = InitWidgetInfo((GX_WIDGET *) button);
    info->pixelmap_id[DISABLED_PIXELMAP_INDEX] = 0;
    info->pixelmap_id[NORMAL_PIXELMAP_INDEX] = 0;
    info->pixelmap_id[SELECTED_PIXELMAP_INDEX] = 0;
    return info;
}

GX_WIDGET *pixelmap_button_service_provider::GenerateFromInfo(GX_WIDGET *parent, widget_info *info)
{
    GX_PIXELMAP_BUTTON *button = new GX_PIXELMAP_BUTTON;

    gx_pixelmap_button_create(button,
        (CHAR *) info->app_name.GetString(),
        parent,
        info->pixelmap_id[NORMAL_PIXELMAP_INDEX],
        info->pixelmap_id[SELECTED_PIXELMAP_INDEX],
        info->pixelmap_id[DISABLED_PIXELMAP_INDEX],
        info->style, 0, &info->size);

    gx_widget_fill_color_set((GX_WIDGET *) button,
        info->color_id[NORMAL_FILL_COLOR_INDEX], info->color_id[SELECTED_FILL_COLOR_INDEX], info->color_id[DISABLED_FILL_COLOR_INDEX]);
    return ((GX_WIDGET *) button);
}
        
void pixelmap_button_service_provider::SaveToProject(xml_writer &writer, studiox_project *project, int display, widget_info *info)
{
    button_service_provider::SaveToProject(writer, project, display, info);
    WritePixelmapId(writer, project, display, "normal_pixelmap", info->pixelmap_id[NORMAL_PIXELMAP_INDEX]);
    WritePixelmapId(writer, project, display, "selected_pixelmap", info->pixelmap_id[SELECTED_PIXELMAP_INDEX]);
    WritePixelmapId(writer, project, display, "disabled_pixelmap", info->pixelmap_id[DISABLED_PIXELMAP_INDEX]);
}

void pixelmap_button_service_provider::ReadFromProject(xml_reader &reader, studiox_project *project, int display, widget_info *info, ULONG valid_styles)
{
    valid_styles |= GX_PIXELMAP_HALIGN_MASK|GX_PIXELMAP_VALIGN_MASK;
    button_service_provider::ReadFromProject(reader, project, display, info, valid_styles);
    info->pixelmap_id[NORMAL_PIXELMAP_INDEX] = ReadPixelmapId(reader, project, display, "normal_pixelmap");
    info->pixelmap_id[SELECTED_PIXELMAP_INDEX] = ReadPixelmapId(reader, project, display, "selected_pixelmap");
    info->pixelmap_id[DISABLED_PIXELMAP_INDEX] = ReadPixelmapId(reader, project, display, "disabled_pixelmap");
}
        

void pixelmap_button_service_provider::AssignPixelmap(widget_info *info, int index,
    GX_RESOURCE_ID pixmap_id)
{
    GX_PIXELMAP_BUTTON *pb = (GX_PIXELMAP_BUTTON *) (info->widget);

    switch(index)
    {
    case NORMAL_PIXELMAP_INDEX:
        info->pixelmap_id[NORMAL_PIXELMAP_INDEX] = pixmap_id;

        if (pb)
        {
            gx_pixelmap_button_pixelmap_set(pb, pixmap_id,
                pb->gx_pixelmap_button_selected_id,
                pb->gx_pixelmap_button_disabled_id);
        }
        break;
    
    case SELECTED_PIXELMAP_INDEX:
        info->pixelmap_id[SELECTED_PIXELMAP_INDEX] = pixmap_id;

        if (pb)
        {
            gx_pixelmap_button_pixelmap_set(pb,
                pb->gx_pixelmap_button_normal_id,
                pixmap_id,
                pb->gx_pixelmap_button_disabled_id);
        }
        break;

    case DISABLED_PIXELMAP_INDEX:
        info->pixelmap_id[DISABLED_PIXELMAP_INDEX] = pixmap_id;

        if (pb)
        {
            gx_pixelmap_button_pixelmap_set(pb,
                pb->gx_pixelmap_button_normal_id,
                pb->gx_pixelmap_button_selected_id,
                pixmap_id);
        }
        break;
    }
}

void pixelmap_button_service_provider::Autosize(widget_info *info)
{
    CheckResizeToPixelmap(info, RESIZE_MODE_ALL);
}
