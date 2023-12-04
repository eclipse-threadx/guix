
#include "studiox_includes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

radial_slider_service_provider::radial_slider_service_provider()
{
}

CString radial_slider_service_provider::GetVarDeclaration()
{
    return CString("GX_RADIAL_SLIDER_MEMBERS_DECLARE");
}

CString radial_slider_service_provider::DeclarePropertiesStruct()
{
    CString out(""
        "typedef struct\n"
        "{\n"
        "    GX_VALUE       xcenter;\n"
        "    GX_VALUE       ycenter;\n"
        "    USHORT         radius;\n"
        "    USHORT         track_width;\n");

    if (project_lib_version() >= GX_VERSION_RADIAL_SLIDER_NEEDLE_OFFSET_CHANGE)
    {
        out += "    GX_VALUE       needle_offset;\n";
    }

    out+=
        "    GX_VALUE       current_angle;\n"
        "    GX_VALUE       min_angle;\n"
        "    GX_VALUE       max_angle;\n"
        "    GX_RESOURCE_ID background_pixelmap;\n"
        "    GX_RESOURCE_ID needle_pixelmap;\n"
        "    USHORT         animation_total_steps;\n"
        "    USHORT         animation_delay;\n"
        "    USHORT         animation_style;\n"
        "    VOID         (*animation_update_callback)(struct GX_RADIAL_SLIDER_STRUCT *slider);\n"
        "} GX_RADIAL_SLIDER_PROPERTIES;\n\n";
    return out;
}

CString radial_slider_service_provider::WriteExtendedProperties(screen_generator *gen, CString &prefix, widget_info *info)
{
    CString out;
    CString propname = prefix + info->app_name;
    CString callback;
    CString animation_style;

    animation_style = easing_function_select_dlg::GetEasingFuncIdName(info->ewi.radial_slider.animation_style);

    if (animation_style.IsEmpty())
    {
        animation_style = _T("0");
    }
    else
    {
        animation_style.MakeUpper();
        animation_style.Replace(' ', '_');
        animation_style.Insert(0, _T("GX_ANIMATION_"));
    }

    if (info->callback_func.IsEmpty())
    {
        callback = "GX_NULL";
    }
    else
    {
        callback = info->callback_func;
    }

    CString needle_offset("");

    if (project_lib_version() >= GX_VERSION_RADIAL_SLIDER_NEEDLE_OFFSET_CHANGE)
    {
        needle_offset.Format(_T("    %d, /* needle offset */\n"), info->ewi.radial_slider.needle_offset);
    }

    out.Format(_T("GX_RADIAL_SLIDER_PROPERTIES %s_properties =\n")
        _T("{\n")
        _T("    %d, /* xcenter */\n")
        _T("    %d, /* ycenter */\n")
        _T("    %d, /* radius */\n")
        _T("    %d, /* track width*/\n")
        _T("%s")
        _T("    %d, /* current angle */\n")
        _T("    %d, /* min angle */\n")
        _T("    %d, /* max angle */\n")
        _T("    %s, /* background pixelmap */\n")
        _T("    %s, /* needle pixelmap */\n")
        _T("    %d, /* animation total steps */\n")
        _T("    %d, /* animation delay */\n")
        _T("    %s, /* animation style */\n")
        _T("    %s, /* animation update callback func */\n")
        _T("};\n"),
        propname,
        info->ewi.radial_slider.xcenter,
        info->ewi.radial_slider.ycenter,
        info->ewi.radial_slider.radius,
        info->ewi.radial_slider.track_width,
        needle_offset,
        info->ewi.radial_slider.current_angle,
        info->ewi.radial_slider.min_angle,
        info->ewi.radial_slider.max_angle,
        gen->GetPixelmapIdName(info->pixelmap_id[NORMAL_PIXELMAP_INDEX]),
        gen->GetPixelmapIdName(info->pixelmap_id[NEEDLE_PIXELMAP_INDEX]),
        info->ewi.radial_slider.animation_total_steps,
        info->ewi.radial_slider.animation_delay,
        animation_style,
        callback);

    return out;
}

CString radial_slider_service_provider::GetCreateFromDefFunctionName()
{
    return CString("gx_studio_radial_slider_create");
}

