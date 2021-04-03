/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"
#include "demo_guix_scroll_wheel_resources.h"
#include "demo_guix_scroll_wheel_specifications.h"

#define MEMORY_POOL_SIZE (DISPLAY_1_X_RESOLUTION * DISPLAY_1_Y_RESOLUTION)

/* Define variables.  */
TX_BYTE_POOL       memory_pool;
GX_COLOR           memory_pool_buffer[MEMORY_POOL_SIZE];
GX_WINDOW_ROOT    *root;

/* Define prototypes.   */
VOID start_guix(VOID);
extern UINT win32_graphics_driver_setup_24xrgb(GX_DISPLAY *display);
VOID scrolling_speed_set();
VOID selected_day_set();
VOID day_wheel_range_set();
UINT string_length_get(GX_CONST GX_CHAR* input_string, UINT max_string_length);

/******************************************************************************************/
/* Application entry.                                                                     */
/******************************************************************************************/
int main(int argc, char ** argv)
{
    tx_kernel_enter();
    return(0);
}

/******************************************************************************************/
/* Application defined memory allocate function.                                          */
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
/* Application defined memory free function.                                              */
/******************************************************************************************/
VOID memory_free(VOID *mem)
{
    tx_byte_release(mem);
}

/******************************************************************************************/
/* Define tx_application_define function.                                                 */
/******************************************************************************************/
VOID tx_application_define(void *first_unused_memory)
{
    /* create byte pool for rotate to use */
    tx_byte_pool_create(&memory_pool, "memory_pool", memory_pool_buffer, MEMORY_POOL_SIZE * sizeof(GX_COLOR));

    start_guix();
}

/******************************************************************************************/
/* Initiate and run GUIX.                                                                 */
/******************************************************************************************/
VOID start_guix(VOID)
{
    /* Initialize GUIX.  */
    gx_system_initialize();

    /* install our memory allocator and de-allocator */
    gx_system_memory_allocator_set(memory_allocate, memory_free);

    /* Configure display. */
    gx_studio_display_configure(DISPLAY_1, win32_graphics_driver_setup_24xrgb,
                                LANGUAGE_ENGLISH, DISPLAY_1_THEME_1, &root);

    /* create the main screen */
    gx_studio_named_widget_create("main_screen", (GX_WIDGET *) root, GX_NULL);

    scrolling_speed_set();

    /* Show the root window to make it and patients screen visible.  */
    gx_widget_show(root);

    /* let GUIX run */
    gx_system_start();
}

/******************************************************************************************/
/* Override the default event processing of "main_screen" to handle signals from my child */
/* widgets.                                                                               */
/******************************************************************************************/
UINT main_screen_event_process(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
    case GX_SIGNAL(ID_ROUNED_CHECK, GX_EVENT_TOGGLE_ON):
        /* Add round style. */
        gx_widget_style_add(&main_screen.main_screen_scroll_wheel_day, GX_STYLE_TEXT_SCROLL_WHEEL_ROUND);
        gx_widget_style_add(&main_screen.main_screen_scroll_wheel_month, GX_STYLE_TEXT_SCROLL_WHEEL_ROUND);
        break;

    case GX_SIGNAL(ID_ROUNED_CHECK, GX_EVENT_TOGGLE_OFF):
        /* Remove round style. */
        gx_widget_style_remove(&main_screen.main_screen_scroll_wheel_day, GX_STYLE_TEXT_SCROLL_WHEEL_ROUND);
        gx_widget_style_remove(&main_screen.main_screen_scroll_wheel_month, GX_STYLE_TEXT_SCROLL_WHEEL_ROUND);
        break;

    case GX_SIGNAL(ID_WRAP_CHECK, GX_EVENT_TOGGLE_ON):
        /* Add wrap style. */
        gx_widget_style_add(&main_screen.main_screen_scroll_wheel_day, GX_STYLE_WRAP);
        gx_widget_style_add(&main_screen.main_screen_scroll_wheel_month, GX_STYLE_WRAP);
        break;

    case GX_SIGNAL(ID_WRAP_CHECK, GX_EVENT_TOGGLE_OFF):
        /* Remove wrap style. */
        gx_widget_style_remove(&main_screen.main_screen_scroll_wheel_day, GX_STYLE_WRAP);
        gx_widget_style_remove(&main_screen.main_screen_scroll_wheel_month, GX_STYLE_WRAP);
        break;

    case GX_SIGNAL(ID_SELECTED_DAY_SET, GX_EVENT_CLICKED):
        /* Set selected day. */
        selected_day_set();
        break;

    case GX_SIGNAL(ID_SCROLL_WHEEL_DAY, GX_EVENT_LIST_SELECT):
        /* Update current selected row index of day wheel. */
        gx_numeric_prompt_value_set(&main_screen.main_screen_selected_day_prompt, event_ptr->gx_event_payload.gx_event_longdata);
        break;

    case GX_SIGNAL(ID_SCROLL_WHEEL_MONTH, GX_EVENT_LIST_SELECT):
        /* Update current selecrted row index of month wheel. */
        gx_numeric_prompt_value_set(&main_screen.main_screen_selected_month_prompt, event_ptr->gx_event_payload.gx_event_longdata);
        break;

    case GX_SIGNAL(ID_RANGE_SET, GX_EVENT_CLICKED):
        /* Set day wheel range. */
        day_wheel_range_set();
        break;

    default:
        return gx_window_event_process(window, event_ptr);
    }
    return 0;
}

