
#include "studiox_includes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

slider_service_provider::slider_service_provider()
{
}

CString slider_service_provider::GetVarDeclaration()
{
    return CString("GX_SLIDER_MEMBERS_DECLARE");
}

CString slider_service_provider::DeclarePropertiesStruct()
{
    CString out(""
    "typedef struct\n"
    "{\n"
    "    int tickmark_count;\n"
    "    int minval;\n"
    "    int maxval;\n"
    "    int current_val;\n"
    "    int increment;\n"
    "    GX_VALUE min_travel;\n"
    "    GX_VALUE max_travel;\n"
    "    GX_VALUE needle_width;\n"
    "    GX_VALUE needle_height;\n"
    "    GX_VALUE needle_inset;\n"
    "    GX_VALUE needle_hotspot;\n"
    "} GX_SLIDER_PROPERTIES;\n\n");
    return out;
}

CString slider_service_provider::WriteExtendedProperties(screen_generator *gen, CString &prefix, widget_info *info)
{
    CString out;
    CString propname = prefix + info->app_name;
    out.Format(_T("GX_SLIDER_PROPERTIES %s_properties =\n")
        _T("{\n")
        _T("    %d, /* tickmark count */\n")
        _T("    %d, /* mimimun value */\n")
        _T("    %d, /* maximum value */\n")
        _T("    %d, /* current value */\n")
        _T("    %d, /* increment */\n")
        _T("    %d, /* minimum travel */\n")
        _T("    %d, /* maximum travel */\n")
        _T("    %d, /* needle width */\n")
        _T("    %d, /* needle height */\n")
        _T("    %d, /* needle inset */\n")
        _T("    %d  /* needle hotspot */\n};\n"),
        propname,
        info ->misc_value,
        info->ewi.slider.gx_slider_info_min_val,
        info->ewi.slider.gx_slider_info_max_val,
        info->ewi.slider.gx_slider_info_current_val,
        info->ewi.slider.gx_slider_info_increment,
        info->ewi.slider.gx_slider_info_min_travel,
        info->ewi.slider.gx_slider_info_max_travel,
        info->ewi.slider.gx_slider_info_needle_width,
        info->ewi.slider.gx_slider_info_needle_height,
        info->ewi.slider.gx_slider_info_needle_inset,
        info->ewi.slider.gx_slider_info_needle_hotspot_offset);
    return out;
}

CString slider_service_provider::GetCreateFromDefFunctionName()
{
    return CString("gx_studio_slider_create");
}

CString slider_service_provider::GetCreateFromDefFunction(int version)
{
    CString out;
    MakeCreatePreamble("slider", version, out);

    out += "{\n"
        "    UINT status;\n"
        "    GX_SLIDER *slider = (GX_SLIDER *) control_block;\n"
        "    GX_SLIDER_PROPERTIES *props = (GX_SLIDER_PROPERTIES *) info->properties;\n"   
        "    GX_SLIDER_INFO slider_info;\n"
        "    slider_info.gx_slider_info_min_val = props->minval;\n"
        "    slider_info.gx_slider_info_max_val = props->maxval;\n"
        "    slider_info.gx_slider_info_current_val = props->current_val;\n"
        "    slider_info.gx_slider_info_increment = props->increment;\n"
        "    slider_info.gx_slider_info_min_travel = props->min_travel;\n"
        "    slider_info.gx_slider_info_max_travel = props->max_travel;\n"
        "    slider_info.gx_slider_info_needle_width = props->needle_width;\n"
        "    slider_info.gx_slider_info_needle_height = props->needle_height;\n"
        "    slider_info.gx_slider_info_needle_inset = props->needle_inset;\n"
        "    slider_info.gx_slider_info_needle_hotspot_offset = props->needle_hotspot;\n"
        "    status = gx_slider_create(slider,\n"
        "                    info->widget_name,\n"
        "                    parent,\n"
        "                    props->tickmark_count,\n"
        "                    &slider_info,\n"
        "                    info->style,\n"
        "                    info->widget_id,\n"
        "                    &info->size);\n"
        "    return status;\n"
        "}\n";
    return out;

}

