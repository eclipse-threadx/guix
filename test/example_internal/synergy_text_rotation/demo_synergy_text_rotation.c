/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"

#include "synergy_text_rotation_resources.h"
#include "synergy_text_rotation_specifications.h"

#define           ROTATION_TIMER 1

GX_WINDOW_ROOT    *root;
GX_CANVAS         text_canvas;
TX_BYTE_POOL      memory_pool;

GX_PIXELMAP       Alphamap_8bit_font;
GX_PIXELMAP       Alphamap_4bit_font;
GX_PIXELMAP       Alphamap_1bit_font;
GX_PIXELMAP       Alphamap_8bit_dynamic;
 
INT               text_angle = 30;
GX_BOOL           animation_running = GX_FALSE;
INT               xshift_8bpp = 10;
INT               yshift_8bpp = 10;
INT               xshift_4bpp = 5;
INT               yshift_4bpp = 5;
INT               xshift_1bpp = 5;
INT               yshift_1bpp = 5;
INT               xshift_dynamic = 0;
INT               yshift_dynamic = 0;
INT               dynamic_count = 123456789;
GX_CHAR           dynamic_text[20];

GX_RESOURCE_ID    text_color = GX_COLOR_ID_RED;

/* define some memory for storage of rotated text pixelmap */
#define  ROTATION_MEMORY_SIZE (MAIN_DISPLAY_X_RESOLUTION * MAIN_DISPLAY_Y_RESOLUTION)
GX_UBYTE rotation_memory[ROTATION_MEMORY_SIZE];

/* Define prototypes.   */

VOID  start_guix(VOID);
extern UINT win32_dave2d_graphics_driver_setup_24xrgb(GX_DISPLAY *display);
VOID RenderTextToAlphamaps(GX_WIDGET *window);

int main(int argc, char ** argv)
{
  tx_kernel_enter();
  return(0);
}

/******************************************************************************************/
VOID *memory_allocate(ULONG size)
{
    VOID *memptr;

    if (tx_byte_allocate(&memory_pool, &memptr, size, TX_NO_WAIT) == TX_SUCCESS)
    {
        return memptr;
    }
    return NULL;
}

/******************************************************************************************/
void memory_free(VOID *mem)
{
    tx_byte_release(mem);
}

/******************************************************************************************/
VOID tx_application_define(void *first_unused_memory)
{
    /* create byte pool for needle rotation to use */
    tx_byte_pool_create(&memory_pool, "scratchpad", rotation_memory, 
                        ROTATION_MEMORY_SIZE);
    start_guix();
}

/******************************************************************************************/
VOID  start_guix(VOID)
{
    /* Initialize GUIX.  */
    gx_system_initialize();

    gx_studio_display_configure(MAIN_DISPLAY, win32_dave2d_graphics_driver_setup_24xrgb,
        LANGUAGE_ENGLISH, MAIN_DISPLAY_DEFAULT_THEME, &root);

    /* install our memory allocator and de-allocator */
    gx_system_memory_allocator_set(memory_allocate, memory_free);

    /* create the main screen */
    gx_studio_named_widget_create("screen_base", (GX_WIDGET *)root, GX_NULL);

    /* Show the root window to make it and patients screen visible.  */
    gx_widget_show(root);

    /* start GUIX thread */
    gx_system_start();
}

