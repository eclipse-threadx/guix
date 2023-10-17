/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"

#include "generic_scroll_wheel_resources.h"
#include "generic_scroll_wheel_specifications.h"

#define MEMORY_SIZE (DISPLAY_1_X_RESOLUTION * DISPLAY_1_Y_RESOLUTION)
#define SCROLL_WHEEL_VISIBLE_ROWS 6
#define SCROLL_WHEEL_ICON_WIDTH   38
#define SCROLL_WHEEL_ICON_HEIGHT  38

/* Define the ThreadX demo thread control block and stack.  */

TX_THREAD          demo_thread;
UCHAR              demo_thread_stack[4096];
GX_WINDOW_ROOT    *root;
TX_BYTE_POOL       memory_pool;
GX_UBYTE           memory_buffer[MEMORY_SIZE];

/* Define vertical list row item structure. */
typedef struct {
    GX_NUMERIC_PROMPT prompt;
    GX_ICON   icon;
}SCROLL_WHEEL_ROW;

SCROLL_WHEEL_ROW  scroll_wheel_nowrap_rows[SCROLL_WHEEL_VISIBLE_ROWS + 1];
SCROLL_WHEEL_ROW  scroll_wheel_wrap_rows[SCROLL_WHEEL_VISIBLE_ROWS + 1];

/* Define prototypes.   */
VOID  demo_thread_entry(ULONG thread_input);
extern UINT win32_graphics_driver_setup_565rgb(GX_DISPLAY *display);
void widget_scroll_wheel_init();
void string_to_int(char* str, int len, int* value);

int main(int argc, char ** argv)
{
  tx_kernel_enter();
  return(0);
}


VOID* memory_allocate(ULONG size)
{
    VOID* memptr;

    if (tx_byte_allocate(&memory_pool, &memptr, size, TX_NO_WAIT) == TX_SUCCESS)
    {
        return memptr;
    }

    return NULL;
}

VOID memory_free(VOID* mem)
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
    /* create byte pool for rotate to use */
    tx_byte_pool_create(&memory_pool, "memory poll", memory_buffer, MEMORY_SIZE);

    /* Initialize GUIX.  */
    gx_system_initialize();

    gx_system_memory_allocator_set(memory_allocate, memory_free);

    gx_studio_display_configure(DISPLAY_1, win32_graphics_driver_setup_565rgb, 
                                LANGUAGE_ENGLISH, DISPLAY_1_THEME_1, &root);

    /* create the button screen */
    gx_studio_named_widget_create("main_screen", (GX_WIDGET*)root, GX_NULL);

    widget_scroll_wheel_init();

    /* Show the root window to make it and patients screen visible.  */
    gx_widget_show(root);

    /* let GUIX run */
    gx_system_start();
}

UINT main_screen_event_process(GX_WINDOW* window, GX_EVENT* event_ptr)
{
    GX_CHAR* input_string;
    UINT string_size;
    UINT value;

    switch (event_ptr->gx_event_type)
    {
    case GX_SIGNAL(ID_SET_TOTAL_ROWS, GX_EVENT_CLICKED):
        gx_single_line_text_input_buffer_get(&main_screen.main_screen_text_input_total_row, &input_string, &string_size, GX_NULL);
        string_to_int(input_string, string_size, &value);

        gx_generic_scroll_wheel_total_rows_set(&main_screen.main_screen_scroll_wheel, value);
        gx_generic_scroll_wheel_total_rows_set(&main_screen.main_screen_scroll_wheel_nowrap, value);
        gx_generic_scroll_wheel_total_rows_set(&main_screen.main_screen_scroll_wheel_wrap, value);
        break;

    case GX_SIGNAL(ID_SET_SELECTED_ROW, GX_EVENT_CLICKED):
        gx_single_line_text_input_buffer_get(&main_screen.main_screen_text_input_selected_row, &input_string, &string_size, GX_NULL);
        string_to_int(input_string, string_size, &value);

        gx_scroll_wheel_selected_set((GX_SCROLL_WHEEL *)&main_screen.main_screen_scroll_wheel, value);
        gx_scroll_wheel_selected_set((GX_SCROLL_WHEEL *)&main_screen.main_screen_scroll_wheel_nowrap, value);
        gx_scroll_wheel_selected_set((GX_SCROLL_WHEEL *)&main_screen.main_screen_scroll_wheel_wrap, value);
        break;

    default:
        return gx_window_event_process(window, event_ptr);
    }

    return 0;
}

