/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"
#include "gx_canvas.h"

#include "graphics_565rgb_synergy_resources.h"
#include "graphics_565rgb_synergy_specifications.h"

MAIN_WINDOW_CONTROL_BLOCK     *pMainWin;
GX_WINDOW                     *pGraphicsWin;
GX_RESOURCE_ID    line_color = GX_COLOR_ID_CANVAS;
GX_RESOURCE_ID    fill_color = GX_COLOR_ID_SCROLL_BUTTON;
INT               start_angle = 0;
INT               end_angle = 90;
INT               radius = 100;
GX_BOOL           anti_aliased = GX_TRUE;
GX_BOOL           solid_fill = GX_TRUE;
GX_BOOL           pixelmap_fill = GX_FALSE;
INT               compress = 0;
INT               brush_width = 1;

GX_RESOURCE_ID    pixelmap_id[2] = {GX_PIXELMAP_ID_PIXELMAP_RAW, GX_PIXELMAP_ID_PIXELMAP_COMPRESSED };
#define CIRCLE    0
#define ARC       1
#define PIE       2
#define POLYGON   3
#define ELLIPSE   4

INT               draw_shape = CIRCLE;
INT               pre_shape;
INT               a = 100;
INT               b = 50;

/* Define the ThreadX demo thread control block and stack.  */

TX_THREAD          demo_thread;
UCHAR              demo_thread_stack[4096];

GX_WINDOW_ROOT    *root;

/* Define prototypes.   */
VOID  demo_thread_entry(ULONG thread_input);
extern UINT win32_dave2d_graphics_driver_setup_565rgb(GX_DISPLAY *display);

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


    gx_studio_display_configure(DISPLAY_1, win32_dave2d_graphics_driver_setup_565rgb,
                                LANGUAGE_ENGLISH, DISPLAY_1_DEFAULT_THEME, &root);

    /* create the main screen */
    gx_studio_named_widget_create("main_window", (GX_WIDGET *) root, (GX_WIDGET **) &pMainWin);

    /* get a pointer to polygon window */
    pGraphicsWin = &pMainWin -> main_window_graphics_window;

    /* Show the root window to make it and patients screen visible.  */
    gx_widget_show(root);

    /* let GUIX run */
    gx_system_start();
}

VOID update_prompt_value(GX_PROMPT *pp, INT value)
{
static GX_CHAR str[10];

    if(pp)
    {
        gx_utility_ltoa(value, str, 10);
        gx_prompt_text_set(pp, str);
    }
}

void ToggleScreen(GX_WINDOW *new_win, GX_WINDOW *old_win)
{
    if (!new_win->gx_widget_parent)
    {
        gx_widget_attach(root, (GX_WIDGET *) new_win);
    }
    else
    {
        gx_widget_show((GX_WIDGET *) new_win);
    }
    gx_widget_hide((GX_WIDGET *) old_win);
}

