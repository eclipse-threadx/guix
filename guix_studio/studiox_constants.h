
#define COLOR_BLACK  0xff000000
#define COLOR_WHITE  0xffffffff

#define RGB_COLOR_HIGHLIGH  RGB(0, 122, 204)
#define RGB_COLOR_BLUE      RGB(0, 100, 255)
#define RGB_COLOR_WHITE     RGB(255, 255, 255)
#define RGB_COLOR_BLACK     RGB(0, 0, 0)
#define RGB_COLOR_GRAY      RGB(100, 100, 100)
#define RGB_BUTTON_FOCUS    RGB(0, 80, 219)

#define COLOR_REQUIRED_ASTERISK RGB(255, 0, 0)

#define KEY_STATE_DOWN 0x8000

#define DEFAULT_DPI_96 96
#define DEFAULT_TEXT_SCALER 100

#define MEDIUM_FONT_PT_SIZE 11
#define NORMAL_FONT_PT_SIZE 8
#define ASTERISK_FONT_PT_SIZE 13
#define VIEW_HEADER_FONT_PT_SIZE 12

#define TITLE_FONT_HEIGHT 20
#define TYNY_FONT_HEIGHT 12

#define DEFAULT_DLG_FONT_POINT_SIZE 8

enum WINDOWS_USER_MESSAGES {
    GUIX_READY = WM_USER,
    GUIX_DIRTY,
    STUDIO_TEST,
    USR_MSG_CHECK_STATE_CHANGE,
    USR_MSG_NODE_SELECT,
    USR_MSG_REBUILD_STIRNG_ITEMS,
    USR_MSG_UPDATE_STRING_TABLE_FONTS,
    USR_MSG_OPEN_RESOURCE_ITEM,
    USR_MSG_REPO_CLONE_MSG_UPDATE,
    USR_MSG_DIALOG_VISIBLE
};

enum WindowIds {
    TARGET_FRAME_ID = 8000,
    TARGET_TOOLBAR_ID
};

enum TARGET_CPU {
    CPU_GENERIC = 0,
    CPU_SYNERGY,
    CPU_RZ,
    CPU_ST_CHROMEART,
    CPU_RA,
    CPU_RX
};

enum TARGET_TOOLS {
    TOOLS_GENERIC = 0,
    TOOLS_IAR,
    TOOLS_GNU,
    TOOLS_CCRX,
    TOOLS_KEIL,
    TOOLS_ARM,
    TOOLS_MULTI,
    TOOLS_CODE_WARRIOR,
    TOOLS_MICROCHIP
};

// Define GUIX versions.

#define GX_WHITESPACE_IS_UBYTE               50500
#define GX_VERSION_STRING_LENGTH_FIX         50600
#define GX_VERSION_DUPLICATE_BINRES_DATA_FIX 50600
#define GX_VERSION_3BIT_GLYPH_DRAW_SUPPORT   50600

#define GX_VERSION_BINRES_DATA_ALIGNMENT_FIX 50601

//Not generate system resource ids to generated resource header for new version GUIX
#define GX_VERSION_RESOURCE_ID_GENERATE_FIX  60000

#define GX_VERSION_GEN_LINE_BREAK_STATUS     60001
#define GX_VERSION_BINRES_FONT_ALIGNMENT_FIX 60002

#define GX_VERSION_DISPLAY_ROTATION          60103
#define GX_VERSION_8BIT_PALETTE_DISPLAY_ROTATION 60104

#define GX_VERSION_SCREEN_ROTATION_DEFS        60105      // switch to GX_SCREEN_ROTATON_NONE, GX_SCREEN_ROTATION_FLIP in specs file
#define GX_VERSION_PIXELMAP_ROTATION_FLAGS_FIX 60105      // change pixelmap flags definitions to remove hard reference to 90/270, use CW/CCW

#define GX_VERSION_WITH_GENERIC_SCROLL_WHEEL   60107

// The 6.1.6 GUIX release is decided as official 
// ssp 2.0 release. For this release we no longer
// require synergy custom fonts when running at 
// 8bpp palette mode, just use standard system fonts.

#define GX_VERSION_SYNERGY_GLYPH_GEN_CHANGE  60106      // stop using custom synergy fonts, just use software glyph drawing

#define GX_VERSION_WITH_GENERIC_SCROLL_WHEEL 60107

#define GX_VERSION_USE_DAVE2D_PNG_DRAW       60108  // Implement custom PNG decode to produce dave2d compatible pixelmap

#define GX_VERSION_BIDI_TEXT_BASE_DIRECTION_FIX 60110

#define GX_VERSION_RADIAL_SLIDER_NEEDLE_OFFSET_CHANGE 60110

// Define GUIX Studio versions.
#define STUDIO_VERSION_USE_INTERNAL_SYSTEM_PNG_DATA 6010501  // Support using internally linked system png data.

// Define project versions.
#define PROJECT_VERSION_WRITE_COLOR_FORMAT_NAME 56 // Generate color format names to XML file instead of intege values.

#define PROJECT_VERSION_INITIAL_RESOURCE_XML 56 // The first version of resource XML file.