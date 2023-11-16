/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include <string.h>
#include "gx_api.h"

#include "lines_8bpp_resources.h"
#include "lines_8bpp_specifications.h"

#define DROP_LIST_VISIBLE_ROWS 5
#define LINE_LENGTH 100

MAIN_WINDOW_CONTROL_BLOCK *pMainWin;
GX_WINDOW *pLineWin;
int        line_angle = 0;
int        line_width = 1;
GX_BOOL    anti_aliased = GX_FALSE;
GX_BOOL    rounded = GX_FALSE;
GX_RESOURCE_ID line_color = GX_COLOR_ID_CANVAS;
int        line_x1;
int        line_x2;
int        line_y1;
int        line_y2;
typedef struct COLOR_LIST_STRUCT{
    GX_CHAR color[256];
    GX_RESOURCE_ID color_id;
}COLOR_LIST;

COLOR_LIST color_list[]={
    {"BLACK", GX_COLOR_ID_CANVAS},
    {"WHITE", GX_COLOR_ID_SELECTED_TEXT},
    {"ORANGE", GX_COLOR_ID_SCROLL_BUTTON},
    {"YELLOW", GX_COLOR_ID_BTN_LOWER},
    {"LIGHTGRAY", GX_COLOR_ID_TEXT_INPUT_FILL}
};

GX_PROMPT drop_list_widgets[DROP_LIST_VISIBLE_ROWS + 1];
GX_SCROLLBAR list_scroll;

/* Define the ThreadX demo thread control block and stack.  */

TX_THREAD          demo_thread;
UCHAR              demo_thread_stack[4096];

GX_WINDOW_ROOT    *root;

/* Define prototypes.   */
VOID  PopulateDropList();
VOID  demo_thread_entry(ULONG thread_input);
extern UINT win32_graphics_driver_setup_8bit_palette(GX_DISPLAY *display);

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


    gx_studio_display_configure(DISPLAY_1, win32_graphics_driver_setup_8bit_palette, 
                                LANGUAGE_ENGLISH, DISPLAY_1_DEFAULT_THEME, &root);

    /* create the main screen */
    gx_studio_named_widget_create("main_window", (GX_WIDGET *) root, (GX_WIDGET **) &pMainWin);
    PopulateDropList();

    /* get a pointer to line window */
    pLineWin = &pMainWin->main_window_line_window;

    /* Show the root window to make it and patients screen visible.  */
    gx_widget_show(root);

    /* let GUIX run */
    gx_system_start();
}

VOID calculate_line_ends(VOID)
{
    INT scaled_angle;
    INT x_dist;
    INT y_dist;

    INT x_center = (pLineWin->gx_widget_size.gx_rectangle_left + pLineWin->gx_widget_size.gx_rectangle_right) / 2;
    INT y_center = (pLineWin->gx_widget_size.gx_rectangle_top + pLineWin->gx_widget_size.gx_rectangle_bottom) / 2;

    scaled_angle = GX_FIXED_VAL_MAKE(line_angle);
    x_dist = GX_FIXED_VAL_TO_INT(gx_utility_math_cos(scaled_angle) * LINE_LENGTH);
    y_dist = GX_FIXED_VAL_TO_INT((gx_utility_math_sin(scaled_angle) * LINE_LENGTH));

    line_x1 = x_center - x_dist;
    line_x2 = x_center + x_dist;
    line_y1 = y_center - y_dist;
    line_y2 = y_center + y_dist;
}

VOID update_width_prompt(VOID)
{
GX_PROMPT *pp;
static GX_CHAR width_val[10];
GX_STRING str;

    gx_widget_find(pMainWin, ID_WIDTH_DISPLAY, 0, &pp);

    if (pp)
    {
        gx_utility_ltoa(line_width, width_val, 10);
        str.gx_string_ptr = width_val;
        str.gx_string_length = strnlen(width_val, sizeof(width_val));
        gx_prompt_text_set_ext(pp, &str); 
    }
}

VOID update_angle_prompt(VOID)
{
GX_PROMPT *pp;
static GX_CHAR angle_val[10];
GX_STRING str;

    gx_widget_find(pMainWin, ID_ANGLE_DISPLAY, 0, &pp);
    
    if (pp)
    {
        gx_utility_ltoa(line_angle, angle_val, 10);
        str.gx_string_ptr = angle_val;
        str.gx_string_length = strnlen(angle_val, sizeof(angle_val));
        gx_prompt_text_set_ext(pp, &str); 
    }
}

