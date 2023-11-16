/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"

#include "lines_resources.h"
#include "lines_specifications.h"


#define DROP_LIST_VISIBLE_ROWS 5
#define LINE_WINDOW_LINE_LENGTH 80
#define LINE_CENTER_X_OFFSET 159
#define LINE_CENTER_Y_OFFSET 159

#define MAX_COLOR_TEXT_LENGTH 10

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
    GX_CHAR color[MAX_COLOR_TEXT_LENGTH + 1] ;
    GX_RESOURCE_ID color_id ;
}COLOR_LIST;

COLOR_LIST color_list[] = {
    { "BLACK", GX_COLOR_ID_CANVAS },
    { "WHITE", GX_COLOR_ID_SELECTED_TEXT },
    { "ORANGE", GX_COLOR_ID_SCROLL_BUTTON },
    { "YELLOW", GX_COLOR_ID_BTN_LOWER },
    { "LIGHTGRAY", GX_COLOR_ID_TEXT_INPUT_FILL }
};

GX_PROMPT    drop_list_widgets[DROP_LIST_VISIBLE_ROWS + 1];
GX_SCROLLBAR list_scroll;
GX_UBYTE     brush_alpha = 255;

#define LINE_LENGTH 100

/* Define the ThreadX demo thread control block and stack.  */

GX_WINDOW_ROOT    *root;

/* Define prototypes.   */
VOID  PopulateDropList(void);
VOID  start_guix(VOID);
extern UINT win32_graphics_driver_setup_24xrgb(GX_DISPLAY *display);
UINT string_length_get(GX_CONST GX_CHAR* input_string, UINT max_string_length);

int main(int argc, char ** argv)
{
  tx_kernel_enter();
  return(0);
}


VOID tx_application_define(void *first_unused_memory)
{
    start_guix();
}


VOID  start_guix(VOID)
{
    /* Initialize GUIX.  */
    gx_system_initialize();


    gx_studio_display_configure(DISPLAY_1, win32_graphics_driver_setup_24xrgb, 
                                LANGUAGE_ENGLISH, DISPLAY_1_DEFAULT_THEME, &root);

    /* create the main screen */
    gx_studio_named_widget_create("main_window", (GX_WIDGET *) root, (GX_WIDGET **) &pMainWin);
    PopulateDropList();

    /* get a pointer to line window */
    pLineWin = &pMainWin->main_window_line_window;

    /* Show the root window to make it and patients screen visible.  */
    gx_widget_show(root);

    /* start GUIX thread */
    gx_system_start();
}

VOID calculate_line_ends(VOID)
{
    GX_FIXED_VAL scaled_angle;
    INT x_dist;
    INT y_dist;

    INT x_center = pLineWin->gx_widget_size.gx_rectangle_left + LINE_CENTER_X_OFFSET;
    INT y_center = pLineWin->gx_widget_size.gx_rectangle_top + LINE_CENTER_Y_OFFSET;

    scaled_angle = GX_FIXED_VAL_MAKE(line_angle);
    x_dist = gx_utility_math_cos(scaled_angle) * LINE_LENGTH;
    y_dist = gx_utility_math_sin(scaled_angle) * LINE_LENGTH;
    x_dist = GX_FIXED_VAL_TO_INT(x_dist);
    y_dist = GX_FIXED_VAL_TO_INT(y_dist);

    line_x1 = x_center - x_dist;
    line_x2 = x_center + x_dist;
    line_y1 = y_center - y_dist;
    line_y2 = y_center + y_dist;
}

VOID update_prompt(GX_RESOURCE_ID id, INT value)
{
GX_PROMPT *pp;
static GX_CHAR val[10];
GX_STRING string;

    gx_widget_find(pMainWin, (USHORT)id, 0, &pp);
    
    if (pp)
    {
        gx_utility_ltoa(value, val, 10);
        string.gx_string_ptr = val;
        string.gx_string_length = string_length_get(val, sizeof(val) - 1);
        gx_prompt_text_set_ext(pp, &string);
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
    case GX_SIGNAL(ID_BRUSH_ALPHA_SLIDER, GX_EVENT_SLIDER_VALUE):
        brush_alpha = (GX_UBYTE)myevent->gx_event_payload.gx_event_longdata;
        update_prompt(ID_BRUSH_ALPHA_PROMPT, brush_alpha);
        gx_system_dirty_mark(pLineWin);
        break;

    case GX_SIGNAL(ID_LINE_WIDTH, GX_EVENT_SLIDER_VALUE):
        line_width = myevent ->gx_event_payload.gx_event_longdata;
        update_prompt(ID_WIDTH_DISPLAY, line_width);
        gx_system_dirty_mark(pLineWin);
        break;

    case GX_SIGNAL(ID_LINE_ANGLE, GX_EVENT_SLIDER_VALUE):
        line_angle = myevent ->gx_event_payload.gx_event_longdata;
        update_prompt(ID_ANGLE_DISPLAY, line_angle);
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
GX_BRUSH *brush;

    gx_window_draw((GX_WINDOW*) window);
    gx_context_brush_get(&brush);
    brush -> gx_brush_alpha = brush_alpha;

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
    GX_STRING string;

    GX_PROMPT *prompt = (GX_PROMPT *)widget;

    gx_widget_created_test(widget, &created);

    if (!created)
    {
        gx_utility_rectangle_define(&size, 0, 0, 100, 30);
        gx_prompt_create(prompt, GX_NULL, list, 0, GX_STYLE_ENABLED | GX_STYLE_TEXT_LEFT | GX_STYLE_BORDER_NONE, 0, &size);
        gx_prompt_text_color_set(prompt, color_list[index].color_id, color_list[index].color_id, color_list[index].color_id);
    }

    string.gx_string_ptr = color_list[index].color;
    string.gx_string_length = string_length_get(string.gx_string_ptr, MAX_COLOR_TEXT_LENGTH);
    gx_prompt_text_set_ext(prompt, &string);
}

VOID PopulateDropList(void)
{
    int index;
    GX_VERTICAL_LIST *list;

    GX_DROP_LIST *drop = &pMainWin->main_window_color_list;
    gx_drop_list_popup_get(drop, &list);

    for (index = 0; index < DROP_LIST_VISIBLE_ROWS; index++)
    {
        drop_list_color_create(list, (GX_WIDGET *)&drop_list_widgets[index], index);
    }

    gx_vertical_scrollbar_create(&list_scroll, "list_scroll", list, GX_NULL,
        GX_SCROLLBAR_VERTICAL | GX_SCROLLBAR_RELATIVE_THUMB | GX_SCROLLBAR_END_BUTTONS);
}

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