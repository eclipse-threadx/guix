
#include "studiox_includes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

radio_button_service_provider::radio_button_service_provider()
{
}

CString radio_button_service_provider::GetVarDeclaration()
{
    return CString("GX_RADIO_BUTTON_MEMBERS_DECLARE");
}

CString radio_button_service_provider::DeclarePropertiesStruct()
{
    CString out;

    out.Format(_T("typedef struct\n")
        _T("{\n")
        _T("%s\n")
        _T("    GX_RESOURCE_ID off_pixelmap_id;\n")
        _T("    GX_RESOURCE_ID on_pixelmap_id;\n")
        _T("    GX_RESOURCE_ID off_disabled_pixelmap_id;\n")
        _T("    GX_RESOURCE_ID on_disabled_pixelmap_id;\n")
        _T("} GX_RADIO_BUTTON_PROPERTIES;\n\n"),
        text_button_service_provider::DeclaraPropertiesStructContent());

    return out;
}

CString radio_button_service_provider::WriteExtendedProperties(screen_generator *gen, CString &prefix, widget_info *info)
{
    GX_RESOURCE_ID radio_on_id;
    GX_RESOURCE_ID radio_off_id;

    radio_on_id = info->pixelmap_id[RADIO_ON_PIXELMAP_INDEX];
    if (radio_on_id == GX_PIXELMAP_RADIO_ON_ID)
    {
        // if the id is the default, set it to 0 to avoid 
        // generating pixelmap set call
        radio_on_id = 0;
    }

    radio_off_id = info->pixelmap_id[RADIO_OFF_PIXELMAP_INDEX];
    if (radio_off_id == GX_PIXELMAP_RADIO_OFF_ID)
    {
        // if the id is the default, set it to 0 to avoid 
        // generating pixelmap set call
        radio_off_id = 0;
    }

    CString out;
    CString propname = prefix + info->app_name;
    out.Format(_T("GX_RADIO_BUTTON_PROPERTIES %s_properties =\n")
        _T("{\n")
        _T("%s\n")
        _T("    %s, /* off pixelmap id */\n")
        _T("    %s, /* on pixelmap id */\n")
        _T("    %s, /* off disabled pixelmap id */\n")
        _T("    %s  /* on disabled pixelmap id */\n};\n"),
        propname,
        text_button_service_provider::WriteExtendedPropertiesContent(gen, info, TRUE),
        gen->GetPixelmapIdName(radio_off_id),
        gen->GetPixelmapIdName(radio_on_id),
        gen->GetPixelmapIdName(info->pixelmap_id[RADIO_OFF_DISABLED_PIXELMAP_INDEX]),
        gen->GetPixelmapIdName(info->pixelmap_id[RADIO_ON_DISABLED_PIXELMAP_INDEX])
        );
    return out;
}

CString radio_button_service_provider::GetCreateFromDefFunctionName()
{
    return CString("gx_studio_radio_button_create");
}

CString radio_button_service_provider::GetCreateFromDefFunction(int version)
{
    CString out;
    MakeCreatePreamble("radio_button", version, out);

    out += "{\n"
        "    UINT status;\n"
        "    GX_RADIO_BUTTON *button = (GX_RADIO_BUTTON *) control_block;\n"
        "    GX_TEXT_BUTTON *text_button = (GX_TEXT_BUTTON *) button;\n"
        "    GX_RADIO_BUTTON_PROPERTIES *props = (GX_RADIO_BUTTON_PROPERTIES *) info->properties;\n"
        "    status = gx_radio_button_create(button, info->widget_name, parent, props->string_id, info->style, info->widget_id, &info->size);\n"
        "    if (status == GX_SUCCESS)\n"
        "    {\n"
        "        gx_text_button_font_set(text_button, props->font_id);\n";

        if (version > 50401)
        {
            out += "#if defined(GUIX_5_4_0_COMPATIBILITY)\n"
                "        gx_text_button_text_color_set(text_button, props->normal_text_color_id, props->selected_text_color_id);\n"
                "#else\n"
                "        gx_text_button_text_color_set(text_button, props->normal_text_color_id, props->selected_text_color_id, props->disabled_text_color_id);\n"
                "#endif\n";
        }
        else
        {
            out += "        gx_text_button_text_color_set(text_button, props->normal_text_color_id, props->selected_text_color_id);\n";
        }

        out += "\n"
        "        if (props->off_pixelmap_id ||\n"
        "            props->on_pixelmap_id ||\n"
        "            props->off_disabled_pixelmap_id ||\n"
        "            props->on_disabled_pixelmap_id)\n"
        "        {\n"
        "            gx_radio_button_pixelmap_set(button,\n"
        "                                     props->off_pixelmap_id,\n"
        "                                     props->on_pixelmap_id,\n"
        "                                     props->off_disabled_pixelmap_id,\n"
        "                                     props->on_disabled_pixelmap_id);\n"
        "        }\n"
        "    }\n"
        "    return status;\n"
        "}\n";

    return out;
}

