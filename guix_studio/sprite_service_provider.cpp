
#include "studiox_includes.h"
#include "gx_win32_studio_display_driver.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define MAX_SPRITE_FRAMES     256

static void studio_sprite_draw(GX_SPRITE* sprite)
{
    GX_SPRITE_FRAME* frame;
    GX_PIXELMAP* map;
    GX_VALUE         xpos;
    GX_VALUE         ypos;
    int              theme_index;
    res_info* info;
    studiox_project* project = GetOpenProject();


    GX_WIN32_DISPLAY_DRIVER_DATA* data = gx_win32_get_data_instance_by_thread_id(GetCurrentThreadId());

    if ((!data) || (!project))
    {
        return;
    }

    /* Draw sprite background.  */
    gx_widget_background_draw((GX_WIDGET*)sprite);

    if (sprite->gx_sprite_frame_list)
    {
        frame = &sprite->gx_sprite_frame_list[sprite->gx_sprite_current_frame];
        theme_index = project->mDisplays[data->win32_display_index].active_theme;
        info = project->FindResource(data->win32_display_index, theme_index, RES_TYPE_PIXELMAP, frame->gx_sprite_frame_pixelmap);

        if (!info)
        {
            return;
        }

        if (sprite->gx_sprite_current_frame < sprite->gx_sprite_frame_count)
        {
            map = info->GetPixelmap(sprite->gx_sprite_current_frame);
            if (map)
            {
                xpos = (GX_VALUE)(sprite->gx_widget_size.gx_rectangle_left + frame->gx_sprite_frame_x_offset);
                ypos = (GX_VALUE)(sprite->gx_widget_size.gx_rectangle_top + frame->gx_sprite_frame_y_offset);

                if (frame->gx_sprite_frame_alpha != GX_ALPHA_VALUE_OPAQUE)
                {
                    gx_canvas_pixelmap_blend(xpos, ypos, map, frame->gx_sprite_frame_alpha);
                }
                else
                {
                    gx_canvas_pixelmap_draw(xpos, ypos, map);
                }
            }
        }
    }

    /* Draw children widgets of sprite widget.  */
    gx_widget_children_draw((GX_WIDGET*)sprite);
}

sprite_service_provider::sprite_service_provider()
{
}

CString sprite_service_provider::GetVarDeclaration()
{
    return CString("GX_SPRITE_MEMBERS_DECLARE");
}

CString sprite_service_provider::DeclarePropertiesStruct()
{
    CString out(""
    "typedef struct\n"
    "{\n"
    "    GX_SPRITE_FRAME *frame_list;\n"
    "    USHORT           frame_count;\n"
    "} GX_SPRITE_PROPERTIES;\n\n");

    return out;
}

TCHAR *sprite_background_string[] = {
    _T("GX_SPRITE_BACKGROUND_NO_ACTION"),
    _T("GX_SPRITE_BACKGROUND_SOLID_FILL"),
    _T("GX_SPRITE_BACKGROUND_RESTORE")
}; 

