

#include "studiox_includes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//#define TEST_WIDGET_COPY

extern UINT  StudioClipboardFormat;

///////////////////////////////////////////////////////////////////////////////
copy_paste_info::copy_paste_info()
{
    template_dependencies = NULL;
    copied_widgets = NULL;
}

///////////////////////////////////////////////////////////////////////////////
copy_paste_info::~copy_paste_info()
{
    if (template_dependencies)
    {
        delete template_dependencies;
    }

    if (copied_widgets)
    {
        delete copied_widgets;
    }
}

///////////////////////////////////////////////////////////////////////////////
copy_paste_engine::copy_paste_engine()
{
    write_color_list.SetSize(0,  10);
    write_font_list.SetSize(0, 10);
    write_pixelmap_list.SetSize(0, 10);
    write_string_list.SetSize(0, 10);

    color_trans.SetSize(0, 10);
    string_trans.SetSize(0, 10);

    mProject = GetOpenProject();
}

///////////////////////////////////////////////////////////////////////////////
copy_paste_engine::~copy_paste_engine()
{
}

///////////////////////////////////////////////////////////////////////////////
void copy_paste_engine::SetProject(studiox_project *project)
{
    if (project)
    {
        mProject = project;
    }
}

///////////////////////////////////////////////////////////////////////////////
BOOL copy_paste_engine::CopyToClipboard(CArray<widget_info *> &info_list, HWND hWnd, folder_info *folder)
{
    if ((!mProject) || (info_list.IsEmpty() && !folder))
    {
        return FALSE;
    }

    xml_writer writer;
    CSharedFile *pMemFile = new CSharedFile();

    ClearResourceList();

    if (folder)
    {
        mDisplay = mProject->GetDisplayIndex(folder);
    }
    else
    {
        mDisplay = mProject->GetDisplayIndex(info_list.GetAt(0));
    }

    widget_info *info;

    for (int index = 0; index < info_list.GetCount(); index++)
    {
        info = info_list.GetAt(index);

        BuildBaseList(info);
    }

    writer.OpenMemFile(pMemFile);

    writer.OpenTag("GUIX_Studio_Copy");
    WriteColorDependencies(writer);
    WriteFontDependencies(writer);
    WritePixelmapDependencies(writer);
    WriteStringDependencies(writer);

    if (base_list.GetCount())
    {
        writer.OpenTag("template_dependencies");
        for (int index = base_list.GetCount() - 1; index >= 0; index--)
        {
            widget_writer::WriteWidgets(writer, mProject, mDisplay, base_list.GetAt(index), FALSE);
        }
        writer.CloseTag("template_dependencies");
    }

    writer.OpenTag("copied_widgets");

    if (folder)
    {
        //write folder information
        writer.WriteString("folder_name", folder->folder_name);
    }

    if (info_list.GetCount())
    {
        for (int index = 0; index < info_list.GetCount(); index++)
        {
            widget_writer::WriteWidgets(writer, mProject, mDisplay, info_list.GetAt(index), FALSE);
        }
    }

    writer.CloseTag("copied_widgets");


    writer.CloseTag("GUIX_Studio_Copy");

    HANDLE hMem = pMemFile->Detach();
    delete pMemFile;
    if (OpenClipboard(hWnd))
    {
        SetClipboardData(StudioClipboardFormat, hMem);
        CloseClipboard();
        return TRUE;
    }

    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
BOOL copy_paste_engine::CopyToClipboard(widget_info *info, HWND hWnd)
{
    CArray<widget_info *> info_list;
    info_list.Add(info);

    return CopyToClipboard(info_list, hWnd);
}

///////////////////////////////////////////////////////////////////////////////
/*  Note of Copy folder: Since only one folder can be selceted at same time, so just consider copy one folder here.
Import project use the logic copy-paste widgets. */
BOOL copy_paste_engine::CopyToClipboard(folder_info *folder, HWND hWnd)
{
    CArray<widget_info *> info_list;
    widget_info *screen = folder->GetFirstChildWidget();

    while (screen)
    {
        info_list.Add(screen);
        screen = screen->GetNextWidgetInfo();
    }

    return CopyToClipboard(info_list, hWnd, folder);
}


///////////////////////////////////////////////////////////////////////////////
void copy_paste_engine::ClearResourceList()
{
    write_color_list.RemoveAll();
    write_font_list.RemoveAll();
    write_pixelmap_list.RemoveAll();
    write_string_list.RemoveAll();

    base_list.RemoveAll();
}

void copy_paste_engine::ClearResourceTransList()
{
    color_trans.RemoveAll();
    string_trans.RemoveAll();
}

///////////////////////////////////////////////////////////////////////////////
folder_info *copy_paste_engine::ReadWidgets(xml_reader &reader, const char *tagname)
{
    folder_info *new_folder = NULL;

    if (reader.EnterSection(tagname))
    {
        new_folder = new folder_info();

        reader.ReadString("folder_name", new_folder->folder_name);

        widget_info *new_widget = NULL;
        widget_info *pre = NULL;

        while (reader.EnterSection(CT2A(widget_factory::WidgetTypeToString(GX_TYPE_WIDGET))))
        {
            new_widget = widget_reader::ReadOneWidget(reader, mProject, mDisplay);

            if (new_widget)
            {
                if (!new_folder->GetFirstChildWidget())
                {
                    new_folder->SetFirstChildWidget(new_widget);
                }

                if (pre)
                {
                    pre->SetNextWidgetInfo(new_widget);
                }
                pre = new_widget;
                widget_reader::ReadChildWidgets(reader, mProject, mDisplay, new_widget);
            }

            reader.CloseSection(TRUE, TRUE);
        }
        TranslateWidgetResourceIds(new_folder->GetFirstChildWidget());

        reader.CloseSection(TRUE, TRUE);
    }

    return new_folder;
}

///////////////////////////////////////////////////////////////////////////////
copy_paste_info *copy_paste_engine::PasteFromClipboard(HWND hWnd, int Display)
{
    if (!mProject)
    {
        return NULL;
    }

    copy_paste_info *paste_info = NULL;
    mDisplay = Display;

    if (OpenClipboard(hWnd))
    {
        HANDLE hMem = GetClipboardData(StudioClipboardFormat);

        if (!hMem)
        {
            CloseClipboard();
            ErrorMsg("Unknown clipboard data format.");
            return NULL;
        }
        CSharedFile *pFile = new CSharedFile;
        pFile->SetHandle(hMem);
        xml_reader reader;
        reader.ReadFile(pFile);

        if (reader.EnterSection("GUIX_Studio_Copy"))
        {
            mProject->mHeader.warn_missing_font = TRUE;
            mProject->mHeader.warn_missing_image = TRUE;
            ClearResourceTransList();

            if (ReadColorDependencies(reader) &&
                ReadFontDependencies(reader) &&
                ReadPixelmapDependencies(reader) &&
                ReadStringDependencies(reader))
            {
                GetResourceView()->FinalizePasteResources();

                paste_info = new copy_paste_info;
                paste_info->template_dependencies = ReadWidgets(reader, "template_dependencies");
                paste_info->copied_widgets = ReadWidgets(reader, "copied_widgets");
            }
            reader.CloseSection(TRUE, TRUE);
        }

        pFile->Detach();
        delete pFile;
        CloseClipboard();
    }

    return paste_info;
}

///////////////////////////////////////////////////////////////////////////////
void copy_paste_engine::WriteColorDependencies(xml_writer &writer)
{
    int index;
    int active_theme;

    writer.OpenTag("colors");

    for (index = 0; index < write_color_list.GetCount(); index++)
    {
        active_theme = mProject->mDisplays[mDisplay].active_theme;

        res_info *color_info = mProject->FindResource(mDisplay, active_theme, RES_TYPE_COLOR, write_color_list.GetAt(index));

        if (color_info)
        {
            if (color_info->is_default == FALSE)
            {
                writer.OpenTag("resource");
                //always write RGB color
                GX_COLOR color_value = color_info->colorval;
                theme_info *theme = &mProject->mDisplays[mDisplay].themes[active_theme];
                color_info->colorval = ProjectConfigDlg::GetRGBColor(color_info->colorval,
                                                                     theme->palette,
                                                                     theme->palette_predefined,
                                                                     mProject->mDisplays[mDisplay].colorformat);
                mProject->WriteOneResource(writer, color_info);
                color_info->colorval = color_value;
                writer.CloseTag("resource");
            }
        }
    }
    writer.CloseTag("colors");
}

///////////////////////////////////////////////////////////////////////////////
BOOL copy_paste_engine::ReadColorDependencies(xml_reader &reader)
{
    res_info *newres;
    CString res_type_name;
    resource_view *res_view = GetResourceView();
    int conflict_option = 0;
    
    if (reader.EnterSection("colors"))
    {
        while(reader.EnterSection("resource"))
        {
            resource_trans_entry entry;

            newres = NULL;
            reader.ReadString("type", res_type_name);
            int restype = mProject->ResStringToType(res_type_name);

            if (restype == RES_TYPE_COLOR)
            {
                newres = new res_info(RES_TYPE_COLOR);
                mProject->ReadOneResource(reader, mDisplay, newres);
            }
            reader.CloseSection(TRUE, TRUE);

            if (newres)
            {
                entry.source_id_name = newres->name;
                conflict_option = res_view->PasteResource(newres, conflict_option);
                entry.translated_id_name = newres->name;
                color_trans.Add(entry);

                delete newres;

                if (conflict_option == PASTE_CONFLICT_OPTION_ABORT)
                {
                    return FALSE;
                }
            }
        }
        reader.CloseSection(TRUE, TRUE);

        return TRUE;
    }
    return FALSE;
}


///////////////////////////////////////////////////////////////////////////////
void copy_paste_engine::WriteFontDependencies(xml_writer &writer)
{
    int index;
    int active_theme = mProject->mDisplays[mDisplay].active_theme;

    writer.OpenTag("fonts");
    for (index = 0; index < write_font_list.GetCount(); index++)
    {
        res_info *font_info = mProject->FindResource(mDisplay, active_theme, RES_TYPE_FONT, write_font_list.GetAt(index));
        if (font_info)
        {
            if (font_info->is_default == FALSE)
            {
                writer.OpenTag("resource");
                mProject->WriteOneResource(writer, font_info);
                writer.CloseTag("resource");
            }
        }
    }
    writer.CloseTag("fonts");
}

///////////////////////////////////////////////////////////////////////////////
BOOL copy_paste_engine::ReadFontDependencies(xml_reader &reader)
{
    res_info *newres;
    CString res_type_name;
    resource_view *res_view = GetResourceView();
    
    if (reader.EnterSection("fonts"))
    {
        while(reader.EnterSection("resource"))
        {
            newres = NULL;
            reader.ReadString("type", res_type_name);
            int restype = mProject->ResStringToType(res_type_name);

            if (restype == RES_TYPE_FONT)
            {
                newres = new res_info(RES_TYPE_FONT);
                mProject->ReadOneResource(reader, mDisplay, newres);
            }
            reader.CloseSection(TRUE, TRUE);

            if (newres)
            {
                res_view->PasteResource(newres);
                delete newres;
            }
        }
        reader.CloseSection(TRUE, TRUE);

        return TRUE;
    }
    return FALSE;
}


///////////////////////////////////////////////////////////////////////////////
void copy_paste_engine::WritePixelmapDependencies(xml_writer &writer)
{
    int index;
    int active_theme = mProject->mDisplays[mDisplay].active_theme;

    writer.OpenTag("pixelmaps");
    for (index = 0; index < write_pixelmap_list.GetCount(); index++)
    {
        res_info *pix_info = mProject->FindResource(mDisplay, active_theme, RES_TYPE_PIXELMAP, write_pixelmap_list.GetAt(index));
        if (pix_info)
        {
            if (pix_info->is_default == FALSE)
            {
                writer.OpenTag("resource");
                mProject->WriteOneResource(writer, pix_info);
                writer.CloseTag("resource");
            }
        }
    }
    writer.CloseTag("pixelmaps");
}

///////////////////////////////////////////////////////////////////////////////
BOOL copy_paste_engine::ReadPixelmapDependencies(xml_reader &reader)
{
    res_info *newres;
    CString res_type_name;
    resource_view *res_view = GetResourceView();

    if (reader.EnterSection("pixelmaps"))
    {
        while(reader.EnterSection("resource"))
        {
            newres = NULL;
            reader.ReadString("type", res_type_name);
            int restype = mProject->ResStringToType(res_type_name);

            if (restype == RES_TYPE_PIXELMAP)
            {
                newres = new res_info(RES_TYPE_PIXELMAP);
                mProject->ReadOneResource(reader, mDisplay, newres);
            }
            reader.CloseSection(TRUE, TRUE);

            if (newres)
            {
                res_view->PasteResource(newres);
                delete newres;
            }
        }
        reader.CloseSection(TRUE, TRUE);

        return TRUE;
    }
    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
void copy_paste_engine::WriteStringDependencies(xml_writer &writer)
{
    int index;
    int string_id;
    int language;
    CString id_name;
    string_table_record record;

    string_table *table = mProject->mDisplays[mDisplay].stable;

    writer.OpenTag("string_table");

    if (table && write_string_list.GetCount() > 0)
    {
        writer.WriteInt("num_strings", write_string_list.GetCount());
        writer.WriteInt("num_languages", table->CountLanguages());

        if (table->CountLanguages() != mProject->mHeader.num_languages)
        {
            ErrorMsg("Internal Error: Language count discrepency");
            writer.CloseTag("string_table");
            return;
        }

        for (index = 0; index < write_string_list.GetCount(); index++)
        {
            string_id = write_string_list.GetAt(index);
            id_name = table->GetResourceIdName(string_id);
            record = table->GetRecord(id_name);
            writer.OpenTag("string_record");
            writer.WriteString("id_name", record.id_name);
            writer.WriteInt("font", record.font_id);
            writer.WriteString("notes", record.notes);
            
            for (language = 0; language < table->CountLanguages(); language++)
            {
                writer.WriteString("val", table->GetString(id_name, language), TRUE);
            }
            writer.CloseTag("string_record");
        }
    }
    writer.CloseTag("string_table");
}

///////////////////////////////////////////////////////////////////////////////
BOOL copy_paste_engine::ReadStringDependencies(xml_reader &reader)
{
    int index;
    int num_strings;
    int num_languages;
    int language;
    string_table_record record;
    int conflict_option = 0;

    resource_view *res_view = GetResourceView();
    string_table *table = mProject->mDisplays[mDisplay].stable;

    if (!table)
    {
        return FALSE;
    }
    if (reader.EnterSection("string_table"))
    {
        reader.ReadInt("num_strings", num_strings);
        reader.ReadInt("num_languages", num_languages);

        if (num_strings && num_languages != mProject->mHeader.num_languages)
        {
            ErrorMsg("Source and Destination projects have inconsistent langauge count.");
            reader.CloseSection();
            return FALSE;
        }

        for (index = 0; index < num_strings; index++)
        {   
            if (reader.EnterSection("string_record"))
            {
                resource_trans_entry entry;

                reader.ReadString("id_name", record.id_name);
                reader.ReadInt("font", record.font_id);
                reader.ReadString("notes", record.notes);

                entry.source_id_name = record.id_name;

                record.strings = new CString[num_languages];

                for (language = 0; language < table->CountLanguages(); language++)
                {
                    reader.ReadString("val", record.strings[language]);
                }
                reader.CloseSection(TRUE, TRUE);
                if (record.strings)
                {
                    conflict_option = res_view->PasteResource(record, conflict_option);
                    entry.translated_id_name = record.id_name;
                    string_trans.Add(entry);

                    if (conflict_option == PASTE_CONFLICT_OPTION_ABORT)
                    {
                        delete[] record.strings;
                        return FALSE;
                    }

                    delete[] record.strings;
                }
            }
        }
        reader.CloseSection(TRUE, TRUE);
        return TRUE;
    }
    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
void copy_paste_engine::CheckAddToList(CArray<int> &list, int res_id)
{
    if (!IsItemInArray<int>(list, res_id))
    {
        list.Add(res_id);
    }
}

///////////////////////////////////////////////////////////////////////////////
void copy_paste_engine::BuildResourceList(widget_info *info, BOOL entry)
{
    int index;

    // Go through info adding his resources to the resource lists.
    // Call recursively to add child resources

    while(info)
    {  
        for (index = 0; index < NUM_WIDGET_COLORS; index++)
        {
            if (info->color_id[index] != 0)
            {
                CheckAddToList(write_color_list, info->color_id[index]);
            }
        }

        for (index = 0; index < NUM_WIDGET_FONTS; index++)
        {
            if (info->font_id[index] != 0)
            {
                CheckAddToList(write_font_list, info->font_id[index]);
            }
        }


        for (index = 0; index < NUM_WIDGET_PIXELMAPS; index++)
        {
            if (info->pixelmap_id[index] != 0)
            {
                CheckAddToList(write_pixelmap_list, info->pixelmap_id[index]);
            }
        }

        if (info->basetype == GX_TYPE_STRING_SCROLL_WHEEL)
        {
            if (info->ewi.string_scroll_wheel.string_id_list)
            {
                for (index = 0; index < info->ewi.string_scroll_wheel.base.total_rows; index++)
                {
                    if (info->ewi.string_scroll_wheel.string_id_list[index] != 0)
                    {
                        CheckAddToList(write_string_list, info->ewi.string_scroll_wheel.string_id_list[index]);
                    }
                }
            }
        }
        else
        {
            for (index = 0; index < NUM_WIDGET_STRINGS; index++)
            {
                if (info->string_id[index] != 0)
                {
                    CheckAddToList(write_string_list, info->string_id[index]);
                }
            }
        }


        // scrollbar and sprite have additional pixelmaps:
        if (info->basetype == GX_TYPE_VERTICAL_SCROLL ||
            info->basetype == GX_TYPE_HORIZONTAL_SCROLL)
        {
            if (info->ewi.scroll.gx_scroll_down_pixelmap)
            {
                CheckAddToList(write_pixelmap_list, info->ewi.scroll.gx_scroll_down_pixelmap);
            }
            if (info->ewi.scroll.gx_scroll_up_pixelmap)
            {
                CheckAddToList(write_pixelmap_list, info->ewi.scroll.gx_scroll_up_pixelmap);
            }
            if (info->ewi.scroll.gx_scroll_fill_pixelmap)
            {
                CheckAddToList(write_pixelmap_list, info->ewi.scroll.gx_scroll_fill_pixelmap);
            }
            if (info->ewi.scroll.gx_scroll_thumb_pixelmap)
            {
                CheckAddToList(write_pixelmap_list, info->ewi.scroll.gx_scroll_thumb_pixelmap);
            }
        }
        else
        {
            if (info->basetype == GX_TYPE_SPRITE)
            {
                GX_SPRITE_FRAME *frame = info->ewi.sprite.framelist;

                for (index = 0; index < info->ewi.sprite.frame_count; index++)
                {
                    if (frame->gx_sprite_frame_pixelmap)
                    {
                        CheckAddToList(write_pixelmap_list, frame->gx_sprite_frame_pixelmap);
                    }
                    frame++;
                }
            }
        }
    
        if (info->GetChildWidgetInfo())
        {
            BuildResourceList(info->GetChildWidgetInfo());
        }
        if (entry)
        {
            info = NULL;
        }
        else
        {
            info = info->GetNextWidgetInfo();
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void copy_paste_engine::BuildBaseList(widget_info *base)
{
    if (!base->is_template)
    {
        BuildResourceList(base, TRUE);
    }

    folder_info *folder = mProject->mDisplays[mDisplay].GetFirstChildFolder();

    while (base->basetype == GX_TYPE_TEMPLATE)
    {
        base = mProject->FindWidgetInfo(folder, base->base_name, FALSE);

        CheckAddToBaseList(base);
    }
}

///////////////////////////////////////////////////////////////////////////////
void copy_paste_engine::CheckAddToBaseList(widget_info *base)
{
    if (IsItemInArray<widget_info *>(base_list, base))
    {
        return;
    }

    BuildResourceList(base, TRUE);
    base_list.Add(base);
}

///////////////////////////////////////////////////////////////////////////////
int copy_paste_engine::TranslateId(CArray<resource_trans_entry> &list, int res_type, int res_id)
{
    int index;
    CString source_name;
    CString translate_name;
    string_table *table;
    int translate_id = res_id;

    switch (res_type)
    {
    case RES_TYPE_COLOR:
        mProject->GetResourceName(mDisplay, res_type, res_id, source_name);
        break;
    case RES_TYPE_STRING:
        table = mProject->mDisplays[mDisplay].stable;
        if (table)
        {
            source_name = table->GetResourceIdName(res_id);
        }
        break;
    }
   
    for (index = 0; index < list.GetCount(); index++)
    {
        if (list.GetAt(index).source_id_name == source_name)
        {
            translate_name = list.GetAt(index).translated_id_name;
            break;
        }
    }

    if (translate_name.IsEmpty())
    {
        //default color id
        translate_name = source_name;
    }

    if (translate_name != source_name)
    {
        switch (res_type)
        {
        case RES_TYPE_COLOR:
            translate_id = mProject->GetResourceId(mDisplay, res_type, translate_name);
            break;
        case RES_TYPE_STRING:
            if (table)
            {
                translate_id = table->GetResourceId(translate_name);
            }
            break;
        }
    }

    return translate_id;
}

///////////////////////////////////////////////////////////////////////////////
void copy_paste_engine::TranslateWidgetResourceIds(widget_info *info)
{
    int index;
    CString name;
    CString translate_name;

    while (info)
    {
        for (index = 0; index < NUM_WIDGET_COLORS; index++)
        {
            if (info->color_id[index] != 0)
            {
                info->color_id[index] = TranslateId(color_trans, RES_TYPE_COLOR, info->color_id[index]);
            }
        }

        for (index = 0; index < NUM_WIDGET_STRINGS; index++)
        {
            if (info->string_id[index] != 0)
            {
                info->string_id[index] = TranslateId(string_trans, RES_TYPE_STRING, info->string_id[index]);
            }
        }

        if (info->GetChildWidgetInfo())
        {
            TranslateWidgetResourceIds(info->GetChildWidgetInfo());
        }
        info = info->GetNextWidgetInfo();
    }
}

