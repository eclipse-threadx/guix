
#include "system_pngs.h"
#include "studiox_includes.h"
#include "config_languages_dlg.h"
#include "string_export_dlg.h"
#include "gx_api.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern "C" {
extern GX_FONT _gx_system_font_8bpp;
extern GX_FONT _gx_system_font_4bpp;
extern GX_FONT _gx_system_font_mono;
extern GX_FONT _gx_dave2d_system_font_4bpp;
extern GX_FONT _gx_dave2d_system_font_mono;
};

#define SYSTEM_FONT_8BPP_STORAGE 7992
#define SYSTEM_FONT_4BPP_STORAGE 5084
#define SYSTEM_FONT_MONO_STORAGE 1763

// KGM FIXME: Why aren't we using the GUIX color format definitions here??????

#define GXS_IMG_SAVE_ALPHA          (1 << 2) 
#define GXS_IMG_DO_COMPRESSION      (1 << 3)
#define GXS_IMG_COLOR_DEPTH_24XRGB  1

#define RANGE_CHECK(val, min, max) \
           if(val > max)           \
           {                       \
               val = max;          \
           }                       \
           else if (val < min)     \
           {                       \
               val = min;          \
           }

#define RANGE_MIN_CHECK(val, min) \
           if(val < min)          \
           {                      \
               val = min;         \
           }

//extern WIDGET_TABLE_ENTRY widget_table[];

STRING_VAL_PAIR res_types[] = {
    {L"HEADER",   RES_TYPE_HEADER},
    {L"GROUP",    RES_TYPE_GROUP},
    {L"FOLDER",   RES_TYPE_FOLDER},
    {L"FONT",     RES_TYPE_FONT},
    {L"COLOR",    RES_TYPE_COLOR},
    {L"PIXELMAP", RES_TYPE_PIXELMAP},
    {L"STRING",   RES_TYPE_STRING},
    {NULL, 0}
};

STRING_VAL_PAIR res_folder_ids[] = {
    {L"DEFAULT_COLOR_FOLDER", DEFAULT_COLOR_FOLDER},
    {L"CUSTOM_COLOR_FOLDER", CUSTOM_COLOR_FOLDER},
    {L"DEFAULT_FONT_FOLDER", DEFAULT_FONT_FOLDER},
    {L"CUSTOM_FONT_FOLDER", CUSTOM_FONT_FOLDER},
    {L"DEFAULT_PIXELMAP_FOLDER", DEFAULT_PIXELMAP_FOLDER},
    {L"CUSTOM_PIXELMAP_FOLDER", CUSTOM_PIXELMAP_FOLDER},
    // Backward compatibility
    {L"4096", DEFAULT_COLOR_FOLDER},
    {L"4097", CUSTOM_COLOR_FOLDER},
    {L"4098", DEFAULT_FONT_FOLDER},
    {L"4099", CUSTOM_FONT_FOLDER},
    {L"4100", DEFAULT_PIXELMAP_FOLDER},
    {L"4101", CUSTOM_PIXELMAP_FOLDER},
    {L"", 0}
};

STRING_VAL_PAIR res_group_ids[] = {
    {L"COLOR_GROUP", COLOR_GROUP},
    {L"FONT_GROUP", FONT_GROUP},
    {L"PIXELMAP_GROUP", PIXELMAP_GROUP},
    {L"STRING_GROUP", STRING_GROUP},
    // Backward compatibility
    {L"4096", COLOR_GROUP},
    {L"4097", FONT_GROUP},
    {L"4098", PIXELMAP_GROUP},
    {L"4099", STRING_GROUP},
    {L"", 0}
};

STRING_VAL_PAIR res_header_ids[] = {
    {L"THEME_HEADER", THEME_HEADER},
    // Backward compatibility
    {L"4096", THEME_HEADER},
    {L"", 0}
};


#define DEFAULT_COLOR_CANVAS                     GX_COLOR_BLACK
#define DEFAULT_COLOR_WIDGET_FILL                0xff787c78
#define DEFAULT_COLOR_WINDOW_FILL                0xffe2e2e2
#define DEFAULT_COLOR_STANDARD_BORDER            0xff9b9b73
#define DEFAULT_COLOR_WINDOW_BORDER              0xff7599aa
#define DEFAULT_COLOR_NORMAL_TEXT                GX_COLOR_BLACK
#define DEFAULT_COLOR_SELECTED_TEXT              GX_COLOR_WHITE
#define DEFAULT_COLOR_SELECTED_FILL              GX_COLOR_BLUE
#define DEFAULT_COLOR_DISABLED_TEXT              0xffa0a0a0
#define DEFAULT_COLOR_DISABLED_FILL              0xff787c78

#define DEFAULT_COLOR_SHADOW                     GX_COLOR_DARKGRAY
#define DEFAULT_COLOR_SHINE                      0xffdadada

#define DEFAULT_COLOR_BUTTON_BORDER              0xffe0c060
#define DEFAULT_COLOR_BUTTON_UPPER               0xfff8f8e0
#define DEFAULT_COLOR_BUTTON_LOWER               0xfff8ecb0
#define DEFAULT_COLOR_BUTTON_TEXT                GX_COLOR_BLACK

#define DEFAULT_COLOR_SCROLL_FILL                0xffbababa
#define DEFAULT_COLOR_SCROLL_BUTTON              0xff7d7d7d
#define DEFAULT_COLOR_TEXT_INPUT_TEXT            GX_COLOR_BLACK
#define DEFAULT_COLOR_TEXT_INPUT_FILL            GX_COLOR_WHITE
#define DEFAULT_COLOR_READONLY_TEXT              GX_COLOR_WHITE
#define DEFAULT_COLOR_READONLY_FILL              0xff787c78

#define DEFAULT_COLOR_SLIDER_TICK                GX_COLOR_BLACK
#define DEFAULT_COLOR_SLIDER_GROOVE_TOP          GX_COLOR_LIGHTGRAY
#define DEFAULT_COLOR_SLIDER_GROOVE_BOTTOM       GX_COLOR_WHITE
#define DEFAULT_COLOR_SLIDER_NEEDLE_OUTLINE      GX_COLOR_BLACK
#define DEFAULT_COLOR_SLIDER_NEEDLE_FILL         GX_COLOR_DARKGRAY

///////////////////////////////////////////////////////////////////////////////
COLOR_RECORD DEFAULT_COLOR_TABLE[] = {
    {"CANVAS",                GX_COLOR_ID_CANVAS,                DEFAULT_COLOR_CANVAS},
    {"WIDGET_FILL",           GX_COLOR_ID_WIDGET_FILL,           DEFAULT_COLOR_WIDGET_FILL},
    {"WINDOW_FILL",           GX_COLOR_ID_WINDOW_FILL,           DEFAULT_COLOR_WINDOW_FILL},
    {"DEFAULT_BORDER",        GX_COLOR_ID_DEFAULT_BORDER,        DEFAULT_COLOR_STANDARD_BORDER},
    {"WINDOW_BORDER",         GX_COLOR_ID_WINDOW_BORDER,         DEFAULT_COLOR_WINDOW_BORDER},
    {"TEXT",                  GX_COLOR_ID_TEXT,                  DEFAULT_COLOR_NORMAL_TEXT},
    {"SELECTED_TEXT",         GX_COLOR_ID_SELECTED_TEXT,         DEFAULT_COLOR_SELECTED_TEXT},
    {"SELECTED_FILL",         GX_COLOR_ID_SELECTED_FILL,         DEFAULT_COLOR_SELECTED_FILL},
    {"SHADOW",                GX_COLOR_ID_SHADOW,                DEFAULT_COLOR_SHADOW},
    {"SHINE",                 GX_COLOR_ID_SHINE,                 DEFAULT_COLOR_SHINE},
    {"BTN_BORDER",            GX_COLOR_ID_BUTTON_BORDER,         DEFAULT_COLOR_BUTTON_BORDER},
    {"BTN_UPPER",             GX_COLOR_ID_BUTTON_UPPER,          DEFAULT_COLOR_BUTTON_UPPER},
    {"BTN_LOWER",             GX_COLOR_ID_BUTTON_LOWER,          DEFAULT_COLOR_BUTTON_LOWER},
    {"BTN_TEXT",              GX_COLOR_ID_BUTTON_TEXT,           DEFAULT_COLOR_BUTTON_TEXT},
    {"SCROLL_FILL",           GX_COLOR_ID_SCROLL_FILL,           DEFAULT_COLOR_SCROLL_FILL},
    {"SCROLL_BUTTON",         GX_COLOR_ID_SCROLL_BUTTON,         DEFAULT_COLOR_SCROLL_BUTTON},
    {"TEXT_INPUT_TEXT",       GX_COLOR_ID_TEXT_INPUT_TEXT,       GX_COLOR_WHITE},
    {"TEXT_INPUT_FILL",       GX_COLOR_ID_TEXT_INPUT_FILL,       DEFAULT_COLOR_WINDOW_FILL},
    {"SLIDER_TICK",           GX_COLOR_ID_SLIDER_TICK,           DEFAULT_COLOR_SLIDER_TICK},
    {"SLIDER_GROOVE_TOP",     GX_COLOR_ID_SLIDER_GROOVE_TOP,     DEFAULT_COLOR_SLIDER_GROOVE_TOP},
    {"SLIDER_GROOVE_BOTTOM",  GX_COLOR_ID_SLIDER_GROOVE_BOTTOM,  DEFAULT_COLOR_SLIDER_GROOVE_BOTTOM},
    {"SLIDER_NEEDLE_OUTLINE", GX_COLOR_ID_SLIDER_NEEDLE_OUTLINE, DEFAULT_COLOR_SLIDER_NEEDLE_OUTLINE},
    {"SLIDER_NEEDLE_FILL",    GX_COLOR_ID_SLIDER_NEEDLE_FILL,    DEFAULT_COLOR_SLIDER_NEEDLE_FILL},
    {"SLIDER_NEEDLE_LINE1",   GX_COLOR_ID_SLIDER_NEEDLE_LINE1,   GX_COLOR_LIGHTGRAY},
    {"SLIDER_NEEDLE_LINE2",   GX_COLOR_ID_SLIDER_NEEDLE_LINE2,   DEFAULT_COLOR_BUTTON_BORDER},
    { "DISABLED_TEXT", GX_COLOR_ID_DISABLED_TEXT, DEFAULT_COLOR_DISABLED_TEXT },
    { "DISABLED_FILL", GX_COLOR_ID_DISABLED_FILL, DEFAULT_COLOR_DISABLED_FILL },
    { "READONLY_TEXT", GX_COLOR_ID_READONLY_TEXT, DEFAULT_COLOR_READONLY_TEXT },
    { "READONLY_FILL", GX_COLOR_ID_READONLY_FILL, DEFAULT_COLOR_READONLY_FILL },
    {NULL,                    0,                                 0}
};

/* Pre-define color which used for 2bpp and 4bpp grayscale */
#define GX_COLOR_LIGHTGRAY_GRAYSCALE             0x55555555
#define GX_COLOR_DARKGRAY_GRAYSCALE              0xaaaaaaaa
///////////////////////////////////////////////////////////////////////////////
/* member of DEFAULT_COLOR_TABLE_GRAYSCALE[] should be same with DEFAULT_COLOR_TABLE[] */
COLOR_RECORD DEFAULT_COLOR_TABLE_GRAYSCALE[] = {
    { "CANVAS",          GX_COLOR_ID_CANVAS,          GX_COLOR_BLACK },
    { "WIDGET_FILL",     GX_COLOR_ID_WIDGET_FILL,     GX_COLOR_LIGHTGRAY_GRAYSCALE },
    { "WINDOW_FILL",     GX_COLOR_ID_WINDOW_FILL,     GX_COLOR_DARKGRAY_GRAYSCALE },
    { "DEFAULT_BORDER",  GX_COLOR_ID_DEFAULT_BORDER,  GX_COLOR_BLACK },
    { "WINDOW_BORDER",   GX_COLOR_ID_WINDOW_BORDER,   GX_COLOR_BLACK },
    { "TEXT",            GX_COLOR_ID_TEXT,            GX_COLOR_BLACK },
    { "SELECTED_TEXT",   GX_COLOR_ID_SELECTED_TEXT,   GX_COLOR_WHITE },
    { "SELECTED_FILL",   GX_COLOR_ID_SELECTED_FILL,   GX_COLOR_BLACK },
    { "SHADOW",          GX_COLOR_ID_SHADOW,          GX_COLOR_DARKGRAY_GRAYSCALE },
    { "SHINE",           GX_COLOR_ID_SHINE,           GX_COLOR_LIGHTGRAY_GRAYSCALE },
    { "BTN_BORDER",      GX_COLOR_ID_BUTTON_BORDER,   GX_COLOR_BLACK },
    { "BTN_UPPER",       GX_COLOR_ID_BUTTON_UPPER,    GX_COLOR_DARKGRAY_GRAYSCALE },
    { "BTN_LOWER",       GX_COLOR_ID_BUTTON_LOWER,    GX_COLOR_LIGHTGRAY_GRAYSCALE },
    { "BTN_TEXT",        GX_COLOR_ID_BUTTON_TEXT,     GX_COLOR_BLACK },
    { "SCROLL_FILL",     GX_COLOR_ID_SCROLL_FILL,     GX_COLOR_LIGHTGRAY_GRAYSCALE },
    { "SCROLL_BUTTON",   GX_COLOR_ID_SCROLL_BUTTON,   GX_COLOR_DARKGRAY_GRAYSCALE },
    { "TEXT_INPUT_TEXT", GX_COLOR_ID_TEXT_INPUT_TEXT, GX_COLOR_WHITE },
    { "TEXT_INPUT_FILL", GX_COLOR_ID_TEXT_INPUT_FILL, GX_COLOR_DARKGRAY_GRAYSCALE },
    { "SLIDER_TICK",       GX_COLOR_ID_SLIDER_TICK,    GX_COLOR_WHITE },
    { "SLIDER_GROOVE_TOP", GX_COLOR_ID_SLIDER_GROOVE_TOP, GX_COLOR_WHITE },
    { "SLIDER_GROOVE_BOTTOM", GX_COLOR_ID_SLIDER_GROOVE_BOTTOM, GX_COLOR_BLACK },
    { "SLIDER_NEEDLE_OUTLINE", GX_COLOR_ID_SLIDER_NEEDLE_OUTLINE, GX_COLOR_BLACK },
    { "SLIDER_NEEDLE_FILL",  GX_COLOR_ID_SLIDER_NEEDLE_FILL, GX_COLOR_WHITE },
    { "SLIDER_NEEDLE_LINE1", GX_COLOR_ID_SLIDER_NEEDLE_LINE1, GX_COLOR_DARKGRAY_GRAYSCALE },
    { "SLIDER_NEEDLE_LINE2", GX_COLOR_ID_SLIDER_NEEDLE_LINE2, GX_COLOR_LIGHTGRAY_GRAYSCALE },
    { "DISABLED_TEXT", GX_COLOR_ID_DISABLED_TEXT, GX_COLOR_LIGHTGRAY_GRAYSCALE },
    { "DISABLED_FILL", GX_COLOR_ID_DISABLED_FILL, GX_COLOR_DARKGRAY_GRAYSCALE },
    { "READONLY_TEXT", GX_COLOR_ID_READONLY_TEXT, GX_COLOR_WHITE },
    { "READONLY_FILL", GX_COLOR_ID_READONLY_FILL, GX_COLOR_LIGHTGRAY_GRAYSCALE },
    { NULL, 0, 0 }
};

/* member of DEFAULT_COLOR_TABLE_GRAYSCALE[] should be same with DEFAULT_COLOR_TABLE[] */
COLOR_RECORD DEFAULT_COLOR_TABLE_MONOCHROME[] = {
    { "CANVAS",          GX_COLOR_ID_CANVAS,          GX_COLOR_BLACK },
    { "WIDGET_FILL",     GX_COLOR_ID_WIDGET_FILL,     GX_COLOR_BLACK },
    { "WINDOW_FILL",     GX_COLOR_ID_WINDOW_FILL,     GX_COLOR_BLACK },
    { "DEFAULT_BORDER",  GX_COLOR_ID_DEFAULT_BORDER,  GX_COLOR_WHITE },
    { "WINDOW_BORDER",   GX_COLOR_ID_WINDOW_BORDER,   GX_COLOR_WHITE },
    { "TEXT",            GX_COLOR_ID_TEXT,            GX_COLOR_WHITE },
    { "SELECTED_TEXT",   GX_COLOR_ID_SELECTED_TEXT,   GX_COLOR_WHITE },
    { "SELECTED_FILL",   GX_COLOR_ID_SELECTED_FILL,   GX_COLOR_BLACK },
    { "SHADOW",          GX_COLOR_ID_SHADOW,          GX_COLOR_BLACK },
    { "SHINE",           GX_COLOR_ID_SHINE,           GX_COLOR_BLACK },
    { "BTN_BORDER",      GX_COLOR_ID_BUTTON_BORDER,   GX_COLOR_WHITE },
    { "BTN_UPPER",       GX_COLOR_ID_BUTTON_UPPER,    GX_COLOR_BLACK },
    { "BTN_LOWER",       GX_COLOR_ID_BUTTON_LOWER,    GX_COLOR_BLACK },
    { "BTN_TEXT",        GX_COLOR_ID_BUTTON_TEXT,     GX_COLOR_WHITE },
    { "SCROLL_FILL",     GX_COLOR_ID_SCROLL_FILL,     GX_COLOR_BLACK },
    { "SCROLL_BUTTON",   GX_COLOR_ID_SCROLL_BUTTON,   GX_COLOR_BLACK },
    { "TEXT_INPUT_TEXT", GX_COLOR_ID_TEXT_INPUT_TEXT, GX_COLOR_WHITE },
    { "TEXT_INPUT_FILL", GX_COLOR_ID_TEXT_INPUT_FILL, GX_COLOR_BLACK },
    { "SLIDER_TICK",       GX_COLOR_ID_SLIDER_TICK,    GX_COLOR_WHITE },
    { "SLIDER_GROOVE_TOP", GX_COLOR_ID_SLIDER_GROOVE_TOP, GX_COLOR_BLACK },
    { "SLIDER_GROOVE_BOTTOM", GX_COLOR_ID_SLIDER_GROOVE_BOTTOM, GX_COLOR_BLACK },
    { "SLIDER_NEEDLE_OUTLINE", GX_COLOR_ID_SLIDER_NEEDLE_OUTLINE, GX_COLOR_WHITE },
    { "SLIDER_NEEDLE_FILL",  GX_COLOR_ID_SLIDER_NEEDLE_FILL, GX_COLOR_BLACK },
    { "SLIDER_NEEDLE_LINE1", GX_COLOR_ID_SLIDER_NEEDLE_LINE1, GX_COLOR_WHITE },
    { "SLIDER_NEEDLE_LINE2", GX_COLOR_ID_SLIDER_NEEDLE_LINE2, GX_COLOR_WHITE },
    { "DISABLED_TEXT", GX_COLOR_ID_DISABLED_TEXT, GX_COLOR_WHITE },
    { "DISABLED_FILL", GX_COLOR_ID_DISABLED_FILL, GX_COLOR_BLACK },
    { "READONLY_TEXT", GX_COLOR_ID_READONLY_TEXT, GX_COLOR_WHITE },
    { "READONLY_FILL", GX_COLOR_ID_READONLY_FILL, GX_COLOR_BLACK },
    { NULL, 0, 0 }
};

///////////////////////////////////////////////////////////////////////////////
FONT_RECORD DEFAULT_FONT_TABLE[] = {
    {"SYSTEM",        GX_FONT_ID_DEFAULT,       &_gx_system_font_8bpp},
    {"BUTTON",        GX_FONT_ID_BUTTON,        &_gx_system_font_8bpp},
    {"PROMPT",        GX_FONT_ID_PROMPT,        &_gx_system_font_8bpp},
    {"TEXT_INPUT",    GX_FONT_ID_TEXT_INPUT,    &_gx_system_font_8bpp},
    {NULL, -1, NULL}
};

extern IMAGE_INFO _system_png_radio_on;
extern IMAGE_INFO _system_png_radio_off;
extern IMAGE_INFO _system_png_checkbox_on;
extern IMAGE_INFO _system_png_checkbox_off;

// FIXME: do we want to use hardcoded path? 
PIXELMAP_RECORD DEFAULT_PIXELMAP_TABLE[] = {
    { "RADIO_ON", GX_PIXELMAP_RADIO_ON_ID, TRUE, &_system_png_radio_on},
    { "RADIO_OFF", GX_PIXELMAP_RADIO_OFF_ID, TRUE, &_system_png_radio_off },
    { "CHECKBOX_ON", GX_PIXELMAP_CHECKBOX_ON_ID, FALSE, &_system_png_checkbox_on },
    { "CHECKBOX_OFF", GX_PIXELMAP_CHECKBOX_OFF_ID, FALSE, &_system_png_checkbox_off },
    {NULL, 0, NULL}
};

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
// the normal constructor
widget_info::widget_info()
{
    init(GX_TYPE_WIDGET);
}

///////////////////////////////////////////////////////////////////////////////
widget_info::widget_info(int type)
{
    init(type);
}

///////////////////////////////////////////////////////////////////////////////
// the copy constructor
widget_info::widget_info(const widget_info &other, BOOL copy_next)
{
    copy(other);
 
    widget_info *sib_put;
    const widget_info *sib_get;
    
    if (other.child)
    {
        this->child = new widget_info(*other.child, TRUE);
    }
    if (copy_next)
    {
        sib_put = this;
        sib_get = &other;

        while(sib_get->next)
        {
            sib_put->next = new widget_info(*sib_get->next, FALSE);
            sib_put = sib_put->next;
            sib_get = sib_get->next;
        }
    } 
}

///////////////////////////////////////////////////////////////////////////////
widget_info &widget_info::operator=(const widget_info &other)
{
    copy(other);
    return *this;
}

///////////////////////////////////////////////////////////////////////////////
widget_info::~widget_info()
{
    widget_info *test;
    widget_info *temp;

    switch(basetype)
    {
    case GX_TYPE_SPRITE:
        if (ewi.sprite.framelist)
        {
            delete [] ewi.sprite.framelist;
        }
        break;

    case GX_TYPE_STRING_SCROLL_WHEEL:
        if (ewi.string_scroll_wheel.string_id_list)
        {
            delete ewi.string_scroll_wheel.string_id_list;
            ewi.string_scroll_wheel.string_id_list = NULL;
        }
        break;
    }
    
    if (child)
    {
        delete child;
        child = NULL;
    }

    test = next;
    next = NULL;

    while(test)
    {
        temp = test->next;
        test->next = NULL;
        delete test;
        test = temp;
    }

    if (widget)
    {
        widget_factory::CleanupWidgets(this);
    }
}

