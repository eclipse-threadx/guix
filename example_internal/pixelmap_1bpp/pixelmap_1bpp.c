/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"

/* Define constants for the GUIX Win32 demo.  */

/* Define constants specific to this implemenation.  */

#define DEMO_DISPLAY_WIDTH                         320
#define DEMO_DISPLAY_HEIGHT                        240

/* User-defined color ID */
#define GX_1BPP_BLACK    0x00
#define GX_1BPP_WHITE    0x01


/* User-defined color table. */
static GX_COLOR demo_color_table[] = 
{
    GX_1BPP_BLACK,
    GX_1BPP_WHITE,
};

extern GX_PIXELMAP pic1_1bpp_nocompressed_pixelmap;
extern GX_PIXELMAP pic1_1bpp_compressed_pixelmap;
extern GX_PIXELMAP pic1_1bpp_transparent_pixelmap;
GX_PIXELMAP *demo_pixelmap_table[] = {
    NULL,
    &pic1_1bpp_nocompressed_pixelmap,
    &pic1_1bpp_compressed_pixelmap,
    &pic1_1bpp_transparent_pixelmap,
};

enum demo_pixelmaps {
    PIC_1BPP_RAW = 1,
    PIC_1BPP_COMPRESSED,
    PIC_1BPP_TRANSPARENT,
    NUMBER_OF_PIXELMAPS
};

#define DEFAULT_CANVAS_PIXELS     (DEMO_DISPLAY_WIDTH * DEMO_DISPLAY_HEIGHT)

/* Define the ThreadX demo thread control block and stack.  */

TX_THREAD          demo_thread;

/* Define the stack for the demo thread. */
ULONG              demo_thread_stack[4096 / sizeof(ULONG)];

/* Define the GUIX resources for this demo.  */
GX_CANVAS         default_canvas;

GX_DISPLAY        demo_display;
GX_WINDOW_ROOT    demo_root_window;

GX_WINDOW         pixelmap_window;
GX_WINDOW         main_window;
GX_BOOL           compressed = GX_FALSE;
GX_BOOL           transparent = GX_FALSE;
INT               xshift;
INT               yshift;
INT               horizontal_slider = 0;
INT               vertical_slider = 0;

GX_PROMPT         width_label;
GX_PROMPT         height_label;
GX_PROMPT         width_prompt;
GX_PROMPT         height_prompt;
GX_SLIDER         width_slider;
GX_SLIDER         height_slider;
GX_CHECKBOX       compressed_box;
GX_CHECKBOX       transparent_box;

/* Define GUIX canvas for this demo.  */

ULONG default_canvas_memory[DEFAULT_CANVAS_PIXELS/sizeof(ULONG)];

/* Define GUIX strings and string IDs for this demo.  */

enum demo_string_ids
{
    SID_WIDTH = 1,
    SID_HEIGHT,
    SID_COMPRESSED,
    SID_TRANSPARENT,
    STRING_TABLE_MAX
};

char *demo_strings[] = {
    NULL,
    "Width",
    "Height",
    "Compressed",
    "Transparent",
};

/* widget id values used by this demo */
enum demo_widget_ids {
    ID_WIDTH_SLIDER = 1, 
    ID_HEIGHT_SLIDER,
    BID_COMPRESSED,
    BID_TRANSPARENT,
};

/* Define prototypes.   */

VOID  demo_thread_entry(ULONG thread_input);

UINT  pic_win_event_handler(GX_WIDGET *me, GX_EVENT *myevent);
VOID  pic_win_draw(GX_WINDOW *window);
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

    /* Initialize GUIX.  */
    gx_system_initialize();

#ifdef WIN32
    /* Create the demo display and associated driver.  */
    gx_display_create(&demo_display, "demo display", win32_graphics_driver_setup_monochrome,
                      DEMO_DISPLAY_WIDTH, DEMO_DISPLAY_HEIGHT);
