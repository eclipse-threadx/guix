/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"
#include "guix_drop_list_resources.h"
#include "guix_drop_list_specifications.h"

#define DROP_LIST_VISIBLE_ROWS  4
#define DROP_LIST_MAX_TEXT 80

extern UINT win32_graphics_driver_setup_24xrgb(GX_DISPLAY *display);

GX_WINDOW_ROOT    *root;
GX_SCROLLBAR       list_scroll;

/* A structure to hold control block and text for each drop-list item */
typedef struct {
    DROP_LIST_CHILD_WIN_CONTROL_BLOCK child_win;
    GX_CHAR   text[DROP_LIST_MAX_TEXT + 1];
} DROP_LIST_CHILD;

DROP_LIST_CHILD drop_list_widgets[DROP_LIST_VISIBLE_ROWS + 1];

/* Define prototypes.   */
VOID populate_drop_list();
VOID start_guix(void);

/*****************************************************************************/
/* Application entry.                                                        */
/*****************************************************************************/
int main(int argc, char ** argv)
{
  tx_kernel_enter();
  return(0);
}
/*****************************************************************************/
/* Function called by ThreadX startup to define initial system               */
/*****************************************************************************/
VOID tx_application_define(void *first_unused_memory)
{
    start_guix();
}

/*****************************************************************************/
/* Called by tx_application_define (above), configure and start GUIX         */
/*****************************************************************************/
VOID start_guix(void)
{
    /* Initialize GUIX.  */
    gx_system_initialize();

    gx_studio_display_configure(PRIMARY, win32_graphics_driver_setup_24xrgb, 
                                LANGUAGE_ENGLISH, PRIMARY_THEME_1, &root);

    /* create the button screen */
    gx_studio_named_widget_create("main_screen", (GX_WIDGET*)root, GX_NULL);

    /* Add child widgets to the drop-down list */
    populate_drop_list();

    /* Show the root window to make it and patients screen visible.  */
    gx_widget_show(root);

    /* start the GUIX thread */
    gx_system_start();
}

/*****************************************************************************/
/* Create child element for the drop list. This function is called during    */
/* initial setup to populate children, and also called as the drop-list is   */
/* scrolled to update the child elements.                                    */
/*****************************************************************************/
extern GX_STUDIO_WIDGET drop_list_child_win_define;

VOID drop_list_row_create(GX_VERTICAL_LIST *list, GX_WIDGET *widget, INT index)
{
    GX_BOOL created;
    GX_CHAR temp[10];
    GX_STRING string;

    DROP_LIST_CHILD *entry = (DROP_LIST_CHILD *) widget;
   
    strcpy(entry->text, "List Entry #");
    gx_utility_ltoa(index + 1, temp, 10);
    strcat(entry->text, (char*)temp);

    gx_widget_created_test(&entry->child_win, &created);

    if (!created)
    {
        gx_studio_widget_create((GX_BYTE *) &entry->child_win, &drop_list_child_win_define, (GX_WIDGET *) list);
    }

    string.gx_string_ptr = entry->text;
    string.gx_string_length = strnlen(entry->text, DROP_LIST_MAX_TEXT);
    gx_prompt_text_set_ext(&entry->child_win.drop_list_child_win_drop_list_child_prompt, &string);
}

/*****************************************************************************/
/* Create initial set of child elements for drop list                        */
/*****************************************************************************/
VOID populate_drop_list(void)
{
    int index;
    GX_VERTICAL_LIST *list;
    GX_SCROLLBAR_APPEARANCE sa;
    GX_DROP_LIST *drop = &main_screen.main_screen_droplist;
    gx_drop_list_popup_get(drop, &list);

    for (index = 0; index <= DROP_LIST_VISIBLE_ROWS; index++)
    {
        drop_list_row_create(list, (GX_WIDGET *)&drop_list_widgets[index], index);
    }

    sa.gx_scroll_thumb_pixelmap = GX_PIXELMAP_ID_LIST_SCROLL_THUMB;
    sa.gx_scroll_thumb_travel_min = 4;
    sa.gx_scroll_thumb_travel_max = 4;
    sa.gx_scroll_thumb_width = 8;
    sa.gx_scroll_width = 10;

    gx_vertical_scrollbar_create(&list_scroll, "list_scroll", list, &sa, GX_SCROLLBAR_VERTICAL|GX_STYLE_ENABLED);
}