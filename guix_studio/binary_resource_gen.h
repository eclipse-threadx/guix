#pragma once

enum SREC_TYPE{
    SREC_TYPE_S0 = 1,//Header, 16bi "0000"
    SREC_TYPE_S1,//Data, 16-bit address
    SREC_TYPE_S2,//Data, 24-bit address
    SREC_TYPE_S3,//Data, 32-bit address
    SREC_TYPE_S5,//Count, 16-bit count
    SREC_TYPE_S6 //Count, 24-bit count
};

#define SREC_MAX_DATA_SIZE 32

typedef struct {
    CString name;
    CArray<ULONG> size_table;
    ULONG total_size;
    USHORT write_res_index;
}file_info;

class binary_resource_gen : public resource_gen
{
public:
    binary_resource_gen(studiox_project *proj, BOOL file_format);
    ~binary_resource_gen();

public:
    BOOL GenerateBinaryFile(int display);
    BOOL GenerateBinaryData();
    void InitResource();
    void DestroyResource();
    void WritePixelmapBlock(res_info* info, USHORT map_id, USHORT output_map_id, USHORT theme_id, int frame_id = 0);
    void WriteFontBlock(res_info *info, USHORT font_id, USHORT theme_id);
    void WriteStandaloneResHeader();
    void SetDisplay(int display) { m_display = display; };
    void MakeFileDictionary(void);
    void DestroyFileDictionay(void);
    file_info* FindFileInfo(CArray<file_info*>* file_dict, CString name);
    static INT  GetResourceHeaderSize(){ return GX_RESOURCE_HEADER_SIZE; };
    static INT  GetThemeHeaderSize(){ return GX_THEME_HEADER_SIZE; };
    static INT  GetColorHeaderSize(){ return GX_COLOR_HEADER_SIZE; };
    static INT  GetPaletteHeaderSize(){ return GX_PALETTE_HEADER_SIZE; };
    static INT  GetFontHeaderSize();
    static INT  GetPageHeaderSize(){ return GX_PAGE_HEADER_SIZE; };
    static INT  GetGlyphHeaderSize();
    static INT  GetKerningGlyphHeaderSize();
    static INT  GetPixelmapHeaderSize();
    static INT  GetStringHeaderSize(){ return GX_STRING_HEADER_SIZE; };
    static INT  GetLanguageHeaderSize(){ return GX_LANGUAGE_HEADER_SIZE; };

protected:
    ULONG GetThemeDataSize(SHORT theme_count);
    ULONG GetThemeDataSize(ULONG written_size, USHORT theme_id);
    ULONG GetColorBlockSize(USHORT theme_id);
    ULONG GetPaletteBlockSize(USHORT theme_id);
    ULONG GetFontBlockSize(ULONG written_size, USHORT theme_id, SHORT font_count);
    ULONG GetPixelmapBlockSize(ULONG written_size, USHORT theme_id, SHORT map_id, SHORT frame_id);
    ULONG GetOnePixelmapDataSize(ULONG written_size, res_info *info, USHORT theme_id, INT frame_id, USHORT outmap_id);
    ULONG GetFontDataSize(ULONG written_size, res_info *info, USHORT theme_id, USHORT font_id);
    ULONG GetFontDataOffset(USHORT font_id, USHORT theme_id);
    ULONG GetPixelmapDataOffset(USHORT map_id, USHORT frame_id, USHORT theme_id);
    ULONG GetGlyphDataSize(ULONG written_size, const GX_FONT *font, INT glyph_index);
    USHORT GetGlyphKerningTableSize(const GX_FONT *font, INT glyph_index);
    ULONG GetPageDataSize(ULONG written_size, const GX_FONT *font);
    ULONG GetStringDataSize();
    ULONG GetStringDataSize(USHORT language_id);
    USHORT GetEnabledThemeCount();
    USHORT GetEnabledLanguageCount();
    USHORT GetColorCount(USHORT theme_id);
    USHORT GetFontCount(USHORT theme_id);
    USHORT GetPageCount(res_info *info, USHORT theme_id, USHORT font_id);
    USHORT GetPixelmapCount(USHORT theme_id);

    void InitFontSizeStorage(int display);
    void DeleteFontSizeStorage(int display);

    void WriteResourceHeader();
    void WriteThemeHeader(GX_THEME_HEADER *header);
    void WriteColorHeader(GX_COLOR_HEADER *header);
    void WritePaletteHeader(GX_PALETTE_HEADER *header);
    void WriteFontHeader(GX_FONT_HEADER *header);
    void WritePageHeader(GX_PAGE_HEADER *header);
    void WriteGlyphHeader(GX_GLYPH_HEADER *header);
    void WriteKerningGlyphHeader(GX_KERNING_GLYPH_HEADER *header);
    void WritePixelmapHeader(GX_PIXELMAP_HEADER *header);
    void WriteStringHeader(GX_STRING_HEADER *header);
    void WriteLanguageHeader(GX_LANGUAGE_HEADER *header);

    USHORT SwapUshort(USHORT s);
    UINT  SwapUint(UINT l);

    void WriteThemeBlock(USHORT theme_id);
    void WriteColorBlock(int theme_index);
    void WritePaletteBlock(int theme_index);
    void WriteFontData(res_info *info, USHORT theme_id, USHORT font_id);
    void WritePageBlock(const GX_FONT *font, USHORT page_id);
    void WriteGlyphBock(const GX_FONT *font, USHORT glyph_id);
    void WriteStringBlock();
    void WriteStringData(USHORT language_id);
    void WriteOneStringData(GX_STRING *string);


    VOID WriteBigEndianUintData(GX_PIXELMAP *map);
    VOID WriteBigEndianUshortData(GX_PIXELMAP *map);
    VOID WriteBigEndianCompressedUshortData(GX_PIXELMAP *map);

    VOID WriteOneSRecord(INT type);
    VOID WriteOneSRecord();
    VOID WriteSRecordHeader();
    VOID WriteSRecordData(const void *data_ptr, INT data_size);
    VOID WriteSRecordEnd();

    VOID WriteDataOut(const void *lpBuf, UINT nCount);

    void InitRotatedPixelmaps();
    void DestroyRotatedPixelmaps();

private:
    ULONG m_written_start_address;
    BOOL m_file_format;
    INT  m_srec_record_count;
    ULONG m_srec_address;
    UCHAR m_srec_data[SREC_MAX_DATA_SIZE];
    INT  m_srec_data_size;
    INT *m_font_data_size[MAX_THEMES];

    CArray<GX_PIXELMAP *> m_rotated_pixelmaps[MAX_THEMES];
    CArray<file_info*> m_file_dictionary;
    INT m_output_resource_id;
};

