
struct string_table_record
{
    string_table_record()
    {
        id_name = _T("");
        strings = NULL;
        notes = _T("");
        font_id = 0;
    }
    CString id_name;
    CString *strings;
    CString notes;
    int     font_id;
};

class widget_info;

struct string_reference_record
{
    CString id_name;
    CArray<widget_info *> reference_list;
};

class FontCharMap
{
    public:
        FontCharMap();
        ~FontCharMap();
        DWORD GetMapSize(){ return map_size; };
        void SetMapSize(DWORD size);
        void Clear();
        void Set(DWORD index);
        void Set(DWORD start, DWORD end);

        BOOL Test(DWORD index);
        BOOL Test(DWORD start, DWORD end);

        BOOL TestAndSet(DWORD index);
        BOOL AddToMap(CString &val);
        void Overlay(FontCharMap *other);
        BOOL IsEmpty();

    private:
        unsigned char *map;
        DWORD map_size;
};

class string_table {
    public:
        string_table();
        string_table(const string_table &other);     // copy constructor

        ~string_table();
        static BOOL IsRight2LeftLanguage(int language_index);
        static void EnableDisableRuntimeBidiText(int language_index);
        static BOOL TestGenerateLanguageDirectionTable();
        void Initialize(int num_languages, int num_strings);

        //import string from files
        void ImportString(CString &id,
            CString &src_string,
            CString &target_string,
            CString &notes,
            int src_table_column,
            int target_table_column);

        // this version is used by string table editor
        int AddString();

        // this version is used when reading project
        int AddString(CString &id_name, CString &str, bool update_guix = TRUE);

        // this version is used when user types in properties window
        int  CheckAddString(CString &str, CString base_name = _T(""));

        int FindStringIndex(CString &id_name);

        void AddLanguage(void);

        void RemoveString(CString &id_name);
        BOOL RemoveString(int string_index);
        string_table_record &GetRecord(int string_index);
        string_table_record &GetRecord(CString id_name);
        CString &GetString(CString &id_name, int language = 0);
        CString &GetString(int string_index, int language = 0);
        CString &GetStringId(int string_index);
        CString &GetNotes(int string_index);

        // assign id and string for given index and language
        void SetString(int string_index, CString &id_name, int language, CString &str, bool update_guix = TRUE);

        // find id, assign string for given language
        void SetString(CString &id_name, int language, CString &str, bool update_guix = TRUE);

        // assign only the string for index and language
        void SetString(int string_index, int language, CString &str, bool update_guix = TRUE);

        // sort string table by ID name
        void SortById();

        // sort string table by string
        void SortByString();

        // sort string table by reference count
        void SortByReferenceCount();

        void Sort();

        // assign only the string Id:
        BOOL SetStringId(int string_index, CString &id_name);

        void SetNotes(int string_index, CString &notes);

        // assign font used to calculate string width
        void SetDisplayFont(int string_index, int font_id);

        int CountStrings();
        int CountLanguages();
        
        void CreateId(CString &id_name, CString base_name = _T(""));

        void UpdateGuixLanguageTable(GX_DISPLAY *display = GX_NULL, BOOL update_resource_view = GX_TRUE);
        void SetActiveLanguage(int language);
        int GetActiveLanguage(void) {return m_active_language;}
        FontCharMap *GenerateCleanCharacterMap(void);
        FontCharMap *GetCharacterMap(void) {return &char_map;}
        static void DeleteGuixLanguageTable(GX_STRING **pOldTable, int language_count,  int string_count);
        static void DeleteGuixLanguageDirectionTable(GX_DISPLAY *display);
    
        void AddToDictionary(CString &id_name, int res_id = -1);
        void RemoveFromDictionary(CString &id_name);
        CString UpdateRichTextResourceName(CString& val, int res_type, CString old_id_name, CString new_id_name);
        void UpdateStringTableRichTextResourceName(int res_type, CString& old_id_name, CString& new_id_name);
        void UpdateDictionaryResourceName(CString &old_id_name, CString &new_id_name);
        CString GetResourceIdName(int res_id, widget_info *info = NULL);
        GX_RESOURCE_ID GetResourceId(CString &id_name);

        void SetSortColumn(int sort_column){ m_sort_column = sort_column; };
        int GetSortColumn(){ return m_sort_column; };

        inline void ClearStringTableCharMap() { char_map.Clear(); };

        void MakeMLViewReferenceRecord(int display);
        string_reference_record *GetMLViewReferenceRecord(CString &id_name);
        CArray<widget_info *> *GetMLViewReferenceWidgetInfoList(CString& id_name);
        int CountGeneratedStrings();

    private:
        bool MakeMLViewReferenceRecord(widget_info* child);
        void CleanUpMLViewRefereceRecord();
        
    private:
        void CleanupStringTable();

        FontCharMap char_map;
        int m_languages;
        int m_active_language;
        GX_CHAR ***language_table;
        int m_sort_column;

        CArray<string_table_record> m_records;
        CArray<CString> m_dictionary;
        CArray<string_reference_record *> m_reference_records;
};

