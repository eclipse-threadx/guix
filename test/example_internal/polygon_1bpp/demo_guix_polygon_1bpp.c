/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include <string.h>
#include "gx_api.h"

/* Define constants for the GUIX Win32 demo.  */

/* Define constants specific to this implemenation.  */

#define DEMO_DISPLAY_WIDTH                         640
#define DEMO_DISPLAY_HEIGHT                        480

/*
#define DEMO_COLOR_FORMAT       GX_CF_16BIT_RGB
*/

#define DEFAULT_CANVAS_PIXELS     (DEMO_DISPLAY_WIDTH * DEMO_DISPLAY_HEIGHT)

/* Define the ThreadX demo thread control block and stack.  */

TX_THREAD          demo_thread;
UCHAR              demo_thread_stack[4096];

/* User-defined color ID */
#define GX_1BPP_BLACK    0x00
#define GX_1BPP_WHITE    0x01


/* User-defined color table. */
static GX_COLOR demo_color_table[] = 
{
    GX_1BPP_BLACK,
    GX_1BPP_WHITE,
};

extern GX_FONT _gx_system_font_mono;
GX_FONT *demo_font_table[] =
{
    &_gx_system_font_mono,
    &_gx_system_font_mono,
    &_gx_system_font_mono,
    &_gx_system_font_mono
};

/* Define the GUIX resources for this demo.  */
GX_CANVAS         default_canvas;

GX_DISPLAY        demo_display;
GX_WINDOW_ROOT    demo_root_window;

typedef struct {
    GX_WINDOW_MEMBERS_DECLARE
    INT line_x1;
    INT line_y1;
    INT line_x2;
    INT line_y2;
    GX_COLOR line_color;
    INT line_angle;
    GX_PROMPT angle_label;
    GX_PROMPT angle_prompt;
    GX_SLIDER angle_slider;
} LINE_WINDOW;

LINE_WINDOW   line_window;

#define LINE_WINDOW_COLOR 0x000000
#define LINE_WINDOW_LINE_LENGTH 80
#define LINE_CENTER_X_OFFSET 180
#define LINE_CENTER_Y_OFFSET 180

/* Define GUIX canvas for this demo.  */

GX_COLOR default_canvas_memory[DEFAULT_CANVAS_PIXELS];

/* Define GUIX strings and string IDs for this demo.  */

enum demo_string_ids
{
    SID_ANGLE,
    STRING_TABLE_MAX
};

GX_CHAR *demo_strings[] = {
    NULL,
    (GX_CHAR*)"Line Angle",
};

/* widget id values used by this demo */
enum demo_widget_ids {
    ID_WIDTH_SLIDER = 1, 
    ID_ANGLE_SLIDER
};

/* Define prototypes.   */

VOID  demo_thread_entry(ULONG thread_input);

UINT  line_win_event_handler(GX_WIDGET *me, GX_EVENT *myevent);
VOID  line_win_draw(GX_WIDGET *me);
VOID  calculate_line_ends();


UINT  win32_graphics_driver_setup_monochrome(GX_DISPLAY *graphic);


int main(int argc, char ** argv)
{

    tx_kernel_enter();

    return(0);
}

VOID tx_application_define(void *first_unused_memory)
{

    /* Create the main demo thread.  */
    tx_thread_create(&demo_thread, "GUIX Demo Thread", demo_thread_entry, 0,  
                     demo_thread_stack, sizeof(demo_thread_stack), 
                     1, 1, TX_NO_TIME_SLICE, TX_AUTO_START);
}

