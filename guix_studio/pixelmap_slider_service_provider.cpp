
#include "studiox_includes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

pixelmap_slider_service_provider::pixelmap_slider_service_provider()
{
}

CString pixelmap_slider_service_provider::DeclarePropertiesStruct()
{
    CString out(""
    "typedef struct\n"
    "{\n"
    "    int min_val;\n"
    "    int max_val;\n"
    "    int current_val;\n"
    "    int increment;\n"
    "    GX_VALUE min_travel;\n"
    "    GX_VALUE max_travel;\n"
    "    GX_VALUE needle_width;\n"
    "    GX_VALUE needle_height;\n"
    "    GX_VALUE needle_inset;\n"
    "    GX_VALUE needle_hotspot;\n"
    "    GX_RESOURCE_ID lower_pixelmap;\n"
    "    GX_RESOURCE_ID upper_pixelmap;\n"
    "    GX_RESOURCE_ID needle_pixelmap;\n"
    "} GX_PIXELMAP_SLIDER_PROPERTIES;\n\n");
    return out;
}

CString pixelmap_slider_service_provider::WriteExtendedProperties(screen_generator *gen, CString &prefix, widget_info *info)
{
    CString out;
    CString propname = prefix + info->app_name;
    out.Format(_T("GX_PIXELMAP_SLIDER_PROPERTIES %s_properties =\n")
        _T("{\n")
        _T("    %d, /* minimum value */\n")
        _T("    %d, /* maximum value */\n")
        _T("    %d, /* current value */\n")
        _T("    %d, /* increment */\n")
        _T("    %d, /* minimum travel */\n")
        _T("    %d, /* maximum travel */\n")
        _T("    %d, /* needle width */\n")
        _T("    %d, /* needle height */\n")
        _T("    %d, /* needle inset */\n")
        _T("    %d, /* needle hotspot */\n")
        _T("    %s, /* lower pixelmap id */\n")
        _T("    %s, /* upper pixelmap id */\n")
        _T("    %s  /* needle pixelmap id */\n};\n"),
        propname,
        info->ewi.slider.gx_slider_info_min_val,
        info->ewi.slider.gx_slider_info_max_val,
        info->ewi.slider.gx_slider_info_current_val,
        info->ewi.slider.gx_slider_info_increment,
        info->ewi.slider.gx_slider_info_min_travel,
        info->ewi.slider.gx_slider_info_max_travel,
        info->ewi.slider.gx_slider_info_needle_width,
        info->ewi.slider.gx_slider_info_needle_height,
        info->ewi.slider.gx_slider_info_needle_inset,
        info->ewi.slider.gx_slider_info_needle_hotspot_offset,
        gen->GetPixelmapIdName(info->pixelmap_id[LOWER_PIXELMAP_INDEX]),
        gen->GetPixelmapIdName(info->pixelmap_id[UPPER_PIXELMAP_INDEX]),
        gen->GetPixelmapIdName(info->pixelmap_id[NEEDLE_PIXELMAP_INDEX]));
    return out;
}

CString pixelmap_slider_service_provider::GetCreateFromDefFunctionName()
{
    return CString("gx_studio_pixelmap_slider_create");
}