UINT main_event_handler(GX_WINDOW *window, GX_EVENT *myevent)
{
UINT status = 0;
GX_VERTICAL_LIST *list;
GX_DROP_LIST *drop;
GX_PROMPT *prompt;

    switch(myevent -> gx_event_type)
    {
    case GX_SIGNAL(ID_LINE_WIDTH, GX_EVENT_SLIDER_VALUE):
        line_width = myevent ->gx_event_payload.gx_event_longdata;
        update_width_prompt();
        gx_system_dirty_mark(pLineWin);
        break;

    case GX_SIGNAL(ID_LINE_ANGLE, GX_EVENT_SLIDER_VALUE):
        line_angle = myevent ->gx_event_payload.gx_event_longdata;
        update_angle_prompt();
        calculate_line_ends();
        gx_system_dirty_mark(pLineWin);
        break;

    case GX_SIGNAL(ID_ANTI_ALIASED, GX_EVENT_TOGGLE_ON):
        anti_aliased = GX_TRUE;
        gx_system_dirty_mark(pLineWin);
        break;

    case GX_SIGNAL(ID_ANTI_ALIASED, GX_EVENT_TOGGLE_OFF):
        anti_aliased = GX_FALSE;
        gx_system_dirty_mark(pLineWin);
        break;

    case GX_SIGNAL(ID_ROUNDED, GX_EVENT_TOGGLE_ON):
        rounded = GX_TRUE;
        gx_system_dirty_mark(pLineWin);
        break;

    case GX_SIGNAL(ID_ROUNDED, GX_EVENT_TOGGLE_OFF):
        rounded = GX_FALSE;
        gx_system_dirty_mark(pLineWin);
        break;

    case GX_SIGNAL(ID_COLOR_LIST, GX_EVENT_LIST_SELECT):
        drop = &pMainWin->main_window_color_list;
        gx_drop_list_popup_get(drop, &list);

        gx_vertical_list_selected_widget_get(list, (GX_WIDGET **)&prompt);

        if (prompt)
        {
            line_color = prompt->gx_prompt_normal_text_color;
            gx_system_dirty_mark(pLineWin);
        }
        break;

    default:
        status = gx_window_event_process(window, myevent);
        break;
    }
    return status;
}



VOID line_win_draw(GX_WINDOW *window)
{
ULONG brush_style = 0;

    gx_window_draw((GX_WINDOW*) window);

    if (anti_aliased)
    {
        brush_style |= GX_BRUSH_ALIAS;
    }
    if (rounded)
    {
        brush_style |= GX_BRUSH_ROUND;
    }
    gx_context_brush_define(line_color, window ->gx_widget_normal_fill_color, brush_style);
    gx_context_brush_width_set(line_width);

    calculate_line_ends();
    gx_canvas_line_draw(line_x1, line_y1, line_x2, line_y2);
}

VOID drop_list_color_create(GX_VERTICAL_LIST *list, GX_WIDGET *widget, INT index)
{
GX_BOOL created;
GX_RECTANGLE size;
GX_STRING str;

    GX_PROMPT *prompt = (GX_PROMPT *)widget;

    gx_widget_created_test(widget, &created);

    if(!created)
    {
        gx_utility_rectangle_define(&size, 0, 0, 100, 30);
        gx_prompt_create(prompt, GX_NULL, list, 0, GX_STYLE_ENABLED|GX_STYLE_TEXT_LEFT|GX_STYLE_BORDER_NONE, 0, &size);
        gx_prompt_text_color_set(prompt, color_list[index].color_id, color_list[index].color_id, color_list[index].color_id);
    }

    str.gx_string_ptr = color_list[index].color;
    str.gx_string_length = strnlen(color_list[index].color, sizeof(color_list[index].color));
    gx_prompt_text_set_ext(prompt, &str);
}

VOID PopulateDropList(void)
{
    int index;
    GX_VERTICAL_LIST *list;

    GX_DROP_LIST *drop = &pMainWin->main_window_color_list;
    gx_drop_list_popup_get(drop, &list);

    for(index = 0; index < DROP_LIST_VISIBLE_ROWS; index++)
    {
        drop_list_color_create(list, (GX_WIDGET *)&drop_list_widgets[index], index);
    }

    gx_vertical_scrollbar_create(&list_scroll, "list_scroll", list, GX_NULL,
        GX_SCROLLBAR_VERTICAL | GX_SCROLLBAR_RELATIVE_THUMB | GX_SCROLLBAR_END_BUTTONS);
}
