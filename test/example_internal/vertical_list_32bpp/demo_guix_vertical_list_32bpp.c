/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"
#include "vertical_list_32bpp_resources.h"
#include "vertical_list_32bpp_specifications.h"

#define LIST_ROWS           20
#define LIST_VISIBLE_ROWS   10
#define LIST_ROW_HEIGHT     36

int list_total_rows = LIST_ROWS;

WINDOW_CONTROL_BLOCK *pMainScreen;

/* Define the ThreadX demo thread control block and stack.  */

TX_THREAD          demo_thread;
UCHAR              demo_thread_stack[4096];

typedef struct {
    GX_PROMPT prompt;
    GX_CHAR   text[40];
}LIST_CHILD;

GX_WINDOW_ROOT    *root;
LIST_CHILD        list_children[LIST_VISIBLE_ROWS + 1];
LIST_CHILD        wrap_list_children[LIST_VISIBLE_ROWS + 1];
INT               list_index;

/* Define prototypes.   */
VOID  demo_thread_entry(ULONG thread_input);
extern UINT win32_graphics_driver_setup_24xrgb(GX_DISPLAY *display);
void vertical_list_children_create(WINDOW_CONTROL_BLOCK *win);
void vertical_wrap_list_children_create(WINDOW_CONTROL_BLOCK *win);

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


GX_STUDIO_DISPLAY_INFO display_info;

VOID  demo_thread_entry(ULONG thread_input)
{
    /* Initialize GUIX.  */
    gx_system_initialize();

    gx_studio_display_configure(DISPLAY_1, win32_graphics_driver_setup_24xrgb, 
                                LANGUAGE_ENGLISH, DISPLAY_1_THEME_1, &root);

    /* create the button screen */
    gx_studio_named_widget_create("window", (GX_WIDGET*)root, (GX_WIDGET **) &pMainScreen);
    gx_studio_named_widget_create("list_child_height_test_screen", GX_NULL, GX_NULL);

    /* Create list children. */
    vertical_list_children_create(pMainScreen);
    vertical_wrap_list_children_create(pMainScreen);

    /* Show the root window to make it and patients screen visible.  */
    gx_widget_show(root);

    /* let GUIX run */
    gx_system_start();
}

void string_to_int(char *str, int len, int *value)
{
int i;
    
    *value = 0;

    for(i = 0; i < len; i++)
    {
        *value = *value * 10;
        *value += str[i] - '0';
    }
}

void remove_last_index()
{
    gx_vertical_list_total_rows_set(&pMainScreen->window_vertical_list_scroll, --list_total_rows);
    gx_vertical_list_total_rows_set(&pMainScreen->window_vertical_list_wrap, list_total_rows);
}

UINT main_event_handler(GX_WINDOW *window, GX_EVENT *event_ptr)
{
GX_SINGLE_LINE_TEXT_INPUT *sl_text_input;


    switch(event_ptr->gx_event_type)
    {
    case GX_SIGNAL(ID_BUTTON_PAGE_INDEX_SET, GX_EVENT_CLICKED):
        sl_text_input = &((WINDOW_CONTROL_BLOCK *)window)->window_text_input_page_index;
        string_to_int(sl_text_input->gx_single_line_text_input_buffer, sl_text_input->gx_single_line_text_input_string_size, &list_index);

        if ((list_index >= 0) && (list_index < LIST_ROWS))
        {
            gx_vertical_list_page_index_set(&((WINDOW_CONTROL_BLOCK *)window)->window_vertical_list_scroll, list_index);
            gx_vertical_list_page_index_set(&((WINDOW_CONTROL_BLOCK *)window)->window_vertical_list_wrap, list_index);
        }

        break;
    case GX_SIGNAL(ID_BUTTON_SELECTED_SET, GX_EVENT_CLICKED):
        sl_text_input = &((WINDOW_CONTROL_BLOCK *)window)->window_text_input_selected_set;
        string_to_int(sl_text_input->gx_single_line_text_input_buffer, sl_text_input->gx_single_line_text_input_string_size, &list_index);

        if ((list_index >= 0) && (list_index < LIST_ROWS))
        {
            gx_vertical_list_selected_set(&((WINDOW_CONTROL_BLOCK *)window)->window_vertical_list_scroll, list_index);
            gx_vertical_list_selected_set(&((WINDOW_CONTROL_BLOCK *)window)->window_vertical_list_wrap, list_index);
        }
        break;

    case GX_SIGNAL(ID_REMOVE_LAST_INDEX, GX_EVENT_CLICKED):
        remove_last_index();
        break;

    case GX_SIGNAL(ID_BUTTON_SET_TOTAL_ROWS, GX_EVENT_CLICKED):
        sl_text_input = &((WINDOW_CONTROL_BLOCK*)window)->window_text_input_set_total_rows;
        string_to_int(sl_text_input->gx_single_line_text_input_buffer, sl_text_input->gx_single_line_text_input_string_size, &list_index);

        if (list_index >= 0)
        {
            gx_vertical_list_total_rows_set(&((WINDOW_CONTROL_BLOCK*)window)->window_vertical_list_scroll, list_index);
            gx_vertical_list_total_rows_set(&((WINDOW_CONTROL_BLOCK*)window)->window_vertical_list_wrap, list_index);
        }
        break;

    case GX_SIGNAL(ID_VERTICAL_LIST_1, GX_EVENT_LIST_SELECT):
        gx_numeric_prompt_value_set(&((WINDOW_CONTROL_BLOCK*)window)->window_vertical_list_1_selected_prompt,
            event_ptr->gx_event_payload.gx_event_longdata + 1);
        break;
    }

    return gx_window_event_process(window, event_ptr);
}

void list_callback(GX_VERTICAL_LIST *list, GX_WIDGET *widget, INT index)
{
    GX_BOOL is_created;
    GX_RECTANGLE size;
    LIST_CHILD *lp = (LIST_CHILD *)widget;
    GX_STRING text;

    gx_widget_created_test(&lp->prompt, &is_created);

    if (!is_created)
    {
        size.gx_rectangle_top = size.gx_rectangle_left = size.gx_rectangle_right = 0;
        size.gx_rectangle_bottom = size.gx_rectangle_top + LIST_ROW_HEIGHT - 1;

        gx_prompt_create(&lp->prompt, GX_NULL, list, 0, GX_STYLE_ENABLED, 0, &size);
    }

    gx_utility_ltoa(index, lp->text, 40);
    text.gx_string_ptr = lp->text;
    text.gx_string_length = strnlen(lp->text, sizeof(lp->text));
    gx_prompt_text_set_ext(&lp->prompt, &text);
}

void vertical_list_children_create(WINDOW_CONTROL_BLOCK *win)
{
    INT count;
    GX_VERTICAL_LIST *list = &win->window_vertical_list_scroll;

    for (count = 0; count <= LIST_VISIBLE_ROWS; count++)
    {
        list_callback(list, (GX_WIDGET *)&list_children[count], count);
    }
}

void vertical_wrap_list_children_create(WINDOW_CONTROL_BLOCK *win)
{
    INT count;
    GX_VERTICAL_LIST *list = &win->window_vertical_list_wrap;

    for (count = 0; count <= LIST_VISIBLE_ROWS; count++)
    {
        list_callback(list, (GX_WIDGET *)&wrap_list_children[count], count);
    }
}