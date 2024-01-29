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
#define DEMO_DISPLAY_WIDTH                         320
#define DEMO_DISPLAY_HEIGHT                        240

/* Define the number of pixels on the canvas */
#define DEFAULT_CANVAS_PIXELS     (DEMO_DISPLAY_WIDTH * DEMO_DISPLAY_HEIGHT)

/* Define the ThreadX demo thread control block. */
TX_THREAD          demo_thread;

/* Define the stack for the demo thread. */
ULONG              demo_thread_stack[4096 / sizeof(ULONG)];

/* Define the GUIX resources for this demo.  */

/* GUIX display represents the physical display device */ 
GX_DISPLAY        demo_display;

/* GUIX canvas is the frame buffer on top of GUIX displayl. */
GX_CANVAS         default_canvas;

/* The root window is a special GUIX background window, right on top of the canvas. */ 
GX_WINDOW_ROOT    demo_root_window;

/* GUIX Prompt displays a string. */
GX_PROMPT         demo_prompt;

/* Memory for the frame buffer. */
GX_COLOR default_canvas_memory[DEFAULT_CANVAS_PIXELS];


/* Define GUIX strings ID for the demo. */
enum demo_string_ids
{
    SID_HELLO_WORLD = 1,
    SID_MAX
};

static GX_CONST GX_CHAR string_hello_world[] = "Hello World";

/* Define GUIX string for the demo. */
GX_CONST GX_STRING demo_strings[] = {
    {NULL, 0},
    {string_hello_world, sizeof(string_hello_world) - 1}
};

/* Define Language table. */
GX_CONST GX_STRING *language_table[1] = 
{
    demo_strings,
};

/* User-defined color ID */
#define GX_COLOR_ID_BLACK     GX_FIRST_USER_COLOR
#define GX_COLOR_ID_WHITE     (GX_FIRST_USER_COLOR + 1)

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
    GX_COLOR_WHITE
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


/* Define prototypes.   */

VOID  demo_thread_entry(ULONG thread_input);


/* Prototype for the win32 screen driver setup routine. */
extern UINT  win32_graphics_driver_setup_24xrgb(GX_DISPLAY *graphic);

int main(int argc, char ** argv)
{

    tx_kernel_enter();

    return (0);
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

GX_RECTANGLE    root_window_size;
GX_RECTANGLE    prompt_position;

    /* Initialize GUIX.  */
    gx_system_initialize();

    /* Create the demo display and associated driver.  */
    gx_display_create(&demo_display, "demo display", win32_graphics_driver_setup_24xrgb,
                      DEMO_DISPLAY_WIDTH, DEMO_DISPLAY_HEIGHT);

    /* Install the demo color table. */
    gx_display_color_table_set(&demo_display, demo_color_table, sizeof(demo_color_table) / sizeof(GX_COLOR));

    /* Install the demo font table. */
    gx_display_font_table_set(&demo_display, demo_font_table, sizeof(demo_font_table) / sizeof(GX_FONT *));

    /* Create the default canvas. */
    gx_canvas_create(&default_canvas, "demo canvas", &demo_display, 
                     GX_CANVAS_MANAGED | GX_CANVAS_VISIBLE,
                     DEMO_DISPLAY_WIDTH, DEMO_DISPLAY_HEIGHT,
                     default_canvas_memory, sizeof(default_canvas_memory));


    /*Define the size of the root window. */
    gx_utility_rectangle_define(&root_window_size, 0, 0, DEMO_DISPLAY_WIDTH - 1, DEMO_DISPLAY_HEIGHT - 1);

    /* Create a background root window and attach to the canvas. */
    gx_window_root_create(&demo_root_window, "demo root window", &default_canvas,
                          GX_STYLE_BORDER_NONE, GX_ID_NONE, &root_window_size);

    /* Set language table.  */
    gx_display_language_table_set_ext(demo_root_window.gx_window_root_canvas->gx_canvas_display, language_table, 1, SID_MAX);
    gx_system_active_language_set(0);

    /* Set the root window to be black */
    gx_widget_fill_color_set(&demo_root_window, GX_COLOR_ID_BLACK, GX_COLOR_ID_BLACK, GX_COLOR_ID_BLACK);

    /* Create a text prompt on the root window.  Set the text color to white, and the background to black. */

    /* Define the size and the position of the prompt. */
    gx_utility_rectangle_define(&prompt_position, 100, 90, 220, 130);

    /* Create the prompt on top of the root window using the string defined by string ID SID_HELLO_WORLD */
    gx_prompt_create(&demo_prompt, NULL, &demo_root_window, SID_HELLO_WORLD, GX_STYLE_NONE, GX_ID_NONE, &prompt_position);    

    /* Set the text color to be white, and the background color to be black. */
    gx_prompt_text_color_set(&demo_prompt, GX_COLOR_ID_WHITE, GX_COLOR_ID_WHITE, GX_COLOR_ID_WHITE);
    
    gx_widget_fill_color_set(&demo_prompt, GX_COLOR_ID_BLACK, GX_COLOR_ID_BLACK, GX_COLOR_ID_BLACK);

    /* Show the root window.  */
    gx_widget_show(&demo_root_window);

    /* let GUIX run */
    gx_system_start();
}