///////////////////////////////////////////////////////////////////////////////
void widget_info::copy(const widget_info &other)
{
    int index;
    init(other.basetype);

    size = other.size;

    for (index = 0; index < NUM_WIDGET_COLORS; index++)
    {
        color_id[index] = other.color_id[index];
    }
    for (index = 0; index < NUM_WIDGET_PIXELMAPS; index++)
    {
        pixelmap_id[index] = other.pixelmap_id[index];
    }
    font_id[0] = other.font_id[0];
    font_id[1] = other.font_id[1];
    string_id[0] = other.string_id[0];
    string_id[1] = other.string_id[1];

    style = other.style;

    is_template = other.is_template;
    visible_at_startup = other.visible_at_startup;
    event_func = other.event_func;
    draw_func = other.draw_func;
    callback_func = other.callback_func;
    format_func = other.format_func;
    
    id_name = other.id_name;
    app_name = other.app_name;          // handy name passed to create
    base_name = other.base_name;        // control structure name
    custom_name = other.custom_name;    // user-defined custom structure name
    user_data = other.user_data;

    ewi = other.ewi;                   // copy the union

    // if copying a sprite, make a new sprite framelist
    if (other.basetype == GX_TYPE_SPRITE)
    {
        if (other.ewi.sprite.framelist)
        {
            ewi.sprite.framelist = new GX_SPRITE_FRAME[ewi.sprite.frame_count];

            for (index = 0; index < ewi.sprite.frame_count; index++)
            {
                ewi.sprite.framelist[index] = other.ewi.sprite.framelist[index];
            }
        }
    }

    if (other.basetype == GX_TYPE_STRING_SCROLL_WHEEL)
    {
        if (other.ewi.string_scroll_wheel.string_id_list)
        {
            int total_rows = other.ewi.string_scroll_wheel.base.total_rows;
            ewi.string_scroll_wheel.string_id_list = new GX_RESOURCE_ID[total_rows];
            memcpy_s(ewi.string_scroll_wheel.string_id_list, total_rows * sizeof(GX_RESOURCE_ID), other.ewi.string_scroll_wheel.string_id_list, total_rows * sizeof(GX_RESOURCE_ID));
        }
    }

    allocation = other.allocation;
    accepts_focus = other.accepts_focus;

    misc_value = other.misc_value;

    this->widget = NULL;
    this->next = NULL;
    this->child = NULL;

    if (other.widget)
    {
        this->copied_widget = other.widget;
    }
    else
    {
        this->copied_widget = other.copied_widget;
    }
}

///////////////////////////////////////////////////////////////////////////////
void widget_info::init(int type)
{
    int index;

    basetype = type;
    base_name = widget_factory::WidgetTypeToString(type);
    custom_name = "";

    app_name = "";
    event_func = "";
    draw_func = "";
    user_data = "";
    id_name = "";
    callback_func = "";
    format_func = "";
    allocation = STATICALLY_ALLOCATED;
    misc_value = 0;

    memset(&ewi, 0, sizeof(extended_widget_info));

    size.gx_rectangle_left = size.gx_rectangle_top = size.gx_rectangle_right = size.gx_rectangle_bottom = 0;
    style = 0;

    for (index = 0; index < NUM_WIDGET_COLORS; index++)
    {
        color_id[index] = 0;
    }
    for (index = 0; index < NUM_WIDGET_PIXELMAPS; index++)
    {
        pixelmap_id[index] = 0;
    }
    for (index = 0; index < NUM_WIDGET_FONTS; index++)
    {
        font_id[index] = 0;
    }
    string_id[0] = 0;
    string_id[1] = 0;

    widget = NULL;
    child = NULL;
    next = NULL;
    copied_widget = NULL;
    is_template = FALSE;
    visible_at_startup = FALSE;
}

///////////////////////////////////////////////////////////////////////////////
void widget_info::SetChildWidgetInfo(widget_info *info)
{
    this->child = info;
}

