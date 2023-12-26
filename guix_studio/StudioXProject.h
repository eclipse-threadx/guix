
#ifndef _STUDIOXPROJECT_
#define _STUDIOXPROJECT_

#include <string>
#include "stdafx.h"

// Studio project (.gxp) file version. Update this when the XML
// schema changes, so that we can add code to handle previous versions correctly.
#define PROJECT_VERSION    56

// Sequential number of studio version
#define STUDIOX_VERSION_NUMBER 0

/* This enabled checking if the project was created by a newer version of Studio.
   Turn this off for testing when the Studio version number is not accurate
*/
#define ENABLE_STUDIO_VERSION_TEST

// Turn on this to build a 'Beta' release
//#define STUDIO_ENGINEERING_RELEASE

#define MAX_DISPLAYS 4
#define MAX_LANGUAGES 128
#define MAX_THEMES 8
#define THUMBNAIL_SIZE 32

#define NUM_FONT_CHAR_RANGES 46
#define NUM_FONT_EXTENDED_CHAR_RANGES 4
#define DEFAULT_THEME 0

#define STATICALLY_ALLOCATED        0
#define DYNAMIC_ALLOCATION_ROOT     1
#define DYNAMIC_ALLOCATION_CHILD    2

#define BINARY_FILE_FORMAT_SREC 0x01
#define BINARY_FILE_FORMAT_BIN  0x02
#define BINARY_FILE_FORMAT_BIN_STANDALONE 0x03

enum resource_item_types {
    RES_TYPE_HEADER = 1,
    RES_TYPE_GROUP,
    RES_TYPE_FOLDER,
    RES_TYPE_ADD_FONT,
    RES_TYPE_ADD_COLOR,
    RES_TYPE_ADD_PIXELMAP,
    RES_TYPE_ADD_STRING,
    RES_TYPE_FONT,
    RES_TYPE_COLOR,
    RES_TYPE_PIXELMAP,
    RES_TYPE_STRING,
};

enum resource_states {
    RES_STATE_UNDETERMINED = -1,
    RES_STATE_DISABLED = 0,
    RES_STATE_ENABLED = 1
};

enum HEADER_IDS{
    THEME_HEADER = 4096
};

enum FolderIds {
    DEFAULT_COLOR_FOLDER = 4096,
    CUSTOM_COLOR_FOLDER,
    DEFAULT_FONT_FOLDER,
    CUSTOM_FONT_FOLDER,
    DEFAULT_PIXELMAP_FOLDER,
    CUSTOM_PIXELMAP_FOLDER
};

enum GROUP_IDS {
    COLOR_GROUP = 4096,
    FONT_GROUP,
    PIXELMAP_GROUP,
    STRING_GROUP
};

enum PATHTYPES {
    PATH_TYPE_PROJECT_RELATIVE = 0,
    PATH_TYPE_INSTALL_RELATIVE,
    PATH_TYPE_ABSOLUTE
};

enum PALETTE_TYPES {
    PALETTE_TYPE_NONE,
    PALETTE_TYPE_PRIVATE,
    PALETTE_TYPE_SHARED
};

enum RESOURCE_CLICK_COMMANDS {
    CMD_NO_COMMAND = 0,
    CMD_ADD_RESOURCE,
    CMD_INC_LANGUAGE,
    CMD_DEC_LANGUAGE,
    CMD_INC_THEME,
    CMD_DEC_THEME
};

enum DECODER_TYPE
{
    DECODER_TYPE_NONE = 0,
    DECODER_TYPE_SW,
    DECODER_TYPE_HW
};

enum TreeNodeLevels {
    PROJECT_NODE_LEVEL = 0,
    DISPLAY_NODE_LEVEL,
    FOLDER_NODE_LEVEL,
    SCREEN_NODE_LEVEL
};

enum ID_TYPES{
    ID_TYPE_WIDGET = 0,
    ID_TYPE_ANIMATION
};