VOID RotateAndShift(GX_WINDOW *window)
{
INT *xshift;
INT *yshift;

    text_angle += 5;
    if (text_angle >= 360)
    {
        text_angle = 0;
    }

    switch(window->gx_widget_id)
    {
    case TEXT_WIN_8BPP:
        xshift = &xshift_8bpp;
        yshift = &yshift_8bpp;
        break;

    case TEXT_WIN_4BPP:
        xshift = &xshift_4bpp;
        yshift = &yshift_4bpp;
        break;

    case TEXT_WIN_1BPP:
        xshift = &xshift_1bpp;
        yshift = &yshift_1bpp;
        break;

    case DYNAMIC_TEXT_WINDOW:
        xshift = &xshift_dynamic;
        yshift = &yshift_dynamic;
        dynamic_count += 17;
        gx_system_dirty_mark(&screen_base.screen_base_dynamic_text_window);
        break;
    }

    if (*xshift || *yshift)
    {
        gx_widget_shift(window, *xshift, *yshift, GX_TRUE);

        if (*xshift > 0)
        {
            if (window->gx_widget_size.gx_rectangle_right >= MAIN_DISPLAY_X_RESOLUTION)
            {
                *xshift = -*xshift;
            }
        }
        else
        {
            if (window->gx_widget_size.gx_rectangle_left <= 0)
            {
                *xshift = -*xshift;
            }
        }
        if (*yshift > 0)
        {
            if (window->gx_widget_size.gx_rectangle_bottom >= MAIN_DISPLAY_Y_RESOLUTION)
            {
                *yshift = -*yshift;
            }
        }
        else
        {
            if (window->gx_widget_size.gx_rectangle_top <= 0)
            {
                *yshift = -*yshift;
            }
        }
    }
}

VOID RotateAndShiftAll(VOID)
{
    RotateAndShift(&screen_base.screen_base_text_window);
    RotateAndShift(&screen_base.screen_base_text_window_4bpp);
    RotateAndShift(&screen_base.screen_base_text_window_1bpp);
    RotateAndShift(&screen_base.screen_base_dynamic_text_window);
}

/******************************************************************************************/
UINT background_win_event_process(GX_WINDOW *window, GX_EVENT *myevent)
{
    switch(myevent->gx_event_type)
    {
    case GX_EVENT_SHOW:
        gx_window_event_process(window, myevent);

        // For the text that doesn't change, render it once to an alpha-map. Then
        // we can just rotate this alphamap with each drawing update.
        RenderTextToAlphamaps((GX_WIDGET *) window);
        break;

    case GX_SIGNAL(ID_BUTTON1, GX_EVENT_CLICKED):
        text_color = GX_COLOR_ID_RED;
        if (!animation_running)
        {
            RotateAndShiftAll();
        }
        break;

    case GX_SIGNAL(ID_BUTTON2, GX_EVENT_CLICKED):
        text_color = GX_COLOR_ID_GREEN;
        if (!animation_running)
        {
            RotateAndShiftAll();
        }
        break;

    case GX_SIGNAL(ID_BUTTON3, GX_EVENT_CLICKED):
        text_color = GX_COLOR_ID_BLUE;
        if (!animation_running)
        {
            RotateAndShiftAll();
        }
        break;

    case GX_SIGNAL(ID_HOME, GX_EVENT_CLICKED):
        if (animation_running)
        {
            gx_system_timer_stop(window, ROTATION_TIMER);
            animation_running = GX_FALSE;
        }
        else
        {
            gx_system_timer_start(window, ROTATION_TIMER, 5, 5);
            animation_running = GX_TRUE;
        }
        break;

    case GX_EVENT_TIMER:
        RotateAndShiftAll();
        break;

    default:
        return gx_window_event_process(window, myevent);
    }
    return 0;
}

VOID RenderTextToAlphamaps(GX_WIDGET *widget)
{
GX_FONT *font;

    GX_CONST GX_CHAR string_hello_world[] = "Hello World";
    GX_CONST GX_CHAR string_4bpp_font[] = "4 bpp font";
    GX_CONST GX_CHAR string_1bpp_font[] = "1 bpp font";
    GX_STRING string;


    gx_widget_font_get(widget, GX_FONT_ID_SCREEN_LABEL, &font);
    string.gx_string_ptr = string_hello_world;
    string.gx_string_length = sizeof(string_hello_world) - 1;
    gx_utility_string_to_alphamap_ext(&string, font, &Alphamap_8bit_font);

    // KGM: Add these once done in gx_utility

    gx_widget_font_get(widget, GX_FONT_ID_SMALL_BOLD, &font);
    string.gx_string_ptr = string_4bpp_font;
    string.gx_string_length = sizeof(string_4bpp_font) - 1;
    gx_utility_string_to_alphamap_ext(&string, font, &Alphamap_4bit_font);

    gx_widget_font_get(widget, GX_FONT_ID_MONO_FONT, &font);
    string.gx_string_ptr = string_1bpp_font;
    string.gx_string_length = sizeof(string_1bpp_font) - 1;
    gx_utility_string_to_alphamap_ext(&string, font, &Alphamap_1bit_font);
}