///////////////////////////////////////////////////////////////////////////////
void widget_info::SetNextWidgetInfo(widget_info *info)
{
    this->next = info;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
res_info::res_info(int ResType)
{
    type = ResType;

    switch (type)
    {
    case RES_TYPE_ADD_COLOR:
        name = "Add New Color";
        break;

    case RES_TYPE_ADD_FONT:
        name = "Add New Font";
        break;

    case RES_TYPE_ADD_PIXELMAP:
        name = "Add New Pixelmap";
        break;

    case RES_TYPE_ADD_STRING:
        name = "Add New String";
        break;

    default:
        name = "";
        break;
    }

    pathinfo.pathname = "";
    pathinfo.pathtype = PATH_TYPE_PROJECT_RELATIVE;
    next = NULL;
    child = NULL;
    parent = NULL;

    if (ResType == RES_TYPE_PIXELMAP)
    {
        compress = TRUE;
    }
    else
    {
        compress = FALSE;
    }

    keep_alpha = FALSE;
    dither = FALSE;
    raw = FALSE;
    palette_type = PALETTE_TYPE_NONE;
    font_height = 0;
    font_bits = 8;
    font_charset_include_string_table = FALSE;
    font_pages = NULL;
    font_pages_count = 0;
    font_support_extended_unicode = FALSE;
    map_list.RemoveAll();
    map_delay_list.RemoveAll();
    thumbnail = NULL;
    font = NULL;
    colorval = RGB(0, 0, 0);
    folder_id = 0;
    storage_size = 0;
    output_color_format = 0;
    output_file_enabled = FALSE;
    output_file = "";
    binary_mode = FALSE;
    is_default = FALSE;
    enabled = TRUE;

    is_modified = FALSE;
}

///////////////////////////////////////////////////////////////////////////////
// the copy constructor
res_info::res_info(const res_info *in_parent, const res_info &other, BOOL copy_next)
{
    type = other.type;
    name = other.name;
    pathinfo.pathname = other.pathinfo.pathname;
    pathinfo.pathtype = other.pathinfo.pathtype;
    next = NULL;
    child = NULL;
    parent = (res_info *) in_parent;
    compress = other.compress;
    font_kerning = other.font_kerning;
    keep_alpha = other.keep_alpha;
    dither = other.dither;
    raw = other.raw;
    palette_type = other.palette_type;
    storage_size = other.storage_size;
    output_color_format = other.output_color_format;
    output_file_enabled = other.output_file_enabled;
    output_file = other.output_file;
    binary_mode = other.binary_mode;

    font_height = other.font_height;
    font_bits = other.font_bits;
    font_charset_include_string_table = other.font_charset_include_string_table;
    font_pages = NULL;
    font_pages_count = 0;
    font_support_extended_unicode = other.font_support_extended_unicode;

    int page_count = NUM_FONT_CHAR_RANGES;
    if (other.font_support_extended_unicode)
    {
        page_count += NUM_FONT_EXTENDED_CHAR_RANGES;
    }

    if (other.font_pages)
    {
        font_pages = new font_page_info[page_count];
        memcpy_s(font_pages, sizeof(font_page_info) * page_count, other.font_pages, sizeof(font_page_info) * page_count);
    }

    colorval = other.colorval;
    folder_id = other.folder_id;
    is_default = other.is_default;
    enabled = other.enabled;

    map_list.RemoveAll();
    map_delay_list.RemoveAll();
    thumbnail = NULL;
    font = NULL;

    if (other.child)
    {
        child = new res_info(this, *other.child, TRUE);
    }

    if (copy_next)
    {
        res_info *sib_put = this;
        const res_info *sib_get = &other;

        while(sib_get->next)
        {
            sib_put->next = new res_info(parent, *sib_get->next, FALSE);
            sib_put = sib_put->next;
            sib_get = sib_get->next;
        }
    }

    is_modified = FALSE;
}

///////////////////////////////////////////////////////////////////////////////
res_info::~res_info()
{
    res_info *test;

    switch(type)
    {
    case RES_TYPE_FONT:
        if (font)
        {
            if (!is_default || !pathinfo.pathname.IsEmpty())
            {
                DestroyFont(font);
            }
        }
        if (font_pages)
        {
            delete [] font_pages;
            font_pages = NULL;
        }
        break;

    case RES_TYPE_PIXELMAP:
        if (map_list.GetCount())
        {
            pixelmap_list_destroy(map_list);
        }
        if (thumbnail)
        {
            pixelmap_destroy(thumbnail);
        }
        map_delay_list.RemoveAll();
        break;

    default:
        break;
    }
    while(child)
    {
        test = child->next;
        delete child;
        child = test;
    }
}

///////////////////////////////////////////////////////////////////////////////
void res_info::Attach(res_info *pRes)
{
    res_info *test;
    pRes->next = NULL;

    if (child)
    {
        test = child;
        while(test->next)
        {
            test = test->next;
        }
        test->next = pRes;
    }
    else
    {
        child = pRes;
    }
    pRes->parent = this;
}

///////////////////////////////////////////////////////////////////////////////
void res_info::Detach()
{

    res_info *previous;

    if (parent)
    {
        if (parent->child == this)
        {
            // the easy case, first child:
            parent->child = next;
        }
        else
        {
            previous = parent->child;
            while (previous->next != this)
            {
                previous = previous->next;
            }
            previous->next = next;
        }
    }
    next = NULL;
    parent = NULL;
}

///////////////////////////////////////////////////////////////////////////////
GX_PIXELMAP *res_info::GetPixelmap(int frame_id)
{

    if (frame_id < map_list.GetCount())
    {
        return map_list.GetAt(frame_id);
    }

    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
INT res_info::GetPixelmapDelayTime(int frame_id)
{
    if (frame_id < map_delay_list.GetCount())
    {
        return map_delay_list.GetAt(frame_id);
    }

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
CString studiox_project::ResTypeToString(int type)
{
    return FindPairString(res_types, type);
}

///////////////////////////////////////////////////////////////////////////////
int studiox_project::ResStringToType(CString &name)
{
    return FindPairVal(res_types, name);
}

///////////////////////////////////////////////////////////////////////////////
CString studiox_project::FindFolderIdString(int res_type, int val)
{
    STRING_VAL_PAIR *entry = NULL;

    switch (res_type)
    {
    case RES_TYPE_GROUP:
        entry = res_group_ids;
        break;

    case RES_TYPE_FOLDER:
        entry = res_folder_ids;
        break;

    case RES_TYPE_HEADER:
        entry = res_header_ids;
        break;

    default:
        break;
    }

    if (entry)
    {
        return FindPairString(entry, val);
    }

    return L"";
}

///////////////////////////////////////////////////////////////////////////////
int studiox_project::FindFolderIdVal(int res_type, CString string)
{
    STRING_VAL_PAIR *entry = NULL;

    switch (res_type)
    {
    case RES_TYPE_GROUP:
        entry = res_group_ids;
        break;

    case RES_TYPE_FOLDER:
        entry = res_folder_ids;
        break;

    case RES_TYPE_HEADER:
        entry = res_header_ids;
        break;

    default:
        break;
    }

    if (entry)
    {
        return FindPairVal(entry, string);
    }

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
studiox_project::studiox_project(const CString &path, const CString &name, BOOL bNewProject)
{
    InitProjectHeader(bNewProject);
    mHeader.project_path = path;
    mHeader.project_name = name;
    is_modified = FALSE;
}

///////////////////////////////////////////////////////////////////////////////
studiox_project::~studiox_project()
{
    for (int index = 0; index < MAX_DISPLAYS; index++)
    {
        if (mDisplays[index].GetFirstChildFolder())
        {
            const folder_info *test = mDisplays[index].GetFirstChildFolder();
            
            if (test)
            {
                delete test;
            }
        }
        CleanupDisplayResources(&mDisplays[index]);

        if (mDisplays[index].stable)
        {
            delete mDisplays[index].stable;
        }

        if (mDisplays[index].screenflow)
        {
            delete mDisplays[index].screenflow;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void studiox_project::InitStringExportHeader()
{
    mHeader.string_export_src = 0;
    mHeader.string_export_target = 1;
    mHeader.string_export_version = 2;
    mHeader.string_export_path = ".\\";
    mHeader.string_export_filename = "";
    mHeader.string_export_filetype = STRING_EXPORT_TYPE_XLIFF;
}

///////////////////////////////////////////////////////////////////////////////
void studiox_project::InitProjectHeader(BOOL bNewProject)
{
    CString screen_name;
    CString theme_name;
    char    index_char[4];

    mHeader.project_version = PROJECT_VERSION;
    mHeader.guix_version = GuixVersionFieldsToVersionNumber(GUIX_MAJOR_VERSION, GUIX_MINOR_VERSION, GUIX_PATCH_VERSION);
    mHeader.studio_version = CalculateStudioVersion();

    mHeader.num_displays = 1;
    mHeader.num_languages = 1;
    mHeader.languages[0].name = CString("English");

    for (int index = 0; index < MAX_LANGUAGES; index++)
    {
        mHeader.languages[index].support_bidi_text = FALSE;
        mHeader.languages[index].gen_reordered_bidi_text = FALSE;
        mHeader.languages[index].support_thai_glyph_shaping = FALSE;
        mHeader.languages[index].gen_adjusted_thai_string = FALSE;
        mHeader.languages[index].statically_defined = TRUE;
    }

    mHeader.max_displays = MAX_DISPLAYS;

    // configure defaults for all screen info stuctures
    for (int index = 0; index < MAX_DISPLAYS; index++)
    {
        screen_name = "display_";
        _ltoa_s(index + 1, index_char, 4, 10);
        screen_name += index_char;
        mDisplays[index].name = screen_name;
        mDisplays[index].num_themes = 1;

        mDisplays[index].xres = 320;
        mDisplays[index].yres = 240;
        mDisplays[index].bits_per_pix = 16;
        mDisplays[index].grayscale = FALSE;
        mDisplays[index].packed_format = FALSE;
        mDisplays[index].reverse_order = FALSE;
        mDisplays[index].format_555 = FALSE;
        mDisplays[index].format_4444 = FALSE;
        mDisplays[index].format_332 = FALSE;
        mDisplays[index].colorformat = GX_COLOR_FORMAT_565RGB;
        mDisplays[index].SetFirstChildFolder(NULL);
        mDisplays[index].enabled = TRUE;
        mDisplays[index].default_map_format = TRUE;
        mDisplays[index].allocate_canvas = TRUE;
        mDisplays[index].rotation_angle = GX_SCREEN_ROTATION_NONE;
        InitDisplayThemes(index);

        for (int language = 0; language < mHeader.num_languages; language++)
        {
            mDisplays[index].gen_string_table[language] = TRUE;
        }

        mDisplays[index].stable = NULL;
        mDisplays[index].screenflow = NULL;

        if (bNewProject)
        {
            CreateDefaultResources(index, mDisplays[index].active_theme);
        }
    }

    // configure defaults for directories:
    mHeader.header_path = ".\\";
    mHeader.source_path = ".\\";
    mHeader.resource_path = ".\\";
    mHeader.malloc_name = "";
    mHeader.free_name = "";
    mHeader.additional_headers = "";
    mHeader.insert_headers_before = FALSE;
    mHeader.target_cpu = CPU_GENERIC;
    mHeader.target_tools = TOOLS_GENERIC;
    mHeader.big_endian = FALSE;
    mHeader.dave2d_graph_accelerator = TRUE;
    mHeader.renesas_png_decoder = DECODER_TYPE_NONE;
    mHeader.renesas_jpeg_decoder = DECODER_TYPE_HW;
    mHeader.grid_enabled = FALSE;
    mHeader.snap_enabled = FALSE;
    mHeader.snap_to_widget_enabled = FALSE;
    mHeader.grid_spacing = 10;
    mHeader.snap_spacing = 10;
    mHeader.gen_binary = FALSE;
    mHeader.gen_res_header = TRUE;
    mHeader.binary_file_format = BINARY_FILE_FORMAT_SREC;
    mHeader.memory_offset = 0;
    mHeader.custom_resource_enabled = FALSE;
    mHeader.custom_resource_file_name = "";
    if (bNewProject)
    {
        mHeader.b_new_project = TRUE;
    }
    else
    {
        mHeader.b_new_project = FALSE;
    }
    mHeader.app_execute_xpos = 20;
    mHeader.app_execute_ypos = 20;
    mHeader.is_widget_position_locked = FALSE;
    mHeader.palette_mode_aa_text_colors = 8;

    InitStringExportHeader();
}


///////////////////////////////////////////////////////////////////////////////
studiox_project *studiox_project::Clone(const studiox_project *src)
{
    int index;
    int display;
    int theme;

    studiox_project *new_project = new studiox_project(
        src->mHeader.project_path, src->mHeader.project_name, FALSE);

    new_project->mHeader.project_version = PROJECT_VERSION;
    new_project->mHeader.guix_version = src->mHeader.guix_version;
    new_project->mHeader.studio_version = src->mHeader.studio_version;
    new_project->mHeader.max_displays = MAX_DISPLAYS;

    new_project->mHeader.num_displays = src->mHeader.num_displays;
    new_project->mHeader.num_languages = src->mHeader.num_languages;
    new_project->mHeader.header_path = src->mHeader.header_path;
    new_project->mHeader.source_path = src->mHeader.source_path;
    new_project->mHeader.resource_path = src->mHeader.resource_path;
    
    new_project->mHeader.malloc_name = src->mHeader.malloc_name;
    new_project->mHeader.free_name = src->mHeader.free_name;
    new_project->mHeader.additional_headers = src->mHeader.additional_headers;
    new_project->mHeader.insert_headers_before = src->mHeader.insert_headers_before;
    new_project->mHeader.target_cpu = src->mHeader.target_cpu;
    new_project->mHeader.target_tools = src->mHeader.target_tools;
    new_project->mHeader.big_endian = src->mHeader.big_endian;
    new_project->mHeader.dave2d_graph_accelerator = src->mHeader.dave2d_graph_accelerator;
    new_project->mHeader.renesas_png_decoder = src->mHeader.renesas_png_decoder;
    new_project->mHeader.renesas_jpeg_decoder = src->mHeader.renesas_jpeg_decoder;
    new_project->mHeader.grid_enabled = src->mHeader.grid_enabled;
    new_project->mHeader.snap_enabled = src->mHeader.snap_enabled;
    new_project->mHeader.snap_to_widget_enabled = src->mHeader.snap_to_widget_enabled;
    new_project->mHeader.grid_spacing = src->mHeader.grid_spacing;
    new_project->mHeader.snap_spacing = src->mHeader.snap_spacing;
    new_project->mHeader.gen_binary = src->mHeader.gen_binary;
    new_project->mHeader.gen_res_header = src->mHeader.gen_res_header;
    new_project->mHeader.memory_offset = src->mHeader.memory_offset;
    new_project->mHeader.binary_file_format = src->mHeader.binary_file_format;
    new_project->mHeader.custom_resource_enabled = src->mHeader.custom_resource_enabled;
    new_project->mHeader.custom_resource_file_name = src->mHeader.custom_resource_file_name;
    new_project->mHeader.app_execute_xpos = src->mHeader.app_execute_xpos;
    new_project->mHeader.app_execute_ypos = src->mHeader.app_execute_ypos;
    new_project->mHeader.is_widget_position_locked = src->mHeader.is_widget_position_locked;
    new_project->mHeader.palette_mode_aa_text_colors = src->mHeader.palette_mode_aa_text_colors;

    new_project->mHeader.string_export_src = src->mHeader.string_export_src;
    new_project->mHeader.string_export_target = src->mHeader.string_export_target;
    new_project->mHeader.string_export_version = src->mHeader.string_export_version;
    new_project->mHeader.string_export_path = src->mHeader.string_export_path;
    new_project->mHeader.string_export_filename = src->mHeader.string_export_filename;
    new_project->mHeader.string_export_filetype = src->mHeader.string_export_filetype;

    // clone language names
    for (index = 0; index < new_project->mHeader.num_languages; index++)
    {
        new_project->mHeader.languages[index].name = src->mHeader.languages[index].name;
        new_project->mHeader.languages[index].support_bidi_text = src->mHeader.languages[index].support_bidi_text;
        new_project->mHeader.languages[index].gen_reordered_bidi_text = src->mHeader.languages[index].gen_reordered_bidi_text;
        new_project->mHeader.languages[index].support_thai_glyph_shaping = src->mHeader.languages[index].support_thai_glyph_shaping;
        new_project->mHeader.languages[index].gen_adjusted_thai_string = src->mHeader.languages[index].gen_adjusted_thai_string;
        new_project->mHeader.languages[index].statically_defined = src->mHeader.languages[index].statically_defined;
    }

    // clone dislay info
    for (display = 0; display < MAX_DISPLAYS; display++)
    {
        new_project->mDisplays[display].name = src->mDisplays[display].name;
        new_project->mDisplays[display].num_themes = src->mDisplays[display].num_themes;
        new_project->mDisplays[display].active_theme = src->mDisplays[display].active_theme;

        for (theme = 0; theme < MAX_THEMES; theme++)
        {
            new_project->mDisplays[display].themes[theme].SetFirstResourceInfo(NULL);

            new_project->mDisplays[display].themes[theme].theme_name = src->mDisplays[display].themes[theme].theme_name;
            new_project->mDisplays[display].themes[theme].VScrollAppearance = src->mDisplays[display].themes[theme].VScrollAppearance;
            new_project->mDisplays[display].themes[theme].HScrollAppearance, src ->mDisplays[display].themes[theme].HScrollAppearance;
            new_project->mDisplays[display].themes[theme].VScrollStyle = src->mDisplays[display].themes[theme].VScrollStyle;
            new_project->mDisplays[display].themes[theme].HScrollStyle = src->mDisplays[display].themes[theme].HScrollStyle;
            
            new_project->mDisplays[display].themes[theme].palette_total_size = src->mDisplays[display].themes[theme].palette_total_size;
            new_project->mDisplays[display].themes[theme].palette_predefined = src->mDisplays[display].themes[theme].palette_predefined;
            new_project->mDisplays[display].themes[theme].gen_color_table = src->mDisplays[display].themes[theme].gen_color_table;
            new_project->mDisplays[display].themes[theme].gen_font_table = src->mDisplays[display].themes[theme].gen_font_table;
            new_project->mDisplays[display].themes[theme].gen_pixelmap_table = src->mDisplays[display].themes[theme].gen_pixelmap_table;
            new_project->mDisplays[display].themes[theme].enabled = src->mDisplays[display].themes[theme].enabled;
            new_project->mDisplays[display].themes[theme].statically_defined = src->mDisplays[display].themes[theme].statically_defined;

            if (src->mDisplays[display].themes[theme].palette)
            {
                theme_info *des_tinfo = &new_project->mDisplays[display].themes[theme];
                const theme_info *src_tinfo = &src->mDisplays[display].themes[theme];

                switch(src->mDisplays[display].colorformat)
                {
                case GX_COLOR_FORMAT_8BIT_PALETTE:
                    new_project->mDisplays[display].themes[theme].palette = new GX_COLOR[256];
                    break;

                case GX_COLOR_FORMAT_4BIT_GRAY:
                    new_project->mDisplays[display].themes[theme].palette = new GX_COLOR[16];
                    break;

                default:
                    new_project->mDisplays[display].themes[theme].palette = new GX_COLOR[2];
                    break;
                }

                memcpy_s(des_tinfo->palette, des_tinfo->palette_total_size * sizeof(GX_COLOR), src_tinfo->palette, src_tinfo->palette_total_size * sizeof(GX_COLOR));
            }

            for (index = 0; index < MAX_LANGUAGES; index++)
            {
                new_project->mDisplays[display].gen_string_table[index] = src->mDisplays[display].gen_string_table[index];
            }
        }
        new_project->mDisplays[display].xres = src->mDisplays[display].xres;
        new_project->mDisplays[display].yres = src->mDisplays[display].yres;
        new_project->mDisplays[display].bits_per_pix = src->mDisplays[display].bits_per_pix;
        new_project->mDisplays[display].grayscale = src->mDisplays[display].grayscale;
        new_project->mDisplays[display].packed_format = src->mDisplays[display].packed_format;
        new_project->mDisplays[display].reverse_order = src->mDisplays[display].reverse_order;
        new_project->mDisplays[display].format_555 = src->mDisplays[display].format_555;
        new_project->mDisplays[display].format_4444 = src->mDisplays[display].format_4444;
        new_project->mDisplays[display].format_332 = src->mDisplays[display].format_332;
        new_project->mDisplays[display].colorformat = src->mDisplays[display].colorformat;
        new_project->mDisplays[display].SetFirstChildFolder(NULL);
        new_project->mDisplays[display].enabled = src->mDisplays[display].enabled;
        new_project->mDisplays[display].default_map_format = src->mDisplays[display].default_map_format;
        new_project->mDisplays[display].allocate_canvas = src->mDisplays[display].allocate_canvas;

        new_project->mDisplays[display].stable = NULL;
        new_project->mDisplays[display].screenflow = NULL;
        
        // clone the widget info
        if (src->mDisplays[display].GetFirstChildFolder())
        {
            folder_info *folder = new folder_info(*src->mDisplays[display].GetFirstChildFolder(), TRUE);
            new_project->mDisplays[display].SetFirstChildFolder(folder);
        }

        // clone the resource info

        for (theme = 0; theme < src->mDisplays[display].num_themes; theme++)
        {
            if (src->mDisplays[display].themes[theme].GetFirstResourceInfo())
            {
                res_info * info = new res_info(NULL, *src->mDisplays[display].themes[theme].GetFirstResourceInfo(), TRUE);
                if (info)
                {
                    new_project->mDisplays[display].themes[theme].SetFirstResourceInfo(info);

                }
            }
        }

        // clone the string table:
        if (src->mDisplays[display].stable)
        {
            new_project->mDisplays[display].stable = new string_table(*src->mDisplays[display].stable);
        }

        // clone the screen flow information
        if (src->mDisplays[display].screenflow)
        {
            new_project->mDisplays[display].screenflow = new screen_flow(*src->mDisplays[display].screenflow);
        }
    }
    return new_project;
}

////////////////////////////////////////////////////////////////////////////////
PIXELMAP_RECORD *studiox_project::GetDefaultPixelmapRecord(CString name)
{
    // Retrieve default pixelmap record according to id name.

    PIXELMAP_RECORD *record = DEFAULT_PIXELMAP_TABLE;

    while (record->name && record->image_info)
    {
        if (record->name == name)
        {
            return record;
        }

        record++;
    }

    return NULL;
}


////////////////////////////////////////////////////////////////////////////////
void studiox_project::CheckEmptyScreenFlow()
{
    int item_count;
    int index;
    flow_item *item;
    BOOL no_triggers;

    for (int display = 0; display < mHeader.num_displays; display++)
    {
        screen_flow *flow = mDisplays[display].screenflow;
        if (flow)
        {
            item_count = flow->GetFlowListCount();

            if (!item_count)
            {
                delete flow;
                mDisplays[display].screenflow = NULL;
            }
            else
            {
                no_triggers = TRUE;
                for (index = 0; index < item_count; index++)
                {
                    item = flow->GetFlowItem(index);
                    if (item->trigger_list)
                    {
                        no_triggers = FALSE;
                        break;
                    }
                }

                if (no_triggers)
                {
                    delete flow;
                    mDisplays[display].screenflow = NULL;
                }
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void studiox_project::CleanupThemeResources(display_info *display, int ThemeIndex)
{
    res_info *current = display->themes[ThemeIndex].GetFirstResourceInfo();
    res_info *next;

    while (current)
    {
        next = current->next;
        delete current;
        current = next;
    }

    if (display->themes[ThemeIndex].palette)
    {
        delete[] display->themes[ThemeIndex].palette;
        display->themes[ThemeIndex].palette = NULL;
    }

    display->themes[ThemeIndex].SetFirstResourceInfo(NULL);
}


///////////////////////////////////////////////////////////////////////////////
void studiox_project::CleanupDisplayResources(display_info *display)
{
    
    //Clean up resources of all themes of the given display
    for (int theme = 0; theme < display->num_themes; theme++)
    {
        CleanupThemeResources(display, theme);
    }
}

///////////////////////////////////////////////////////////////////////////////
// This function initializes font and pixelmap resources by reading a converting
// the resource source file into a pixelmap or GX_FONT.
//
// This is done after a project is cloned, or after a project file is read,
// rather than doing it inline because it takes time and the cloned project
// might never even be used.
// 
// Called when a new project is opened
///////////////////////////////////////////////////////////////////////////////

static DWORD WINAPI InitializeProjectResourcesThreadEntry(LPVOID thread_input)
{
    studiox_project *project = GetOpenProject();

    if (project)
    {
        project->InitializeAllPixelmaps();

        for (int display = 0; display < MAX_DISPLAYS; display++)
        {
            if (display >= project->mHeader.num_displays)
            {
                project->mHeader.warn_missing_font = FALSE;
            }
            else
            {
                project->mHeader.warn_missing_font = TRUE;
            }

            for (int theme = 0; theme < MAX_THEMES; theme++)
            {
                project->InitializeFonts(project->mDisplays[display].themes[theme].GetFirstResourceInfo(), display);
            }
        }
    }

    EndBusyMsg();

    return TRUE;
}

void studiox_project::InitializeProjectResources()
{
    StartWorkThread(InitializeProjectResourcesThreadEntry, 0, "Initializing Resources.", TRUE);
}

///////////////////////////////////////////////////////////////////////////////
// This is called by the configure_theme_dialog when the user adds a new theme
void studiox_project::InitializeThemeResources(int display, int theme, res_info *start)
{
    InitializeThemePixelmaps(GetDisplayIndex(start), theme);
    InitializeFonts(start, display);
}

///////////////////////////////////////////////////////////////////////////////
void studiox_project::ConfigureDefaultFont(res_info *put, int display)
{
    GX_FONT *def_font = NULL;

    switch(put->font_bits)
    {
    case 1:
        if (IsDave2dFontFormat(GetOpenProject(), display))
        {
            def_font = &_gx_dave2d_system_font_mono;
        }
        else
        {
            def_font = &_gx_system_font_mono;
        }
        put->storage_size = SYSTEM_FONT_MONO_STORAGE;
        break;

    case 4:
        if (IsRenesasDave2D(GetOpenProject()))
        {
            def_font = &_gx_dave2d_system_font_4bpp;
        }
        else
        {
            def_font = &_gx_system_font_4bpp;
        }
        put->storage_size = SYSTEM_FONT_4BPP_STORAGE;
        break;

    case 8:
        def_font = &_gx_system_font_8bpp;
        put->storage_size = SYSTEM_FONT_8BPP_STORAGE;
        break;
    }

    put->font = def_font;
    put->font_height = def_font->gx_font_line_height;
    put->font_bits = GetFontBits(def_font->gx_font_format);
    put->compress = FALSE;
    put->font_kerning = FALSE;

    if (put->font_pages)
    {
        put->font_pages[0].first_char = def_font->gx_font_first_glyph;
        put->font_pages[0].last_char = def_font->gx_font_last_glyph;
    }
}

///////////////////////////////////////////////////////////////////////////////
void studiox_project::InitializeFonts(res_info *info, int display)
{
    GX_FONT *font;

    while(info)
    {
        if (info->type == RES_TYPE_FONT)
        {
            // first clean up any existing font
            if (info->font)
            {
                if (!info->is_default || !info->pathinfo.pathname.IsEmpty())
                {
                    DestroyFont(info->font);
                }
            }
            if (info->is_default && info->pathinfo.pathname.IsEmpty())
            {
                ConfigureDefaultFont(info, display);
            }
            else
            {
                font = MakeFont(info, display, mHeader.warn_missing_font);
                info->font = font;

                /* Get output font size. */
                if (!info->font_charset_include_string_table)
                {
                    info->storage_size = GetFontStorage(info, this, display);
                }

                if (!font)
                {
                    // just warn one time
                    mHeader.warn_missing_font = FALSE;
                }
            }
        }
        if (info->child)
        {
            InitializeFonts(info->child, display);
        }
        info = info->next;
    }
}


///////////////////////////////////////////////////////////////////////////////
void studiox_project::AssignSharedPaletteToPixelmaps(res_info *info)
{
    while(info)
    {
        if (info->type == RES_TYPE_PIXELMAP)
        {
            info->palette_type = PALETTE_TYPE_SHARED;
        }
        if (info->child)
        {
            AssignSharedPaletteToPixelmaps(info->child);
        }
        info = info->next;
    }
}


///////////////////////////////////////////////////////////////////////////////
void studiox_project::CheckAssignSharedPaletteToPixelmaps()
{
    int display;
    int theme;
    int display_color_format;

    /* private and shared palettes are only supported when running
       at 16 bpp and higher color depths. In 8 bit palette mode,
       we only support a single global palette right now (needs to be improved).
    */
    for (display = 0; display <  MAX_DISPLAYS; display++)
    {
        display_color_format = mDisplays[display].colorformat;

        if (display_color_format == GX_COLOR_FORMAT_8BIT_PALETTE)
        {
            for (theme = 0; theme < MAX_THEMES; theme++)
            {
                AssignSharedPaletteToPixelmaps(mDisplays[display].themes[theme].GetFirstResourceInfo());
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
BOOL studiox_project::InitializeOnePixelmap(res_info *info, palette_info *theme_palette, int display_color_format)
{
    int display_index = -1;
    int color_format;
    palette_creater palCreator;
    palette_info private_palette;

    GX_PIXELMAP *pmap = NULL;

    if (info->GetPixelmapFrameCount())
    {
        pixelmap_list_destroy(info->map_list);
        info->map_delay_list.RemoveAll();
    }
    if (info->thumbnail)
    {
        pixelmap_destroy(info->thumbnail);
        info->thumbnail = NULL;
    }

    if (display_color_format == -1)
    {
        display_color_format = GetDisplayColorFormat(info);
    }        

    // test for Dave2D incompatible format, force output format to format that will 
    // work with Dave2d. This test is only required because old project may have
    // incompatible format, before we were doing the right checks

    if (IsRenesasDave2D(this))
    {
        if (display_color_format == GX_COLOR_FORMAT_565RGB)
        {
            if (info->keep_alpha)
            {
                if (!IsAlphaFormat(info->output_color_format))
                {
                    info->output_color_format = GX_COLOR_FORMAT_32ARGB;
                }
            }
        }
        else
        {
            if ((display_color_format == GX_COLOR_FORMAT_24XRGB) || (display_color_format == GX_COLOR_FORMAT_32ARGB))
            {
                if (info->keep_alpha && (info->output_color_format == GX_COLOR_FORMAT_565RGB))
                {
                    info->output_color_format = GX_COLOR_FORMAT_32ARGB;
                }
            }
        }
    }
   
    info->storage_size = 0;

    image_reader *pReader = NULL;
    CString abspath;
    IMAGE_INFO *default_image_info = NULL;
    int frame_count = 1;
    int frame_id = -1;

    if (info->pathinfo.pathname.IsEmpty())
    {
        if (info->is_default)
        {
            // Read image from internally linked system png data.
            PIXELMAP_RECORD *record = GetDefaultPixelmapRecord(info->name);
            if (record)
            {
                default_image_info = record->image_info;
                pReader = image_reader::CreateProperReader(default_image_info->data, default_image_info->data_len);
            }
        }
    }
    else
    {
        abspath = MakeAbsolutePathname(info->pathinfo);
        pReader = image_reader::CreateProperReader(abspath);

        if (info->raw)
        {
            frame_id = 0;
        }
        else
        {
            frame_count = image_reader::GetFrameCount(abspath);
        }
    }

    BOOL result = FALSE;

    if (pReader)
    {
        if (info->output_color_format)
        {
            color_format = info->output_color_format;

            // info passed from palette_creater do not have a parent
            if (info->parent)
            {
                display_index = GetDisplayIndex(info);
                mDisplays[display_index].default_map_format = FALSE;
            }
        }
        else
        {
            color_format = display_color_format;
        }

        pReader->SetDither(info->dither);

        if (info->keep_alpha)
        {
            pReader->SetSaveAlphaVal(TRUE);
        }
        else
        {
            pReader->SetSaveAlphaVal(FALSE);
        }

        pReader->SetOutputColorFormat(color_format, display_color_format);

        if (color_format == GX_COLOR_FORMAT_8BIT_PALETTE)
        {
            switch(info->palette_type)
            {
            case PALETTE_TYPE_PRIVATE:
                palCreator.CreatePaletteForOnePixelmap(info, &private_palette);
                pReader->SetPalette(private_palette.palette, private_palette.total_size, private_palette.used_size);
                break;

            case PALETTE_TYPE_SHARED:
                if (theme_palette)
                {
                    if (display_color_format > GX_COLOR_FORMAT_8BIT_PALETTE)
                    {
                        // If the display color format is > 8bpp palette, then the image requires
                        // image.pAuxData points at the palette. This might be a shared or a private palette.
                        // When we delete the image, we don't know if the palette is shared, so make a copy
                        // of the palette so that we can always delete it when the image is deleted.
                        private_palette.palette = new GX_COLOR[theme_palette->total_size];
                        memcpy_s(private_palette.palette, theme_palette->total_size * sizeof(GX_COLOR), theme_palette->palette, (theme_palette->total_size * sizeof(GX_COLOR)));
                        pReader->SetPalette(private_palette.palette,
                                    theme_palette->total_size,
                                    theme_palette->used_size);
                    }
                    else
                    {
                        // if the display color format is 8bpp palette, then the image pAuxData will be set to NULL,
                        // because the palette is a global palette that is not attached to any image.
                        pReader->SetPalette(theme_palette->palette,
                                    theme_palette->total_size,
                                    theme_palette->used_size);
                    }
                }
                break;

            default:
                ErrorMsg("Internal Error: Invalid image palette type");
                delete pReader;
                return FALSE;
            }
        }

        if (default_image_info)
        {
            result = pReader->ReadImage(default_image_info->data, default_image_info->data_len);
        }
        else
        {
            result = pReader->ReadImage(abspath, frame_id);
        }
    }

    if (result)
    {
        for (int index = 0; index < frame_count; index++)
        {
            pmap = pReader->GetPixelmap(index);
            info->map_list.Add(pmap);

            if (frame_count > 1)
            {
                info->map_delay_list.Add(pReader->GetDelayTime(index));
            }

            if (!info->thumbnail)
            {

                // Make thumbnail.
                if (pmap->gx_pixelmap_width > THUMBNAIL_SIZE ||
                    pmap->gx_pixelmap_height > THUMBNAIL_SIZE)
                {
                    int xstretch = pmap->gx_pixelmap_width * 100 / THUMBNAIL_SIZE;
                    int ystretch = pmap->gx_pixelmap_height * 100 / THUMBNAIL_SIZE;

                    if (xstretch >= ystretch)
                    {
                        ystretch = pmap->gx_pixelmap_height * 100 / xstretch;
                        xstretch = pmap->gx_pixelmap_width * 100 / xstretch;
                    }
                    else
                    {
                        xstretch = pmap->gx_pixelmap_width * 100 / ystretch;
                        ystretch = pmap->gx_pixelmap_height * 100 / ystretch;
                    }
                    GX_PIXELMAP* thumbnail = pReader->ResizeImage(pmap, xstretch, ystretch);

                    if (thumbnail)
                    {
                        info->thumbnail = thumbnail;
                    }
                }
            }

            // now compress the image
            if (info->compress)
            {
                BOOL result;

                pReader->DoCompress(TRUE);
                if (IsRenesasDave2D(this))
                {
                    result = pReader->RleEncode(pmap, TRUE);
                }
                else
                {
                    result = pReader->RleEncode(pmap);
                }

                if (display_index < 0)
                {
                    display_index = GetDisplayIndex(info);
                }

                if ((mDisplays[display_index].rotation_angle == GX_SCREEN_ROTATION_NONE) && (result == FALSE))
                {
                    info->compress = FALSE;
                }
            }
        }

        /* Calculate pixelmap storage */
        info->storage_size = GetPixelmapStorage(info);
    }
    else
    {
        if (mHeader.warn_missing_image)
        {
            CString msg;
            msg.Format(_T("Pixelmap Name = %s\nUnable to open Image file: %s"), info->name, abspath);
            ErrorMsg(msg);

            // just issue one warning
            mHeader.warn_missing_image = FALSE;
        }
    }

    if (pReader)
    {
        delete pReader;
    }

    return result;
}


///////////////////////////////////////////////////////////////////////////////
// This function is called when the user changes the display color depth
// settings. We need to re-read all of our pixelmaps to make sure they are
// formatted correctly for each display.
///////////////////////////////////////////////////////////////////////////////
void studiox_project::InitializePixelmaps(res_info *info, palette_info *theme_palette)
{
    while(info)
    {
        if (info->type == RES_TYPE_PIXELMAP)
        {
            InitializeOnePixelmap(info, theme_palette);
        }
        if (info->child)
        {
            InitializePixelmaps(info->child, theme_palette);
        }
        info = info->next;
    }
}

///////////////////////////////////////////////////////////////////////////////
// CreateThemePalette
//
// Called by InitializeThemePixelmaps and by resource_gen::WritePalette
void studiox_project::CreateThemePalette(int display, int theme, palette_info *theme_palette)
{
    res_info *info;
    palette_creater palCreator;
    palette_info image_palette;
    int free_slots;

    // if this theme does not yet have a palette, create a default palette:
    if (mDisplays[display].themes[theme].palette == NULL)
    {
        ProjectConfigDlg::CreateDefaultPalette(this, display, theme);
    }

    info = mDisplays[display].themes[theme].GetFirstResourceInfo();

    if (!info || mDisplays[display].themes[theme].palette == NULL)
    {
        return;
    }            
    // create optimal palette for this display[theme]
    free_slots = mDisplays[display].themes[theme].palette_total_size;
    free_slots -= mDisplays[display].themes[theme].palette_predefined + 1;

    if (free_slots > 0)
    {
        palCreator.CreatePaletteForPixelmaps(info, &image_palette, TRUE, free_slots);

        if (image_palette.palette)
        {
            // safety check, should not happen
            if (image_palette.used_size > free_slots)
            {
                image_palette.used_size = free_slots;
            }

            // merge the optimal palette with our reserved palette:
            theme_info* tinfo = &mDisplays[display].themes[theme];
            GX_COLOR* put = tinfo->palette;
            put += tinfo->palette_predefined;
            memcpy_s(put, tinfo->palette_total_size * sizeof(GX_COLOR), image_palette.palette, image_palette.used_size * sizeof(GX_COLOR));
            delete[] image_palette.palette;
        }
    }

    // the theme_palette is the merged predefined and image palette:
    if (theme_palette)
    {
        theme_palette->palette = mDisplays[display].themes[theme].palette;
        theme_palette->total_size = mDisplays[display].themes[theme].palette_total_size;
        theme_palette->used_size = mDisplays[display].themes[theme].palette_total_size;
    }
}

///////////////////////////////////////////////////////////////////////////////
BOOL studiox_project::TestForPixelmapsUsingGlobalPalette(const res_info *info) const
{
    while(info)
    {
        if (info->type == RES_TYPE_PIXELMAP)
        {
            if ((info->palette_type == PALETTE_TYPE_SHARED) && (!info->raw))
            {
                return TRUE;
            }
        }
        if (info->child)
        {
            if (TestForPixelmapsUsingGlobalPalette(info->child))
            {
                return TRUE;
            }
        }
        info = info->next;
    }
    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
void studiox_project::InitializeThemePixelmaps(int display, int theme)
{
    res_info *info;
    int display_color_format;
    palette_info theme_palette;
    BOOL need_palette = FALSE;

    info = mDisplays[display].themes[theme].GetFirstResourceInfo();

    if (info)
    {
        display_color_format = mDisplays[display].colorformat;

        if (display_color_format == GX_COLOR_FORMAT_8BIT_PALETTE)
        {
            need_palette = TRUE;
        }
        else
        {
            if (display_color_format >= GX_COLOR_FORMAT_5551BGRX &&
                TestForPixelmapsUsingGlobalPalette(info))
            {
                need_palette = TRUE;
            }
        }
        if (need_palette)
        {
            CreateThemePalette(display, theme, &theme_palette);
        }
        else
        {
            // this theme doesn't need a palette. If it has one, delete it
            if (mDisplays[display].themes[theme].palette)
            {
                delete [] mDisplays[display].themes[theme].palette;
                mDisplays[display].themes[theme].palette = NULL;
                mDisplays[display].themes[theme].palette_total_size = 0;
            }
        }

        InitializePixelmaps(info, &theme_palette);
    }
}

void studiox_project::TaskInitializeAllPixelmaps()
{
    // This is done in case we somehow converted the project to 8bpp palette
    // mode but did not correctly reset the palette type. Shouldn't happen,
    // but just in case!
    CheckAssignSharedPaletteToPixelmaps();

    for (int display = 0; display < MAX_DISPLAYS; display++)
    {
        if (display >= mHeader.num_displays)
        {
            mHeader.warn_missing_image = FALSE;
        }
        else
        {
            mHeader.warn_missing_image = TRUE;
        }
        for (int theme = 0; theme < MAX_THEMES; theme++)
        {
            InitializeThemePixelmaps(display, theme);
        }
    }
}

static DWORD WINAPI TaskInitializeAllPixelmapsThreadEntry(LPVOID thread_input)
{
    studiox_project* project = GetOpenProject();

    if (project)
    {
        project->TaskInitializeAllPixelmaps();
    }

    EndBusyMsg();

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// Called when the display format is changed.
void studiox_project::InitializeAllPixelmaps()
{
    StartWorkThread(TaskInitializeAllPixelmapsThreadEntry, 0, "Initializing Pixelmaps.", TRUE);
}

///////////////////////////////////////////////////////////////////////////////
static DWORD WINAPI InitializeFontsThreadEntry(LPVOID thread_input)
{
    studiox_project* project = GetOpenProject();

    if (project)
    {
        int display;
        int theme;
        res_info* info;
        project->mHeader.warn_missing_font = TRUE;

        for (display = 0; display < MAX_DISPLAYS; display++)
        {
            if (display > project->mHeader.num_displays - 1)
            {
                project->mHeader.warn_missing_font = FALSE;
            }
            for (theme = 0; theme < MAX_THEMES; theme++)
            {
                info = project->mDisplays[display].themes[theme].GetFirstResourceInfo();

                if (info)
                {
                    project->InitializeFonts(info, display);
                }
            }
        }
    }

    EndBusyMsg();

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
void studiox_project::InitializeFonts()
{
    StartWorkThread(InitializeFontsThreadEntry, 0, "Initializing Fonts.", TRUE);
}

///////////////////////////////////////////////////////////////////////////////
void studiox_project::DefaultScrollbarAppearance(display_info *pInfo, int theme)
{
    memset(&pInfo->themes[theme].VScrollAppearance, 0, sizeof(GX_SCROLLBAR_APPEARANCE));
    pInfo->themes[theme].VScrollAppearance.gx_scroll_width = 20;
    pInfo->themes[theme].VScrollAppearance.gx_scroll_thumb_travel_min = 20;
    pInfo->themes[theme].VScrollAppearance.gx_scroll_thumb_travel_max = 20;
    pInfo->themes[theme].VScrollAppearance.gx_scroll_thumb_width = 18;
    pInfo->themes[theme].VScrollAppearance.gx_scroll_thumb_color = GX_COLOR_ID_SCROLL_BUTTON;
    pInfo->themes[theme].VScrollAppearance.gx_scroll_thumb_border_color = GX_COLOR_ID_SCROLL_BUTTON;
    pInfo->themes[theme].VScrollAppearance.gx_scroll_button_color = GX_COLOR_ID_SCROLL_BUTTON;
    pInfo->themes[theme].VScrollAppearance.gx_scroll_thumb_border_style = GX_STYLE_BORDER_THIN;
    pInfo->themes[theme].VScrollStyle =  GX_SCROLLBAR_VERTICAL | GX_SCROLLBAR_RELATIVE_THUMB | GX_SCROLLBAR_END_BUTTONS;

    memset(&pInfo->themes[theme].HScrollAppearance, 0, sizeof(GX_SCROLLBAR_APPEARANCE));
    pInfo->themes[theme].HScrollAppearance.gx_scroll_width = 20;
    pInfo->themes[theme].HScrollAppearance.gx_scroll_thumb_travel_min = 20;
    pInfo->themes[theme].HScrollAppearance.gx_scroll_thumb_travel_max = 20;
    pInfo->themes[theme].HScrollAppearance.gx_scroll_thumb_width = 18;
    pInfo->themes[theme].HScrollAppearance.gx_scroll_thumb_color = GX_COLOR_ID_SCROLL_BUTTON;
    pInfo->themes[theme].VScrollAppearance.gx_scroll_thumb_border_color = GX_COLOR_ID_SCROLL_BUTTON;
    pInfo->themes[theme].HScrollAppearance.gx_scroll_button_color = GX_COLOR_ID_SCROLL_BUTTON;
    pInfo->themes[theme].HScrollAppearance.gx_scroll_thumb_border_style = GX_STYLE_BORDER_THIN;
    pInfo->themes[theme].HScrollStyle =  GX_SCROLLBAR_HORIZONTAL | GX_SCROLLBAR_RELATIVE_THUMB | GX_SCROLLBAR_END_BUTTONS;
}

///////////////////////////////////////////////////////////////////////////////
void studiox_project::InitDisplayThemes(int DisplayIndex)
{
    CString theme_name;
    display_info *pInfo = &mDisplays[DisplayIndex];
    pInfo->num_themes = 1;
    pInfo->active_theme = DEFAULT_THEME;

    for (int theme = 0; theme < MAX_THEMES; theme++)
    {
        theme_name.Format(_T("theme_%d"), theme + 1);
        mDisplays[DisplayIndex].themes[theme].theme_name = theme_name;
        mDisplays[DisplayIndex].themes[theme].palette = NULL;
        mDisplays[DisplayIndex].themes[theme].palette_total_size = 0;
        mDisplays[DisplayIndex].themes[theme].SetFirstResourceInfo(NULL);
        mDisplays[DisplayIndex].themes[theme].gen_color_table = TRUE;
        mDisplays[DisplayIndex].themes[theme].gen_font_table = TRUE;
        mDisplays[DisplayIndex].themes[theme].gen_pixelmap_table = TRUE;
        mDisplays[DisplayIndex].themes[theme].enabled = TRUE;
        mDisplays[DisplayIndex].themes[theme].statically_defined = TRUE;
        DefaultScrollbarAppearance(pInfo, theme);
    }
}

///////////////////////////////////////////////////////////////////////////////
void studiox_project::CreateDefaultResources(int DisplayIndex, int ThemeIndex)
{
    display_info *pInfo = &mDisplays[DisplayIndex];
    CleanupThemeResources(pInfo, ThemeIndex);

    pInfo->colorformat = GX_COLOR_FORMAT_565RGB;

    /* Add theme header */
    res_info *pThemeHeader = new res_info(RES_TYPE_HEADER);
    pThemeHeader->folder_id = THEME_HEADER;
    pThemeHeader->name = pInfo->themes[ThemeIndex].theme_name;
    pInfo->themes[ThemeIndex].SetFirstResourceInfo(pThemeHeader);

    /* Add the Color group */
    res_info *pGroup = new res_info(RES_TYPE_GROUP);
    pGroup->name = _T("Colors");
    pGroup->folder_id = COLOR_GROUP;

    pThemeHeader->next = pGroup;
    //pInfo->themes[ThemeIndex].first_resource = pGroup;

    /* add default color folder */
    res_info *pDefaultColors = new res_info(RES_TYPE_FOLDER);
    pDefaultColors->folder_id = DEFAULT_COLOR_FOLDER;
    pDefaultColors->name = _T("Defaults");
    pGroup->Attach(pDefaultColors);

    /* add default colors to folder */
    COLOR_RECORD *pRecord = DEFAULT_COLOR_TABLE;

    if (pRecord)
    {
        while (pRecord->name)
        {
            res_info *pColor = new res_info(RES_TYPE_COLOR);
            pColor->name = pRecord->name;
            pColor->colorval = pRecord->rgb_val;
            pColor->is_default = TRUE;
            pDefaultColors->Attach(pColor);
            if (ThemeIndex == 0)
            {
                AddToResourceDictionary(DisplayIndex, pColor);
            }
            pRecord++;
        }
    }

    /* add custom color folder */

    res_info *pCustomColors = new res_info(RES_TYPE_FOLDER);
    pCustomColors->folder_id = CUSTOM_COLOR_FOLDER;
    pCustomColors->name = _T("Custom");
    pGroup->Attach(pCustomColors);

    // Add color add item
    res_info* pColorAdd = new res_info(RES_TYPE_ADD_COLOR);
    pCustomColors->Attach(pColorAdd);

    /* Add the font group */

    res_info *pFontGroup = new res_info(RES_TYPE_GROUP);
    pFontGroup->name = _T("Fonts");
    pFontGroup->folder_id = FONT_GROUP;
    pGroup->next = pFontGroup;
    pGroup = pFontGroup;

    /* Add default font folder to font group */
    res_info *pDefaultFontFolder = new res_info(RES_TYPE_FOLDER);
    pDefaultFontFolder->name = _T("Defaults");
    pDefaultFontFolder->folder_id = DEFAULT_FONT_FOLDER;
    pFontGroup->Attach(pDefaultFontFolder);

    /* Add the default fonts to the default font folder */
    FONT_RECORD *pFontRec = DEFAULT_FONT_TABLE;

    while (pFontRec->font)
    {
        res_info *pFont = new res_info(RES_TYPE_FONT);
        pFont->name = pFontRec->name;
        pFont->font = pFontRec->font;
        pFont->font_bits = 8;
        pFont->is_default = TRUE;
        pFont->font_pages = font_path_dialog::CreateDefaultFontPages();
        pFont->font_pages[0].first_char = pFontRec->font->gx_font_first_glyph;
        pFont->font_pages[0].last_char = pFontRec->font->gx_font_last_glyph;
        pFont->font_height = pFontRec->font->gx_font_line_height;
        pDefaultFontFolder->Attach(pFont);
        if (ThemeIndex == 0)
        {
            AddToResourceDictionary(DisplayIndex, pFont);
        }
        pFontRec++;
    }

    /* Add custom font folder to font group */
    res_info *pCustomFontFolder = new res_info(RES_TYPE_FOLDER);
    pCustomFontFolder->name = _T("Custom");
    pCustomFontFolder->folder_id = CUSTOM_FONT_FOLDER;
    pFontGroup->Attach(pCustomFontFolder);

    // Add font add item
    res_info* pFontAdd = new res_info(RES_TYPE_ADD_FONT);
    pCustomFontFolder->Attach(pFontAdd);

    /* Add the pixelmap group */

    res_info *pPixGroup = new res_info(RES_TYPE_GROUP);
    pPixGroup->name = _T("Pixelmaps");
    pPixGroup->folder_id = PIXELMAP_GROUP;
    pGroup->next = pPixGroup;
    pGroup = pPixGroup;

    /* Add system pixelmap folder to pixmap group */
    res_info *pSysMaps = new res_info(RES_TYPE_FOLDER);
    pSysMaps->name = _T("System");
    pSysMaps->folder_id = DEFAULT_PIXELMAP_FOLDER;
    pPixGroup->Attach(pSysMaps);

    /* Add the default pixelmaps to the system pixelmap folder */
    PIXELMAP_RECORD *pPixRec = DEFAULT_PIXELMAP_TABLE;
    while (pPixRec->name && pPixRec->image_info)
    {
        res_info *pmap = new res_info(RES_TYPE_PIXELMAP);
        pmap->name = pPixRec->name;
        pmap->keep_alpha = pPixRec->include_alpha;
        pmap->is_default = TRUE;
        pSysMaps->Attach(pmap);

        if (ThemeIndex == 0)
        {
            AddToResourceDictionary(DisplayIndex, pmap);
        }
        pPixRec++;
    }

    /* Add user pixelmap folder to pixmap group */
    res_info *pUserMaps = new res_info(RES_TYPE_FOLDER);
    pUserMaps->name = _T("Custom");
    pUserMaps->folder_id = CUSTOM_PIXELMAP_FOLDER;
    pPixGroup->Attach(pUserMaps);

    // Add pixelmap add item
    res_info* pPixelmapAdd = new res_info(RES_TYPE_ADD_PIXELMAP);
    pUserMaps->Attach(pPixelmapAdd);

    /* Add the string group */

    res_info *pStringGroup = new res_info(RES_TYPE_GROUP);
    pStringGroup->name = _T("Strings");
    pStringGroup->folder_id = STRING_GROUP;
    pGroup->next = pStringGroup;

    // Add string add item
    res_info* pStringAdd = new res_info(RES_TYPE_ADD_STRING);
    pStringGroup->Attach(pStringAdd);

    /* create a string table instance */
    if (!pInfo->stable)
    {
        pInfo->stable = new string_table();
        pInfo->stable->Initialize(1, 1);
    }
}

///////////////////////////////////////////////////////////////////////////////
int studiox_project::CountResources(int DisplayIndex, int type) const
{
    switch(type)
    {
    case RES_TYPE_COLOR:
        return color_dictionary[DisplayIndex].GetCount();

    case RES_TYPE_FONT:
        return font_dictionary[DisplayIndex].GetCount();

    case RES_TYPE_PIXELMAP:
        return pixelmap_dictionary[DisplayIndex].GetCount();

    default:
        return 0;
    }
}

///////////////////////////////////////////////////////////////////////////////
void studiox_project::LockUlockWidgetPositions(BOOL lock)
{
    mHeader.is_widget_position_locked = lock;
    SetModified();
}

///////////////////////////////////////////////////////////////////////////////
int studiox_project::AddLanguage(int name_index)
{
    int target_table_column = mHeader.num_languages;

    for (int display = 0; display < MAX_DISPLAYS; display++)
    {
        if (mDisplays[display].stable != NULL)
        {
            mDisplays[display].stable->AddLanguage();
        }
    }
    mHeader.num_languages += 1;
    mHeader.languages[target_table_column].name = config_languages_dlg::GetLanguageName(name_index);
    return target_table_column;
}

///////////////////////////////////////////////////////////////////////////////
res_info *studiox_project::AddCustomColor(GX_COLOR color, CString &name, res_info *parent)
{
    res_info *newcolor = NULL;

    GX_DISPLAY *display = get_target_view_display();

    int display_index = GetDisplayIndex(parent);

    newcolor = new res_info(RES_TYPE_COLOR);
    newcolor->colorval = color;
    newcolor->name = name;
    newcolor->compress = FALSE;
    newcolor->is_default = FALSE;
    parent->Attach(newcolor);
    AddToResourceDictionary(display_index, newcolor);
    SetModified();

    return newcolor;
}

///////////////////////////////////////////////////////////////////////////////
void studiox_project::DeleteResource(res_info *which)
{
    res_info *parent;
    res_info *previous;

    parent = which->parent;

    if (parent)
    {
        if (which->parent->child == which)
        {
            // first child of parent, the easy case
            which->parent->child = which->next;
        }
        else
        {
            // not first child, find previous sib:
            previous = which->parent->child;
            while(previous->next != which)
            {
                previous = previous->next;
            }
            previous->next = which->next;
        }
    }

    int display = GetDisplayIndex(which);
    which->parent = NULL;
    which->next = NULL;

    if (display >= 0)
    {
        if (parent && (parent->type == RES_TYPE_FOLDER))
        {
            // if parent is a folder, loop resources from folder group
            parent = parent->parent;
        }

        RemoveFromResourceDictionary(display, which);
    }
    delete which;
    SetModified();
}

///////////////////////////////////////////////////////////////////////////////
int studiox_project::CountEnabledThemes(int DisplayIndex) const
{
    USHORT count = 0;

    CCommandInfo *pCmdInfo = GetCmdInfo();
    BOOL enabled;

    for (int theme = 0; theme < mDisplays[DisplayIndex].num_themes; theme++)
    {
        if (pCmdInfo->IsNoGui())
        {
            enabled = pCmdInfo->IsThemeEnabled(mDisplays[DisplayIndex].themes[theme].theme_name);
        }
        else
        {
            enabled = mDisplays[DisplayIndex].themes[theme].enabled;
        }

        if (enabled)
        {
            count++;
        }
    }

    return count;
}

///////////////////////////////////////////////////////////////////////////////
int studiox_project::CountEnabledLanguages(int DisplayIndex) const
{
    USHORT count = 0;

    CCommandInfo *pCmdInfo = GetCmdInfo();
    BOOL enabled;

    for (int language = 0; language < mHeader.num_languages; language++)
    {
        if (pCmdInfo->IsNoGui())
        {
            enabled = pCmdInfo->IsLanguageEnabled(mHeader.languages[language].name);
        }
        else
        {
            enabled = mDisplays[DisplayIndex].gen_string_table[language];
        }

        if (enabled)
        {
            count++;
        }
    }

    return count;
}

///////////////////////////////////////////////////////////////////////////////
int studiox_project::CountEnabledDisplays() const
{
    USHORT count = 0;

    CCommandInfo *pCmdInfo = GetCmdInfo();
    BOOL enabled;

    for (int display = 0; display < mHeader.num_displays; display++)
    {
        if (pCmdInfo->IsNoGui())
        {
            enabled = pCmdInfo->IsDisplayEnabled(mDisplays[display].name);
        }
        else
        {
            enabled = mDisplays[display].enabled;
        }

        if (enabled)
        {
            count++;
        }
    }

    return count;
}


///////////////////////////////////////////////////////////////////////////////
int studiox_project::CountStaticallyDefinedThemes(int display)
{
    int count = 0;
    display_info *info = &mDisplays[display];
    for (int index = 0; index < info->num_themes; index++)
    {
        if (info->themes[index].statically_defined)
        {
            count++;
        }
    }

    return count;
}

///////////////////////////////////////////////////////////////////////////////
int studiox_project::CountStaticallyDefinedLanguages()
{
    int count = 0;
    for (int index = 0; index < mHeader.num_languages; index++)
    {
        if (mHeader.languages[index].statically_defined)
        {
            count++;
        }
    }

    return count;
}

///////////////////////////////////////////////////////////////////////////////
void studiox_project::DeleteFolder(folder_info *folder)
{
    int display_index = GetDisplayIndex(folder);
    folder_info *folder_item = mDisplays[display_index].GetFirstChildFolder();

    if (folder_item == folder)
    {
        folder_info *next = folder->GetNextFolder();
        mDisplays[display_index].SetFirstChildFolder(next);
    }
    else
    {
        folder_info *pre = NULL;
        
        while (folder_item)
        {
            if (folder_item->GetNextFolder() == folder)
            {
                folder_item->SetNextFolder(folder->GetNextFolder());
                break;
            }
            folder_item = folder_item->GetNextFolder();
        }
    }
    
    folder->SetNextFolder(NULL);
    delete folder;
    SetModified();
}

///////////////////////////////////////////////////////////////////////////////
void studiox_project::DeleteWidget(widget_info *which)
{
    widget_info *parent_info = NULL;
    parent_info = FindParentInfo(which);

    if (parent_info)
    {
        if (parent_info->GetChildWidgetInfo() == which)
        {
            // first child of parent, the easy case
            parent_info->SetChildWidgetInfo(which->GetNextWidgetInfo());
        }
        else
        {
            // not first child, find previous sib:
            widget_info *previous;
            previous = parent_info->GetChildWidgetInfo();
            while (previous->GetNextWidgetInfo() != which)
            {
                previous = previous->GetNextWidgetInfo();
            }
            previous->SetNextWidgetInfo(which->GetNextWidgetInfo());
        }
    }
    else
    {
        // here if we don't have parent info, should be a top-level widget.
        // unlink it from its parent folder
        folder_info *widget_folder;
        widget_info *current;

        widget_folder = FindParentFolderInfo(which);

        if (widget_folder)
        {
            widget_info *prev = NULL;
            current = widget_folder->GetFirstChildWidget();
            while (current)
            {
                if (current == which)
                {
                    if (prev)
                    {
                        prev->SetNextWidgetInfo(which->GetNextWidgetInfo());
                    }
                    else
                    {
                        widget_folder->SetFirstChildWidget(which->GetNextWidgetInfo());
                    }
                    break;
                }
                prev = current;
                current = current->GetNextWidgetInfo();
            }
        }
    }

    which->SetNextWidgetInfo(NULL);
    delete which;

    SetModified();
}

///////////////////////////////////////////////////////////////////////////////
void studiox_project::AddFolderToDisplay(int DisplayIndex, folder_info *info)
{
    if (DisplayIndex < MAX_DISPLAYS)
    {
        folder_info *next = mDisplays[DisplayIndex].GetFirstChildFolder();
        info->SetNextFolder(next);
        mDisplays[DisplayIndex].SetFirstChildFolder(info);
        GetProjectView()->InsertFolder(info);
        SetModified();
    }
}

///////////////////////////////////////////////////////////////////////////////
BOOL studiox_project::IsPaletteMode(int display) const
{
    if (display < MAX_DISPLAYS)
    {
        if (mDisplays[display].colorformat == GX_COLOR_FORMAT_8BIT_PALETTE)
        {
            return TRUE;
        }
    }
    return FALSE;
}


///////////////////////////////////////////////////////////////////////////////
void studiox_project::AddWidgetToFolder(folder_info *folder, widget_info *info)
{
    project_view *pview = GetProjectView();
    
    if (!pview)
    {
        return;
    }

    if (info->GetNextWidgetInfo())
    {
        pview->InsertTreeChild(folder, info);

        widget_info *last = info;
        while(last->GetNextWidgetInfo())
        {
            last = last->GetNextWidgetInfo();
        }
        last->SetNextWidgetInfo(folder->GetFirstChildWidget());
        folder->SetFirstChildWidget(info);
    }
    else
    {
        widget_info *next = folder->GetFirstChildWidget();
        info->SetNextWidgetInfo(next);
        folder->SetFirstChildWidget(info);
        pview->InsertTopLevelWidget(info);
    }

    SetModified();
}

///////////////////////////////////////////////////////////////////////////////
BOOL studiox_project::IsWidgetInInfoTree(const widget_info *start, const widget_info *find) const
{
    while(start)
    {
        if (start == find)
        {
            return TRUE;
        }
        if (start->GetChildWidgetInfo())
        {
            if (IsWidgetInInfoTree(start->GetChildWidgetInfo(), find))
            {
                return TRUE;
            }
        }
        start = start->GetNextWidgetInfo();
    }
    return FALSE;
}


///////////////////////////////////////////////////////////////////////////////
int studiox_project::GetDisplayIndex(const folder_info *folder) const
{
    for (int DisplayIndex = 0; DisplayIndex < MAX_DISPLAYS; DisplayIndex++)
    {
        folder_info *info = mDisplays[DisplayIndex].GetFirstChildFolder();
        while (info)
        {
            if (info == folder)
            {
                return DisplayIndex;
            }
            info = info->GetNextFolder();
        }
    }
    return -1;
}

///////////////////////////////////////////////////////////////////////////////
int studiox_project::GetDisplayIndex(const widget_info *info) const
{
    // find out which display this widget belongs to
    for (int DisplayIndex = 0; DisplayIndex < MAX_DISPLAYS; DisplayIndex++)
    {
        folder_info *folder = mDisplays[DisplayIndex].GetFirstChildFolder();
        while(folder)
        {
            if (IsWidgetInInfoTree(folder->GetFirstChildWidget(), info))
            {
                return DisplayIndex;
            }
            folder = folder->GetNextFolder();
        }
    }
    return -1;
}

///////////////////////////////////////////////////////////////////////////////
int studiox_project::GetDisplayIndex(const res_info *info) const
{
    // find out which display this widget belongs to
    // get top-level node
    while(info->parent)
    {
        info = info->parent;
    }

    for (int DisplayIndex = 0; DisplayIndex < MAX_DISPLAYS; DisplayIndex++)
    {
        // loop through top-level nodes
        for(int ThemeIndex = 0; ThemeIndex < mDisplays[DisplayIndex].num_themes; ThemeIndex++)
        {
            res_info *test = mDisplays[DisplayIndex].themes[ThemeIndex].GetFirstResourceInfo();
            while (test)
            {
                if (test == info)
                {
                    return DisplayIndex;
                }
                test = test->next;
            }
        }
    }
    ErrorMsg("Internal Error: failed to find display associated with resource");
    return -1;
}

///////////////////////////////////////////////////////////////////////////////
int studiox_project::GetDisplayColorFormat(const res_info *info) const
{
    int display = GetDisplayIndex(info);

    if (display >= 0 && display < MAX_DISPLAYS)
    {
        return mDisplays[display].colorformat;
    }
    return GX_COLOR_FORMAT_MONOCHROME;
}

///////////////////////////////////////////////////////////////////////////////
void studiox_project::AddWidgetToParent(widget_info *parent_info, widget_info *child_info)
{
    //info->next = NULL;

    if (!parent_info)
    {
        ErrorMsg("Internal Error: Invalid parent widget.");
        return;
    }

    if (parent_info->widget)
    {
        if (child_info->widget == NULL)
        {
            widget_factory::GenerateWidgets(parent_info->widget, child_info, TRUE, FALSE);
        }
    }

    if ((parent_info->basetype == GX_TYPE_MENU) &&
        (parent_info->ewi.menu.insert_as_menu_item))
    {
        if (parent_info->widget && child_info->widget)
        {
            gx_menu_insert((GX_MENU *)parent_info->widget, child_info->widget);
        }
        parent_info->ewi.menu.list_total_count++;
    }

    GetProjectView()->InsertTreeChild(parent_info, child_info);

    // assume the allocation type of the parent:
    if (mHeader.guix_version > 50000)
    {
        if (parent_info->allocation == STATICALLY_ALLOCATED)
        {
            properties_win::SetChildAllocation(child_info, STATICALLY_ALLOCATED);
        }
        else
        {
            properties_win::SetChildAllocation(child_info, DYNAMIC_ALLOCATION_CHILD);
        }
    }
    else
    {
        properties_win::SetChildAllocation(child_info, STATICALLY_ALLOCATED);
    }

    if (parent_info->GetChildWidgetInfo())
    {
        if ((parent_info->basetype == GX_TYPE_MENU) &&
            (parent_info->ewi.menu.insert_as_menu_item))
        {
            //if the inserted widget is a menu item, we should link
            //the widget to the last menu item
            int list_count = 0;
            int list_total_count = parent_info->ewi.menu.list_total_count;

            if (list_total_count == 0)
            {
                child_info->SetNextWidgetInfo(parent_info->GetChildWidgetInfo());
                parent_info->SetChildWidgetInfo(child_info);
            }
            else
            {
                widget_info *sib = parent_info->GetChildWidgetInfo();
                while (sib->GetNextWidgetInfo() && (list_count + 1 < list_total_count))
                {
                    sib = sib->GetNextWidgetInfo();
                    list_count++;
                }
                child_info->SetNextWidgetInfo(sib->GetNextWidgetInfo());
                sib->SetNextWidgetInfo(child_info);
            }
        }
        else
        {
            widget_info *sib = parent_info->GetChildWidgetInfo();
            while (sib->GetNextWidgetInfo())
            {
                sib = sib->GetNextWidgetInfo();
            }
            sib->SetNextWidgetInfo(child_info);
        }
    }
    else
    {
        parent_info->SetChildWidgetInfo(child_info);
    }

    SetModified();
}

///////////////////////////////////////////////////////////////////////////////
void studiox_project::MoveInfoToFront(widget_info *info)
{
    if (info->GetNextWidgetInfo() == NULL)
    {
        // nothing to do
        return;
    }
    // Move to front actually means we want this widget to be last in list:
    widget_info *parent = FindParentInfo(info);
    widget_info *previous = NULL;
    
    if (parent)
    {
        if (parent->GetChildWidgetInfo() == info)
        {
            parent->SetChildWidgetInfo(info->GetNextWidgetInfo());
        }

        // remove from linked list
        previous = parent->GetChildWidgetInfo();
        while(previous->GetNextWidgetInfo())
        {
            if (previous->GetNextWidgetInfo() == info)
            {
                previous->SetNextWidgetInfo(info->GetNextWidgetInfo());
            }
            previous = previous->GetNextWidgetInfo();
        }
        // and link at tail position
        previous->SetNextWidgetInfo(info);
        info->SetNextWidgetInfo(NULL);
        SetModified();
    }
}

///////////////////////////////////////////////////////////////////////////////
void studiox_project::MoveInfoToBack(widget_info *info)
{
    // Move to back actually means we want this widget to be first in child list:
    widget_info *parent = FindParentInfo(info);
    widget_info *previous = NULL;
    
    if (parent)
    {
        if (parent->GetChildWidgetInfo() == info)
        {
           // already in back, return
            return;
        }

        // remove from linked list
        previous = parent->GetChildWidgetInfo();
        while(previous->GetNextWidgetInfo())
        {
            if (previous->GetNextWidgetInfo() == info)
            {
                previous->SetNextWidgetInfo(info->GetNextWidgetInfo());
                break;
            }
            previous = previous->GetNextWidgetInfo();
        }
        // link at list head
        info->SetNextWidgetInfo(parent->GetChildWidgetInfo());
        parent->SetChildWidgetInfo(info);
        SetModified();
    }
}

///////////////////////////////////////////////////////////////////////////////
void studiox_project::WritePaletteType(xml_writer &writer, res_info *res)
{
    switch(res->palette_type)
    {
    case PALETTE_TYPE_PRIVATE:
        writer.WriteString("palette_type", "Private");
        break;

    case PALETTE_TYPE_SHARED:
        writer.WriteString("palette_type", "Shared");
        break;

    case PALETTE_TYPE_NONE:
    default:
        writer.WriteString("palette_type", "None");
        break;
    }
}



///////////////////////////////////////////////////////////////////////////////
void studiox_project::WriteOneResource(xml_writer &writer, res_info *res, GX_BOOL xml_mode)
{
    int page_count;

    writer.WriteString("type", ResTypeToString(res->type));
    writer.WriteString("name", res->name);
    writer.WritePathInfo(res->pathinfo);

    if(!xml_mode)
    {
        writer.WriteBool("is_default", res->is_default);
        writer.WriteBool("enabled", res->enabled);
    }
    writer.WriteBool("compress", res->compress);

    switch(res->type)
    {
    case RES_TYPE_PIXELMAP:
        writer.WriteBool("alpha", res->keep_alpha);
        writer.WriteBool("dither", res->dither);
        writer.WriteBool("raw", res->raw);
        writer.WriteString("color_format", resource_gen::GetColorFormatName(res->output_color_format));
        if (!xml_mode)
        {
            writer.WriteBool("output_file_enabled", res->output_file_enabled);
            writer.WriteString("output_file", res->output_file);
            writer.WriteBool("binary_mode", res->binary_mode);
        }
        WritePaletteType(writer, res);
        break;
    
    case RES_TYPE_COLOR:
        writer.WriteUnsigned("colorval", res->colorval);
        break;

    case RES_TYPE_FONT:
        writer.WriteInt("height", res->font_height);
        writer.WriteInt("font_bits", res->font_bits);
        writer.WriteBool("font_kerning", res->font_kerning);

        page_count = NUM_FONT_CHAR_RANGES;

        if (res->font_support_extended_unicode)
        {
            page_count += NUM_FONT_EXTENDED_CHAR_RANGES;
        }

        if (xml_mode)
        {
            for (int loop = 0; loop < page_count; loop++)
            {
                if (res->font_pages[loop].enabled)
                {
                    writer.OpenTag("font_page_data");
                    writer.WriteInt("first_char", res->font_pages[loop].first_char);
                    writer.WriteInt("last_char", res->font_pages[loop].last_char);
                    writer.CloseTag("font_page_data");
                }
            }
        }
        else
        {
            writer.WriteBool("font_include_st_glyphs", res->font_charset_include_string_table);
            writer.WriteBool("font_support_extended_unicode", res->font_support_extended_unicode);
            writer.WriteBool("output_file_enabled", res->output_file_enabled);
            writer.WriteString("output_file", res->output_file);
            writer.WriteBool("binary_mode", res->binary_mode);

            writer.OpenTag("font_page_data");

            for (int loop = 0; loop < page_count; loop++)
            {
                writer.WriteBool("enabled", res->font_pages[loop].enabled);
                writer.WriteInt("first_char", res->font_pages[loop].first_char);
                writer.WriteInt("last_char", res->font_pages[loop].last_char);
            }
            writer.CloseTag("font_page_data");
        }
        break;

    case RES_TYPE_ADD_COLOR:
    case RES_TYPE_ADD_FONT:
    case RES_TYPE_ADD_PIXELMAP:
    case RES_TYPE_ADD_STRING:
        break;

    default:
        writer.WriteString("folder_id", FindFolderIdString(res->type, res->folder_id));
        break;
    }
}

///////////////////////////////////////////////////////////////////////////////
void studiox_project::WriteThemeScrollbars(xml_writer &writer, int display, int theme)
{
    display_info *info = &mDisplays[display];

    writer.OpenTag("vscroll_appearance");
    vscroll_service_provider::WriteScrollbarAppearance(writer, this, display, info->themes[theme].VScrollAppearance);
    writer.WriteUnsigned("scroll_style", info->themes[theme].VScrollStyle);
    writer.CloseTag("vscroll_appearance");

    writer.OpenTag("hscroll_appearance");
    vscroll_service_provider::WriteScrollbarAppearance(writer, this, display, info->themes[theme].HScrollAppearance);
    writer.WriteUnsigned("scroll_style", info->themes[theme].HScrollStyle);
    writer.CloseTag("hscroll_appearance");
}

///////////////////////////////////////////////////////////////////////////////
void studiox_project::WriteResources(xml_writer &writer, res_info *start, GX_BOOL xml_mode)
{
    /* KGM notes:

        I decided to just write out ALL of the resource values, even
        if I don't think we will use the saved values for things like
        fixed colors. I think it's better to just have the writer save
        everything, and then the reader can pick and choose what it
        will use from the saved data. So if you notice that the reader is
        not using the Fixed color folder when it reads in a project, that's
        OK, that's how it's supposed to work.
    */

    while(start)
    {
        if (!xml_mode || start->type == RES_TYPE_FONT || start->type == RES_TYPE_PIXELMAP)
        {
            writer.OpenTag("resource");

            WriteOneResource(writer, start, xml_mode);
        }

        if (start->child)
        {
            WriteResources(writer, start->child, xml_mode);
        }

        if (!xml_mode || start->type == RES_TYPE_FONT || start->type == RES_TYPE_PIXELMAP)
        {
            writer.CloseTag("resource");
        }
        start = start->next;
    }
}

///////////////////////////////////////////////////////////////////////////////
void studiox_project::WriteThemePaletteInfo(xml_writer& writer, theme_info *theme, BOOL xml_mode)
{
int palette_index;
GX_COLOR* pal;

    writer.OpenTag("palette");
    writer.WriteInt("total_size", theme->palette_total_size);
    if (!xml_mode)
    {
        writer.WriteInt("predefined", theme->palette_predefined);
    }
    pal = theme->palette;

    for (palette_index = 0; palette_index < theme->palette_predefined; palette_index++)
    {
        writer.WriteUnsigned("rgb", *pal);
        pal++;
    }
    writer.CloseTag("palette");
}

///////////////////////////////////////////////////////////////////////////////
void studiox_project::WriteDisplayInfo(xml_writer &writer, int display_index)
{

    display_info *pInfo = &mDisplays[display_index];
    writer.OpenTag("display_info");
    writer.WriteInt("display_index", display_index);
    writer.WriteString("display_name", pInfo->name);
    writer.WriteInt("xres", pInfo->xres);
    writer.WriteInt("yres", pInfo->yres);
    writer.WriteInt("bits_per_pix", pInfo->bits_per_pix);
    writer.WriteBool("packed_format", pInfo->packed_format);
    writer.WriteBool("format_555", pInfo->format_555);
    writer.WriteBool("format_4444", pInfo->format_4444);
    writer.WriteBool("format_332", pInfo->format_332);
    writer.WriteBool("grayscale", pInfo->grayscale);
    writer.WriteBool("reverse_order", pInfo->reverse_order);
    writer.WriteBool("allocate_canvas", pInfo->allocate_canvas);
    writer.WriteBool("enabled", pInfo->enabled);
    writer.WriteString("rotation_angle", ProjectConfigDlg::FindScreenRotationName(pInfo->rotation_angle));
    writer.WriteBool("default_map_format", pInfo->default_map_format);

    writer.OpenTag("theme_info");
    writer.WriteInt("num_themes", pInfo->num_themes);
    writer.WriteInt("active_theme", pInfo->active_theme);
    
    for (int theme = 0; theme < pInfo->num_themes; theme++)
    {
        writer.WriteString("theme_name", pInfo->themes[theme].theme_name);
        writer.WriteBool("gen_color_table", pInfo->themes[theme].gen_color_table);
        writer.WriteBool("gen_font_table", pInfo->themes[theme].gen_font_table);
        writer.WriteBool("gen_pixelmap_table", pInfo->themes[theme].gen_pixelmap_table);
        writer.WriteBool("enabled", pInfo->themes[theme].enabled);
        writer.WriteBool("statically_defined", pInfo->themes[theme].statically_defined);
        writer.OpenTag("theme_data");

        WriteResources(writer, pInfo->themes[theme].GetFirstResourceInfo());
        WriteThemeScrollbars(writer, display_index, theme);

        if (pInfo->themes[theme].palette)
        {
            WriteThemePaletteInfo(writer, &pInfo->themes[theme]);
        }

        writer.CloseTag("theme_data");
    }
    writer.CloseTag("theme_info");

    for (int index = 0; index < mHeader.num_languages; index++)
    {
        writer.WriteBool(CT2A(mHeader.languages[index].name), mDisplays[display_index].gen_string_table[index]);
    }

    if (display_index < mHeader.num_displays)
    {
        WriteStringTable(writer, pInfo->stable);
        WriteScreenFlow(writer, pInfo->screenflow);
    }
    widget_writer::WriteWidgetFolders(writer, this, display_index, pInfo->GetFirstChildFolder());

    writer.CloseTag("display_info");
}

///////////////////////////////////////////////////////////////////////////////
void studiox_project::WriteStringTable(xml_writer &writer, string_table *table)
{
    int string_id;
    int num_strings;
    int language;
    string_table_record record;
    CString id_name;

    writer.OpenTag("string_table");
    if (table)
    {
        num_strings = table->CountStrings();
        writer.WriteInt("sort_column", table->GetSortColumn());
        writer.WriteInt("num_strings", num_strings);
        writer.WriteInt("num_languages", table->CountLanguages());

        if (table->CountLanguages() != mHeader.num_languages)
        {
            ErrorMsg("Internal Error: Language count discrepency");
        }

        for (string_id = 1; string_id < num_strings; string_id++)
        {
            id_name = table->GetResourceIdName(string_id);
            record = table->GetRecord(id_name);
            writer.OpenTag("string_record");
            writer.WriteString("id", record.id_name);
            writer.WriteInt("font", record.font_id);
            writer.WriteString("notes", record.notes);
            
            for (language = 0; language < table->CountLanguages(); language++)
            {
                writer.WriteString("val", record.strings[language], TRUE);
            }
            writer.CloseTag("string_record");
        }
    }
        
    writer.CloseTag("string_table");
}

///////////////////////////////////////////////////////////////////////////////
void studiox_project::WriteScreenFlow(xml_writer &writer, screen_flow *screen_flow)
{

    if (screen_flow)
    {

        int index;

        writer.OpenTag("screen_flow");

        flow_item *item;

        writer.WriteInt("scale", screen_flow->GetScale());

        for (index = 0; index < screen_flow->GetFlowListCount(); index++)
        {
            item = screen_flow->GetFlowItem(index);
            WriteFlowItem(writer, item);
        }
        writer.CloseTag("screen_flow");
    }
}

///////////////////////////////////////////////////////////////////////////////
void studiox_project::WriteFlowItem(xml_writer &writer, flow_item *item)
{
    if (item)
    {
        GX_RECTANGLE rect;
        rect.gx_rectangle_left = (GX_VALUE)item->rect.left;
        rect.gx_rectangle_top = (GX_VALUE)item->rect.top;
        rect.gx_rectangle_right = (GX_VALUE)item->rect.right;
        rect.gx_rectangle_bottom = (GX_VALUE)item->rect.bottom;

        writer.OpenTag("flow_item");
        writer.WriteString("screen_name", item->screen_name);
        writer.WriteRect("rect", rect);
        writer.WriteBool("enabled", item->enabled);

        trigger_info *trigger = item->trigger_list;

        WriteTriggerInfo(writer, trigger);

        writer.CloseTag("flow_item");
    }
}

///////////////////////////////////////////////////////////////////////////////
void studiox_project::WriteTriggerInfo(xml_writer &writer, trigger_info *trigger)
{
    CArray<action_info *> *action_list;
    action_info *action;
    int id;

    while (trigger)
    {
        CString type_name;

        writer.OpenTag("trigger_info");
        writer.WriteString("trigger_name", trigger->trigger_name);
        writer.WriteString("signal_id_name", trigger->signal_id_name);
        type_name = trigger_edit_dlg::GetTriggerTypeName(trigger->trigger_type);
        writer.WriteString("trigger_type", type_name);
        type_name = trigger_edit_dlg::GetEventTypeName(trigger->event_type);
        writer.WriteString("event_type", type_name);
        writer.WriteString("system_event_animat_id_name", trigger->system_event_animat_id_name);
        writer.WriteString("user_event_id_name", trigger->user_event_id_name);


        writer.OpenTag("action_list");

        action_list = &trigger->action_list;

        for (int count = 0; count < action_list->GetCount(); count++)
        {
            action = action_list->GetAt(count);
            writer.OpenTag("action_info");
            writer.WriteString("action_name", action->action_name);
            type_name = trigger_action_select_dlg::GetActionTypeName(action->action_type);
            writer.WriteString("action_type", type_name);
            writer.WriteString("target_widget_name", action->target_widget_name);
            writer.WriteString("parent_widget_name", action->parent_widget_name);
            writer.WriteString("animation_id_name", action->animation_id_name);
            writer.WriteBool("target_show_child_widgets", action->target_show_child_widgets);
            writer.WriteBool("parent_show_child_widgets", action->parent_show_child_widgets);

            if (action->animation)
            {
                writer.OpenTag("animation_info");
                writer.WriteInt("start_x", action->animation->gx_animation_start_position.gx_point_x);
                writer.WriteInt("start_y", action->animation->gx_animation_start_position.gx_point_y);
                writer.WriteInt("end_x", action->animation->gx_animation_end_position.gx_point_x);
                writer.WriteInt("end_y", action->animation->gx_animation_end_position.gx_point_y);
                writer.WriteUByte("steps", action->animation->gx_animation_steps);
                writer.WriteUnsigned("frame_interval", action->animation->gx_animation_frame_interval);
                writer.WriteUnsigned("start_delay", action->animation->gx_animation_start_delay);
                writer.WriteUByte("start_alpha", action->animation->gx_animation_start_alpha);
                writer.WriteUByte("end_alpha", action->animation->gx_animation_end_alpha);
                writer.WriteBool("detach_target", action->animation->gx_animation_style & GX_ANIMATION_DETACH);
                writer.WriteBool("push_target", action->animation->gx_animation_style & GX_ANIMATION_PUSH_STACK);

                id = action->animation->gx_animation_style & GX_ANIMATION_EASING_FUNC_MASK;
                writer.WriteString("easing_func_id_name", easing_function_select_dlg::GetEasingFuncIdName(id));
                writer.CloseTag("animation_info");
            }
            writer.CloseTag("action_info");
        }

        writer.CloseTag("action_list");

        writer.CloseTag("trigger_info");
        trigger = trigger->next;
    }
}

///////////////////////////////////////////////////////////////////////////////
void studiox_project::WriteProjectHeader(xml_writer &writer)
{
    int index;

    writer.OpenTag("header");
    writer.WriteInt("project_version", mHeader.project_version);
    writer.WriteInt("guix_version", mHeader.guix_version);
    writer.WriteInt("studio_version", mHeader.studio_version);

    writer.WriteString("project_name", mHeader.project_name);
    writer.WriteString("source_path", mHeader.source_path);
    writer.WriteString("header_path", mHeader.header_path);
    writer.WriteString("resource_path", mHeader.resource_path);
    writer.WriteString("allocator_function", mHeader.malloc_name);
    writer.WriteString("free_function", mHeader.free_name);
    writer.WriteString("additional_headers", mHeader.additional_headers);
    writer.WriteBool("insert_headers_before", mHeader.insert_headers_before);

    writer.WriteInt("target_cpu", mHeader.target_cpu);
    writer.WriteInt("target_tools", mHeader.target_tools);
    writer.WriteBool("big_endian", mHeader.big_endian);
    writer.WriteBool("dave2d_graph_accelerator", mHeader.dave2d_graph_accelerator);
    writer.WriteInt("renesas_jpeg_decoder", mHeader.renesas_jpeg_decoder);
    writer.WriteInt("renesas_png_decoder", mHeader.renesas_png_decoder);

    writer.WriteBool("grid_enabled", mHeader.grid_enabled);
    writer.WriteBool("snap_enabled", mHeader.snap_enabled);
    writer.WriteBool("snap_to_widget_enabled", mHeader.snap_to_widget_enabled);
    writer.WriteInt("grid_spacing", mHeader.grid_spacing);
    writer.WriteInt("snap_spacing", mHeader.snap_spacing);

    writer.WriteBool("gen_binary", mHeader.gen_binary);
    writer.WriteUnsigned("binary_file_format", mHeader.binary_file_format);
    writer.WriteUnsigned("memory_offset", mHeader.memory_offset);
    writer.WriteBool("gen_res_header", mHeader.gen_res_header);

    writer.WriteBool("custom_resource_enabled", mHeader.custom_resource_enabled);
    writer.WriteString("custom_resource_file_name", mHeader.custom_resource_file_name);
    writer.WriteInt("app_execute_xpos", mHeader.app_execute_xpos);
    writer.WriteInt("app_execute_ypos", mHeader.app_execute_ypos);
    writer.WriteBool("is_widget_position_locked", mHeader.is_widget_position_locked);
    writer.WriteInt("palette_mode_aa_text_colors", mHeader.palette_mode_aa_text_colors);

    writer.WriteInt("num_displays", mHeader.num_displays);
    writer.WriteInt("max_displays", mHeader.max_displays);
    writer.WriteInt("num_languages", mHeader.num_languages);
    writer.OpenTag("language_names");

    for (index = 0; index < mHeader.num_languages; index++)
    {
        writer.WriteString("language", mHeader.languages[index].name);
        writer.WriteBool("support_bidi_text", mHeader.languages[index].support_bidi_text);
        writer.WriteBool("gen_reordered_bidi_text", mHeader.languages[index].gen_reordered_bidi_text);
        writer.WriteBool("support_thai_glyph_shaping", mHeader.languages[index].support_thai_glyph_shaping);
        writer.WriteBool("gen_adjusted_thai_string", mHeader.languages[index].gen_adjusted_thai_string);
        writer.WriteBool("statically_defined", mHeader.languages[index].statically_defined);
    }
    writer.CloseTag("language_names");

    writer.OpenTag("string_export");
    writer.WriteInt("string_export_src", mHeader.string_export_src);
    writer.WriteInt("string_export_target", mHeader.string_export_target);
    writer.WriteInt("string_export_version", mHeader.string_export_version);
    writer.WriteString("string_export_path", mHeader.string_export_path);
    writer.WriteString("string_export_name", mHeader.string_export_filename);

    CString file_typename = string_export_dlg::GetStringExportTypeName(mHeader.string_export_filetype);
    writer.WriteString("string_export_filetype", file_typename);
    writer.CloseTag("string_export");

    writer.CloseTag("header");
}

///////////////////////////////////////////////////////////////////////////////
BOOL studiox_project::Save()
{
    if (GetPropsWin())
    {
        //send a kill focus message to the current focus owner.
        GetPropsWin()->SendEditFocusLoseMessage();
    }

    CString pathname = mHeader.project_path;
    pathname += "\\";
    pathname += mHeader.project_name;

    pathname += ".gxp";
    xml_writer writer;
    if (!writer.OpenFile(pathname))
    {
        
        ErrorMsg("Unable to open project file.");
        return FALSE;
    }

    writer.WriteHeader("GUIX_Studio_Project");
    writer.OpenTag("project");
    WriteProjectHeader(writer);

    for (int index = 0; index < mHeader.max_displays; index++)
    {
        WriteDisplayInfo(writer, index);
    }
    writer.CloseTag("project");
    writer.CloseFile();
    is_modified = FALSE;
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
BOOL studiox_project::SaveAs()
{
    CString old_pathname = mHeader.project_path;
    CString old_project_name = mHeader.project_name;

    TCHAR SavePathname[MAX_PATH];
    TCHAR FileName[MAX_PATH];

    SavePathname[0] = 0;
    FileName[0] = 0;
    _tcscpy(SavePathname, mHeader.project_name.GetBuffer());

    if (GetOutputFileName(SavePathname, FileName, 
                          _T("Save Project As"),
                          _T("GUIX Studio Projects\0*.gxp\0\0"),
                          mHeader.project_path.GetBuffer(),
                          _T("gxp")))
    {

        UndoManager()->Reset();
        CString new_path(SavePathname);
        CString new_name(FileName);
        new_path = new_path.Left(new_path.GetLength() - new_name.GetLength());
        new_path.TrimRight('\\');
        
        // string the extension
        new_name = new_name.Left(new_name.GetLength() - 4);
        mHeader.project_path = new_path;
        mHeader.project_name = new_name;
        BOOL val = Save();
        //SetProjectDirectory(new_path);

        // KGM- restore original path and name, in case they saved it somewhere else we don't
        // want everything to be broken
        mHeader.project_path = old_pathname;
        mHeader.project_name = old_project_name;
        return val;
    }
    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
int studiox_project::GetResourceType(res_info* start)
{
    while (start)
    {
        switch (start->type)
        {
        case RES_TYPE_GROUP:
        case RES_TYPE_FOLDER:
            return GetResourceType(start->child);
        case RES_TYPE_PIXELMAP:
        case RES_TYPE_FONT:
            return start->type;
        default:
            break;
        }

        start = start->next;
    }

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
void studiox_project::GenerateResourceXml(CString &pathname, CString &filename, res_info *start)
{
    if (!start)
    {
        return;
    }

    xml_writer writer;
    if (!writer.OpenFile(pathname))
    {

        ErrorMsg("Unable to open xml file.");
    }

    writer.WriteHeader("GUIX_Studio_Resource");

    writer.OpenTag("resource_project");

    // Write header.
    writer.OpenTag("header");
    writer.WriteString("name", filename);
    writer.WriteInt("version", mHeader.project_version);
    writer.WriteString("converter", "GUIX Studio");
    writer.WriteInt("studio_version", mHeader.studio_version);
    writer.WriteInt("guix_version", mHeader.guix_version);
    writer.WriteString("target_cpu", ProjectConfigDlg::FindTargetCPUName(mHeader.target_cpu));
    writer.WriteString("target_tools", ProjectConfigDlg::FindTargetCompilerName(mHeader.target_tools));
    writer.WriteBool("dave2d_graph_accelerator", mHeader.dave2d_graph_accelerator);
    writer.CloseTag("header");

    // Write display information
    int display = GetProjectView()->GetActiveDisplay();
    display_info *dinfo = &mDisplays[display];
    theme_info *theme = &dinfo->themes[dinfo->active_theme];
    writer.OpenTag("display_info");
    writer.WriteString("display_color_format", resource_gen::GetColorFormatName(dinfo->colorformat));
    writer.WriteString("rotation_angle", ProjectConfigDlg::FindScreenRotationName(dinfo->rotation_angle));
    if (theme->palette && (GetResourceType(start) == RES_TYPE_PIXELMAP) && (dinfo->colorformat < GX_COLOR_FORMAT_5551BGRX))
    {
        CreateThemePalette(display, dinfo->active_theme, NULL);
        theme->palette_predefined = theme->palette_total_size;
        WriteThemePaletteInfo(writer, theme, TRUE);
    }
    writer.CloseTag("display_info");

    // Write resource information
    if (start->type == RES_TYPE_FONT || start->type == RES_TYPE_PIXELMAP)
    {
        writer.OpenTag("resource");
        WriteOneResource(writer, start, TRUE);
        writer.CloseTag("resource");
    }
    else if (start->child)
    {
        WriteResources(writer, start->child, TRUE);
    }

    writer.CloseTag("resource_project");
    writer.CloseFile();
}

///////////////////////////////////////////////////////////////////////////////
BOOL studiox_project::ReadXMLFile(CString &pathname)
{
    xml_reader reader;

    if (!reader.ReadFile(pathname))
    {
        CString error("Unable to read xml file: ");
        error += pathname;
        ErrorMsg(error);
        return FALSE;
    }

    CString filename;
    INT version;
    CString converter;
    CString string;

    // Read header.
    if (reader.EnterSection("resource_project"))
    {
        if (reader.EnterSection("header"))
        {
            reader.ReadString("name", filename);
            reader.ReadInt("version", version);

            if (version < PROJECT_VERSION_INITIAL_RESOURCE_XML)
            {
                ErrorMsg("Invalid Project Version!");
                return FALSE;
            }

            reader.ReadString("converter", converter);

            if (converter != L"GUIX Studio")
            {
                ErrorMsg("Unkown converter!");
                return FALSE;
            }

            reader.ReadInt("studio_version", mHeader.studio_version);

            if (mHeader.studio_version > CalculateStudioVersion())
            {
                ErrorMsg("The version of GUIX Studio being utilized is below the required version!");
                return FALSE;
            }

            reader.ReadInt("guix_version", mHeader.guix_version);
            reader.ReadString("target_cpu", string);
            mHeader.target_cpu = ProjectConfigDlg::FindTargetCPUVal(string);
            reader.ReadString("target_tools", string);
            mHeader.target_tools = ProjectConfigDlg::FindTargetCompilerVal(string);
            reader.ReadBool("dave2d_graph_accelerator", mHeader.dave2d_graph_accelerator);
            reader.CloseSection(TRUE, TRUE);

            mHeader.num_displays = 1;
            mDisplays[0].num_themes = 1;

            if (reader.EnterSection("display_info"))
            {
                reader.ReadString("display_color_format", string);
                mDisplays[0].colorformat = resource_gen::GetColorFormatVal(string);
                reader.ReadString("rotation_angle", string);
                mDisplays[0].rotation_angle = ProjectConfigDlg::FindScreenRotationVal(string);
                ReadThemePaletteInfo(reader, &mDisplays[0].themes[0], TRUE);
                reader.CloseSection(TRUE, TRUE);
            }

            ReadResources(reader, 0, 0, NULL);
            reader.CloseSection();

            TaskInitializeAllPixelmaps();

            return TRUE;
        }
    }
    else
    {
        ErrorMsg("The selected file is not a valid GUIX Studio resource file.");
    }

    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
void studiox_project::ReadWidgetFolders(xml_reader &reader, int index)
{
    display_info *pInfo = &mDisplays[index];
    pInfo->SetFirstChildFolder(NULL);
    //if (mHeader.studio_version < 5040001)
    if (!reader.EnterSection("widget_folder"))
    {
        /* Create default folder for old version */
        folder_info * folder = new folder_info(CString("default_folder"));
        if (folder)
        {
            pInfo->SetFirstChildFolder(folder);
        }
        widget_reader::ReadWidgets(reader, this, index, pInfo->GetFirstChildFolder());
    }
    else
    {
        reader.CloseSection(FALSE, FALSE);
        widget_reader::ReadWidgetFolders(reader, this, index);
    }
}

///////////////////////////////////////////////////////////////////////////////
void studiox_project::ReadPaletteType(int display_color_format, xml_reader &reader, res_info *res)
{
    char palette_type[40];
    res->palette_type = PALETTE_TYPE_NONE;

    memset(palette_type, 0, 40);

    if (display_color_format == GX_COLOR_FORMAT_8BIT_PALETTE)
    {
        res->palette_type = PALETTE_TYPE_SHARED;
        return;
    }
    else
    {
        if (res->output_color_format != GX_COLOR_FORMAT_8BIT_PALETTE)
        {
            res->palette_type = PALETTE_TYPE_NONE;
            return;
        }
    }

    if (reader.ReadString("palette_type", palette_type, 40))
    {
        if (strlen(palette_type) > 1)
        {
            if (strcmp(palette_type, "Private") == 0)
            {
                res->palette_type = PALETTE_TYPE_PRIVATE;
            }
            else
            {
                if (strcmp(palette_type, "Shared") == 0)
                {
                    res->palette_type = PALETTE_TYPE_SHARED;
                }
            }
        }
        else
        {
            // here for older projects. The palette type was saved as a number.
            //    PALETTE_TYPE_PRIVATE = 0
            //    PALETTE_TYPE_SHARED = 1
            //    PALETTE_TYPE_GLOBAL = 2
        
            if (palette_type[0] == '0')
            {
                res->palette_type = PALETTE_TYPE_PRIVATE;
            }
            else
            {
                if (palette_type[0] == '1' ||
                    palette_type[0] == '2')
                {
                    res->palette_type = PALETTE_TYPE_SHARED;
                }
            }
        }
    }                            
}


///////////////////////////////////////////////////////////////////////////////
void studiox_project::ReadOneResource(xml_reader &reader, int display_index, res_info *put)
{
    ULONG gxColor;
    int page_count;
    int res_id;
    CString message;
    CString name;
    CString num;
    CString base_name;
    CCommandInfo *pCmdInfo = GetCmdInfo();

    reader.ReadString("name", put->name);
    reader.ReadPathInfo(put->pathinfo);

    if (put->type == RES_TYPE_PIXELMAP ||
        put->type == RES_TYPE_FONT ||
        put->type == RES_TYPE_COLOR)
    {
        if (!TestInputName(put->name, NULL, NULL, FALSE))
        {
            message = _T("The project contains an invalid resource name: ");

            // trim the bad name (in case it is really really long)
            put->name = put->name.Left(40);
            message += put->name;

            base_name = _T("RESOURCE_");
            res_id = 1;

            while(1)
            {
                num.Format(_T("%d"), res_id);
                name = base_name + num;
                
                if (!NameExists(display_index, put->type, name))
                {
                    put->name = name;
                    break;
                }
                res_id++;
            }

            message += (_T(". Resetting the resource Id to: "));
            message += name;
            ErrorMsg(message);
        }
    }
        

    if (mHeader.project_version <= 52)
    {
        reader.ReadInt("resource_id", res_id);

        if (put->type == RES_TYPE_FOLDER ||
            put->type == RES_TYPE_HEADER ||
            put->type == RES_TYPE_GROUP)
        {
            put->folder_id = res_id;
        }
    }
    else
    {
        res_id = -1;

        reader.ReadString("folder_id", name);
        put->folder_id = FindFolderIdVal(put->type, name);
    }

    reader.ReadBool("is_default", put->is_default);

    if (!reader.ReadBool("enabled", put->enabled))
    {
        put->enabled = TRUE;
    }

    switch(put->type)
    {
    case RES_TYPE_COLOR:
        reader.ReadUnsigned("colorval", gxColor);
        put->colorval = gxColor;
        put->compress = FALSE;
        AddToResourceDictionary(display_index, put, res_id);
        break;

    case RES_TYPE_FONT:

        reader.ReadInt("height", put->font_height);
        RANGE_CHECK(put->font_height, 1, 255);

        reader.ReadInt("font_bits", put->font_bits, 8);

        if (!IsFontBitsSupported(put->font_bits))
        {
            put->font_bits = 8;
        }

        reader.ReadBool("font_include_st_glyphs", put->font_charset_include_string_table);
        reader.ReadBool("font_support_extended_unicode", put->font_support_extended_unicode);

        // older projects did not support font compression:
        if ((mHeader.project_version >= 51)&&
            (IsRenesasDave2D(this)) &&
            (mDisplays[display_index].colorformat > GX_COLOR_FORMAT_8BIT_PALETTE))
        {
            reader.ReadBool("compress", put->compress);
        }
        else
        {
            put->compress = FALSE;
        }

        if ((mHeader.project_version >= 53) &&
            (!IsRenesasDave2D(this)))
        {
            reader.ReadBool("font_kerning", put->font_kerning);
        }
        else
        {
            put->font_kerning = FALSE;
        }

        reader.ReadBool("output_file_enabled", put->output_file_enabled);
        reader.ReadString("output_file", put->output_file);
        reader.ReadBool("binary_mode", put->binary_mode);

        if (pCmdInfo->IsXmlMode())
        {
            CArray<font_page_info> pages;
            font_page_info page_info;
            while (reader.EnterSection("font_page_data"))
            {
                page_info.enabled = TRUE;
                reader.ReadInt("first_char", page_info.first_char);
                RANGE_CHECK(page_info.first_char, 0, GX_MAX_GLYPH_CODE);
                reader.ReadInt("last_char", page_info.last_char);
                RANGE_CHECK(page_info.last_char, 0, GX_MAX_GLYPH_CODE);
                pages.Add(page_info);
                reader.CloseSection(TRUE, TRUE);
            }

            put->font_pages_count = pages.GetCount();

            if (pages.GetCount())
            {
                put->font_pages = new font_page_info[pages.GetCount()];
                for (int index = 0; index < pages.GetCount(); index++)
                {
                    put->font_pages[index] = pages.GetAt(index);
                }
            }
        }
        else
        {

            page_count = NUM_FONT_CHAR_RANGES;

            if (put->font_support_extended_unicode)
            {
                put->font_pages = font_path_dialog::CreateDefaultFontPages(TRUE);
                page_count += NUM_FONT_EXTENDED_CHAR_RANGES;
            }
            else
            {
                put->font_pages = font_path_dialog::CreateDefaultFontPages();
            }

            if (reader.EnterSection("font_page_data"))
            {
                for (int loop = 0; loop < page_count; loop++)
                {
                    reader.ReadBool("enabled", put->font_pages[loop].enabled);

                    reader.ReadInt("first_char", put->font_pages[loop].first_char);
                    RANGE_CHECK(put->font_pages[loop].first_char, 0, GX_MAX_GLYPH_CODE);
                    reader.ReadInt("last_char", put->font_pages[loop].last_char);
                    RANGE_CHECK(put->font_pages[loop].last_char, 0, GX_MAX_GLYPH_CODE);
                }
                reader.CloseSection(TRUE, TRUE);
            }
            else
            {
                // old projects do not have font_page_data section,
                // so read the first and last char data into first page data

                reader.ReadInt("firstchar", put->font_pages[0].first_char);
                RANGE_CHECK(put->font_pages[0].first_char, 0, GX_MAX_GLYPH_CODE);
                reader.ReadInt("lastchar", put->font_pages[0].last_char);
                RANGE_CHECK(put->font_pages[0].last_char, 0, GX_MAX_GLYPH_CODE);
            }
        }

        put->font = NULL;
        AddToResourceDictionary(display_index, put, res_id);

        if (put -> is_default && put->pathinfo.pathname.IsEmpty())
        {
            ConfigureDefaultFont(put, display_index);
        }
        break;

    case RES_TYPE_PIXELMAP:
        if (mHeader.studio_version < STUDIO_VERSION_USE_INTERNAL_SYSTEM_PNG_DATA)
        {
            if (put->is_default)
            {
                if (put->pathinfo.pathname.Find(L"graphics\\system_png\\radiobutton_on.png") >= 0 ||
                    put->pathinfo.pathname.Find(L"graphics\\system_png\\radiobutton_off.png") >= 0 ||
                    put->pathinfo.pathname.Find(L"graphics\\system_png\\checkbox_on.png") >= 0 ||
                    put->pathinfo.pathname.Find(L"graphics\\system_png\\checkbox_off.png") >= 0)
                {
                    // Clear path information to use internally linked system pngs.
                    put->pathinfo.pathtype = PATH_TYPE_PROJECT_RELATIVE;
                    put->pathinfo.pathname = "";
                }
            }
        }
        reader.ReadBool("alpha", put->keep_alpha);
        reader.ReadBool("dither", put->dither);
        reader.ReadBool("raw", put->raw);
        if (mHeader.project_version >= PROJECT_VERSION_WRITE_COLOR_FORMAT_NAME)
        {
            reader.ReadString("color_format", name);
            put->output_color_format = resource_gen::GetColorFormatVal(name);
        }
        else
        {
            reader.ReadInt("color_format", put->output_color_format, 0);
        }

        reader.ReadBool("output_file_enabled", put->output_file_enabled);
        reader.ReadString("output_file", put->output_file);
        reader.ReadBool("binary_mode", put->binary_mode);
        ReadPaletteType(mDisplays[display_index].colorformat, reader, put);
        reader.ReadBool("compress", put->compress);
        AddToResourceDictionary(display_index, put, res_id);

        // sanity check

        if (put->output_color_format < 0 || put->output_color_format > 50)
        {
            put->output_color_format = 0;
        }
        break;

    default:
        break;
    }

}

///////////////////////////////////////////////////////////////////////////////
void studiox_project::ReadResources(xml_reader &reader, int DisplayIndex,
    int theme_index, res_info *parent)
{
    display_info *pInfo = &mDisplays[DisplayIndex];

    int restype;
    CString res_type_name;
    res_info *newres;
    res_info *previous = NULL;

    while(reader.EnterSection("resource"))
    {
        newres = NULL;
        reader.ReadString("type", res_type_name);
        restype = ResStringToType(res_type_name);

        if (restype)
        {
            newres = new res_info(restype);
            ReadOneResource(reader, DisplayIndex, newres);

            if (newres->type == RES_TYPE_COLOR)
            {
                if (FindResource(parent, RES_TYPE_COLOR, newres->name))
                {
                    //This is a duplicate color resource, continue processing.
                    delete newres;
                    reader.CloseSection(TRUE, TRUE);
                    continue;
                }
            }
            else if (newres->type == RES_TYPE_FOLDER)
            {
                res_info* pAdd = NULL;

                switch (newres->folder_id)
                {
                case CUSTOM_COLOR_FOLDER:
                    pAdd = new res_info(RES_TYPE_ADD_COLOR);
                    newres->Attach(pAdd);
                    break;

                case CUSTOM_FONT_FOLDER:
                    pAdd = new res_info(RES_TYPE_ADD_FONT);
                    newres->Attach(pAdd);
                    break;

                case CUSTOM_PIXELMAP_FOLDER:
                    pAdd = new res_info(RES_TYPE_ADD_PIXELMAP);
                    newres->Attach(pAdd);
                    break;

                default:
                    break;
                }
            }
            else if (newres->type == RES_TYPE_GROUP)
            {
                if (newres->folder_id == STRING_GROUP)
                {
                    res_info *pAdd = new res_info(RES_TYPE_ADD_STRING);
                    newres->Attach(pAdd);
                }
            }

            if (parent)
            {
                parent->Attach(newres);
            }
            else
            {
                if (pInfo->themes[theme_index].GetFirstResourceInfo())
                {
                    previous = pInfo->themes[theme_index].GetFirstResourceInfo();
                    while(previous->next)
                    {
                        previous = previous->next;
                    }
                    previous->next = newres;
                }
                else
                {
                    pInfo->themes[theme_index].SetFirstResourceInfo(newres);
                }
            }
        }

        if (newres)
        {
            ReadResources(reader, DisplayIndex, theme_index, newres);
        }
        reader.CloseSection(TRUE, TRUE);
    }

    if (parent == NULL)
    {
        newres = pInfo->themes[theme_index].GetFirstResourceInfo();
        if (newres)
        {
            if ((newres->type != RES_TYPE_HEADER) ||
                (newres->folder_id != THEME_HEADER))
            {
                //Add theme header resource info
                res_info *pThemeHeader = new res_info(RES_TYPE_HEADER);
                pThemeHeader->folder_id = THEME_HEADER;
                pThemeHeader->name = pInfo->themes[theme_index].theme_name;
                pThemeHeader->next = pInfo->themes[theme_index].GetFirstResourceInfo();
                pInfo->themes[theme_index].SetFirstResourceInfo(pThemeHeader);
            }
        }
        else
        {
            // Inform the user that the project has issues:
            ErrorMsg("Internal Error: The project resource tree is invalid and is being reconstructed.");

            //Create default theme resources
            CreateDefaultResources(DisplayIndex, theme_index);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void studiox_project::ReadStringTable(xml_reader &reader, display_info *info)
{
    int index;
    int font_id;
    int sort_column;
    int num_strings;
    int num_languages;
    int language;
    CString temp_id;
    CString temp_val;

    if (info->stable)
    {
        delete info->stable;
    }
    string_table *table = new string_table;
    info->stable = table;

    if (reader.EnterSection("string_table"))
    {
        reader.ReadInt("sort_column", sort_column, -1);

        table->SetSortColumn(sort_column);

        reader.ReadInt("num_strings", num_strings, 1);
        reader.ReadInt("num_languages", num_languages, 1);

        RANGE_CHECK(num_languages, 1, MAX_LANGUAGES);
        RANGE_CHECK(num_strings, 1, MAX_STRING_TABLE_STRINGS);

        table->Initialize(num_languages, num_strings);

        index = 1;
        while (index < num_strings)
        {
            if (reader.EnterSection("string_record"))
            {
                reader.ReadString("id", temp_id);
                table->AddToDictionary(temp_id, index);
                reader.ReadInt("font", font_id);
                table->SetDisplayFont(index, font_id);
                reader.ReadString("notes", temp_val);
                if (!temp_val.IsEmpty())
                {
                    table->SetNotes(index, temp_val);
                }
            
                reader.ReadString("val", temp_val);
                table->SetString(index, temp_id, 0, temp_val, FALSE);

                for (language = 1; language < num_languages; language++)
                {
                    reader.ReadString("val", temp_val);
                    table->SetString(index, language, temp_val, FALSE);
                }
                reader.CloseSection(TRUE, TRUE);
                index++;
            }
            else
            {
                table->RemoveString(index);
                num_strings--;
            }
        }
        reader.CloseSection(TRUE, TRUE);

        //Sort string table according to specified column
        table->Sort();
    }
    else
    {
        // if the string table is missing, initialize to 
        // one language and one string
        table->Initialize(1, 1);
    }

    table->SetActiveLanguage(0);
}

///////////////////////////////////////////////////////////////////////////////
void studiox_project::ReadScreenFlow(xml_reader &reader, int display_index)
{
    display_info *info = &mDisplays[display_index];

    if (info->screenflow)
    {
        delete info->screenflow;
        info->screenflow = NULL;
    }

    if (reader.EnterSection("screen_flow"))
    {
        info->screenflow = new screen_flow;

        int scale;

        if (!reader.ReadInt("scale", scale))
        {
            scale = 100;
        }

        RANGE_CHECK(scale, SCREEN_FLOW_MIN_SCALE, SCREEN_FLOW_MAX_SCALE);

        info->screenflow->SetScale(scale);

        ReadFlowItem(reader, display_index);
        reader.CloseSection(TRUE, TRUE);
    }
}

///////////////////////////////////////////////////////////////////////////////
void studiox_project::ReadFlowItem(xml_reader &reader, int display_index)
{
    screen_flow *flow= mDisplays[display_index].screenflow;

    flow_item *item;
    GX_RECTANGLE rect;
    while (reader.EnterSection("flow_item"))
    {
        item = new flow_item;
        reader.ReadString("screen_name", item->screen_name);
        reader.ReadRect("rect", rect);
        item->rect.left = rect.gx_rectangle_left;
        item->rect.top = rect.gx_rectangle_top;
        item->rect.right = rect.gx_rectangle_right;
        item->rect.bottom = rect.gx_rectangle_bottom;
        if (!reader.ReadBool("enabled", item->enabled))
        {
            item->enabled = TRUE;
        }
        ReadTriggerInfo(reader, display_index, item);

        flow->AddFlowItem(item);

        reader.CloseSection(TRUE, TRUE);
    }
}

///////////////////////////////////////////////////////////////////////////////
void studiox_project::ReadTriggerInfo(xml_reader &reader, int display_index, flow_item *item)
{
    trigger_info *head = NULL;
    trigger_info *pre = NULL;
    trigger_info *trigger;
    GX_BOOL detach;
    GX_BOOL push_target;

    while (reader.EnterSection("trigger_info"))
    {
        trigger = new trigger_info;
        CString type_name;
        reader.ReadString("trigger_name", trigger->trigger_name);
        reader.ReadString("signal_id_name", trigger->signal_id_name);
        reader.ReadString("trigger_type", type_name);
        trigger->trigger_type = trigger_edit_dlg::GetTriggerType(type_name);
        reader.ReadString("event_type", type_name);
        trigger->event_type = trigger_edit_dlg::GetEventType(type_name);
        reader.ReadString("system_event_animat_id_name", trigger->system_event_animat_id_name);
        reader.ReadString("user_event_id_name", trigger->user_event_id_name);
        trigger->next = NULL;

        if (reader.EnterSection("action_list"))
        {
            while (reader.EnterSection("action_info"))
            {
                action_info *action = new action_info;

                reader.ReadString("action_name", action->action_name);
                reader.ReadString("action_type", type_name);
                action->action_type = trigger_action_select_dlg::GetActionType(type_name);

                if (action->action_type)
                {
                    reader.ReadString("target_widget_name", action->target_widget_name);
                    reader.ReadString("parent_widget_name", action->parent_widget_name);
                    reader.ReadString("animation_id_name", action->animation_id_name);

                    if (!action->animation_id_name.IsEmpty())
                    {
                        //add to animation id dictionary
                        AddToIdDictionary(display_index, ID_TYPE_ANIMATION, action->animation_id_name);
                    }

                    if (!reader.ReadBool("target_show_child_widgets", action->target_show_child_widgets))
                    {
                        action->target_show_child_widgets = FALSE;
                    }

                    if (!reader.ReadBool("parent_show_child_widgets", action->parent_show_child_widgets))
                    {
                        action->parent_show_child_widgets = FALSE;
                    }

                    if (reader.EnterSection("animation_info"))
                    {
                        action->animation = new GX_ANIMATION_INFO;
                        memset(action->animation, 0, sizeof(GX_ANIMATION_INFO));

                        reader.ReadValue("start_x", action->animation->gx_animation_start_position.gx_point_x);
                        reader.ReadValue("start_y", action->animation->gx_animation_start_position.gx_point_y);
                        reader.ReadValue("end_x", action->animation->gx_animation_end_position.gx_point_x);
                        reader.ReadValue("end_y", action->animation->gx_animation_end_position.gx_point_y);
                        reader.ReadUByte("steps", action->animation->gx_animation_steps);

                        if (!reader.ReadUShort("frame_interval", action->animation->gx_animation_frame_interval))
                        {
                            reader.ReadUShort("delay_time", action->animation->gx_animation_frame_interval);
                        }

                        if (!reader.ReadUShort("start_delay", action->animation->gx_animation_start_delay))
                        {
                            reader.ReadUShort("delay_before", action->animation->gx_animation_start_delay);
                        }
                        reader.ReadUByte("start_alpha", action->animation->gx_animation_start_alpha);
                        reader.ReadUByte("end_alpha", action->animation->gx_animation_end_alpha);
                        reader.ReadBool("detach_target", detach);
                        reader.ReadBool("push_target", push_target);

                        /* we only support translate style animation in Studio project today */
                        action->animation->gx_animation_style = GX_ANIMATION_TRANSLATE;
                        if (detach)
                        {
                            action->animation->gx_animation_style |= GX_ANIMATION_DETACH;
                        }

                        if (push_target)
                        {
                            action->animation->gx_animation_style |= GX_ANIMATION_PUSH_STACK;
                        }

                        CString id_name;
                        reader.ReadString("easing_func_id_name", id_name);
                        action->animation->gx_animation_style |= easing_function_select_dlg::GetEasingFuncId(id_name);

                        reader.CloseSection(TRUE, TRUE);
                    }
                    trigger->action_list.Add(action);
                }
                else
                {
                    delete action;
                }

                reader.CloseSection(TRUE, TRUE);
            }
            

            reader.CloseSection(TRUE, TRUE);
        }

        if (!head)
        {
            head = trigger;
        }

        if (pre)
        {
            pre->next = trigger;
        }
        pre = trigger;
        reader.CloseSection(TRUE, TRUE);
    }

    item->trigger_list = head;
}

///////////////////////////////////////////////////////////////////////////////
void studiox_project::ReadThemeScrollbars(xml_reader &reader, int display, int theme)
{
    display_info *info = &mDisplays[display];

    if (reader.EnterSection("vscroll_appearance"))
    {
        vscroll_service_provider::ReadScrollbarAppearance(reader, this, display, info->themes[theme].VScrollAppearance);
        reader.ReadUnsigned("scroll_style", info->themes[theme].VScrollStyle);
        reader.CloseSection();
    }
    if (reader.EnterSection("hscroll_appearance"))
    {
        vscroll_service_provider::ReadScrollbarAppearance(reader, this, display, info->themes[theme].HScrollAppearance);
        reader.ReadUnsigned("scroll_style", info->themes[theme].HScrollStyle);
        reader.CloseSection();
    }
}

///////////////////////////////////////////////////////////////////////////////
void studiox_project::ReadThemePaletteInfo(xml_reader& reader, theme_info *theme, BOOL xml_mode)
{
    int palette_index;
    int test_size;
    GX_COLOR* pal;

    if (reader.EnterSection("palette"))
    {
        reader.ReadInt("total_size", test_size);

        // sanity check
        if (test_size > 256)
        {
            test_size = 256;
        }
        theme->palette_total_size = test_size;

        if (xml_mode)
        {
            test_size = theme->palette_total_size;
        }
        else
        {
            reader.ReadInt("predefined", test_size);
        }

        // sanity check
        if (test_size > theme->palette_total_size)
        {
            test_size = theme->palette_total_size;
        }
        if (test_size < 0)
        {
            test_size = 0;
        }
        theme->palette_predefined = test_size;

        if (theme->palette_total_size > 0)
        {
            theme->palette = new GX_COLOR[256];
            pal = theme->palette;
            memset(pal, 0, 256 * sizeof(GX_COLOR));

            for (palette_index = 0; palette_index < theme->palette_predefined; palette_index++)
            {
                reader.ReadUnsigned("rgb", *pal);

                if (mHeader.project_version < 52)
                {
                    //default alpha value as 0xff
                    (*pal) |= 0xff000000;
                }

                pal++;
            }
        }
        reader.CloseSection(TRUE, TRUE);
    }
    else
    {
        theme->palette = NULL;
        theme->palette_total_size = 0;
    }
}

///////////////////////////////////////////////////////////////////////////////
BOOL studiox_project::ReadDisplayInfo(xml_reader &reader, int index)
{
    int open_index;
    int theme;
    int enabled;
    
    // KGM FIXME:
    //
    // this flag is just so that we can temporarily read old project files. 
    // rid of this flag as soon as we have updated all the old projects.
    
    BOOL old_theme_data_format = FALSE;

    color_dictionary[index].RemoveAll();
    font_dictionary[index].RemoveAll();
    pixelmap_dictionary[index].RemoveAll();
    pixelmap_dictionary[index].Add(CString(""));

    widget_id_dictionary[index].RemoveAll();
    animation_id_dictionary[index].RemoveAll();

    display_info *pInfo = &mDisplays[index];
    if (reader.EnterSection("display_info"))
    {
        reader.ReadInt("display_index", open_index);
        if (open_index == index)
        {
            reader.ReadString("display_name", pInfo->name);
            reader.ReadInt("xres", pInfo->xres);
            reader.ReadInt("yres", pInfo->yres);

            // if display resolution is not valid, default to
            // 320x240

            if (pInfo->xres > GX_MAX_DISPLAY_RESOLUTION || pInfo->xres <= 0)
            {
                pInfo->xres = 320;
            }
            if (pInfo->yres > GX_MAX_DISPLAY_RESOLUTION || pInfo->yres <= 0)
            {
                pInfo->yres = 240;
            }
        
            reader.ReadInt("bits_per_pix", pInfo->bits_per_pix);


            /* Fixme KGM */
            reader.ReadBool("packed_format", pInfo->packed_format);
            reader.ReadBool("format_555", pInfo->format_555);
            reader.ReadBool("format_4444", pInfo->format_4444);
            reader.ReadBool("format_332", pInfo->format_332);
            reader.ReadBool("grayscale", pInfo->grayscale);
            reader.ReadBool("reverse_order", pInfo->reverse_order);
            reader.ReadBool("enabled", pInfo->enabled);

            CString string;
            reader.ReadString("rotation_angle", string);
            pInfo->rotation_angle = ProjectConfigDlg::FindScreenRotationVal(string);

            reader.ReadBool("default_map_format", pInfo->default_map_format);

            /* KGM - this is a fix for previous versions of Studio allowing invalid
                     Dave2D display format
            */
            if (IsCpuWithDave2D(mHeader.target_cpu))
            {
                pInfo->packed_format = FALSE;
                pInfo->format_555 = FALSE;
                pInfo->format_332 = FALSE;
                pInfo->reverse_order = FALSE;
            }
            
            if (!reader.ReadBool("allocate_canvas", pInfo->allocate_canvas))
            {
                // old project without this flag, default to TRUE
                pInfo->allocate_canvas = TRUE;
            }

            /* KGM fixeme: This should be determined by bits-per-pix and FLAGS */
            switch(pInfo->bits_per_pix)
            {
            case 1:
                pInfo->colorformat = GX_COLOR_FORMAT_MONOCHROME;
                break;

            case 2:
                ErrorMsg("This project specifies an unsupported output format. Closing.");
                return FALSE;

            case 4:
                pInfo->colorformat = GX_COLOR_FORMAT_4BIT_GRAY;
                break;

            case 8:
                if (pInfo->format_332)
                {
                    pInfo->colorformat = GX_COLOR_FORMAT_8BIT_PACKED_PIXEL;
                }
                else
                {
                    pInfo->colorformat = GX_COLOR_FORMAT_8BIT_PALETTE;
                }
                break;

            case 24:
                if (pInfo->packed_format)
                {
                    pInfo->colorformat = GX_COLOR_FORMAT_24RGB;
                }
                else
                {
                    pInfo->colorformat = GX_COLOR_FORMAT_24XRGB;
                }
                break;

            case 32:
                if (pInfo->reverse_order)
                {
                    pInfo->colorformat = GX_COLOR_FORMAT_32BGRA;
                }
                else
                {
                    pInfo->colorformat = GX_COLOR_FORMAT_32ARGB;
                }
                break;

            default:
                pInfo->bits_per_pix = 16;

                if (pInfo->format_4444)
                {
                    if (pInfo->reverse_order)
                    {
                        pInfo->colorformat = GX_COLOR_FORMAT_4444BGRA;
                    }
                    else
                    {
                        pInfo->colorformat = GX_COLOR_FORMAT_4444ARGB;
                    }
                }
                else if (pInfo->format_555)
                {
                    if (pInfo->reverse_order)
                    {
                        pInfo->colorformat = GX_COLOR_FORMAT_5551BGRX;
                    }
                    else
                    {
                        pInfo->colorformat = GX_COLOR_FORMAT_1555XRGB;
                    }
                }
                else
                {
                    if (pInfo->reverse_order)
                    {
                        pInfo->colorformat = GX_COLOR_FORMAT_565BGR;
                    }
                    else
                    {
                        pInfo->colorformat = GX_COLOR_FORMAT_565RGB;
                    }
                }
            }

            CleanupDisplayResources(pInfo);

            if (reader.EnterSection("theme_info"))
            {
                reader.ReadInt("num_themes", pInfo->num_themes, 1);

                if (!reader.ReadInt("active_theme", pInfo->active_theme))
                {
                    pInfo->active_theme = DEFAULT_THEME;
                }

                if (pInfo->num_themes <= 0 || pInfo->num_themes > MAX_THEMES)
                {
                    // corrupt or missing theme info, create defaults
                    pInfo->num_themes = 1;
                    pInfo->active_theme = DEFAULT_THEME;
                    InitDisplayThemes(index);
                }
                else
                {
                    if (pInfo->active_theme < 0 || pInfo->active_theme >= pInfo->num_themes)
                    {
                        ErrorMsg("The project file is corrupted, restoring defaults.");
                        pInfo->active_theme = DEFAULT_THEME;
                    }
               
                    for (theme = 0; theme < pInfo->num_themes; theme++)
                    {
                        DefaultScrollbarAppearance(pInfo, theme);

                        reader.ReadString("theme_name", pInfo->themes[theme].theme_name);

                        if (!reader.ReadBool("gen_color_table", pInfo->themes[theme].gen_color_table))
                        {
                            pInfo->themes[theme].gen_color_table = TRUE;
                        }

                        if (!reader.ReadBool("gen_font_table", pInfo->themes[theme].gen_font_table))
                        {
                            pInfo->themes[theme].gen_font_table = TRUE;
                        }

                        if (!reader.ReadBool("gen_pixelmap_table", pInfo->themes[theme].gen_pixelmap_table))
                        {
                            pInfo->themes[theme].gen_pixelmap_table = TRUE;
                        }

                        if (!reader.ReadBool("enabled", pInfo->themes[theme].enabled))
                        {
                            pInfo->themes[theme].enabled = TRUE;
                        }

                        if (!reader.ReadBool("statically_defined", pInfo->themes[theme].statically_defined))
                        {
                            pInfo->themes[theme].statically_defined = TRUE;
                        }

                        if (reader.EnterSection("theme_data"))
                        {
                            if (mHeader.project_version >= 53)
                            {
                                // resources come first in the new schema
                                ReadResources(reader, index, theme, NULL);
                            }

                            ReadThemeScrollbars(reader, index, theme);
                            
                            ReadThemePaletteInfo(reader, &pInfo->themes[theme]);

                            if (mHeader.project_version <= 52)
                            {
                                // in older projects, resources come after the theme info
                                ReadResources(reader, index, theme, NULL);
                            }
                            reader.CloseSection(TRUE, TRUE);


                            if (mHeader.project_version <= 54)
                            {
                                //add default "disabled text color", "disabled fill color"
                                //            "readonly text color", "readonly fill color"

                                res_info *parent = FindResourceFolder(pInfo->themes[theme].GetFirstResourceInfo(), RES_TYPE_FOLDER, DEFAULT_COLOR_FOLDER);
                                COLOR_RECORD *default_record = ProjectConfigDlg::GetDefaultColorTable(pInfo->colorformat);

                                res_info *newres;
                                while (default_record->name)
                                {
                                    switch (default_record->color_id)
                                    {
                                    case GX_COLOR_ID_DISABLED_TEXT:
                                    case GX_COLOR_ID_DISABLED_FILL:
                                    case GX_COLOR_ID_READONLY_TEXT:
                                    case GX_COLOR_ID_READONLY_FILL:
                                        if (FindResource(parent, RES_TYPE_COLOR, CString(default_record->name)) == NULL)
                                        {
                                            newres = new res_info(RES_TYPE_COLOR);

                                            newres->colorval = ProjectConfigDlg::GetColorVal(default_record->rgb_val,
                                                                                            pInfo->themes[theme].palette,
                                                                                            pInfo->themes[theme].palette_predefined,
                                                                                            pInfo->colorformat);
                                            newres->name = default_record->name;
                                            newres->is_default = TRUE;
                                            parent->Attach(newres);
                                        }
                                        break;
                                    default:
                                        break;
                                    }

                                    default_record++;
                                }
                            }
                        }
                        else
                        {
                            old_theme_data_format = TRUE;
                        }
                    }
                }
                reader.CloseSection(TRUE, TRUE);
            }
            else
            {
                return FALSE;
            }

            for (int language = 0; language < mHeader.num_languages; language++)
            {
                if (reader.ReadBool(CT2A(mHeader.languages[language].name), enabled))
                {
                    mDisplays[index].gen_string_table[language] = enabled;
                }
            }
            
            if (mHeader.project_version <= 52)
            {
                ReadWidgetFolders(reader, index);

                if (old_theme_data_format)
                {
                    ReadResources(reader, index, 0, NULL);
                }
                ReadStringTable(reader, pInfo);
            }
            else
            {
                ReadStringTable(reader, pInfo);
                ReadScreenFlow(reader, index);
                ReadWidgetFolders(reader, index);
            }
        }
        reader.CloseSection(TRUE, TRUE);
    }
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
void studiox_project::ReadProjectHeader(xml_reader &reader)
{
    InitProjectHeader(FALSE);
    int index;

    mHeader.warn_missing_image = TRUE;
    mHeader.warn_missing_font = TRUE;

    if (reader.EnterSection("header"))
    {
        reader.ReadInt("project_version", mHeader.project_version, PROJECT_VERSION);

        // default to the original version, 5.0, if no version information
        reader.ReadInt("guix_version", mHeader.guix_version, 50000);
        
        // default to version 5.3.2.0, if no version information
        reader.ReadInt("studio_version", mHeader.studio_version, 5030200);

        if (mHeader.guix_version < 50000)
        {
            int version = mHeader.guix_version;
            // convert to new format
            int major = version / 10;
            version -= (major * 10);
            int minor = version;
            mHeader.guix_version = GuixVersionFieldsToVersionNumber(major, minor, 0);
        }

        reader.ReadString("project_name", mHeader.project_name);
        reader.ReadString("source_path", mHeader.source_path);
        reader.ReadString("header_path", mHeader.header_path);
        reader.ReadString("resource_path", mHeader.resource_path);
        reader.ReadString("allocator_function", mHeader.malloc_name);
        reader.ReadString("free_function", mHeader.free_name);
        reader.ReadString("additional_headers", mHeader.additional_headers);
        reader.ReadBool("insert_headers_before", mHeader.insert_headers_before);

        reader.ReadInt("target_cpu", mHeader.target_cpu);
        reader.ReadInt("target_tools", mHeader.target_tools);
        reader.ReadBool("big_endian", mHeader.big_endian);

        if (IsCpuWithDave2D(mHeader.target_cpu))
        {
            if (!reader.ReadBool("dave2d_graph_accelerator", mHeader.dave2d_graph_accelerator))
            {
                if (!reader.ReadBool("synergy_graph_accelerator", mHeader.dave2d_graph_accelerator))
                {
                    mHeader.dave2d_graph_accelerator = TRUE;
                }
            }

            if (!reader.ReadInt("renesas_jpeg_decoder", mHeader.renesas_jpeg_decoder))
            {
                if (!reader.ReadInt("synergy_jpeg_decoder", mHeader.renesas_jpeg_decoder))
                {
                    mHeader.renesas_jpeg_decoder = DECODER_TYPE_HW;
                }
            }
            
            if (!reader.ReadInt("renesas_png_decoder", mHeader.renesas_png_decoder))
            {
                if (!reader.ReadInt("synergy_png_decoder", mHeader.renesas_png_decoder))
                {
                    mHeader.renesas_png_decoder = DECODER_TYPE_NONE;
                }
            }
        }
        else
        {
            mHeader.dave2d_graph_accelerator = FALSE;
            mHeader.renesas_jpeg_decoder = DECODER_TYPE_NONE;
            mHeader.renesas_png_decoder = DECODER_TYPE_NONE;
        }

        if (!reader.ReadBool("grid_enabled", mHeader.grid_enabled))
        {
            mHeader.grid_enabled = FALSE;
        }

        if (!reader.ReadBool("snap_enabled", mHeader.snap_enabled))
        {
            mHeader.snap_enabled = FALSE;
        }

        if (!reader.ReadBool("snap_to_widget_enabled", mHeader.snap_to_widget_enabled))
        {
            mHeader.snap_to_widget_enabled = FALSE;
        }

        if (!reader.ReadInt("grid_spacing", mHeader.grid_spacing))
        {
            mHeader.grid_spacing = 10;
        }

        RANGE_CHECK(mHeader.grid_spacing, MIN_GRID_SNAP_SPACE, MAX_GRID_SNAP_SPACE);

        if (!reader.ReadInt("snap_spacing", mHeader.snap_spacing))
        {
            mHeader.snap_spacing = 10;
        }

        RANGE_CHECK(mHeader.snap_spacing, MIN_GRID_SNAP_SPACE, MAX_GRID_SNAP_SPACE);

        reader.ReadBool("gen_binary", mHeader.gen_binary);
        reader.ReadUnsigned("binary_file_format", mHeader.binary_file_format, BINARY_FILE_FORMAT_SREC);
        reader.ReadUnsigned("memory_offset", mHeader.memory_offset, 0);

        if (!reader.ReadBool("gen_res_header", mHeader.gen_res_header))
        {
            mHeader.gen_res_header = TRUE;
        }

        if (!reader.ReadBool("custom_resource_enabled", mHeader.custom_resource_enabled))
        {
            mHeader.custom_resource_enabled = FALSE;
        }

        if (!reader.ReadString("custom_resource_file_name", mHeader.custom_resource_file_name))
        {
            mHeader.custom_resource_file_name = "";
        }

        // app execute position will be checked before use, so no need to do range check here.

        reader.ReadInt("app_execute_xpos", mHeader.app_execute_xpos);
        reader.ReadInt("app_execute_ypos", mHeader.app_execute_ypos);

        reader.ReadBool("is_widget_position_locked", mHeader.is_widget_position_locked);

        if (!reader.ReadInt("palette_mode_aa_text_colors", mHeader.palette_mode_aa_text_colors))
        {
            mHeader.palette_mode_aa_text_colors = 8;
        }

        reader.ReadInt("num_displays", mHeader.num_displays, 1);
        RANGE_CHECK(mHeader.num_displays, 1, MAX_DISPLAYS);
        reader.ReadInt("max_displays", mHeader.max_displays, 4);
        RANGE_CHECK(mHeader.max_displays, mHeader.num_displays, MAX_DISPLAYS);
        reader.ReadInt("num_languages", mHeader.num_languages, 1);
        RANGE_CHECK(mHeader.num_languages, 1, MAX_LANGUAGES);

        BOOL enabled;
        if (reader.EnterSection("language_names"))
        {
            for (index = 0; index < mHeader.num_languages; index++)
            {
                // we used to save the combined name in the project,
                // i.e. the language name + the { symbol }. Now
                // we just save the language name, so check for the
                // symbol and get rid of it if it is there:

                CString temp_name;
                int symbol_index;

                reader.ReadString("language", temp_name);
                symbol_index = temp_name.ReverseFind('{');

                if (symbol_index > 0)
                {
                    temp_name = temp_name.Left(symbol_index - 1);
                }

                mHeader.languages[index].name = config_languages_dlg::ConvertOldLanguageName(temp_name);

                if (reader.ReadBool("enabled", enabled))
                {
                    for (int display_index = 0; display_index < mHeader.num_displays; display_index++)
                    {
                        mDisplays[display_index].gen_string_table[index] = enabled;
                    }
                }

                if (!reader.ReadBool("support_bidi_text", mHeader.languages[index].support_bidi_text))
                {
                    mHeader.languages[index].support_bidi_text = FALSE;
                }

                if (!reader.ReadBool("gen_reordered_bidi_text", mHeader.languages[index].gen_reordered_bidi_text))
                {
                    mHeader.languages[index].gen_reordered_bidi_text = FALSE;
                }

                if (!reader.ReadBool("support_thai_glyph_shaping", mHeader.languages[index].support_thai_glyph_shaping))
                {
                    mHeader.languages[index].support_thai_glyph_shaping = FALSE;
                }

                if (!reader.ReadBool("gen_adjusted_thai_string", mHeader.languages[index].gen_adjusted_thai_string))
                {
                    mHeader.languages[index].gen_adjusted_thai_string = FALSE;
                }

                if (!reader.ReadBool("statically_defined", mHeader.languages[index].statically_defined))
                {
                    mHeader.languages[index].statically_defined = TRUE;
                }
            }
            reader.CloseSection(TRUE, TRUE);
        }
    
        if (reader.EnterSection("string_export"))
        {
            reader.ReadInt("string_export_src", mHeader.string_export_src);
            reader.ReadInt("string_export_target", mHeader.string_export_target);
            reader.ReadInt("string_export_version", mHeader.string_export_version);
            reader.ReadString("string_export_path", mHeader.string_export_path);
            reader.ReadString("string_export_name", mHeader.string_export_filename);

            CString file_typename;
            reader.ReadString("string_export_filetype", file_typename);
            mHeader.string_export_filetype = string_export_dlg::GetStringExportType(file_typename);

            reader.CloseSection(TRUE, TRUE);
        }
        else if (reader.EnterSection("xliff"))
        {
            reader.ReadInt("xliff_src", mHeader.string_export_src);
            reader.ReadInt("xliff_target", mHeader.string_export_target);
            reader.ReadInt("xliff_version", mHeader.string_export_version);
            reader.ReadString("xliff_path", mHeader.string_export_path);
            reader.ReadString("xliff_name", mHeader.string_export_filename);
            reader.CloseSection(TRUE, TRUE);
        }
        else
        {
            InitStringExportHeader();
        }

        reader.CloseSection(TRUE, TRUE);
    }
}

///////////////////////////////////////////////////////////////////////////////
BOOL studiox_project::Read(CString &pathname)
{
    xml_reader reader;

    if (!reader.ReadFile(pathname))
    {
        CString error("Unable to read project file: ");
        error += pathname;
        ErrorMsg(error);
        return FALSE;
    }

    if (reader.EnterSection("project"))
    {
        ReadProjectHeader(reader);

        int studio_version = CalculateStudioVersion();

        #ifdef ENABLE_STUDIO_VERSION_TEST
        if ((mHeader.project_version > PROJECT_VERSION) ||
            (mHeader.studio_version > studio_version))
        {
            if (!AskUser("This project was created by a newer release of GUIX Studio. Do you want to continue?"))
            {
                return FALSE;
            }
        }
        #endif

        // make sure the project name matches the actual filename, which can 
        // happen if the user copies a project:
        mHeader.project_name = pathname;
        int index = mHeader.project_name.ReverseFind('\\');
        if (index > 0)
        {
            mHeader.project_name = mHeader.project_name.Mid(index + 1);
        }

        index = mHeader.project_name.ReverseFind('.');
        if (index > 0)
        {
            mHeader.project_name = mHeader.project_name.Left(index);
        }

        for (int index = 0; index < mHeader.max_displays; index++)
        {
            if (!ReadDisplayInfo(reader, index))
            {
                ErrorMsg("Invalid project file, unable to open project.");
                return FALSE;
            }
        }
        InitializeProjectResources();

        if (mDisplays[0].themes[0].GetFirstResourceInfo() == GX_NULL)
        {
            ErrorMsg("Invalid project file, unable to open project.");
            return FALSE;
        }

        reader.CloseSection();

        // update to the current studio version and project version
        if ((mHeader.studio_version != studio_version)||
            (mHeader.project_version != PROJECT_VERSION))
        {
            mHeader.studio_version = studio_version;
            mHeader.project_version = PROJECT_VERSION;
            is_modified = TRUE;
        }
        else
        {
            is_modified = FALSE;
        }
        return TRUE;
    }
    else
    {
        ErrorMsg("The selected file is not a valid GUIX Studio project.");
        return FALSE;
    }
}

///////////////////////////////////////////////////////////////////////////////
 BOOL studiox_project::AddToIdDictionary(int DisplayIndex, int id_type, CString &id_name)
 {
     CArray<id_info> *dictionary;
     id_info         *get_info;
     id_info          new_info;

     if (DisplayIndex < 0)
     {
         return FALSE;
     }

     switch (id_type)
     {
     case ID_TYPE_WIDGET:
         dictionary = &widget_id_dictionary[DisplayIndex];
         if (id_name == "ID_DROP_LIST_BUTTON")
         {
             //ID_DROP_LIST_BUTTON is defined internal
             return FALSE;
         }
         break;

     case ID_TYPE_ANIMATION:
         dictionary = &animation_id_dictionary[DisplayIndex];
         break;

     default:
         return FALSE;
     }

     if (dictionary->GetCount() == 0)
     {
         new_info.id_name = "";
         new_info.reference_count = 0;
         dictionary->Add(new_info);
     }

    for (int index = 0; index < dictionary->GetCount(); index++)
    {
        get_info = &dictionary->GetAt(index);

        /* Check if the id name already exist. */
        if(get_info->id_name == id_name)
        {
            get_info->reference_count++;
            return GX_FALSE;
        }
    }

    new_info.id_name = id_name;
    new_info.reference_count = 1;

    dictionary->Add(new_info);
    return GX_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
BOOL studiox_project::RemoveFromIdDictionary(int DisplayIndex, int id_type, CString &id_name)
{
     CArray<id_info> *dictionary;
     id_info *get_info;

     if (DisplayIndex < 0)
     {
         return FALSE;
     }

     switch (id_type)
     {
     case ID_TYPE_WIDGET:
         dictionary = &widget_id_dictionary[DisplayIndex];
         break;

     case ID_TYPE_ANIMATION:
         dictionary = &animation_id_dictionary[DisplayIndex];
         break;
       
     default:
         return FALSE;
     }

     for (int index = 0; index < dictionary->GetCount(); index++)
     {
         get_info = &dictionary->GetAt(index);

         if (get_info->id_name == id_name)
         {
             if (get_info->reference_count == 1)
             {
                 /* Remove the id name from the id dictionary. */
                 dictionary->RemoveAt(index);
                 return GX_TRUE;
             }
             else
             {
                 get_info->reference_count--;
                 return GX_FALSE;
             }
         }
     }

     return GX_FALSE;
}

INT studiox_project::GetIdIndex(const CArray<id_info> &dictionary, const CString &id_name) const
{
    for (int index = 1; index < dictionary.GetCount(); index++)
    {
        if (dictionary.GetAt(index).id_name == id_name)
        {
            /* Set id value according to dictionary index. */
            return index;
        }
    }

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
INT studiox_project::GetIdIndex(int DisplayIndex, int id_type, const CString &id_name) const
{
    if (DisplayIndex < 0)
    {
        return 0;
    }

    switch (id_type)
    {
    case ID_TYPE_WIDGET:
        if (id_name == "ID_DROP_LIST_BUTTON")
        {
            //ID_DROP_LIST_BUTTON is defined internal
            return ID_DROP_LIST_BUTTON;
        }
        else
        {
            return GetIdIndex(widget_id_dictionary[DisplayIndex], id_name);
        }

    case ID_TYPE_ANIMATION:
        return GetIdIndex(animation_id_dictionary[DisplayIndex], id_name);

    default:
        return 0;
    }

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
CString studiox_project::GetIdName(int DisplayIndex, int id_type, int index) const
{
    const CArray<id_info> *dictionary;

    if (DisplayIndex < 0)
    {
        return  _T("");
    }

    switch (id_type)
    {
    case ID_TYPE_WIDGET:
        dictionary = &widget_id_dictionary[DisplayIndex];
        break;

    case ID_TYPE_ANIMATION:
        dictionary = &animation_id_dictionary[DisplayIndex];
        break;

    default:
        return _T("");
    }

    if (index < dictionary->GetCount())
    {
        return dictionary->GetAt(index).id_name;
    }

    return _T("");
}

///////////////////////////////////////////////////////////////////////////////
void studiox_project::CleanupIdDictionary(int DisplayIndex, int id_type)
{
    CArray<id_info> *dictionary;

    if (DisplayIndex < 0)
    {
        return ;
    }

    switch (id_type)
    {
    case ID_TYPE_WIDGET:
        dictionary = &widget_id_dictionary[DisplayIndex];
        break;

    case ID_TYPE_ANIMATION:
        dictionary = &animation_id_dictionary[DisplayIndex];
        break;

    default:
        return;
    }

    dictionary->RemoveAll();
}

///////////////////////////////////////////////////////////////////////////////
BOOL studiox_project::CopyIdDictionary(int DisplayIndex, int id_type, CArray<id_info> *copied_dictionary)
{
    if (copied_dictionary)
    {
        const CArray<id_info> *dictionary;

        switch (id_type)
        {
        case ID_TYPE_WIDGET:
            dictionary = &widget_id_dictionary[DisplayIndex];
            break;

        case ID_TYPE_ANIMATION:
            dictionary = &animation_id_dictionary[DisplayIndex];
            break;

        default:
            return FALSE;
        }

        copied_dictionary->RemoveAll();
        for (int index = 0; index < dictionary->GetCount(); index++)
        {
            copied_dictionary->Add(dictionary->GetAt(index));
        }

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static int compare_id(id_info *id_1, id_info *id_2)
{
    return id_1->id_name.Compare(id_2->id_name);
}

///////////////////////////////////////////////////////////////////////////////
void studiox_project::SortIdDictionary(CArray<id_info> *dictionary)
{

    if(dictionary->GetSize() > 2)
    {
        qsort(&dictionary->GetAt(1), dictionary->GetSize() - 1, sizeof(id_info), (int(*)(const void *, const void *))compare_id);
    }
}

///////////////////////////////////////////////////////////////////////////////
BOOL studiox_project::AddToResourceDictionary(int DisplayIndex, res_info *info, int res_id)
{
    int index;
    CArray<CString> *dictionary;

    switch (info->type)
    {
    case RES_TYPE_COLOR:
        dictionary = &color_dictionary[DisplayIndex];
        if ((mHeader.project_version <= 54) &&
            (dictionary->GetCount() == 0))
        {
            //Add default color ids
            COLOR_RECORD *color_record = DEFAULT_COLOR_TABLE;
            while (color_record->name)
            {
                dictionary->Add(CString(color_record->name));
                color_record++;
            }
        }
        break;

    case RES_TYPE_FONT:
        dictionary = &font_dictionary[DisplayIndex];
        break;

    case RES_TYPE_PIXELMAP:
        dictionary = &pixelmap_dictionary[DisplayIndex];
        if (dictionary->GetCount() == 0)
        {
            dictionary->Add(CString(""));
        }
        break;

    default:
        return FALSE;
    }

    for (index = 0; index < dictionary->GetCount(); index++)
    {
        if (dictionary->GetAt(index) == info->name)
        {
            return FALSE;
        }
    }

    if (res_id == -1)
    {
        dictionary->Add(info->name);
    }
    else
    {
        dictionary->SetAtGrow(res_id, info->name);
    }
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
BOOL studiox_project::RemoveFromResourceDictionary(int DisplayIndex, res_info *info)
{
    int index;
    CArray<CString> *dictionary;

    switch (info->type)
    {
    case RES_TYPE_COLOR:
        dictionary = &color_dictionary[DisplayIndex];
        break;

    case RES_TYPE_FONT:
        dictionary = &font_dictionary[DisplayIndex];
        break;

    case RES_TYPE_PIXELMAP:
        dictionary = &pixelmap_dictionary[DisplayIndex];
        break;

    default:
        return FALSE;
    }

    for (index = 0; index < dictionary->GetCount(); index++)
    {
        if (dictionary->GetAt(index) == info->name)
        {
            dictionary->RemoveAt(index);
            return TRUE;
        }
    }

    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
BOOL studiox_project::UpdateDictionaryResourceName(int DisplayIndex, CString &old_name, res_info *info)
{
    int index;
    CArray<CString> *dictionary;

    switch (info->type)
    {
    case RES_TYPE_COLOR:
        dictionary = &color_dictionary[DisplayIndex];
        break;

    case RES_TYPE_FONT:
        dictionary = &font_dictionary[DisplayIndex];
        break;

    case RES_TYPE_PIXELMAP:
        dictionary = &pixelmap_dictionary[DisplayIndex];
        break;

    default:
        return FALSE;
    }

    for (index = 0; index < dictionary->GetCount(); index++)
    {
        if (dictionary->GetAt(index) == old_name)
        {
            dictionary->SetAt(index, info->name);
            return TRUE;
        }
    }

    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
BOOL studiox_project::GetResourceName(int Display, int res_type, int index, CString &return_name) const
{
    const CArray<CString> *dictionary;
    BOOL found = FALSE;
    return_name.Empty();

    switch(res_type)
    {
    case RES_TYPE_COLOR:
        dictionary = &color_dictionary[Display];
        break;

    case RES_TYPE_FONT:
        dictionary = &font_dictionary[Display];
        break;

    case RES_TYPE_PIXELMAP:    
        dictionary = &pixelmap_dictionary[Display];
        break;

    default:
        return FALSE;
    }
    
    if (index < dictionary->GetCount())
    {
        return_name = dictionary->GetAt(index);
        return TRUE;
    }
    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
BOOL studiox_project::CopyDictionary(int Display, int res_type, CArray<CString> *copied_dictionary)
{
    if (copied_dictionary)
    {
        const CArray<CString> *dictionary;

        switch (res_type)
        {
        case RES_TYPE_COLOR:
            dictionary = &color_dictionary[Display];
            break;

        case RES_TYPE_FONT:
            dictionary = &font_dictionary[Display];
            break;

        case RES_TYPE_PIXELMAP:
            dictionary = &pixelmap_dictionary[Display];
            break;

        default:
            return FALSE;
        }

        copied_dictionary->RemoveAll();
        for (int index = 0; index < dictionary->GetCount(); index++)
        {
            copied_dictionary->Add(dictionary->GetAt(index));
        }

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static int compare_string(CString *record_1, CString *record_2)
{
    return record_1->Compare(*record_2);
}

///////////////////////////////////////////////////////////////////////////////
void studiox_project::SortResDictionary(INT res_type, CArray<CString> *dictionary)
{
    int skip_count = 0;

    switch (res_type)
    {
    case RES_TYPE_PIXELMAP:
    {
        CCommandInfo* pCmdInfo = GetCmdInfo();
        if (!pCmdInfo->IsXmlMode())
        {
            skip_count = GX_DEFAULT_PIXELMAP_COUNT;
        }
    }
        break;

    case RES_TYPE_FONT:
    case RES_TYPE_COLOR:
        break;

    default:
        return;
    }

    if (dictionary->GetSize() > skip_count + 1)
    {
        qsort(&dictionary->GetAt(skip_count), dictionary->GetSize() - skip_count,
            sizeof(CString), (int(*)(const void *, const void *))compare_string);
    }
}

///////////////////////////////////////////////////////////////////////////////
GX_RESOURCE_ID studiox_project::GetResourceId(int Display, const int res_type, const CString &name) const
{
    const CArray<CString> *dictionary;
    int index;

    if (name.IsEmpty())
    {
        return 0;
    }

    switch(res_type)
    {
    case RES_TYPE_COLOR:
        dictionary = &color_dictionary[Display];
        break;

    case RES_TYPE_FONT:
        dictionary = &font_dictionary[Display];
        break;

    case RES_TYPE_PIXELMAP:    
        dictionary = &pixelmap_dictionary[Display];
        break;

    default:
        return 0;
    }
    
    for (index = 0; index < dictionary->GetCount(); index++)
    {
        if (dictionary->GetAt(index) == name)
        {
            return index;
        }
    }
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
GX_RESOURCE_ID studiox_project::GetResourceId(int Display, const res_info *info) const
{
    return GetResourceId(Display, info->type, info->name);
}

///////////////////////////////////////////////////////////////////////////////
res_info *studiox_project::FindResource(int Display, int theme, int restype, const CString &name) const
{
    return FindResource(mDisplays[Display].themes[theme].GetFirstResourceInfo(), restype, name);
}


///////////////////////////////////////////////////////////////////////////////
res_info *studiox_project::FindResource(int Display, int theme_id, int res_type, GX_RESOURCE_ID res_id) const
{
    CString name;
    const CArray<CString> *dictionary;

    res_info *pRes = mDisplays[Display].themes[theme_id].GetFirstResourceInfo();

    switch(res_type)
    {
    case RES_TYPE_COLOR:
        dictionary = &color_dictionary[Display];
        if (res_id < (ULONG) dictionary->GetCount())
        {
            name = dictionary->GetAt(res_id);
            return FindResource(pRes, res_type, name);    
        }
        else
        {
            return NULL;
        }
        break;
        
    case RES_TYPE_FONT:
        dictionary = &font_dictionary[Display];
        if (res_id < (ULONG) dictionary->GetCount())
        {
            name = dictionary->GetAt(res_id);
            return FindResource(pRes, res_type, name);    
        }
        else
        {
            return NULL;
        }
        break;

    case RES_TYPE_PIXELMAP:
        dictionary = &pixelmap_dictionary[Display];

        if (res_id < (ULONG) dictionary->GetCount())
        {
            name = dictionary->GetAt(res_id);
            return FindResource(pRes, res_type, name);    
        }
        else
        {
            return NULL;
        }
        break;

    default:
        return FindResourceFolder(pRes, res_type, res_id);
    }
}




///////////////////////////////////////////////////////////////////////////////
res_info *studiox_project::FindResource(const res_info *pRes, int restype, const CString &name) const
{
    res_info *found = NULL;

    while(pRes)
    {
        if (pRes->type == restype &&
            pRes->name == name)
        {
            return (res_info *)pRes;
        }
        if (pRes->child)
        {
            found = FindResource(pRes->child, restype, name);
            if (found)
            {
                return found;
            }
        }
        pRes = pRes->next;
    }
    return NULL;
} 

///////////////////////////////////////////////////////////////////////////////
res_info* studiox_project::FindResourceFolder(int Display, int theme_id, int res_type, GX_RESOURCE_ID res_id, const CString& name) const
{
    return  FindResourceFolder(mDisplays[Display].themes[theme_id].GetFirstResourceInfo(), res_type, res_id, name);
}

///////////////////////////////////////////////////////////////////////////////
res_info *studiox_project::FindResourceFolder(const res_info *pRes, int restype, int folder_id, CString name) const
{
    res_info *found = NULL;

    while(pRes)
    {

        if (pRes->type == restype &&
            pRes->folder_id == folder_id && 
            ((pRes->name == name) || name.IsEmpty()))
        {
            return (res_info *)pRes;
        }
        if (pRes->child)
        {
            found = FindResourceFolder(pRes->child, restype, folder_id, name);
            if (found)
            {
                return found;
            }
        }
        pRes = pRes->next;
    }
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
widget_info *studiox_project::FindParentInfo(const widget_info *start, const widget_info *child) const
{
    widget_info *test;
    widget_info *found;

    while(start)
    {
        test = start->GetChildWidgetInfo();

        while(test)
        {
            if (test == child)
            {
                return (widget_info *)start;
            }
            found = FindParentInfo(test, child);
            if (found)
            {
                return found;
            }
            test = test->GetNextWidgetInfo();
        }
        start = start->GetNextWidgetInfo();
    }
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
widget_info *studiox_project::FindParentInfo(const widget_info *child) const
{
    widget_info *found = NULL;

    for (int Display = 0; Display < MAX_DISPLAYS; Display++)
    {
        folder_info *pInfo = mDisplays[Display].GetFirstChildFolder();
        found = FindParentInfo(pInfo, child);
        if (found)
        {
            return found;
        }
    }
    return NULL;    
}

///////////////////////////////////////////////////////////////////////////////
widget_info *studiox_project::FindParentInfo(const folder_info *folder, const widget_info *child) const
{
    widget_info *found = GX_NULL;
    while (folder)
    {
        found = FindParentInfo(folder->GetFirstChildWidget(), child);
        if (found)
        {
            return found;
        }
        folder = folder->GetNextFolder();
    }
    return GX_NULL;
}

///////////////////////////////////////////////////////////////////////////////
widget_info *studiox_project::FindWidgetInfo(const folder_info *folder, const CString &name, BOOL search_child) const
{
    widget_info *found = GX_NULL;

    while (folder)
    {
        found = FindWidgetInfo(folder->GetFirstChildWidget(), name, search_child);
        if (found)
        {
            return found;
        }
        folder = folder->GetNextFolder();
    }
    return GX_NULL;
}

///////////////////////////////////////////////////////////////////////////////
widget_info *studiox_project::FindWidgetInfo(const widget_info *start, const CString &name, BOOL search_child) const
{
    widget_info *found = NULL;

    while(start)
    {
        if (start->app_name == name)
        {
            return (widget_info *)start;
        }

        if (search_child && start->GetChildWidgetInfo())
        {
            found = FindWidgetInfo(start->GetChildWidgetInfo(), name, search_child);

            if (found)
            {
                return found;
            }
        }
        start = start->GetNextWidgetInfo();
    }
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
const widget_info *studiox_project::GetTailInfo(const widget_info *info) const
{
    const widget_info *tail = info;
    while (tail->GetNextWidgetInfo())
    {
        tail = tail->GetNextWidgetInfo();
    }

    return tail;
}

///////////////////////////////////////////////////////////////////////////////
folder_info *studiox_project::FindFolderInfo(const folder_info *start, const CString &name) const
{
    while (start)
    {
        if (start->folder_name == name)
        {
            return (folder_info *)start;
        }
        start = start->GetNextFolder();
    }
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
folder_info *studiox_project::FindFolderInfo(int display_index, const CString &name) const
{
    folder_info *found = NULL;

    if (display_index < MAX_DISPLAYS)
    {
        found = FindFolderInfo(mDisplays[display_index].GetFirstChildFolder(), name);
        if (found)
        {
            return found;
        }
    }

    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
folder_info *studiox_project::FindParentFolderInfo(const widget_info *child) const
{
    for (int DisplayIndex = 0; DisplayIndex < MAX_DISPLAYS; DisplayIndex++)
    {
        folder_info *folder = mDisplays[DisplayIndex].GetFirstChildFolder();
        while (folder)
        {
            if (IsWidgetInInfoTree(folder->GetFirstChildWidget(), child))
            {
                return folder;
            }
            folder = folder->GetNextFolder();
        }
    }

    return NULL;
}


///////////////////////////////////////////////////////////////////////////////
widget_info *studiox_project::FindWidgetInfo(const GX_WIDGET *widget) const
{
    widget_info *found = NULL;

    if (widget && (widget->gx_widget_type == GX_TYPE_MENU_LIST))
    {
        widget = ((GX_MENU_LIST *)widget)->gx_menu_list_owner;
    }

    for (int Display = 0; Display < MAX_DISPLAYS; Display++)
    {
        folder_info *pFolder = mDisplays[Display].GetFirstChildFolder();
        found = FindWidgetInfo(pFolder, widget);
        if (found)
        {
            return found;
        }
    }
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
widget_info *studiox_project::FindWidgetInfo(const folder_info *folder, const GX_WIDGET *widget) const
{
    widget_info *found = NULL;
    while (folder)
    {
        widget_info *child = folder->GetFirstChildWidget();

        if (child)
        {
            if (child->widget == widget)
            {
                return child;
            }
            found = FindWidgetInfo(child, widget);

            if (found)
            {
                return found;
            }
        }
        folder = folder->GetNextFolder();
    }
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
widget_info *studiox_project::FindWidgetInfo(const widget_info *start, const GX_WIDGET *widget) const
{
    widget_info *found = NULL;

    while(start)
    {
        if (start->widget == widget)
        {
            return (widget_info *)start;
        }

        if (start->GetChildWidgetInfo())
        {
            found = FindWidgetInfo(start->GetChildWidgetInfo(), widget);

            if (found)
            {
                return found;
            }
        }
        start = start->GetNextWidgetInfo();
    }
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
void folder_info::copy(const folder_info &other)
{
    folder_name = other.folder_name;
    output_filename = other.output_filename;
    this->first_widget = NULL;
    this->next = NULL;
}

///////////////////////////////////////////////////////////////////////////////
folder_info &folder_info::operator=(const folder_info &other)
{
    copy(other);
    return *this;
}

///////////////////////////////////////////////////////////////////////////////
folder_info::folder_info()
{
    folder_name.Empty();
    output_filename.Empty();
    first_widget = NULL;
    next = NULL;
}

///////////////////////////////////////////////////////////////////////////////
folder_info::folder_info(const folder_info &other, BOOL copy_next)  // copy constructor
{
    copy(other);

    folder_info * sib_dst;
    const folder_info *sib_src;

    if (other.first_widget)
    {
        this->first_widget = new widget_info(*other.first_widget, TRUE);
    }

    if (copy_next)
    {
        sib_dst = this;
        sib_src = &other;

        while (sib_src->next)
        {
            sib_dst->next = new folder_info(*sib_src->next, FALSE);
            sib_dst = sib_dst->next;
            sib_src = sib_src->next;
        }
    }

}

///////////////////////////////////////////////////////////////////////////////
void display_info::SetFirstChildFolder(folder_info *folder)
{
    this->first_folder = folder;
}

///////////////////////////////////////////////////////////////////////////////
folder_info::folder_info(CString name)
{
    folder_name = name;
    output_filename.Empty();
    first_widget = NULL;
    next = NULL;
}

///////////////////////////////////////////////////////////////////////////////
folder_info::~folder_info()
{
    folder_info *temp;
    folder_info *test;
    if (first_widget)
    {
        delete first_widget;
        first_widget = NULL;
    }


    test = next;
    next = NULL;
    while (test)
    {
        temp = test->next;
        test->next = NULL;
        delete test;
        test = temp;
    }
}

///////////////////////////////////////////////////////////////////////////////
void folder_info::SetFirstChildWidget(widget_info *info)
{
    //if (first_widget)
    //{
    //    info->next = first_widget;
    //}
    first_widget = info;
}

///////////////////////////////////////////////////////////////////////////////
void folder_info::SetNextFolder(folder_info *info)
{
    next = info;
}

void theme_info::SetFirstResourceInfo(res_info *info)
{
    first_resource = info;
}