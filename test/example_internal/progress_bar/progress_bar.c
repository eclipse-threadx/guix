/***************************************************************************
 * Copyright (c) 2024 Microsoft Corporation 
 * 
 * This program and the accompanying materials are made available under the
 * terms of the MIT License which is available at
 * https://opensource.org/licenses/MIT.
 * 
 * SPDX-License-Identifier: MIT
 **************************************************************************/



/* Include necessary system files. */
#include "gx_api.h"

/* Define constants for the GUIX Win32 demo.  */

/* Define the display dimentions specific to this implemenation.  */
#define DEMO_DISPLAY_WIDTH                         640
#define DEMO_DISPLAY_HEIGHT                        480

/* Define the number of pixels on the canvas */
#define DEFAULT_CANVAS_PIXELS     (DEMO_DISPLAY_WIDTH * DEMO_DISPLAY_HEIGHT)
#define ID_PROGRESS_BAR_TIMER     1

/* Define the GUIX resources for this demo.  */

/* GUIX display represents the physical display device */ 
GX_DISPLAY        demo_display;

/* GUIX canvas is the frame buffer on top of GUIX displayl. */
GX_CANVAS         default_canvas;

/* The root window is a special GUIX background window, right on top of the canvas. */ 
GX_WINDOW_ROOT    demo_root_window;
GX_WINDOW         border_style_window;
GX_WINDOW         fill_style_window;

/* GUIX Progress Bar. */
GX_PROGRESS_BAR   progress_bar;

/* GUIX Progress Bar information. */
GX_PROGRESS_BAR_INFO info;

GX_RADIO_BUTTON   radio_button_1;
GX_RADIO_BUTTON   radio_button_2;
GX_RADIO_BUTTON   radio_button_3;
GX_RADIO_BUTTON   radio_button_4;
GX_RADIO_BUTTON   radio_button_5;
GX_RADIO_BUTTON   radio_button_6;
GX_RADIO_BUTTON   radio_button_7;
GX_CHECKBOX       checkbox;

GX_PROMPT         prompt_1;
GX_PROMPT         prompt_2;
GX_PROMPT         prompt_3;

/* Memory for the frame buffer. */
GX_COLOR default_canvas_memory[DEFAULT_CANVAS_PIXELS];

/* User-defined color ID */
#define GX_COLOR_ID_BLACK     GX_FIRST_USER_COLOR
#define GX_COLOR_ID_WHITE     (GX_FIRST_USER_COLOR + 1)
#define GX_COLOR_ID_GREEN     (GX_FIRST_USER_COLOR + 2)
#define GX_COLOR_ID_YELLOW    (GX_FIRST_USER_COLOR + 3)

/* Define the default theme colors.   */

#define GX_COLOR_CANVAS                     GX_COLOR_BLACK
#define GX_COLOR_WIDGET_FILL                0x00787c78
#define GX_COLOR_WINDOW_FILL                0x00e2e2e2
#define GX_COLOR_STANDARD_BORDER            0x009b9b73
#define GX_COLOR_WINDOW_BORDER              0x007599aa
#define GX_COLOR_NORMAL_TEXT                GX_COLOR_BLACK
#define GX_COLOR_SELECTED_TEXT              GX_COLOR_WHITE
#define GX_COLOR_SELECTED_FILL              GX_COLOR_BLUE

#define GX_COLOR_SHADOW                     GX_COLOR_DARKGRAY
#define GX_COLOR_SHINE                      0x00dadada

#define GX_COLOR_BUTTON_BORDER              0x00e0c060
#define GX_COLOR_BUTTON_UPPER               0x00f8f8e0
#define GX_COLOR_BUTTON_LOWER               0x00f8ecb0
#define GX_COLOR_BUTTON_TEXT                GX_COLOR_BLACK

#define GX_COLOR_SCROLL_FILL                0x00e8d8f8
#define GX_COLOR_SCROLL_BUTTON              0x00e8ccb0
#define GX_COLOR_TEXT_INPUT_TEXT            GX_COLOR_BLACK
#define GX_COLOR_TEXT_INPUT_FILL            GX_COLOR_WHITE

