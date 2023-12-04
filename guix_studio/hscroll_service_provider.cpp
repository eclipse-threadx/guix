
#include "studiox_includes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
hscroll_service_provider::hscroll_service_provider()
{
}

///////////////////////////////////////////////////////////////////////////////
CString hscroll_service_provider::GetVarDeclaration()
{
    return CString("GX_HSCROLL_MEMBERS_DECLARE");
}

///////////////////////////////////////////////////////////////////////////////
CString hscroll_service_provider::WriteExtendedProperties(screen_generator *gen, CString &prefix, widget_info *info)
{
    CString out;
    CString propname = prefix + info->app_name;

    if (project_lib_version() >= 50302)
    {
        out.Format(_T("GX_SCROLLBAR_APPEARANCE  %s_properties =\n")
            _T("{\n")
            _T("    %d, /* scroll width */\n")
            _T("    %d, /* thumb width  */\n")
            _T("    %d, /* thumb travel min */\n")
            _T("    %d, /* thumb travel max */\n")
            _T("    %d, /* thumb border style */\n")
            _T("    %s, /* scroll fill pixelmap */\n")
            _T("    %s, /* scroll thumb pixelmap */\n")
            _T("    %s, /* scroll up pixelmap */\n")
            _T("    %s, /* scroll down pixelmap */\n")
            _T("    %s, /* scroll thumb color */\n")
            _T("    %s, /* scroll thumb border color */\n")
            _T("    %s, /* scroll button color */\n")
            _T("};\n"),
            propname,
            info->ewi.scroll.gx_scroll_width,
            info->ewi.scroll.gx_scroll_thumb_width, 
            info->ewi.scroll.gx_scroll_thumb_travel_min,
            info->ewi.scroll.gx_scroll_thumb_travel_max,
            info->ewi.scroll.gx_scroll_thumb_border_style,
            gen->GetPixelmapIdName(info->ewi.scroll.gx_scroll_fill_pixelmap),
            gen->GetPixelmapIdName(info->ewi.scroll.gx_scroll_thumb_pixelmap),
            gen->GetPixelmapIdName(info->ewi.scroll.gx_scroll_up_pixelmap),
            gen->GetPixelmapIdName(info->ewi.scroll.gx_scroll_down_pixelmap),
            gen->GetColorIdName(info->ewi.scroll.gx_scroll_thumb_color),
            gen->GetColorIdName(info->ewi.scroll.gx_scroll_thumb_border_color),
            gen->GetColorIdName(info->ewi.scroll.gx_scroll_button_color));
    }
    else
    {
        out.Format(_T("GX_SCROLLBAR_APPEARANCE  %s_properties =\n")
            _T("{\n")
            _T("    %d, /* scroll width */\n")
            _T("    %d, /* thumb width  */\n")
            _T("    %d, /* thumb travel min */\n")
            _T("    %d, /* thumb travel max */\n")
            _T("    %s, /* scroll fill pixelmap */\n")
            _T("    %s, /* scroll thumb pixelmap */\n")
            _T("    %s, /* scroll up pixelmap */\n")
            _T("    %s, /* scroll down pixelmap */\n")
            _T("    %s, /* scroll fill color */\n")
            _T("    %s, /* scroll button color */\n")
            _T("};\n"),
            propname,
            info->ewi.scroll.gx_scroll_width,
            info->ewi.scroll.gx_scroll_thumb_width, 
            info->ewi.scroll.gx_scroll_thumb_travel_min,
            info->ewi.scroll.gx_scroll_thumb_travel_max,
            gen->GetPixelmapIdName(info->ewi.scroll.gx_scroll_fill_pixelmap),
            gen->GetPixelmapIdName(info->ewi.scroll.gx_scroll_thumb_pixelmap),
            gen->GetPixelmapIdName(info->ewi.scroll.gx_scroll_up_pixelmap),
            gen->GetPixelmapIdName(info->ewi.scroll.gx_scroll_down_pixelmap),
            gen->GetColorIdName(info->color_id[NORMAL_FILL_COLOR_INDEX]),
            gen->GetColorIdName(info->ewi.scroll.gx_scroll_button_color));
    }
    return out;
}

///////////////////////////////////////////////////////////////////////////////
CString hscroll_service_provider::GetCreateFromDefFunctionName()
{
    return CString("gx_studio_horizontal_scrollbar_create");
}

///////////////////////////////////////////////////////////////////////////////
CString hscroll_service_provider::GetCreateFromDefFunction(int version)
{
    CString out;
    MakeCreatePreamble("horizontal_scrollbar", version, out);

    out += "{\n"
        "    UINT status;\n"
        "    GX_SCROLLBAR *scroll = (GX_SCROLLBAR *) control_block;\n"
        "    GX_SCROLLBAR_APPEARANCE *appearance = (GX_SCROLLBAR_APPEARANCE *) info->properties;\n"
        "    status = gx_horizontal_scrollbar_create(scroll, info->widget_name, parent, appearance, info->style);\n"
        "    return status;\n"
        "}\n";
 
    return out;
}

