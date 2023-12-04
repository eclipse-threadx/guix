
#ifndef _STUDIOX_SCREEN_DRIVER_
#define _STUDIOX_SCREEN_DRIVER_

#define MAX_SNAP_LINES 6

enum SNAP_LINE_TYPES {
    SNAP_LINE_TYPE_LEFT,
    SNAP_LINE_TYPE_VERTICAL,
    SNAP_LINE_TYPE_RIGHT,
    SNAP_LINE_TYPE_TOP,
    SNAP_LINE_TYPE_HORIZONTAL,
    SNAP_LINE_TYPE_BOTTOM,
    SNAP_LINE_TYPE_COUNT
};

enum SNAP_LINE_DIRECTION {
    SNAP_LINE_DIRECTION_VERTICAL,
    SNAP_LINE_DIRECTION_HORIZONTAL
};

typedef struct {
    int direction; // Snap line direction (vertical or horizontal)
    int target_x_y; // Target snapline x/y for vertical/horizontal snapline
    int target_top_left; // Target snapline top/left for vertical/horizontal snapline
    int target_bottom_right; // Target snapline bottom/right for vertical/horizontal snapline
    int source_top_left; // Source snapline top/left for vetical/horizontal snapline
    int source_bottom_right; // Source snapline bottom/right for vertical/horizontal snapline
}SNAPLINE_INFO;

typedef struct {
    int grid_space;
    int crosshair_size;
    int crosshair_cx;
    int crosshair_cy;
    int crosshair_enabled;
    SNAPLINE_INFO snap_lines[MAX_SNAP_LINES];
    int snap_line_count;
}GRID_SNAP_INFO;

GX_WINDOW_ROOT *get_root_window();
GX_DISPLAY *get_target_view_display();
GX_CANVAS *get_target_win_canvas(void);
HWND get_target_win_handle();

VOID guix_canvas_flush(HDC win_device, int xpos, int ypos, GX_CANVAS* canvas, GX_RECTANGLE* dirty, int scale, GRID_SNAP_INFO* gsi);
VOID guix_bitmap_flush(HDC win_device, int xpos, int ypos, int width, VOID* memptr, GX_BMP_INFO* bmpinfo);

INT guix_studio_create_display(GX_DISPLAY* display, char* name,
                               int xres, int yres, int colorformat,
                               int target_cpu, int IsSynergyD2D, int IsSynergyFontFormat,
                               GX_COLOR* palette, int pal_size, int aliased_font_palsize);

INT guix_create_app_display(GX_DISPLAY* display, char* name,
                            int xres, int yres, int colorformat,
                            int target_cpu, int IsSynergyD2D, int IsSynergyFontFormat,
                            GX_COLOR* palette, int pal_size, int aliased_font_palsize);

void guix_studio_delete_display(GX_DISPLAY* display);

void initialize_guix(HWND hwnd);
BOOL guix_configure_target_win_canvas(int xres, int yres, int colorformat,
                                      int target_cpu, int IsSynergyD2D, int IsSynergyFontFormat,
                                      GX_COLOR* palette, int palsize, int font_palsize);
VOID guix_cleanup_target_win_canvas(VOID);
void guix_canvas_paint(HDC dc, GX_RECTANGLE dirty, int scale, GRID_SNAP_INFO *gsi);

#endif