enum STRING_EXPORT_TYPES{
    STRING_EXPORT_TYPE_XLIFF = 1,
    STRING_EXPORT_TYPE_CSV
};

struct font_page_info {
    BOOL enabled;
    int  first_char;
    int  last_char;
};

class palette_info {
    public:
        palette_info()
        {
            memset(this, 0, sizeof(palette_info));
        }
        
        GX_COLOR *palette;
        int       total_size;
        int       used_size;
};

typedef struct {
    CString id_name;
    int     reference_count;
} id_info;

/***************************************************************************************/
// ** IMPORTANT NOTE TO SELF **
//
// If you modify the members of this class in any way, make SURE you initialize them
// in the default constructor, and even more important make SURE you copy the new 
// members in the copy constructur!!
/***************************************************************************************/
class res_info {
    public:
        res_info(int ResType = 0);
        res_info(const res_info *in_parent, const res_info &other, BOOL copy_next);  // copy constructor

        ~res_info();

        void Attach(res_info *pRes);
        void Detach();
        GX_PIXELMAP* GetPixelmap(int frame_id = 0);
        INT GetPixelmapDelayTime(int frame_id = 0);
        int GetPixelmapFrameCount() { return map_list.GetCount(); }

        int type;
        CString name;           // id name
        PATHINFO pathinfo;

        res_info *next;
        res_info *child;
        res_info *parent;

        CArray<GX_PIXELMAP *> map_list;
        CArray<INT> map_delay_list;
        GX_PIXELMAP *thumbnail;

        GX_FONT *font;
        ULONG   storage_size;
        BOOL    is_default;     // a default font or color id
        BOOL    enabled;
        int     folder_id;

        /* pixelmap fields */
        BOOL    compress;
        BOOL    keep_alpha;
        BOOL    dither;
        BOOL    raw;
        BOOL    output_file_enabled;
        CString output_file;
        BOOL    binary_mode;
        int     palette_type;       // global palette, shared palette, private palette
        int     output_color_format;

        /* color fields */
        GX_COLOR colorval;

        /* font fields */
        int     font_height;
        int     font_bits;
        BOOL    font_charset_include_string_table;
        BOOL    font_support_extended_unicode;
        BOOL    font_kerning;
        font_page_info *font_pages;
        int             font_pages_count;

        BOOL    is_modified; // mark whether the resource property is modified.
};

struct vertical_list_info {
    int total_rows;
    int seperation;
};

struct drop_list_info {
    int total_rows;
    int seperation;
    int open_height;
};

struct text_input_info {
    int     whitespace;
    int     line_space;
    int     buffer_size;
    BOOL    dynamic_buffer;
};

struct progress_info {
    int  min_val;
    int  max_val;
    int  current_val;
};

struct sprite_info {
    GX_SPRITE_FRAME *framelist;
    int              frame_count;
    BOOL             apply_to_all_frames;
};

struct gauge_info{
    GX_CIRCULAR_GAUGE_INFO info;
    int                    start_angle;
};

struct scroll_wheel_info {
    int             total_rows;
    int             row_height;
    int             selected_row;
    int             start_alpha;
    int             end_alpha;
};

struct string_scroll_wheel_info {
    scroll_wheel_info base;
    GX_RESOURCE_ID   *string_id_list;
};

struct numeric_scroll_wheel_info {
    scroll_wheel_info base;
    int               start_val;
    int               end_val;
};

struct menu_info{
    int text_x_offset;
    int text_y_offset;
    int list_total_count;
    BOOL insert_as_menu_item;
};

struct radial_slider_info{
    GX_VALUE xcenter;
    GX_VALUE ycenter;
    USHORT   radius;
    USHORT   track_width;
    GX_VALUE needle_offset;
    GX_VALUE current_angle;
    GX_VALUE min_angle;
    GX_VALUE max_angle;
    GX_RESOURCE_ID background_pixelmap;
    GX_RESOURCE_ID needle_pixelmap;
    USHORT animation_total_steps;
    USHORT animation_delay;
    USHORT animation_style;
};