#endif

    gx_display_color_table_set(&demo_display, demo_color_table, sizeof(demo_color_table) / sizeof(GX_COLOR));

    gx_display_pixelmap_table_set(&demo_display, demo_pixelmap_table, NUMBER_OF_PIXELMAPS);

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


    /* add width adjustment children */
    gx_window_create((GX_WINDOW *) &main_window, NULL, &demo_root_window, GX_STYLE_BORDER_THICK, GX_ID_NONE, &size);
    gx_widget_fill_color_set(&main_window, GX_1BPP_WHITE, GX_1BPP_WHITE, GX_1BPP_WHITE);
    gx_widget_event_process_set(&main_window, pic_win_event_handler);
    
    gx_utility_rectangle_define(&size, 55, 155, 214, 172);
    memset(&info, 0, sizeof(GX_SLIDER_INFO));

    info.gx_slider_info_current_val = 5;
    info.gx_slider_info_min_val = 1;
    info.gx_slider_info_max_val = 10;
    info.gx_slider_info_increment = 1;
    info.gx_slider_info_needle_inset = 5;
    info.gx_slider_info_needle_height = 10;
    info.gx_slider_info_needle_width = 5;
    info.gx_slider_info_min_travel = 10;
    info.gx_slider_info_max_travel = 10;


    gx_slider_create(&width_slider, NULL, &main_window,
                     10, &info, GX_STYLE_ENABLED|GX_STYLE_SHOW_TICKMARKS|GX_STYLE_SHOW_NEEDLE|GX_STYLE_BORDER_THIN,
                     ID_WIDTH_SLIDER, &size);
    width_slider.gx_widget_normal_fill_color = GX_1BPP_WHITE;
    width_slider.gx_widget_selected_fill_color = GX_1BPP_WHITE;
    width_slider.gx_slider_tick_color = GX_1BPP_BLACK;

    gx_utility_rectangle_define(&size, 268, 25, 283, 174);
    memset(&info, 0, sizeof(GX_SLIDER_INFO));

    info.gx_slider_info_current_val = 5;
    info.gx_slider_info_min_val = 1;
    info.gx_slider_info_max_val = 10;
    info.gx_slider_info_increment = 1;
    info.gx_slider_info_needle_inset = 5;
    info.gx_slider_info_needle_height = 5;
    info.gx_slider_info_needle_width = 10;
    info.gx_slider_info_min_travel = 10;
    info.gx_slider_info_max_travel = 10;

    gx_slider_create(&height_slider, NULL, &main_window,
                     10, &info, GX_STYLE_ENABLED|GX_STYLE_SHOW_TICKMARKS|GX_STYLE_SHOW_NEEDLE|GX_STYLE_BORDER_THIN|GX_STYLE_SLIDER_VERTICAL,
                     ID_HEIGHT_SLIDER, &size);
    height_slider.gx_widget_normal_fill_color = GX_1BPP_WHITE;
    height_slider.gx_widget_selected_fill_color = GX_1BPP_WHITE;
    height_slider.gx_slider_tick_color = GX_1BPP_BLACK;

    gx_utility_rectangle_define(&size, 27, 206, 150, 229);
    gx_checkbox_create(&compressed_box, NULL, &main_window, SID_COMPRESSED,
                       GX_STYLE_TEXT_LEFT, BID_COMPRESSED, &size);

    gx_utility_rectangle_define(&size, 177, 206, 300, 229);
    gx_checkbox_create(&transparent_box, NULL, &main_window, SID_TRANSPARENT,
                       GX_STYLE_TEXT_LEFT, BID_TRANSPARENT, &size);

    gx_utility_rectangle_define(&size, 95, 70, 165, 110);
    gx_window_create(&pixelmap_window, NULL, &main_window, 0, GX_ID_NONE, &size);
    gx_window_wallpaper_set(&pixelmap_window, PIC_1BPP_RAW, TRUE); 
    gx_widget_fill_color_set(&pixelmap_window, GX_1BPP_WHITE, GX_1BPP_WHITE, GX_1BPP_WHITE);
    gx_widget_draw_set(&pixelmap_window, pic_win_draw);
    /* Show the root window.  */
    gx_widget_show(&demo_root_window);


#if defined(GUIX_VALIDATION)
    /* do_validation(); */
#endif

    /* let GUIX run */
    gx_system_start();
}

UINT pic_win_event_handler(GX_WIDGET *widget, GX_EVENT *myevent)
{
UINT status = 0;
GX_WINDOW *window = (GX_WINDOW*)widget;
GX_SLIDER *slider_w = &width_slider;
GX_SLIDER *slider_h = &height_slider;

    switch(myevent -> gx_event_type)
    {
    case GX_SIGNAL(ID_WIDTH_SLIDER, GX_EVENT_SLIDER_VALUE):

        /* kgm FIXME- should really just mark the line area as dirty */
        xshift = myevent ->gx_event_payload.gx_event_longdata;

        if(horizontal_slider == 0)
        {
            horizontal_slider = (slider_h ->gx_slider_info.gx_slider_info_max_val + slider_h ->gx_slider_info.gx_slider_info_min_val) >> 1;
        }
        gx_widget_shift(&pixelmap_window, xshift - horizontal_slider, 0, GX_TRUE);
        horizontal_slider = xshift;
        break;

    case GX_SIGNAL(ID_HEIGHT_SLIDER, GX_EVENT_SLIDER_VALUE):
        /* kgm FIXME- should really just mark the line area as dirty */
        yshift = myevent ->gx_event_payload.gx_event_longdata;

        if(vertical_slider == 0)
        {
            vertical_slider = (slider_w ->gx_slider_info.gx_slider_info_max_val + slider_w ->gx_slider_info.gx_slider_info_min_val) >> 1;
        }
        gx_widget_shift(&pixelmap_window, 0, vertical_slider - yshift, GX_TRUE);
        vertical_slider = yshift;
        break;

    case GX_SIGNAL(BID_COMPRESSED, GX_EVENT_TOGGLE_ON):
        compressed = GX_TRUE;
        gx_system_dirty_mark(widget);
        break;

    case GX_SIGNAL(BID_COMPRESSED, GX_EVENT_TOGGLE_OFF):
        compressed = GX_FALSE;
        gx_system_dirty_mark(widget);
        break;

    case GX_SIGNAL(BID_TRANSPARENT, GX_EVENT_TOGGLE_ON):
        transparent = GX_TRUE;
        gx_system_dirty_mark(widget);
        break;

    case GX_SIGNAL(BID_TRANSPARENT, GX_EVENT_TOGGLE_OFF):
        transparent = GX_FALSE;
        gx_system_dirty_mark(widget);
        break;

    default:
        status = gx_window_event_process(window, myevent);
        break;
    }
    return status;
}


VOID pic_win_draw(GX_WINDOW *window)
{
    if(transparent)
    {
        gx_window_wallpaper_set(window, PIC_1BPP_TRANSPARENT, GX_TRUE);
    }
    else
    {
        if(compressed)
        {
            gx_window_wallpaper_set(window, PIC_1BPP_COMPRESSED, GX_TRUE);
        }
        else
        {
            gx_window_wallpaper_set(window, PIC_1BPP_RAW, GX_TRUE);
        }
    }
    gx_window_draw((GX_WINDOW*) window);
}

