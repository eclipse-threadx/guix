#ifndef _COPY_PASTE_ENGINE_
#define _COPY_PASTE_ENGINE_

enum PASTE_CONFLICT_OPTIONS{
    PASTE_CONFLICT_OPTION_USE_SOURCE = 1,
    PASTE_CONFLICT_OPTION_USE_DESTINATION,
    PASTE_CONFLICT_OPTION_RENAME,
    PASTE_CONFLICT_OPTION_ABORT
};

typedef struct {
    CString source_id_name;
    CString translated_id_name;
} resource_trans_entry;

class copy_paste_info
{
public:
    copy_paste_info();
    ~copy_paste_info();
    folder_info *template_dependencies;
    folder_info *copied_widgets;
};

class copy_paste_engine
{
    public:
        copy_paste_engine();
        ~copy_paste_engine();

        void SetProject(studiox_project *project);

        BOOL CopyToClipboard(CArray<widget_info *> &info_list, HWND hWnd, folder_info *folder = NULL);
        BOOL CopyToClipboard(widget_info *info, HWND hWnd);
        BOOL CopyToClipboard(folder_info *folder, HWND hWnd);
        copy_paste_info *PasteFromClipboard(HWND hWnd, int Display);

    private:
        folder_info *ReadWidgets(xml_reader &reader, const char *tagname);
        void ClearResourceList();
        void BuildResourceList(widget_info *info, BOOL entry = FALSE);
        void BuildBaseList(widget_info *base);
        void CheckAddToList(CArray<int> &list, int res_id);
        void CheckAddToBaseList(widget_info *base);

        void ClearResourceTransList();
        void WriteColorDependencies(xml_writer &writer);
        void WriteFontDependencies(xml_writer &writer);
        void WritePixelmapDependencies(xml_writer &writer);
        void WriteStringDependencies(xml_writer &writer);

        BOOL ReadColorDependencies(xml_reader &reader);
        BOOL ReadFontDependencies(xml_reader &reader);
        BOOL ReadPixelmapDependencies(xml_reader &reader);
        BOOL ReadStringDependencies(xml_reader &reader);

        int TranslateId(CArray<resource_trans_entry> &list, int res_type, int res_id);

        void TranslateWidgetResourceIds(widget_info *info);

        int              mDisplay;
        studiox_project *mProject;

        // these arrays hold the resource ids that need to be written out:
        CArray<int> write_color_list;
        CArray<int> write_font_list;
        CArray<int> write_pixelmap_list;
        CArray<int> write_string_list;
        CArray<widget_info *> base_list;

        // These arrays hold the old-to-new resource id name mapping tables:
        CArray<resource_trans_entry> color_trans;
        CArray<resource_trans_entry> string_trans;
};
#endif