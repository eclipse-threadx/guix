
#ifndef _UNDO_LIST_HEADER_
#define _UNDO_LIST_HEADER_


#define MAX_UNDO_ENTRIES 40

enum undo_types {
    UNDO_TYPE_NONE = 0,
    UNDO_TYPE_POSITION,
    UNDO_TYPE_SIZE,
    UNDO_TYPE_STYLE,
    UNDO_TYPE_SLIDER_INFO,
    UNDO_TYPE_PROGRESS_BAR_INFO,
    UNDO_TYPE_RADIAL_PROGRES_BAR_INFO,
    UNDO_TYPE_LIST_ROWS,
    UNDO_TYPE_OPEN_HEIGHT,
    UNDO_TYPE_DYNAMIC_TEXT_BUFFER,
    UNDO_TYPE_TEXT_VIEW_LINE_SPACE,
    UNDO_TYPE_TEXT_VIEW_WHITESPACE,
    UNDO_TYPE_TEXT_BUFFER_SIZE,
    UNDO_TYPE_SCROLL_APPEARANCE,
    UNDO_TYPE_SCROLL_STYLE,
    UNDO_TYPE_INSERT_WIDGET,
    UNDO_TYPE_DELETE_WIDGET,
    UNDO_TYPE_COLOR,
    UNDO_TYPE_FONT,
    UNDO_TYPE_PIXELMAP,
    UNDO_TYPE_STRING,
    UNDO_TYPE_NAMES,
    UNDO_TYPE_USER_DATA,
    UNDO_TYPE_ALLOCATION,
    UNDO_TYPE_FOCUS,
    UNDO_TYPE_CIRCULAR_GAUGE_INFO,
    UNDO_TYPE_CHART_INFO,
    UNDO_TYPE_SCROLL_WHEEL_INFO,
    UNDO_TYPE_TEXT_SCROLL_WHEEL_INFO,
    UNDO_TYPE_STRING_SCROLL_WHEEL_INFO,
    UNDO_TYPE_NUMRIC_SCROLL_WHEEL_INFO,
    UNDO_TYPE_TEMPLATE,
    UNDO_TYPE_NUMERIC_PROMPT_INFO,
    UNDO_TYPE_MENU_INFO,
    UNDO_TYPE_TREE_VIEW_INFO,
    UNDO_TYPE_VISIBLE_AT_STARTUP,
    UNDO_TYPE_INSERT_FOLDER,
    UNDO_TYPE_DELETE_FOLDER,
    UNDO_TYPE_INSERT_TOP_LEVEL_WIDGETS,
    UNDO_TYPE_RADIAL_SLIDER_INFO
};


class undo_widget
{
    public:
        undo_widget() {
            parent_info = NULL;
            copy_info = NULL;
            res_index = 0;
            project_info = NULL;
            folder = NULL;
        }

        // pointer to the parent widget_info structure in project:
        widget_info  *parent_info;

        // a copy of the widget information, so that we can revert changes
        widget_info  *copy_info;

        // pointer to widget_info structure in the open project
        widget_info  *project_info;

        // pointer to folder_info structure (only used when folder is modified)
        folder_info  *folder;
        CString      folder_name; // folder belongs to
        int          res_index;
};


typedef struct
{
    int type;
    CArray<undo_widget> widget_list;
} undo_record;

class undo_manager {
    public:
        undo_manager();
        ~undo_manager();
        void AddEntry(int type, folder_info *folder, int display);
        void AddEntry(int type, widget_info *info, int res_index = 0, BOOL fold = FALSE);
        void AddEntry(int type, CArray<WIDGET_SELECT_ENTRY> &widget_list, BOOL fold = FALSE);
        void Undo();
        void Reset();
        int CountEntries() {return entries;}

    private:
        void ClearUndoRecord(undo_record *entry);        
        void LockAddEntry(){ is_add_entry_locked = TRUE; };
        void FreeAddEntry(){ is_add_entry_locked = FALSE; };
        BOOL IsAddEntryLocked(){ return is_add_entry_locked; };

        void UpdateUndoRecordInfoPointers(widget_info *new_info, widget_info *old_info);
        void         SearchAndReplaceInfoPointers(undo_record *search, widget_info **node_list, int node_list_size);
        widget_info **BuildInfoNodeArray(widget_info **write, widget_info *new_tree, widget_info *old_tree);
        int          CountInfoNodes(widget_info *start, int count);

        undo_record *tail;
        undo_record *current;
        undo_record undo_list[MAX_UNDO_ENTRIES];
        int entries;
        BOOL is_add_entry_locked;
};

#endif