CString radial_slider_service_provider::GetCreateFromDefFunction(int version)
{
    CString out;
    MakeCreatePreamble("radial_slider", version, out);

    out += "{\n"
        "    UINT status;\n"
        "    GX_RADIAL_SLIDER *slider = (GX_RADIAL_SLIDER *) control_block;\n"
        "    GX_RADIAL_SLIDER_INFO slider_info;\n"
        "    GX_RADIAL_SLIDER_PROPERTIES *props = (GX_RADIAL_SLIDER_PROPERTIES *)info->properties;\n"
        "    memset(&slider_info, 0, sizeof(GX_RADIAL_SLIDER_INFO));\n"
        "    slider_info.gx_radial_slider_info_xcenter = props->xcenter;\n"
        "    slider_info.gx_radial_slider_info_ycenter = props->ycenter;\n"
        "    slider_info.gx_radial_slider_info_radius = props->radius;\n"
        "    slider_info.gx_radial_slider_info_track_width = props->track_width;\n";

    if (project_lib_version() >= GX_VERSION_RADIAL_SLIDER_NEEDLE_OFFSET_CHANGE)
    {
        out += "    slider_info.gx_radial_slider_info_needle_offset = props->needle_offset;\n";
    }

    out +=
        "    slider_info.gx_radial_slider_info_current_angle = props->current_angle;\n"
        "    slider_info.gx_radial_slider_info_min_angle = props->min_angle;\n"
        "    slider_info.gx_radial_slider_info_max_angle = props->max_angle;\n"
        "    slider_info.gx_radial_slider_info_background_pixelmap = props->background_pixelmap;\n"
        "    slider_info.gx_radial_slider_info_needle_pixelmap = props->needle_pixelmap;\n"
        "    status = gx_radial_slider_create(slider,\n"
        "                    info->widget_name,\n"
        "                    parent,\n"
        "                    &slider_info,\n"
        "                    info->style,\n"
        "                    info->widget_id,\n"
        "                    &info->size);\n"
        "    gx_radial_slider_animation_set(slider, props->animation_total_steps, props->animation_delay, props->animation_style, props->animation_update_callback);\n"
        "    return status;\n"
        "}\n";
    return out;

}

widget_info *radial_slider_service_provider::CreateNewInstance(GX_WIDGET *parent)
{
    GX_RADIAL_SLIDER_INFO props;
    int xcenter;
    int ycenter;
    GX_RECTANGLE size;

    xcenter = (parent->gx_widget_size.gx_rectangle_left + parent->gx_widget_size.gx_rectangle_right) >> 1;
    ycenter = (parent->gx_widget_size.gx_rectangle_top + parent->gx_widget_size.gx_rectangle_bottom) >> 1;

    GX_RADIAL_SLIDER *slider = new GX_RADIAL_SLIDER;
    memset(slider, 0, sizeof(GX_RADIAL_SLIDER));
    props.gx_radial_slider_info_xcenter = 100;
    props.gx_radial_slider_info_ycenter = 100;
    props.gx_radial_slider_info_radius = 100;
    props.gx_radial_slider_info_track_width = 20;
    props.gx_radial_slider_info_needle_offset = 0;
    props.gx_radial_slider_info_current_angle = -90;
    props.gx_radial_slider_info_min_angle = -90;
    props.gx_radial_slider_info_max_angle = 270;
    props.gx_radial_slider_info_needle_pixelmap = GX_NULL;
    props.gx_radial_slider_info_background_pixelmap = GX_NULL;
    props.gx_radial_slider_info_angle_list = GX_NULL;

    size.gx_rectangle_left = xcenter - 100;
    size.gx_rectangle_top = ycenter - 100;
    size.gx_rectangle_right = xcenter + 100;
    size.gx_rectangle_bottom = ycenter + 100;

    gx_radial_slider_create(slider,
        "radial_slider",
        parent, &props,
        GX_STYLE_ENABLED | GX_STYLE_BORDER_NONE, 0, &size);

    widget_info *info = InitWidgetInfo((GX_WIDGET *)slider);

    info->ewi.radial_slider.xcenter = props.gx_radial_slider_info_xcenter;
    info->ewi.radial_slider.ycenter = props.gx_radial_slider_info_ycenter;
    info->ewi.radial_slider.radius = props.gx_radial_slider_info_radius;
    info->ewi.radial_slider.track_width = props.gx_radial_slider_info_track_width;
    info->ewi.radial_slider.needle_offset = props.gx_radial_slider_info_needle_offset;
    info->ewi.radial_slider.current_angle = props.gx_radial_slider_info_current_angle;
    info->ewi.radial_slider.min_angle = props.gx_radial_slider_info_min_angle;
    info->ewi.radial_slider.max_angle = props.gx_radial_slider_info_max_angle;
    info->ewi.radial_slider.needle_pixelmap = props.gx_radial_slider_info_needle_pixelmap;
    info->ewi.radial_slider.background_pixelmap = props.gx_radial_slider_info_background_pixelmap;
    info->ewi.radial_slider.animation_total_steps = slider->gx_radial_slider_animation_total_steps;
    info->ewi.radial_slider.animation_delay = slider->gx_radial_slider_animation_delay;
    info->ewi.radial_slider.animation_style = GX_ANIMATION_CIRC_EASE_IN_OUT;
    return info;
}

