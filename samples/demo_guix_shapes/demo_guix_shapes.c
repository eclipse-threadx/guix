/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"

#include "guix_shapes_resources.h"
#include "guix_shapes_specifications.h"

GX_WINDOW        *pShapesScreen;
GX_WINDOW        *pTextWindow;
GX_WINDOW        *pShapesWindow;
GX_RESOURCE_ID    line_color = GX_COLOR_ID_GRAY;
GX_RESOURCE_ID    fill_color = GX_COLOR_ID_TEAL;
INT               radius = 150;
GX_BOOL           anti_aliased = GX_TRUE;
GX_BOOL           round_end = GX_TRUE;
GX_BOOL           solid_fill = GX_TRUE;
GX_BOOL           pixelmap_fill = GX_FALSE;
INT               brush_width = 2;
INT               start_angle = 60;
INT               end_angle = 90;
INT               angle_off = 0;
INT               ellipse_b = 100;

#define CIRCLE    0
#define ARC       1
#define PIE       2
#define POLYGON   3
#define ELLIPSE   4
#define RECTANGLE 5

#define ARC_TIMER     5
#define PIE_TIMER     6

#define ARC_TICKS     2
#define PIE_TICKS     10


INT   draw_shape = CIRCLE;
INT   pre_shape;

/* Define the ThreadX demo thread control block and stack.  */

TX_THREAD          demo_thread;
UCHAR              demo_thread_stack[4096];
TX_BYTE_POOL       memory_pool;
GX_WINDOW_ROOT    *root;

/* Define prototypes.   */
VOID  demo_thread_entry(ULONG thread_input);
extern UINT win32_graphics_driver_setup_24xrgb(GX_DISPLAY *display);
VOID *memory_allocate(ULONG size);
VOID  memory_free(VOID *mem);
UINT string_length_get(GX_CONST GX_CHAR* input_string, UINT max_string_length);

/* define some memory for storage of rotated text pixelmap */
#define  ROTATION_MEMORY_SIZE (DISPLAY_1_X_RESOLUTION * DISPLAY_1_Y_RESOLUTION)
GX_UBYTE rotation_memory[ROTATION_MEMORY_SIZE];

int main(int argc, char ** argv)
{
  tx_kernel_enter();
  return(0);
}

VOID *memory_allocate(ULONG size)
{
    VOID *memptr;

    if (tx_byte_allocate(&memory_pool, &memptr, size, TX_NO_WAIT) == TX_SUCCESS)
    {
        return memptr;
    }
    return NULL;
}

void memory_free(VOID *mem)
{
    tx_byte_release(mem);
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
    /* create byte pool for needle rotation to use */
    tx_byte_pool_create(&memory_pool, "scratchpad", rotation_memory,
                        ROTATION_MEMORY_SIZE);

    /* Initialize GUIX.  */
    gx_system_initialize();

    gx_studio_display_configure(DISPLAY_1, win32_graphics_driver_setup_24xrgb, 
                                LANGUAGE_ENGLISH, DISPLAY_1_DEFAULT_THEME, &root);

    /* install our memory allocator and de-allocator */
    gx_system_memory_allocator_set(memory_allocate, memory_free);

    /* create the main screen */
    gx_studio_named_widget_create("Shapes_Screen", (GX_WIDGET *) root, (GX_WIDGET **) &pShapesScreen);

    /* create the main screen */
    gx_studio_named_widget_create("Text_Rotaion_Window", GX_NULL, (GX_WIDGET **)&pTextWindow);

    /* get a pointer to polygon window */
    pShapesWindow = &(((SHAPES_SCREEN_CONTROL_BLOCK *)pShapesScreen)->Shapes_Screen_Shapes_Window);

    /* Show the root window to make it and patients screen visible.  */
    gx_widget_show(root);

    /* let GUIX run */
    gx_system_start();
}

VOID update_prompt_value(GX_PIXELMAP_PROMPT *pp, INT value)
{
static GX_CHAR str[10];
GX_STRING string;

    if(pp)
    {
        gx_utility_ltoa(value, str, 10);
        string.gx_string_ptr = str;
        string.gx_string_length = string_length_get(str, sizeof(str) - 1);
        gx_prompt_text_set_ext((GX_PROMPT *) pp, &string);
    }
}

