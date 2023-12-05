#include "studiox_includes.h"

UCHAR filling_string[] = "000";

#define GX_RESOURCE_STANDALONE_HEADER_SIZE 8 /* 2 bytes type, 2 bytes version, 4 bytes count. */

///////////////////////////////////////////////////////////////////////////////
binary_resource_gen::binary_resource_gen(studiox_project *proj, BOOL file_format)
    : resource_gen(proj)
{
    m_num_displays = proj->mHeader.num_displays;
    m_project = proj;

    m_written_size = 0;
    m_written_start_address = 0;
    m_file_format = file_format;
    m_srec_address = proj->mHeader.memory_offset;
    m_srec_data_size = 0;
    memset(m_srec_data, 0, SREC_MAX_DATA_SIZE);
    m_srec_record_count = 0;
    m_output_resource_id = 0;

    if (GetCmdInfo()->IsNoGui())
    {
        m_big_endian = GetCmdInfo()->IsBigEndian();
    }
    else
    {
        m_big_endian = m_project->mHeader.big_endian;
    }
}


///////////////////////////////////////////////////////////////////////////////
binary_resource_gen::~binary_resource_gen()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////
void binary_resource_gen::InitRotatedPixelmaps()
{
    for (int theme = 0; theme < m_project->mDisplays[m_display].num_themes; theme++)
    {
        m_rotated_pixelmaps[theme].RemoveAll();

        for (int index = 0; index < m_pixelmap_table_size; index++)
        {
            m_rotated_pixelmaps[theme].Add(GX_NULL);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////
void binary_resource_gen::DestroyRotatedPixelmaps()
{
    GX_PIXELMAP *map;

    for (int theme = 0; theme < m_project->mDisplays[m_display].num_themes; theme++)
    {
        for (int index = 0; index < m_pixelmap_table_size; index++)
        {
            map = m_rotated_pixelmaps[theme].GetAt(index);
            if (map)
            {
                pixelmap_destroy(map);
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void binary_resource_gen::InitResource()
{
    m_project->CopyDictionary(m_display, RES_TYPE_PIXELMAP, &m_pixelmap_dictionary);
    m_project->SortResDictionary(RES_TYPE_PIXELMAP, &m_pixelmap_dictionary);

    CalculateTableSizes();

    InitOptimizedFonts();
    InitRotatedPixelmaps();
    InitFontSizeStorage(m_display);

    if (m_file_format == BINARY_FILE_FORMAT_BIN_STANDALONE)
    {
        MakeFileDictionary();
    }

}

///////////////////////////////////////////////////////////////////////////////
void binary_resource_gen::DestroyResource()
{
    DestroyOptimizedFonts();
    DestroyRotatedPixelmaps();
    DeleteFontSizeStorage(m_display);

    CloseOutputFiles();

    if (m_file_format == BINARY_FILE_FORMAT_BIN_STANDALONE)
    {
        DestroyFileDictionay();
    }
}

///////////////////////////////////////////////////////////////////////////////
BOOL binary_resource_gen::GenerateBinaryFile(int display)
{
    m_display = display;
    m_color_table_size = m_font_table_size = m_pixelmap_table_size = 0;

    GotoProjectDirectory();

    // for now just use project name for output name
    CCommandInfo *pCmdInfo = GetCmdInfo();
    CString res_file_name;

    CCommandInfo *pCmd = GetCmdInfo();
    BOOL gen_res_header;

    if (pCmd->IsNoGui())
    {
        gen_res_header = pCmd->GenResHeader();
    }
    else
    {
        gen_res_header = m_project->mHeader.gen_res_header;
    }

    InitResource();

    if (gen_res_header)
    {
        res_file_name = m_project->mHeader.header_path;
        if (!res_file_name.IsEmpty())
        {
            if (res_file_name.GetAt(res_file_name.GetLength() - 1) != '\\')
            {
                res_file_name += "\\";
            }
        }
        res_file_name += GetResourceFileName() + _T(".h");

        if (m_outfile)
        {
            delete m_outfile;
        }

        m_outfile = new CFile();

        /* Generate head file. */
        if (!m_outfile->Open(res_file_name, CFile::modeCreate | CFile::modeWrite))
        {
            CString msg;
            msg.Format(_T("Could not open output file:\n%s\nPlease check resource file path."), res_file_name);

            ErrorMsg(msg);

            delete m_outfile;
            DestroyResource();
            return FALSE;
        }
        GenerateResourceHeader();
        m_outfile->Close();
    }

    // Generate binary resource data
    res_file_name = m_project->mHeader.resource_path;
    if (!res_file_name.IsEmpty())
    {
        if (res_file_name.GetAt(res_file_name.GetLength() - 1) != '\\')
        {
            res_file_name += "\\";
        }
    }

    res_file_name += GetResourceFileName();

    switch (m_file_format)
    {
    case BINARY_FILE_FORMAT_BIN:
    case BINARY_FILE_FORMAT_BIN_STANDALONE:
        res_file_name += _T(".bin");
        break;

    case BINARY_FILE_FORMAT_SREC:
        res_file_name += _T(".srec");
        break;
    }

    if (m_outfile)
    {
        delete m_outfile;
        m_outfile = NULL;
    }

    m_outfile = new CFile();

    if (!m_outfile->Open(res_file_name, CFile::modeCreate | CFile::modeWrite))
    {
        CString msg;
        msg.Format(_T("Could not open output file:\n%s\nPlease check resource file path."), res_file_name);

        ErrorMsg(msg);
        delete m_outfile;
        DestroyResource();
        return FALSE;
    }

    outfile_list.Add(m_outfile);

    if (m_file_format == BINARY_FILE_FORMAT_SREC)
    {
        m_written_size = m_srec_address;
        m_written_start_address = m_srec_address;
        WriteSRecordHeader();
    }
    else
    {
        m_written_size = 0;
        m_written_start_address = 0;
    }

    if (pCmdInfo->IsXmlMode())
    {
        BOOL enabled;
        m_warn_on_error = TRUE;

        WriteStandaloneResHeader();

        for (int theme = 0; theme < m_project->mDisplays[m_display].num_themes; theme++)
        {
            theme_info* current = &m_project->mDisplays[m_display].themes[theme];

            if (pCmd->IsNoGui())
            {
                enabled = pCmd->IsThemeEnabled(current->theme_name);
            }
            else
            {
                enabled = current->enabled;
            }

            if (!enabled)
            {
                continue;
            }

            for (int font_id = 0; font_id < m_project->CountResources(m_display, RES_TYPE_FONT); font_id++)
            {
                res_info* info = m_project->FindResource(m_display, theme, RES_TYPE_FONT, font_id);

                if (IsResEnabled(info))
                {
                    WriteFontBlock(info, 0, theme);
                }
            }

            for (int pixelmap_id = 1; pixelmap_id < m_pixelmap_dictionary.GetCount(); pixelmap_id++)
            {
                res_info *info = m_project->FindResource(m_display, theme, RES_TYPE_PIXELMAP, m_pixelmap_dictionary.GetAt(pixelmap_id));

                if (IsResEnabled(info) && info->GetPixelmapFrameCount())
                {
                    for (int frame_id = 0; frame_id < info->GetPixelmapFrameCount(); frame_id++)
                    {
                        WritePixelmapBlock(info, 0, 0, theme, frame_id);
                    }
                }
            }
        }
    }
    else
    {
        GenerateBinaryData();
    }
    
    if (m_file_format == BINARY_FILE_FORMAT_SREC)
    {
        if (m_srec_data_size)
        {
            WriteOneSRecord();
        }

        WriteSRecordEnd();
    }

    DestroyResource();

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
BOOL binary_resource_gen::GenerateBinaryData()
{
    WriteResourceHeader();

    BOOL enabled;
    CCommandInfo *pCmd = GetCmdInfo();

    for (int theme = 0; theme < m_project->mDisplays[m_display].num_themes; theme++)
    {
        theme_info *current = &m_project->mDisplays[m_display].themes[theme];

        if (pCmd->IsNoGui())
        {
            enabled = pCmd->IsThemeEnabled(current->theme_name);
        }
        else
        {
            enabled = current->enabled;
        }

        if (enabled)
        {
            WriteThemeBlock(theme);
        }
    }

    if (GetEnabledLanguageCount())
    {
        WriteStringBlock();
    }
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
void binary_resource_gen::WriteResourceHeader()
{
    //write magic number
    GX_RESOURCE_HEADER header;
    ULONG theme_data_size = GetThemeDataSize(-1);
    ULONG string_data_size = GetStringDataSize();

    header.gx_resource_header_magic_number = GX_MAGIC_NUMBER;
    header.gx_resource_header_version = m_project->mHeader.guix_version;
    header.gx_resource_header_theme_count = GetEnabledThemeCount();
    header.gx_resource_header_language_count = GetEnabledLanguageCount();
    header.gx_resource_header_theme_data_size = theme_data_size;
    header.gx_resource_header_string_data_size = string_data_size;
    header.gx_resource_header_data_size = theme_data_size + string_data_size;

    UCHAR header_array[GX_RESOURCE_HEADER_SIZE];
    int index = 0;
    SHORT s;
    LONG l;

    s = SwapUshort(header.gx_resource_header_magic_number);
    memcpy_s(header_array, GX_RESOURCE_HEADER_SIZE, &s, sizeof(USHORT));
    index += sizeof(USHORT);

    if (index < GX_RESOURCE_HEADER_SIZE)
    {
        s = SwapUshort(header.gx_resource_header_version);
        memcpy_s(header_array + index, GX_RESOURCE_HEADER_SIZE - index, &s, sizeof(USHORT));
        index += sizeof(USHORT);
    }

    if (index < GX_RESOURCE_HEADER_SIZE)
    {
        s = SwapUshort(header.gx_resource_header_theme_count);
        memcpy_s(header_array + index, GX_RESOURCE_HEADER_SIZE - index, &s, sizeof(USHORT));
        index += sizeof(USHORT);
    }

    if (index < GX_RESOURCE_HEADER_SIZE)
    {
        s = SwapUshort(header.gx_resource_header_language_count);
        memcpy_s(header_array + index, GX_RESOURCE_HEADER_SIZE - index, &s, sizeof(USHORT));
        index += sizeof(USHORT);
    }

    if (index < GX_RESOURCE_HEADER_SIZE)
    {
        l = SwapUint(header.gx_resource_header_theme_data_size);
        memcpy_s(header_array + index, GX_RESOURCE_HEADER_SIZE - index, &l, sizeof(LONG));
        index += sizeof(LONG);
    }

    if (index < GX_RESOURCE_HEADER_SIZE)
    {
        l = SwapUint(header.gx_resource_header_string_data_size);
        memcpy_s(header_array + index, GX_RESOURCE_HEADER_SIZE - index, &l, sizeof(LONG));
        index += sizeof(LONG);
    }

    if (index < GX_RESOURCE_HEADER_SIZE)
    {
        l = SwapUint(header.gx_resource_header_data_size);
        memcpy_s(header_array + index, GX_RESOURCE_HEADER_SIZE - index, &l, sizeof(LONG));
        index += sizeof(LONG);
    }

    WriteDataOut(header_array, GX_RESOURCE_HEADER_SIZE);
}

///////////////////////////////////////////////////////////////////////////////
void binary_resource_gen::WriteThemeHeader(GX_THEME_HEADER* header)
{
    UCHAR header_array[GX_THEME_HEADER_SIZE];
    int index = 0;
    SHORT s;
    LONG l;

    s = SwapUshort(header->gx_theme_header_magic_number);
    memcpy_s(header_array, GX_THEME_HEADER_SIZE, &s, sizeof(USHORT));
    index += sizeof(USHORT);

    if (index < GX_THEME_HEADER_SIZE)
    {
        s = SwapUshort(header->gx_theme_header_index);
        memcpy_s(header_array + index, GX_THEME_HEADER_SIZE - index, &s, sizeof(USHORT));
        index += sizeof(USHORT);
    }

    if (index < GX_THEME_HEADER_SIZE)
    {
        s = SwapUshort(header->gx_theme_header_color_count);
        memcpy_s(header_array + index, GX_THEME_HEADER_SIZE - index, &s, sizeof(USHORT));
        index += sizeof(USHORT);
    }

    if (index < GX_THEME_HEADER_SIZE)
    {
        s = SwapUshort(header->gx_theme_header_palette_count);
        memcpy_s(header_array + index, GX_THEME_HEADER_SIZE - index, &s, sizeof(USHORT));
        index += sizeof(USHORT);
    }

    if (index < GX_THEME_HEADER_SIZE)
    {
        s = SwapUshort(header->gx_theme_header_font_count);
        memcpy_s(header_array + index, GX_THEME_HEADER_SIZE - index, &s, sizeof(USHORT));
        index += sizeof(USHORT);
    }

    if (index < GX_THEME_HEADER_SIZE)
    {
        s = SwapUshort(header->gx_theme_header_pixelmap_count);
        memcpy_s(header_array + index, GX_THEME_HEADER_SIZE - index, &s, sizeof(USHORT));
        index += sizeof(USHORT);
    }

    if (index < GX_THEME_HEADER_SIZE)
    {
        s = SwapUshort(header->gx_theme_header_vscroll_appearance.gx_scroll_width);
        memcpy_s(header_array + index, GX_THEME_HEADER_SIZE - index, &s, sizeof(SHORT));
        index += sizeof(SHORT);
    }

    if (index < GX_THEME_HEADER_SIZE)
    {
        s = SwapUshort(header->gx_theme_header_vscroll_appearance.gx_scroll_thumb_width);
        memcpy_s(header_array + index, GX_THEME_HEADER_SIZE - index, &s, sizeof(SHORT));
        index += sizeof(SHORT);
    }

    if (index < GX_THEME_HEADER_SIZE)
    {
        s = SwapUshort(header->gx_theme_header_vscroll_appearance.gx_scroll_thumb_travel_min);
        memcpy_s(header_array + index, GX_THEME_HEADER_SIZE - index, &s, sizeof(SHORT));
        index += sizeof(SHORT);
    }

    if (index < GX_THEME_HEADER_SIZE)
    {
        s = SwapUshort(header->gx_theme_header_vscroll_appearance.gx_scroll_thumb_travel_max);
        memcpy_s(header_array + index, GX_THEME_HEADER_SIZE - index, &s, sizeof(SHORT));
        index += sizeof(SHORT);
    }

    if (index + 1 < GX_THEME_HEADER_SIZE)
    {
        header_array[index++] = header->gx_theme_header_vscroll_appearance.gx_scroll_thumb_border_style;
        l = SwapUint(header->gx_theme_header_vscroll_appearance.gx_scroll_fill_pixelmap);
        memcpy_s(header_array + index, GX_THEME_HEADER_SIZE - index, &l, sizeof(LONG));
        index += sizeof(LONG);
    }

    if (index < GX_THEME_HEADER_SIZE)
    {
        l = SwapUint(header->gx_theme_header_vscroll_appearance.gx_scroll_thumb_pixelmap);
        memcpy_s(header_array + index, GX_THEME_HEADER_SIZE - index, &l, sizeof(LONG));
        index += sizeof(LONG);
    }

    if (index < GX_THEME_HEADER_SIZE)
    {
        l = SwapUint(header->gx_theme_header_vscroll_appearance.gx_scroll_up_pixelmap);
        memcpy_s(header_array + index, GX_THEME_HEADER_SIZE - index, &l, sizeof(LONG));
        index += sizeof(LONG);
    }

    if (index < GX_THEME_HEADER_SIZE)
    {
        l = SwapUint(header->gx_theme_header_vscroll_appearance.gx_scroll_down_pixelmap);
        memcpy_s(header_array + index, GX_THEME_HEADER_SIZE - index, &l, sizeof(LONG));
        index += sizeof(LONG);
    }

    if (index < GX_THEME_HEADER_SIZE)
    {
        l = SwapUint(header->gx_theme_header_vscroll_appearance.gx_scroll_thumb_color);
        memcpy_s(header_array + index, GX_THEME_HEADER_SIZE - index, &l, sizeof(LONG));
        index += sizeof(LONG);
    }

    if (index < GX_THEME_HEADER_SIZE)
    {
        l = SwapUint(header->gx_theme_header_vscroll_appearance.gx_scroll_thumb_border_color);
        memcpy_s(header_array + index, GX_THEME_HEADER_SIZE - index, &l, sizeof(LONG));
        index += sizeof(LONG);
    }

    if (index < GX_THEME_HEADER_SIZE)
    {
        l = SwapUint(header->gx_theme_header_vscroll_appearance.gx_scroll_button_color);
        memcpy_s(header_array + index, GX_THEME_HEADER_SIZE - index, &l, sizeof(LONG));
        index += sizeof(LONG);
    }

    if (index < GX_THEME_HEADER_SIZE)
    {
        s = SwapUshort(header->gx_theme_header_hscroll_appearance.gx_scroll_width);
        memcpy_s(header_array + index, GX_THEME_HEADER_SIZE - index, &s, sizeof(SHORT));
        index += sizeof(SHORT);
    }

    if (index < GX_THEME_HEADER_SIZE)
    {
        s = SwapUshort(header->gx_theme_header_hscroll_appearance.gx_scroll_thumb_width);
        memcpy_s(header_array + index, GX_THEME_HEADER_SIZE - index, &s, sizeof(SHORT));
        index += sizeof(SHORT);
    }

    if (index < GX_THEME_HEADER_SIZE)
    {
        s = SwapUshort(header->gx_theme_header_hscroll_appearance.gx_scroll_thumb_travel_min);
        memcpy_s(header_array + index, GX_THEME_HEADER_SIZE - index, &s, sizeof(SHORT));
        index += sizeof(SHORT);
    }

    if (index < GX_THEME_HEADER_SIZE)
    {
        s = SwapUshort(header->gx_theme_header_hscroll_appearance.gx_scroll_thumb_travel_max);
        memcpy_s(header_array + index, GX_THEME_HEADER_SIZE - index, &s, sizeof(SHORT));
        index += sizeof(SHORT);
    }

    if (index + 1 < GX_THEME_HEADER_SIZE)
    {
        header_array[index++] = header->gx_theme_header_hscroll_appearance.gx_scroll_thumb_border_style;
        l = SwapUint(header->gx_theme_header_hscroll_appearance.gx_scroll_fill_pixelmap);
        memcpy_s(header_array + index, GX_THEME_HEADER_SIZE - index, &l, sizeof(LONG));
        index += sizeof(LONG);
    }

    if (index < GX_THEME_HEADER_SIZE)
    {
        l = SwapUint(header->gx_theme_header_hscroll_appearance.gx_scroll_thumb_pixelmap);
        memcpy_s(header_array + index, GX_THEME_HEADER_SIZE - index, &l, sizeof(LONG));
        index += sizeof(LONG);
    }

    if (index < GX_THEME_HEADER_SIZE)
    {
        l = SwapUint(header->gx_theme_header_hscroll_appearance.gx_scroll_up_pixelmap);
        memcpy_s(header_array + index, GX_THEME_HEADER_SIZE - index, &l, sizeof(LONG));
        index += sizeof(LONG);
    }

    if (index < GX_THEME_HEADER_SIZE)
    {
        l = SwapUint(header->gx_theme_header_hscroll_appearance.gx_scroll_down_pixelmap);
        memcpy_s(header_array + index, GX_THEME_HEADER_SIZE - index, &l, sizeof(LONG));
        index += sizeof(LONG);
    }

    if (index < GX_THEME_HEADER_SIZE)
    {
        l = SwapUint(header->gx_theme_header_hscroll_appearance.gx_scroll_thumb_color);
        memcpy_s(header_array + index, GX_THEME_HEADER_SIZE - index, &l, sizeof(LONG));
        index += sizeof(LONG);
    }

    if (index < GX_THEME_HEADER_SIZE)
    {
        l = SwapUint(header->gx_theme_header_hscroll_appearance.gx_scroll_thumb_border_color);
        memcpy_s(header_array + index, GX_THEME_HEADER_SIZE - index, &l, sizeof(LONG));
        index += sizeof(LONG);
    }

    if (index < GX_THEME_HEADER_SIZE)
    {
        l = SwapUint(header->gx_theme_header_hscroll_appearance.gx_scroll_button_color);
        memcpy_s(header_array + index, GX_THEME_HEADER_SIZE - index, &l, sizeof(LONG));
        index += sizeof(LONG);
    }

    if (index < GX_THEME_HEADER_SIZE)
    {
        l = SwapUint(header->gx_theme_header_vscroll_style);
        memcpy_s(header_array + index, GX_THEME_HEADER_SIZE - index, &l, sizeof(LONG));
        index += sizeof(LONG);
    }

    if (index < GX_THEME_HEADER_SIZE)
    {
        l = SwapUint(header->gx_theme_header_hscroll_style);
        memcpy_s(header_array + index, GX_THEME_HEADER_SIZE - index, &l, sizeof(LONG));
        index += sizeof(LONG);
    }

    if (index < GX_THEME_HEADER_SIZE)
    {
        l = SwapUint(header->gx_theme_header_color_data_size);
        memcpy_s(header_array + index, GX_THEME_HEADER_SIZE - index, &l, sizeof(LONG));
        index += sizeof(LONG);
    }

    if (index < GX_THEME_HEADER_SIZE)
    {
        l = SwapUint(header->gx_theme_header_palette_data_size);
        memcpy_s(header_array + index, GX_THEME_HEADER_SIZE - index, &l, sizeof(LONG));
        index += sizeof(LONG);
    }

    if (index < GX_THEME_HEADER_SIZE)
    {
        l = SwapUint(header->gx_theme_header_font_data_size);
        memcpy_s(header_array + index, GX_THEME_HEADER_SIZE - index, &l, sizeof(LONG));
        index += sizeof(LONG);
    }

    if (index < GX_THEME_HEADER_SIZE)
    {
        l = SwapUint(header->gx_theme_header_pixelmap_data_size);
        memcpy_s(header_array + index, GX_THEME_HEADER_SIZE - index, &l, sizeof(LONG));
        index += sizeof(LONG);
    }

    if (index < GX_THEME_HEADER_SIZE)
    {
        l = SwapUint(header->gx_theme_header_data_size);
        memcpy_s(header_array + index, GX_THEME_HEADER_SIZE - index, &l, sizeof(LONG));
        index += sizeof(LONG);
    }

    WriteDataOut(header_array, GX_THEME_HEADER_SIZE);
}

///////////////////////////////////////////////////////////////////////////////
void binary_resource_gen::WriteColorHeader(GX_COLOR_HEADER *header)
{
    UCHAR header_array[GX_COLOR_HEADER_SIZE];
    int index = 0;
    SHORT s;
    LONG l;

    s = SwapUshort(header->gx_color_header_magic_number);
    memcpy_s(header_array, GX_COLOR_HEADER_SIZE, &s, sizeof(USHORT));
    index += sizeof(USHORT);

    if (index < GX_COLOR_HEADER_SIZE)
    {
        s = SwapUshort(header->gx_color_header_color_count);
        memcpy_s(header_array + index, GX_COLOR_HEADER_SIZE - index, &s, sizeof(USHORT));
        index += sizeof(USHORT);
    }

    if (index < GX_COLOR_HEADER_SIZE)
    {
        l = SwapUint(header->gx_color_header_data_size);
        memcpy_s(header_array + index, GX_COLOR_HEADER_SIZE - index, &l, sizeof(ULONG));
        index += sizeof(ULONG);
    }

    WriteDataOut(header_array, GX_COLOR_HEADER_SIZE);
}

///////////////////////////////////////////////////////////////////////////////
void binary_resource_gen::WritePaletteHeader(GX_PALETTE_HEADER *header)
{
    UCHAR header_array[GX_PALETTE_HEADER_SIZE];
    int index = 0;
    SHORT s;
    LONG l;

    s = SwapUshort(header->gx_palette_header_magic_number);
    memcpy_s(header_array, GX_PALETTE_HEADER_SIZE, &s, sizeof(USHORT));
    index += sizeof(USHORT);

    if (index < GX_PALETTE_HEADER_SIZE)
    {
        s = SwapUshort(header->gx_palette_header_color_count);
        memcpy_s(header_array + index, GX_PALETTE_HEADER_SIZE - index, &s, sizeof(USHORT));
        index += sizeof(USHORT);
    }

    if (index < GX_PALETTE_HEADER_SIZE)
    {
        l = SwapUint(header->gx_palette_header_data_size);
        memcpy_s(header_array + index, GX_PALETTE_HEADER_SIZE - index, &l, sizeof(ULONG));
        index += sizeof(ULONG);
    }

    WriteDataOut(header_array, GX_PALETTE_HEADER_SIZE);
}

///////////////////////////////////////////////////////////////////////////////
void binary_resource_gen::WriteFontHeader(GX_FONT_HEADER *header)
{
    UCHAR header_array[GX_FONT_HEADER_SIZE];
    int index = 0;
    SHORT s;
    LONG l;

    s = SwapUshort(header->gx_font_header_magic_number);
    memcpy_s(header_array, GX_FONT_HEADER_SIZE, &s, sizeof(USHORT));
    index += sizeof(USHORT);

    if (index < GX_FONT_HEADER_SIZE)
    {
        s = SwapUshort(header->gx_font_header_index);
        memcpy_s(header_array + index, GX_FONT_HEADER_SIZE - index, &s, sizeof(USHORT));
        index += sizeof(USHORT);
    }

    if (index < GX_FONT_HEADER_SIZE)
    {
        s = SwapUshort(header->gx_font_header_page_count);
        memcpy_s(header_array + index, GX_FONT_HEADER_SIZE  - index, &s, sizeof(USHORT));
        index += sizeof(USHORT);
    }

    if (index + 1 < GX_FONT_HEADER_SIZE)
    {
        header_array[index++] = header->gx_font_header_deault;
        header_array[index++] = header->gx_font_header_bits;
    }

    if (index < GX_FONT_HEADER_SIZE)
    {
        l = SwapUint(header->gx_font_header_data_size);
        memcpy_s(header_array + index, GX_FONT_HEADER_SIZE - index, &l, sizeof(ULONG));
        index += sizeof(ULONG);
    }

    if ((project_lib_version() >= GX_VERSION_DUPLICATE_BINRES_DATA_FIX) && (index < GX_FONT_HEADER_SIZE))
    {
        l = SwapUint(header->gx_font_header_data_offset);
        memcpy_s(header_array + index, GX_FONT_HEADER_SIZE - index, &l, sizeof(ULONG));
        index += sizeof(ULONG);
    }

    WriteDataOut(header_array, GetFontHeaderSize());
}

///////////////////////////////////////////////////////////////////////////////
void binary_resource_gen::WritePageHeader(GX_PAGE_HEADER *header)
{
    UCHAR header_array[GX_PAGE_HEADER_SIZE];
    int index = 0;
    SHORT s;
    LONG l;

    s = SwapUshort(header->gx_page_header_magic_number);
    memcpy_s(header_array, GX_PAGE_HEADER_SIZE , &s, sizeof(USHORT));
    index += sizeof(USHORT);

    if (index < GX_PAGE_HEADER_SIZE)
    {
        s = SwapUshort(header->gx_page_header_index);
        memcpy_s(header_array + index, GX_PAGE_HEADER_SIZE  - index, &s, sizeof(USHORT));
        index += sizeof(USHORT);
    }

    if (index + 4 < GX_PAGE_HEADER_SIZE)
    {
        header_array[index++] = header->gx_page_header_format;
        header_array[index++] = header->gx_page_header_prespace;
        header_array[index++] = header->gx_page_header_postspace;
        header_array[index++] = header->gx_page_header_line_height;
        header_array[index++] = header->gx_page_header_baseline;
    }

    if (index < GX_PAGE_HEADER_SIZE)
    {
        l = SwapUint((UINT)header->gx_page_header_first_glyph);
        memcpy_s(header_array + index, GX_PAGE_HEADER_SIZE  - index, &l, sizeof(ULONG));
        index += sizeof(ULONG);
    }

    if (index < GX_PAGE_HEADER_SIZE)
    {
        l = SwapUint((UINT)header->gx_page_header_last_glyph);
        memcpy_s(header_array + index, GX_PAGE_HEADER_SIZE - index, &l, sizeof(ULONG));
        index += sizeof(ULONG);
    }

    if (index < GX_PAGE_HEADER_SIZE)
    {
        l = SwapUint(header->gx_page_header_data_size);
        memcpy_s(header_array + index, GX_PAGE_HEADER_SIZE - index, &l, sizeof(ULONG));
        index += sizeof(ULONG);
    }

    WriteDataOut(header_array, GX_PAGE_HEADER_SIZE);
}

///////////////////////////////////////////////////////////////////////////////
void binary_resource_gen::WriteGlyphHeader(GX_GLYPH_HEADER* header)
{
    UCHAR header_array[GX_GLYPH_HEADER_SIZE];
    int index = 0;
    SHORT s;
    LONG l;

    s = SwapUshort(header->gx_glyph_header_magic_number);
    memcpy_s(header_array, GX_GLYPH_HEADER_SIZE, &s, sizeof(USHORT));
    index += sizeof(USHORT);

    if (index < GX_GLYPH_HEADER_SIZE)
    {
        s = SwapUshort(header->gx_glyph_header_map_size);
        memcpy_s(header_array + index, GX_GLYPH_HEADER_SIZE - index, &s, sizeof(USHORT));
        index += sizeof(USHORT);
    }

    if ((project_lib_version() >= GX_VERSION_BINRES_FONT_ALIGNMENT_FIX) && (index < GX_GLYPH_HEADER_SIZE))
    {
        l = SwapUint(header->gx_glyph_header_map_offset);
        memcpy_s(header_array + index, GX_GLYPH_HEADER_SIZE - index, &l, sizeof(ULONG));
        index += sizeof(ULONG);
    }

    if (index < GX_GLYPH_HEADER_SIZE)
    {
        s = SwapUshort(header->gx_glyph_header_index);
        memcpy_s(header_array + index, GX_GLYPH_HEADER_SIZE - index, &s, sizeof(USHORT));
        index += sizeof(USHORT);
    }

    if (index < GX_GLYPH_HEADER_SIZE)
    {
        s = SwapUshort(header->gx_glyph_header_ascent);
        memcpy_s(header_array + index, GX_GLYPH_HEADER_SIZE - index, &s, sizeof(USHORT));
        index += sizeof(USHORT);
    }

    if (index < GX_GLYPH_HEADER_SIZE)
    {
        s = SwapUshort(header->gx_glyph_header_descent);
        memcpy_s(header_array + index, GX_GLYPH_HEADER_SIZE - index, &s, sizeof(USHORT));
        index += sizeof(USHORT);
    }

    if (index + 3 < GX_GLYPH_HEADER_SIZE)
    {
        header_array[index++] = header->gx_glyph_header_advance;
        header_array[index++] = header->gx_glyph_header_leading;
        header_array[index++] = header->gx_glyph_header_width;
        header_array[index++] = header->gx_glyph_header_height;
    }

    if (index < GX_GLYPH_HEADER_SIZE)
    {
        l = SwapUint(header->gx_glyph_header_data_size);
        memcpy_s(header_array + index, GX_GLYPH_HEADER_SIZE - index, &l, sizeof(ULONG));
    }

    WriteDataOut(header_array, GetGlyphHeaderSize());
}


///////////////////////////////////////////////////////////////////////////////
void binary_resource_gen::WriteKerningGlyphHeader(GX_KERNING_GLYPH_HEADER *header)
{
    UCHAR header_array[GX_KERNING_GLYPH_HEADER_SIZE];
    int index = 0;
    SHORT s;
    LONG l;

    s = SwapUshort(header->gx_glyph_header_magic_number);
    memcpy_s(header_array, GX_KERNING_GLYPH_HEADER_SIZE, &s, sizeof(USHORT));
    index += sizeof(USHORT);

    if (index < GX_KERNING_GLYPH_HEADER_SIZE)
    {
        s = SwapUshort(header->gx_glyph_header_map_size);
        memcpy_s(header_array + index, GX_KERNING_GLYPH_HEADER_SIZE - index, &s, sizeof(USHORT));
        index += sizeof(USHORT);
    }

    if ((project_lib_version() >= GX_VERSION_BINRES_FONT_ALIGNMENT_FIX) && (index < GX_GLYPH_HEADER_SIZE))
    {
        l = SwapUint(header->gx_glyph_header_map_offset);
        memcpy_s(header_array + index, GX_GLYPH_HEADER_SIZE - index, &l, sizeof(ULONG));
        index += sizeof(ULONG);
    }

    if (index < GX_KERNING_GLYPH_HEADER_SIZE)
    {
        s = SwapUshort(header->gx_glyph_header_index);
        memcpy_s(header_array + index, GX_KERNING_GLYPH_HEADER_SIZE - index, &s, sizeof(USHORT));
        index += sizeof(USHORT);
    }

    if (index < GX_KERNING_GLYPH_HEADER_SIZE)
    {
        s = SwapUshort(header->gx_glyph_header_ascent);
        memcpy_s(header_array + index, GX_KERNING_GLYPH_HEADER_SIZE - index, &s, sizeof(USHORT));
        index += sizeof(USHORT);
    }

    if (index < GX_KERNING_GLYPH_HEADER_SIZE)
    {
        s = SwapUshort(header->gx_glyph_header_descent);
        memcpy_s(header_array + index, GX_KERNING_GLYPH_HEADER_SIZE - index, &s, sizeof(USHORT));
        index += sizeof(USHORT);
    }

    if (index + 3 < GX_KERNING_GLYPH_HEADER_SIZE)
    {
        header_array[index++] = header->gx_glyph_header_advance;
        header_array[index++] = header->gx_glyph_header_leading;
        header_array[index++] = header->gx_glyph_header_width;
        header_array[index++] = header->gx_glyph_header_height;
    }

    if (index < GX_KERNING_GLYPH_HEADER_SIZE)
    {
        l = SwapUint(header->gx_glyph_header_data_size);
        memcpy_s(header_array + index, GX_KERNING_GLYPH_HEADER_SIZE - index, &l, sizeof(ULONG));
        index += sizeof(ULONG);
    }

    if (index < GX_KERNING_GLYPH_HEADER_SIZE)
    {
        s = SwapUshort(header->gx_glyph_header_kerning_table_size);
        memcpy_s(header_array + index, GX_KERNING_GLYPH_HEADER_SIZE - index, &s, sizeof(USHORT));
    }

    WriteDataOut(header_array, GetKerningGlyphHeaderSize());
}

///////////////////////////////////////////////////////////////////////////////
void binary_resource_gen::WritePixelmapHeader(GX_PIXELMAP_HEADER *header)
{
    UCHAR header_array[GX_PIXELMAP_HEADER_SIZE];
    int index = 0;
    SHORT s;
    LONG l;

    s = SwapUshort(header->gx_pixelmap_header_magic_number);
    memcpy_s(header_array, GX_PIXELMAP_HEADER_SIZE, &s, sizeof(USHORT));
    index += sizeof(USHORT);

    if (index < GX_PIXELMAP_HEADER_SIZE)
    {
        s = SwapUshort(header->gx_pixelmap_header_index);
        memcpy_s(header_array + index, GX_PIXELMAP_HEADER_SIZE - index, &s, sizeof(USHORT));
        index += sizeof(USHORT);
    }

    if (index + 3 < GX_PIXELMAP_HEADER_SIZE)
    {
        header_array[index++] = header->gx_pixelmap_header_version_major;
        header_array[index++] = header->gx_pixelmap_header_version_minor;
        header_array[index++] = header->gx_pixelmap_header_flags;
        header_array[index++] = header->gx_pixelmap_header_format;
    }

    if (index < GX_PIXELMAP_HEADER_SIZE)
    {
        l = SwapUint(header->gx_pixelmap_header_map_size);
        memcpy_s(header_array + index, GX_PIXELMAP_HEADER_SIZE - index, &l, sizeof(ULONG));
        index += sizeof(ULONG);
    }

    if (index < GX_PIXELMAP_HEADER_SIZE)
    {
        l = SwapUint(header->gx_pixelmap_header_aux_data_size);
        memcpy_s(header_array + index, GX_PIXELMAP_HEADER_SIZE - index, &l, sizeof(ULONG));
        index += sizeof(ULONG);
    }

    if (index < GX_PIXELMAP_HEADER_SIZE)
    {
        l = SwapUint(header->gx_pixelmap_header_transparent_color);
        memcpy_s(header_array + index, GX_PIXELMAP_HEADER_SIZE - index, &l, sizeof(GX_COLOR));
        index += sizeof(GX_COLOR);
    }
    
    if (index < GX_PIXELMAP_HEADER_SIZE)
    {
        s = SwapUshort(header->gx_pixelmap_header_width);
        memcpy_s(header_array + index, GX_PIXELMAP_HEADER_SIZE - index, &s, sizeof(USHORT));
        index += sizeof(USHORT);
    }

    if (index < GX_PIXELMAP_HEADER_SIZE)
    {
        s = SwapUshort(header->gx_pixelmap_header_height);
        memcpy_s(header_array + index, GX_PIXELMAP_HEADER_SIZE - index, &s, sizeof(USHORT));
        index += sizeof(USHORT);
    }

    if (index < GX_PIXELMAP_HEADER_SIZE)
    {
        l = SwapUint(header->gx_pixelmap_header_data_size);
        memcpy_s(header_array + index, GX_PIXELMAP_HEADER_SIZE - index, &l, sizeof(ULONG));
        index += sizeof(ULONG);
    }

    if ((project_lib_version() >= GX_VERSION_DUPLICATE_BINRES_DATA_FIX) && (index < GX_PIXELMAP_HEADER_SIZE))
    {
        l = SwapUint(header->gx_pixelmap_header_data_offset);
        memcpy_s(header_array + index, GX_PIXELMAP_HEADER_SIZE - index, &l, sizeof(ULONG));
        index += sizeof(ULONG);
    }

    WriteDataOut(header_array, GetPixelmapHeaderSize());
}

///////////////////////////////////////////////////////////////////////////////
void binary_resource_gen::WriteStringHeader(GX_STRING_HEADER *header)
{
    UCHAR header_array[GX_STRING_HEADER_SIZE];
    int index = 0;
    SHORT s;
    LONG l;

    s = SwapUshort(header->gx_string_header_magic_number);
    memcpy_s(header_array, GX_STRING_HEADER_SIZE, &s, sizeof(USHORT));
    index += sizeof(USHORT);

    if (index < GX_STRING_HEADER_SIZE)
    {
        s = SwapUshort(header->gx_string_header_language_count);
        memcpy_s(header_array + index, GX_STRING_HEADER_SIZE  - index, &s, sizeof(USHORT));
        index += sizeof(USHORT);
    }

    if (index < GX_STRING_HEADER_SIZE)
    {
        s = SwapUshort(header->gx_string_header_string_count);
        memcpy_s(header_array + index, GX_STRING_HEADER_SIZE - index, &s, sizeof(USHORT));
        index += sizeof(USHORT);
    }

    if (index < GX_STRING_HEADER_SIZE)
    {
        l = SwapUint(header->gx_string_header_data_size);
        memcpy_s(header_array + index, GX_STRING_HEADER_SIZE - index, &l, sizeof(ULONG));
    }

    WriteDataOut(header_array, GX_STRING_HEADER_SIZE);
}

///////////////////////////////////////////////////////////////////////////////
void binary_resource_gen::WriteLanguageHeader(GX_LANGUAGE_HEADER *header)
{
    UCHAR header_array[GX_LANGUAGE_HEADER_SIZE];
    int index = 0;
    SHORT s;
    LONG l;

    s = SwapUshort(header->gx_language_header_magic_number);
    memcpy_s(header_array, GX_LANGUAGE_HEADER_SIZE, &s, sizeof(USHORT));
    index += sizeof(USHORT);

    if (index < GX_LANGUAGE_HEADER_SIZE)
    {
        s = SwapUshort(header->gx_language_header_index);
        memcpy_s(header_array + index, GX_LANGUAGE_HEADER_SIZE  - index, &s, sizeof(USHORT));
        index += sizeof(USHORT);
    }

    if (index < GX_LANGUAGE_HEADER_SIZE)
    {
        memcpy_s(header_array + index, GX_LANGUAGE_HEADER_SIZE - index, header->gx_language_header_name, GX_LANGUAGE_HEADER_NAME_SIZE);
        index += GX_LANGUAGE_HEADER_NAME_SIZE;
    }

    if (index < GX_LANGUAGE_HEADER_SIZE)
    {
        l = SwapUint(header->gx_language_header_data_size);
        memcpy_s(header_array + index, GX_LANGUAGE_HEADER_SIZE - index, &l, sizeof(ULONG));
        index += sizeof(ULONG);
    }

    WriteDataOut(header_array, GX_LANGUAGE_HEADER_SIZE);
}

///////////////////////////////////////////////////////////////////////////////
void binary_resource_gen::WriteStandaloneResHeader()
{
    if (m_written_size)
    {
        return;
    }

    file_info* info;
    CString filename = RemoveFileExtension(m_outfile->GetFileName());
    filename.MakeUpper();

    info = FindFileInfo(&m_file_dictionary, filename);
    if (!info)
    {
        info = m_file_dictionary.GetAt(0);
    }

    UCHAR header_array[GX_RESOURCE_STANDALONE_HEADER_SIZE];
    int index = 0;
    SHORT s;
    LONG l;
    ULONG offset;
    int res_count = info->size_table.GetCount();

    // Write resource type.
    s = SwapUshort(GX_RESOURCE_TYPE_BINRES_STANDALONE);
    memcpy_s(header_array, GX_RESOURCE_STANDALONE_HEADER_SIZE, &s, sizeof(USHORT));
    index += sizeof(USHORT);

    // Write version.
    if (index < GX_RESOURCE_STANDALONE_HEADER_SIZE)
    {
        s = SwapUshort(m_project->mHeader.guix_version);
        memcpy_s(header_array + index, GX_RESOURCE_STANDALONE_HEADER_SIZE - index, &s, sizeof(USHORT));
        index += sizeof(USHORT);
    }

    // Write resource count.
    if (index < GX_RESOURCE_STANDALONE_HEADER_SIZE)
    {
        l = SwapUint(res_count);
        memcpy_s(header_array + index, GX_RESOURCE_STANDALONE_HEADER_SIZE - index, &l, sizeof(ULONG));
        index += sizeof(ULONG);
    }

    WriteDataOut(header_array, GX_RESOURCE_STANDALONE_HEADER_SIZE);
    
    if (res_count > 1)
    {
        // Generate offset table when more than one resource is generated to one binary file.
        offset = m_written_size + sizeof(ULONG) * res_count;

        for (int index = 0; index < res_count; index++)
        {
            l = SwapUint(offset);
            WriteDataOut((void*)&l, sizeof(ULONG));

            offset += info->size_table.GetAt(index);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
USHORT binary_resource_gen::SwapUshort(USHORT s)
{
    if (m_big_endian)
    {
        UCHAR b1, b2;

        b1 = s & 0xff;
        b2 = (s >> 8) & 0xff;

        return (b1 << 8) + b2;
    }
    else
    {
        return s;
    }
}

///////////////////////////////////////////////////////////////////////////////
UINT  binary_resource_gen::SwapUint(UINT l)
{
    if (m_big_endian)
    {
        UCHAR b1, b2, b3, b4;

        b1 = l & 0xff;
        b2 = (l >> 8) & 0xff;
        b3 = (l >> 16) & 0xff;
        b4 = (l >> 24) & 0xff;

        return ((LONG)b1 << 24) + ((LONG)b2 << 16) + ((LONG)b3 << 8) + b4;
    }
    else
    {
        return l;
    }
}

///////////////////////////////////////////////////////////////////////////////
void binary_resource_gen::WriteThemeBlock(USHORT theme_id)
{
    if (!m_project)
    {
        return;
    }

    GX_THEME_HEADER header;

    header.gx_theme_header_magic_number = GX_MAGIC_NUMBER;
    header.gx_theme_header_index = theme_id;
    header.gx_theme_header_color_count = GetColorCount(theme_id);
    if (m_project->mDisplays[m_display].colorformat == GX_COLOR_FORMAT_8BIT_PALETTE &&
        m_project->mDisplays[m_display].themes[theme_id].palette != NULL)
    {
        /* ONly 8bit palette size is wanted to be generated.
        4bpp driver contains palette table. But it is just used inside Studio.*/
        header.gx_theme_header_palette_count = m_project->mDisplays[m_display].themes[theme_id].palette_total_size;
    }
    else
    {
        header.gx_theme_header_palette_count = 0;
    }

    header.gx_theme_header_font_count = GetFontCount(theme_id);
    header.gx_theme_header_pixelmap_count = GetPixelmapCount(theme_id);
    header.gx_theme_header_color_data_size = GetColorBlockSize(theme_id);
    header.gx_theme_header_palette_data_size = GetPaletteBlockSize(theme_id);

    ULONG written_size = m_written_size;
    written_size += GetThemeHeaderSize();
    written_size += header.gx_theme_header_color_data_size;
    written_size += header.gx_theme_header_palette_data_size;

    header.gx_theme_header_font_data_size = GetFontBlockSize(written_size, theme_id, -1);

    written_size += header.gx_theme_header_font_data_size;

    header.gx_theme_header_pixelmap_data_size = GetPixelmapBlockSize(written_size, theme_id, -1, -1);
    /* calculate theme data size. */
    header.gx_theme_header_data_size = header.gx_theme_header_color_data_size;
    header.gx_theme_header_data_size += header.gx_theme_header_palette_data_size;
    header.gx_theme_header_data_size += header.gx_theme_header_font_data_size;
    header.gx_theme_header_data_size += header.gx_theme_header_pixelmap_data_size;

    memcpy_s(&header.gx_theme_header_vscroll_appearance,
           sizeof(header.gx_theme_header_vscroll_appearance),
           &m_project->mDisplays[m_display].themes[theme_id].VScrollAppearance, sizeof(GX_SCROLLBAR_APPEARANCE));

    memcpy_s(&header.gx_theme_header_hscroll_appearance,
        sizeof(header.gx_theme_header_hscroll_appearance),
        &m_project->mDisplays[m_display].themes[theme_id].HScrollAppearance, sizeof(GX_SCROLLBAR_APPEARANCE));

    header.gx_theme_header_vscroll_style = m_project->mDisplays[m_display].themes[theme_id].VScrollStyle;
    header.gx_theme_header_hscroll_style = m_project->mDisplays[m_display].themes[theme_id].HScrollStyle;

    // Write theme header
    WriteThemeHeader(&header);

    // Write theme data
    theme_info *info = &m_project->mDisplays[m_display].themes[theme_id];
    CCommandInfo *pCmd = GetCmdInfo();
    BOOL gen_color_table;
    BOOL gen_font_table;
    BOOL gen_pixelmap_table;

    if (pCmd->IsNoGui())
    {
        gen_color_table = TRUE;
        gen_font_table = TRUE;
        gen_pixelmap_table = TRUE;
    }
    else
    {
        gen_color_table = info->gen_color_table;
        gen_font_table = info->gen_font_table;
        gen_pixelmap_table = info->gen_pixelmap_table;
    }

    if (gen_color_table &&
        header.gx_theme_header_color_count)
    {
        WriteColorBlock(theme_id);
    }

    if (info->palette &&
        header.gx_theme_header_palette_count)
    {
        WritePaletteBlock(theme_id);
    }

    if (gen_font_table &&
        header.gx_theme_header_font_count)
    {
        int font_id;

        for (font_id = 0; font_id < m_project->CountResources(m_display, RES_TYPE_FONT); font_id++)
        {
            res_info *info = m_project->FindResource(m_display, theme_id, RES_TYPE_FONT, font_id);

            if (info)
            {
                WriteFontBlock(info, font_id, theme_id);
            }
        }
    }

    if (gen_pixelmap_table &&
        header.gx_theme_header_pixelmap_count)
    {
        int pixelmap_id;
        int frame_id;
        int output_id = 1;

        for (pixelmap_id = 1; pixelmap_id < m_pixelmap_dictionary.GetCount(); pixelmap_id++)
        {
            res_info *info = m_project->FindResource(m_display, theme_id, RES_TYPE_PIXELMAP, m_pixelmap_dictionary.GetAt(pixelmap_id));

            if (info && info->GetPixelmapFrameCount() && (info->enabled) && (info->parent->enabled))
            {
                for (frame_id = 0; frame_id < info->GetPixelmapFrameCount(); frame_id++)
                {
                    WritePixelmapBlock(info, pixelmap_id, output_id++, theme_id, frame_id);
                }
            }
            else if (IsSystemPixelmap(pixelmap_id))
            {
                output_id++;
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void binary_resource_gen::WriteColorBlock(int theme_id)
{
    int color_count = GetColorCount(theme_id);

    GX_COLOR_HEADER header;
    header.gx_color_header_magic_number = GX_MAGIC_NUMBER;
    header.gx_color_header_color_count = color_count;
    header.gx_color_header_data_size = color_count * sizeof(GX_COLOR);

    //Write Color Header
    WriteColorHeader(&header);

    //Write color data
    int color_id;
    GX_COLOR rgb_color;
    GX_COLOR native_color;
    int color_format;

    color_format = m_project->mDisplays[m_display].colorformat;

    // write color table data
    for (color_id = 0; color_id < m_project->CountResources(m_display, RES_TYPE_COLOR); color_id++)
    {
        res_info *info = m_project->FindResource(m_display, theme_id, RES_TYPE_COLOR, color_id);

        if (info)
        {
            rgb_color = info->colorval;
            native_color = resource_view::GetNativeColor(rgb_color, color_format);

            native_color = SwapUint(native_color);

            WriteDataOut(&native_color, sizeof(GX_COLOR));
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void binary_resource_gen::WritePaletteBlock(int theme_id)
{
    theme_info *info = &m_project->mDisplays[m_display].themes[theme_id];
    int color_count = info->palette_total_size;
    int palette_index;
    GX_COLOR rgb_color;

    if (color_count)
    {
        GX_PALETTE_HEADER header;
        header.gx_palette_header_magic_number = GX_MAGIC_NUMBER;
        header.gx_palette_header_color_count = color_count; 
        header.gx_palette_header_data_size = color_count * sizeof(GX_COLOR);

        //Write Palette Header
        WritePaletteHeader(&header);

        //Write palette data

        for (palette_index = 0; palette_index < color_count; palette_index++)
        {
            rgb_color = SwapUint(info->palette[palette_index]);
            WriteDataOut(&rgb_color, sizeof(ULONG));
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
void binary_resource_gen::WriteFontBlock(res_info *info, USHORT font_id, USHORT theme_id)
{
    GX_FONT_HEADER header;
    int reference_theme = -1;
    USHORT output_font_id = font_id;

    if (m_file_format == BINARY_FILE_FORMAT_BIN_STANDALONE)
    {
        CString filename = RemoveFileExtension(m_outfile->GetFileName());
        filename.MakeUpper();

        file_info *finfo = FindFileInfo(&m_file_dictionary, filename);
        if (!finfo)
        {
            finfo = m_file_dictionary.GetAt(0);
        }
        output_font_id = finfo->write_res_index++;
    }

    memset(&header, 0, sizeof(GX_FONT_HEADER));
    header.gx_font_header_magic_number = GX_MAGIC_NUMBER;
    header.gx_font_header_index = output_font_id;
    header.gx_font_header_page_count = GetPageCount(info, theme_id, font_id);
    header.gx_font_header_bits = info->font_bits;

    if (info->is_default && info->pathinfo.pathname.IsEmpty() &&
        !IsRotatedResourceSupported(m_project, m_display))
    {
        header.gx_font_header_deault = TRUE;
    }
    else
    {
        header.gx_font_header_deault = FALSE;

        if (project_lib_version() >= GX_VERSION_DUPLICATE_BINRES_DATA_FIX)
        {
            reference_theme = FindResourceReferenceTheme(info, theme_id);
        }
    }

    if (reference_theme >= 0)
    {
        header.gx_font_header_data_offset = GetFontDataOffset(font_id, reference_theme);

        //write font header
        WriteFontHeader(&header);
    }
    else
    {
        header.gx_font_header_data_size = GetFontDataSize(m_written_size + GetFontHeaderSize(), info, theme_id, font_id);

        //write font header
        WriteFontHeader(&header);

        //write font data
        WriteFontData(info, theme_id, font_id);
    }
}

///////////////////////////////////////////////////////////////////////////////
void binary_resource_gen::WriteFontData(res_info *info, USHORT theme_id, USHORT font_id)
{

    const GX_FONT *font_page;
    GX_FONT *head_page;
    int page_index;

    if (info->is_default && info->pathinfo.pathname.IsEmpty() &&
        !IsRotatedResourceSupported(m_project, m_display))
    {
        return;
    }

    head_page = GetPageHead(info, theme_id, font_id);

    //write font data
    font_page = head_page;
    page_index = 1;
    while (font_page)
    {
        WritePageBlock(font_page, page_index);

        page_index++;
        font_page = font_page->gx_font_next_page;
    }
}


///////////////////////////////////////////////////////////////////////////////
void binary_resource_gen::WritePageBlock(const GX_FONT *font, USHORT page_id)
{
    if (!font)
    {
        return;
    }

    GX_PAGE_HEADER header;
    header.gx_page_header_magic_number = GX_MAGIC_NUMBER;
    header.gx_page_header_index = page_id;
    header.gx_page_header_format = font->gx_font_format;
    header.gx_page_header_prespace = font->gx_font_prespace;
    header.gx_page_header_postspace = font->gx_font_postspace;
    header.gx_page_header_line_height = font->gx_font_line_height;
    header.gx_page_header_baseline = font->gx_font_baseline;
    header.gx_page_header_first_glyph = font->gx_font_first_glyph;
    header.gx_page_header_last_glyph = font->gx_font_last_glyph;
    header.gx_page_header_data_size = GetPageDataSize(m_written_size + GetPageHeaderSize(), font);

    if (IsRotatedResourceSupported(m_project, m_display))
    {
        switch (m_project->mDisplays[m_display].rotation_angle)
        {
        case GX_SCREEN_ROTATION_CW:
            header.gx_page_header_format |= GX_FONT_FORMAT_ROTATED_90;
            break;
        case GX_SCREEN_ROTATION_CCW:
            header.gx_page_header_format |= GX_FONT_FORMAT_ROTATED_270;
            break;
        }
    }

    //Write font page header
    WritePageHeader(&header);

    // Write font page data
    GX_CHAR_CODE charval;
    INT glyph_index;

    for (charval = font->gx_font_first_glyph; charval <= font->gx_font_last_glyph; charval++)
    {
        glyph_index = charval - font->gx_font_first_glyph;
        WriteGlyphBock(font, glyph_index);
    }
}

///////////////////////////////////////////////////////////////////////////////
void binary_resource_gen::WriteGlyphBock(const GX_FONT *font, USHORT glyph_index)
{
    studiox_project *project = GetOpenProject();
    if (project)
    {
        const GX_GLYPH *glyph = NULL;
        GX_KERNING_GLYPH_HEADER header;

        if (font->gx_font_format & GX_FONT_FORMAT_COMPRESSED)
        {
            const GX_COMPRESSED_GLYPH *compressed_glyph = &font ->gx_font_glyphs.gx_font_compressed_glyphs[glyph_index];
            header.gx_glyph_header_map_size = compressed_glyph->gx_glyph_map_size;
            glyph = (const GX_GLYPH *)compressed_glyph;
        }
        else if (font->gx_font_format & GX_FONT_FORMAT_KERNING)
        {
            const GX_KERNING_GLYPH *kerning_glyph = &font->gx_font_glyphs.gx_font_kerning_glyphs[glyph_index];
            header.gx_glyph_header_map_size = 0;
            glyph = (const GX_GLYPH *)kerning_glyph;
        }
        else
        {
            glyph = &font->gx_font_glyphs.gx_font_normal_glyphs[glyph_index];
            header.gx_glyph_header_map_size = 0;
        }

        const UCHAR *data = glyph->gx_glyph_map;
        UCHAR *rotated_data = NULL;
        int map_size = header.gx_glyph_header_map_size;
        int map_data_pos;

        if (IsRotatedResourceSupported(m_project, m_display))
        {
            RotateGlyphData(glyph, font->gx_font_format, &rotated_data, &map_size);

            header.gx_glyph_header_map_size = map_size;

            data = rotated_data;
        }

        header.gx_glyph_header_magic_number = GX_MAGIC_NUMBER;
        header.gx_glyph_header_index = glyph_index;
        header.gx_glyph_header_ascent = glyph->gx_glyph_ascent;
        header.gx_glyph_header_descent = glyph->gx_glyph_descent;
        header.gx_glyph_header_advance = glyph->gx_glyph_advance;
        header.gx_glyph_header_leading = glyph->gx_glyph_leading;
        header.gx_glyph_header_width = glyph->gx_glyph_width;
        header.gx_glyph_header_height = glyph->gx_glyph_height;
        header.gx_glyph_header_kerning_table_size = 0; 
        header.gx_glyph_header_map_offset = (m_written_size - m_written_start_address);

        if (font->gx_font_format & GX_FONT_FORMAT_KERNING)
        {
            header.gx_glyph_header_map_offset += GX_KERNING_GLYPH_HEADER_SIZE;
            map_data_pos = m_written_size + GetKerningGlyphHeaderSize();
        }
        else
        {
            header.gx_glyph_header_map_offset += GX_GLYPH_HEADER_SIZE;
            map_data_pos = m_written_size + GetGlyphHeaderSize();
        }

        if (header.gx_glyph_header_map_offset & 0x03)
        {
            header.gx_glyph_header_map_offset = (header.gx_glyph_header_map_offset + 3) & (~0x03UL);
        }

        if (map_size)
        {
            header.gx_glyph_header_data_size = map_size & 0x7fff;
            if (map_data_pos & 0x03)
            {
                header.gx_glyph_header_data_size += (4 - (map_data_pos & 0x03));
            }
        }
        else
        {
            header.gx_glyph_header_data_size = GetGlyphDataSize(map_data_pos, font, glyph_index);
        }

        if (font->gx_font_format & GX_FONT_FORMAT_KERNING)
        {
            header.gx_glyph_header_kerning_table_size = GetGlyphKerningTableSize(font, glyph_index);
            WriteKerningGlyphHeader(&header);
        }
        else
        {
            // Write glyph header
            WriteGlyphHeader((GX_GLYPH_HEADER *)&header);
        }

        // Write Glyph Data
        if (data)
        {
            int padding = 0;

            if ((project_lib_version() >= GX_VERSION_BINRES_FONT_ALIGNMENT_FIX) &&
                (m_written_size & 0x03))
            {
                padding = 4 - (m_written_size & 0x03);
                WriteDataOut(filling_string, padding);
            }

            WriteDataOut(data, header.gx_glyph_header_data_size - padding);
        }

        if (rotated_data)
        {
            delete rotated_data;
        }

        if (font -> gx_font_format & GX_FONT_FORMAT_KERNING)
        {
            if (header.gx_glyph_header_kerning_table_size)
            {
                WriteDataOut(((GX_KERNING_GLYPH *)glyph)->gx_kerning_table, header.gx_glyph_header_kerning_table_size);
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
VOID binary_resource_gen::WriteBigEndianUintData(GX_PIXELMAP *map)
{
    UINT *swap_data = new UINT[map->gx_pixelmap_data_size / 4];
    UINT *data_p = (UINT *)map->gx_pixelmap_data;
    UINT val;

    for (UINT i = 0; i < map->gx_pixelmap_data_size / 4; i++)
    {
        val = SwapUint(*data_p);
        swap_data[i] = val;
        data_p++;
    }

    WriteDataOut(swap_data, map->gx_pixelmap_data_size);

    delete swap_data;
}

///////////////////////////////////////////////////////////////////////////////
VOID binary_resource_gen::WriteBigEndianUshortData(GX_PIXELMAP *map)
{
    USHORT *swap_data = new USHORT[map->gx_pixelmap_data_size / 2];
    USHORT *data_p = (USHORT *)map->gx_pixelmap_data;
    USHORT val;

    for (UINT i = 0; i < map->gx_pixelmap_data_size / 2; i++)
    {
        val = SwapUshort(*data_p);
        swap_data[i] = val;
        data_p++;
    }

    WriteDataOut(swap_data, map->gx_pixelmap_data_size);

    delete swap_data;
}

///////////////////////////////////////////////////////////////////////////////
VOID binary_resource_gen::WriteBigEndianCompressedUshortData(GX_PIXELMAP *map)
{
    USHORT *swap_data = new USHORT[map->gx_pixelmap_data_size / 2];
    USHORT *data_p = (USHORT *)map->gx_pixelmap_data;
    USHORT val;

    for (UINT i = 0; i < map->gx_pixelmap_data_size / 2; i++)
    {
        swap_data[i++] = *data_p;
        data_p++;

        val = SwapUshort(*data_p);
        swap_data[i] = val;
        data_p++;
    }

    WriteDataOut(swap_data, map->gx_pixelmap_data_size);

    delete swap_data;
}

///////////////////////////////////////////////////////////////////////////////
VOID binary_resource_gen::WriteOneSRecord()
{
    INT srec_type;
    if (m_srec_address <= 0xffff)
    {
        srec_type = SREC_TYPE_S1;
    }
    else if (m_srec_address <= 0xffffff)
    {
        srec_type = SREC_TYPE_S2;
    }
    else
    {
        srec_type = SREC_TYPE_S3;
    }

    WriteOneSRecord(srec_type);
}

///////////////////////////////////////////////////////////////////////////////
VOID binary_resource_gen::WriteOneSRecord(INT type)
{
    ULONG crc = 0;
    char buffer[80];
    int  write_size = 0;
    int  count = m_srec_data_size;

    buffer[0] = 'S';
    switch (type)
    {
    case SREC_TYPE_S0:
        count += 3;
        crc += count;
        buffer[1] = '0';
        sprintf_s(buffer + 2, sizeof(buffer) - 2, "%02X", count);
        sprintf_s(buffer + 4, sizeof(buffer) - 4, "%04X", 0);
        write_size = 8;
        break;

    case SREC_TYPE_S1:
        count += 3;
        crc += count;
        crc += m_srec_address & 0xff;
        crc += m_srec_address >> 8;
        buffer[1] = '1';
        sprintf_s(buffer + 2, sizeof(buffer) - 2, "%02X", count);
        sprintf_s(buffer + 4, sizeof(buffer) - 4, "%04X", m_srec_address);
        write_size = 8;
        break;

    case SREC_TYPE_S2:
        count += 4;
        crc += count;
        crc += m_srec_address & 0xff;
        crc += (m_srec_address >> 8) & 0xff;
        crc += m_srec_address >> 16;
        buffer[1] = '2';
        sprintf_s(buffer + 2, sizeof(buffer) - 2,  "%02X", count);
        sprintf_s(buffer + 4, sizeof(buffer) - 4, "%06X", m_srec_address);
        write_size = 10;
        break;

    case SREC_TYPE_S3:
        count += 5;
        crc += count;
        crc += m_srec_address & 0xff;
        crc += (m_srec_address >> 8) & 0xff;
        crc += (m_srec_address >> 16) & 0xff;
        crc += m_srec_address >> 24;
        buffer[1] = '3';
        sprintf_s(buffer + 2, sizeof(buffer) - 2, "%02X", count);
        sprintf_s(buffer + 4, sizeof(buffer) - 4, "%08X", m_srec_address);
        write_size = 12;
        break;

    case SREC_TYPE_S5:
        count += 3;
        crc += count;
        crc += m_srec_record_count & 0xff;
        crc += m_srec_record_count >> 8;
        buffer[1] = '5';
        sprintf_s(buffer + 2, sizeof(buffer) - 2, "%02X", count);
        sprintf_s(buffer + 4, sizeof(buffer) - 4, "%04X", m_srec_record_count);
        write_size = 8;
        break;

    case SREC_TYPE_S6:
        count += 4;
        crc += count;
        crc += m_srec_record_count & 0xff;
        crc += (m_srec_record_count >> 8) & 0xff;
        crc += m_srec_record_count >> 16;
        buffer[1] = '6';
        sprintf_s(buffer + 2, sizeof(buffer) - 2, "%02X", count);
        sprintf_s(buffer + 4, sizeof(buffer) - 4, "%06X", m_srec_record_count);
        write_size = 10;
        break;
    }

    for (int index = 0; index < m_srec_data_size; index++)
    {
        crc += m_srec_data[index];

        if (write_size < sizeof(buffer))
        {
            sprintf_s(buffer + write_size, sizeof(buffer) - write_size, "%02X", m_srec_data[index]);
            write_size += 2;
        }
    }

    crc &= 0xff;
    crc = (~(UCHAR)crc) & 0xff;

    if (write_size < sizeof(buffer))
    {
        sprintf_s(buffer + write_size, sizeof(buffer) - write_size, "%02X", crc);
        write_size += 2;
    }

    if (write_size + 1 < sizeof(buffer))
    {
        buffer[write_size++] = '\r';
        buffer[write_size++] = '\n';
    }

    FileWrite(buffer, write_size);//write srecord

    if (type != SREC_TYPE_S0)
    {
        m_srec_address += m_srec_data_size;
        m_srec_record_count++;
    }
    m_srec_data_size = 0;
}

///////////////////////////////////////////////////////////////////////////////
VOID binary_resource_gen::WriteSRecordData(const void *data_ptr, INT data_size)
{
    INT remain_size;
    INT written_size;

    if (data_size + m_srec_data_size < SREC_MAX_DATA_SIZE)
    {
        memcpy_s(m_srec_data + m_srec_data_size, SREC_MAX_DATA_SIZE - m_srec_data_size, data_ptr, data_size);
        m_srec_data_size += data_size;
    }
    else
    {
        written_size = 0;

        while (written_size != data_size)
        {
            if (m_srec_data_size == SREC_MAX_DATA_SIZE)
            {
                WriteOneSRecord();
            }

            remain_size = SREC_MAX_DATA_SIZE - m_srec_data_size;

            if (data_size - written_size <= remain_size)
            {
                memcpy_s(m_srec_data + m_srec_data_size, remain_size, (char *)data_ptr + written_size, data_size - written_size);
                m_srec_data_size += data_size - written_size;
                written_size = data_size;
            }
            else
            {
                memcpy_s(m_srec_data + m_srec_data_size, remain_size, (char *)data_ptr + written_size, remain_size);
                written_size += remain_size;
                m_srec_data_size += remain_size;
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
VOID binary_resource_gen::WriteSRecordHeader()
{
    INT type = SREC_TYPE_S0;
    const char header[] = "guix_resource_data";

    m_srec_data_size = strlen(header);
    memcpy_s(m_srec_data, sizeof(m_srec_data), header, m_srec_data_size);

    WriteOneSRecord(type);
}

///////////////////////////////////////////////////////////////////////////////
VOID binary_resource_gen::WriteSRecordEnd()
{
    INT type;

    if (m_srec_record_count < 0xffff)
    {
        type = SREC_TYPE_S5;
    }
    else
    {
        return;
    }

    WriteOneSRecord(type);
}

///////////////////////////////////////////////////////////////////////////////
VOID binary_resource_gen::WriteDataOut(const void *lpBuf, UINT nCount)
{
    switch (m_file_format)

    {
    case BINARY_FILE_FORMAT_BIN:
    case BINARY_FILE_FORMAT_BIN_STANDALONE:
        FileWrite(lpBuf, nCount);
        break;

    case BINARY_FILE_FORMAT_SREC:
        WriteSRecordData(lpBuf, nCount);
        break;
    }

    m_written_size += nCount;
}

///////////////////////////////////////////////////////////////////////////////
void binary_resource_gen::WritePixelmapBlock(res_info *info, USHORT map_id, USHORT output_map_id, USHORT theme_id, int frame_id)
{
    GX_PIXELMAP *map = info->GetPixelmap(frame_id);

    if (!map)
    {
        return;
    }

    GX_PIXELMAP_HEADER header;
    int refer_theme = -1;

    if (m_file_format == BINARY_FILE_FORMAT_BIN_STANDALONE)
    {
        CString filename = RemoveFileExtension(m_outfile->GetFileName());
        filename.MakeUpper();

        file_info* finfo = FindFileInfo(&m_file_dictionary, filename);
        if (!finfo)
        {
            finfo = m_file_dictionary.GetAt(0);
        }

        output_map_id = finfo->write_res_index++;
    }

    memset(&header, 0, sizeof(GX_PIXELMAP_HEADER));

    header.gx_pixelmap_header_magic_number = GX_MAGIC_NUMBER;
    header.gx_pixelmap_header_index = output_map_id;
    header.gx_pixelmap_header_version_major = map->gx_pixelmap_version_major;
    header.gx_pixelmap_header_version_minor = map->gx_pixelmap_version_minor;
    header.gx_pixelmap_header_flags = map->gx_pixelmap_flags;
    header.gx_pixelmap_header_format = map->gx_pixelmap_format;
    header.gx_pixelmap_header_transparent_color = map->gx_pixelmap_transparent_color;
    header.gx_pixelmap_header_width = map->gx_pixelmap_width;
    header.gx_pixelmap_header_height = map->gx_pixelmap_height;

    if (project_lib_version() >= GX_VERSION_DUPLICATE_BINRES_DATA_FIX && map_id)
    {
        refer_theme = FindResourceReferenceTheme(info, theme_id);

        if (refer_theme >= 0)
        {
            header.gx_pixelmap_header_data_offset = GetPixelmapDataOffset(map_id, frame_id, refer_theme);

            // write pixelmap header
            WritePixelmapHeader(&header);

            return;
        }
    }

    int data_output_pos = m_written_size + GetPixelmapHeaderSize();
    int padding = 0;

    if (project_lib_version() >= GX_VERSION_BINRES_DATA_ALIGNMENT_FIX)
    {
        if (data_output_pos & 0x03)
        {
            // 32bit alignment
            padding = 4 - (data_output_pos & 0x03);
        }
    }
    else
    {
        switch (map->gx_pixelmap_format)
        {
        case GX_COLOR_FORMAT_24XRGB:
        case GX_COLOR_FORMAT_24BGRX:
        case GX_COLOR_FORMAT_32ARGB:
        case GX_COLOR_FORMAT_32RGBA:
        case GX_COLOR_FORMAT_32ABGR:
        case GX_COLOR_FORMAT_32BGRA:
            if (data_output_pos & 0x03)
            {
                // 32bit alignment
                padding = (data_output_pos & 0x03);
            }
            break;
        case GX_COLOR_FORMAT_565RGB:
        case GX_COLOR_FORMAT_565BGR:
        case GX_COLOR_FORMAT_4444ARGB:
        case GX_COLOR_FORMAT_4444BGRA:
        case GX_COLOR_FORMAT_5551BGRX:
        case GX_COLOR_FORMAT_1555XRGB:
            if (data_output_pos & 0x01)
            {
                // 16bit alignment
                padding = (data_output_pos & 0x01);
            }
            break;

        default:
            // Do nothing
            break;
        }
    }

    if (info->raw)
    {
        //write raw image
        CString path;
        UCHAR buffer[1024];
        long file_size;
        long total_bytes_written;
        int chunk_size;

        path = MakeAbsolutePathname(info->pathinfo);
        FILE *file = _tfopen(path.GetBuffer(), _T("rb"));

        if (!file)
        {
            return;
        }

        fseek(file, 0, SEEK_END);
        file_size = ftell(file);

        header.gx_pixelmap_header_flags |= GX_PIXELMAP_RAW_FORMAT;
        header.gx_pixelmap_header_map_size = file_size;
        header.gx_pixelmap_header_aux_data_size = 0;
        header.gx_pixelmap_header_data_size = file_size;

        // write pixelmap header
        WritePixelmapHeader(&header);

        // Padding for alignment
        if (padding)
        {
            WriteDataOut(filling_string, padding);
        }

        fseek(file, 0, SEEK_SET);

        total_bytes_written = 0;

        while (total_bytes_written < file_size)
        {
            chunk_size = fread(buffer, 1, 1024, file);

            WriteDataOut(buffer, chunk_size);
            total_bytes_written += chunk_size;
        }

        fclose(file);
    }
    else
    {
        GX_PIXELMAP* rotated_map = GX_NULL;

        if (IsRotatedResourceSupported(m_project, m_display))
        {
            rotated_map = m_rotated_pixelmaps[theme_id].GetAt(output_map_id - 1);

            if (!rotated_map)
            {
                rotated_map = RotatePixelmap(info, theme_id, map, frame_id);
                m_rotated_pixelmaps[theme_id].SetAt(output_map_id - 1, rotated_map);
            }
            map = rotated_map;
        }

        header.gx_pixelmap_header_flags = map->gx_pixelmap_flags;
        header.gx_pixelmap_header_map_size = map->gx_pixelmap_data_size;
        header.gx_pixelmap_header_aux_data_size = map->gx_pixelmap_aux_data_size;
        header.gx_pixelmap_header_data_size = map->gx_pixelmap_data_size + map->gx_pixelmap_aux_data_size;

        // write pixelmap header
        WritePixelmapHeader(&header);

        // write image as guix format
        if (map->gx_pixelmap_data)
        {
            // Padding for alignment
            if (padding)
            {
                WriteDataOut(filling_string, padding);
            }

            // Write pixelmap data
            if (m_big_endian)
            {
                switch (map->gx_pixelmap_format)
                {
                case GX_COLOR_FORMAT_24XRGB:
                case GX_COLOR_FORMAT_24BGRX:
                case GX_COLOR_FORMAT_32ARGB:
                case GX_COLOR_FORMAT_32RGBA:
                case GX_COLOR_FORMAT_32ABGR:
                case GX_COLOR_FORMAT_32BGRA:
                    WriteBigEndianUintData(map);
                    break;
                case GX_COLOR_FORMAT_565RGB:
                case GX_COLOR_FORMAT_565BGR:
                case GX_COLOR_FORMAT_4444ARGB:
                case GX_COLOR_FORMAT_4444BGRA:
                case GX_COLOR_FORMAT_5551BGRX:
                case GX_COLOR_FORMAT_1555XRGB:
                    if (m_big_endian &&
                        (map->gx_pixelmap_flags & GX_PIXELMAP_COMPRESSED) &&
                        (map->gx_pixelmap_flags & GX_PIXELMAP_ALPHA))
                    {
                        WriteBigEndianCompressedUshortData(map);
                    }
                    else
                    {
                        WriteBigEndianUshortData(map);
                    }
                    break;

                case GX_COLOR_FORMAT_8BIT_PACKED_PIXEL:
                    if ((map->gx_pixelmap_flags & GX_PIXELMAP_COMPRESSED) &&
                        (map->gx_pixelmap_flags & GX_PIXELMAP_ALPHA))
                    {
                        WriteBigEndianUshortData(map);
                    }
                    else
                    {
                        WriteDataOut(map->gx_pixelmap_data, map->gx_pixelmap_data_size);
                    }
                    break;
                default:
                    WriteDataOut(map->gx_pixelmap_data, map->gx_pixelmap_data_size);
                    break;
                }
            }
            else
            {
                WriteDataOut(map->gx_pixelmap_data, map->gx_pixelmap_data_size);
            }
        }

        if (map->gx_pixelmap_aux_data)
        {
            WriteDataOut(map->gx_pixelmap_aux_data, map->gx_pixelmap_aux_data_size);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void binary_resource_gen::WriteStringBlock()
{
    string_table *pTable = m_project->mDisplays[m_display].stable;

    GX_STRING_HEADER header;
    header.gx_string_header_magic_number = GX_MAGIC_NUMBER;
    header.gx_string_header_language_count = GetEnabledLanguageCount();
    header.gx_string_header_string_count = pTable->CountGeneratedStrings();
    header.gx_string_header_data_size = GetStringDataSize();

    // Write string header
    WriteStringHeader(&header);

    BOOL enabled;
    CCommandInfo *pCmd = GetCmdInfo();

    // write string data
    for (int language_id = 0; language_id < m_project->mHeader.num_languages; language_id++)
    {
        if (pCmd->IsNoGui())
        {
            enabled = pCmd->IsLanguageEnabled(m_project->mHeader.languages[language_id].name);
        }
        else
        {
            enabled = m_project->mDisplays[m_display].gen_string_table[language_id];
        }

        if (enabled)
        {
            WriteStringData(language_id);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void binary_resource_gen::WriteStringData(USHORT language_id)
{
    string_table *pTable = m_project->mDisplays[m_display].stable;
    int num_strings = pTable->CountStrings();
    CString val;
    char end_of_string[] = "";

    GX_LANGUAGE_HEADER header;
    header.gx_language_header_magic_number = GX_MAGIC_NUMBER;
    header.gx_language_header_index = language_id;
    memset(header.gx_language_header_name, 0, GX_LANGUAGE_HEADER_NAME_SIZE);
    strcpy_s((char *)header.gx_language_header_name, GX_LANGUAGE_HEADER_NAME_SIZE,  CT2A(m_project->mHeader.languages[language_id].name));
    header.gx_language_header_data_size = GetStringDataSize(language_id);

    // Write language header
    WriteLanguageHeader(&header);

    CString id_name;
    GX_STRING utf8str;
    CArray<widget_info*> *info_list;
    widget_info* info;
    int reference_count;

    // Write strings
    for (int string_id = 1; string_id < num_strings; string_id++)
    {
        id_name = pTable->GetResourceIdName(string_id);
        val = pTable->GetString(id_name, language_id);

        if (val.IsEmpty())
        {
            utf8str.gx_string_ptr = end_of_string;
            utf8str.gx_string_length = 0;
            WriteOneStringData(&utf8str);
        }
        else
        {

            info_list = pTable->GetMLViewReferenceWidgetInfoList(id_name);

            if (info_list)
            {
                reference_count = info_list->GetCount();
            }
            else
            {
                reference_count = 1;
            }

            info = NULL;

            for (int index = 0; index < reference_count; index++)
            {
                if (info_list)
                {
                    info = info_list->GetAt(index);
                }

                MakeUtf8String(m_project, val, language_id, &utf8str, m_display, TRUE, info);
                WriteOneStringData(&utf8str);
                delete[] utf8str.gx_string_ptr;
                utf8str.gx_string_length = 0;
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void binary_resource_gen::WriteOneStringData(GX_STRING *string)
{
    USHORT  write_ushort;

    if (project_lib_version() >= GX_VERSION_STRING_LENGTH_FIX)
    {
        // Write out string length
        write_ushort = string->gx_string_length;
        write_ushort = SwapUshort(write_ushort);
        WriteDataOut(&write_ushort, sizeof(USHORT));
    }

    // Write out string
    WriteDataOut(string->gx_string_ptr, string->gx_string_length + 1);
}

///////////////////////////////////////////////////////////////////////////////
USHORT binary_resource_gen::GetEnabledThemeCount()
{
    if (!m_project)
    {
        return 0;
    }

    USHORT count = 0;
    BOOL enabled;
    CString theme_name;

    CCommandInfo *pCmd = GetCmdInfo();

    for (int theme = 0; theme < m_project->mDisplays[m_display].num_themes; theme++)
    {
        if (pCmd->IsNoGui())
        {
            theme_name = m_project->mDisplays[m_display].themes[theme].theme_name;
            enabled = pCmd->IsThemeEnabled(theme_name);
        }
        else
        {
            enabled = m_project->mDisplays[m_display].themes[theme].enabled;
        }
        
        if (enabled)
        {
            count++;
        }
    }

    return count;
}

///////////////////////////////////////////////////////////////////////////////
USHORT binary_resource_gen::GetEnabledLanguageCount()
{
    if (!m_project)
    {
        return 0;
    }

    USHORT count = 0;
    BOOL enabled;
    CCommandInfo *pCmd = GetCmdInfo();
    CString lang_name;

    for (int language = 0; language < m_project->mHeader.num_languages; language++)
    {
        if (pCmd->IsNoGui())
        {
            lang_name = m_project->mHeader.languages[language].name;
            enabled = pCmd->IsLanguageEnabled(lang_name);
        }
        else
        {
            enabled = m_project->mDisplays[m_display].gen_string_table[language];
        }

        if (enabled)
        {
            count++;
        }
    }

    return count;
}

///////////////////////////////////////////////////////////////////////////////
USHORT binary_resource_gen::GetColorCount(USHORT theme_id)
{
    int color_count = 0;

    CCommandInfo *pCmd = GetCmdInfo();

    if (pCmd->IsNoGui() || m_project->mDisplays[m_display].themes[theme_id].gen_color_table)
    {
        color_count = m_project->CountResources(m_display, RES_TYPE_COLOR);
    }

    return color_count;
}

///////////////////////////////////////////////////////////////////////////////
USHORT binary_resource_gen::GetFontCount(USHORT theme_id)
{
    int font_count = 0;

    CCommandInfo *pCmd = GetCmdInfo();

    if (pCmd->IsNoGui() || m_project->mDisplays[m_display].themes[theme_id].gen_font_table)
    {
        font_count = m_project->CountResources(m_display, RES_TYPE_FONT);
    }
    return font_count;
}

///////////////////////////////////////////////////////////////////////////////
USHORT binary_resource_gen::GetPageCount(res_info *info, USHORT theme_id, USHORT font_id)
{
    const GX_FONT *font_page;
    GX_FONT *head_page;
    int page_index;

    if (info->is_default && info->pathinfo.pathname.IsEmpty() &&
        !IsRotatedResourceSupported(m_project, m_display))
    {
        return 0;
    }

    head_page = GetPageHead(info, theme_id, font_id);

    //write font data
    font_page = head_page;
    page_index = 0;
    while (font_page)
    {
        page_index++;
        font_page = font_page->gx_font_next_page;
    }

    return page_index;
}

///////////////////////////////////////////////////////////////////////////////
USHORT binary_resource_gen::GetPixelmapCount(USHORT theme_id)
{
    int pixlemap_count = 0;

    CCommandInfo *pCmd = GetCmdInfo();

    if (pCmd->IsNoGui() || m_project->mDisplays[m_display].themes[theme_id].gen_pixelmap_table)
    {
        INT pixelmap_id;

        for (pixelmap_id = 1; pixelmap_id < m_pixelmap_dictionary.GetCount(); pixelmap_id++)
        {
            res_info *info = m_project->FindResource(m_display, theme_id, RES_TYPE_PIXELMAP, m_pixelmap_dictionary.GetAt(pixelmap_id));

            if (info)
            {
                if ((info->enabled) || (IsSystemPixelmap(pixelmap_id)))
                {
                    pixlemap_count += info->GetPixelmapFrameCount();
                }
            }
        }
    }

    return pixlemap_count;
}

///////////////////////////////////////////////////////////////////////////////
INT  binary_resource_gen::GetFontHeaderSize()
{
    if (project_lib_version() >= GX_VERSION_DUPLICATE_BINRES_DATA_FIX)
    {
        // new member "data_offset" is added
        return GX_FONT_HEADER_SIZE;
    }
    else
    {
        return GX_FONT_HEADER_SIZE - sizeof(ULONG);
    }
}

///////////////////////////////////////////////////////////////////////////////
INT  binary_resource_gen::GetGlyphHeaderSize()
{
    if (project_lib_version() >= GX_VERSION_BINRES_FONT_ALIGNMENT_FIX)
    {
        return GX_GLYPH_HEADER_SIZE;
    }
    else
    {
        return GX_GLYPH_HEADER_SIZE - sizeof(ULONG);
    }
}

///////////////////////////////////////////////////////////////////////////////
INT  binary_resource_gen::GetKerningGlyphHeaderSize()
{
    if (project_lib_version() >= GX_VERSION_BINRES_FONT_ALIGNMENT_FIX)
    {
        return GX_KERNING_GLYPH_HEADER_SIZE;
    }
    else
    {
        return GX_KERNING_GLYPH_HEADER_SIZE - sizeof(ULONG);
    }
}

///////////////////////////////////////////////////////////////////////////////
INT  binary_resource_gen::GetPixelmapHeaderSize()
{
    if (project_lib_version() >= GX_VERSION_DUPLICATE_BINRES_DATA_FIX)
    {
        // new member "data_offset" is added
        return GX_PIXELMAP_HEADER_SIZE;
    }
    else
    {
        return GX_PIXELMAP_HEADER_SIZE - sizeof(ULONG);
    }
}

///////////////////////////////////////////////////////////////////////////////
ULONG binary_resource_gen::GetThemeDataSize(SHORT theme_count)
{
    ULONG size = 0;
    ULONG data_size = 0;
    ULONG written_size = GetResourceHeaderSize();

    CCommandInfo *pCmd = GetCmdInfo();
    BOOL enabled;

    if (theme_count < 0)
    {
        theme_count = m_project->mDisplays[m_display].num_themes;
    }

    for (int theme = 0; theme < theme_count; theme++)
    {
        if (pCmd->IsNoGui())
        {
            enabled = pCmd->IsThemeEnabled(m_project->mDisplays[m_display].themes[theme].theme_name);
        }
        else
        {
            enabled = m_project->mDisplays[m_display].themes[theme].enabled;
        }

        if (enabled)
        {
            size = GetThemeHeaderSize();
            written_size += size;
            data_size += size;

            size = GetThemeDataSize(written_size, theme);
            written_size += size;
            data_size += size;
        }
    }

    return data_size;
}

///////////////////////////////////////////////////////////////////////////////
ULONG binary_resource_gen::GetThemeDataSize(ULONG written_size, USHORT theme_id)
{
    int theme_data_size = 0;
    int data_size;

    //Calcualte color data size
    theme_data_size += GetColorBlockSize(theme_id);

    //Calcualte palette table size
    theme_data_size += GetPaletteBlockSize(theme_id);

    written_size += theme_data_size;

    //Calculate font data size
    data_size = GetFontBlockSize(written_size, theme_id, -1);
    written_size += data_size;
    theme_data_size += data_size;

    //Calculate pixelmap data size
    theme_data_size += GetPixelmapBlockSize(written_size, theme_id, -1, -1);

    return theme_data_size;
}

///////////////////////////////////////////////////////////////////////////////
ULONG binary_resource_gen::GetPaletteBlockSize(USHORT theme_id)
{
    ULONG data_size = 0;

    if (m_project->mDisplays[m_display].themes[theme_id].palette)
    {
        INT color_count = m_project->mDisplays[m_display].themes[theme_id].palette_total_size;

        if (color_count)
        {
            data_size += GetPaletteHeaderSize();
            data_size += color_count * sizeof(GX_COLOR);
        }
    }

    return data_size;
}

///////////////////////////////////////////////////////////////////////////////
ULONG binary_resource_gen::GetColorBlockSize(USHORT theme_id)
{
    ULONG data_size = 0;

    CCommandInfo *pCmd = GetCmdInfo();

    if (pCmd->IsNoGui() || m_project->mDisplays[m_display].themes[theme_id].gen_color_table)
    {
        INT color_count = GetColorCount(theme_id);

        if (color_count)
        {
            data_size += GetColorHeaderSize();
            data_size += color_count * sizeof(GX_COLOR);
        }
    }

    return data_size;
}

///////////////////////////////////////////////////////////////////////////////
ULONG binary_resource_gen::GetFontBlockSize(ULONG written_size, USHORT theme_id, SHORT font_count)
{
    ULONG data_size = 0;
    ULONG size;
    int reference_theme = -1;

    CCommandInfo *pCmd = GetCmdInfo();

    if (font_count < 0)
    {
        font_count = m_project->CountResources(m_display, RES_TYPE_FONT);
    }

    if (pCmd->IsNoGui() || m_project->mDisplays[m_display].themes[theme_id].gen_font_table)
    {
        int font_id = 0;

        for (font_id = 0; font_id < font_count; font_id++)
        {
            res_info *info = m_project->FindResource(m_display, theme_id, RES_TYPE_FONT, font_id);

            if (info)
            {
                if (project_lib_version() >= GX_VERSION_DUPLICATE_BINRES_DATA_FIX)
                {
                    reference_theme = FindResourceReferenceTheme(info, theme_id);
                }

                data_size += GetFontHeaderSize();
                written_size += GetFontHeaderSize();

                /* This font is duplicate with the font in "reference_theme",
                    don't need to generate the same font data. */
                if (reference_theme < 0)
                {
                    size = GetFontDataSize(written_size, info, theme_id, font_id);
                    data_size += size;
                    written_size += size;
                }
            }
        }
    }

    return data_size;
}

///////////////////////////////////////////////////////////////////////////////
ULONG binary_resource_gen::GetOnePixelmapDataSize(ULONG written_size, res_info *info, USHORT theme_id, INT frame_id, USHORT outmap_id)
{
    ULONG data_size = 0;
    INT padding;

    GX_PIXELMAP *map = info->GetPixelmap(frame_id);

    if (project_lib_version() >= GX_VERSION_BINRES_DATA_ALIGNMENT_FIX)
    {
        if (written_size & 0x03)
        {
            padding = (4 - (written_size & 0x03));
            data_size += padding;
            written_size += padding;
        }
    }
    else
    {

        switch (map->gx_pixelmap_format)
        {
        case GX_COLOR_FORMAT_24XRGB:
        case GX_COLOR_FORMAT_24BGRX:
        case GX_COLOR_FORMAT_32ARGB:
        case GX_COLOR_FORMAT_32RGBA:
        case GX_COLOR_FORMAT_32ABGR:
        case GX_COLOR_FORMAT_32BGRA:
            if (written_size & 0x03)
            {
                data_size += written_size & 0x03;
                written_size += written_size & 0x03;
            }
            break;
        case GX_COLOR_FORMAT_565RGB:
        case GX_COLOR_FORMAT_565BGR:
        case GX_COLOR_FORMAT_4444ARGB:
        case GX_COLOR_FORMAT_4444BGRA:
        case GX_COLOR_FORMAT_5551BGRX:
        case GX_COLOR_FORMAT_1555XRGB:
            if (written_size & 0x01)
            {
                //16bit alignment
                data_size += written_size & 0x01;
                written_size += written_size & 0x01;
            }
        }
    }

    if (info->raw)
    {
        CString path = MakeAbsolutePathname(info->pathinfo);
        FILE* file = _tfopen(path.GetBuffer(), _T("rb"));

        if (!file)
        {
            return 0;
        }

        fseek(file, 0, SEEK_END);
        data_size += ftell(file);
        written_size += ftell(file);
    }
    else
    {
        GX_PIXELMAP* rotated_map = GX_NULL;

        if (IsRotatedResourceSupported(m_project, m_display))
        {
            rotated_map = m_rotated_pixelmaps[theme_id].GetAt(outmap_id - 1);

            if (!rotated_map)
            {
                rotated_map = RotatePixelmap(info, theme_id, map, frame_id);
                m_rotated_pixelmaps[theme_id].SetAt(outmap_id - 1, rotated_map);
            }

            map = rotated_map;
        }

        data_size += map->gx_pixelmap_data_size;
        data_size += map->gx_pixelmap_aux_data_size;
        written_size += map->gx_pixelmap_data_size;
        written_size += map->gx_pixelmap_aux_data_size;
    }

    return data_size;
}

///////////////////////////////////////////////////////////////////////////////
ULONG binary_resource_gen::GetPixelmapBlockSize(ULONG written_size, USHORT theme_id, SHORT map_id, SHORT frame_id)
{
    ULONG data_size = 0;
    ULONG size;
    int reference_theme = -1;
    int frame_count;
    int index;

    CCommandInfo *pCmd = GetCmdInfo();

    if (map_id < 0)
    {
        map_id = m_pixelmap_dictionary.GetCount() - 1;
    }

    if (pCmd->IsNoGui() || m_project->mDisplays[m_display].themes[theme_id].gen_pixelmap_table)
    {
        int pixelmap_id;
        int output_pixelmap_id = 1;

        for (pixelmap_id = 1; pixelmap_id <= map_id; pixelmap_id++)
        {
            res_info *info = m_project->FindResource(m_display, theme_id, RES_TYPE_PIXELMAP, m_pixelmap_dictionary.GetAt(pixelmap_id));

            if (info && info->GetPixelmapFrameCount() && (info->enabled) && (info->parent->enabled))
            {
                if (project_lib_version() >= GX_VERSION_DUPLICATE_BINRES_DATA_FIX)
                {
                    reference_theme = FindResourceReferenceTheme(info, theme_id);
                }

                if ((pixelmap_id < map_id) || (frame_id < 0))
                {
                    frame_count = info->GetPixelmapFrameCount();
                }
                else
                {
                    frame_count = frame_id;
                }

                for (index = 0; index < frame_count; index++)
                {
                    data_size += GetPixelmapHeaderSize();
                    written_size += GetPixelmapHeaderSize();

                    if (reference_theme < 0)
                    {
                        size = GetOnePixelmapDataSize(written_size, info, theme_id, index, output_pixelmap_id);
                        data_size += size;
                        written_size += size;
                    }
                    output_pixelmap_id++;
                }
            }
            else if(IsSystemPixelmap(pixelmap_id))
            {
                output_pixelmap_id++;
            }
        }
    }

    return data_size;
}

///////////////////////////////////////////////////////////////////////////////
void binary_resource_gen::InitFontSizeStorage(int display)
{
    int font_count = m_project->CountResources(display, RES_TYPE_FONT);
    for (int index = 0; index < m_project->mDisplays[display].num_themes; index++)
    {
        m_font_data_size[index] = new INT[font_count];
        memset(m_font_data_size[index], 0, sizeof(INT) * font_count);
    }
}

///////////////////////////////////////////////////////////////////////////////
void binary_resource_gen::DeleteFontSizeStorage(int display)
{
    for (int index = 0; index < m_project->mDisplays[display].num_themes; index++)
    {
        delete m_font_data_size[index];
    }
}

///////////////////////////////////////////////////////////////////////////////
ULONG binary_resource_gen::GetFontDataSize(ULONG written_size, res_info *info, USHORT theme_id, USHORT font_id)
{
    if (m_font_data_size[theme_id][font_id])
    {
        return m_font_data_size[theme_id][font_id];
    }

    if (info->is_default && info->pathinfo.pathname.IsEmpty() &&
        !IsRotatedResourceSupported(m_project, m_display))
    {
        // Default font.
        return 0;
    }

    ULONG data_size = 0;
    ULONG size;
    GX_FONT* head_page = GetPageHead(info, theme_id, font_id);

    const GX_FONT *font_page = head_page;
    while (font_page)
    {
        data_size += GetPageHeaderSize();
        written_size += GetPageHeaderSize();
        
        size = GetPageDataSize(written_size, font_page);
        data_size += size;
        written_size += size;

        font_page = font_page->gx_font_next_page;
    }

    m_font_data_size[theme_id][font_id] = data_size;

    return data_size;
}

///////////////////////////////////////////////////////////////////////////////
ULONG binary_resource_gen::GetFontDataOffset(USHORT font_id, USHORT theme_id)
{
    ULONG data_offset;

    data_offset = GetResourceHeaderSize();
    data_offset += GetThemeDataSize(theme_id);

    data_offset += GetThemeHeaderSize();

    //Calculate color block size
    data_offset += GetColorBlockSize(theme_id);

    //Calcualte palette table size
    data_offset += GetPaletteBlockSize(theme_id);

    if (font_id)
    {
        data_offset += GetFontBlockSize(data_offset, theme_id, font_id);
    }

    return data_offset;
}

///////////////////////////////////////////////////////////////////////////////
ULONG binary_resource_gen::GetPixelmapDataOffset(USHORT map_id, USHORT frame_id, USHORT theme_id)
{
    ULONG data_offset;

    data_offset = GetResourceHeaderSize();
    data_offset += GetThemeDataSize(theme_id);

    data_offset += GetThemeHeaderSize();

    //Calculate color block size
    data_offset += GetColorBlockSize(theme_id);

    //Calcualte palette block size
    data_offset += GetPaletteBlockSize(theme_id);

    // Calcualte font block size
    data_offset += GetFontBlockSize(data_offset, theme_id, -1);

    if (map_id > 1)
    {
        data_offset += GetPixelmapBlockSize(data_offset, theme_id, map_id, frame_id);
    }

    return data_offset;
}

///////////////////////////////////////////////////////////////////////////////
ULONG binary_resource_gen::GetPageDataSize(ULONG written_size, const GX_FONT *font)
{
    if (!font)
    {
        return 0;
    }

    GX_CHAR_CODE charval;
    ULONG data_size = 0;
    ULONG size;

    if (font -> gx_font_format & GX_FONT_FORMAT_KERNING)
    {
        for (charval = font->gx_font_first_glyph; charval <= font->gx_font_last_glyph; charval++)
        {
            data_size += GetKerningGlyphHeaderSize();
            written_size += GetKerningGlyphHeaderSize();

            size = GetGlyphDataSize(written_size, font, charval - font->gx_font_first_glyph);
            data_size += size;
            written_size += size;
            
            size = GetGlyphKerningTableSize(font, charval - font->gx_font_first_glyph);
            data_size += size;
            written_size += size;
        }
    }
    else
    {
        for (charval = font->gx_font_first_glyph; charval <= font->gx_font_last_glyph; charval++)
        {
            data_size += GetGlyphHeaderSize();
            written_size += GetGlyphHeaderSize();

            size = GetGlyphDataSize(written_size, font, charval - font->gx_font_first_glyph);
            data_size += size;
            written_size += size;
        }
    }

    return data_size;
}

///////////////////////////////////////////////////////////////////////////////
USHORT binary_resource_gen::GetGlyphKerningTableSize(const GX_FONT *font, INT glyph_index)
{
    USHORT table_size = 0;
    studiox_project *pProject = GetOpenProject();

    if (pProject)
    {
        GX_CONST GX_KERNING_GLYPH *glyph = &font->gx_font_glyphs.gx_font_kerning_glyphs[glyph_index];
        if (glyph->gx_kerning_table)
        {
            INT pair_counts = *(glyph->gx_kerning_table);
            table_size = 1 + pair_counts * (sizeof(GX_UBYTE) + sizeof(GX_CHAR));
        }
    }

    return table_size;
}

///////////////////////////////////////////////////////////////////////////////
ULONG binary_resource_gen::GetGlyphDataSize(ULONG written_size, const GX_FONT *font, INT glyph_index)
{
    ULONG data_size = 0;
    studiox_project *pProject = GetOpenProject();

    if (pProject)
    {
        if (font->gx_font_format & GX_FONT_FORMAT_COMPRESSED)
        {
            GX_CONST GX_COMPRESSED_GLYPH *glyph = &font->gx_font_glyphs.gx_font_compressed_glyphs[glyph_index];

            if (glyph->gx_glyph_map)
            {
                if (IsRotatedResourceSupported(m_project, m_display))
                {
                    UCHAR* rotated_data;
                    int mapsize;
                    RotateGlyphData((GX_GLYPH *)glyph, font->gx_font_format, &rotated_data, &mapsize);
                    if (rotated_data)
                    {
                        delete rotated_data;
                    }
                    data_size = mapsize & 0x7fff;
                }
                else
                {
                    data_size = glyph->gx_glyph_map_size & 0x7fff;
                }
            }
        }
        else
        {
            GX_GLYPH* glyph;
            if (font->gx_font_format & GX_FONT_FORMAT_KERNING)
            {
                glyph = (GX_GLYPH *) &font->gx_font_glyphs.gx_font_kerning_glyphs[glyph_index];
            }
            else
            {
                glyph = (GX_GLYPH*) &(font->gx_font_glyphs.gx_font_normal_glyphs[glyph_index]);
            }

            if (glyph->gx_glyph_map)
            {
                if (IsRotatedResourceSupported(m_project, m_display))
                {
                    data_size = GetRowPitch(glyph->gx_glyph_height, GetFontBits(font->gx_font_format));
                    data_size *= glyph->gx_glyph_width;
                }
                else
                {
                    int pitch = GetRowPitch(glyph->gx_glyph_width, GetFontBits(font->gx_font_format));
                    data_size = pitch * glyph->gx_glyph_height;
                }
            }
        }

        if (data_size &&
            (project_lib_version() >= GX_VERSION_BINRES_FONT_ALIGNMENT_FIX) &&
            (written_size & 0x03))
        {
            data_size += (4 - (written_size & 0x03));
        }
    }

    return data_size;
}

///////////////////////////////////////////////////////////////////////////////
ULONG binary_resource_gen::GetStringDataSize()
{
    ULONG data_size = 0;

    CCommandInfo *pCmd = GetCmdInfo();
    BOOL enabled;

    data_size += GetStringHeaderSize();
    for (int language = 0; language < m_project->mHeader.num_languages; language++)
    {
        if (pCmd->IsNoGui())
        {
            enabled = pCmd->IsLanguageEnabled(m_project->mHeader.languages[language].name);
        }
        else
        {
            enabled = m_project->mDisplays[m_display].gen_string_table[language];
        }

        if (enabled)
        {
            data_size += GetLanguageHeaderSize();
            data_size += GetStringDataSize(language);
        }
    }

    return data_size;
}

///////////////////////////////////////////////////////////////////////////////
ULONG binary_resource_gen::GetStringDataSize(USHORT language_id)
{
    ULONG string_data_size = 0;

    string_table *pTable = m_project->mDisplays[m_display].stable;
    int num_strings = pTable->CountStrings();
    CString val;
    CString id_name;
    GX_STRING string;
    CArray<widget_info*>* info_list;
    widget_info* info;
    int reference_count;

    // calcualte string data size
    for (int string_id = 1; string_id < num_strings; string_id++)
    {
        id_name = pTable->GetResourceIdName(string_id);
        val = pTable->GetString(id_name, language_id);

        info_list = pTable->GetMLViewReferenceWidgetInfoList(id_name);

        if (info_list)
        {
            reference_count = info_list->GetCount();
        }
        else
        {
            reference_count = 1;
        }

        info = NULL;

        for(int index = 0; index < reference_count; index++)
        {
            if (info_list)
            {
                info = info_list->GetAt(index);
            }

            if (val.IsEmpty())
            {
                string.gx_string_ptr = GX_NULL;
                string.gx_string_length = 0;
            }
            else
            {
                MakeUtf8String(m_project, val, language_id, &string, m_display, TRUE, info);
            }

            if (project_lib_version() >= GX_VERSION_STRING_LENGTH_FIX)
            {
                //two bytes for string length
                string_data_size += sizeof(USHORT);
            }

            string_data_size += string.gx_string_length + 1;
            if (string.gx_string_ptr)
            {
                delete string.gx_string_ptr;
            }
        }
    }

    return string_data_size;
}

////////////////////////////////////////////////////////////////////////////////////////////////
void binary_resource_gen::MakeFileDictionary(void)
{
    res_info* info;
    CCommandInfo* pCmdInfo = GetCmdInfo();
    BOOL enabled;
    CString output_file;
    file_info* finfo;
    ULONG blocksize;

    finfo = new file_info();
    finfo->name = "";
    finfo->total_size = 0;
    finfo->write_res_index = 0;
    finfo->size_table.RemoveAll();
    m_file_dictionary.Add(finfo);

    ULONG written_size = 0;

    for (int theme = 0; theme < m_project->mDisplays[m_display].num_themes; theme++)
    {
        if (pCmdInfo->IsNoGui())
        {
            enabled = pCmdInfo->IsThemeEnabled(m_project->mDisplays[m_display].themes[theme].theme_name);
        }
        else
        {
            enabled = m_project->mDisplays[m_display].themes[theme].gen_pixelmap_table;
        }

        if (!enabled) continue;

        for (int font_id = 0; font_id < m_project->CountResources(m_display, RES_TYPE_FONT); font_id++)
        {
            res_info* info = m_project->FindResource(m_display, theme, RES_TYPE_FONT, font_id);

            if (IsResEnabled(info))
            {
                if (info->output_file_enabled)
                {
                    output_file = RemoveFileExtension(info->output_file);
                    output_file.MakeUpper();
                    finfo = FindFileInfo(&m_file_dictionary, output_file);

                    if (!finfo)
                    {
                        finfo = new file_info;
                        finfo->name = output_file;
                        finfo->total_size = 0;
                        finfo->write_res_index = 0;
                        finfo->size_table.RemoveAll();
                        m_file_dictionary.Add(finfo);
                    }
                }
                else
                {
                    finfo = m_file_dictionary.GetAt(0);
                }

                written_size = finfo->total_size;
                written_size += GetFontHeaderSize();
                blocksize = GetFontDataSize(written_size, info, theme, font_id);
                written_size += blocksize;
                blocksize += GetFontHeaderSize();

                finfo->size_table.Add(blocksize);
                finfo->total_size = written_size;
            }
        }

        GX_PIXELMAP *map;
        USHORT outmap_id = 1;

        for (int id = 1; id < m_pixelmap_dictionary.GetCount(); id++)
        {
            info = m_project->FindResource(m_display, theme, RES_TYPE_PIXELMAP, m_pixelmap_dictionary.GetAt(id));

            if (IsResEnabled(info) && info->GetPixelmapFrameCount())
            {
                for (int frame_id = 0; frame_id < info->GetPixelmapFrameCount(); frame_id++)
                {
                    map = info->GetPixelmap(frame_id);

                    if (!map)
                    {
                        continue;
                    }

                    if (info->output_file_enabled)
                    {
                        output_file = RemoveFileExtension(info->output_file);
                        output_file.MakeUpper();

                        finfo = FindFileInfo(&m_file_dictionary, output_file);

                        if (!finfo)
                        {

                            finfo = new file_info;
                            finfo->name = output_file;
                            finfo->total_size = 0;
                            finfo->write_res_index = 0;
                            finfo->size_table.RemoveAll();
                            m_file_dictionary.Add(finfo);
                        }
                    }
                    else
                    {
                        finfo = m_file_dictionary.GetAt(0);
                    }

                    written_size = finfo->total_size;
                    written_size += GetPixelmapHeaderSize();
                    blocksize = GetOnePixelmapDataSize(written_size, info, theme, frame_id, outmap_id);
                    written_size += blocksize;
                    blocksize += GetPixelmapHeaderSize();

                    finfo->size_table.Add(blocksize);
                    finfo->total_size = written_size;

                    outmap_id++;
                }
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////
void binary_resource_gen::DestroyFileDictionay()
{
    file_info* info;
    for (int index = 0; index < m_file_dictionary.GetCount(); index++)
    {
        info = m_file_dictionary.GetAt(index);
        delete info;
    }

    m_file_dictionary.RemoveAll();
}


///////////////////////////////////////////////////////////////////////////////
file_info *binary_resource_gen::FindFileInfo(CArray<file_info*>* file_dict, CString name)
{
    file_info* find;
    int index;

    for (index = 1; index < file_dict->GetCount(); index++)
    {
        find = file_dict->GetAt(index);

        if (find->name == name)
        {
            return find;
        }
    }

    return GX_NULL;
}