CString pixelmap_slider_service_provider::GetCreateFromDefFunction(int version)
{
    CString out;
    MakeCreatePreamble("pixelmap_slider", version, out);

    out += "{\n"
        "    UINT status;\n"
        "    GX_PIXELMAP_SLIDER *slider = (GX_PIXELMAP_SLIDER *) control_block;\n"
        "    GX_PIXELMAP_SLIDER_PROPERTIES *props = (GX_PIXELMAP_SLIDER_PROPERTIES *) info->properties;\n"   
        "    GX_PIXELMAP_SLIDER_INFO pixelmap_info;\n"
        "    GX_SLIDER_INFO slider_info;\n"
        "    slider_info.gx_slider_info_min_val = props->min_val;\n"
        "    slider_info.gx_slider_info_max_val = props->max_val;\n"
        "    slider_info.gx_slider_info_current_val = props->current_val;\n"
        "    slider_info.gx_slider_info_increment = props->increment;\n"
        "    slider_info.gx_slider_info_min_travel = props->min_travel;\n"
        "    slider_info.gx_slider_info_max_travel = props->max_travel;\n"
        "    slider_info.gx_slider_info_needle_width = props->needle_width;\n"
        "    slider_info.gx_slider_info_needle_height = props->needle_height;\n"
        "    slider_info.gx_slider_info_needle_inset = props->needle_inset;\n"
        "    slider_info.gx_slider_info_needle_hotspot_offset = props->needle_hotspot;\n"
        "    pixelmap_info.gx_pixelmap_slider_info_lower_background_pixelmap = props->lower_pixelmap;\n"
        "    pixelmap_info.gx_pixelmap_slider_info_upper_background_pixelmap = props->upper_pixelmap;\n"
        "    pixelmap_info.gx_pixelmap_slider_info_needle_pixelmap = props->needle_pixelmap;\n"
        "    status = gx_pixelmap_slider_create(slider,\n"
        "                    info->widget_name,\n"
        "                    parent,\n"
        "                    &slider_info,\n"
        "                    &pixelmap_info,\n"
        "                    info->style,\n"
        "                    info->widget_id,\n"
        "                    &info->size);\n"
        "    return status;\n"
        "}\n";
    return out;
}

widget_info *pixelmap_slider_service_provider::CreateNewInstance(GX_WIDGET *parent)
{
    GX_RECTANGLE size;
    gx_utility_rectangle_define(&size, 0, 0, 79, 23);
    gx_utility_rectangle_center(&size, &parent->gx_widget_size);

    GX_PIXELMAP_SLIDER *slider = new GX_PIXELMAP_SLIDER;
    GX_PIXELMAP_SLIDER_INFO pixelmap_info;
    GX_SLIDER_INFO slider_info;

    slider_info.gx_slider_info_min_val = 0;
    slider_info.gx_slider_info_max_val = 100;
    slider_info.gx_slider_info_current_val = 50;
    slider_info.gx_slider_info_increment = 10;
    slider_info.gx_slider_info_min_travel = 0;
    slider_info.gx_slider_info_max_travel = 0;
    slider_info.gx_slider_info_needle_width = 5;
    slider_info.gx_slider_info_needle_height = 10;
    slider_info.gx_slider_info_needle_inset = 5;
    slider_info.gx_slider_info_needle_hotspot_offset = 2;

    pixelmap_info.gx_pixelmap_slider_info_lower_background_pixelmap = 0;
    pixelmap_info.gx_pixelmap_slider_info_upper_background_pixelmap = 0;
    pixelmap_info.gx_pixelmap_slider_info_needle_pixelmap = 0;

    gx_pixelmap_slider_create(slider,
                    "pixelmap_slider",
                    parent,
                    &slider_info,
                    &pixelmap_info,
                    GX_STYLE_BORDER_RAISED|GX_STYLE_ENABLED, 0,
                    &size);

    widget_info *info = InitWidgetInfo((GX_WIDGET *) slider);
    
    info->ewi.slider = slider_info;
    return info;
}

GX_WIDGET *pixelmap_slider_service_provider::GenerateFromInfo(GX_WIDGET *parent, widget_info *info)
{
    GX_PIXELMAP_SLIDER *slider = new GX_PIXELMAP_SLIDER;
    GX_PIXELMAP_SLIDER_INFO pixelmap_info;

    pixelmap_info.gx_pixelmap_slider_info_lower_background_pixelmap = info->pixelmap_id[LOWER_PIXELMAP_INDEX];
    pixelmap_info.gx_pixelmap_slider_info_upper_background_pixelmap = info->pixelmap_id[UPPER_PIXELMAP_INDEX];
    pixelmap_info.gx_pixelmap_slider_info_needle_pixelmap = info->pixelmap_id[NEEDLE_PIXELMAP_INDEX];

    gx_pixelmap_slider_create(slider,
                    (CHAR *) info->app_name.GetString(),
                    parent,
                    &info->ewi.slider,
                    &pixelmap_info,
                    info->style, 0,
                     &info->size);
    gx_widget_fill_color_set((GX_WIDGET *) slider,
        info->color_id[NORMAL_FILL_COLOR_INDEX], info->color_id[SELECTED_FILL_COLOR_INDEX], info->color_id[DISABLED_FILL_COLOR_INDEX]);

    return ((GX_WIDGET *) slider);
}
        
