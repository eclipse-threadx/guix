 
#include "studiox_includes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
window_service_provider::window_service_provider()
{
}

///////////////////////////////////////////////////////////////////////////////
CString window_service_provider::GetVarDeclaration()
{
    return CString("GX_WINDOW_MEMBERS_DECLARE");
}

///////////////////////////////////////////////////////////////////////////////
CString window_service_provider::DeclarePropertiesStruct()
{
    CString out(""
    "typedef struct\n"
    "{\n"
    "    GX_RESOURCE_ID wallpaper_id;\n"
    "} GX_WINDOW_PROPERTIES;\n\n");
    return out;
}

///////////////////////////////////////////////////////////////////////////////
CString window_service_provider::WriteExtendedProperties(screen_generator *gen, CString &prefix, widget_info *info)
{
    CString out;
    CString propname = prefix + info->app_name;
    out.Format(_T("GX_WINDOW_PROPERTIES %s_properties =\n")
        _T("{\n")
        _T("    %s  /* wallpaper pixelmap id */\n};\n"), 
        propname,
        gen->GetPixelmapIdName(info->pixelmap_id[WALLPAPER_PIXELMAP_INDEX]));
    return out;
}

///////////////////////////////////////////////////////////////////////////////
CString window_service_provider::GetCreateFromDefFunctionName()
{
    return CString("gx_studio_window_create");
}

///////////////////////////////////////////////////////////////////////////////
CString window_service_provider::GetCreateFromDefFunction(int version)
{
    CString out;
    MakeCreatePreamble("window", version, out);

    out +="{\n"
        "    UINT status;\n"
        "    GX_WINDOW *window = (GX_WINDOW *) control_block;\n"
        "    GX_WINDOW_PROPERTIES *props = (GX_WINDOW_PROPERTIES *) info->properties;\n"   
        "    status = gx_window_create(window, info->widget_name, parent, info->style, info->widget_id, &info->size);\n"
        "    if (status == GX_SUCCESS)\n"
        "    {\n"
        "        if (props->wallpaper_id)\n"
        "        {\n"
        "            gx_window_wallpaper_set(window, props->wallpaper_id, info->style & GX_STYLE_TILE_WALLPAPER);\n"
        "        }\n"
        "    }\n"
        "    return status;\n"
        "}\n";
    return out;
}


///////////////////////////////////////////////////////////////////////////////
widget_info *window_service_provider::CreateNewInstance(GX_WIDGET *parent)
{
    int parent_size;

    GX_RECTANGLE size = parent->gx_widget_size;

    if (parent->gx_widget_type != GX_TYPE_ROOT_WINDOW)
    {
        gx_utility_rectangle_define(&size, 0, 0, 79, 23);
        parent_size = (parent->gx_widget_size.gx_rectangle_right -
                       parent->gx_widget_size.gx_rectangle_left) / 2;
        size.gx_rectangle_right = size.gx_rectangle_left + parent_size;

        parent_size = (parent->gx_widget_size.gx_rectangle_bottom -
                       parent->gx_widget_size.gx_rectangle_top) / 2;
        size.gx_rectangle_bottom = size.gx_rectangle_top + parent_size;

        gx_utility_rectangle_center(&size, &parent->gx_widget_size);
    }
    
    GX_WINDOW *window = new GX_WINDOW;
    gx_window_create(window, "window", parent,
        GX_STYLE_ENABLED | GX_STYLE_BORDER_THIN, 0, &size);
    gx_widget_style_remove(window, GX_STYLE_DRAW_SELECTED);
    widget_info *info = InitWidgetInfo((GX_WIDGET *) window);
    info->pixelmap_id[WALLPAPER_PIXELMAP_INDEX] = 0;

    return info;
}

///////////////////////////////////////////////////////////////////////////////
GX_WIDGET *window_service_provider::GenerateFromInfo(GX_WIDGET *parent, widget_info *info)
{
    GX_WINDOW *win = new GX_WINDOW;
    gx_window_create(win,
        (CHAR *) info->app_name.GetString(),
        parent,
        info->style,
        0,
        &info->size);

    gx_widget_fill_color_set(win,
        info->color_id[NORMAL_FILL_COLOR_INDEX],
        info->color_id[SELECTED_FILL_COLOR_INDEX],
        info->color_id[DISABLED_FILL_COLOR_INDEX]);

    if (info->pixelmap_id[WALLPAPER_PIXELMAP_INDEX] > 0)
    {
        BOOL tile = FALSE;

        if (info->style & GX_STYLE_TILE_WALLPAPER)
        {
            tile = TRUE;
        }
        gx_window_wallpaper_set(win, info->pixelmap_id[WALLPAPER_PIXELMAP_INDEX], tile);
    }
    return ((GX_WIDGET *) win);
}
        
///////////////////////////////////////////////////////////////////////////////
void window_service_provider::SaveToProject(xml_writer &writer, studiox_project *project, int display, widget_info *info)
{
    widget_service_provider::SaveToProject(writer, project, display, info);
    WritePixelmapId(writer, project, display, "wallpaper", info->pixelmap_id[WALLPAPER_PIXELMAP_INDEX]);
}

///////////////////////////////////////////////////////////////////////////////
void window_service_provider::ReadFromProject(xml_reader &reader, studiox_project *project, int display, widget_info *info, ULONG valid_styles)
{
    valid_styles |= GX_STYLE_TILE_WALLPAPER;

    widget_service_provider::ReadFromProject(reader, project, display, info, valid_styles);
    info->pixelmap_id[WALLPAPER_PIXELMAP_INDEX] = ReadPixelmapId(reader, project, display, "wallpaper");

    if (project->mHeader.project_version <= 54)
    {
        //selected fill color is enabled after version 5.4.0
        info->color_id[SELECTED_FILL_COLOR_INDEX] = info->color_id[NORMAL_FILL_COLOR_INDEX];
    }
}
        
///////////////////////////////////////////////////////////////////////////////
void window_service_provider::AssignPixelmap(widget_info *info, int index,
    GX_RESOURCE_ID pixmap_id)
{
    GX_WINDOW *window = (GX_WINDOW *) (info->widget);

    switch(index)
    {
    case WALLPAPER_PIXELMAP_INDEX:
        info->pixelmap_id[WALLPAPER_PIXELMAP_INDEX] = pixmap_id;

        if (window)
        {
            gx_window_wallpaper_set(window, pixmap_id, info->style & GX_STYLE_TILE_WALLPAPER);
        }
        break;

    default:
        break;
    }
}

///////////////////////////////////////////////////////////////////////////////
void window_service_provider::Autosize(widget_info *info)
{
    if (info->pixelmap_id[0] &&
        !(info->style & GX_STYLE_TILE_WALLPAPER))
    {
        CheckResizeToPixelmap(info, RESIZE_MODE_ALL);
    }
}