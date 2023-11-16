/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include <string.h>
#include "gx_api.h"

/* Define constants for the GUIX Win32 demo.  */

/* Define constants specific to this implemenation.  */

#define DEMO_DISPLAY_WIDTH                         640
#define DEMO_DISPLAY_HEIGHT                        480

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

extern GX_PIXELMAP PRIMARY_CHECKBOX_ON_pixelmap;
extern GX_PIXELMAP PRIMARY_CHECKBOX_OFF_pixelmap;

GX_PIXELMAP *demo_pixelmap_table[] = {
    NULL,
    NULL,
    NULL,
    &PRIMARY_CHECKBOX_ON_pixelmap,
    &PRIMARY_CHECKBOX_OFF_pixelmap,
};

#define PRIMARY_PIXELMAP_TABLE_SIZE GX_DEFAULT_PIXELMAP_COUNT

#define DEFAULT_CANVAS_PIXELS     (DEMO_DISPLAY_WIDTH * DEMO_DISPLAY_HEIGHT)

/* Define the ThreadX demo thread control block and stack.  */

TX_THREAD          demo_thread;

/* Define the stack for the demo thread. */
ULONG              demo_thread_stack[4096 / sizeof(ULONG)];

/* Define the GUIX resources for this demo.  */
GX_CANVAS         default_canvas;

GX_DISPLAY        demo_display;
GX_WINDOW_ROOT    demo_root_window;



GX_WINDOW   line_window;
INT         line_x1;
INT         line_y1;
INT         line_x2;
INT         line_y2;
INT         line_width;
INT         line_pattern;
GX_COLOR    line_color;
INT         line_angle;
GX_BOOL     is_round;
GX_PROMPT   width_label;
GX_PROMPT   angle_label;
GX_PROMPT   width_prompt;
GX_PROMPT   angle_prompt;
GX_SLIDER   width_slider;
GX_SLIDER   angle_slider;
GX_CHECKBOX round_box;
GX_CHECKBOX pattern_box;

#define LINE_WINDOW_COLOR 0x000000
#define LINE_WINDOW_LINE_LENGTH 80
#define LINE_CENTER_X_OFFSET 180
#define LINE_CENTER_Y_OFFSET 180

/* Define GUIX canvas for this demo.  */

ULONG default_canvas_memory[DEFAULT_CANVAS_PIXELS/sizeof(ULONG)];

/* Define GUIX strings and string IDs for this demo.  */

enum demo_string_ids
{
    SID_WIDTH = 1,
    SID_ANGLE,
    SID_ROUND,
    SID_PATTERN,
    STRING_TABLE_MAX
};

GX_CONST GX_CHAR string_Line_Width[] = "Line Width";
GX_CONST GX_CHAR string_Line_Angle[] = "Line Angle";
GX_CONST GX_CHAR string_Round_End[] = "Round End";
GX_CONST GX_CHAR string_Pattern_line[] = "Pattern line";

GX_CONST GX_STRING demo_strings[] = {
    {NULL ,0},
    {string_Line_Width, sizeof(string_Line_Width) - 1},
    {string_Line_Angle, sizeof(string_Line_Angle) - 1},
    {string_Round_End, sizeof(string_Round_End) - 1},
    {string_Pattern_line, sizeof(string_Pattern_line) - 1},
};

GX_CONST GX_STRING *language_table[1] = 
{
    demo_strings
};

/* widget id values used by this demo */
enum demo_widget_ids {
    ID_WIDTH_SLIDER = 1, 
    ID_ANGLE_SLIDER,
    BID_ROUND,
    BID_PATTERN
};

/* Define prototypes.   */

VOID  demo_thread_entry(ULONG thread_input);