CString sprite_service_provider::WriteExtendedProperties(screen_generator *gen, CString &prefix, widget_info *info)
{
    int index;
    int back_op;
    CString temp;
    CString list;
    GX_SPRITE_FRAME *frame;
    CString propname = prefix + info->app_name;

    if (info->ewi.sprite.frame_count > 0)
    {
        list.Format(_T("GX_SPRITE_FRAME %s_frame_list[%d] =\n{\n"),
            propname, info->ewi.sprite.frame_count);

        frame = info->ewi.sprite.framelist;
        int frame_id;

        for (index = 0; index < info->ewi.sprite.frame_count; index++)
        {
            if (info->pixelmap_id[NORMAL_PIXELMAP_INDEX])
            {
                frame_id = index;
            }
            else
            {
                frame_id = 0;
            }

            back_op = frame->gx_sprite_frame_background_operation;
            if (back_op >= 3)
            {
                back_op = 0;
            }

            temp.Format(_T("    {\n")
                        _T("        %s,  /* pixelmap id */\n")
                        _T("        %d,  /* x offset */\n")
                        _T("        %d,  /* y offset */\n")
                        _T("        %d,  /* frame delay */\n")
                        _T("        %s,  /* background operation */\n")
                        _T("        %d  /* alpha value*/\n")
                        _T("    }"),
                gen->GetPixelmapIdName(frame->gx_sprite_frame_pixelmap, frame_id),
                frame->gx_sprite_frame_x_offset,
                frame->gx_sprite_frame_y_offset,
                frame->gx_sprite_frame_delay,
                sprite_background_string[back_op],
                frame->gx_sprite_frame_alpha);

            if (index == info->ewi.sprite.frame_count - 1)
            {
                temp += "\n";
            }
            else
            {
                temp += ",\n";
            }
            list += temp;
            frame++;
        }
        list += "};\n\n";

        temp.Format(_T("GX_SPRITE_PROPERTIES %s_properties =\n")
            _T("{\n")
            _T("    %s_frame_list,     /* address of frame list */\n")
            _T("    %d,                /* frame count */\n")
            _T("};\n"),
            propname, propname, info->ewi.sprite.frame_count);

        list += temp;
    }
    else
    {
        list.Format(_T("GX_SPRITE_PROPERTIES %s_properties =\n")
                    _T("{\n")
                    _T("    GX_NULL,     /* address of frame list */\n")
                    _T("    0,           /* frame count */\n")
                    _T("};\n"),
            propname);
    }
    return list;
}

CString sprite_service_provider::GetCreateFromDefFunctionName()
{
    return CString("gx_studio_sprite_create");
}

CString sprite_service_provider::GetCreateFromDefFunction(int version)
{
    CString out;
    MakeCreatePreamble("sprite", version, out);

    out += "{\n"
        "    UINT status;\n"
        "    GX_SPRITE *sprite = (GX_SPRITE *) control_block;\n"
        "    GX_SPRITE_PROPERTIES *props = (GX_SPRITE_PROPERTIES *) info->properties;\n"   
        "    status = gx_sprite_create(sprite, info->widget_name, parent,\n"
        "               props->frame_list, props->frame_count,\n"
        "               info->style, info->widget_id, &info->size);\n"
        "    return status;\n"
        "}\n";
    return out;
}

widget_info *sprite_service_provider::CreateNewInstance(GX_WIDGET *parent)
{
    GX_RECTANGLE size;
    gx_utility_rectangle_define(&size, 0, 0, 63, 63);
    gx_utility_rectangle_center(&size, &parent->gx_widget_size);
    
    GX_SPRITE *sprite = new GX_SPRITE;
    gx_sprite_create(sprite, "sprite", parent,
        GX_NULL, 0, GX_STYLE_ENABLED, 0, &size);
    widget_info *info = InitWidgetInfo((GX_WIDGET *) sprite);
    info->ewi.sprite.framelist = NULL;
    info->ewi.sprite.frame_count = 0;
    info->ewi.sprite.apply_to_all_frames = FALSE;
    return info;
}

GX_WIDGET *sprite_service_provider::GenerateFromInfo(GX_WIDGET *parent, widget_info *info)
{
    GX_SPRITE *sprite = new GX_SPRITE;
    ULONG style = info->style;
    
    gx_sprite_create(sprite,
        (CHAR *) info->app_name.GetString(),
        parent,
        info->ewi.sprite.framelist, info->ewi.sprite.frame_count,
        style, 0, &info->size);

    if (info->pixelmap_id[NORMAL_PIXELMAP_INDEX])
    {
        gx_widget_draw_set(sprite, studio_sprite_draw);
    }

    gx_widget_fill_color_set((GX_WIDGET *) sprite,
        info->color_id[NORMAL_FILL_COLOR_INDEX], info->color_id[SELECTED_FILL_COLOR_INDEX], info->color_id[DISABLED_FILL_COLOR_INDEX]);
    return ((GX_WIDGET *) sprite);
}
   
