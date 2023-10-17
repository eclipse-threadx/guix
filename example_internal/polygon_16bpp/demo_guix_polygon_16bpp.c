/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include <string.h>
#include "gx_api.h"

#include "polygon_16bpp_resources.h"
#include "polygon_16bpp_specifications.h"

MAIN_WINDOW_CONTROL_BLOCK *pMainWin;
GX_WINDOW      *pPolygonWin;
int            line_width = 1;
GX_RESOURCE_ID line_color = GX_COLOR_ID_CANVAS;
GX_RESOURCE_ID fill_color = GX_COLOR_ID_SCROLL_BUTTON;

GX_BOOL    anti_aliased = GX_TRUE;
GX_BOOL    style_round = GX_TRUE;
GX_BOOL    solid_fill = GX_TRUE;
GX_BOOL    pixelmap_fill = GX_FALSE;
int        alpha = 0;
int        compress = 0;

GX_RESOURCE_ID pixelmap_id[2][2] = {{GX_PIXELMAP_ID_RADIOBUTTON_RAW, GX_PIXELMAP_ID_RADIOBUTTON_COMPRESS}, 
                                    {GX_PIXELMAP_ID_RADIOBUTTON_ALPHA, GX_PIXELMAP_ID_RADIOBUTTON_ALPHA_COMPRESS}};

/* Define the ThreadX demo thread control block and stack.  */

TX_THREAD          demo_thread;
UCHAR              demo_thread_stack[4096];

GX_WINDOW_ROOT    *root;

/* Define prototypes.   */
VOID  demo_thread_entry(ULONG thread_input);
extern UINT win32_graphics_driver_setup_565rgb(GX_DISPLAY *display);

int main(int argc, char ** argv)
{
  tx_kernel_enter();
  return(0);
}


VOID tx_application_define(void *first_unused_memory)
{

    /* Create the main demo thread.  */
    tx_thread_create(&demo_thread, "GUIX Demo Thread", demo_thread_entry, 
                     0,  demo_thread_stack, sizeof(demo_thread_stack), 
                     1, 1, TX_NO_TIME_SLICE, TX_AUTO_START);
}


VOID  demo_thread_entry(ULONG thread_input)
{
    /* Initialize GUIX.  */
    gx_system_initialize();


    gx_studio_display_configure(DISPLAY_1, win32_graphics_driver_setup_565rgb, 
                                LANGUAGE_ENGLISH, DISPLAY_1_DEFAULT_THEME, &root);

    /* create the main screen */
    gx_studio_named_widget_create("main_window", (GX_WIDGET *) root, (GX_WIDGET **) &pMainWin);

    /* get a pointer to polygon window */
    pPolygonWin = &pMainWin -> main_window_polygon_window;

    /* Show the root window to make it and patients screen visible.  */
    gx_widget_show(root);

    /* let GUIX run */
    gx_system_start();
}

VOID update_line_width_prompt()
{
GX_PROMPT *pp;
static GX_CHAR width_buf[10];
GX_STRING width;

    gx_widget_find(pMainWin, ID_LINE_WIDTH_DISPLAY, 0, &pp);

    if(pp)
    {
        gx_utility_ltoa(line_width, width_buf, 10);
        width.gx_string_ptr = width_buf;
        width.gx_string_length = strnlen(width_buf, sizeof(width_buf) - 1);
        gx_prompt_text_set_ext(pp, &width);
    }
}

