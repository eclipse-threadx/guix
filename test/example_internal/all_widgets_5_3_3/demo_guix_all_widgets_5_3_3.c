/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "all_widgets_5_3_3_resources.h"
#include "all_widgets_5_3_3_specifications.h"

#define DROP_LIST_VISIBLE_ROWS 100

GX_WINDOW *pTextScreen;
GX_WINDOW *pWindowScreen;
GX_WINDOW *pButtonScreen;
GX_WINDOW *pIndicatorScreen;
GX_WINDOW *pPopupScreen;
GX_WINDOW *pGaugeScreen;
GX_WINDOW *pSpriteScreen;
GX_SCROLLBAR list_scroll;

/* Define the ThreadX demo thread control block and stack.  */

GX_WINDOW_ROOT    *root;

TX_BYTE_POOL       rotate_pool;

#define            SCRATCHPAD_PIXELS (PRIMARY_X_RESOLUTION * PRIMARY_X_RESOLUTION)
GX_COLOR           scratchpad[SCRATCHPAD_PIXELS];

typedef struct {
    GX_PROMPT prompt;
    CHAR text[80];
} DROP_LIST_WIDGET;

DROP_LIST_WIDGET drop_list_widgets[DROP_LIST_VISIBLE_ROWS + 1];

/* Define prototypes.   */
VOID PopulateDropList();
VOID start_guix(VOID);
extern UINT win32_graphics_driver_setup_24xrgb(GX_DISPLAY *display);


int main(int argc, char ** argv)
{
  tx_kernel_enter();
  return(0);
}

VOID *rotate_memory_allocate(ULONG size)
{
    VOID *memptr;

    if (tx_byte_allocate(&rotate_pool, &memptr, size, TX_NO_WAIT) == TX_SUCCESS)
    {
        return memptr;
    }

    return NULL;
}

VOID rotate_memory_free(VOID *mem)
{
    tx_byte_release(mem);
}

VOID tx_application_define(void *first_unused_memory)
{
    /* Create the main demo thread.  */

    /* create byte pool for rotate to use */
    tx_byte_pool_create(&rotate_pool, "scratchpad", scratchpad,
                        SCRATCHPAD_PIXELS * sizeof(GX_COLOR));

    start_guix();

    /* install our memory allocator and de-allocator */
    gx_system_memory_allocator_set(rotate_memory_allocate, rotate_memory_free);
}

VOID start_guix(void)
{
    /* Initialize GUIX.  */
    gx_system_initialize();

    gx_studio_display_configure(PRIMARY, win32_graphics_driver_setup_24xrgb, 
                                LANGUAGE_ENGLISH, PRIMARY_THEME_1, &root);

    /* create the button screen */
    gx_studio_named_widget_create("button_screen", (GX_WIDGET *) root, (GX_WIDGET **) &pButtonScreen);

    /* create the text widget screen */
    gx_studio_named_widget_create("text_screen", GX_NULL, (GX_WIDGET **) &pTextScreen);

    /* create the windows screen */
    gx_studio_named_widget_create("window_screen", GX_NULL, (GX_WIDGET **) &pWindowScreen);
    PopulateDropList();

    /* create the indicator screen */
    gx_studio_named_widget_create("indicator_screen", GX_NULL, (GX_WIDGET **) &pIndicatorScreen);

    /* create the popup screen */
    gx_studio_named_widget_create("popup_modal", GX_NULL, (GX_WIDGET **) &pPopupScreen);

    /* create circular gauge screen */
    gx_studio_named_widget_create("gauge_screen", GX_NULL, (GX_WIDGET **)&pGaugeScreen);

    /* create sprite screen */
    gx_studio_named_widget_create("sprite_screen", GX_NULL, (GX_WIDGET **)&pSpriteScreen);

    /* Show the root window to make it and patients screen visible.  */
    gx_widget_show(root);

    /* start the GUIX thread */
    gx_system_start();
}


void ToggleScreen(GX_WINDOW *new_win, GX_WINDOW *old_win)
{
    if (!new_win->gx_widget_parent)
    {
        gx_widget_attach(root, (GX_WIDGET *) new_win);
    }
    else
    {
        gx_widget_show((GX_WIDGET *) new_win);
    }
    gx_widget_hide((GX_WIDGET *) old_win);
}