#define NUM_WIDGET_COLORS    8
#define NUM_WIDGET_FONTS     4
#define NUM_WIDGET_PIXELMAPS 8
#define NUM_WIDGET_STRINGS   2

// indexes into pixelmap slider pixelmap array

#define LOWER_PIXELMAP_INDEX  0
#define UPPER_PIXELMAP_INDEX  1
#define NEEDLE_PIXELMAP_INDEX 2

#define WALLPAPER_PIXELMAP_INDEX 0

#define DROP_LIST_PIXELMAP_INDEX 1
#define SCROLL_WHEEL_PIXELMAP_INDEX 1

#define NORMAL_PIXELMAP_INDEX   0
#define SELECTED_PIXELMAP_INDEX 1
#define DISABLED_PIXELMAP_INDEX 2

#define UNCHECKED_PIXELMAP_INDEX          0
#define CHECKED_PIXELMAP_INDEX            1
#define UNCHECKED_DISABLED_PIXELMAP_INDEX 2
#define CHECKED_DISABLED_PIXELMAP_INDEX   3

#define RADIO_OFF_PIXELMAP_INDEX          0
#define RADIO_ON_PIXELMAP_INDEX           1
#define RADIO_OFF_DISABLED_PIXELMAP_INDEX 2
#define RADIO_ON_DISABLED_PIXELMAP_INDEX  3


#define LEFT_PIXELMAP_INDEX     0
#define FILL_PIXELMAP_INDEX     1
#define RIGHT_PIXELMAP_INDEX    2
#define SELECTED_LEFT_PIXELMAP_INDEX     3
#define SELECTED_FILL_PIXELMAP_INDEX     4
#define SELECTED_RIGHT_PIXELMAP_INDEX    5

#define SCROLL_FILL_PIXELMAP_INDEX  0
#define SCROLL_THUMB_PIXELMAP_INDEX 1
#define SCROLL_UP_PIXELMAP_INDEX    2
#define SCROLL_DOWN_PIXELMAP_INDEX  3

#define TREE_VIEW_EXPAND_PIXELMAP_INDEX   1
#define TREE_VIEW_COLLAPSE_PIXELMAP_INDEX 2

// indexes into widget info color array

#define NORMAL_FILL_COLOR_INDEX   0
#define SELECTED_FILL_COLOR_INDEX 1
#define DISABLED_FILL_COLOR_INDEX 2
#define NORMAL_TEXT_COLOR_INDEX   3
#define SELECTED_TEXT_COLOR_INDEX 4
#define DISABLED_TEXT_COLOR_INDEX 5
#define READONLY_FILL_COLOR_INDEX 6
#define READONLY_TEXT_COLOR_INDEX 7

#define TREE_VIEW_ROOT_LINE_COLOR_INDEX 2

#define NORMAL_FONT_INDEX         0
#define SELECTED_FONT_INDEX       1

#define RICH_TEXT_FONT_NORMAL      0
#define RICH_TEXT_FONT_BOLD        1
#define RICH_TEXT_FONT_ITALIC      2
#define RICH_TEXT_FONT_BOLD_ITALIC 3

// Forward references

class screen_flow;
class flow_item;
class trigger_info;

//#define SCROLL_THUMB_COLOR_INDEX 3

typedef union  {
            GX_SLIDER_INFO slider;
            GX_SCROLLBAR_APPEARANCE scroll;
            GX_RADIAL_PROGRESS_BAR_INFO radial_progress;
            radial_slider_info radial_slider;
            GX_LINE_CHART_INFO line_chart_info;
            progress_info progress;
            vertical_list_info vlist;
            drop_list_info     drop_list;
            text_input_info    text_info;
            sprite_info        sprite;
            gauge_info         gauge;
            INT                numeric_prompt_value;
            scroll_wheel_info  scroll_wheel;
            string_scroll_wheel_info  string_scroll_wheel;
            numeric_scroll_wheel_info numeric_scroll_wheel;
            menu_info          menu;
            INT                tree_view_indentation;
            int                template_display;
} extended_widget_info;

