#include "studiox_includes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


circular_gauge_service_provider::circular_gauge_service_provider()
{
}

CString circular_gauge_service_provider::GetVarDeclaration()
{
    return CString("GX_CIRCULAR_GAUGE_MEMBERS_DECLARE");
}

CString circular_gauge_service_provider::DeclarePropertiesStruct()
{
    CString out(""
        "typedef struct\n"
        "{\n"
        "    GX_VALUE start_angle;\n"
        "    GX_VALUE animation_steps;\n"
        "    GX_VALUE animation_delay;\n"
        "    GX_VALUE needle_xpos;\n"
        "    GX_VALUE needle_ypos;\n"
        "    GX_VALUE needle_xcor;\n"
        "    GX_VALUE needle_ycor;\n"
        "    GX_RESOURCE_ID normal_pixelmap_id;\n"
        "    GX_RESOURCE_ID selected_pixelmap_id;\n"
        "    GX_RESOURCE_ID needle_pixelmap_id;\n"
        "} GX_CIRCULAR_GAUGE_PROPERTIES;\n\n");
    return out;
}

CString circular_gauge_service_provider::WriteExtendedProperties(screen_generator *gen, CString &prefix, widget_info *info)
{
    CString out;
    CString propname = prefix + info->app_name;

    out.Format(_T("GX_CIRCULAR_GAUGE_PROPERTIES %s_properties =\n")
        _T("{\n")
        _T("    %d, /* start angle */\n")
        _T("    %d, /* animation steps */\n")
        _T("    %d, /* animation delay */\n")
        _T("    %d, /* needle xpos */\n")
        _T("    %d, /* needle yos */\n")
        _T("    %d, /* needle xcor */\n")
        _T("    %d, /* needle ycor */\n")
        _T("    %s, /* normal pixelmap id */\n")
        _T("    %s, /* selected pixelmap id */\n")
        _T("    %s  /* needle pixelmap id */\n};\n"),
        propname,
        info->ewi.gauge.start_angle,
        info->ewi.gauge.info.gx_circular_gauge_info_animation_steps,
        info->ewi.gauge.info.gx_circular_gauge_info_animation_delay,
        info->ewi.gauge.info.gx_circular_gauge_info_needle_xpos,
        info->ewi.gauge.info.gx_circular_gauge_info_needle_ypos,
        info->ewi.gauge.info.gx_circular_gauge_info_needle_xcor,
        info->ewi.gauge.info.gx_circular_gauge_info_needle_ycor,
        gen->GetPixelmapIdName(info->pixelmap_id[NORMAL_PIXELMAP_INDEX]),
        gen->GetPixelmapIdName(info->pixelmap_id[SELECTED_PIXELMAP_INDEX]),
        gen->GetPixelmapIdName(info->pixelmap_id[NEEDLE_PIXELMAP_INDEX]));
    return out;
}

CString circular_gauge_service_provider::GetCreateFromDefFunctionName()
{
    return CString("gx_studio_circular_gauge_create");
}

CString circular_gauge_service_provider::GetCreateFromDefFunction(int version)
{
    CString out;
    MakeCreatePreamble("circular_gauge", version, out);

    out += "{\n"
        "    UINT status;\n"
        "    GX_CIRCULAR_GAUGE *gauge = (GX_CIRCULAR_GAUGE *) control_block;\n"
        "    GX_CIRCULAR_GAUGE_PROPERTIES *props = (GX_CIRCULAR_GAUGE_PROPERTIES *) info->properties;\n"
        "    GX_CIRCULAR_GAUGE_INFO gauge_info;\n"
        "    gauge_info.gx_circular_gauge_info_needle_pixelmap = props->needle_pixelmap_id;\n"
        "    gauge_info.gx_circular_gauge_info_needle_xpos = props->needle_xpos;\n"
        "    gauge_info.gx_circular_gauge_info_needle_ypos = props->needle_ypos;\n"
        "    gauge_info.gx_circular_gauge_info_needle_xcor = props->needle_xcor;\n"
        "    gauge_info.gx_circular_gauge_info_needle_ycor = props->needle_ycor;\n"
        "    gauge_info.gx_circular_gauge_info_animation_steps = props->animation_steps;\n"
        "    gauge_info.gx_circular_gauge_info_animation_delay = props->animation_delay;\n"
        "    gauge->gx_icon_normal_pixelmap = props->normal_pixelmap_id;\n"
        "    gauge->gx_icon_selected_pixelmap = props->selected_pixelmap_id;\n"
        "    status = gx_circular_gauge_create(gauge,\n"
        "                    info->widget_name,\n"
        "                    parent,\n"
        "                    &gauge_info,\n"
        "                    props->normal_pixelmap_id,\n"
        "                    info->style,\n"
        "                    info->widget_id,\n"
        "                    info->size.gx_rectangle_left,\n"
        "                    info->size.gx_rectangle_top);\n"
        "    return status;\n"
        "}\n";
    return out;
}

