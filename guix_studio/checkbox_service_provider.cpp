
#include "studiox_includes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

checkbox_service_provider::checkbox_service_provider()
{
}

CString checkbox_service_provider::GetVarDeclaration()
{
    return CString("GX_CHECKBOX_MEMBERS_DECLARE");
}

CString checkbox_service_provider::DeclarePropertiesStruct()
{
    CString out;

    out.Format(_T("typedef struct\n")
        _T("{\n")
        _T("%s\n")
        _T("    GX_RESOURCE_ID unchecked_pixelmap_id;\n")
        _T("    GX_RESOURCE_ID checked_pixelmap_id;\n")
        _T("    GX_RESOURCE_ID unchecked_disabled_pixelmap_id;\n")
        _T("    GX_RESOURCE_ID checked_disabled_pixelmap_id;\n")
        _T("} GX_CHECKBOX_PROPERTIES;\n\n"),
        text_button_service_provider::DeclaraPropertiesStructContent());

    return out;
}

CString checkbox_service_provider::WriteExtendedProperties(screen_generator *gen, CString &prefix, widget_info *info)
{
    CString out;
    CString propname = prefix + info->app_name;

    GX_RESOURCE_ID check_on_id;
    GX_RESOURCE_ID check_off_id;

    check_on_id = info->pixelmap_id[CHECKED_PIXELMAP_INDEX];
    if (check_on_id == GX_PIXELMAP_CHECKBOX_ON_ID)
    {
        // if the id is the default, set it to 0 to avoid 
        // generating pixelmap set call
        check_on_id = 0;
    }

    check_off_id = info->pixelmap_id[UNCHECKED_PIXELMAP_INDEX];
    if (check_off_id == GX_PIXELMAP_CHECKBOX_OFF_ID)
    {
        // if the id is the default, set it to 0 to avoid 
        // generating pixelmap set call
        check_off_id = 0;
    }


    out.Format(_T("GX_CHECKBOX_PROPERTIES %s_properties =\n")
            _T("{\n")
            _T("%s\n")
            _T("    %s, /* unchecked pixelmap id */\n")
            _T("    %s, /* checked pixelmap id */\n")
            _T("    %s, /* unchecked disabled pixelmap id */\n")
            _T("    %s  /* checked disabled pixelmap id */\n};\n"),
            propname,
            text_button_service_provider::WriteExtendedPropertiesContent(gen, info, TRUE),
            gen->GetPixelmapIdName(check_off_id),
            gen->GetPixelmapIdName(check_on_id),
            gen->GetPixelmapIdName(info->pixelmap_id[UNCHECKED_DISABLED_PIXELMAP_INDEX]),
            gen->GetPixelmapIdName(info->pixelmap_id[CHECKED_DISABLED_PIXELMAP_INDEX])
            );
    return out;
}

CString checkbox_service_provider::GetCreateFromDefFunctionName()
{
    return CString("gx_studio_checkbox_create");
}

CString checkbox_service_provider::GetCreateFromDefFunction(int version)
{
    CString out;
    MakeCreatePreamble("checkbox", version, out);

    out += "{\n"
        "    UINT status;\n"
        "    GX_CHECKBOX *button = (GX_CHECKBOX *) control_block;\n"
        "    GX_TEXT_BUTTON *text_button = (GX_TEXT_BUTTON *) button;\n"
        "    GX_CHECKBOX_PROPERTIES *props = (GX_CHECKBOX_PROPERTIES *) info->properties;\n"
        "    status = gx_checkbox_create(button, info->widget_name, parent, props->string_id, info->style, info->widget_id, &info->size);\n"
        "    if (status == GX_SUCCESS)\n"
        "    {\n"
        "        gx_text_button_font_set(text_button, props->font_id);\n";

    if (version > 50401)
    {
        out +=  "#if defined(GUIX_5_4_0_COMPATIBILITY)\n"
            "        gx_text_button_text_color_set(text_button, props->normal_text_color_id, props->selected_text_color_id);\n "
            "#else\n"
            "        gx_text_button_text_color_set(text_button, props->normal_text_color_id, props->selected_text_color_id, props->disabled_text_color_id);\n"
            "#endif\n";
    }
    else
    {
        out +=  "        gx_text_button_text_color_set(text_button, props->normal_text_color_id, props->selected_text_color_id);\n";
    }

    out +=  "\n"
            "        if (props->unchecked_pixelmap_id ||\n"
            "            props->checked_pixelmap_id ||\n"
            "            props->unchecked_disabled_pixelmap_id ||\n"
            "            props->checked_disabled_pixelmap_id)\n"
            "        {\n"
            "            gx_checkbox_pixelmap_set(button,\n"
            "                                     props->unchecked_pixelmap_id,\n"
            "                                     props->checked_pixelmap_id,\n"
            "                                     props->unchecked_disabled_pixelmap_id,\n"
            "                                     props->checked_disabled_pixelmap_id);\n"
            "        }\n"
            "    }\n"
            "    return status;\n"
            "}\n";

    return out;
}

