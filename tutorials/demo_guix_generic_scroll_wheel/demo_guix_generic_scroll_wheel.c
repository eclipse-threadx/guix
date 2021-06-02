/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"

#include "demo_guix_generic_scroll_wheel_resources.h"
#include "demo_guix_generic_scroll_wheel_specifications.h"

#define MEMORY_SIZE (DISPLAY_1_X_RESOLUTION * DISPLAY_1_Y_RESOLUTION)
#define SCROLL_WHEEL_VISIBLE_ROWS 5
#define SCROLL_WHEEL_ICON_WIDTH   38
#define SCROLL_WHEEL_ICON_HEIGHT  38

/* Define the ThreadX demo thread control block and stack.  */

TX_THREAD          demo_thread;
UCHAR              demo_thread_stack[4096];
GX_WINDOW_ROOT    *root;
TX_BYTE_POOL       memory_pool;
GX_UBYTE           memory_buffer[MEMORY_SIZE];

/* Define generic scroll wheel row item structure. */

typedef struct {
    GX_NUMERIC_PROMPT prompt;
    GX_ICON   icon;
}SCROLL_WHEEL_ROW;

SCROLL_WHEEL_ROW  scroll_wheel_nowrap_rows[SCROLL_WHEEL_VISIBLE_ROWS + 1];
SCROLL_WHEEL_ROW  scroll_wheel_wrap_rows[SCROLL_WHEEL_VISIBLE_ROWS + 1];

/* Define prototypes.   */

extern UINT win32_graphics_driver_setup_565rgb(GX_DISPLAY* display);
VOID demo_thread_entry(ULONG thread_input);
VOID generic_scroll_wheel_children_create();
UINT string_to_int(GX_CHAR* str, INT len);

/******************************************************************************************/
/* Application entry.                                                                     */
/******************************************************************************************/
int main(int argc, char ** argv)
{
  tx_kernel_enter();
  return(0);
}

/******************************************************************************************/
/* Define tx_application_define function.                                                 */
/******************************************************************************************/
VOID tx_application_define(void *first_unused_memory)
{
    /* Create the main demo thread.  */
    tx_thread_create(&demo_thread, "GUIX Demo Thread", demo_thread_entry, 
                     0,  demo_thread_stack, sizeof(demo_thread_stack), 
                     1, 1, TX_NO_TIME_SLICE, TX_AUTO_START);
}

/******************************************************************************************/
/* Demo thread entry.                                                                     */
/******************************************************************************************/
VOID  demo_thread_entry(ULONG thread_input)
{

    /* Initialize GUIX.  */
    gx_system_initialize();

    /* Configure display.  */
    gx_studio_display_configure(DISPLAY_1, win32_graphics_driver_setup_565rgb, 
                                LANGUAGE_ENGLISH, DISPLAY_1_THEME_1, &root);

    /* Create the main screen.  */
    gx_studio_named_widget_create("main_screen", (GX_WIDGET*)root, GX_NULL);

    /* Create generic scroll wheel children.  */
    generic_scroll_wheel_children_create();

    /* Show the root window.  */
    gx_widget_show(root);

    /* let GUIX run.  */
    gx_system_start();
}

/******************************************************************************************/
/* Override the default event processing of "main_screen" to handle signals from my child */
/* widgets.                                                                               */
/******************************************************************************************/
UINT main_screen_event_process(GX_WINDOW* window, GX_EVENT* event_ptr)
{
GX_CHAR *input_string;
UINT string_size;
UINT value;

    switch (event_ptr->gx_event_type)
    {
    case GX_SIGNAL(ID_SET_TOTAL_ROWS, GX_EVENT_CLICKED):
        gx_single_line_text_input_buffer_get(&main_screen.main_screen_text_input_total_row, &input_string, &string_size, GX_NULL);
        value = string_to_int(input_string, string_size);

        /* Set total rows.  */
        gx_generic_scroll_wheel_total_rows_set(&main_screen.main_screen_scroll_wheel, value);
        gx_generic_scroll_wheel_total_rows_set(&main_screen.main_screen_scroll_wheel_nowrap, value);
        gx_generic_scroll_wheel_total_rows_set(&main_screen.main_screen_scroll_wheel_wrap, value);
        break;

    case GX_SIGNAL(ID_SET_SELECTED_ROW, GX_EVENT_CLICKED):
        gx_single_line_text_input_buffer_get(&main_screen.main_screen_text_input_selected_row, &input_string, &string_size, GX_NULL);
        value = string_to_int(input_string, string_size);

        /* Set selected row.  */
        gx_scroll_wheel_selected_set((GX_SCROLL_WHEEL *)&main_screen.main_screen_scroll_wheel, value);
        gx_scroll_wheel_selected_set((GX_SCROLL_WHEEL *)&main_screen.main_screen_scroll_wheel_nowrap, value);
        gx_scroll_wheel_selected_set((GX_SCROLL_WHEEL *)&main_screen.main_screen_scroll_wheel_wrap, value);
        break;

    case GX_SIGNAL(ID_SCROLL_WHEEL, GX_EVENT_LIST_SELECT):
        gx_numeric_prompt_value_set(&main_screen.main_screen_scroll_wheel_selected_value, event_ptr->gx_event_payload.gx_event_ulongdata);
        break;

    case GX_SIGNAL(ID_SCROLL_WHEEL_NOWRAP, GX_EVENT_LIST_SELECT):
        gx_numeric_prompt_value_set(&main_screen.main_screen_scroll_wheel_nowrap_selected_value, event_ptr->gx_event_payload.gx_event_ulongdata);
        break;

    case GX_SIGNAL(ID_SCROLL_WHEEL_WRAP, GX_EVENT_LIST_SELECT):
        gx_numeric_prompt_value_set(&main_screen.main_screen_scroll_wheel_wrap_selected_value, event_ptr->gx_event_payload.gx_event_ulongdata);
        break;

    default:
        return gx_window_event_process(window, event_ptr);
    }

    return 0;
}