#define GX_COLOR_SLIDER_TICK                GX_COLOR_BLACK
#define GX_COLOR_SLIDER_GROOVE_TOP          GX_COLOR_LIGHTGRAY
#define GX_COLOR_SLIDER_GROOVE_BOTTOM       GX_COLOR_WHITE
#define GX_COLOR_SLIDER_NEEDLE_OUTLINE      GX_COLOR_BLACK
#define GX_COLOR_SLIDER_NEEDLE_FILL         GX_COLOR_DARKGRAY
#define GX_COLOR_SLIDER_NEEDLE_LINE1        GX_COLOR_LIGHTGRAY
#define GX_COLOR_SLIDER_NEEDLE_LINE2        GX_COLOR_BUTTON_BORDER

#define GX_SYSTEM_DEFAULT_COLORS_DECLARE \
    GX_COLOR_CANVAS,                     \
    GX_COLOR_WIDGET_FILL,                \
    GX_COLOR_WINDOW_FILL,                \
    GX_COLOR_STANDARD_BORDER,            \
    GX_COLOR_WINDOW_BORDER,              \
    GX_COLOR_NORMAL_TEXT,                \
    GX_COLOR_SELECTED_TEXT,              \
    GX_COLOR_SELECTED_FILL,              \
    GX_COLOR_SHADOW,                     \
    GX_COLOR_SHINE,                      \
    GX_COLOR_BUTTON_BORDER,              \
    GX_COLOR_BUTTON_UPPER,               \
    GX_COLOR_BUTTON_LOWER,               \
    GX_COLOR_BUTTON_TEXT,                \
    GX_COLOR_SCROLL_FILL,                \
    GX_COLOR_SCROLL_BUTTON,              \
    GX_COLOR_TEXT_INPUT_TEXT,            \
    GX_COLOR_TEXT_INPUT_FILL,            \
    GX_COLOR_SLIDER_TICK,                \
    GX_COLOR_SLIDER_GROOVE_TOP,          \
    GX_COLOR_SLIDER_GROOVE_BOTTOM,       \
    GX_COLOR_SLIDER_NEEDLE_OUTLINE,      \
    GX_COLOR_SLIDER_NEEDLE_FILL,         \
    GX_COLOR_SLIDER_NEEDLE_LINE1,        \
    GX_COLOR_SLIDER_NEEDLE_LINE2

/* User-defined color table. */
static GX_COLOR demo_color_table[] = 
{
    /* First, bring in GUIX default color table.  These colors are used
       by GUIX internals. */
    GX_SYSTEM_DEFAULT_COLORS_DECLARE,

    /* In this demo, two color entries are added to the color table. */
    GX_COLOR_BLACK,
    GX_COLOR_WHITE,
    GX_COLOR_GREEN,
    GX_COLOR_YELLOW
};

/* Font Table                                                                  */


extern GX_FONT _gx_system_font_8bpp;
GX_FONT *demo_font_table[] =
{
    &_gx_system_font_8bpp,
    &_gx_system_font_8bpp,
    &_gx_system_font_8bpp,
    &_gx_system_font_8bpp
};

extern GX_PIXELMAP RADIO_ON_pixelmap;
extern GX_PIXELMAP RADIO_OFF_pixelmap;
extern GX_PIXELMAP CHECKBOX_ON_pixelmap;
extern GX_PIXELMAP CHECKBOX_OFF_pixelmap;
extern GX_PIXELMAP TFIELD_FILL_SMALL_pixelmap;
extern GX_PIXELMAP VERTICAL_FILL_pixelmap;

GX_PIXELMAP *demo_pixelmap_table[] = {
    NULL,
    &RADIO_ON_pixelmap,
    &RADIO_OFF_pixelmap,
    &CHECKBOX_ON_pixelmap,
    &CHECKBOX_OFF_pixelmap,
    &TFIELD_FILL_SMALL_pixelmap,
    &VERTICAL_FILL_pixelmap
};

enum demo_pixelmaps {
    BID_RADIO_ON = 1,
    BID_RADIO_OFF,
    BID_CHECKBOX_ON,
    BID_CHECKBOX_OFF,
    BID_PROMPT_FILL,
    BID_VERTICAL_FILL,
    NUMBER_OF_PIXELMAPS
};

/* Define GUIX strings and string IDs for this demo.  */

enum demo_string_ids
{
    RAISED_BORDER = 1,
    RECESSED_BORDER,
    THIN_BORDER,
    NO_BORDER,
    SEGMENTED_FILL,
    SOLID_FILL,
    PIXELMAP_FILL,
    SHOW_TEXT,
    BORDER_STYLE,
    FILL_STYLE,
    TEXT,
    STRING_TABLE_MAX
};