widget_info *radio_button_service_provider::CreateNewInstance(GX_WIDGET *parent)
{
    GX_RECTANGLE size;
    gx_utility_rectangle_define(&size, 0, 0, 79, 23);
    gx_utility_rectangle_center(&size, &parent->gx_widget_size);
    
    GX_RADIO_BUTTON *button = new GX_RADIO_BUTTON;
    GX_RESOURCE_ID string_id = GetStringId("radio");
    gx_radio_button_create(button, "radio_button", parent,
        string_id, GX_STYLE_ENABLED|GX_STYLE_TRANSPARENT|GX_STYLE_TEXT_LEFT|GX_STYLE_BUTTON_RADIO, 0, &size);

    widget_info *info = InitWidgetInfo((GX_WIDGET *) button);

    info->string_id[0]  = string_id;

    info->color_id[NORMAL_TEXT_COLOR_INDEX] = button->gx_text_button_normal_text_color;
    info->color_id[SELECTED_TEXT_COLOR_INDEX] = button->gx_text_button_selected_text_color;
    info->color_id[DISABLED_TEXT_COLOR_INDEX] = button->gx_text_button_disabled_text_color;
    info->font_id[0] = button->gx_text_button_font_id;

    info->pixelmap_id[RADIO_ON_PIXELMAP_INDEX] = GX_PIXELMAP_RADIO_ON_ID;
    info->pixelmap_id[RADIO_OFF_PIXELMAP_INDEX] = GX_PIXELMAP_RADIO_OFF_ID;

    return info;
}

GX_WIDGET *radio_button_service_provider::GenerateFromInfo(GX_WIDGET *parent, widget_info *info)
{
    GX_RADIO_BUTTON *button = new GX_RADIO_BUTTON;
    gx_radio_button_create(button,
        (CHAR *) info->app_name.GetString(),
        parent,
        info->string_id[0],
        info->style,
        0,
        &info->size);

    if(info->pixelmap_id[RADIO_OFF_PIXELMAP_INDEX] == 0)
    {
        info->pixelmap_id[RADIO_OFF_PIXELMAP_INDEX] = GX_PIXELMAP_RADIO_OFF_ID;
    }
    if (info->pixelmap_id[RADIO_ON_PIXELMAP_INDEX] == 0)
    {
        info->pixelmap_id[RADIO_ON_PIXELMAP_INDEX] = GX_PIXELMAP_RADIO_ON_ID;
    }

    gx_radio_button_pixelmap_set(button,
        info->pixelmap_id[RADIO_OFF_PIXELMAP_INDEX],
        info->pixelmap_id[RADIO_ON_PIXELMAP_INDEX],
        info->pixelmap_id[RADIO_OFF_DISABLED_PIXELMAP_INDEX],
        info->pixelmap_id[RADIO_ON_DISABLED_PIXELMAP_INDEX]);

    GX_TEXT_BUTTON *tb = (GX_TEXT_BUTTON *) button;

    gx_widget_fill_color_set(tb,
        info->color_id[NORMAL_FILL_COLOR_INDEX],
        info->color_id[SELECTED_FILL_COLOR_INDEX],
        info->color_id[DISABLED_FILL_COLOR_INDEX]);

    gx_text_button_text_color_set(tb,
        info->color_id[NORMAL_TEXT_COLOR_INDEX],
        info->color_id[SELECTED_TEXT_COLOR_INDEX],
        info->color_id[DISABLED_TEXT_COLOR_INDEX]);

    gx_text_button_font_set(tb, info->font_id[0]);
    return ((GX_WIDGET *) button);
}
        
