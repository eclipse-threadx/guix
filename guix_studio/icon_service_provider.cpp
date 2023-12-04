
#include "studiox_includes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

icon_service_provider::icon_service_provider()
{
}

CString icon_service_provider::GetVarDeclaration()
{
    return CString("GX_ICON_MEMBERS_DECLARE");
}

CString icon_service_provider::DeclarePropertiesStruct()
{
    CString out(""
    "typedef struct\n"
    "{\n"
    "    GX_RESOURCE_ID normal_pixelmap_id;\n"
    "    GX_RESOURCE_ID selected_pixelmap_id;\n"
    "} GX_ICON_PROPERTIES;\n\n");
    return out;
}

CString icon_service_provider::WriteExtendedProperties(screen_generator *gen, CString &prefix, widget_info *info)
{
    CString out;
    CString propname = prefix + info->app_name;
    out.Format(_T("GX_ICON_PROPERTIES %s_properties =\n")
        _T("{\n")
        _T("    %s, /* normal pixelmap id */\n")
        _T("    %s  /* selected pixelmap id */\n")
        _T("};\n"),
        propname,
        gen->GetPixelmapIdName(info->pixelmap_id[NORMAL_PIXELMAP_INDEX]),
        gen->GetPixelmapIdName(info->pixelmap_id[SELECTED_PIXELMAP_INDEX]));
    return out;
}

CString icon_service_provider::GetCreateFromDefFunctionName()
{
    return CString("gx_studio_icon_create");
}

CString icon_service_provider::GetCreateFromDefFunction(int version)
{
    CString out;
    MakeCreatePreamble("icon", version, out);

   out += "{\n"
        "    UINT status;\n"
        "    GX_ICON *icon = (GX_ICON *) control_block;\n"
        "    GX_ICON_PROPERTIES *props = (GX_ICON_PROPERTIES *) info->properties;\n"   
        "    status = gx_icon_create(icon, info->widget_name, parent, props->normal_pixelmap_id, info->style, info->widget_id, info->size.gx_rectangle_left, info->size.gx_rectangle_top);\n"
        "    if (props->selected_pixelmap_id)\n"
        "    {\n"
        "        gx_icon_pixelmap_set(icon, props->normal_pixelmap_id, props->selected_pixelmap_id);\n"
        "    }\n"
        "    else\n"
        "    {\n"
        "        gx_widget_resize((GX_WIDGET *)icon, (GX_RECTANGLE *)&info->size);\n"
        "    }\n"
        "    return status;\n"
        "}\n";
    return out;
}

widget_info *icon_service_provider::CreateNewInstance(GX_WIDGET *parent)
{
    GX_RECTANGLE size;
    gx_utility_rectangle_define(&size, 0, 0, 39, 39);
    gx_utility_rectangle_center(&size, &parent->gx_widget_size);
    
    GX_ICON *icon = new GX_ICON;
    gx_icon_create(icon, "icon", parent, 0,
        GX_STYLE_ENABLED|GX_STYLE_HALIGN_LEFT|GX_STYLE_VALIGN_TOP,
        0, size.gx_rectangle_left, size.gx_rectangle_top);

    // the icon will be 1x1 pixels since it has no pixelmap, so make it larger
    // so that you can actually see it.
    gx_widget_resize((GX_WIDGET *) icon, &size);

    widget_info *info = InitWidgetInfo((GX_WIDGET *) icon);
    info->pixelmap_id[NORMAL_PIXELMAP_INDEX] = 0;
    info->pixelmap_id[SELECTED_PIXELMAP_INDEX] = 0;
    return info;
}

GX_WIDGET *icon_service_provider::GenerateFromInfo(GX_WIDGET *parent, widget_info *info)
{
    GX_ICON *icon = new GX_ICON;
    gx_icon_create(icon,
        (CHAR *) info->app_name.GetString(),
        parent, info->pixelmap_id[NORMAL_PIXELMAP_INDEX],
        info->style, 0,
        info->size.gx_rectangle_left, info->size.gx_rectangle_top);

    gx_widget_resize((GX_WIDGET *) icon, &info->size);
    gx_widget_fill_color_set((GX_WIDGET *) icon,
        info->color_id[NORMAL_FILL_COLOR_INDEX], info->color_id[SELECTED_FILL_COLOR_INDEX], info->color_id[DISABLED_FILL_COLOR_INDEX]);

    return ((GX_WIDGET *) icon);
}
        
void icon_service_provider::SaveToProject(xml_writer &writer, studiox_project *project, int display, widget_info *info)
{
    widget_service_provider::SaveToProject(writer, project, display, info);
    WritePixelmapId(writer, project, display, "normal_map_id", info->pixelmap_id[NORMAL_PIXELMAP_INDEX]);
    WritePixelmapId(writer, project, display, "selected_map_id", info->pixelmap_id[SELECTED_PIXELMAP_INDEX]);
}

void icon_service_provider::ReadFromProject(xml_reader &reader, studiox_project *project, int display,widget_info *info, ULONG valid_styles)
{
    valid_styles |= GX_PIXELMAP_HALIGN_MASK|GX_PIXELMAP_VALIGN_MASK;
    widget_service_provider::ReadFromProject(reader, project, display, info, valid_styles);
    info->pixelmap_id[NORMAL_PIXELMAP_INDEX] = ReadPixelmapId(reader, project, display, "normal_map_id");
    info->pixelmap_id[SELECTED_PIXELMAP_INDEX] = ReadPixelmapId(reader, project, display, "selected_map_id");

    // Validate HALIGN and VALIGN styles, as these were added later

    if ((info->style & GX_PIXELMAP_HALIGN_MASK) == 0)
    {
        info->style |= GX_STYLE_HALIGN_LEFT;
    }

    if ((info->style & GX_PIXELMAP_VALIGN_MASK) == 0)
    {
        info->style |= GX_STYLE_VALIGN_TOP;
    }
}
        

void icon_service_provider::AssignPixelmap(widget_info *info, int index,
    GX_RESOURCE_ID pixmap_id)
{
    GX_ICON *picon = (GX_ICON *) (info->widget);
    GX_RECTANGLE newsize;

    switch (index)
    {
    case NORMAL_PIXELMAP_INDEX:
        info->pixelmap_id[NORMAL_PIXELMAP_INDEX] = pixmap_id;

        if (picon)
        {
            gx_icon_pixelmap_set(picon, pixmap_id,
                picon->gx_icon_selected_pixelmap);

        }
        break;
    
    case SELECTED_PIXELMAP_INDEX:
        info->pixelmap_id[SELECTED_PIXELMAP_INDEX] = pixmap_id;

        if (picon)
        {
            gx_icon_pixelmap_set(picon,
                picon->gx_icon_normal_pixelmap,
                pixmap_id);

            GetPropsWin()->WidgetWasMoved();
            GetTargetScreen()->WidgetWasMoved(info);
        }
        break;
    }

    // check to see if the icon auto-sized to the pixelmap, if so 
    // notify the properties window
    if (picon)
    {
        newsize = picon->gx_widget_size;

        if (!gx_utility_rectangle_compare(&newsize, &info->size))
        {
            info->size = newsize;
            GetPropsWin()->WidgetWasMoved();
            GetTargetScreen()->WidgetWasMoved(info);
        }
    }
}

void icon_service_provider::Autosize(widget_info *info)
{
    CheckResizeToPixelmap(info, RESIZE_MODE_ALL);
}