GX_CONST GX_CHAR string_raised_border[] = "raised border";
GX_CONST GX_CHAR string_recessed_border[] = "recessed border";
GX_CONST GX_CHAR string_thin_border[] = "thin border";
GX_CONST GX_CHAR string_no_border[] = "no border";
GX_CONST GX_CHAR string_segmented_fill[] = "segmented fill";
GX_CONST GX_CHAR string_solid_fill[] = "solid fill";
GX_CONST GX_CHAR string_pixelmap_fill[]= "pixelmap fill";
GX_CONST GX_CHAR string_show_text[] = "show text";
GX_CONST GX_CHAR string_border_style[] = "border style";
GX_CONST GX_CHAR string_fill_style[] = "fill style";
GX_CONST GX_CHAR string_text[] = "text";

/* Define GUIX string for the demo. */
GX_CONST GX_STRING demo_strings[] = {
    {NULL, 0},
   {string_raised_border, sizeof(string_raised_border) - 1},
   {string_recessed_border, sizeof(string_recessed_border) - 1},
   {string_thin_border, sizeof(string_thin_border) - 1},
   {string_no_border, sizeof(string_no_border) - 1},
   {string_segmented_fill, sizeof(string_segmented_fill) - 1},
   {string_solid_fill, sizeof(string_solid_fill) - 1},
   {string_pixelmap_fill, sizeof(string_pixelmap_fill) - 1},
   {string_show_text, sizeof(string_show_text) - 1},
   {string_border_style, sizeof(string_border_style) - 1},
   {string_fill_style, sizeof(string_fill_style) - 1},
   {string_text, sizeof(string_text) - 1}
};

/* Define Language table. */
GX_CONST GX_STRING *language_table[1] = 
{
    demo_strings,
};

/* widget id values used by this demo */
enum demo_widget_ids {
    ID_RAISED_BORDER = 1, 
    ID_RECESSED_BORDER,
    ID_THIN_BORDER,
    ID_NO_BORDER,
    ID_SEGMENTED_FILL,
    ID_SOLID_FILL,
    ID_PIXELMAP_FILL,
    ID_SHOW_TEXT
};

/* Define prototypes.   */

VOID  start_guix(VOID);
UINT  main_event_handler(GX_WIDGET *widget, GX_EVENT *myevent);
UINT  border_win_event_handler(GX_WIDGET *widget, GX_EVENT *myevent);
UINT  fill_win_event_handler(GX_WIDGET *widget, GX_EVENT *myevent);

/* Prototype for the win32 screen driver setup routine. */
extern UINT  win32_graphics_driver_setup_24xrgb(GX_DISPLAY *graphic);

int main(int argc, char ** argv)
{
    tx_kernel_enter();
    return (0);
}



VOID tx_application_define(void *first_unused_memory)
{
    start_guix();
}

