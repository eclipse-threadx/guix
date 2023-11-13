/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
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
    INT line_width;
    GX_COLOR line_color;
    INT line_angle;
    GX_PROMPT width_label;
    GX_PROMPT angle_label;
    GX_PROMPT width_prompt;
    GX_PROMPT angle_prompt;
    GX_SLIDER width_slider;
    GX_SLIDER angle_slider;
    GX_CHECKBOX aliased_box;
    GX_CHECKBOX round_box;
    GX_BOOL  is_round;
    GX_BOOL  is_aliased;
} LINE_WINDOW;

LINE_WINDOW   line_window;

#define LINE_WINDOW_COLOR 0x000000
#define LINE_WINDOW_LINE_LENGTH 80
#define LINE_CENTER_X_OFFSET 180
#define LINE_CENTER_Y_OFFSET 180

/* Define GUIX canvas for this demo.  */

GX_COLOR default_canvas_memory[DEFAULT_CANVAS_PIXELS];

/* Define color table for this demo. */
#define GX_COLOR_ID_BLACK     GX_FIRST_USER_COLOR
#define GX_COLOR_ID_BLUE      (GX_FIRST_USER_COLOR + 1)
#define GX_COLOR_ID_WHITE     (GX_FIRST_USER_COLOR + 2)

/* Define the content of the color table. */
static GX_COLOR demo_color_table[] = 
{
    GX_SYSTEM_DEFAULT_COLORS_DECLARE,
    GX_COLOR_BLACK,
    GX_COLOR_BLUE,
    GX_COLOR_WHITE
};



/* Define GUIX strings and string IDs for this demo.  */

enum demo_string_ids
{
    SID_WIDTH = 1,
    SID_ANGLE,
    SID_ALIASED,
    SID_ROUND,
    STRING_TABLE_MAX
};

char *demo_strings[] = {
    NULL,
    "Line Width",
    "Line Angle",
    "Anti Aliased",
    "Round End"
};

enum demo_pixelmaps {
    
    BID_WALLPAPER = GX_FIRST_USER_PIXELMAP,
    NUMBER_OF_PIXELMAPS
};

/* widget id values used by this demo */
enum demo_widget_ids {
    ID_WIDTH_SLIDER = 1, 
    ID_ANGLE_SLIDER,
    BID_ALIASED,
    BID_ROUND
};

/* Define prototypes.   */

VOID  demo_thread_entry(ULONG thread_input);

UINT  line_win_event_handler(GX_WIDGET *me, GX_EVENT *myevent);
VOID  line_win_draw(GX_WIDGET *me);
VOID  calculate_line_ends();


#ifdef WIN32
UINT  win32_graphics_driver_setup_16bpp(GX_DISPLAY *graphic);

int WINAPI WinMain(HINSTANCE Instance, HINSTANCE PrevInstance,
    LPSTR pCmd, int CmdShow)
{
    /* Enter the ThreadX kernel.  */
    tx_kernel_enter();
}
#else
int main(int argc, char ** argv)
{

    tx_kernel_enter();

    return (0);
}

#endif

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

    /* Initialize GUIX.  */
    gx_system_initialize();

    /* Install the demo string table.  */
    gx_system_string_table_set(demo_strings, STRING_TABLE_MAX);

    /* Install the demo bitmap table. */
    /* gx_system_pixelmap_table_set(demo_pixelmap_table, NUMBER_OF_PIXELMAPS); */

    gx_system_color_table_set(demo_color_table, sizeof(demo_color_table) / sizeof(GX_COLOR));


#ifdef WIN32
    /* Create the demo display and associated driver.  */
    gx_display_create(&demo_display, "demo display", win32_graphics_driver_setup_16bpp,
                      DEMO_DISPLAY_WIDTH, DEMO_DISPLAY_HEIGHT);