struct animation_info_struct;

/***************************************************************************************/
// ** IMPORTANT NOTE TO SELF **
//
// If you modify the members of this class in any way, make SURE you initialize them
// in the default constructor, and even more important make SURE you copy the new 
// members in the copy constructur!!
/***************************************************************************************/
class widget_info {
    public:
        widget_info();
        widget_info(int basetype);      // normal constructor
        widget_info(const widget_info &other, BOOL copy_next = FALSE);  // copy constructor

        ~widget_info();

        widget_info &operator=(const widget_info &other);

        int basetype;
        int misc_value;
        int allocation;       // static or dynamic allocation?

        GX_RECTANGLE size;
        GX_RESOURCE_ID color_id[NUM_WIDGET_COLORS];
        GX_RESOURCE_ID pixelmap_id[NUM_WIDGET_PIXELMAPS];
        GX_RESOURCE_ID string_id[NUM_WIDGET_STRINGS];
        GX_RESOURCE_ID font_id[NUM_WIDGET_FONTS];

        ULONG    style;

        CString   event_func;       // name of drawing function, if overridden
        CString   draw_func;        // name of event function, if overwridden
        CString   id_name;          // ID of child widget, like ID_BUTTON_NEXT
        CString   app_name;         // control block variable name like "background_win"
        CString   base_name;        // GUIX widget type ("GX_WINDOW"), or template name ("BASE_SCREEN")
        CString   custom_name;      // not currently used, the idea is allow user to specify custom control block type
        CString   callback_func;    // for vlist and drop list
        CString   format_func;      // for numeric prompt and pixelmap prompt
        CString   user_data;        // user defined data field
        BOOL      accepts_focus;
        BOOL      is_template;      // Widget is used as template
        BOOL      visible_at_startup; // Indicates if the widget is visible when the application starts up

        GX_WIDGET *widget;
        GX_WIDGET *copied_widget;   // used for Undo manager
        extended_widget_info ewi;

        widget_info *GetChildWidgetInfo() const { return child; };
        void SetChildWidgetInfo(widget_info *info);
        widget_info *GetNextWidgetInfo() const { return next; };
        void SetNextWidgetInfo(widget_info * info);

    private:
        widget_info *child;
        widget_info *next;
        void init(int type);
        void copy(const widget_info &other);

};

class folder_info
{
public:
    folder_info();
    folder_info(CString folder_name);      // normal constructor
    folder_info(const folder_info &other, BOOL copy_next = FALSE);  // copy constructor

    ~folder_info();

    widget_info *GetFirstChildWidget() const { return first_widget; };
    void SetFirstChildWidget(widget_info *info);

    folder_info *GetNextFolder() const { return next; };
    void SetNextFolder(folder_info *info);

    CString      folder_name;
    CString      output_filename;

private:
    void copy(const folder_info &other);
    folder_info &operator=(const folder_info &other);

    widget_info *first_widget;
    folder_info *next;

};

///////////////////////////////////////////////////////////////////////////////
class theme_info {
    public:
        CString theme_name;
        GX_SCROLLBAR_APPEARANCE VScrollAppearance;
        GX_SCROLLBAR_APPEARANCE HScrollAppearance;
        ULONG    VScrollStyle;
        ULONG    HScrollStyle;
        GX_COLOR *palette;
        int      palette_total_size;
        int      palette_predefined;
        BOOL     gen_color_table;
        BOOL     gen_font_table;
        BOOL     gen_pixelmap_table;
        BOOL     enabled;
        BOOL     statically_defined;
        res_info *GetFirstResourceInfo() const { return first_resource; };
        void     SetFirstResourceInfo(res_info *info);
    private:
        res_info *first_resource;
};