void sprite_service_provider::SaveToProject(xml_writer &writer, studiox_project *project, int display, widget_info *info)
{
    int index;
    GX_SPRITE_FRAME *frame;

    widget_service_provider::SaveToProject(writer, project, display, info);
    WritePixelmapId(writer, project, display, "pixelmap_id", info->pixelmap_id[NORMAL_PIXELMAP_INDEX]);
    writer.WriteInt("framecount", info->ewi.sprite.frame_count);
    writer.WriteBool("apply_to_all_frames", info->ewi.sprite.apply_to_all_frames);
    writer.OpenTag("framelist");
    frame = info->ewi.sprite.framelist;

    for (index = 0; index < info->ewi.sprite.frame_count; index++)
    {
        writer.OpenTag("sprite_frame");
        WritePixelmapId(writer, project, display, "pixelmap", frame->gx_sprite_frame_pixelmap);
        writer.WriteInt("x_offset", frame->gx_sprite_frame_x_offset);
        writer.WriteInt("y_offset", frame->gx_sprite_frame_y_offset);
        writer.WriteInt("delay", frame->gx_sprite_frame_delay);
        writer.WriteInt("background", frame->gx_sprite_frame_background_operation);
        writer.WriteUnsigned("alpha", frame->gx_sprite_frame_alpha);
        writer.CloseTag("sprite_frame");
        frame++;
    }
    writer.CloseTag("framelist");
}

void sprite_service_provider::ReadFromProject(xml_reader &reader, studiox_project *project, int display, widget_info *info, ULONG valid_styles)
{
    int framecount;
    int index;
    int val;
    unsigned long uval;
    GX_SPRITE_FRAME *frame;

    valid_styles |= GX_STYLE_SPRITE_AUTO|GX_STYLE_SPRITE_LOOP;
 
    widget_service_provider::ReadFromProject(reader, project, display, info, valid_styles);

    info->pixelmap_id[NORMAL_PIXELMAP_INDEX] = ReadPixelmapId(reader, project, display, "pixelmap_id");

    reader.ReadInt("framecount", framecount);
    info->ewi.sprite.frame_count = framecount;

    reader.ReadBool("apply_to_all_frames", info->ewi.sprite.apply_to_all_frames);

    if (framecount)
    {
        info->ewi.sprite.framelist = new GX_SPRITE_FRAME[framecount];
        memset(info->ewi.sprite.framelist, 0, sizeof(GX_SPRITE_FRAME) * framecount);
        frame = info->ewi.sprite.framelist;

        if (reader.EnterSection("framelist"))
        {
            for (index = 0; index < framecount; index++)
            {
                if (reader.EnterSection("sprite_frame"))
                {
                    frame->gx_sprite_frame_pixelmap = ReadPixelmapId(reader, project, display, "pixelmap");
                    reader.ReadValue("x_offset", frame->gx_sprite_frame_x_offset);
                    reader.ReadValue("y_offset", frame->gx_sprite_frame_y_offset);
                    reader.ReadInt("delay", val);
                    frame->gx_sprite_frame_delay = val;
                    reader.ReadInt("background", val);
                    frame->gx_sprite_frame_background_operation = val;
                    reader.ReadUnsigned("alpha", uval);
                    frame->gx_sprite_frame_alpha = (unsigned char) uval;
                    reader.CloseSection(TRUE);
                }
                frame++;
            }
            reader.CloseSection(TRUE);
        }
    } 
}

