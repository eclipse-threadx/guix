/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"
#include "demo_guix_drop_list_resources.h"
#include "demo_guix_drop_list_specifications.h"

#define DROP_LIST_TOTAL_ROWS  10
#define MAX_ROW_TEXT_LENGTH   31

/* Define prototypes.   */
VOID populate_drop_list();
VOID start_guix(VOID);
extern UINT win32_graphics_driver_setup_24xrgb(GX_DISPLAY *display);
UINT string_length_get(GX_CONST GX_CHAR* input_string, UINT max_string_length);

/* Define drop list item. */
typedef struct {
    GX_PROMPT prompt;
    CHAR text[MAX_ROW_TEXT_LENGTH + 1];
} DROP_LIST_ITEM;

/* Define variabled.  */
GX_WINDOW_ROOT    *root;
DROP_LIST_ITEM     drop_list_items[DROP_LIST_TOTAL_ROWS];
GX_SCROLLBAR       list_scroll;

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
VOID start_guix(void)
{
    /* Initialize GUIX.  */
    gx_system_initialize();

    gx_studio_display_configure(PRIMARY, win32_graphics_driver_setup_24xrgb, 
                                LANGUAGE_ENGLISH, PRIMARY_THEME_1, &root);

    /* create the button screen */
    gx_studio_named_widget_create("main_screen", (GX_WIDGET *) root, GX_NULL);

    /* Add child widgets to the drop-down list */
    populate_drop_list();

    /* Show the root window to make it and patients screen visible.  */
    gx_widget_show(root);

    /* start the GUIX thread */
    gx_system_start();
}


/******************************************************************************************/
/* Handle drop list select event.                                                         */
/******************************************************************************************/
VOID drop_list_select_event_handler()
{
    GX_VERTICAL_LIST *list;
    INT               selected_index;
    DROP_LIST_ITEM   *selected_item;
    GX_STRING         text;

    /* Get popup list of the drop list. */
    gx_drop_list_popup_get(&main_screen.main_screen_drop_list, &list);

    /* Get the selected index of the popup list. */
    gx_vertical_list_selected_index_get(list, &selected_index);

    /* Set selected index. */
    gx_numeric_prompt_value_set(&main_screen.main_screen_selected_index, selected_index);

    /* Get the selected item of the popup list. */
    gx_vertical_list_selected_widget_get(list, (GX_WIDGET **)&selected_item);

    /* Get the text of the selected item. */
    gx_prompt_text_get_ext(&selected_item->prompt, &text);

    /* Set selected string. */
    gx_prompt_text_set_ext(&main_screen.main_screen_selected_string, &text);
}

/******************************************************************************************/
/* Override the default event processing of "main_screen" to handle signals from my child */
/* widgets.                                                                               */
/******************************************************************************************/
UINT main_screen_event_handler(GX_WINDOW *window, GX_EVENT *event_ptr)
{

    switch (event_ptr->gx_event_type)
    {
    case GX_SIGNAL(ID_DROP_LIST, GX_EVENT_LIST_SELECT):
        drop_list_select_event_handler();
        break;

    default:
        return gx_window_event_process(window, event_ptr);
    }

    return 0;
}

/*****************************************************************************/
/* Create one item for the specified popup list.                             */
/*****************************************************************************/
VOID drop_list_row_create(GX_VERTICAL_LIST *list, GX_WIDGET *widget, INT index)
{
    GX_RECTANGLE size;
    GX_CHAR temp[10];
    GX_STRING string;

    DROP_LIST_ITEM *item = (DROP_LIST_ITEM *)widget;

    strcpy(item->text, "List Entry #");
    gx_utility_ltoa(index + 1, temp, 10);
    strcat(item->text, (char*)temp);

    gx_utility_rectangle_define(&size, 0, 0, 100, 44);
    gx_prompt_create(&item->prompt, item->text, list, 0, GX_STYLE_ENABLED | GX_STYLE_TEXT_LEFT | GX_STYLE_BORDER_NONE, 0, &size);
    gx_widget_fill_color_set(&item->prompt, GX_COLOR_ID_LIGHT_SLATE_GRAY, GX_COLOR_ID_SLATE_GRAY, GX_COLOR_ID_LIGHT_SLATE_GRAY);
    gx_prompt_text_color_set(&item->prompt, GX_COLOR_ID_WHITE, GX_COLOR_ID_WHITE, GX_COLOR_ID_WHITE);

    string.gx_string_ptr = item->text;
    string.gx_string_length = string_length_get(item->text, MAX_ROW_TEXT_LENGTH);

    gx_prompt_text_set_ext(&item->prompt, &string);
}

/*****************************************************************************/
/* Create drop list items.                                                   */
/*****************************************************************************/
VOID populate_drop_list(void)
{
    INT index;
    GX_VERTICAL_LIST *list;
    GX_SCROLLBAR_APPEARANCE scrollbar_appearance;

    /* Get popup list of the drop list. */
    gx_drop_list_popup_get(&main_screen.main_screen_drop_list, &list);

    for (index = 0; index < DROP_LIST_TOTAL_ROWS; index++)
    {
        /* Create row items. */
        drop_list_row_create(list, (GX_WIDGET *)&drop_list_items[index], index);
    }

    scrollbar_appearance.gx_scroll_thumb_pixelmap = GX_PIXELMAP_ID_LIST_SCROLL_THUMB;
    scrollbar_appearance.gx_scroll_thumb_travel_min = 4;
    scrollbar_appearance.gx_scroll_thumb_travel_max = 4;
    scrollbar_appearance.gx_scroll_thumb_width = 8;
    scrollbar_appearance.gx_scroll_width = 10;

    /* Create scrollbar for popup list. */
    gx_vertical_scrollbar_create(&list_scroll, "list_scroll", list,
        &scrollbar_appearance, GX_SCROLLBAR_VERTICAL | GX_STYLE_ENABLED);
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