UINT main_event_handler(GX_WINDOW *window, GX_EVENT *myevent)
{
UINT status = 0;
GX_PROMPT *prompt;

    switch(myevent -> gx_event_type)
    {
    case GX_SIGNAL(ID_START_ANGLE_SLIDER, GX_EVENT_SLIDER_VALUE):
        start_angle = myevent ->gx_event_payload.gx_event_longdata;
        prompt = &pMainWin->main_window_start_angle_prompt;
        update_prompt_value(prompt, start_angle);
        gx_system_dirty_mark(pGraphicsWin);
        break;

    case GX_SIGNAL(ID_END_ANGLE_SLIDER, GX_EVENT_SLIDER_VALUE):
        end_angle = myevent ->gx_event_payload.gx_event_longdata;
        prompt = &pMainWin->main_window_end_angle_prompt;
        update_prompt_value(prompt, end_angle);
        gx_system_dirty_mark(pGraphicsWin);
        break;

    case GX_SIGNAL(ID_RADIUS_SLIDER, GX_EVENT_SLIDER_VALUE):
        radius = myevent ->gx_event_payload.gx_event_longdata;
        prompt = &pMainWin->main_window_radius_prompt;
        update_prompt_value(prompt, radius);
        gx_system_dirty_mark(pGraphicsWin);
        break;
    
    case GX_SIGNAL(ID_BRUSH_WIDTH, GX_EVENT_SLIDER_VALUE):
        brush_width = myevent->gx_event_payload.gx_event_longdata;
        prompt = &pMainWin->main_window_brush_width_prompt;
        update_prompt_value(prompt, brush_width);
        gx_system_dirty_mark(pGraphicsWin);
        break;

    case GX_SIGNAL(ID_SOLID_FILL, GX_EVENT_TOGGLE_ON):
        solid_fill = GX_TRUE;
        gx_system_dirty_mark(pGraphicsWin);
        break;

    case GX_SIGNAL(ID_SOLID_FILL, GX_EVENT_TOGGLE_OFF):
        solid_fill = GX_FALSE;
        gx_system_dirty_mark(pGraphicsWin);
        break;

    case GX_SIGNAL(ID_PIXELMAP_FILL, GX_EVENT_TOGGLE_ON):
        pixelmap_fill = GX_TRUE;
        gx_system_dirty_mark(pGraphicsWin);
        break;

    case GX_SIGNAL(ID_PIXELMAP_FILL, GX_EVENT_TOGGLE_OFF):
        pixelmap_fill = GX_FALSE;
        gx_system_dirty_mark(pGraphicsWin);
        break;

    case GX_SIGNAL(ID_COMPRESS, GX_EVENT_TOGGLE_ON):
        compress = 1;
        gx_system_dirty_mark(pGraphicsWin);
        break;

    case GX_SIGNAL(ID_COMPRESS, GX_EVENT_TOGGLE_OFF):
        compress = 0;
        gx_system_dirty_mark(pGraphicsWin);
        break;

    case GX_SIGNAL(ID_WALLPAPER, GX_EVENT_TOGGLE_ON):
        gx_window_wallpaper_set(pGraphicsWin, GX_PIXELMAP_ID_CHECKBOX_OFF, GX_TRUE);
        break;

    case GX_SIGNAL(ID_WALLPAPER, GX_EVENT_TOGGLE_OFF):
        gx_window_wallpaper_set(pGraphicsWin, GX_NULL, GX_TRUE);
        break;

    case GX_SIGNAL(ID_BLACK, GX_EVENT_RADIO_SELECT):
        fill_color = GX_COLOR_ID_CANVAS;
        gx_system_dirty_mark(pGraphicsWin);
        break;

    case GX_SIGNAL(ID_FRESHCOLOR, GX_EVENT_RADIO_SELECT):
        fill_color = GX_COLOR_ID_SCROLL_BUTTON;
        gx_system_dirty_mark(pGraphicsWin);
        break;

    case GX_SIGNAL(ID_CIRCLE, GX_EVENT_RADIO_SELECT):
        draw_shape = CIRCLE;
        gx_system_dirty_mark(pGraphicsWin);
        break;

    case GX_SIGNAL(ID_ARC, GX_EVENT_RADIO_SELECT):
        draw_shape = ARC;
        gx_system_dirty_mark(pGraphicsWin);
        break;

    case GX_SIGNAL(ID_PIE, GX_EVENT_RADIO_SELECT):
        draw_shape = PIE;
        gx_system_dirty_mark(pGraphicsWin);
        break;

    case GX_SIGNAL(ID_POLYGON, GX_EVENT_RADIO_SELECT):
        draw_shape = POLYGON;
        gx_system_dirty_mark(pGraphicsWin);
        break;

    case GX_SIGNAL(ID_ELLIPSE, GX_EVENT_RADIO_SELECT):
        draw_shape = ELLIPSE;
        gx_system_dirty_mark(pGraphicsWin);
        break;

    default:
        status = gx_window_event_process(window, myevent);
        break;
    }
    return status;
}

VOID graphics_draw(GX_WINDOW *window)
{
ULONG brush_style = 0;
GX_POINT rectangle[4] = {{188, 50}, {254, 50}, {254, 150}, {188, 150}};
GX_POINT pentagon[5] = {{290, 90}, {335, 50}, {380, 90}, {360, 150}, {310, 150}};
GX_POINT concave[6] = {{50, 50}, {90, 80}, {130, 50}, {130, 150}, {90, 110}, {50, 150}};
GX_POINT star[10] = {{173, 232}, {212, 232}, {223, 192}, {237, 232}, {273, 232}, {244, 258}, {256, 299}, {226, 275}, {192, 298}, {203, 258}};
GX_POINT self_intersection[8] = {{110, 326}, {189, 420}, {266, 326}, {334, 420}, {334, 326}, {264, 420}, {189, 326}, {110, 420}};

    gx_window_draw((GX_WINDOW*) window);

    if(anti_aliased)
    {
        brush_style |= GX_BRUSH_ALIAS;
    }

    if(solid_fill)
    {
        brush_style |= GX_BRUSH_SOLID_FILL;
    }

    if(pixelmap_fill)
    {
        brush_style |= GX_BRUSH_PIXELMAP_FILL;
        gx_context_pixelmap_set(pixelmap_id[compress]);
    }

    gx_context_brush_define(line_color, fill_color, brush_style);
    gx_context_brush_width_set(brush_width);

    switch(draw_shape)
    {
    case CIRCLE:
        gx_canvas_circle_draw(213, 230, radius);
        break;

    case ARC:
        gx_canvas_arc_draw(213, 230, radius, start_angle, end_angle);
        break;

    case PIE:
        gx_canvas_pie_draw(213, 230, radius, start_angle, end_angle);
        gx_context_fill_color_set(GX_COLOR_ID_SHINE);
        if (pixelmap_fill)
        {
            gx_context_pixelmap_set(GX_PIXELMAP_ID_CHECKBOX_ON);
        }
        gx_canvas_pie_draw(213, 230, radius, end_angle, end_angle + 30);
        break;

    case POLYGON:
        gx_canvas_polygon_draw(rectangle, 4);
        gx_canvas_polygon_draw(pentagon, 5);
        gx_canvas_polygon_draw(concave, 6);
        gx_canvas_polygon_draw(star, 10);
        gx_canvas_polygon_draw(self_intersection, 8);
        break;

    case ELLIPSE:
        gx_canvas_ellipse_draw(213, 230, a, b);
        break;
    }

    gx_widget_children_draw(window);
}