#endif

    /* Create the default canvas, painted on by the graphics driver.  */
    gx_canvas_create(&default_canvas, "demo canvas", &demo_display, 
                     GX_CANVAS_MANAGED | GX_CANVAS_VISIBLE,
                     DEMO_DISPLAY_WIDTH, DEMO_DISPLAY_HEIGHT,
                     default_canvas_memory, sizeof(default_canvas_memory));

    /* Create a background root window and attach to the background canvas.  */

    gx_utility_rectangle_define(&size, 0, 0, DEMO_DISPLAY_WIDTH - 1, DEMO_DISPLAY_HEIGHT - 1);
    gx_window_root_create(&demo_root_window, "demo root window", &default_canvas,
                          GX_STYLE_BORDER_NONE, GX_ID_NONE, &size);
    gx_widget_fill_color_set(&demo_root_window, GX_COLOR_ID_BLACK, GX_COLOR_ID_BLACK);

    /* create the main window on which we will draw the lines: */
    gx_utility_rectangle_resize(&size, -20);
    gx_window_create((GX_WINDOW *) &line_window, NULL, &demo_root_window, GX_STYLE_BORDER_THICK, GX_ID_NONE, &size);
    line_window.is_aliased = GX_FALSE;
    line_window.is_round = GX_FALSE;
    line_window.line_angle = 0;
    line_window.line_width = 1;
    line_window.line_color = GX_COLOR_ID_BLUE;
    line_window.gx_widget_normal_fill_color = GX_COLOR_ID_WHITE;
    calculate_line_ends();

    gx_widget_draw_set(&line_window, line_win_draw);
    gx_widget_event_process_set(&line_window, line_win_event_handler);

    /* add width adjustment children */
    gx_utility_rectangle_define(&size, 360, 40, 460, 60);
    gx_prompt_create(&line_window.width_label, NULL, &line_window, SID_WIDTH, GX_STYLE_NONE, GX_ID_NONE, &size);    
    
    gx_utility_rectangle_shift(&size, 100, 0);
    memset(&info, 0, sizeof(GX_SLIDER_INFO));

    info.gx_slider_info_current_val = 1;
    info.gx_slider_info_min_val = 1;
    info.gx_slider_info_max_val = 10;
    info.gx_slider_info_increment = 1;
    info.gx_slider_info_needle_inset = 4;
    info.gx_slider_info_needle_height = 12;
    info.gx_slider_info_min_travel = 10;
    info.gx_slider_info_max_travel = 10;

    gx_slider_create(&line_window.width_slider, NULL, &line_window,
        10, &info, GX_STYLE_SHOW_TICKMARKS|GX_STYLE_SHOW_NEEDLE|GX_STYLE_BORDER_RAISED,
        ID_WIDTH_SLIDER, &size);

    gx_utility_rectangle_define(&size, 580, 40, 610, 60);
    gx_prompt_create(&line_window.width_prompt, NULL, &line_window, 0,
        GX_STYLE_BORDER_THIN, GX_ID_NONE, &size);    
    gx_prompt_text_set(&line_window.width_prompt, "1");

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
        10, &info, GX_STYLE_SHOW_TICKMARKS|GX_STYLE_SHOW_NEEDLE|GX_STYLE_BORDER_RAISED,
        ID_ANGLE_SLIDER, &size);

    gx_utility_rectangle_define(&size, 570, 80, 610, 100);
    gx_prompt_create(&line_window.angle_prompt, NULL, &line_window, 0,
        GX_STYLE_BORDER_THIN, GX_ID_NONE, &size);    
    gx_prompt_text_set(&line_window.angle_prompt, "0");

    /* add anti-aliased checkbox */
    gx_utility_rectangle_define(&size, 370, 120, 500, 140);
    gx_checkbox_create(&line_window.aliased_box, NULL, &line_window, SID_ALIASED,
        GX_STYLE_TEXT_LEFT, BID_ALIASED, &size);    

    /* add round-end checkbox */
    gx_utility_rectangle_define(&size, 370, 160, 500, 180);
    gx_checkbox_create(&line_window.round_box, NULL, &line_window, SID_ROUND,
        GX_STYLE_TEXT_LEFT, BID_ROUND, &size);    

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

VOID update_width_prompt(VOID)
{
    static CHAR width_val[10];
    gx_utility_ltoa(line_window.line_width, width_val, 10);
    gx_prompt_text_set(&line_window.width_prompt, width_val); 
}

VOID update_angle_prompt(VOID)
{
    static CHAR angle_val[10];
    gx_utility_ltoa(line_window.line_angle, angle_val, 10);
    gx_prompt_text_set(&line_window.angle_prompt, angle_val); 
}

UINT line_win_event_handler(GX_WIDGET *widget, GX_EVENT *myevent)
{
UINT status = 0;
GX_WINDOW *window = (GX_WINDOW*)widget;

    switch(myevent -> gx_event_type)
    {
    case GX_SIGNAL(ID_WIDTH_SLIDER, GX_EVENT_SLIDER_VALUE):
        line_window.line_width = myevent ->gx_event_payload.gx_event_longdata;
        update_width_prompt();

        /* kgm FIXME- should really just mark the line area as dirty */
        gx_system_dirty_mark(widget);
        break;

    case GX_SIGNAL(ID_ANGLE_SLIDER, GX_EVENT_SLIDER_VALUE):
        line_window.line_angle = myevent ->gx_event_payload.gx_event_longdata;
        update_angle_prompt();
        calculate_line_ends();
        /* kgm FIXME- should really just mark the line area as dirty */
        gx_system_dirty_mark(widget);
        break;

    case GX_SIGNAL(BID_ALIASED, GX_EVENT_TOGGLE_ON):
        line_window.is_aliased = GX_TRUE;
        gx_system_dirty_mark(widget);
        break;

    case GX_SIGNAL(BID_ALIASED, GX_EVENT_TOGGLE_OFF):
        line_window.is_aliased = GX_FALSE;
        gx_system_dirty_mark(widget);
        break;

    case GX_SIGNAL(BID_ROUND, GX_EVENT_TOGGLE_ON):
        line_window.is_round = GX_TRUE;
        gx_system_dirty_mark(widget);
        break;

    case GX_SIGNAL(BID_ROUND, GX_EVENT_TOGGLE_OFF):
        line_window.is_round = GX_FALSE;
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
LINE_WINDOW *win = (LINE_WINDOW *) widget;
ULONG brush_style = 0;

    gx_window_draw((GX_WINDOW*)widget);

    if (win -> is_aliased)
    {
        brush_style |= GX_BRUSH_ALIAS;
    }
    if (win -> is_round)
    {
        brush_style |= GX_BRUSH_ROUND;
    }
    gx_context_brush_define(win -> line_color, win ->gx_widget_normal_fill_color, brush_style);
    gx_context_brush_width_set(win -> line_width);

    /* calculate the line end points */
    gx_canvas_line_draw(win ->line_x1, win ->line_y1, win ->line_x2, win ->line_y2);
}