VOID  start_guix(VOID)
{

GX_RECTANGLE    root_window_size;
GX_RECTANGLE    size;

    /* Initialize GUIX.  */
    gx_system_initialize();

    /* Create the demo display and associated driver.  */
    gx_display_create(&demo_display, "demo display", win32_graphics_driver_setup_24xrgb,
                      DEMO_DISPLAY_WIDTH, DEMO_DISPLAY_HEIGHT);

    /* Install the demo color table. */
    gx_display_color_table_set(&demo_display, demo_color_table, sizeof(demo_color_table) / sizeof(GX_COLOR));

    /* Install the demo font table. */
    gx_display_font_table_set(&demo_display, demo_font_table, sizeof(demo_font_table) / sizeof(GX_FONT *));

    /* Install the demo pixelmap table. */
    gx_display_pixelmap_table_set(&demo_display, demo_pixelmap_table, sizeof(demo_pixelmap_table) / sizeof(GX_PIXELMAP *));

    /* Install the demo language table.  */
    gx_display_language_table_set_ext(&demo_display, language_table, 1, STRING_TABLE_MAX);
    gx_system_active_language_set(0);

    /* Create the default canvas. */
    gx_canvas_create(&default_canvas, "demo canvas", &demo_display, 
                     GX_CANVAS_MANAGED | GX_CANVAS_VISIBLE,
                     DEMO_DISPLAY_WIDTH, DEMO_DISPLAY_HEIGHT,
                     default_canvas_memory, sizeof(default_canvas_memory));

    /* Define the size of the root window. */
    gx_utility_rectangle_define(&root_window_size, 0, 0, DEMO_DISPLAY_WIDTH - 1, DEMO_DISPLAY_HEIGHT - 1);

    /* Create a background root window and attach to the canvas. */
    gx_window_root_create(&demo_root_window, "demo root window", &default_canvas,
                          GX_STYLE_BORDER_NONE, GX_ID_NONE, &root_window_size);

    /* Set the root window to be black */
    gx_widget_fill_color_set(&demo_root_window, GX_COLOR_ID_WHITE, GX_COLOR_ID_WHITE, GX_COLOR_ID_WHITE);

    gx_widget_event_process_set((GX_WIDGET *)&demo_root_window, main_event_handler);

    /* Define the size and the position of the progress bar. */
    gx_utility_rectangle_define(&size, 200, 60, 389, 79);

    /* Initialize the information of the progress bar. */
    info.gx_progress_bar_info_max_val = 100;
    info.gx_progress_bar_info_min_val = 0;
    info.gx_progress_bar_info_current_val = 0;
    info.gx_progress_bar_font_id = GX_FONT_ID_PROMPT;
    info.gx_progress_bar_normal_text_color = GX_COLOR_ID_BLACK;
    info.gx_progress_bar_selected_text_color = GX_COLOR_ID_BLACK;

    /* Create the progress bar on top of the root window. */
    gx_progress_bar_create(&progress_bar, NULL, &demo_root_window, &info, GX_STYLE_BORDER_THIN|GX_STYLE_PROGRESS_SEGMENTED_FILL|GX_STYLE_PROGRESS_TEXT_DRAW|GX_STYLE_PROGRESS_PERCENT, GX_ID_NONE, &size);

    gx_utility_rectangle_define(&size, 55, 155, 215, 355);
    gx_window_create(&border_style_window, "border style window", &demo_root_window, GX_STYLE_BORDER_NONE, GX_ID_NONE, &size);
    gx_widget_fill_color_set(&border_style_window, GX_COLOR_ID_WHITE, GX_COLOR_ID_WHITE, GX_COLOR_ID_WHITE);
    gx_widget_event_process_set((GX_WIDGET *)&border_style_window, border_win_event_handler);

    gx_utility_rectangle_define(&size, 60, 160, 210, 190);
    gx_prompt_create(&prompt_1, NULL, &border_style_window, BORDER_STYLE, GX_STYLE_BORDER_THIN|GX_STYLE_TEXT_LEFT, GX_ID_NONE, &size);
    prompt_1.gx_widget_normal_fill_color = GX_COLOR_ID_WIDGET_FILL;
    gx_prompt_text_color_set(&prompt_1, GX_COLOR_ID_WHITE, GX_COLOR_ID_WHITE, GX_COLOR_ID_WHITE);

    /* Define the size and the position of the radio button for selecting the border style. */
    gx_utility_rectangle_define(&size, 60, 200, 210, 230);
    gx_radio_button_create(&radio_button_1, NULL, &border_style_window, RAISED_BORDER, GX_STYLE_TEXT_LEFT|GX_STYLE_ENABLED, ID_RAISED_BORDER, &size);

    gx_utility_rectangle_define(&size, 60, 240, 210, 270);
    gx_radio_button_create(&radio_button_2, NULL, &border_style_window, RECESSED_BORDER, GX_STYLE_TEXT_LEFT|GX_STYLE_ENABLED, ID_RECESSED_BORDER, &size);

    gx_utility_rectangle_define(&size, 60, 280, 210, 310);
    gx_radio_button_create(&radio_button_3, NULL, &border_style_window, THIN_BORDER, GX_STYLE_TEXT_LEFT|GX_STYLE_ENABLED|GX_STYLE_BUTTON_PUSHED, ID_THIN_BORDER, &size);

    gx_utility_rectangle_define(&size, 60, 320, 210, 350);
    gx_radio_button_create(&radio_button_4, NULL, &border_style_window, NO_BORDER, GX_STYLE_TEXT_LEFT|GX_STYLE_ENABLED, ID_NO_BORDER, &size);

    gx_utility_rectangle_define(&size, 215, 155, 375, 315);
    gx_window_create(&fill_style_window, "fill style window", &demo_root_window, GX_STYLE_BORDER_NONE, GX_ID_NONE, &size);
    gx_widget_fill_color_set(&fill_style_window, GX_COLOR_ID_WHITE, GX_COLOR_ID_WHITE, GX_COLOR_ID_WHITE);
    gx_widget_event_process_set((GX_WIDGET *)&fill_style_window, fill_win_event_handler);

    gx_utility_rectangle_define(&size, 220, 160, 370, 190);
    gx_prompt_create(&prompt_2, NULL, &fill_style_window, FILL_STYLE, GX_STYLE_BORDER_THIN|GX_STYLE_TEXT_LEFT, GX_ID_NONE, &size);  
    prompt_2.gx_widget_normal_fill_color = GX_COLOR_ID_WIDGET_FILL;
    gx_prompt_text_color_set(&prompt_2, GX_COLOR_ID_WHITE, GX_COLOR_ID_WHITE, GX_COLOR_ID_WHITE);

    /* Define the size and the position of the radio button for selecting the fill style. */
    gx_utility_rectangle_define(&size, 220, 200, 370, 230);
    gx_radio_button_create(&radio_button_5, NULL, &fill_style_window, SEGMENTED_FILL, GX_STYLE_TEXT_LEFT|GX_STYLE_ENABLED|GX_STYLE_BUTTON_PUSHED, ID_SEGMENTED_FILL, &size);

    gx_utility_rectangle_define(&size, 220, 240, 370, 270);
    gx_radio_button_create(&radio_button_6, NULL, &fill_style_window, SOLID_FILL, GX_STYLE_TEXT_LEFT|GX_STYLE_ENABLED, ID_SOLID_FILL, &size);

    gx_utility_rectangle_define(&size, 220, 280, 370, 310);
    gx_radio_button_create(&radio_button_7, NULL, &fill_style_window, PIXELMAP_FILL, GX_STYLE_TEXT_LEFT|GX_STYLE_ENABLED, ID_PIXELMAP_FILL, &size);

    gx_utility_rectangle_define(&size, 380, 160, 480, 190);
    gx_prompt_create(&prompt_3, NULL, &demo_root_window, TEXT, GX_STYLE_BORDER_THIN|GX_STYLE_TEXT_LEFT, GX_ID_NONE, &size);
    prompt_3.gx_widget_normal_fill_color = GX_COLOR_ID_WIDGET_FILL;
    gx_prompt_text_color_set(&prompt_3, GX_COLOR_ID_WHITE, GX_COLOR_ID_WHITE, GX_COLOR_ID_WHITE);

    /* Define the size and the position of the checkbox button for selecting whether to show text or not. */
    gx_utility_rectangle_define(&size, 380, 200, 480, 230);
    gx_checkbox_create(&checkbox, NULL, &demo_root_window, SHOW_TEXT, GX_STYLE_TEXT_LEFT|GX_STYLE_ENABLED|GX_STYLE_BUTTON_PUSHED, ID_SHOW_TEXT, &size);
    checkbox.gx_widget_normal_fill_color = GX_COLOR_ID_WHITE;

    /* Show the root window.  */
    gx_widget_show(&demo_root_window);

    /* start GUIX thread */
    gx_system_start();
}