VOID  demo_thread_entry(ULONG thread_input)
{

GX_RECTANGLE    size;
GX_SLIDER_INFO  info;
GX_STRING       str;

    /* Initialize GUIX.  */
    gx_system_initialize();

    /* Create the demo screen and associated driver.  */
    gx_display_create(&demo_display, "demo display", win32_graphics_driver_setup_monochrome,
                      DEMO_DISPLAY_WIDTH, DEMO_DISPLAY_HEIGHT);

    gx_display_color_table_set(&demo_display, demo_color_table, sizeof(demo_color_table) / sizeof(GX_COLOR));

    gx_display_font_table_set(&demo_display, demo_font_table, sizeof(demo_font_table) / sizeof(GX_FONT *));

    /* Create the default canvas, painted on by the graphics driver.  */
    gx_canvas_create(&default_canvas, "demo canvas", &demo_display, 
                     GX_CANVAS_MANAGED | GX_CANVAS_VISIBLE,
                     DEMO_DISPLAY_WIDTH, DEMO_DISPLAY_HEIGHT,
                     default_canvas_memory, sizeof(default_canvas_memory));

    /* Create a background root window and attach to the background canvas.  */

    gx_utility_rectangle_define(&size, 0, 0, DEMO_DISPLAY_WIDTH - 1, DEMO_DISPLAY_HEIGHT - 1);
    gx_window_root_create(&demo_root_window, "demo root window", &default_canvas,
                          GX_STYLE_BORDER_NONE, GX_ID_NONE, &size);
    gx_widget_fill_color_set(&demo_root_window, GX_1BPP_WHITE, GX_1BPP_WHITE, GX_1BPP_WHITE);

    /* create the main window on which we will draw the lines: */
    gx_utility_rectangle_resize(&size, -20);
    gx_window_create((GX_WINDOW *) &line_window, NULL, &demo_root_window, GX_STYLE_BORDER_THICK, GX_ID_NONE, &size);
    line_window.line_angle = 0;
    line_window.line_color = GX_1BPP_BLACK;

    gx_widget_fill_color_set(&line_window, GX_1BPP_WHITE, GX_1BPP_WHITE, GX_1BPP_WHITE);
    calculate_line_ends();

    gx_widget_draw_set(&line_window, line_win_draw);
    gx_widget_event_process_set(&line_window, line_win_event_handler);

    /* add angle adjustment children */
    gx_utility_rectangle_define(&size, 360, 80, 460, 100);
    gx_prompt_create(&line_window.angle_label, NULL, &line_window, SID_ANGLE, GX_STYLE_NONE, GX_ID_NONE, &size);
    
    gx_utility_rectangle_shift(&size, 100, 0);
    memset(&info, 0, sizeof(GX_SLIDER_INFO));

    info.gx_slider_info_current_val = 0;
    info.gx_slider_info_min_val = -360;
    info.gx_slider_info_max_val = 360;
    info.gx_slider_info_increment = 1;
    info.gx_slider_info_needle_inset = 4;
    info.gx_slider_info_needle_height = 12;
    info.gx_slider_info_min_travel = 10;
    info.gx_slider_info_max_travel = 10;

    gx_slider_create(&line_window.angle_slider, NULL, &line_window,
                     10, &info, GX_STYLE_ENABLED|GX_STYLE_SHOW_TICKMARKS|GX_STYLE_SHOW_NEEDLE|GX_STYLE_BORDER_RAISED,
                     ID_ANGLE_SLIDER, &size);
    line_window.angle_slider.gx_widget_normal_fill_color = GX_1BPP_WHITE;
    line_window.angle_slider.gx_widget_selected_fill_color = GX_1BPP_WHITE;
    line_window.angle_slider.gx_slider_tick_color = GX_1BPP_BLACK;

    gx_utility_rectangle_define(&size, 570, 80, 610, 100);
    gx_prompt_create(&line_window.angle_prompt, NULL, &line_window, 0,
                     GX_STYLE_BORDER_THIN, GX_ID_NONE, &size);    
    str.gx_string_ptr = "0";
    str.gx_string_length = 1;
    gx_prompt_text_set_ext(&line_window.angle_prompt, &str);

    /* Add a slider for line width */
    /* Show the root window.  */
    gx_widget_show(&demo_root_window);

#if defined(GUIX_VALIDATION)
    /* do_validation(); */
#endif

    /* let GUIX run */
    gx_system_start();
}

VOID calculate_line_ends(VOID)
{
    INT scaled_angle;
    INT x_dist;
    INT y_dist;

    INT x_center = line_window.gx_widget_size.gx_rectangle_left + LINE_CENTER_X_OFFSET;
    INT y_center = line_window.gx_widget_size.gx_rectangle_top + LINE_CENTER_Y_OFFSET;

    scaled_angle = GX_FIXED_VAL_MAKE(line_window.line_angle);
    x_dist = GX_FIXED_VAL_TO_INT(gx_utility_math_cos(scaled_angle) * LINE_WINDOW_LINE_LENGTH);
    y_dist = GX_FIXED_VAL_TO_INT(gx_utility_math_sin(scaled_angle) * LINE_WINDOW_LINE_LENGTH);

    line_window.line_x1 = x_center - x_dist;
    line_window.line_x2 = x_center + x_dist;
    line_window.line_y1 = y_center - y_dist;
    line_window.line_y2 = y_center + y_dist;
}

VOID update_angle_prompt(VOID)
{
    static GX_CHAR angle_val[10];
    GX_STRING str;

    gx_utility_ltoa(line_window.line_angle, angle_val, 10);
    str.gx_string_ptr = angle_val;
    str.gx_string_length = strnlen(angle_val, sizeof(angle_val) - 1);
    gx_prompt_text_set_ext(&line_window.angle_prompt, &str); 
}

UINT line_win_event_handler(GX_WIDGET *widget, GX_EVENT *myevent)
{
UINT status = 0;

    switch(myevent -> gx_event_type)
    {
    case GX_SIGNAL(ID_ANGLE_SLIDER, GX_EVENT_SLIDER_VALUE):
        line_window.line_angle = myevent ->gx_event_payload.gx_event_longdata;
        update_angle_prompt();
        calculate_line_ends();
        /* kgm FIXME- should really just mark the line area as dirty */
        gx_system_dirty_mark(widget);
        break;

    default:
        status = gx_window_event_process((GX_WINDOW*)widget, myevent);
        break;
    }
    return status;
}

#define NUM_VERTICES 3
VOID line_win_draw(GX_WIDGET *widget)
{

GX_POINT vertex[NUM_VERTICES];

    LINE_WINDOW *win = (LINE_WINDOW *) widget;
    ULONG brush_style = 0;

    gx_window_draw((GX_WINDOW*)widget);

    gx_context_brush_define(GX_1BPP_BLACK, GX_1BPP_BLACK, brush_style | GX_BRUSH_ROUND | GX_BRUSH_SOLID_FILL);
    gx_context_brush_width_set(10);

    vertex[0].gx_point_x = win -> line_x1;
    vertex[0].gx_point_y = 300;

    vertex[1].gx_point_x = win -> line_x1;
    vertex[1].gx_point_y = win -> line_y1;

    vertex[2].gx_point_x = win -> line_x2;
    vertex[2].gx_point_y = win -> line_y2;

    gx_canvas_polygon_draw(vertex, NUM_VERTICES);

}