UINT  line_win_event_handler(GX_WIDGET *me, GX_EVENT *myevent);
VOID  line_win_draw(GX_WIDGET *me);
VOID  calculate_line_ends();
extern UINT win32_graphics_driver_setup_monochrome(GX_DISPLAY *display);

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

    /* Create the demo display and associated driver.  */
    gx_display_create(&demo_display, "demo display", win32_graphics_driver_setup_monochrome,
                      DEMO_DISPLAY_WIDTH, DEMO_DISPLAY_HEIGHT);

    /* Install the demo string table.  */
    gx_display_language_table_set_ext(&demo_display, language_table, 1, STRING_TABLE_MAX);
    gx_system_active_language_set(0);

    gx_display_color_table_set(&demo_display, demo_color_table, sizeof(demo_color_table) / sizeof(GX_COLOR));

    gx_display_font_table_set(&demo_display, demo_font_table, sizeof(demo_font_table) / sizeof(GX_FONT *));

    /* Set pixelmap table.  */
    gx_display_pixelmap_table_set(&demo_display, demo_pixelmap_table, PRIMARY_PIXELMAP_TABLE_SIZE);

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
    gx_utility_rectangle_define(&size, 20, 20, 400, 460);
    gx_window_create(&line_window, NULL, &demo_root_window, GX_STYLE_BORDER_THICK, GX_ID_NONE, &size);
    is_round = GX_FALSE;
    line_angle = 0;
    line_width = 1;
    line_color = GX_1BPP_BLACK;
    line_window.gx_widget_normal_fill_color = GX_1BPP_WHITE; 
    line_window.gx_widget_selected_fill_color = GX_1BPP_WHITE;
    line_window.gx_widget_disabled_fill_color = GX_1BPP_WHITE;
    calculate_line_ends();

    gx_widget_draw_set(&line_window, line_win_draw);
    gx_widget_event_process_set(&demo_root_window, line_win_event_handler);

    /* add width adjustment children */
    gx_utility_rectangle_define(&size, 460, 40, 560, 60);
    memset(&info, 0, sizeof(GX_SLIDER_INFO));

    info.gx_slider_info_current_val = 1;
    info.gx_slider_info_min_val = 1;
    info.gx_slider_info_max_val = 10;
    info.gx_slider_info_increment = 1;
    info.gx_slider_info_needle_inset = 4;
    info.gx_slider_info_needle_height = 12;
    info.gx_slider_info_min_travel = 10;
    info.gx_slider_info_max_travel = 10;

    gx_slider_create(&width_slider, NULL, &demo_root_window,
                     10, &info, GX_STYLE_ENABLED|GX_STYLE_SHOW_TICKMARKS|GX_STYLE_SHOW_NEEDLE|GX_STYLE_BORDER_RAISED,
                     ID_WIDTH_SLIDER, &size);
    width_slider.gx_widget_normal_fill_color = GX_1BPP_WHITE;
    width_slider.gx_widget_selected_fill_color = GX_1BPP_WHITE;
    width_slider.gx_slider_tick_color = GX_1BPP_BLACK;

    gx_utility_rectangle_define(&size, 580, 40, 620, 60);
    gx_prompt_create(&width_prompt, NULL, &demo_root_window, 0,
        GX_STYLE_BORDER_THIN, GX_ID_NONE, &size);
    width_prompt.gx_widget_status = GX_STATUS_VISIBLE;
    str.gx_string_ptr = "1";
    str.gx_string_length = 1;
    gx_prompt_text_set_ext(&width_prompt, &str);
    gx_prompt_text_color_set(&width_prompt, GX_1BPP_BLACK, GX_1BPP_BLACK, GX_1BPP_BLACK);
    gx_widget_fill_color_set(&width_prompt, GX_1BPP_WHITE, GX_1BPP_WHITE, GX_1BPP_WHITE);

    /* add angle adjustment children */
    gx_utility_rectangle_define(&size, 460, 80, 560, 100);
    memset(&info, 0, sizeof(GX_SLIDER_INFO));

    info.gx_slider_info_current_val = 0;
    info.gx_slider_info_min_val = -360;
    info.gx_slider_info_max_val = 360;
    info.gx_slider_info_increment = 1;
    info.gx_slider_info_needle_inset = 4;
    info.gx_slider_info_needle_height = 12;
    info.gx_slider_info_min_travel = 10;
    info.gx_slider_info_max_travel = 10;

    gx_slider_create(&angle_slider, NULL, &demo_root_window,
                     10, &info, GX_STYLE_ENABLED|GX_STYLE_SHOW_TICKMARKS|GX_STYLE_SHOW_NEEDLE|GX_STYLE_BORDER_RAISED,
                     ID_ANGLE_SLIDER, &size);
    angle_slider.gx_widget_normal_fill_color = GX_1BPP_WHITE;
    angle_slider.gx_widget_selected_fill_color = GX_1BPP_WHITE;
    angle_slider.gx_slider_tick_color = GX_1BPP_BLACK;

    gx_utility_rectangle_define(&size, 580, 80, 620, 100);
    gx_prompt_create(&angle_prompt, NULL, &demo_root_window, 0,
                     GX_STYLE_BORDER_THIN, GX_ID_NONE, &size);    
    str.gx_string_ptr = "0";
    str.gx_string_length = 1;
    gx_prompt_text_set_ext(&angle_prompt, &str);
    gx_prompt_text_color_set(&angle_prompt, GX_1BPP_BLACK, GX_1BPP_BLACK, GX_1BPP_BLACK);
    gx_widget_fill_color_set(&angle_prompt, GX_1BPP_WHITE, GX_1BPP_WHITE, GX_1BPP_WHITE);

    /* add round-end checkbox */
    gx_utility_rectangle_define(&size, 460, 160, 600, 180);
    gx_checkbox_create(&round_box, "round_box", &demo_root_window, SID_ROUND,
                       GX_STYLE_TRANSPARENT|GX_STYLE_TEXT_LEFT| GX_STYLE_ENABLED, BID_ROUND, &size);
    gx_checkbox_pixelmap_set(&round_box, GX_PIXELMAP_ID_CHECKBOX_OFF, GX_PIXELMAP_ID_CHECKBOX_ON, GX_PIXELMAP_ID_CHECKBOX_OFF, GX_PIXELMAP_ID_CHECKBOX_ON);
   

    /* add round-end checkbox */
    gx_utility_rectangle_define(&size, 460, 200, 600, 220);
    gx_checkbox_create(&pattern_box, "pattern_box", &demo_root_window, SID_PATTERN,
        GX_STYLE_TRANSPARENT | GX_STYLE_TEXT_LEFT | GX_STYLE_ENABLED, BID_PATTERN, &size);
    gx_checkbox_pixelmap_set(&pattern_box, GX_PIXELMAP_ID_CHECKBOX_OFF, GX_PIXELMAP_ID_CHECKBOX_ON, GX_PIXELMAP_ID_CHECKBOX_OFF, GX_PIXELMAP_ID_CHECKBOX_ON);


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

    scaled_angle = GX_FIXED_VAL_MAKE(line_angle);
    x_dist = GX_FIXED_VAL_TO_INT(gx_utility_math_cos(scaled_angle) * LINE_WINDOW_LINE_LENGTH);
    y_dist = GX_FIXED_VAL_TO_INT(gx_utility_math_sin(scaled_angle) * LINE_WINDOW_LINE_LENGTH);

    line_x1 = x_center - x_dist;
    line_x2 = x_center + x_dist;
    line_y1 = y_center - y_dist;
    line_y2 = y_center + y_dist;
}

