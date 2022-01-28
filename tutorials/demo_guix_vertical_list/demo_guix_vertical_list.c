/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"
#include "demo_guix_vertical_list_resources.h"
#include "demo_guix_vertical_list_specifications.h"

#define LIST_ROWS                 20
#define LIST_VISIBLE_ROWS         10
#define LIST_ROW_HEIGHT           36
#define LIST_ROW_TEXT_BUFFER_SIZE 10

/* Define vertical list row item structure. */
typedef struct {
    GX_PROMPT prompt;
    GX_CHAR   text[LIST_ROW_TEXT_BUFFER_SIZE];
}LIST_ROW;

/* Define variables.  */
GX_WINDOW_ROOT    *root;
LIST_ROW          list_children[LIST_VISIBLE_ROWS + 1];
LIST_ROW          wrap_list_children[LIST_VISIBLE_ROWS + 1];

/* Define prototypes.   */
VOID start_guix(VOID);
extern UINT win32_graphics_driver_setup_24xrgb(GX_DISPLAY *display);
void vertical_list_children_create();
void wrap_style_vertical_list_children_create();
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
/* Define tx_application_define function.                                                 */
/******************************************************************************************/
VOID tx_application_define(void *first_unused_memory)
{
    start_guix();
}

/******************************************************************************************/
/* Initiate and run GUIX.                                                                 */
/******************************************************************************************/
VOID start_guix(VOID)
{
    /* Initialize GUIX.  */
    gx_system_initialize();

    /* Configure display. */
    gx_studio_display_configure(DISPLAY_1, win32_graphics_driver_setup_24xrgb,
                                LANGUAGE_ENGLISH, DISPLAY_1_THEME_1, &root);

    /* create the main screen */
    gx_studio_named_widget_create("main_screen", (GX_WIDGET *) root, GX_NULL);

    /* Create list children. */
    vertical_list_children_create();
    wrap_style_vertical_list_children_create();

    /* Show the root window to make it and patients screen visible.  */
    gx_widget_show(root);

    /* let GUIX run */
    gx_system_start();
}

/******************************************************************************************/
/* Convert string to integer value.                                                       */
/******************************************************************************************/
static void string_to_int(char *str, int len, int *value)
{
int index;
    
    *value = 0;

    for (index = 0; index < len; index++)
    {
        *value = *value * 10;
        *value += str[index] - '0';
    }
}

/******************************************************************************************/
/* Set page index for vertical lists.                                                     */
/******************************************************************************************/
static void set_page_index()
{
    GX_SINGLE_LINE_TEXT_INPUT *input;
    GX_CHAR *input_string;
    INT string_size;
    INT index;

    input = &main_screen.main_screen_text_input_page_index;
    gx_single_line_text_input_buffer_get(input, &input_string, (UINT *)&string_size, GX_NULL);
    string_to_int(input_string, string_size, &index);

    if ((index >= 0) && (index < LIST_ROWS))
    {
        gx_vertical_list_page_index_set(&main_screen.main_screen_vertical_list_scroll, index);
        gx_vertical_list_page_index_set(&main_screen.main_screen_vertical_list_wrap, index);
    }
}

/******************************************************************************************/
/* Set selected index for vertical lists.                                                 */
/******************************************************************************************/
static void set_selected_index()
{
    GX_SINGLE_LINE_TEXT_INPUT *input;
    GX_CHAR *input_string;
    int string_size;
    int index;

    input = &main_screen.main_screen_text_input_selected_set;
    gx_single_line_text_input_buffer_get(input, &input_string, (UINT *)&string_size, GX_NULL);
    string_to_int(input_string, string_size, &index);

    if ((index >= 0) && (index < LIST_ROWS))
    {
        gx_vertical_list_selected_set(&main_screen.main_screen_vertical_list_scroll, index);
        gx_vertical_list_selected_set(&main_screen.main_screen_vertical_list_wrap, index);
    }
}

