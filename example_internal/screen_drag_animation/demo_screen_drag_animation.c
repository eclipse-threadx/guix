/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include <string.h>
#include "gx_api.h"
#include "screen_drag_animation_resources.h"
#include "screen_drag_animation_specifications.h"

/* Define the ThreadX demo thread control block and stack.  */

GX_WINDOW_ROOT    *root;

/* Define prototypes.   */
extern UINT win32_graphics_driver_setup_24xrgb(GX_DISPLAY *display);
VOID populate_drop_list(void);

GX_ANIMATION_INFO info;
GX_ANIMATION h_screen_drag_animation;
GX_ANIMATION v_screen_drag_animation;

USHORT h_screen_drag_style = GX_ANIMATION_SCREEN_DRAG | GX_ANIMATION_HORIZONTAL;
USHORT v_screen_drag_style = GX_ANIMATION_SCREEN_DRAG | GX_ANIMATION_VERTICAL;

INT    h_animation_count = 0;
INT    v_animation_count = 0;

#define ANIMATION_ID_H_SCREEN_DRAG 1
#define ANIMATION_ID_V_SCREEN_DRAG 2

/* Define menu screen list. */
GX_WIDGET *h_screen_list[] = {
    (GX_WIDGET *)&main_screen.main_screen_h_menu_window_1,
    (GX_WIDGET *)&h_menu_window_2,
    (GX_WIDGET *)&h_menu_window_3,
    GX_NULL
};

GX_WIDGET *v_screen_list[] = {
    (GX_WIDGET *)&main_screen.main_screen_v_menu_window_1,
    (GX_WIDGET *)&v_menu_window_2,
    (GX_WIDGET *)&v_menu_window_3,
    GX_NULL
};

TX_SEMAPHORE *h_screen_drag_semaphore = GX_NULL;
TX_SEMAPHORE *v_screen_drag_semaphore = GX_NULL;

/* Define easing function data structure. */
typedef struct EASING_FUNCTION_DATA_STRUCT{
    INT            type;
    GX_CHAR       *type_text;
}EASING_FUNCTION_DATA;

#define DROP_LIST_TOTAL_ROWS  31

/* GX_PROMPT control blocks that we will use to populate the drop list */
GX_PROMPT       drop_list_items[DROP_LIST_TOTAL_ROWS];

/* A scrollbar control block */
GX_SCROLLBAR    list_scroll;

GX_CONST EASING_FUNCTION_DATA easing_function_list[] = {
    { 0, "None" },
    { GX_ANIMATION_BACK_EASE_IN, "BackEaseIn" },
    { GX_ANIMATION_BACK_EASE_OUT, "BackEaseOut" },
    { GX_ANIMATION_BACK_EASE_IN_OUT, "BackEaseInOut" },
    { GX_ANIMATION_BOUNCE_EASE_IN, "BounceEaseIn" },
    { GX_ANIMATION_BOUNCE_EASE_OUT, "BounceEaseOut" },
    { GX_ANIMATION_BOUNCE_EASE_IN_OUT, "BounceEaseInOut" },
    { GX_ANIMATION_CIRC_EASE_IN, "CircEaseIn" },
    { GX_ANIMATION_CIRC_EASE_OUT, "CircEaseOut" },
    { GX_ANIMATION_CIRC_EASE_IN_OUT, "CircEaseInOut" },
    { GX_ANIMATION_CUBIC_EASE_IN, "CubicEaseIn" },
    { GX_ANIMATION_CUBIC_EASE_OUT, "CubicEaseOut" },
    { GX_ANIMATION_CUBIC_EASE_IN_OUT, "CubixEaseInOut" },
    { GX_ANIMATION_ELASTIC_EASE_IN, "ElasticEaseIn" },
    { GX_ANIMATION_ELASTIC_EASE_OUT, "ElasticEaseOut" },
    { GX_ANIMATION_ELASTIC_EASE_IN_OUT, "ElasticEaseInOut" },
    { GX_ANIMATION_EXPO_EASE_IN, "ExpoEaseIn" },
    { GX_ANIMATION_EXPO_EASE_OUT, "ExpoEaseOut" },
    { GX_ANIMATION_EXPO_EASE_IN_OUT, "ExpoEaseInOut" },
    { GX_ANIMATION_QUAD_EASE_IN, "QuadEaseIn" },
    { GX_ANIMATION_QUAD_EASE_OUT, "QuadEaseOut" },
    { GX_ANIMATION_QUAD_EASE_IN_OUT, "QuadEaseInOut" },
    { GX_ANIMATION_QUART_EASE_IN, "QuartEaseIn" },
    { GX_ANIMATION_QUART_EASE_OUT, "QuartEaseOut" },
    { GX_ANIMATION_QUART_EASE_IN_OUT, "QuartEaseInOut" },
    { GX_ANIMATION_QUINT_EASE_IN, "QuintEaseIn" },
    { GX_ANIMATION_QUINT_EASE_OUT, "QuintEaseOut" },
    { GX_ANIMATION_QUINT_EASE_IN_OUT, "QuintEaseInOut" },
    { GX_ANIMATION_SINE_EASE_IN, "SineEaseIn" },
    { GX_ANIMATION_SINE_EASE_OUT, "SineEaseOut" },
    { GX_ANIMATION_SINE_EASE_IN_OUT, "SineEaseInOut" },
    { GX_NULL, "" }
};