GX_WIDGET *radial_slider_service_provider::GenerateFromInfo(GX_WIDGET *parent, widget_info *info)
{
    GX_RADIAL_SLIDER *slider = new GX_RADIAL_SLIDER;
    GX_RADIAL_SLIDER_INFO props;

    memset(&props, 0, sizeof(GX_RADIAL_SLIDER_INFO));
    props.gx_radial_slider_info_xcenter = info->ewi.radial_slider.xcenter;
    props.gx_radial_slider_info_ycenter = info->ewi.radial_slider.ycenter;
    props.gx_radial_slider_info_radius = info->ewi.radial_slider.radius;
    props.gx_radial_slider_info_track_width = info->ewi.radial_slider.track_width;
    props.gx_radial_slider_info_needle_offset = info->ewi.radial_slider.needle_offset;
    props.gx_radial_slider_info_current_angle = info->ewi.radial_slider.current_angle;
    props.gx_radial_slider_info_min_angle = info->ewi.radial_slider.min_angle;
    props.gx_radial_slider_info_max_angle = info->ewi.radial_slider.max_angle;
    props.gx_radial_slider_info_needle_pixelmap = info->ewi.radial_slider.needle_pixelmap;
    props.gx_radial_slider_info_background_pixelmap = info->ewi.radial_slider.background_pixelmap;

    gx_radial_slider_create(slider,
        (CHAR *)info->app_name.GetString(),
        parent, &props,
        info->style,
        0, &info->size);

    gx_radial_slider_animation_set(slider, info->ewi.radial_slider.animation_total_steps, info->ewi.radial_slider.animation_delay,
        info->ewi.radial_slider.animation_style, GX_NULL);

    gx_widget_fill_color_set((GX_WIDGET *)slider,
        info->color_id[NORMAL_FILL_COLOR_INDEX],
        info->color_id[SELECTED_FILL_COLOR_INDEX],
        info->color_id[DISABLED_FILL_COLOR_INDEX]);

    return ((GX_WIDGET *)slider);
}

void radial_slider_service_provider::SaveToProject(xml_writer &writer, studiox_project *project, int display, widget_info *info)
{
    widget_service_provider::SaveToProject(writer, project, display, info);
    writer.WriteInt("xcenter", info->ewi.radial_slider.xcenter);
    writer.WriteInt("ycenter", info->ewi.radial_slider.ycenter);
    writer.WriteInt("radius", info->ewi.radial_slider.radius);
    writer.WriteInt("track_width", info->ewi.radial_slider.track_width);
    writer.WriteInt("needle_offset", info->ewi.radial_slider.needle_offset);
    writer.WriteInt("current_angle", info->ewi.radial_slider.current_angle);
    writer.WriteInt("min_angle", info->ewi.radial_slider.min_angle);
    writer.WriteInt("max_angle", info->ewi.radial_slider.max_angle);
    WritePixelmapId(writer, project, display, "background_pixelmap", info->pixelmap_id[NORMAL_PIXELMAP_INDEX]);
    WritePixelmapId(writer, project, display, "needle_pixelmap", info->pixelmap_id[NEEDLE_PIXELMAP_INDEX]);
    writer.WriteInt("animation_total_steps", info->ewi.radial_slider.animation_total_steps);
    writer.WriteInt("animation_delay", info->ewi.radial_slider.animation_delay);
    writer.WriteString("animation_style", easing_function_select_dlg::GetEasingFuncIdName(info->ewi.radial_slider.animation_style));
    writer.WriteString("callback", info->callback_func);
}