widget_info *checkbox_service_provider::CreateNewInstance(GX_WIDGET *parent)
{
    GX_RECTANGLE size;
    gx_utility_rectangle_define(&size, 0, 0, 79, 23);
    gx_utility_rectangle_center(&size, &parent->gx_widget_size);
    
    GX_CHECKBOX *button = new GX_CHECKBOX;
    GX_RESOURCE_ID string_id = GetStringId("checkbox");
    gx_checkbox_create(button, "checkbox", parent,
        string_id, GX_STYLE_ENABLED|GX_STYLE_TRANSPARENT|GX_STYLE_TEXT_LEFT, 0, &size);

    widget_info *info = InitWidgetInfo((GX_WIDGET *) button);

    info->string_id[0]  = string_id;

    info->color_id[NORMAL_TEXT_COLOR_INDEX] = button->gx_text_button_normal_text_color;
    info->color_id[SELECTED_TEXT_COLOR_INDEX] = button->gx_text_button_selected_text_color;
    info->color_id[DISABLED_TEXT_COLOR_INDEX] = button->gx_text_button_disabled_text_color;
    info->font_id[0] = button->gx_text_button_font_id;
    
    info->pixelmap_id[UNCHECKED_PIXELMAP_INDEX] = GX_PIXELMAP_CHECKBOX_OFF_ID;
    info->pixelmap_id[CHECKED_PIXELMAP_INDEX] = GX_PIXELMAP_CHECKBOX_ON_ID;

    return info;
}