void text_window_draw(GX_WINDOW *window)
{
    INT xpos = 0;
    INT ypos = 0;
    GX_PIXELMAP rotated_text;
    GX_PIXELMAP *source_map = GX_NULL;
    GX_STRING string;

    gx_window_draw(window);

    gx_context_line_color_set(text_color);
    gx_context_fill_color_set(text_color);

    switch (window->gx_widget_id)
    {
    case TEXT_WIN_8BPP:
        source_map = &Alphamap_8bit_font;
        break;

    case TEXT_WIN_4BPP:
        source_map = &Alphamap_4bit_font;
        break;

    case TEXT_WIN_1BPP:
        source_map = &Alphamap_1bit_font;
        break;
    }

    if (source_map)
    {
        if(gx_utility_pixelmap_rotate(source_map, text_angle, &rotated_text, &xpos, &ypos) == GX_SUCCESS)
        {
            xpos = (window->gx_widget_size.gx_rectangle_left + window->gx_widget_size.gx_rectangle_right) / 2;
            ypos = (window->gx_widget_size.gx_rectangle_top + window->gx_widget_size.gx_rectangle_bottom) / 2;  
            xpos -= rotated_text.gx_pixelmap_width / 2;
            ypos -= rotated_text.gx_pixelmap_height / 2;

            gx_canvas_pixelmap_draw(xpos, ypos, &rotated_text);
            memory_free((VOID *) rotated_text.gx_pixelmap_data);
        }
    }
    else
    {
        xpos = (window->gx_widget_size.gx_rectangle_left + window->gx_widget_size.gx_rectangle_right) / 2;
        ypos = (window->gx_widget_size.gx_rectangle_top + window->gx_widget_size.gx_rectangle_bottom) / 2;
        gx_context_font_set(GX_FONT_ID_SMALL_BOLD);
        gx_utility_ltoa(dynamic_count, dynamic_text, 20);
        string.gx_string_ptr = dynamic_text;
        string.gx_string_length = strnlen(dynamic_text, sizeof(dynamic_text));
        gx_canvas_rotated_text_draw_ext(&string, xpos, ypos, text_angle);
    }
}

/* Define prototypes.   */
VOID page_button_draw(GX_PIXELMAP_BUTTON *widget)
{
    GX_PIXELMAP *map;
    INT top;
    INT left;
    GX_BOOL draw_selected;

    left = widget->gx_widget_size.gx_rectangle_left;
    top = widget->gx_widget_size.gx_rectangle_top;

    if (widget->gx_widget_style & GX_STYLE_BUTTON_PUSHED)
    {
        top += 3;
        draw_selected = GX_TRUE;
    }
    else
    {
        draw_selected = GX_FALSE;
    }

    if (draw_selected)
    {
        gx_widget_pixelmap_get(widget, GX_PIXELMAP_ID_BUTTON_ACTIVE, &map);
        gx_context_fill_color_set(GX_COLOR_ID_WHITE);
        gx_context_line_color_set(GX_COLOR_ID_WHITE);
    }
    else
    {
        gx_widget_pixelmap_get(widget, GX_PIXELMAP_ID_BUTTON, &map);
        gx_context_fill_color_set(GX_COLOR_ID_LIGHT_GRAY);
        gx_context_line_color_set(GX_COLOR_ID_LIGHT_GRAY);
    }
    
    gx_canvas_pixelmap_draw(left, top, map);
}