/******************************************************************************************/
/* Custom prompt draw function to draw text with different font when the prompt is        */
/* selected.                                                                              */
/******************************************************************************************/
VOID custom_prompt_draw(GX_NUMERIC_PROMPT* prompt)
{

    /* Draw widget background.  */
    gx_widget_background_draw((GX_WIDGET*)prompt);

    /* Set font id according to selection status.  */
    if (prompt->gx_widget_style & GX_STYLE_DRAW_SELECTED)
    {
        prompt->gx_prompt_font_id = GX_FONT_ID_SELECTED;
    }
    else
    {
        prompt->gx_prompt_font_id = GX_FONT_ID_NORMAL;
    }

    /* Draw the prompt text. */
    gx_prompt_text_draw((GX_PROMPT *)prompt);

    /* Draw children widgets of prompt widget.  */
    gx_widget_children_draw((GX_WIDGET*)prompt);
}

/******************************************************************************************/
/* Generic scroll wheel callback function to create a wheel row.                          */
/******************************************************************************************/
VOID generic_scroll_wheel_row_create(GX_GENERIC_SCROLL_WHEEL *wheel, GX_WIDGET *widget, INT index)
{
SCROLL_WHEEL_ROW *row = (SCROLL_WHEEL_ROW *)widget;
GX_BOOL           created;
GX_RECTANGLE      size;
GX_VALUE          width;

    /* Test if the widget is already created.  */
    gx_widget_created_test(widget, &created);

    if (!created)
    {

        /* Get generic scroll wheel client width.  */
        gx_window_client_width_get((GX_WINDOW *)wheel, &width);

        /* Define numeric prompt size.  */
        gx_utility_rectangle_define(&size, 0, 0, width - 1, wheel->gx_scroll_wheel_row_height - 1);

        /* Create numeric prompt.  */
        gx_numeric_prompt_create((GX_NUMERIC_PROMPT*)&row->prompt, "", wheel, 0, GX_STYLE_ENABLED | GX_STYLE_TRANSPARENT, 0, &size);

        /* Define icon size.  */
        size.gx_rectangle_top = (size.gx_rectangle_top + size.gx_rectangle_bottom - SCROLL_WHEEL_ICON_HEIGHT) >> 1;
        size.gx_rectangle_left = size.gx_rectangle_right - SCROLL_WHEEL_ICON_WIDTH - 1;

        /* Create an icon and make it the child of numeric prompt.  */
        gx_icon_create(&row->icon, "", &row->prompt, GX_PIXELMAP_ID_ROW_ICON, GX_STYLE_TRANSPARENT, 0, size.gx_rectangle_left, size.gx_rectangle_top);

        /* Set custom prompt draw function.  */
        gx_widget_draw_set((GX_WIDGET *)&row->prompt, custom_prompt_draw);

        /* Set text color.  */
        gx_prompt_text_color_set((GX_PROMPT*)&row->prompt, GX_COLOR_ID_SHADOW, GX_COLOR_ID_SELECTED_TEXT, GX_COLOR_ID_DISABLED_TEXT);
    }

    /* Update numeric prompt value with the row index.  */
    gx_numeric_prompt_value_set((GX_NUMERIC_PROMPT*)&row->prompt, index);
}

/******************************************************************************************/
/* Create children for "scroll_wheel_nowrap" and "scroll_wheel_wrap" widgets.              */
/******************************************************************************************/
VOID generic_scroll_wheel_children_create()
{
GX_GENERIC_SCROLL_WHEEL *nowrap_wheel = &main_screen.main_screen_scroll_wheel_nowrap;
GX_GENERIC_SCROLL_WHEEL *wrap_wheel = &main_screen.main_screen_scroll_wheel_wrap;
INT                     index = 0;


    for (index = 0; index <= SCROLL_WHEEL_VISIBLE_ROWS; index++)
    {
        generic_scroll_wheel_row_create(nowrap_wheel, (GX_WIDGET *) &scroll_wheel_nowrap_rows[index], index);
        generic_scroll_wheel_row_create(wrap_wheel, (GX_WIDGET *) &scroll_wheel_wrap_rows[index], index);
    }

    /* Set generic scroll wheel flick speed.  */
    gx_scroll_wheel_speed_set((GX_SCROLL_WHEEL *)nowrap_wheel,
        GX_FIXED_VAL_MAKE(1),     /* The rate of start speed to flick speed. */
        GX_FIXED_VAL_MAKE(1) / 5, /* The rate of end speed to flick speed. */
        10,                       /* Max steps for scrolling. */
        4);                       /* Delay time of each step. */

    gx_scroll_wheel_speed_set((GX_SCROLL_WHEEL *)wrap_wheel, GX_FIXED_VAL_MAKE(1), GX_FIXED_VAL_MAKE(1) / 2, 10, 4);
}

/******************************************************************************************/
/* Convert string to integer value.                                                       */
/******************************************************************************************/
UINT string_to_int(GX_CHAR *str, INT len)
{
INT index;
INT value = 0;

    for (index = 0; index < len; index++)
    {
        if (str[index] >= '0' && str[index] <= '9')
        {
            value = value * 10;
            value += str[index] - '0';
        }
    }

    return value;
}