void custom_prompt_draw(GX_NUMERIC_PROMPT* prompt)
{

    /* draw widget background. */
    gx_widget_background_draw((GX_WIDGET*)prompt);

    if (prompt->gx_widget_style & GX_STYLE_DRAW_SELECTED)
    {
        prompt->gx_prompt_font_id = GX_FONT_ID_SELECTED;
    }
    else
    {
        prompt->gx_prompt_font_id = GX_FONT_ID_NORMAL;
    }

    /* draw the text. */
    gx_prompt_text_draw((GX_PROMPT *)prompt);

    /* Draw children widgets of prompt widget.  */
    gx_widget_children_draw((GX_WIDGET*)prompt);
}

void widget_scroll_callback(GX_GENERIC_SCROLL_WHEEL* wheel, GX_WIDGET* widget, INT index)
{
    GX_BOOL created;
    GX_RECTANGLE size;
    SCROLL_WHEEL_ROW* row = (SCROLL_WHEEL_ROW*)widget;
    int width;

    gx_widget_created_test(widget, &created);

    if (!created)
    {
        gx_window_client_width_get((GX_WINDOW *)wheel, &width);

        gx_utility_rectangle_define(&size, 0, 0, width - 1, wheel->gx_scroll_wheel_row_height - 1);
        gx_numeric_prompt_create((GX_NUMERIC_PROMPT*)&row->prompt, "", wheel, 0, GX_STYLE_ENABLED | GX_STYLE_TRANSPARENT, 0, &size);

        size.gx_rectangle_top = (size.gx_rectangle_top + size.gx_rectangle_bottom - SCROLL_WHEEL_ICON_HEIGHT) >> 1;
        size.gx_rectangle_left = size.gx_rectangle_right - SCROLL_WHEEL_ICON_WIDTH - 1;

        gx_icon_create(&row->icon, "", &row->prompt, GX_PIXELMAP_ID_I_PATIENTLIST_LG, GX_STYLE_TRANSPARENT, 0, size.gx_rectangle_left, size.gx_rectangle_top);

        gx_widget_draw_set(widget, custom_prompt_draw);
        gx_prompt_font_set((GX_PROMPT*)&row->prompt, GX_FONT_ID_NORMAL);
        gx_prompt_text_color_set((GX_PROMPT*)&row->prompt, GX_COLOR_ID_TEXT, GX_COLOR_ID_SELECTED, GX_COLOR_ID_DISABLED_TEXT);
    }

    gx_numeric_prompt_value_set((GX_NUMERIC_PROMPT*)&row->prompt, index);
}

void widget_scroll_wheel_init()
{
    INT index = 0;
    GX_GENERIC_SCROLL_WHEEL *nowrap_wheel = &main_screen.main_screen_scroll_wheel_nowrap;
    GX_GENERIC_SCROLL_WHEEL *wrap_wheel = &main_screen.main_screen_scroll_wheel_wrap;

    while (index <= SCROLL_WHEEL_VISIBLE_ROWS)
    {
        widget_scroll_callback(nowrap_wheel, &scroll_wheel_nowrap_rows[index], index);
        widget_scroll_callback(wrap_wheel, &scroll_wheel_wrap_rows[index], index);

        index++;
    }

    gx_scroll_wheel_speed_set(nowrap_wheel,
        GX_FIXED_VAL_MAKE(1),     /* The rate of start speed to flick speed. */
        GX_FIXED_VAL_MAKE(1) / 5, /* The rate of end speed to flick speed. */
        10,                       /* Max steps fro scrolling. */
        2);                       /* Delay time of each step. */

    gx_scroll_wheel_speed_set(wrap_wheel, GX_FIXED_VAL_MAKE(1), GX_FIXED_VAL_MAKE(1) / 2, 10, 2);
}

/******************************************************************************************/
/* Convert string to integer value.                                                       */
/******************************************************************************************/
void string_to_int(char* str, int len, int* value)
{
    int index;

    *value = 0;

    for (index = 0; index < len; index++)
    {
        *value = *value * 10;
        *value += str[index] - '0';
    }
}