void radial_slider_service_provider::ReadFromProject(xml_reader &reader, studiox_project *project, int display, widget_info *info, ULONG valid_styles)
{
    CString easing_func;

    widget_service_provider::ReadFromProject(reader, project, display, info, valid_styles);
    reader.ReadValue("xcenter", info->ewi.radial_slider.xcenter);
    reader.ReadValue("ycenter", info->ewi.radial_slider.ycenter);
    reader.ReadUShort("radius", info->ewi.radial_slider.radius);
    reader.ReadUShort("track_width", info->ewi.radial_slider.track_width);
    reader.ReadValue("needle_offset", info->ewi.radial_slider.needle_offset);
    reader.ReadValue("current_angle", info->ewi.radial_slider.current_angle);
    reader.ReadValue("min_angle", info->ewi.radial_slider.min_angle);
    reader.ReadValue("max_angle", info->ewi.radial_slider.max_angle);
    info->pixelmap_id[NORMAL_PIXELMAP_INDEX] = ReadPixelmapId(reader, project, display, "background_pixelmap");
    info->pixelmap_id[NEEDLE_PIXELMAP_INDEX] = ReadPixelmapId(reader, project, display, "needle_pixelmap");
    info->ewi.radial_slider.background_pixelmap = info->pixelmap_id[NORMAL_PIXELMAP_INDEX];
    info->ewi.radial_slider.needle_pixelmap = info->pixelmap_id[NEEDLE_PIXELMAP_INDEX];
    reader.ReadUShort("animation_total_steps", info->ewi.radial_slider.animation_total_steps);
    reader.ReadUShort("animation_delay", info->ewi.radial_slider.animation_delay);
    reader.ReadString("animation_style", easing_func);
    info->ewi.radial_slider.animation_style = easing_function_select_dlg::GetEasingFuncId(easing_func);
    reader.ReadString("callback", info->callback_func);
}

void radial_slider_service_provider::AssignPixelmap(widget_info *info, int index, GX_RESOURCE_ID pixmap_id)
{
    GX_RADIAL_SLIDER *slider = (GX_RADIAL_SLIDER *) (info->widget);

    switch (index)
    {
    case NORMAL_PIXELMAP_INDEX:
        info->pixelmap_id[NORMAL_PIXELMAP_INDEX] = pixmap_id;
        info->ewi.radial_slider.background_pixelmap = pixmap_id;

        if (slider)
        {
            slider->gx_radial_slider_info.gx_radial_slider_info_background_pixelmap = pixmap_id;
            gx_system_dirty_mark((GX_WIDGET *)slider);
        }
        break;

    case NEEDLE_PIXELMAP_INDEX:
        info->pixelmap_id[NEEDLE_PIXELMAP_INDEX] = pixmap_id;
        info->ewi.radial_slider.needle_pixelmap = pixmap_id;

        if (slider)
        {
            slider->gx_radial_slider_info.gx_radial_slider_info_needle_pixelmap = pixmap_id;
            gx_system_dirty_mark((GX_WIDGET *)slider);
        }
        break;

        break;
    }
}

void radial_slider_service_provider::Autosize(widget_info *info)
{
    CheckResizeToPixelmap(info, RESIZE_MODE_ALL, TRUE);
}

void radial_slider_service_provider::SetRadialSliderInfo(widget_info *info, radial_slider_info *new_info)
{
    GX_RADIAL_SLIDER *slider = (GX_RADIAL_SLIDER *)info->widget;
    GX_RADIAL_SLIDER_INFO *slider_info;

    info->ewi.radial_slider = *new_info;

    gx_radial_slider_info_get(slider, &slider_info);
    slider_info->gx_radial_slider_info_xcenter = new_info->xcenter;
    slider_info->gx_radial_slider_info_ycenter = new_info->ycenter;
    slider_info->gx_radial_slider_info_radius = new_info->radius;
    slider_info->gx_radial_slider_info_track_width = new_info->track_width;
    slider_info->gx_radial_slider_info_needle_offset = new_info->needle_offset;
    slider_info->gx_radial_slider_info_current_angle = new_info->current_angle;
    slider_info->gx_radial_slider_info_min_angle = new_info->min_angle;
    slider_info->gx_radial_slider_info_max_angle = new_info->max_angle;

    gx_radial_slider_info_set(slider, slider_info);
    gx_radial_slider_animation_set(slider, new_info->animation_total_steps, new_info->animation_delay, new_info->animation_style, NULL);
}