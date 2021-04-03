/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include <math.h>
#include "tx_api.h"
#include "gx_api.h"
#include "demo_guix_widget_animation_resources.h"
#include "demo_guix_widget_animation_specifications.h"

#define DROP_LIST_TOTAL_ROWS  31
#define ANIMATION_TOTAL_STEPS 40
#define ANIMATION_ID           1
#define MAX_EASING_TYPE_NAME_LENGTH 30

/* Define prototypes.   */
VOID populate_drop_list();
void animation_start();
extern UINT win32_graphics_driver_setup_24xrgb(GX_DISPLAY *display);
UINT string_length_get(GX_CONST GX_CHAR* input_string, UINT max_string_length);

/* Define easing function data structure. */
typedef struct EASING_FUNCTION_DATA_STRUCT{
    INT            type;
    GX_RESOURCE_ID map_id;
    GX_CHAR       *type_text;
}EASING_FUNCTION_DATA;

/* A pointer to the root window.  */
GX_WINDOW_ROOT *root;

/* GX_PROMPT control blocks that we will use to populate the drop list */
GX_PROMPT       drop_list_items[DROP_LIST_TOTAL_ROWS];

/* A scrollbar control block */
GX_SCROLLBAR    list_scroll;

GX_ANIMATION   *animation = GX_NULL;
USHORT          easing_function_id = 0;

GX_BOOL         animation_back = GX_FALSE;
/* Define easing functions. */