widget_info *circular_gauge_service_provider::CreateNewInstance(GX_WIDGET *parent)
{
    GX_RECTANGLE size;
    gx_utility_rectangle_define(&size, 0, 0, 100, 79);
    gx_utility_rectangle_center(&size, &parent->gx_widget_size);

    GX_CIRCULAR_GAUGE *gauge = new GX_CIRCULAR_GAUGE;
    GX_CIRCULAR_GAUGE_INFO gauge_info;

    gauge->gx_circular_gauge_start_angle = 0;
    gauge_info.gx_circular_gauge_info_needle_pixelmap = 0;
    gauge_info.gx_circular_gauge_info_needle_xpos = 20;
    gauge_info.gx_circular_gauge_info_needle_ypos = 20;
    gauge_info.gx_circular_gauge_info_needle_xcor = 10;
    gauge_info.gx_circular_gauge_info_needle_ycor = 10;
    gauge_info.gx_circular_gauge_info_animation_delay = 1;
    gauge_info.gx_circular_gauge_info_animation_steps = 30;

    gx_circular_gauge_create(gauge,
        "gauge",
        parent, &gauge_info, 0, 
        GX_STYLE_ENABLED, 0,
        size.gx_rectangle_left, size.gx_rectangle_top);

    // the gauge will be 1x1 pixels since it has no pixelmap, so make it larger
    // so that you can actually see it.
    gx_widget_resize((GX_WIDGET *)gauge, &size);

    widget_info *info = InitWidgetInfo((GX_WIDGET *)gauge);
    info->ewi.gauge.info = gauge_info;
    return info;
}

GX_WIDGET *circular_gauge_service_provider::GenerateFromInfo(GX_WIDGET *parent, widget_info *info)
{
    GX_CIRCULAR_GAUGE *gauge = new GX_CIRCULAR_GAUGE;

    gx_circular_gauge_create(gauge,
        (CHAR *)info->app_name.GetString(),
        parent, &info->ewi.gauge.info, info->pixelmap_id[NORMAL_PIXELMAP_INDEX],
        info->style,
        0,
        info->size.gx_rectangle_left, info->size.gx_rectangle_top);

    gx_widget_resize((GX_WIDGET*) gauge, &info->size);

    gx_widget_fill_color_set((GX_WIDGET *)gauge,
        info->color_id[NORMAL_FILL_COLOR_INDEX],
        info->color_id[SELECTED_FILL_COLOR_INDEX],
        info->color_id[DISABLED_FILL_COLOR_INDEX]);

    return ((GX_WIDGET *) gauge);
}

void circular_gauge_service_provider::SaveToProject(xml_writer &writer, studiox_project *project, int display, widget_info *info)
{

    widget_service_provider::SaveToProject(writer, project, display, info);

    writer.WriteInt("start_angle", info->ewi.gauge.start_angle);
    writer.WriteInt("animation_steps", info->ewi.gauge.info.gx_circular_gauge_info_animation_steps);
    writer.WriteInt("animation_delay", info->ewi.gauge.info.gx_circular_gauge_info_animation_delay);
    writer.WriteInt("xpos", info->ewi.gauge.info.gx_circular_gauge_info_needle_xpos);
    writer.WriteInt("ypos", info->ewi.gauge.info.gx_circular_gauge_info_needle_ypos);
    writer.WriteInt("xcor", info->ewi.gauge.info.gx_circular_gauge_info_needle_xcor);
    writer.WriteInt("ycor", info->ewi.gauge.info.gx_circular_gauge_info_needle_ycor);

    WritePixelmapId(writer, project, display, "normal_pixlmap", info->pixelmap_id[NORMAL_PIXELMAP_INDEX]);
    WritePixelmapId(writer, project, display, "selected_pixlmap", info->pixelmap_id[SELECTED_PIXELMAP_INDEX]);
    WritePixelmapId(writer, project, display, "needle_pixlmap", info->pixelmap_id[NEEDLE_PIXELMAP_INDEX]);
}