UINT  main_event_handler(GX_WIDGET *widget, GX_EVENT *myevent)
{
UINT       status = 0;

    switch(myevent -> gx_event_type)
    {
    case GX_EVENT_SHOW:
        gx_widget_event_process(widget, myevent);
        gx_system_timer_start((GX_WIDGET *)(&progress_bar), ID_PROGRESS_BAR_TIMER, 5, 5);
        break;

    case GX_EVENT_HIDE:
        gx_widget_event_process(widget, myevent);
        gx_system_timer_stop((GX_WIDGET *)(&progress_bar), 0);
        break;

    case GX_EVENT_TIMER:
        if(myevent -> gx_event_payload.gx_event_timer_id == ID_PROGRESS_BAR_TIMER)
        {
            if(progress_bar.gx_progress_bar_info.gx_progress_bar_info_current_val < progress_bar.gx_progress_bar_info.gx_progress_bar_info_max_val)
            {
                progress_bar.gx_progress_bar_info.gx_progress_bar_info_current_val++;
            }
            else
            {
                progress_bar.gx_progress_bar_info.gx_progress_bar_info_current_val = progress_bar.gx_progress_bar_info.gx_progress_bar_info_min_val;
            }
            gx_system_dirty_mark(widget);
        }
        break;

    case GX_SIGNAL(ID_SHOW_TEXT, GX_EVENT_TOGGLE_ON):
        progress_bar.gx_widget_style |= GX_STYLE_PROGRESS_TEXT_DRAW;
        gx_system_dirty_mark(widget);
        break;

    case GX_SIGNAL(ID_SHOW_TEXT, GX_EVENT_TOGGLE_OFF):
        progress_bar.gx_widget_style &= ~GX_STYLE_PROGRESS_TEXT_DRAW;
        gx_system_dirty_mark(widget);
        break;

    default:
        status = gx_window_event_process((GX_WINDOW *)widget, myevent);
        break;
    }
    return status;
}