void pixelmap_slider_service_provider::SaveToProject(xml_writer &writer, studiox_project *project, int display, widget_info *info)
{
    slider_service_provider::SaveToProject(writer, project, display, info);
    WritePixelmapId(writer, project, display, "lower_pixmap", info->pixelmap_id[LOWER_PIXELMAP_INDEX]);
    WritePixelmapId(writer, project, display, "upper_pixmap", info->pixelmap_id[UPPER_PIXELMAP_INDEX]);
    WritePixelmapId(writer, project, display, "needle_pixmap", info->pixelmap_id[NEEDLE_PIXELMAP_INDEX]);
}

void pixelmap_slider_service_provider::ReadFromProject(xml_reader &reader, studiox_project *project, int display, widget_info *info, ULONG valid_styles)
{
    valid_styles |= GX_PIXELMAP_HALIGN_MASK|GX_PIXELMAP_VALIGN_MASK|GX_STYLE_TILE_BACKGROUND;
    slider_service_provider::ReadFromProject(reader, project, display, info, valid_styles);
    info->pixelmap_id[LOWER_PIXELMAP_INDEX] = ReadPixelmapId(reader, project, display, "lower_pixmap");
    info->pixelmap_id[UPPER_PIXELMAP_INDEX] = ReadPixelmapId(reader, project, display, "upper_pixmap");
    info->pixelmap_id[NEEDLE_PIXELMAP_INDEX] = ReadPixelmapId(reader, project, display, "needle_pixmap");
}

void pixelmap_slider_service_provider::AssignPixelmap(widget_info *info,
    int index, GX_RESOURCE_ID pixmap_id)
{
    GX_PIXELMAP_SLIDER_INFO slider_info;
    GX_PIXELMAP_SLIDER *slider = (GX_PIXELMAP_SLIDER *) (info->widget);

    switch(index)
    {
    case LOWER_PIXELMAP_INDEX:
        info->pixelmap_id[LOWER_PIXELMAP_INDEX] = pixmap_id;

        if (slider)
        {
            slider_info.gx_pixelmap_slider_info_lower_background_pixelmap = pixmap_id;
            slider_info.gx_pixelmap_slider_info_needle_pixelmap = info->pixelmap_id[NEEDLE_PIXELMAP_INDEX];
            slider_info.gx_pixelmap_slider_info_upper_background_pixelmap = info->pixelmap_id[UPPER_PIXELMAP_INDEX];
            gx_pixelmap_slider_pixelmap_set(slider, &slider_info);
        }
        break;

    case UPPER_PIXELMAP_INDEX:
        info->pixelmap_id[UPPER_PIXELMAP_INDEX] = pixmap_id;

        if (slider)
        {
            slider_info.gx_pixelmap_slider_info_upper_background_pixelmap = pixmap_id;
            slider_info.gx_pixelmap_slider_info_needle_pixelmap = info->pixelmap_id[NEEDLE_PIXELMAP_INDEX];
            slider_info.gx_pixelmap_slider_info_lower_background_pixelmap = info->pixelmap_id[LOWER_PIXELMAP_INDEX];
            gx_pixelmap_slider_pixelmap_set(slider, &slider_info);
        }
        break;

    case NEEDLE_PIXELMAP_INDEX:
        info->pixelmap_id[NEEDLE_PIXELMAP_INDEX] = pixmap_id;

        if (slider)
        {
            slider_info.gx_pixelmap_slider_info_needle_pixelmap = pixmap_id;
            slider_info.gx_pixelmap_slider_info_lower_background_pixelmap = info->pixelmap_id[LOWER_PIXELMAP_INDEX];
            slider_info.gx_pixelmap_slider_info_upper_background_pixelmap = info->pixelmap_id[UPPER_PIXELMAP_INDEX];
            gx_pixelmap_slider_pixelmap_set(slider, &slider_info);
        }
        break;
    }
}

void pixelmap_slider_service_provider::Autosize(widget_info *info)
{
    if (info->style & GX_STYLE_SLIDER_VERTICAL)
    {
        CheckResizeToPixelmap(info, RESIZE_MODE_WIDTH);
    }
    else
    {
        CheckResizeToPixelmap(info, RESIZE_MODE_HEIGHT);
    }
}