void sprite_service_provider::UpdateSpriteFrameInfo(widget_info *info)
{
    studiox_project* project = GetOpenProject();

    if (!project)
    {
        return;
    }

    int display = project->GetDisplayIndex(info);

    if (display < 0)
    {
        return;
    }

    CString name;

    project->GetResourceName(display, RES_TYPE_PIXELMAP, info->pixelmap_id[0], name);

    int active_theme = project->mDisplays[display].active_theme;

    res_info* rinfo = project->FindResource(display, active_theme, RES_TYPE_PIXELMAP, name);

    if (info->ewi.sprite.framelist)
    {
        delete info->ewi.sprite.framelist;
        info->ewi.sprite.framelist = NULL;
        info->ewi.sprite.frame_count = 0;
    }

    if (rinfo && rinfo->map_list.GetCount())
    {
        info->ewi.sprite.frame_count = rinfo->GetPixelmapFrameCount();

        if (info->ewi.sprite.frame_count > MAX_SPRITE_FRAMES)
        {
            info->ewi.sprite.frame_count = MAX_SPRITE_FRAMES;
        }

        info->ewi.sprite.framelist = new GX_SPRITE_FRAME[info->ewi.sprite.frame_count];
        memset(info->ewi.sprite.framelist, 0, sizeof(GX_SPRITE_FRAME) * info->ewi.sprite.frame_count);

        GX_SPRITE_FRAME* frame;

        for (int frameindex = 0; frameindex < info->ewi.sprite.frame_count; frameindex++)
        {
            frame = &info->ewi.sprite.framelist[frameindex];

            frame->gx_sprite_frame_alpha = 255;
            frame->gx_sprite_frame_delay = rinfo->GetPixelmapDelayTime(frameindex);
            frame->gx_sprite_frame_delay /= GX_SYSTEM_TIMER_MS;
            frame->gx_sprite_frame_pixelmap = info->pixelmap_id[0];
        }
    }

    GX_WIDGET* widget = info->widget;

    if (widget)
    {
        GX_SPRITE* sprite = (GX_SPRITE*)widget;
        gx_sprite_frame_list_set(sprite, info->ewi.sprite.framelist, info->ewi.sprite.frame_count);
    }
}

void sprite_service_provider::AssignPixelmap(widget_info *info, int index,
    GX_RESOURCE_ID pixmap_id)
 {
    if (info->pixelmap_id[index] != pixmap_id)
    {
        info->pixelmap_id[index] = pixmap_id;

        UpdateSpriteFrameInfo(info);
    }
    else
    {
        GX_WIDGET* widget = info->widget;

        if (widget)
        {
            GX_SPRITE* sprite = (GX_SPRITE*)widget;
            gx_sprite_frame_list_set(sprite, info->ewi.sprite.framelist, info->ewi.sprite.frame_count);
        }
    }
}

void sprite_service_provider::Autosize(widget_info *info)
{
    GX_PIXELMAP *map = NULL;
    GX_VALUE border_width;
    int map_index;

    int max_map_width = -1;
    int max_map_height = -1;
    GX_SPRITE_FRAME *frame;

    studiox_project *project = GetOpenProject();
    int display = project->GetDisplayIndex(info);
    int active_theme = project->mDisplays[display].active_theme;

    if (display >= 0)
    {
        frame = info->ewi.sprite.framelist;

        for (map_index = 0; map_index < info->ewi.sprite.frame_count; map_index++)
        {
            if (frame->gx_sprite_frame_pixelmap)
            {
                res_info *resource = project->FindResource(display, active_theme, RES_TYPE_PIXELMAP, frame->gx_sprite_frame_pixelmap);

                if (resource)
                {
                    map = resource->GetPixelmap();
                    if (map)
                    {
                        if (map->gx_pixelmap_width > max_map_width)
                        {
                            max_map_width = map->gx_pixelmap_width;
                        }
                        if (map->gx_pixelmap_height > max_map_height)
                        {
                            max_map_height = map->gx_pixelmap_height;
                        }
                    }
                }
            }
        }
    }
    
    if (max_map_height <= 0 || max_map_width <= 0)
    {
        return;
    }

    // check to see if new size if any different

    GX_RECTANGLE newsize = info->widget->gx_widget_size;
    gx_widget_border_width_get(info->widget, &border_width);
    newsize.gx_rectangle_right = newsize.gx_rectangle_left + max_map_width + (border_width * 2) - 1;
    newsize.gx_rectangle_bottom = newsize.gx_rectangle_top + max_map_height + (border_width * 2) - 1;

    if (!gx_utility_rectangle_compare(&newsize, &info->widget->gx_widget_size))
    {
        gx_widget_resize(info->widget, &newsize);
        info->size = newsize;
        GetPropsWin()->WidgetWasMoved();
    }
}
