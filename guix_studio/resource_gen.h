
#ifndef _RESOURCE_GEN_
#define _RESOURCE_GEN_

#define ENABLE_RENESAS_DAVE2D_DISPLAY_ROTATION

class binary_resource_gen;

class resource_gen : public studio_source_writer {
    public:
        resource_gen(studiox_project *project);
        BOOL GenerateResourceFile(int display);
        void InitOptimizedFonts();
        void DestroyOptimizedFonts();
        void CloseOutputFiles();
        GX_FONT *GetPageHead(res_info* info, int theme_id, int font_id);

        static TCHAR** GetColorFormatTable(studiox_project *project);
        static TCHAR* GetColorFormatName(int color_format);
        static int GetColorFormatVal(CString name);
        static void MakeUtf8String(studiox_project* project, CString val, int language_index, GX_STRING *string, int display, BOOL gen, widget_info *info = NULL);
        static CString RichTextResIdName2ResId(studiox_project* project, int display, CString& val);
        static BOOL IsRotatedResourceSupported(studiox_project *project, int display);
        VOID RotateGlyphData(GX_CONST GX_GLYPH* glyph, GX_UBYTE font_format, GX_UBYTE** rotated_map, INT* rotated_map_size);
        GX_PIXELMAP* RotatePixelmap(res_info* pixinfo, int theme_id, GX_PIXELMAP *map, int frame_id);

    private:
        static bool GetMlTextDisplayInfo(widget_info* info, int *font_id, GX_VALUE* display_width);
        static void MakeReorderedBidiText(GX_STRING *utf8str, GX_STRING *out_string, int display, widget_info *info = NULL);
        static void MakeAdjustedThaiString(GX_STRING *utf8str, GX_STRING *out_string);
        BOOL IsStringReferencedbyMlView(widget_info* info, GX_RESOURCE_ID resource_id);

    private:
        BOOL  mUsingDefault8BitFont;
        BOOL  mUsingDefault4BitFont;
        BOOL  mUsingDefaultMonoFont;
        BOOL  mDefaultPaletteWritten;
        BOOL  mPrivatePaletteWritten;
        GX_PIXELMAP *m_map;
        CString      m_ThemeName;

        BOOL GenerateResourceData();

        void WriteDisplayDefines();
        void WriteThemeDefines();
        void WriteLanguageDefines();

        void WriteColorDefines(void);
        void WriteFontDefines(void);
        void WritePixelmapDefines(void);
        void WriteStringDefines(void);

        void WriteColorTable(void);
        void WritePalette(int theme);
        void WritePalette(int palette_size, GX_COLOR *palette,
            CString &pal_name);

        void WriteFontData(void);
        void WriteFontTable(void);
        void WritePixelmapStructure(res_info *info, int frame_id);
        void WriteRawPixelmap(res_info *info, int frame_id);
        void WritePixelmapData(void);
        void WritePixelmapTable(void);
        void WriteStringData(void);
        void WriteOneStringData(CString &display_name, int language_index, int string_id);

        void WriteStringTable(void);
        void WriteLanguageTable(CString display_name, INT language_count);
        void WriteLanguageTableExt(CString display_name, INT language_count);
        void WriteLanguageDirectionTable(CString display_name, INT language_count);
        void WriteOneStringTable(CString &display_name, CString &language_name, int language_index, string_table *stable, int num_strings);
        void WriteOneStringTableExt(CString &display_name, CString &language_name, int language_index, string_table* stable, int num_strings);
        void WriteScrollbarAppearance(GX_SCROLLBAR_APPEARANCE &appear);
        void WriteThemes(void);

        CString UpperProjectName();
        CString UpperDisplayName();
        CString UpperProjDisplayName();

        GX_PIXELMAP *ConvertPixelmapOutputFormat(res_info *info);

        GX_UBYTE* Rotate8bitGlyphData(GX_CONST GX_UBYTE* map, INT width, INT height);
        GX_UBYTE* Rotate4bitGlyphData(GX_CONST GX_UBYTE* map, INT width, INT height);
        GX_UBYTE* Rotate1bitGlyphData(GX_CONST GX_UBYTE* map, INT width, INT height);
        GX_UBYTE* RotateUIntData(GX_CONST GX_UBYTE* data, INT width, INT height);
        GX_UBYTE* RotateUShortData(GX_CONST GX_UBYTE* data, INT width, INT height);
        GX_UBYTE* RotateUCharData(GX_CONST GX_UBYTE* data, INT width, INT height);
        VOID RotatePixelmap(GX_PIXELMAP* map);

        void WriteFont(res_info *info, int theme_id, int font_id);
        void WriteFontPage(const GX_FONT *font, CString &name, int page, BOOL lastpage, BOOL custom_file_enabled);
        void WriteKerningTable(GX_UBYTE *kerning_table, CString &name, GX_CHAR_CODE charval);
        void WritePixelmapData(res_info *pixinfo, int theme_id, int frame_id);
        BOOL CheckLineFeed(CString &out);
        void WriteUintData(res_info *info, int frame_id);
        void WriteUshortData(res_info *info, int frame_id);
        void WriteCCRXCompatibleTargaStream(res_info *info, int frame_id);
        void WriteBigEndianCompressedUshortData(res_info *info, int frame_id);
        void WriteUcharData(res_info *info, int frame_id);
        void WriteUcharAuxData(res_info *info, int frame_id);
        void WriteUintAuxData(res_info *info, int frame_id);
        BOOL SetOutFile(CString filename, BOOL binary_mode = 0);
        
        VOID WriteErrorDirectives();

    protected:
        studiox_project *m_project;
        BOOL m_warn_on_error;
        int   m_display;
        int   m_num_displays;
        int   m_color_table_size;
        int   m_font_table_size;
        int   m_pixelmap_table_size;
        BOOL  m_big_endian;
        CArray<CString> m_pixelmap_dictionary;
        CArray<CFile *> outfile_list;
        CArray<GX_FONT *> m_optimized_fonts[MAX_THEMES];
        binary_resource_gen* mp_bin_generater;
        void CalculateTableSizes();
        BOOL GenerateResourceHeader();
        BOOL FontPageCompare(res_info *info_1, res_info* info_2);
        int  FindResourceReferenceTheme(res_info *info, int theme);
        CString GetResourceFileName();
        BOOL IsSystemPixelmap(INT pixelmap_id);
        BOOL IsResEnabled(res_info* info);
};

#endif