void circular_gauge_service_provider::ReadFromProject(xml_reader &reader, studiox_project *project, int display, widget_info *info, ULONG valid_styles)
{
    valid_styles |= GX_STYLE_CIRCULAR_GAUGE_CCW;
    widget_service_provider::ReadFromProject(reader, project, display, info, valid_styles);

    reader.ReadInt("start_angle", info->ewi.gauge.start_angle);
    reader.ReadInt("animation_steps", info->ewi.gauge.info.gx_circular_gauge_info_animation_steps);
    reader.ReadInt("animation_delay", info->ewi.gauge.info.gx_circular_gauge_info_animation_delay);
    reader.ReadValue("xpos", info->ewi.gauge.info.gx_circular_gauge_info_needle_xpos);
    reader.ReadValue("ypos", info->ewi.gauge.info.gx_circular_gauge_info_needle_ypos);
    reader.ReadValue("xcor", info->ewi.gauge.info.gx_circular_gauge_info_needle_xcor);
    reader.ReadValue("ycor", info->ewi.gauge.info.gx_circular_gauge_info_needle_ycor);

    info->pixelmap_id[NORMAL_PIXELMAP_INDEX] = ReadPixelmapId(reader, project, display, "normal_pixlmap");
    info->pixelmap_id[SELECTED_PIXELMAP_INDEX] = ReadPixelmapId(reader, project, display, "selected_pixlmap");
    info->pixelmap_id[NEEDLE_PIXELMAP_INDEX] = ReadPixelmapId(reader, project, display, "needle_pixlmap");

    info->ewi.gauge.info.gx_circular_gauge_info_needle_pixelmap = info->pixelmap_id[NEEDLE_PIXELMAP_INDEX];
}

void circular_gauge_service_provider::AssignPixelmap(widget_info *info, int index, GX_RESOURCE_ID pixmap_id)
{
    GX_CIRCULAR_GAUGE_INFO gauge_info = info->ewi.gauge.info;
    GX_CIRCULAR_GAUGE *gauge = (GX_CIRCULAR_GAUGE *) (info->widget);
    GX_RECTANGLE newsize;

    switch (index)
    {
    case NORMAL_PIXELMAP_INDEX:
        info->pixelmap_id[NORMAL_PIXELMAP_INDEX] = pixmap_id;

        if (gauge)
        {
            gauge->gx_icon_normal_pixelmap = pixmap_id;
            gauge->gx_icon_selected_pixelmap = info->pixelmap_id[SELECTED_PIXELMAP_INDEX];
            gauge_info.gx_circular_gauge_info_needle_pixelmap = info->pixelmap_id[NEEDLE_PIXELMAP_INDEX];
            gauge->gx_circular_gauge_info = gauge_info;
            gx_icon_pixelmap_set((GX_ICON *)gauge, pixmap_id, info->pixelmap_id[SELECTED_PIXELMAP_INDEX]);
        }
        break;

    case SELECTED_PIXELMAP_INDEX:
        info->pixelmap_id[SELECTED_PIXELMAP_INDEX] = pixmap_id;

        if (gauge)
        {
            gauge->gx_icon_normal_pixelmap = info->pixelmap_id[NORMAL_PIXELMAP_INDEX];
            gauge->gx_icon_selected_pixelmap = pixmap_id;
            gauge_info.gx_circular_gauge_info_needle_pixelmap = info->pixelmap_id[NEEDLE_PIXELMAP_INDEX];
            gauge->gx_circular_gauge_info = gauge_info;
            gx_icon_pixelmap_set((GX_ICON *)gauge, info->pixelmap_id[NORMAL_PIXELMAP_INDEX], pixmap_id);
        }
        break;

    case NEEDLE_PIXELMAP_INDEX:
        info->pixelmap_id[NEEDLE_PIXELMAP_INDEX] = pixmap_id;
        info->ewi.gauge.info.gx_circular_gauge_info_needle_pixelmap = pixmap_id;

        if (gauge)
        {
            gauge->gx_icon_normal_pixelmap = info->pixelmap_id[NORMAL_PIXELMAP_INDEX];
            gauge->gx_icon_selected_pixelmap = info->pixelmap_id[SELECTED_PIXELMAP_INDEX];
            gauge_info.gx_circular_gauge_info_needle_pixelmap = pixmap_id;
            gauge->gx_circular_gauge_info = gauge_info;
            gx_widget_pixelmap_get((GX_WIDGET *)gauge,
                gauge->gx_circular_gauge_info.gx_circular_gauge_info_needle_pixelmap,
                &gauge->gx_circular_gauge_needle_source);
            gx_system_dirty_mark((GX_WIDGET *)gauge);
        }
        break;
    }

    // check to see if the gauge auto-sized to new pixelmap, if so
    // notify the properties window:

    if (gauge)
    {
        newsize = gauge->gx_widget_size;

        if (!gx_utility_rectangle_compare(&newsize, &info->size))
        {
            info->size = newsize;
            GetPropsWin()->WidgetWasMoved();
            GetTargetScreen()->WidgetWasMoved(info);
        }
    }
}

void circular_gauge_service_provider::Autosize(widget_info *info)
{
    CheckResizeToPixelmap(info, RESIZE_MODE_HEIGHT, TRUE);
}