///////////////////////////////////////////////////////////////////////////////
class display_info {
    public:
        CString name;
        int     xres;
        int     yres;
        int     bits_per_pix;
        BOOL    packed_format;          // refers to 24 bpp r:g:b byte mode
        BOOL    format_555;             // for 16 bpp mode
        BOOL    format_4444;            // for 16 bpp mode
        BOOL    format_332;             // for 8bpp mode
        BOOL    grayscale;              // for 4bpp and 8bpp modes
        BOOL    reverse_order;          // bgr instead of rgb
        BOOL    allocate_canvas;
        BOOL    enabled;
        int     rotation_angle;
        BOOL    default_map_format;
        int     colorformat;
        int     num_themes;
        int     active_theme;
        string_table *stable;
        screen_flow  *screenflow;
        theme_info themes[MAX_THEMES];
        BOOL gen_string_table[MAX_LANGUAGES];

        folder_info *GetFirstChildFolder() const { return first_folder; };
        void SetFirstChildFolder(folder_info *folder);

    private:
        folder_info *first_folder;

};

class language_info{
public:
    CString name;
    BOOL    support_bidi_text;
    BOOL    gen_reordered_bidi_text;//generate bidi text in display order
    BOOL    support_thai_glyph_shaping;
    BOOL    gen_adjusted_thai_string;
    BOOL    statically_defined;
};

///////////////////////////////////////////////////////////////////////////////
class project_header
{
    public:
        int     project_version;
        int     guix_version;
        int     studio_version;
        CString project_name;
        CString project_path;
        CString source_path;
        CString header_path;
        CString resource_path;
        CString malloc_name;
        CString free_name;
        CString additional_headers;
        BOOL    insert_headers_before; //insert additional headers before other includes
        int     num_displays;
        int     max_displays;
        int     num_languages;

        int     target_cpu;
        int     target_tools;
        BOOL    big_endian;             // big-endian processor

        language_info languages[MAX_LANGUAGES];

        int     string_export_src;
        int     string_export_target;
        int     string_export_version;
        CString string_export_path;
        CString string_export_filename;
        int     string_export_filetype;

        BOOL    warn_missing_image;
        BOOL    warn_missing_font;

        BOOL    dave2d_graph_accelerator;
        int     renesas_png_decoder;
        int     renesas_jpeg_decoder;

        BOOL    grid_enabled;
        BOOL    snap_enabled;
        BOOL    snap_to_widget_enabled;
        int     grid_spacing;
        int     snap_spacing;

        BOOL    gen_binary;
        BOOL    gen_res_header;
        ULONG   binary_file_format;
        ULONG   memory_offset;

        BOOL    custom_resource_enabled;
        CString custom_resource_file_name;
        BOOL    b_new_project;

        int     app_execute_xpos;
        int     app_execute_ypos;

        BOOL    is_widget_position_locked;
        int     palette_mode_aa_text_colors;
};

struct PIXELMAP_RECORD;

class studiox_project
{
    public:
        studiox_project(const CString &path, const CString &name, BOOL bNew);
        ~studiox_project();

        static PIXELMAP_RECORD *GetDefaultPixelmapRecord(CString name);
         
        BOOL Save();
        BOOL SaveAs();

        BOOL Read(CString &pathname);

        int GetResourceType(res_info* start);
        void GenerateResourceXml(CString &pathname, CString &filename, res_info *start);
        BOOL ReadXMLFile(CString &pathname);

        static studiox_project *Clone(const studiox_project *src);
        BOOL IsModified() const {return is_modified;}
        void SetModified() {is_modified = TRUE;}
        BOOL IsWidgetPositionLocked(){ return mHeader.is_widget_position_locked; }

        project_header mHeader;
        display_info mDisplays[MAX_DISPLAYS];

        void LockUlockWidgetPositions(BOOL lock);

        int AddLanguage(int name_index);