int main(int argc, char ** argv)
{
    tx_kernel_enter();
    return(0);
}

VOID tx_application_define(void *first_unused_memory)
{
    /* Initialize GUIX.  */
    gx_system_initialize();

    gx_studio_display_configure(DISPLAY_1, win32_graphics_driver_setup_24xrgb,
                                LANGUAGE_ENGLISH, DISPLAY_1_THEME_1, &root);

    /* create the main screen */
    gx_studio_named_widget_create("main_screen", (GX_WIDGET *) root, GX_NULL);

    gx_studio_named_widget_create("h_menu_window_2", GX_NULL, GX_NULL);
    gx_studio_named_widget_create("h_menu_window_3", GX_NULL, GX_NULL);
    gx_studio_named_widget_create("v_menu_window_2", GX_NULL, GX_NULL);
    gx_studio_named_widget_create("v_menu_window_3", GX_NULL, GX_NULL);

    memset(&h_screen_drag_animation, 0, sizeof(GX_ANIMATION));
    memset(&v_screen_drag_animation, 0, sizeof(GX_ANIMATION));
    gx_animation_create(&h_screen_drag_animation);
    gx_animation_create(&v_screen_drag_animation);
    populate_drop_list();

    /* Show the root window to make it and patients screen visible.  */
    gx_widget_show(root);

    /* start the GUIX thread */
    gx_system_start();
}

/******************************************************************************************/
/* Handle easing function drop list select event.                                         */
/******************************************************************************************/
VOID easing_function_drop_list_select_event_handler()
{
    GX_VERTICAL_LIST *list;
    INT               selected_index;

    /* Get popup list of the drop list. */
    gx_drop_list_popup_get(&main_screen.main_screen_easing_function_drop_list, &list);

    /* Get the selected index of the popup list. */
    gx_vertical_list_selected_index_get(list, &selected_index);

    h_screen_drag_style &= ~GX_ANIMATION_EASING_FUNC_MASK;
    h_screen_drag_style |= easing_function_list[selected_index].type;

    v_screen_drag_style &= ~GX_ANIMATION_EASING_FUNC_MASK;
    v_screen_drag_style |= easing_function_list[selected_index].type;

    gx_widget_hide(&main_screen.main_screen_h_menu_window);
    gx_widget_hide(&main_screen.main_screen_v_menu_window);
    gx_widget_show(&main_screen.main_screen_h_menu_window);
    gx_widget_show(&main_screen.main_screen_v_menu_window);
}

UINT main_screen_event_handler(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
    case GX_SIGNAL(ID_SCREEN_DRAG_WRAP, GX_EVENT_TOGGLE_ON):
        h_screen_drag_style |= GX_ANIMATION_WRAP;
        v_screen_drag_style |= GX_ANIMATION_WRAP;
        gx_widget_hide(&main_screen.main_screen_h_menu_window);
        gx_widget_hide(&main_screen.main_screen_v_menu_window);
        gx_widget_show(&main_screen.main_screen_h_menu_window);
        gx_widget_show(&main_screen.main_screen_v_menu_window);
        break;

    case GX_SIGNAL(ID_SCREEN_DRAG_WRAP, GX_EVENT_TOGGLE_OFF):
        h_screen_drag_style &= ~GX_ANIMATION_WRAP;
        v_screen_drag_style &= ~GX_ANIMATION_WRAP;
        gx_widget_hide(&main_screen.main_screen_h_menu_window);
        gx_widget_hide(&main_screen.main_screen_v_menu_window);
        gx_widget_show(&main_screen.main_screen_h_menu_window);
        gx_widget_show(&main_screen.main_screen_v_menu_window);
        break;

    case GX_SIGNAL(ID_EASING_FUNCTION_DROP_LIST, GX_EVENT_LIST_SELECT):
        easing_function_drop_list_select_event_handler();
        break;

    default:
        return gx_window_event_process(window, event_ptr);
    }

    return 0;
}