UINT buttons_handler(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    ULONG modal_status;

    switch(event_ptr->gx_event_type)
    {
    case GX_SIGNAL(ID_CHECKBOX, GX_EVENT_TOGGLE_ON):
        gx_widget_style_remove(&button_screen.button_screen_pixelmap_button1, GX_STYLE_ENABLED);
        break;

    case GX_SIGNAL(ID_CHECKBOX, GX_EVENT_TOGGLE_OFF):
        gx_widget_style_add(&button_screen.button_screen_pixelmap_button1, GX_STYLE_ENABLED);
        break;

    case GX_SIGNAL(ID_TEXT_BUTTON, GX_EVENT_CLICKED):
        gx_widget_attach(root, pPopupScreen);
        gx_window_execute(pPopupScreen, &modal_status);
        break;

    default:
        return next_button_handler(window, event_ptr);
    }
    return 0;
}

UINT next_button_handler(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    switch(event_ptr->gx_event_type)
    {
    case GX_SIGNAL(IDB_NEXT, GX_EVENT_CLICKED):
        switch(window ->gx_widget_id)
        {
        case ID_BUTTON_SCREEN:
            /* we are on button window, switch to window */
            ToggleScreen(pWindowScreen, window);
            break;

        case ID_WINDOW_SCREEN:
            ToggleScreen(pTextScreen, window);
            break; 

        case ID_TEXT_SCREEN:
            ToggleScreen(pIndicatorScreen, window);
            break; 

        case ID_INDICATOR_SCREEN:
            ToggleScreen(pGaugeScreen, window);
            break; 
            
        case ID_GAUGE_SCREEN:
            ToggleScreen(pSpriteScreen, window);
            break;

        case ID_SPRITE_SCREEN:
            ToggleScreen(pButtonScreen, window);
        }
        break;

    default:
        return gx_window_event_process(window, event_ptr);
    }
    return 0;
}

UINT ModalWinEvent(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    switch(event_ptr->gx_event_type)
    {
    case GX_SIGNAL(IDB_CANCEL, GX_EVENT_CLICKED):
    case GX_SIGNAL(IDB_OK, GX_EVENT_CLICKED):
        return event_ptr->gx_event_sender;
    
    default:
        return gx_window_event_process(window, event_ptr);
    }
}


VOID drop_list_row_create(GX_VERTICAL_LIST *list, GX_WIDGET *widget, INT index)
{
GX_BOOL created;
GX_RECTANGLE size;
GX_CHAR temp[10];

    DROP_LIST_WIDGET *entry = (DROP_LIST_WIDGET *) widget;

    GX_STRCPY(entry->text, "List Entry #");
    gx_utility_ltoa(index + 1, temp, 10);
    GX_STRCAT(entry->text, (char*)temp);

    gx_widget_created_test(widget, &created);

    if (!created)
    {
        gx_utility_rectangle_define(&size, 0, 0, 100, 30);
        gx_prompt_create(&entry->prompt, entry->text, list, 0, GX_STYLE_ENABLED|GX_STYLE_TEXT_LEFT|GX_STYLE_BORDER_NONE, 0, &size);
        gx_widget_fill_color_set(&entry->prompt, GX_COLOR_ID_BLACK, GX_COLOR_ID_BLUE);
        gx_prompt_text_color_set(&entry->prompt, GX_COLOR_ID_WHITE, GX_COLOR_ID_WHITE);
    }
  
    gx_prompt_text_set(&entry->prompt, (GX_CHAR*)entry->text);
}

VOID PopulateDropList(void)
{
    int index;
    GX_VERTICAL_LIST *list;

    WINDOW_SCREEN_CONTROL_BLOCK *control = (WINDOW_SCREEN_CONTROL_BLOCK *) pWindowScreen;
    GX_DROP_LIST *drop = &control->window_screen_drop_list;
    gx_drop_list_popup_get(drop, &list);

    for (index = 0; index < DROP_LIST_VISIBLE_ROWS; index++)
    {
        drop_list_row_create(list, (GX_WIDGET *) &drop_list_widgets[index], index);
    }

    gx_vertical_scrollbar_create(&list_scroll, "list_scroll", list, GX_NULL,
        GX_SCROLLBAR_VERTICAL | GX_SCROLLBAR_RELATIVE_THUMB | GX_SCROLLBAR_END_BUTTONS);

}

VOID gauge_prompt_draw(GX_PROMPT *widget)
{
GX_CHAR *text;

    gx_system_string_get(GX_STRING_ID_STRING_44, &text);
    gx_context_brush_define(widget->gx_prompt_normal_text_color, 0, GX_BRUSH_UNDERLINE);
    gx_context_brush_width_set(2);
    gx_context_font_set(widget->gx_prompt_font_id);
    gx_canvas_text_draw(widget->gx_widget_size.gx_rectangle_left,
                        widget->gx_widget_size.gx_rectangle_top,
                        text, -1);
}