void radio_button_service_provider::SaveToProject(xml_writer &writer, studiox_project *project, int display, widget_info *info)
{
    text_button_service_provider::SaveToProject(writer, project, display, info);
    WritePixelmapId(writer, project, display, "off_pixelmap_id", info->pixelmap_id[UNCHECKED_PIXELMAP_INDEX]);
    WritePixelmapId(writer, project, display, "on_pixelmap_id", info->pixelmap_id[CHECKED_PIXELMAP_INDEX]);
    WritePixelmapId(writer, project, display, "off_disabled_pixelmap_id", info->pixelmap_id[UNCHECKED_DISABLED_PIXELMAP_INDEX]);
    WritePixelmapId(writer, project, display, "on_disabled_pixelmap_id", info->pixelmap_id[CHECKED_DISABLED_PIXELMAP_INDEX]);
}

void radio_button_service_provider::ReadFromProject(xml_reader &reader, studiox_project *project, int display, widget_info *info, ULONG valid_styles)
{
    text_button_service_provider::ReadFromProject(reader, project, display, info, valid_styles);
    info->pixelmap_id[UNCHECKED_PIXELMAP_INDEX] = ReadPixelmapId(reader, project, display, "off_pixelmap_id");
    info->pixelmap_id[CHECKED_PIXELMAP_INDEX] = ReadPixelmapId(reader, project, display, "on_pixelmap_id");
    info->pixelmap_id[UNCHECKED_DISABLED_PIXELMAP_INDEX] = ReadPixelmapId(reader, project, display, "off_disabled_pixelmap_id");
    info->pixelmap_id[CHECKED_DISABLED_PIXELMAP_INDEX] = ReadPixelmapId(reader, project, display, "on_disabled_pixelmap_id");
}

void radio_button_service_provider::AssignPixelmap(widget_info *info, int index,
    GX_RESOURCE_ID pixmap_id)
{
    info->pixelmap_id[index] = pixmap_id;
    GX_WIDGET *widget = info->widget;

    if (widget)
    {
        GX_RADIO_BUTTON *button = (GX_RADIO_BUTTON *)widget;
        GX_RESOURCE_ID off = button->gx_radio_button_off_pixelmap_id;
        GX_RESOURCE_ID on = button->gx_radio_button_on_pixelmap_id;
        GX_RESOURCE_ID dis_off = button->gx_radio_button_off_disabled_pixelmap_id;
        GX_RESOURCE_ID dis_on = button->gx_radio_button_on_disabled_pixelmap_id;

        switch (index)
        {
        case RADIO_OFF_PIXELMAP_INDEX:
            if (pixmap_id)
            {
                off = pixmap_id;
            }
            else
            {
                off = GX_PIXELMAP_RADIO_OFF_ID;
            }
            break;
        case RADIO_ON_PIXELMAP_INDEX:
            if (pixmap_id)
            {
                on = pixmap_id;
            }
            else
            {
                on = GX_PIXELMAP_RADIO_ON_ID;
            }
            break;
        case RADIO_OFF_DISABLED_PIXELMAP_INDEX:
            if (pixmap_id)
            {
                dis_off = pixmap_id;
            }
            else
            {
                dis_off = GX_PIXELMAP_RADIO_OFF_ID;
            }
            break;
        case RADIO_ON_DISABLED_PIXELMAP_INDEX:
            if (pixmap_id)
            {
                dis_on = pixmap_id;
            }
            else
            {
                dis_on = GX_PIXELMAP_RADIO_ON_ID;
            }
            break;
        }

        gx_radio_button_pixelmap_set(button, off, on, dis_off, dis_on);
    }
}

void radio_button_service_provider::Autosize(widget_info *info)
{
    CheckResizeToText(info);
    CheckResizeToPixelmap(info, RESIZE_MODE_HEIGHT, TRUE);
}