UINT  border_win_event_handler(GX_WIDGET *widget, GX_EVENT *myevent)
{
UINT       status = 0;

    switch(myevent -> gx_event_type)
    {
    case GX_SIGNAL(ID_RAISED_BORDER, GX_EVENT_RADIO_SELECT):
        progress_bar.gx_widget_style &= ~GX_STYLE_BORDER_MASK;
        progress_bar.gx_widget_style |= GX_STYLE_BORDER_RAISED;
        gx_system_dirty_mark((GX_WIDGET *)&demo_root_window);
        break;

    case GX_SIGNAL(ID_RECESSED_BORDER, GX_EVENT_RADIO_SELECT):
        progress_bar.gx_widget_style &= ~GX_STYLE_BORDER_MASK;
        progress_bar.gx_widget_style |= GX_STYLE_BORDER_RECESSED;
        gx_system_dirty_mark((GX_WIDGET *)&demo_root_window);
        break;

    case GX_SIGNAL(ID_THIN_BORDER, GX_EVENT_RADIO_SELECT):
        progress_bar.gx_widget_style &= ~GX_STYLE_BORDER_MASK;
        progress_bar.gx_widget_style |= GX_STYLE_BORDER_THIN;
        gx_system_dirty_mark((GX_WIDGET *)&demo_root_window);
        break;

    case GX_SIGNAL(ID_NO_BORDER, GX_EVENT_RADIO_SELECT):
        progress_bar.gx_widget_style &= ~GX_STYLE_BORDER_MASK;
        progress_bar.gx_widget_style |= GX_STYLE_BORDER_NONE;
        gx_system_dirty_mark((GX_WIDGET *)&demo_root_window);
        break;

    default:
        status = gx_window_event_process((GX_WINDOW *)widget, myevent);
        break;
    }
    return status;
}
UINT  fill_win_event_handler(GX_WIDGET *widget, GX_EVENT *myevent)
{
UINT       status = 0;

    switch(myevent -> gx_event_type)
    {
    case GX_SIGNAL(ID_SEGMENTED_FILL, GX_EVENT_RADIO_SELECT):
        progress_bar.gx_widget_style |= GX_STYLE_PROGRESS_SEGMENTED_FILL;
        progress_bar.gx_progress_bar_info.gx_progress_bar_fill_pixelmap = 0;
        gx_system_dirty_mark((GX_WIDGET *)&demo_root_window);
        break;

    case GX_SIGNAL(ID_SOLID_FILL, GX_EVENT_RADIO_SELECT):
        progress_bar.gx_widget_style &= ~GX_STYLE_PROGRESS_SEGMENTED_FILL;
        progress_bar.gx_progress_bar_info.gx_progress_bar_fill_pixelmap = 0;
        gx_system_dirty_mark((GX_WIDGET *)&demo_root_window);
        break;

    case GX_SIGNAL(ID_PIXELMAP_FILL, GX_EVENT_RADIO_SELECT):
        progress_bar.gx_widget_style &= ~GX_STYLE_PROGRESS_SEGMENTED_FILL;
        gx_progress_bar_pixelmap_set(&progress_bar, BID_PROMPT_FILL);
        gx_system_dirty_mark((GX_WIDGET *)&demo_root_window);
        break;

    default:
        status = gx_window_event_process((GX_WINDOW *)widget, myevent);
        break;
    }
    return status;
}