UINT h_menu_window_event_handler(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_SHOW:
        memset(&info, 0, sizeof(GX_ANIMATION_INFO));
        info.gx_animation_parent = (GX_WIDGET *)window;
        info.gx_animation_style = h_screen_drag_style;
        info.gx_animation_id = ANIMATION_ID_H_SCREEN_DRAG;
        info.gx_animation_frame_interval = 1;
        info.gx_animation_steps = 20;
        info.gx_animation_slide_screen_list = h_screen_list;
        gx_animation_drag_enable(&h_screen_drag_animation, (GX_WIDGET *)window, &info);
        return gx_widget_event_process(window, event_ptr);

    case GX_EVENT_HIDE:
        gx_animation_drag_disable(&h_screen_drag_animation, (GX_WIDGET *)window);
        return gx_widget_event_process(window, event_ptr);

    case GX_EVENT_ANIMATION_COMPLETE:
        h_animation_count++;
        gx_numeric_prompt_value_set(&main_screen.main_screen_h_animation_count_prompt, h_animation_count);

        if (h_screen_drag_semaphore)
        {
            tx_semaphore_put(h_screen_drag_semaphore);
        }
        break;

    default:
        return gx_window_event_process(window, event_ptr);
    }

    return 0;
}

UINT v_menu_window_event_handler(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_SHOW:
        memset(&info, 0, sizeof(GX_ANIMATION_INFO));
        info.gx_animation_parent = (GX_WIDGET *)window;
        info.gx_animation_style = v_screen_drag_style;
        info.gx_animation_id = ANIMATION_ID_V_SCREEN_DRAG;
        info.gx_animation_frame_interval = 1;
        info.gx_animation_steps = 20;
        info.gx_animation_slide_screen_list = v_screen_list;
        gx_animation_drag_enable(&v_screen_drag_animation, (GX_WIDGET *)window, &info);
        return gx_widget_event_process(window, event_ptr);

    case GX_EVENT_HIDE:
        gx_animation_drag_disable(&v_screen_drag_animation, (GX_WIDGET *)window);
        return gx_widget_event_process(window, event_ptr);

    case GX_EVENT_ANIMATION_COMPLETE:
        v_animation_count++;
        gx_numeric_prompt_value_set(&main_screen.main_screen_v_animation_count_prompt, v_animation_count);

        if (v_screen_drag_semaphore)
        {
            tx_semaphore_put(v_screen_drag_semaphore);
        }
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
    GX_PROMPT *prompt = (GX_PROMPT *)widget;
    GX_STRING str;

    /* Define prompt size. */
    gx_utility_rectangle_define(&size, 10, 0, 100, 20);

    /* Create prompt. */
    gx_prompt_create(prompt, easing_function_list[index].type_text, list, 0, GX_STYLE_ENABLED | GX_STYLE_TEXT_LEFT | GX_STYLE_BORDER_NONE, 0, &size);

    /* Set prompt text. */
    str.gx_string_ptr = (GX_CHAR *)easing_function_list[index].type_text;
    str.gx_string_length = strnlen((GX_CHAR *)easing_function_list[index].type_text, 30);
    gx_prompt_text_set_ext(prompt, &str);
}

/*****************************************************************************/
/* Create drop list items.                                                   */
/*****************************************************************************/
VOID populate_drop_list(void)
{
    INT index;
    GX_VERTICAL_LIST *list;

    /* Get popup list of the drop list. */
    gx_drop_list_popup_get(&main_screen.main_screen_easing_function_drop_list, &list);

    for (index = 0; index < DROP_LIST_TOTAL_ROWS; index++)
    {
        /* Create row items. */
        drop_list_row_create(list, (GX_WIDGET *)&drop_list_items[index], index);
    }

    /* Create scrollbar for popup list. */
    gx_vertical_scrollbar_create(&list_scroll, "list_scroll", list,
        GX_NULL, GX_SCROLLBAR_VERTICAL | GX_STYLE_ENABLED);
}