widget_info *slider_service_provider::CreateNewInstance(GX_WIDGET *parent)
{
    GX_RECTANGLE size;
    gx_utility_rectangle_define(&size, 0, 0, 79, 23);
    gx_utility_rectangle_center(&size, &parent->gx_widget_size);

    GX_SLIDER *slider = new GX_SLIDER;
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

    gx_slider_create(slider,
                    "slider",
                    parent, 10, &slider_info,
                    GX_STYLE_SHOW_NEEDLE|GX_STYLE_ENABLED, 0,
                    &size);

    widget_info *info = InitWidgetInfo((GX_WIDGET *) slider);
    info -> misc_value = 10;   
    info -> ewi.slider = slider_info;
    return info;
}

GX_WIDGET *slider_service_provider::GenerateFromInfo(GX_WIDGET *parent, widget_info *info)
{
    GX_SLIDER *slider = new GX_SLIDER;

    int tick_count = info ->misc_value;
    if (tick_count == 0)
    {
        info ->misc_value = 10;
        tick_count = 10;
    }

    gx_slider_create(slider,
        (CHAR *) info->app_name.GetString(),
        parent, tick_count, &info->ewi.slider,
        info->style,
        0,
        &info->size);

    gx_widget_fill_color_set((GX_WIDGET *) slider,
        info->color_id[NORMAL_FILL_COLOR_INDEX], info->color_id[SELECTED_FILL_COLOR_INDEX], info->color_id[DISABLED_FILL_COLOR_INDEX]);
    return ((GX_WIDGET *) slider);
}
        
void slider_service_provider::SaveToProject(xml_writer &writer, studiox_project *project, int display, widget_info *info)
{
    widget_service_provider::SaveToProject(writer, project, display, info);
    writer.WriteInt("tick_count", info->misc_value);
    writer.WriteInt("minval", info->ewi.slider.gx_slider_info_min_val);
    writer.WriteInt("maxval", info->ewi.slider.gx_slider_info_max_val);
    writer.WriteInt("current_val", info->ewi.slider.gx_slider_info_current_val);
    writer.WriteInt("increment", info->ewi.slider.gx_slider_info_increment);
    writer.WriteInt("min_travel", info->ewi.slider.gx_slider_info_min_travel);
    writer.WriteInt("max_travel", info->ewi.slider.gx_slider_info_max_travel);
    writer.WriteInt("needle_width", info->ewi.slider.gx_slider_info_needle_width);
    writer.WriteInt("needle_height", info->ewi.slider.gx_slider_info_needle_height);
    writer.WriteInt("needle_inset", info->ewi.slider.gx_slider_info_needle_inset);
    writer.WriteInt("needle_hotspot", info->ewi.slider.gx_slider_info_needle_hotspot_offset);
}

void slider_service_provider::ReadFromProject(xml_reader &reader, studiox_project *project, int display, widget_info *info, ULONG valid_styles)
{
    valid_styles |= GX_STYLE_SHOW_NEEDLE|GX_STYLE_SHOW_TICKMARKS|GX_STYLE_SLIDER_VERTICAL;

    widget_service_provider::ReadFromProject(reader, project, display, info, valid_styles);
    if (project->mHeader.project_version <= 54)
    {
        //selected fill color of slider is start been used after project version 5.4.0
        //set selected fill color to normal fill color incase broken old projects
        info->color_id[SELECTED_FILL_COLOR_INDEX] = info->color_id[NORMAL_FILL_COLOR_INDEX];
    }
    reader.ReadInt("tick_count", info->misc_value, 10);
    reader.ReadInt("minval", info->ewi.slider.gx_slider_info_min_val);
    reader.ReadInt("maxval", info->ewi.slider.gx_slider_info_max_val);
    reader.ReadInt("current_val", info->ewi.slider.gx_slider_info_current_val);
    reader.ReadInt("increment", info->ewi.slider.gx_slider_info_increment);
    reader.ReadValue("min_travel", info->ewi.slider.gx_slider_info_min_travel);
    reader.ReadValue("max_travel", info->ewi.slider.gx_slider_info_max_travel);
    
    reader.ReadValue("needle_width", info->ewi.slider.gx_slider_info_needle_width);
    reader.ReadValue("needle_height", info->ewi.slider.gx_slider_info_needle_height);

    reader.ReadValue("needle_inset", info->ewi.slider.gx_slider_info_needle_inset);
    reader.ReadValue("needle_hotspot", info->ewi.slider.gx_slider_info_needle_hotspot_offset);
}