GX_WIDGET *checkbox_service_provider::GenerateFromInfo(GX_WIDGET *parent, widget_info *info)
{
    GX_CHECKBOX *button = new GX_CHECKBOX;
    gx_checkbox_create(button,
        (CHAR *) info->app_name.GetString(),
        parent,
        info->string_id[0],
        info->style,
        0,
        &info->size);

    if(info->pixelmap_id[UNCHECKED_PIXELMAP_INDEX] == 0)
    {
        info->pixelmap_id[UNCHECKED_PIXELMAP_INDEX] = GX_PIXELMAP_CHECKBOX_OFF_ID;
    }
    if (info->pixelmap_id[CHECKED_PIXELMAP_INDEX] == 0)
    {
        info->pixelmap_id[CHECKED_PIXELMAP_INDEX] = GX_PIXELMAP_CHECKBOX_ON_ID;
    }


    gx_checkbox_pixelmap_set(button,
        info->pixelmap_id[UNCHECKED_PIXELMAP_INDEX],
        info->pixelmap_id[CHECKED_PIXELMAP_INDEX],
        info->pixelmap_id[UNCHECKED_DISABLED_PIXELMAP_INDEX],
        info->pixelmap_id[CHECKED_DISABLED_PIXELMAP_INDEX]);

    GX_TEXT_BUTTON *tb = (GX_TEXT_BUTTON *)button;

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
        
void checkbox_service_provider::SaveToProject(xml_writer &writer, studiox_project *project, int display, widget_info *info)
{
    text_button_service_provider::SaveToProject(writer, project, display, info);
    WritePixelmapId(writer, project, display, "unchecked_pixelmap_id", info->pixelmap_id[UNCHECKED_PIXELMAP_INDEX]);
    WritePixelmapId(writer, project, display, "checked_pixelmap_id", info->pixelmap_id[CHECKED_PIXELMAP_INDEX]);
    WritePixelmapId(writer, project, display, "unchecked_disabled_pixelmap_id", info->pixelmap_id[UNCHECKED_DISABLED_PIXELMAP_INDEX]);
    WritePixelmapId(writer, project, display, "checked_disabled_pixelmap_id", info->pixelmap_id[CHECKED_DISABLED_PIXELMAP_INDEX]);
}

void checkbox_service_provider::ReadFromProject(xml_reader &reader, studiox_project *project, int display, widget_info *info, ULONG valid_styles)
{
    text_button_service_provider::ReadFromProject(reader, project, display,  info, valid_styles);
    info->pixelmap_id[UNCHECKED_PIXELMAP_INDEX] = ReadPixelmapId(reader, project, display, "unchecked_pixelmap_id");
    info->pixelmap_id[CHECKED_PIXELMAP_INDEX] = ReadPixelmapId(reader, project, display, "checked_pixelmap_id");
    info->pixelmap_id[UNCHECKED_DISABLED_PIXELMAP_INDEX] = ReadPixelmapId(reader, project, display, "unchecked_disabled_pixelmap_id");
    info->pixelmap_id[CHECKED_DISABLED_PIXELMAP_INDEX] = ReadPixelmapId(reader, project, display, "checked_disabled_pixelmap_id");
}

void checkbox_service_provider::AssignText(widget_info *info, int index, GX_RESOURCE_ID string_id)
{
    GX_WIDGET *widget = info->widget;

    if (widget)
    {
        GX_TEXT_BUTTON *pb = (GX_TEXT_BUTTON *)widget;

        gx_text_button_text_id_set(pb, string_id);
        gx_system_dirty_mark(widget);
    }

    info->string_id[0] = string_id;
}

void checkbox_service_provider::AssignFont(widget_info *info, int index, GX_RESOURCE_ID font_id)
{
    GX_WIDGET *widget = info->widget;

    if (widget)
    {
        GX_TEXT_BUTTON *pb = (GX_TEXT_BUTTON *)widget;

        gx_text_button_font_set(pb, font_id);
    }

    info->font_id[0] = font_id;
}

       
void checkbox_service_provider::AssignPixelmap(widget_info *info, int index, GX_RESOURCE_ID pixmap_id)
{
    GX_WIDGET *widget = info->widget;

    if (widget)
    {
        GX_CHECKBOX *box = (GX_CHECKBOX *)widget;
        GX_RESOURCE_ID ucheck = box->gx_checkbox_unchecked_pixelmap_id;
        GX_RESOURCE_ID check = box->gx_checkbox_checked_pixelmap_id;
        GX_RESOURCE_ID ducheck = box->gx_checkbox_unchecked_disabled_pixelmap_id;
        GX_RESOURCE_ID dcheck = box->gx_checkbox_checked_disabled_pixelmap_id;

        switch (index)
        {
        case UNCHECKED_PIXELMAP_INDEX:
            if (!pixmap_id)
            {
                ucheck = pixmap_id;
            }
            else
            {
                //set to default pixelmap
                ucheck = GX_PIXELMAP_CHECKBOX_OFF_ID;
            }
            break;
        case CHECKED_PIXELMAP_INDEX:
            if (pixmap_id)
            {
                check = pixmap_id;
            }
            else
            {
                check = GX_PIXELMAP_CHECKBOX_ON_ID;
            }
            break;
        case UNCHECKED_DISABLED_PIXELMAP_INDEX:
            if (pixmap_id)
            {
                ducheck = pixmap_id;
            }
            else
            {
                ducheck = GX_PIXELMAP_CHECKBOX_OFF_ID;
            }
            break;
        case CHECKED_DISABLED_PIXELMAP_INDEX:
            if (pixmap_id)
            {
                dcheck = pixmap_id;
            }
            else
            {
                dcheck = GX_PIXELMAP_CHECKBOX_ON_ID;
            }
            break;
        }

        gx_checkbox_pixelmap_set(box, ucheck, check, ducheck, dcheck);
    }

    info->pixelmap_id[index] = pixmap_id;

}

void checkbox_service_provider::Autosize(widget_info *info)
{
    CheckResizeToText(info);
    CheckResizeToPixelmap(info, RESIZE_MODE_HEIGHT, TRUE);
}