///////////////////////////////////////////////////////////////////////////////
void hscroll_service_provider::SaveToProject(xml_writer &writer, studiox_project *project, int display, widget_info *info)
{
    widget_service_provider::SaveToProject(writer, project, display, info);
    vscroll_service_provider::WriteScrollbarAppearance(writer, project, display, info->ewi.scroll);
}

///////////////////////////////////////////////////////////////////////////////
void hscroll_service_provider::ReadFromProject(xml_reader &reader, studiox_project *project, int display, widget_info *info, ULONG valid_styles)
{
    valid_styles |= GX_STYLE_TILE_BACKGROUND|GX_SCROLLBAR_RELATIVE_THUMB|GX_SCROLLBAR_END_BUTTONS|GX_SCROLLBAR_VERTICAL|GX_SCROLLBAR_HORIZONTAL|GX_STYLE_TILE_BACKGROUND;
    widget_service_provider::ReadFromProject(reader, project, display, info, valid_styles);
    vscroll_service_provider::ReadScrollbarAppearance(reader, project, display, info->ewi.scroll);

    // copy the scroll appearance pixlemap ids into the normal slots so that the
    // properties win can correctly detect changes.
    info->pixelmap_id[SCROLL_FILL_PIXELMAP_INDEX] = info->ewi.scroll.gx_scroll_fill_pixelmap;
    info->pixelmap_id[SCROLL_THUMB_PIXELMAP_INDEX] = info->ewi.scroll.gx_scroll_thumb_pixelmap;
    info->pixelmap_id[SCROLL_UP_PIXELMAP_INDEX] = info->ewi.scroll.gx_scroll_up_pixelmap;
    info->pixelmap_id[SCROLL_DOWN_PIXELMAP_INDEX] = info->ewi.scroll.gx_scroll_down_pixelmap;

    if (project->mHeader.project_version < 54)
    {
        //selected fill color of scrollbar is start been used after project version 5.4.0
        //set selected fill color to normal fill color incase broken old projects
        info->color_id[SELECTED_FILL_COLOR_INDEX] = info->color_id[NORMAL_FILL_COLOR_INDEX];
    }
}

///////////////////////////////////////////////////////////////////////////////
widget_info *hscroll_service_provider::CreateNewInstance(GX_WIDGET *parent)
{
    GX_RECTANGLE size;
    if (parent->gx_widget_type >= GX_TYPE_WINDOW)
    {
        gx_utility_rectangle_define(&size, 0, 0, 79, 23);
        gx_utility_rectangle_center(&size, &parent->gx_widget_size);
    
        GX_SCROLLBAR *scroll = new GX_SCROLLBAR;
        gx_horizontal_scrollbar_create(scroll, "hscroll", parent, NULL,
            GX_SCROLLBAR_HORIZONTAL | GX_SCROLLBAR_RELATIVE_THUMB | GX_SCROLLBAR_END_BUTTONS | GX_STYLE_ENABLED);
    
        widget_info *info = InitWidgetInfo((GX_WIDGET *) scroll);
        gx_system_scroll_appearance_get(GX_SCROLLBAR_HORIZONTAL, &info->ewi.scroll);

        return info;
    }
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
GX_WIDGET *hscroll_service_provider::GenerateFromInfo(GX_WIDGET *parent, widget_info *info)
{
    if (parent->gx_widget_type >= GX_TYPE_WINDOW)
    {
        GX_SCROLLBAR *scroll = new GX_SCROLLBAR;
        gx_horizontal_scrollbar_create(scroll,
            (CHAR *) info->app_name.GetString(),
            parent,  &info->ewi.scroll, info->style);
        gx_widget_fill_color_set((GX_WIDGET *) scroll,
            info->color_id[NORMAL_FILL_COLOR_INDEX], info->color_id[SELECTED_FILL_COLOR_INDEX], info->color_id[DISABLED_FILL_COLOR_INDEX]);

        return ((GX_WIDGET *) scroll);
    }
    return NULL;
}


///////////////////////////////////////////////////////////////////////////////
void hscroll_service_provider::AssignPixelmap(widget_info *info, int index,
    GX_RESOURCE_ID pixmap_id)
{
    GX_SCROLLBAR_APPEARANCE appearance = info->ewi.scroll;

    switch(index)
    {
    case SCROLL_FILL_PIXELMAP_INDEX:
        appearance.gx_scroll_fill_pixelmap = pixmap_id;
        break;

    case SCROLL_THUMB_PIXELMAP_INDEX:
        appearance.gx_scroll_thumb_pixelmap = pixmap_id;
        break;

    case SCROLL_UP_PIXELMAP_INDEX:
        appearance.gx_scroll_up_pixelmap = pixmap_id;
        break;

    case SCROLL_DOWN_PIXELMAP_INDEX:
        appearance.gx_scroll_down_pixelmap = pixmap_id;
        break;
    }
    // Even though the scrollbar keeps the pixelmap ids in it's own
    // appearance structure, we need to save the pixelmap ID into the standard
    // widget_info slot so that we can detect assignment changes
    // correctly

    info->pixelmap_id[index] = pixmap_id;
    widget_factory::ReplaceScrollbar(info, &appearance);
}

///////////////////////////////////////////////////////////////////////////////
void hscroll_service_provider::Autosize(widget_info *info)
{
    CheckResizeToPixelmap(info, RESIZE_MODE_HEIGHT);
}