        res_info *AddCustomColor(GX_COLOR color, CString &name, res_info *parent);
        void AddWidgetToFolder(folder_info *folder, widget_info *info);
        void AddFolderToDisplay(int DisplayIndex, folder_info *child);
        void AddWidgetToParent(widget_info *parent, widget_info *child);
        void DeleteWidget(widget_info *child);
        void DeleteFolder(folder_info *folder);
        int  GetDisplayIndex(const folder_info *folder) const;
        int  GetDisplayIndex(const widget_info *info) const;
        int  GetDisplayIndex(const res_info *info) const;
        int  GetDisplayColorFormat(const res_info *info) const;

        void MoveInfoToFront(widget_info *info);
        void MoveInfoToBack(widget_info *info);

        int CountResources(int DisplayIndex, int type) const;
        void DeleteResource(res_info *which);

        int CountEnabledThemes(int DisplayIndex) const;
        int CountEnabledLanguages(int DisplayIndex) const;
        int CountEnabledDisplays() const;
        int CountStaticallyDefinedThemes(int display);
        int CountStaticallyDefinedLanguages();

        //widget id, animation and event id dictionary
        BOOL AddToIdDictionary(int DisplayIndex, int id_type, CString &id_name);
        BOOL RemoveFromIdDictionary(int DisplayIndex, int id_type, CString &id_name);
        INT GetIdIndex(int DisplayIndex, int id_type, const CString &id_name) const;
        INT GetIdIndex(const CArray<id_info> &dictionary, const CString &id_name) const;
        CString GetIdName(int DisplayIndex, int id_type, int index) const;
        void CleanupIdDictionary(int DisplayIndex, int id_type);
        BOOL CopyIdDictionary(int Display, int id_type, CArray<id_info> *copied_dictionary);
        void SortIdDictionary(CArray<id_info> *dictionary);

        //resource dictionary
        BOOL AddToResourceDictionary(int DisplayIndex, res_info *info, int res_id = -1);
        BOOL RemoveFromResourceDictionary(int DisplayIndex, res_info *info);
        BOOL UpdateDictionaryResourceName(int DisplayIndex, CString &old_name, res_info *info);
        GX_RESOURCE_ID GetResourceId(int DisplayIndex, const res_info *info) const;
        GX_RESOURCE_ID GetResourceId(int DisplayIndex, const int restpe, const CString  &name) const;
        BOOL GetResourceName(int DisplayIndex, int res_type, int index, CString &return_name) const;
        BOOL CopyDictionary(int DisplayIndex, int res_type, CArray<CString> *dictionary);
        void SortResDictionary(INT res_type, CArray<CString> *dictionary);

        res_info *FindResource(int DisplayIndex, int ThemeIndex, int res_type, GX_RESOURCE_ID res_id) const;
        res_info *FindResource(int Display, int theme, int restype, const CString &name) const;
        res_info *FindResource(const res_info *start, int restype, const CString &name) const;
        res_info *FindResourceFolder(int Display, int theme_id, int res_type, GX_RESOURCE_ID res_id, const CString& name) const;
        res_info *FindResourceFolder(const res_info *start, int restype, int folder_id, CString name = _T("")) const;

        widget_info *FindWidgetInfo(const GX_WIDGET *widget) const;
        widget_info *FindWidgetInfo(const folder_info *folder, const GX_WIDGET *widget) const;
        widget_info *FindWidgetInfo(const widget_info *start, const GX_WIDGET *widget) const;
        widget_info *FindParentInfo(const widget_info *start, const widget_info *child) const;
        widget_info *FindParentInfo(const folder_info *folder, const widget_info *child) const;
        widget_info *FindParentInfo(const widget_info *child) const;
        widget_info *FindWidgetInfo(const widget_info *start, const CString &name, BOOL search_child) const;
        widget_info *FindWidgetInfo(const folder_info *folder, const CString &name, BOOL search_child) const;
        folder_info *FindParentFolderInfo(const widget_info *child) const;
        folder_info *FindFolderInfo(int display_index, const CString &name) const;
        folder_info *FindFolderInfo(const folder_info *start, const CString &name) const;
        const widget_info *GetTailInfo(const widget_info *info) const;

        //void MarkAllTemplateDirty(int Display);
        void CheckEmptyScreenFlow();

