/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"
#include "demo_guix_horizontal_list_resources.h"
#include "demo_guix_horizontal_list_specifications.h"

#define LIST_COLS                 20
#define LIST_VISIBLE_COLS         10
#define LIST_ROW_HEIGHT           36
#define LIST_COL_TEXT_BUFFER_SIZE 10
#define MAX_COL_TEXT_LENGTH       31

/* Define vertical list col item structure. */
typedef struct {
    GX_PROMPT prompt;
    GX_CHAR   text[MAX_COL_TEXT_LENGTH + 1];
}LIST_COL;

/* Define variables.  */
GX_WINDOW_ROOT *root;
LIST_COL        list_children[LIST_VISIBLE_COLS + 1];
LIST_COL        wrap_list_children[LIST_VISIBLE_COLS + 1];

/* Define prototypes.   */
VOID start_guix();
extern UINT win32_graphics_driver_setup_24xrgb(GX_DISPLAY *display);
void horizontal_list_children_create();
void wrap_style_horizontal_list_children_create();
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
VOID  start_guix()
{
    /* Initialize GUIX.  */
    gx_system_initialize();

    /* Configure display. */
    gx_studio_display_configure(DISPLAY_1, win32_graphics_driver_setup_24xrgb, 
                                LANGUAGE_ENGLISH, DISPLAY_1_THEME_1, &root);

    /* create the button screen */
    gx_studio_named_widget_create("main_screen", (GX_WIDGET *) root, GX_NULL);

    /* Create list children. */
    horizontal_list_children_create();
    wrap_style_horizontal_list_children_create();

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
    int string_size;
    int index;

    input = &main_screen.main_screen_text_input_page_index;
    gx_single_line_text_input_buffer_get(input, &input_string, &string_size, GX_NULL);
    string_to_int(input_string, string_size, &index);

    if ((index >= 0) && (index < LIST_COLS))
    {
        gx_horizontal_list_page_index_set(&main_screen.main_screen_horizontal_list_scroll, index);
        gx_horizontal_list_page_index_set(&main_screen.main_screen_horizontal_list_wrap, index);
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
    gx_single_line_text_input_buffer_get(input, &input_string, &string_size, GX_NULL);
    string_to_int(input_string, string_size, &index);

    if ((index >= 0) && (index < LIST_COLS))
    {
        gx_horizontal_list_selected_set(&main_screen.main_screen_horizontal_list_scroll, index);
        gx_horizontal_list_selected_set(&main_screen.main_screen_horizontal_list_wrap, index);
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
    }

    return gx_window_event_process(window, event_ptr);
}

/*****************************************************************************/
/* Create one item for the specified horizontal list.                        */
/*****************************************************************************/
void horizontal_list_col_create(GX_HORIZONTAL_LIST *list, GX_WIDGET *widget, INT index)
{
    GX_BOOL is_created;
    GX_RECTANGLE size;
    LIST_COL *col = (LIST_COL *)widget;
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

    gx_widget_created_test(&col->prompt, &is_created);

    /* Test if the widget have already been created. */
    if (!is_created)
    {
        size.gx_rectangle_left = size.gx_rectangle_top = size.gx_rectangle_bottom = 0;
        size.gx_rectangle_right = size.gx_rectangle_left + LIST_ROW_HEIGHT - 1;

        gx_prompt_create(&col->prompt, GX_NULL, list, 0, GX_STYLE_ENABLED, 0, &size);
    }

    /* Initiate row text. */
    gx_utility_ltoa(index, col->text, LIST_COL_TEXT_BUFFER_SIZE);

    /* Set row text to prompt. */
    string.gx_string_ptr = col->text;
    string.gx_string_length = string_length_get(col->text, MAX_COL_TEXT_LENGTH);
    gx_prompt_text_set_ext(&col->prompt, &string);

    /* Set text color. */
    gx_prompt_text_color_set(&col->prompt, GX_COLOR_ID_OFF_WHITE, GX_COLOR_ID_BLACK, GX_COLOR_ID_OFF_WHITE);

    /* Set fill color. */
    gx_widget_fill_color_set(&col->prompt, bkgnd_color, GX_COLOR_ID_GREEN, bkgnd_color);
}

/*****************************************************************************/
/* Create horizontal list row items.                                         */
/*****************************************************************************/
void horizontal_list_children_create()
{
    INT count;
    GX_HORIZONTAL_LIST *list = &main_screen.main_screen_horizontal_list_scroll;

    for (count = 0; count <= LIST_VISIBLE_COLS; count++)
    {
        horizontal_list_col_create(list, (GX_WIDGET *)&list_children[count], count);
    }
}

/*****************************************************************************/
/* Create wrap style horizontal list row items.                              */
/*****************************************************************************/
void wrap_style_horizontal_list_children_create()
{
    INT count;
    GX_HORIZONTAL_LIST *list = &main_screen.main_screen_horizontal_list_wrap;

    for (count = 0; count <= LIST_VISIBLE_COLS; count++)
    {
        horizontal_list_col_create(list, (GX_WIDGET *)&wrap_list_children[count], count);
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