/******************************************************************************************/
/* Set total rows for vertical lists.                                                     */
/******************************************************************************************/
static void set_total_rows()
{
    GX_SINGLE_LINE_TEXT_INPUT* input;
    GX_CHAR* input_string;
    int string_size;
    int index;

    input = &main_screen.main_screen_text_input_total_rows_set;
    gx_single_line_text_input_buffer_get(input, &input_string, (UINT*)&string_size, GX_NULL);
    string_to_int(input_string, string_size, &index);

    if (index >= 0)
    {
        gx_vertical_list_total_rows_set(&main_screen.main_screen_vertical_list_scroll, index);
        gx_vertical_list_total_rows_set(&main_screen.main_screen_vertical_list_wrap, index);
    }
}

/******************************************************************************************/
/* Override the default event processing of "main_screen" to handle signals from my child */
/* widgets.                                                                               */
/******************************************************************************************/
UINT main_screen_event_handler(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    switch(event_ptr->gx_event_type)
    {
    case GX_SIGNAL(ID_BUTTON_PAGE_INDEX_SET, GX_EVENT_CLICKED):
        set_page_index();
        break;

    case GX_SIGNAL(ID_BUTTON_SELECTED_SET, GX_EVENT_CLICKED):
        set_selected_index();
        break;

    case GX_SIGNAL(ID_BUTTON_TOTAL_ROWS_SET, GX_EVENT_CLICKED):
        set_total_rows();
        break;
    }

    return gx_window_event_process(window, event_ptr);
}

/*****************************************************************************/
/* Create one item for the specified vertical list.                          */
/*****************************************************************************/
void vertical_list_row_create(GX_VERTICAL_LIST *list, GX_WIDGET *widget, INT index)
{
    GX_BOOL is_created;
    GX_RECTANGLE size;
    LIST_ROW *row = (LIST_ROW *)widget;
    GX_RESOURCE_ID bkgnd_color;
    GX_STRING string;

    if (index & 1)
    {
        bkgnd_color = GX_COLOR_ID_WINDOW_FILL;
    }
    else
    {
        bkgnd_color = GX_COLOR_ID_ODD_ROW;
    }

    /* Test if the widget have already been created. */
    gx_widget_created_test(&row->prompt, &is_created);

    if (!is_created)
    {
        size.gx_rectangle_top = size.gx_rectangle_left = size.gx_rectangle_right = 0;
        size.gx_rectangle_bottom = size.gx_rectangle_top + LIST_ROW_HEIGHT - 1;

        gx_prompt_create(&row->prompt, GX_NULL, list, 0, GX_STYLE_ENABLED, 0, &size);
    }

    /* Initiate row text. */
    gx_utility_ltoa(index, row->text, LIST_ROW_TEXT_BUFFER_SIZE);

    /* Set row text to prompt. */
    string.gx_string_ptr = row->text;
    string.gx_string_length = string_length_get(row->text, LIST_ROW_TEXT_BUFFER_SIZE - 1);
    gx_prompt_text_set_ext(&row->prompt, &string);

    /* Set text color. */
    gx_prompt_text_color_set(&row->prompt, GX_COLOR_ID_OFF_WHITE, GX_COLOR_ID_BLACK, GX_COLOR_ID_OFF_WHITE);

    /* Set fill color. */
    gx_widget_fill_color_set(&row->prompt, bkgnd_color, GX_COLOR_ID_GREEN, bkgnd_color);
}

/*****************************************************************************/
/* Create vertical list row items.                                           */
/*****************************************************************************/
void vertical_list_children_create()
{
    INT count;
    GX_VERTICAL_LIST *list = &main_screen.main_screen_vertical_list_scroll;

    for (count = 0; count <= LIST_VISIBLE_ROWS; count++)
    {
        /* Create one row item. */
        vertical_list_row_create(list, (GX_WIDGET *)&list_children[count], count);
    }
}

/*****************************************************************************/
/* Create wrap style vertical list row items.                                */
/*****************************************************************************/
void wrap_style_vertical_list_children_create()
{
    INT count;
    GX_VERTICAL_LIST *list = &main_screen.main_screen_vertical_list_wrap;

    for (count = 0; count <= LIST_VISIBLE_ROWS; count++)
    {
        /* Create one row item. */
        vertical_list_row_create(list, (GX_WIDGET *)&wrap_list_children[count], count);
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