        void ReadOneResource(xml_reader &reader, int display, res_info *put);
        void WriteOneResource(xml_writer &writer, res_info *res, GX_BOOL xml_mode = FALSE);

        BOOL IsWidgetInInfoTree(const widget_info *start, const widget_info *info) const;
        BOOL IsPaletteMode(int display) const;
        BOOL TestForPixelmapsUsingGlobalPalette(const res_info *info) const;

        void InitializeThemeResources(int display, int theme, res_info *start);

        BOOL InitializeOnePixelmap(res_info *info, palette_info *palette, int display_color_format = -1);
        void InitializeThemePixelmaps(int display, int theme);
        void TaskInitializeAllPixelmaps();
        void InitializeAllPixelmaps();
        void CreateThemePalette(int display, int theme, palette_info *theme_palette);

        void InitializeFonts();

        static void ConfigureDefaultFont(res_info *put, int display);

        static CString ResTypeToString(int type);
        static int ResStringToType(CString &name);
        static CString FindFolderIdString(int res_type, int val);
        static int FindFolderIdVal(int res_type, CString string);

        void CleanupThemeResources(display_info *display, int ThemeIndex);
        void CleanupDisplayResources(display_info *display);
        void InitializeFonts(res_info* start, int display);
    protected:
        void InitStringExportHeader(void);
        void InitDisplayThemes(int DisplayIndex);
        void DefaultScrollbarAppearance(display_info *pInfo, int theme);

        void WriteProjectHeader(xml_writer &writer);
        void WriteDisplayInfo(xml_writer &writer, int index);
        void WriteThemePaletteInfo(xml_writer& writer, theme_info *theme, BOOL xml_mode = FALSE);
        void WriteStringTable(xml_writer &writer, string_table *table);
        void WriteScreenFlow(xml_writer &writer, screen_flow *screen_flow);
        void WriteFlowItem(xml_writer &writer, flow_item *item);
        void WriteTriggerInfo(xml_writer &writer, trigger_info *trigger);

        void WriteResources(xml_writer &writer, res_info *start, GX_BOOL xml_mode = FALSE);
        void WriteThemeScrollbars(xml_writer &writer, int display_index, int theme);

        void ReadProjectHeader(xml_reader &reader);
        BOOL ReadDisplayInfo(xml_reader &reader, int index);
        void ReadStringTable(xml_reader &reader, display_info *info);
        void ReadScreenFlow(xml_reader &reader, int display_index);
        void ReadFlowItem(xml_reader &reader, int display_index);
        void ReadTriggerInfo(xml_reader &reader, int display_index, flow_item *item);
        void ReadWidgetFolders(xml_reader &reader, int index);

        void ReadPaletteType(int display_format, xml_reader &reader, res_info *res);
        void WritePaletteType(xml_writer &writer, res_info *res);

        void ReadResources(xml_reader &reader, int display, int theme, res_info *parent);
        void ReadThemeScrollbars(xml_reader &reader, int display_index, int theme);
        void ReadThemePaletteInfo(xml_reader &reader, theme_info *theme, BOOL xml_mode = FALSE);

    private:
        void AssignSharedPaletteToPixelmaps(res_info *info);
        void CheckAssignSharedPaletteToPixelmaps();
        void CreateDefaultResources(int DisplayIndex, int ThemeIndex);
        void InitializeProjectResources(void);
        void InitProjectHeader(BOOL bNew);
        void InitializePixelmaps(res_info *info, palette_info *palette);

        //void MarkAllTemplateDirty(widget_info *start);

        BOOL is_modified;
        CArray<CString> color_dictionary[MAX_DISPLAYS];
        CArray<CString> font_dictionary[MAX_DISPLAYS];
        CArray<CString> pixelmap_dictionary[MAX_DISPLAYS];
        CArray<id_info> widget_id_dictionary[MAX_DISPLAYS];
        CArray<id_info> animation_id_dictionary[MAX_DISPLAYS];
};

#endif