/* Would it be difficult to draw the easing function graphi instead of 
   using pre-defined pixelmaps for each shape?
*/
GX_CONST EASING_FUNCTION_DATA easing_function_list[] = {
    { 0, GX_PIXELMAP_ID_LINEAR, "Linear" },
    { GX_ANIMATION_BACK_EASE_IN, GX_PIXELMAP_ID_BACK_EASE_IN, "BackEaseIn" },
    { GX_ANIMATION_BACK_EASE_OUT, GX_PIXELMAP_ID_BACK_EASE_OUT, "BackEaseOut" },
    { GX_ANIMATION_BACK_EASE_IN_OUT, GX_PIXELMAP_ID_BACK_EASE_IN_OUT, "BackEaseInOut" },
    { GX_ANIMATION_BOUNCE_EASE_IN, GX_PIXELMAP_ID_BOUNCE_EASE_IN, "BounceEaseIn" },
    { GX_ANIMATION_BOUNCE_EASE_OUT, GX_PIXELMAP_ID_BOUNCE_EASE_OUT, "BounceEaseOut" },
    { GX_ANIMATION_BOUNCE_EASE_IN_OUT, GX_PIXELMAP_ID_BOUNCE_EASE_IN_OUT, "BounceEaseInOut" },
    { GX_ANIMATION_CIRC_EASE_IN, GX_PIXELMAP_ID_CIRC_EASE_IN, "CircEaseIn" },
    { GX_ANIMATION_CIRC_EASE_OUT, GX_PIXELMAP_ID_CIRC_EASE_OUT, "CircEaseOut" },
    { GX_ANIMATION_CIRC_EASE_IN_OUT, GX_PIXELMAP_ID_CIRC_EASE_IN_OUT, "CircEaseInOut" },
    { GX_ANIMATION_CUBIC_EASE_IN, GX_PIXELMAP_ID_CUBIC_EASE_IN, "CubicEaseIn" },
    { GX_ANIMATION_CUBIC_EASE_OUT, GX_PIXELMAP_ID_CUBIC_EASE_OUT, "CubicEaseOut" },
    { GX_ANIMATION_CUBIC_EASE_IN_OUT, GX_PIXELMAP_ID_CUBIC_EASE_IN_OUT, "CubixEaseInOut" },
    { GX_ANIMATION_ELASTIC_EASE_IN, GX_PIXELMAP_ID_ELASTIC_EASE_IN, "ElasticEaseIn" },
    { GX_ANIMATION_ELASTIC_EASE_OUT, GX_PIXELMAP_ID_ELASTIC_EASE_OUT, "ElasticEaseOut" },
    { GX_ANIMATION_ELASTIC_EASE_IN_OUT, GX_PIXELMAP_ID_ELASTIC_EASE_IN_OUT, "ElasticEaseInOut" },
    { GX_ANIMATION_EXPO_EASE_IN, GX_PIXELMAP_ID_EXPO_EASE_IN, "ExpoEaseIn" },
    { GX_ANIMATION_EXPO_EASE_OUT, GX_PIXELMAP_ID_EXPO_EASE_OUT, "ExpoEaseOut" },
    { GX_ANIMATION_EXPO_EASE_IN_OUT, GX_PIXELMAP_ID_EXPO_EASE_IN_OUT, "ExpoEaseInOut" },
    { GX_ANIMATION_QUAD_EASE_IN, GX_PIXELMAP_ID_QUAD_EASE_IN, "QuadEaseIn" },
    { GX_ANIMATION_QUAD_EASE_OUT, GX_PIXELMAP_ID_QUAD_EASE_OUT, "QuadEaseOut" },
    { GX_ANIMATION_QUAD_EASE_IN_OUT, GX_PIXELMAP_ID_QUAD_EASE_IN_OUT, "QuadEaseInOut" },
    { GX_ANIMATION_QUART_EASE_IN, GX_PIXELMAP_ID_QUART_EASE_IN, "QuartEaseIn" },
    { GX_ANIMATION_QUART_EASE_OUT, GX_PIXELMAP_ID_QUART_EASE_OUT, "QuartEaseOut" },
    { GX_ANIMATION_QUART_EASE_IN_OUT, GX_PIXELMAP_ID_QUART_EASE_IN_OUT, "QuartEaseInOut" },
    { GX_ANIMATION_QUINT_EASE_IN, GX_PIXELMAP_ID_QUINT_EASE_IN, "QuintEaseIn" },
    { GX_ANIMATION_QUINT_EASE_OUT, GX_PIXELMAP_ID_QUINT_EASE_OUT, "QuintEaseOut" },
    { GX_ANIMATION_QUINT_EASE_IN_OUT, GX_PIXELMAP_ID_QUINT_EASE_IN_OUT, "QuintEaseInOut" },
    { GX_ANIMATION_SINE_EASE_IN, GX_PIXELMAP_ID_SINE_EASE_IN, "SineEaseIn" },
    { GX_ANIMATION_SINE_EASE_OUT, GX_PIXELMAP_ID_SINE_EASE_OUT, "SineEaseOut" },
    { GX_ANIMATION_SINE_EASE_IN_OUT, GX_PIXELMAP_ID_SINE_EASE_IN_OUT, "SineEaseInOut" },
    { GX_NULL, 0, "" }
};

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
    /* Initialize GUIX.  */
    gx_system_initialize();

    gx_studio_display_configure(PRIMARY, win32_graphics_driver_setup_24xrgb,
        LANGUAGE_ENGLISH, PRIMARY_THEME_1, &root);

    /* create the button screen */
    gx_studio_named_widget_create("main_screen", (GX_WIDGET *)root, GX_NULL);

    /* add selectable child items to the drop list */
    populate_drop_list();

    /* Show the root window to make it and patients screen visible.  */
    gx_widget_show(root);

    animation_start();

    /* start the GUIX thread */
    gx_system_start();
}