/******************************************************************************************/
/* Row value format callback of day wheel.                                                */
/******************************************************************************************/
UINT day_wheel_value_format(GX_NUMERIC_SCROLL_WHEEL *wheel, INT row, GX_STRING *string)
{
    sprintf(wheel->gx_numeric_scroll_wheel_string_buffer, "%02d", row + 1);

    string->gx_string_ptr = wheel->gx_numeric_scroll_wheel_string_buffer;
    string->gx_string_length = string_length_get(string->gx_string_ptr, GX_NUMERIC_PROMPT_BUFFER_SIZE - 1);

    return GX_SUCCESS;
}

/******************************************************************************************/
/* Convert string to integer value.                                                       */
/******************************************************************************************/
static void string_to_int(char *str, int len, int *value)
{
    int index = 0;
    int flag = 1;

    *value = 0;

    if (str[0] == '-')
    {
        flag = -1;
        index = 1;
    }

    while(index < len)
    {
        *value = *value * 10;
        *value += str[index] - '0';
        index++;
    }

    *value = (*value) * flag;
}

/******************************************************************************************/
/* Set selected day.                                                                      */
/******************************************************************************************/
VOID selected_day_set()
{
    GX_SINGLE_LINE_TEXT_INPUT *day_input;
    GX_CHAR  *input_buffer;
    UINT     string_size;
    INT      value;

    day_input = &main_screen.main_screen_day_input;

    /* Pick up buffer address and input string size. */
    gx_single_line_text_input_buffer_get(day_input, &input_buffer, &string_size, GX_NULL);

    /* Convert string to integer. */
    string_to_int(input_buffer, string_size, &value);

    /* Set selected day. */
    gx_scroll_wheel_selected_set(&main_screen.main_screen_scroll_wheel_day, value - 1);
}

/******************************************************************************************/
/* Set day wheel range.                                                                   */
/******************************************************************************************/
VOID day_wheel_range_set()
{
    GX_CHAR  *input_buffer;
    UINT     string_size;
    INT      start_val;
    INT      end_val;

    /* Pick up buffer address and input string size. */
    gx_single_line_text_input_buffer_get(&main_screen.main_screen_day_startval_input, &input_buffer, &string_size, GX_NULL);

    /* Convert string to integer. */
    string_to_int(input_buffer, string_size, &start_val);

    /* Pick up buffer address and input string size. */
    gx_single_line_text_input_buffer_get(&main_screen.main_screen_day_endval_input, &input_buffer, &string_size, GX_NULL);

    /* Convert string to integer. */
    string_to_int(input_buffer, string_size, &end_val);

    /* Set day wheel range. */
    gx_numeric_scroll_wheel_range_set(&main_screen.main_screen_scroll_wheel_day, start_val, end_val);
}

/******************************************************************************************/
/* Set scrolling speed.                                                                   */
/******************************************************************************************/
VOID scrolling_speed_set()
{
    GX_NUMERIC_SCROLL_WHEEL *day_wheel = &main_screen.main_screen_scroll_wheel_day;
    GX_STRING_SCROLL_WHEEL *month_wheel = &main_screen.main_screen_scroll_wheel_month;

    gx_scroll_wheel_speed_set(day_wheel,
                              GX_FIXED_VAL_MAKE(1),     /* The rate of start speed to flick speed. */
                              GX_FIXED_VAL_MAKE(1) / 5, /* The rate of end speed to flick speed. */
                              10,                       /* Max steps fro scrolling. */
                              2);                       /* Delay time of each step. */

    gx_scroll_wheel_speed_set(month_wheel, GX_FIXED_VAL_MAKE(1) / 2, 200, 10, 2);
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