VOID update_width_prompt(VOID)
{
    static CHAR width_val[10];
    GX_STRING str;

    gx_utility_ltoa(line_width, width_val, 10);
    str.gx_string_ptr = width_val;
    str.gx_string_length = strnlen(width_val, sizeof(width_val));
    gx_prompt_text_set_ext(&width_prompt, &str); 
}

VOID update_angle_prompt(VOID)
{
    static CHAR angle_val[10];
    GX_STRING str;

    gx_utility_ltoa(line_angle, angle_val, 10);
    str.gx_string_ptr = angle_val;
    str.gx_string_length = strnlen(angle_val, sizeof(angle_val));
    gx_prompt_text_set_ext(&angle_prompt, &str); 
}

UINT line_win_event_handler(GX_WIDGET *widget, GX_EVENT *myevent)
{
UINT status = 0;
GX_WINDOW *window = (GX_WINDOW*)widget;

    switch(myevent -> gx_event_type)
    {
    case GX_SIGNAL(ID_WIDTH_SLIDER, GX_EVENT_SLIDER_VALUE):
        line_width = myevent ->gx_event_payload.gx_event_longdata;
        update_width_prompt();

        /* kgm FIXME- should really just mark the line area as dirty */
        gx_system_dirty_mark(widget);
        break;

    case GX_SIGNAL(ID_ANGLE_SLIDER, GX_EVENT_SLIDER_VALUE):
        line_angle = myevent ->gx_event_payload.gx_event_longdata;
        update_angle_prompt();
        calculate_line_ends();
        /* kgm FIXME- should really just mark the line area as dirty */
        gx_system_dirty_mark(widget);
        break;

    case GX_SIGNAL(BID_ROUND, GX_EVENT_TOGGLE_ON):
        is_round = GX_TRUE;
        gx_system_dirty_mark(widget);
        break;

    case GX_SIGNAL(BID_ROUND, GX_EVENT_TOGGLE_OFF):
        is_round = GX_FALSE;
        gx_system_dirty_mark(widget);
        break;

    case GX_SIGNAL(BID_PATTERN, GX_EVENT_TOGGLE_ON):
        line_pattern = 0x0f0f0f0f;
        gx_system_dirty_mark(widget);
        break;

    case GX_SIGNAL(BID_PATTERN, GX_EVENT_TOGGLE_OFF):
        line_pattern = 0;
        gx_system_dirty_mark(widget);
        break;

    default:
        status = gx_window_event_process(window, myevent);
        break;
    }
    return status;
}



VOID line_win_draw(GX_WIDGET *widget)
{
ULONG brush_style = 0;
GX_BRUSH *brush = GX_NULL;

    gx_window_draw((GX_WINDOW*)widget);

    if (is_round)
    {
        brush_style |= GX_BRUSH_ROUND;
    }

    gx_context_brush_width_set(line_width);
    gx_context_raw_brush_define(0x0, 0xffffff, brush_style);

    gx_context_brush_get(&brush);

    if (brush)
    {
        brush -> gx_brush_line_pattern = line_pattern;
        gx_context_brush_set(brush);

        /* calculate the line end points */
        gx_canvas_line_draw(line_x1, line_y1, line_x2, line_y2);
    }

}