/******************************************************************************************/
/* Start animation.                                                                       */
/******************************************************************************************/
void animation_start()
{
    GX_ANIMATION_INFO  animation_info;
    GX_WINDOW *chart_win = &main_screen.main_screen_easing_function_chart_win;
    GX_RECTANGLE *size = &chart_win->gx_widget_size;

    if (animation)
    {
        gx_animation_stop(animation);
    }
    else
    {
        gx_system_animation_get(&animation);
    }

    if (animation)
    {
        memset(&animation_info, 0, sizeof(GX_ANIMATION_INFO));

        animation_info.gx_animation_start_position.gx_point_x = size->gx_rectangle_left - 40;
        animation_info.gx_animation_end_position.gx_point_x = size->gx_rectangle_left - 40;

        if (animation_back)
        {
            animation_info.gx_animation_start_position.gx_point_y = size->gx_rectangle_top + 68;
            animation_info.gx_animation_end_position.gx_point_y = size->gx_rectangle_bottom - 63;
        }
        else
        {
            animation_info.gx_animation_start_position.gx_point_y = size->gx_rectangle_bottom - 63;
            animation_info.gx_animation_end_position.gx_point_y = size->gx_rectangle_top+ 68;
        }

        animation_info.gx_animation_id = ANIMATION_ID;
        animation_info.gx_animation_frame_interval = 2;
        animation_info.gx_animation_steps = ANIMATION_TOTAL_STEPS;
        animation_info.gx_animation_start_alpha = 0xff;
        animation_info.gx_animation_end_alpha = 0xff;
        animation_info.gx_animation_target = (GX_WIDGET *)&main_screen.main_screen_animation_button;
        animation_info.gx_animation_parent = (GX_WIDGET *)&main_screen;
        animation_info.gx_animation_style = easing_function_id;

        /* pass in parameter to fire off the animation sequence */
        gx_animation_start(animation, &animation_info);
    }
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

    gx_window_wallpaper_set(&main_screen.main_screen_easing_function_chart_win, easing_function_list[selected_index].map_id, GX_FALSE);

    easing_function_id = easing_function_list[selected_index].type;
    animation_start();
}

/******************************************************************************************/
/* Override the default event processing of "main_screen" to handle signals from my child */
/* widgets.                                                                               */
/******************************************************************************************/
UINT main_screen_event_process(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
    case GX_SIGNAL(ID_EASING_FUNCTION_DROP_LIST, GX_EVENT_LIST_SELECT):
        animation_back = GX_FALSE;
        easing_function_drop_list_select_event_handler();
        break;

    case GX_EVENT_ANIMATION_COMPLETE:
        /* The animation controller is retrieved from system, when animation complete,
           it will be released. */
        animation = GX_NULL;
        if (animation_back)
        {
            animation_back = GX_FALSE;
        }
        else
        {
            animation_back = GX_TRUE;
        }
        animation_start();
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
    GX_STRING string;

    /* Define prompt size. */
    gx_utility_rectangle_define(&size, 0, 0, 100, 20);

    /* Create prompt. */
    gx_prompt_create(prompt, easing_function_list[index].type_text, list, 0, GX_STYLE_ENABLED | GX_STYLE_TEXT_LEFT | GX_STYLE_BORDER_NONE, 0, &size);

    /* Set prompt fill color. */
    gx_widget_fill_color_set(prompt, GX_COLOR_ID_LIGHT_SLATE_GRAY, GX_COLOR_ID_SLATE_GRAY, GX_COLOR_ID_LIGHT_SLATE_GRAY);

    /* Set prompt text color. */
    gx_prompt_text_color_set(prompt, GX_COLOR_ID_WHITE, GX_COLOR_ID_WHITE, GX_COLOR_ID_WHITE);

    /* Set prompt text. */
    string.gx_string_ptr = easing_function_list[index].type_text;
    string.gx_string_length = string_length_get(string.gx_string_ptr, MAX_EASING_TYPE_NAME_LENGTH);
    gx_prompt_text_set_ext(prompt, &string);
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
    gx_drop_list_popup_get(&main_screen.main_screen_easing_function_drop_list, &list);

    for (index = 0; index < DROP_LIST_TOTAL_ROWS; index++)
    {
        /* Create row items. */
        drop_list_row_create(list, (GX_WIDGET *)&drop_list_items[index], index);
    }

    /* Set scrollbar appearance. */
    memset(&scrollbar_appearance, 0, sizeof(GX_SCROLLBAR_APPEARANCE));
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