UINT main_event_handler(GX_WINDOW *window, GX_EVENT *myevent)
{
    UINT status = 0;

    switch(myevent -> gx_event_type)
    {
    case GX_SIGNAL(ID_LINE_WIDTH, GX_EVENT_SLIDER_VALUE):
        line_width = myevent ->gx_event_payload.gx_event_longdata;
        update_line_width_prompt();
        gx_system_dirty_mark(pPolygonWin);
        break;
    
    case GX_SIGNAL(ID_ANTI_ALIASED, GX_EVENT_TOGGLE_ON):
        anti_aliased = GX_TRUE;
        gx_system_dirty_mark(pPolygonWin);
        break;

    case GX_SIGNAL(ID_ANTI_ALIASED, GX_EVENT_TOGGLE_OFF):
        anti_aliased = GX_FALSE;
        gx_system_dirty_mark(pPolygonWin);
        break;

    case GX_SIGNAL(ID_ROUND, GX_EVENT_TOGGLE_ON):
        style_round = GX_TRUE;
        gx_system_dirty_mark(pPolygonWin);
        break;

    case GX_SIGNAL(ID_ROUND, GX_EVENT_TOGGLE_OFF):
        style_round = GX_FALSE;
        gx_system_dirty_mark(pPolygonWin);
        break;

    case GX_SIGNAL(ID_SOLID_FILL, GX_EVENT_TOGGLE_ON):
        solid_fill = GX_TRUE;
        gx_system_dirty_mark(pPolygonWin);
        break;

    case GX_SIGNAL(ID_SOLID_FILL, GX_EVENT_TOGGLE_OFF):
        solid_fill = GX_FALSE;
        gx_system_dirty_mark(pPolygonWin);
        break;
    case GX_SIGNAL(ID_PIXELMAP_FILL, GX_EVENT_TOGGLE_ON):
        pixelmap_fill = GX_TRUE;
        gx_system_dirty_mark(pPolygonWin);
        break;

    case GX_SIGNAL(ID_PIXELMAP_FILL, GX_EVENT_TOGGLE_OFF):
        pixelmap_fill = GX_FALSE;
        gx_system_dirty_mark(pPolygonWin);
        break;

    case GX_SIGNAL(ID_ALPHA, GX_EVENT_TOGGLE_ON):
        alpha = 1;
        gx_system_dirty_mark(pPolygonWin);
        break;

    case GX_SIGNAL(ID_ALPHA, GX_EVENT_TOGGLE_OFF):
        alpha = 0;
        gx_system_dirty_mark(pPolygonWin);
        break;

    case GX_SIGNAL(ID_COMPRESS, GX_EVENT_TOGGLE_ON):
        compress = 1;
        gx_system_dirty_mark(pPolygonWin);
        break;

    case GX_SIGNAL(ID_COMPRESS, GX_EVENT_TOGGLE_OFF):
        compress = 0;
        gx_system_dirty_mark(pPolygonWin);
        break;

    case GX_SIGNAL(ID_WALLPAPER, GX_EVENT_TOGGLE_ON):
        gx_window_wallpaper_set(&pMainWin->main_window_polygon_window, GX_PIXELMAP_ID_CHECKBOX_OFF, GX_TRUE);
        break;

    case GX_SIGNAL(ID_WALLPAPER, GX_EVENT_TOGGLE_OFF):
        gx_window_wallpaper_set(&pMainWin->main_window_polygon_window, GX_NULL, GX_TRUE);
        break;

    case GX_SIGNAL(ID_COLOR_BLACK, GX_EVENT_RADIO_SELECT):
        fill_color = GX_COLOR_ID_CANVAS;
        gx_system_dirty_mark(pPolygonWin);
        break;

    case GX_SIGNAL(ID_COLOR_BLUE, GX_EVENT_RADIO_SELECT):
        fill_color = GX_COLOR_ID_SCROLL_BUTTON;
        gx_system_dirty_mark(pPolygonWin);
        break;

    default:
        status = gx_window_event_process(window, myevent);
        break;
    }
    return status;
}


#define NUM_VERTICES 6
VOID polygon_draw(GX_WINDOW *window)
{
GX_POINT polygon_1[4] = { { 70, 162 }, { 116, 163 }, { 96, 264 }, { 50, 263 } };
GX_POINT polygon_2[4] = { { 300, 162 }, { 346, 163 }, { 326, 274 }, { 280, 263 } };
GX_POINT polygon_3[4] = { { 50, 302 }, { 96, 303 }, { 41, 434 }, { 40, 403 } };
GX_POINT rectangle[4] = {{188, 50}, {254, 50}, {254, 150}, {188, 150}};
GX_POINT pentagon[5] = {{290, 90}, {335, 50}, {380, 90}, {360, 150}, {310, 150}};
GX_POINT concave[6] = {{50, 50}, {90, 80}, {130, 50}, {130, 150}, {90, 110}, {50, 150}};
GX_POINT star[10] = {{173, 232}, {212, 232}, {223, 192}, {237, 232}, {273, 232}, {244, 258}, {256, 299}, {226, 275}, {192, 298}, {203, 258}};
GX_POINT self_intersection[8] = {{110, 330}, {189, 420}, {266, 330}, {334, 424}, {334, 330}, {264, 424}, {189, 330}, {110, 424}};
ULONG brush_style = 0;

    gx_window_draw((GX_WINDOW*) window);

    if(anti_aliased)
    {
        brush_style |= GX_BRUSH_ALIAS;
    }

    if(style_round)
    {
        brush_style |= GX_BRUSH_ROUND;
    }

    if(solid_fill)
    {
        brush_style |= GX_BRUSH_SOLID_FILL;
    }

    if(pixelmap_fill)
    {
        brush_style |= GX_BRUSH_PIXELMAP_FILL;
        gx_context_pixelmap_set(pixelmap_id[alpha][compress]);
    }

    gx_context_brush_define(line_color, fill_color, brush_style);
    gx_context_brush_width_set(line_width);

    gx_canvas_polygon_draw(rectangle, 4);
    gx_canvas_polygon_draw(pentagon, 5);
    gx_canvas_polygon_draw(concave, 6);
    gx_canvas_polygon_draw(star, 10);
    gx_canvas_polygon_draw(self_intersection, 8);

    gx_canvas_polygon_draw(polygon_1, 4);
    gx_canvas_polygon_draw(polygon_2, 4);
    gx_canvas_polygon_draw(polygon_3, 4);
}
