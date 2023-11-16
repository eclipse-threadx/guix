/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"
#include "all_widgets_synergy_565rgb_resources.h"
#include "all_widgets_synergy_565rgb_specifications.h"

#define DROP_LIST_VISIBLE_ROWS 100
#define NUMBER_ANIMATION_TIMER 2

extern TEXT_SCREEN_CONTROL_BLOCK text_screen;
extern MENU_SCREEN_CONTROL_BLOCK menu_screen;

GX_WINDOW *pTextScreen;
GX_WINDOW *pWindowScreen;
GX_WINDOW *pButtonScreen;
GX_WINDOW *pIndicatorScreen;
GX_WINDOW *pPopupScreen;
GX_WINDOW *pGaugeScreen;
GX_WINDOW *pSpriteScreen;
GX_WINDOW *pScrollWheelScreen;
GX_SCROLLBAR list_scroll;

#define NESTED_WINDOW_WIDTH  94
#define NESTED_WINDOW_HEIGHT  73

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

/* Define screen toggle function that used for resgression tests. */
void ToggleScreen(GX_WINDOW *new_win, GX_WINDOW *old_win)
{
    if(!new_win->gx_widget_parent)
    {
        gx_widget_attach(root, (GX_WIDGET *)new_win);
    }
    else
    {
        gx_widget_show((GX_WIDGET *)new_win);
    }
    gx_widget_hide((GX_WIDGET *)old_win);
}

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

    gx_studio_display_configure(PRIMARY, _gx_synergy_display_driver_setup,
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

    /* create menu screen */
    gx_studio_named_widget_create("menu_screen", GX_NULL, GX_NULL);

    gx_studio_named_widget_create("glyph_screen", GX_NULL, GX_NULL);
    gx_widget_style_add(&glyph_screen.glyph_screen_view, GX_STYLE_USE_LOCAL_ALPHA);

    gx_studio_named_widget_create("scroll_wheel_screen", GX_NULL, GX_NULL);

    /* Show the root window to make it and patients screen visible.  */
    gx_widget_show(root);

    /* start the GUIX thread */
    gx_system_start();
}

UINT glyph_screen_event_process(GX_WINDOW* window, GX_EVENT* event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
    case GX_SIGNAL(ID_8BPP_RAW, GX_EVENT_RADIO_SELECT):
        gx_multi_line_text_view_font_set(&glyph_screen.glyph_screen_view, GX_FONT_ID_NORMAL_FONT);
        break;

    case GX_SIGNAL(ID_8BPP_COMPRESS, GX_EVENT_RADIO_SELECT):
        gx_multi_line_text_view_font_set(&glyph_screen.glyph_screen_view, GX_FONT_ID_COMPRESS_8BPP);

        break;

    case GX_SIGNAL(ID_4BPP_RAW, GX_EVENT_RADIO_SELECT):
        gx_multi_line_text_view_font_set(&glyph_screen.glyph_screen_view, GX_FONT_ID_NORMAL_4BPP);
        break;

    case GX_SIGNAL(ID_4BPP_COMPRESS, GX_EVENT_RADIO_SELECT):
        gx_multi_line_text_view_font_set(&glyph_screen.glyph_screen_view, GX_FONT_ID_COMPRESS_4BPP);
        break;

    case GX_SIGNAL(ID_1BPP_RAW, GX_EVENT_RADIO_SELECT):
        gx_multi_line_text_view_font_set(&glyph_screen.glyph_screen_view, GX_FONT_ID_NORMAL_1BPP);

        break;

    case GX_SIGNAL(ID_1BPP_COMPRESS, GX_EVENT_RADIO_SELECT):
        gx_multi_line_text_view_font_set(&glyph_screen.glyph_screen_view, GX_FONT_ID_COMPRESS_1BPP);
        break;

    case GX_SIGNAL(ID_BRUSH_ALPHA_SLIDER, GX_EVENT_SLIDER_VALUE):
        glyph_screen.glyph_screen_view.gx_widget_alpha = event_ptr->gx_event_payload.gx_event_ulongdata;
        gx_system_dirty_mark(&glyph_screen.glyph_screen_view);
        break;

    default:
        return gx_window_event_process(window, event_ptr);
    }

    return 0;
}


VOID drop_list_row_create(GX_VERTICAL_LIST *list, GX_WIDGET *widget, INT index)
{
GX_BOOL created;
GX_RECTANGLE size;
GX_CHAR temp[10];
GX_STRING text;

    DROP_LIST_WIDGET *entry = (DROP_LIST_WIDGET *) widget;

    GX_STRCPY(entry->text, " List Entry #");
    gx_utility_ltoa(index + 1, temp, 10);
    GX_STRCAT(entry->text, (char*)temp);

    gx_widget_created_test(widget, &created);

    if (!created)
    {
        gx_utility_rectangle_define(&size, 0, 0, 100, 30);
        gx_prompt_create(&entry->prompt, entry->text, list, 0, GX_STYLE_ENABLED|GX_STYLE_TEXT_LEFT|GX_STYLE_BORDER_NONE|GX_STYLE_TRANSPARENT, 0, &size);
        gx_prompt_text_color_set(&entry->prompt, GX_COLOR_ID_CANVAS, GX_COLOR_ID_CANVAS, GX_COLOR_ID_CANVAS);
    }

    text.gx_string_ptr = entry->text;
    text.gx_string_length = strnlen(entry->text, sizeof(entry->text));
  
    gx_prompt_text_set_ext(&entry->prompt, &text);
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
        GX_SCROLLBAR_VERTICAL | GX_SCROLLBAR_RELATIVE_THUMB | GX_SCROLLBAR_END_BUTTONS | GX_STYLE_ENABLED);

}