UINT ShapesScreen_event_handler(GX_WINDOW *window, GX_EVENT *myevent)
{
    UINT status = 0;
    GX_PIXELMAP_PROMPT *prompt;
    GX_WINDOW *pGraphicsWin = &(((SHAPES_SCREEN_CONTROL_BLOCK *)pShapesScreen)->Shapes_Screen_graphics_window);

    switch (myevent->gx_event_type)
    {
    case GX_EVENT_SHOW:
        gx_window_event_process(window, myevent);
        break;

    case GX_SIGNAL(ID_RADIUS_SLIDER, GX_EVENT_SLIDER_VALUE):
        radius = myevent->gx_event_payload.gx_event_longdata;
        prompt = &(((SHAPES_SCREEN_CONTROL_BLOCK *)pShapesScreen)->Shapes_Screen_radius_val);
        update_prompt_value(prompt, radius);
        gx_system_dirty_mark(pGraphicsWin);
        break;

    case GX_SIGNAL(ID_BRUSH_WIDTH, GX_EVENT_SLIDER_VALUE):
        brush_width = myevent->gx_event_payload.gx_event_longdata;
        prompt = &(((SHAPES_SCREEN_CONTROL_BLOCK *)pShapesScreen)->Shapes_Screen_brush_width_val);
        update_prompt_value(prompt, brush_width);
        gx_system_dirty_mark(pGraphicsWin);
        break;

    case GX_SIGNAL(ID_ANTI_ALIASED, GX_EVENT_TOGGLE_ON):
        anti_aliased = GX_TRUE;
        gx_system_dirty_mark(pGraphicsWin);
        break;

    case GX_SIGNAL(ID_ANTI_ALIASED, GX_EVENT_TOGGLE_OFF):
        anti_aliased = GX_FALSE;
        gx_system_dirty_mark(pGraphicsWin);
        break;

    case GX_SIGNAL(ID_ROUND_END, GX_EVENT_TOGGLE_ON):
        round_end = GX_TRUE;
        gx_system_dirty_mark(pGraphicsWin);
        break;

    case GX_SIGNAL(ID_ROUND_END, GX_EVENT_TOGGLE_OFF):
        round_end = GX_FALSE;
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

    case GX_SIGNAL(ID_CIRCLE, GX_EVENT_RADIO_SELECT):
        if (pShapesWindow->gx_widget_parent == GX_NULL)
        {
            gx_widget_detach(pTextWindow);
            gx_widget_attach(pShapesScreen, pShapesWindow);
            gx_system_dirty_mark(pShapesScreen);
        }
        draw_shape = CIRCLE;
        gx_system_dirty_mark(pGraphicsWin);
        break;

    case GX_SIGNAL(ID_ELLIPSE, GX_EVENT_RADIO_SELECT):
        if (pShapesWindow->gx_widget_parent == GX_NULL)
        {
            gx_widget_detach(pTextWindow);
            gx_widget_attach(pShapesScreen, pShapesWindow);
            gx_system_dirty_mark(pShapesScreen);
        }
        draw_shape = ELLIPSE;
        gx_system_dirty_mark(pGraphicsWin);
        break;

    case GX_SIGNAL(ID_ARC, GX_EVENT_RADIO_SELECT):
        if (pShapesWindow->gx_widget_parent == GX_NULL)
        {
            gx_widget_detach(pTextWindow);
            gx_widget_attach(pShapesScreen, pShapesWindow);
            gx_system_dirty_mark(pShapesScreen);
        }
        draw_shape = ARC;
        end_angle = start_angle;
        gx_system_timer_start(pGraphicsWin, ARC_TIMER, ARC_TICKS, ARC_TICKS);
        break;

    case GX_SIGNAL(ID_PIE, GX_EVENT_RADIO_SELECT):
        if (pShapesWindow->gx_widget_parent == GX_NULL)
        {
            gx_widget_detach(pTextWindow);
            gx_widget_attach(pShapesScreen, pShapesWindow);
            gx_system_dirty_mark(pShapesScreen);
        }
        draw_shape = PIE;
        gx_system_timer_start(pGraphicsWin, PIE_TIMER, PIE_TICKS, PIE_TICKS);
        gx_system_dirty_mark(pGraphicsWin);
        break;

    case GX_SIGNAL(ID_POLYGON, GX_EVENT_RADIO_SELECT):
        if (pShapesWindow->gx_widget_parent == GX_NULL)
        {
            gx_widget_detach(pTextWindow);
            gx_widget_attach(pShapesScreen, pShapesWindow);
            gx_system_dirty_mark(pShapesScreen);
        }
        draw_shape = POLYGON;
        gx_system_dirty_mark(pGraphicsWin);
        break;

    case GX_SIGNAL(ID_RECTANGLE, GX_EVENT_RADIO_SELECT):
        if (pShapesWindow->gx_widget_parent == GX_NULL)
        {
            gx_widget_detach(pTextWindow);
            gx_widget_attach(pShapesScreen, pShapesWindow);
            gx_system_dirty_mark(pShapesScreen);
        }
        draw_shape = RECTANGLE;
        gx_system_dirty_mark(pGraphicsWin);
        break;

    case GX_EVENT_TIMER:
        if (myevent->gx_event_payload.gx_event_timer_id == ARC_TIMER)
        {
            /* Update arc parameter.  */
            end_angle += 2;
            if (end_angle == 360)
            {
                gx_system_timer_stop(pGraphicsWin, ARC_TIMER);
            }
        }
        else if (myevent->gx_event_payload.gx_event_timer_id == PIE_TIMER)
        {
            /* Update pie parameter.  */
            angle_off += 2;
            if (angle_off >= 360)
            {
                angle_off = 0;
            }
        }
        gx_system_dirty_mark(pGraphicsWin);
        break;

    case GX_SIGNAL(ID_TEXT, GX_EVENT_RADIO_SELECT):
        if (pTextWindow->gx_widget_parent == GX_NULL)
        {
            gx_widget_detach(pShapesWindow);
            gx_widget_attach( pShapesScreen, pTextWindow);
            gx_system_dirty_mark(pShapesScreen);
        }
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
GX_RECTANGLE rect;
GX_POINT rectangle[4] = { { 208, 63 },{ 274, 63 },{ 274, 163 },{ 208, 163 } };
GX_POINT pentagon[5] = { { 315, 103 },{ 360, 63 },{ 405, 103 },{ 385, 163 },{ 335, 163 } };
GX_POINT concave[6] = { { 70, 63 },{ 110, 93 },{ 150, 63 },{ 150, 163 },{ 110, 123 },{ 70, 163 } };
GX_POINT star[10] = { { 188, 227 },{ 227, 227 },{ 238, 187 },{ 252, 227 },{ 288, 227 },{ 259, 253 },{ 271, 294 },{ 241, 270 },{ 207, 293 },{ 218, 253 } };
GX_POINT self_intersection[8] = { { 118, 316 },{ 197, 410 },{ 274, 316 },{ 342, 410 },{ 342, 316 },{ 272, 410 },{ 197, 316 },{ 118, 410 } };
INT      xcenter;
INT      ycenter;

    gx_window_draw((GX_WINDOW*)window);

    if (anti_aliased)
    {
        brush_style |= GX_BRUSH_ALIAS;
    }

    if (solid_fill)
    {
        brush_style |= GX_BRUSH_SOLID_FILL;
    }

    if (pixelmap_fill)
    {
        brush_style |= GX_BRUSH_PIXELMAP_FILL;
        gx_context_pixelmap_set(GX_PIXELMAP_ID_FISH);
    }

    if (round_end)
    {
        brush_style |= GX_BRUSH_ROUND;
    }

    gx_context_brush_define(line_color, fill_color, brush_style);
    gx_context_brush_width_set(brush_width);

    xcenter = window -> gx_widget_size.gx_rectangle_left + ((window -> gx_widget_size.gx_rectangle_right - window -> gx_widget_size.gx_rectangle_left + 1) >> 1);
    ycenter = window -> gx_widget_size.gx_rectangle_top + ((window -> gx_widget_size.gx_rectangle_bottom - window -> gx_widget_size.gx_rectangle_top + 1) >> 1);

    switch (draw_shape)
    {
    case CIRCLE:
        gx_context_brush_define(GX_COLOR_ID_LIGHT_GRAY, GX_COLOR_ID_GREEN, brush_style);
        gx_canvas_circle_draw(xcenter, ycenter, radius);
        break;

    case ARC:
        gx_context_brush_define(GX_COLOR_ID_LIGHT_GRAY, GX_COLOR_ID_PURPLE, brush_style);
        gx_canvas_arc_draw(xcenter, ycenter, radius, start_angle, end_angle);
        break;

    case PIE:
        gx_context_brush_define(GX_COLOR_ID_LIGHT_GRAY, GX_COLOR_ID_INDIAN_RED, brush_style);
        gx_canvas_pie_draw(xcenter, ycenter, radius, 60 + angle_off, 150 + angle_off);

        gx_context_brush_define(GX_COLOR_ID_LIGHT_GRAY, GX_COLOR_ID_YELLOW, brush_style);
        gx_canvas_pie_draw(xcenter, ycenter, radius, 150 + angle_off, 200 + angle_off);

        gx_context_brush_define(GX_COLOR_ID_LIGHT_GRAY, GX_COLOR_ID_GREEN, brush_style);
        gx_canvas_pie_draw(xcenter, ycenter, radius, 200 + angle_off, 280 + angle_off);

        gx_context_brush_define(GX_COLOR_ID_LIGHT_GRAY, GX_COLOR_ID_PURPLE, brush_style);
        gx_canvas_pie_draw(xcenter, ycenter, radius, 280 + angle_off, 60 + angle_off);
        break;

    case RECTANGLE:
        gx_context_line_color_set(GX_COLOR_ID_LIGHT_TEAL);
        gx_context_fill_color_set(GX_COLOR_ID_GREEN);
        rect = window->gx_widget_size;
        gx_utility_rectangle_resize(&rect, -10);
        gx_canvas_rectangle_draw(&rect);

        gx_utility_rectangle_resize(&rect, -30);
        gx_context_line_color_set(GX_COLOR_ID_INDIAN_RED);
        gx_context_fill_color_set(GX_COLOR_ID_PURPLE);
        gx_canvas_rectangle_draw(&rect);

        gx_utility_rectangle_resize(&rect, -30);
        gx_context_line_color_set(GX_COLOR_ID_LIGHT_GRAY);
        gx_context_fill_color_set(GX_COLOR_ID_BLACK);
        gx_canvas_rectangle_draw(&rect);
        break;

    case POLYGON:
        gx_context_brush_define(GX_COLOR_ID_LIGHT_GRAY, GX_COLOR_ID_GREEN, brush_style);
        gx_canvas_polygon_draw(rectangle, 4);
        gx_canvas_polygon_draw(pentagon, 5);
        gx_canvas_polygon_draw(concave, 6);
        gx_canvas_polygon_draw(star, 10);
        gx_canvas_polygon_draw(self_intersection, 8);
        break;

    case ELLIPSE:
        gx_context_brush_define(GX_COLOR_ID_LIGHT_GRAY, GX_COLOR_ID_SKY_BLUE, brush_style);
        gx_canvas_ellipse_draw(xcenter, ycenter, radius, ellipse_b);

        if (radius > 50)
        {
            gx_context_brush_define(GX_COLOR_ID_LIGHT_GRAY, GX_COLOR_ID_BLACK, brush_style);
            gx_canvas_ellipse_draw(xcenter, ycenter, radius - 50, ellipse_b);
        }

        if (radius > 100)
        {
            gx_context_brush_define(GX_COLOR_ID_LIGHT_GRAY, GX_COLOR_ID_SKY_BLUE, brush_style);
            gx_canvas_ellipse_draw(xcenter, ycenter, radius - 100, ellipse_b);
        }
        break;
    }
}

/******************************************************************************************/
/* Calculate string length.                                                               */
/******************************************************************************************/
UINT string_length_get(GX_CONST GX_CHAR* input_string, UINT max_string_length)
{
    UINT length = 0;

    if (input_string)
    {
        /* Traverse the string.  */
        for (length = 0; input_string[length]; length++)
        {
            /* Check if the string length is bigger than the max string length.  */
            if (length >= max_string_length)
            {
                break;
            }
        }
    }

    return length;
}

