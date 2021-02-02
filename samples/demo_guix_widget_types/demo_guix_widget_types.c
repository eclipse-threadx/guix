/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"
#include "guix_widget_types_resources.h"
#include "guix_widget_types_specifications.h"


#define SCRATCHPAD_PIXELS (PRIMARY_X_RESOLUTION * PRIMARY_Y_RESOLUTION)
#define DROP_LIST_VISIBLE_ROWS  4
#define NUM_SLIDERS             8

#define COUNTER_TIMER  2
#define UNIT_TIMER     3
#define BLEND_TIMER    4
#define SLIDER_ANIMATION_TIMER 5
#define PROMPT_VALUE_UPDATE_TIMER 6

#define SLIDER_ANIMATION_STEPS 12

#define MAX_ROW_TEXT_LENGTH 80
#define MAX_SLIDER_VAL_TEXT_LENGTH 10

GX_BOOL APP_SHIFT_FLAG = GX_FALSE;
GX_BOOL APP_CTRL_FLAG = GX_FALSE;

/* allocate memory for composite canvas */
GX_COLOR           scratchpad[SCRATCHPAD_PIXELS];

extern GX_STUDIO_DISPLAY_INFO guix_widget_types_display_table[];
extern UINT win32_graphics_driver_setup_24xrgb(GX_DISPLAY *display);
extern GX_SCROLLBAR_APPEARANCE  Window_Screen_Vertical_Scroll_properties;
void ToggleScreen(GX_WINDOW *new_win, GX_WINDOW *old_win);
UINT string_length_get(GX_CONST GX_CHAR* input_string, UINT max_string_length);

TX_BYTE_POOL       rotate_pool;
GX_WINDOW_ROOT    *root;
GX_SCROLLBAR       list_scroll;

INT                blend_alpha;
INT                blend_increment;
INT                numeric_prompt_value = 10000;

/* A structure to hold control block and text for each drop-list item */
typedef struct {
    GX_PROMPT prompt;
    GX_CHAR   text[MAX_ROW_TEXT_LENGTH + 1];
} DROP_LIST_WIDGET;

DROP_LIST_WIDGET drop_list_widgets[DROP_LIST_VISIBLE_ROWS + 1];

/* A structure to hold values used to animate slider controls */
typedef struct {
    int target_val;
    int current_val;
    int increment;
    GX_SLIDER *slider;
} SLIDER_VAL_ENTRY;

SLIDER_VAL_ENTRY slider_vals[NUM_SLIDERS];

/* Define prototypes.   */
VOID PopulateDropList();
VOID reset_sliders(GX_WINDOW *parent);
GX_BOOL update_sliders();
VOID start_guix(void);

int glyph_struct_size;

static GX_CHAR slider_val_text[MAX_SLIDER_VAL_TEXT_LENGTH + 1];

/*****************************************************************************/
/* Application entry.                                                        */
/*****************************************************************************/
int main(int argc, char ** argv)
{
  tx_kernel_enter();
  return(0);
}

/*****************************************************************************/
/* Function called by GUIX internals to allocate memory block. This is used  */
/* to allocate memory for rotated gauge needle image                         */
/*****************************************************************************/
VOID *rotate_memory_allocate(ULONG size)
{
    VOID *memptr;

    if (tx_byte_allocate(&rotate_pool, &memptr, size, TX_NO_WAIT) == TX_SUCCESS)
    {
        return memptr;
    }

    return NULL;
}

/*****************************************************************************/
/* Function called by GUIX internals to free memory dynamically allocated    */
/*****************************************************************************/
VOID rotate_memory_free(VOID *mem)
{
    tx_byte_release(mem);
}

/*****************************************************************************/
/* Function called by ThreadX startup to define initial system               */
/*****************************************************************************/
VOID tx_application_define(void *first_unused_memory)
{
    /* create byte pool for rotate to use */
    tx_byte_pool_create(&rotate_pool, "scratchpad", scratchpad,
                        SCRATCHPAD_PIXELS * sizeof(GX_COLOR));

    glyph_struct_size = sizeof(GX_GLYPH);
    start_guix();
}

/*****************************************************************************/
/* Called by tx_application_define (above), configure and start GUIX         */
/*****************************************************************************/
VOID start_guix(void)
{
    /* Initialize GUIX.  */
    gx_system_initialize();

    /* install our memory allocator and de-allocator */
    gx_system_memory_allocator_set(rotate_memory_allocate, rotate_memory_free);

    gx_studio_display_configure(PRIMARY, win32_graphics_driver_setup_24xrgb, 
                                LANGUAGE_ENGLISH, PRIMARY_THEME_1, &root);

    /* create the button screen */
    gx_studio_named_widget_create("Button_Screen", GX_NULL, GX_NULL);

    /* create the text widget screen */
    gx_studio_named_widget_create("Text_Screen", GX_NULL, GX_NULL);

    /* create the slider widget screen */
    gx_studio_named_widget_create("Slider_Screen", GX_NULL, GX_NULL);

    /* create circular gauge screen */
    gx_studio_named_widget_create("Gauge_Screen", GX_NULL, GX_NULL);
    gx_circular_gauge_angle_set(&Gauge_Screen.Gauge_Screen_Gauge_Animated, -111);
    gx_circular_gauge_angle_set(&Gauge_Screen.Gauge_Screen_Gauge_Not_Animated, -111);

    gx_studio_named_widget_create("Window_Screen", GX_NULL, GX_NULL);

    /* Create scroll wheel screen. */
    gx_studio_named_widget_create("Scroll_Wheel_Screen", (GX_WIDGET *)root, GX_NULL);

    /* Create menu screen. */
    gx_studio_named_widget_create("Menu_Screen", GX_NULL, GX_NULL);

    /* Add child widgets to the drop-down list */
    PopulateDropList();

    /* Show the root window to make it and patients screen visible.  */
    gx_widget_show(root);

    /* start the GUIX thread */
    gx_system_start();
}


/*****************************************************************************/
/* Override event processing of "text screen" to update numeric prompt value.*/
/*****************************************************************************/
UINT text_screen_event_handler(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_SHOW:
        gx_system_timer_start(&Text_Screen, PROMPT_VALUE_UPDATE_TIMER, 20, 20);
        gx_window_event_process(window, event_ptr);
        break;

    case GX_EVENT_TIMER:
        if (event_ptr->gx_event_payload.gx_event_timer_id == PROMPT_VALUE_UPDATE_TIMER)
        {
            numeric_prompt_value++;

            if (numeric_prompt_value > 19999)
            {
                numeric_prompt_value = 10000;
            }
            gx_numeric_prompt_value_set(&Text_Screen.Text_Screen_numeric_prompt, numeric_prompt_value);
            gx_numeric_pixelmap_prompt_value_set(&Text_Screen.Text_Screen_numeric_pixelmap_prompt, numeric_prompt_value);
        }
        break;

    default:
        return gx_window_event_process(window, event_ptr);
    }
    return 0;
}

/*****************************************************************************/
/* Override event processing of "window screen" to implement canvas blend    */
/* animation.                                                                */
/*****************************************************************************/
UINT window_screen_event_handler(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    switch(event_ptr->gx_event_type)
    {
    case GX_EVENT_SHOW:
        gx_window_event_process(window, event_ptr);
        blend_alpha = 10;
        blend_increment = 8;
        gx_system_timer_start(window, BLEND_TIMER, 1, 1);
        break;

    case GX_EVENT_HIDE:
        gx_system_timer_stop(window, BLEND_TIMER);
        gx_window_event_process(window, event_ptr);
        break;

    case GX_EVENT_TIMER:
        gx_system_dirty_mark(&((WINDOW_SCREEN_CONTROL_BLOCK *)window)->Window_Screen_Nested_Window_BG);
        break;

    default:
        return gx_window_event_process(window, event_ptr);
    }
    return 0;
}

/*****************************************************************************/
/* Override drawing of "nested_window" to draw the window with specified     */
/* alpha.                                                                    */
/*****************************************************************************/
VOID nested_parent_window_draw(GX_WINDOW *window)
{
GX_BRUSH   *brush;

    gx_context_brush_get(&brush);

    blend_alpha += blend_increment;
    if (blend_alpha > 255)
    {
        blend_alpha = 255;
        blend_increment = -blend_increment;
    }
    else
    {
        if (blend_alpha < 0)
        {
            blend_alpha = 0;
            blend_increment = -blend_increment;
        }
    }
    brush -> gx_brush_alpha = blend_alpha;
    gx_window_draw(window);
}

/*****************************************************************************/
/* Update slider value.                                                      */
/*****************************************************************************/
void slider_value_update(GX_EVENT *event_ptr)
{
    INT pos;
    GX_STRING string;

    string.gx_string_ptr = slider_val_text;

    pos = event_ptr->gx_event_payload.gx_event_longdata;
    gx_progress_bar_value_set(&Slider_Screen.Slider_Screen_Progress_Bar, pos);
    gx_utility_ltoa(pos, (GX_CHAR *)string.gx_string_ptr, MAX_SLIDER_VAL_TEXT_LENGTH);
    strncat((GX_CHAR *)string.gx_string_ptr, "%", 1);
    string.gx_string_length = string_length_get(string.gx_string_ptr, MAX_SLIDER_VAL_TEXT_LENGTH);
    gx_prompt_text_set_ext(&Slider_Screen.Slider_Screen_Progress_Bar_Prompt, &string);
}

/*****************************************************************************/
/* Override event processing of "slider screen" to start animation for slider*/
/* widgets.                                                                  */
/*****************************************************************************/
UINT slider_screen_event_process(GX_WINDOW *window, GX_EVENT *event_ptr)
{

    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_SHOW:
        gx_window_event_process(window, event_ptr);

        /* save slider current value and reset current value to 0 */
        reset_sliders(window);
        gx_system_timer_start(window, SLIDER_ANIMATION_TIMER, 1, 1);
        break;

    case GX_SIGNAL(ID_PIXELMAP_SLIDER_THIN_H, GX_EVENT_SLIDER_VALUE):
        /* propogate this slider value to the progress bar */
        slider_value_update(event_ptr);
        break;

    case GX_EVENT_TIMER:
        if (event_ptr->gx_event_payload.gx_event_timer_id == SLIDER_ANIMATION_TIMER)
        {
            /* Animate the sliders, moving them towards target value */
            if (!update_sliders())
            {
                gx_system_timer_stop(window, SLIDER_ANIMATION_TIMER);
            }
        }
        break;

    default:
        return gx_window_event_process(window, event_ptr);
    }

    return 0;
}


/*****************************************************************************/
/* Look for each child slider on the slider screen, save its current value,  */
/* calculate increment to arrive at final value when animation completes.    */
/*****************************************************************************/
VOID reset_sliders(GX_WINDOW *parent)
{
    GX_WIDGET *widget;
    GX_SLIDER *slider;
    int slider_index;

    for (slider_index = 0; slider_index < NUM_SLIDERS; slider_index++)
    {
        slider_vals[slider_index].slider = GX_NULL;
    }

    slider_index = 0;
    widget = parent->gx_widget_first_child; 

    while(widget)
    {
        if (widget->gx_widget_type == GX_TYPE_SLIDER ||
            widget->gx_widget_type == GX_TYPE_PIXELMAP_SLIDER)
        {
            slider = (GX_SLIDER *) widget;
            slider_vals[slider_index].slider = slider;
            slider_vals[slider_index].target_val = slider->gx_slider_info.gx_slider_info_current_val << 4;
            slider_vals[slider_index].current_val = 0;
            slider_vals[slider_index].increment = slider_vals[slider_index].target_val / SLIDER_ANIMATION_STEPS;
            gx_slider_value_set(slider, &slider->gx_slider_info, 0);
            slider_index++;
        }
        widget= widget->gx_widget_next;
    }
}

/*****************************************************************************/
/* Go through the list of sliders and update each slider value               */
/*****************************************************************************/
GX_BOOL update_sliders()
{
    SLIDER_VAL_ENTRY *sval = slider_vals;
    int value;
    int status = GX_FALSE;

    while(sval->slider)
    {
        if (sval->current_val != sval->target_val)
        {
            status = GX_TRUE;
            value = sval->current_val;
            value += sval->increment;

            if (value > sval->target_val)
            {
                value = sval->target_val;
            }
            sval -> current_val = value;
            gx_slider_value_set(sval->slider, &sval->slider->gx_slider_info, value >> 4);
        }
        sval++;
    }
    return status;
}


/*****************************************************************************/
/* Create child element for the drop list. This function is called during    */
/* initial setup to populate children, and also called as the drop-list is   */
/* scrolled to update the child elements.                                    */
/*****************************************************************************/
VOID drop_list_row_create(GX_VERTICAL_LIST *list, GX_WIDGET *widget, INT index)
{
    GX_BOOL created;
    GX_RECTANGLE size;
    GX_CHAR temp[10];
    GX_STRING string;

    DROP_LIST_WIDGET *entry = (DROP_LIST_WIDGET *)widget;

    strcpy(entry->text, "List Entry #");
    gx_utility_ltoa(index + 1, temp, 10);
    strcat(entry->text, (char*)temp);

    gx_widget_created_test(widget, &created);

    if (!created)
    {
        gx_utility_rectangle_define(&size, 0, 0, 100, 44);
        gx_prompt_create(&entry->prompt, entry->text, list, 0, GX_STYLE_ENABLED | GX_STYLE_TEXT_LEFT | GX_STYLE_BORDER_NONE, 0, &size);
        gx_widget_fill_color_set(&entry->prompt, GX_COLOR_ID_TEAL, GX_COLOR_ID_DARK_TEAL, GX_COLOR_ID_TEAL);
        gx_prompt_text_color_set(&entry->prompt, GX_COLOR_ID_WHITE, GX_COLOR_ID_WHITE, GX_COLOR_ID_WHITE);
    }

    string.gx_string_ptr = entry->text;
    string.gx_string_length = string_length_get(entry->text, MAX_ROW_TEXT_LENGTH);
    gx_prompt_text_set_ext(&entry->prompt, &string);
}

/*****************************************************************************/
/* Create initial set of child elements for drop list                        */
/*****************************************************************************/
VOID PopulateDropList(void)
{
    int index;
    GX_VERTICAL_LIST *list;
    GX_SCROLLBAR_APPEARANCE sa;

    WINDOW_SCREEN_CONTROL_BLOCK *control = &Window_Screen;
    GX_DROP_LIST *drop = &control->Window_Screen_Drop_List;
    gx_drop_list_popup_get(drop, &list);

    for (index = 0; index < DROP_LIST_VISIBLE_ROWS; index++)
    {
        drop_list_row_create(list, (GX_WIDGET *)&drop_list_widgets[index], index);
    }

    sa.gx_scroll_thumb_pixelmap = GX_PIXELMAP_ID_LIST_SCROLL_THUMB;
    sa.gx_scroll_thumb_travel_min = 4;
    sa.gx_scroll_thumb_travel_max = 4;
    sa.gx_scroll_thumb_width = 8;
    sa.gx_scroll_width = 10;

    gx_vertical_scrollbar_create(&list_scroll, "list_scroll", list,
        &sa, GX_SCROLLBAR_VERTICAL|GX_STYLE_ENABLED);
}

/*****************************************************************************/
/* Draw the custom border for icon button widget                             */
/*****************************************************************************/
VOID custom_icon_button_draw(GX_ICON_BUTTON *button)
{
    GX_PIXELMAP *map;
    INT icon_offset;
    INT xpos;
    INT ypos;

    // draw the background
    if (button->gx_widget_style & GX_STYLE_BUTTON_PUSHED)
    {
        gx_context_pixelmap_get(GX_PIXELMAP_ID_BUTTON_SM_ACTIVE, &map);
        icon_offset = 1;
    }
    else
    {
        gx_context_pixelmap_get(GX_PIXELMAP_ID_BUTTON_SM, &map);
        icon_offset = 0;
    }
    gx_canvas_pixelmap_draw(button->gx_widget_size.gx_rectangle_left,
                            button->gx_widget_size.gx_rectangle_top,
                            map);

    // draw the icon
    switch (button->gx_widget_id)
    {
    case ICON_BUTTON_DISC:
        gx_context_pixelmap_get(GX_PIXELMAP_ID_ICON_DISC, &map);
        break;

    case ICON_BUTTON_USER:
        gx_context_pixelmap_get(GX_PIXELMAP_ID_ICON_USER, &map);
        break;

    case ICON_BUTTON_PEN:
        gx_context_pixelmap_get(GX_PIXELMAP_ID_ICON_PEN, &map);
        break;

    case ICON_BUTTON_TRASH:
        gx_context_pixelmap_get(GX_PIXELMAP_ID_ICON_TRASH, &map);
        break;

    case ICON_BUTTON_COG:
        gx_context_pixelmap_get(GX_PIXELMAP_ID_ICON_COG, &map);
        break;

    default:
        break;
    }

    xpos = button->gx_widget_size.gx_rectangle_right - button->gx_widget_size.gx_rectangle_left + 1;
    xpos -= map->gx_pixelmap_width;
    xpos /= 2;
    xpos += button->gx_widget_size.gx_rectangle_left + icon_offset;

    ypos = button->gx_widget_size.gx_rectangle_bottom - button->gx_widget_size.gx_rectangle_top + 1;
    ypos -= map->gx_pixelmap_height;
    ypos /= 2;
    ypos += button->gx_widget_size.gx_rectangle_top + icon_offset;
    gx_canvas_pixelmap_draw(xpos, ypos, map);
    gx_widget_children_draw(button);
}

/*****************************************************************************/
/* Draw the custom border for text button widget                             */
/*****************************************************************************/
VOID custom_text_button_draw(GX_TEXT_BUTTON *button)
{
    GX_PIXELMAP *map;

    // draw the background
    if (button->gx_widget_style & GX_STYLE_BUTTON_PUSHED)
    {
        gx_context_pixelmap_get(GX_PIXELMAP_ID_BUTTON_ACTIVE, &map);
    }
    else
    {
        gx_context_pixelmap_get(GX_PIXELMAP_ID_BUTTON, &map);
    }
    gx_canvas_pixelmap_draw(button->gx_widget_size.gx_rectangle_left,
                            button->gx_widget_size.gx_rectangle_top,
                            map);
    gx_text_button_text_draw(button);
    gx_widget_children_draw(button);
}

/*****************************************************************************/
/* Define a custom button draw function.                                     */
/*****************************************************************************/
VOID custom_button_draw(GX_BUTTON *button)
{
    GX_PIXELMAP *map;
    // draw the background
    if (button->gx_widget_style & GX_STYLE_BUTTON_PUSHED)
    {
        gx_context_pixelmap_get(GX_PIXELMAP_ID_BUTTON_ACTIVE, &map);
    }
    else
    {
        gx_context_pixelmap_get(GX_PIXELMAP_ID_BUTTON, &map);
    }
    gx_canvas_pixelmap_draw(button->gx_widget_size.gx_rectangle_left,
                            button->gx_widget_size.gx_rectangle_top,
                            map);
    gx_widget_children_draw(button);
}

/*****************************************************************************/
/* Define a custom draw function of multi-line text button.                  */
/*****************************************************************************/
VOID custom_multi_line_text_button_draw(GX_MULTI_LINE_TEXT_BUTTON *button)
{
    GX_PIXELMAP *map;

    // draw the background
    if (button->gx_widget_style & GX_STYLE_BUTTON_PUSHED)
    {
        gx_context_pixelmap_get(GX_PIXELMAP_ID_BUTTON_LG_ACTIVE, &map);
    }
    else
    {
        gx_context_pixelmap_get(GX_PIXELMAP_ID_BUTTON_LG, &map);
    }
    gx_canvas_pixelmap_draw(button->gx_widget_size.gx_rectangle_left,
                            button->gx_widget_size.gx_rectangle_top,
                            map);

    gx_multi_line_text_button_text_draw(button);
    gx_widget_children_draw(button);
}

/*****************************************************************************/
/* Draw custom thick border for transparent prompt                           */
/*****************************************************************************/
VOID custom_transparent_prompt_thick_border_draw(GX_PROMPT *prompt)
{
    GX_PIXELMAP *map;

    gx_prompt_draw(prompt);

    gx_context_pixelmap_get(GX_PIXELMAP_ID_PROMPT_BORDER_HEAVY, &map);
    
    gx_canvas_pixelmap_draw(prompt->gx_widget_size.gx_rectangle_left,
        prompt->gx_widget_size.gx_rectangle_top,
        map);

    gx_prompt_text_draw(prompt);
    gx_widget_children_draw(prompt);
}

/*****************************************************************************/
/* Override event processing of "scroll frame".                              */
/*****************************************************************************/
UINT scroll_frame_event_handler(GX_WINDOW *frame, GX_EVENT *event_ptr)
{
    if (event_ptr->gx_event_type == GX_EVENT_SHOW)
    {
        gx_window_event_process(frame, event_ptr);
        /* adjust my client are just enough to show the wide frame */
        frame->gx_window_client.gx_rectangle_bottom -= 4;
        return 0;
    }
    else
    {
        return gx_window_event_process(frame, event_ptr);
    }
}

/*****************************************************************************/
/* Custom window drawing. Draw the rounded corners of the scroll frame window*/
/*****************************************************************************/
VOID scroll_frame_draw(GX_WINDOW *frame)
{
GX_PIXELMAP *map;

    gx_window_draw(frame);

    /* draw the rounded corners */
    gx_context_pixelmap_get(GX_PIXELMAP_ID_UL_CORNER, &map);
    gx_canvas_pixelmap_draw(frame->gx_widget_size.gx_rectangle_left,
                            frame->gx_widget_size.gx_rectangle_top, map);

    gx_context_pixelmap_get(GX_PIXELMAP_ID_UR_CORNER, &map);
    gx_canvas_pixelmap_draw(frame->gx_widget_size.gx_rectangle_right - map->gx_pixelmap_width + 1,
                            frame->gx_widget_size.gx_rectangle_top, map);

    gx_context_pixelmap_get(GX_PIXELMAP_ID_LL_CORNER, &map);
    gx_canvas_pixelmap_draw(frame->gx_widget_size.gx_rectangle_left,
                            frame->gx_widget_size.gx_rectangle_bottom - map->gx_pixelmap_height + 1, map);

    gx_context_pixelmap_get(GX_PIXELMAP_ID_LR_CORNER, &map);
    gx_canvas_pixelmap_draw(frame->gx_widget_size.gx_rectangle_right - map->gx_pixelmap_width + 1,
                            frame->gx_widget_size.gx_rectangle_bottom - map->gx_pixelmap_height + 1, map);
        
}

/*****************************************************************************/
/* Simple custom drawing for the next button, which is just a pixelmap       */
/*****************************************************************************/
VOID custom_next_button_draw(GX_PIXELMAP_BUTTON *pixelmap_button)
{
    GX_VALUE x_pos, y_pos;
    GX_PIXELMAP *map = GX_NULL;

    gx_context_pixelmap_get(GX_PIXELMAP_ID_NEXT_BUTTON_2, &map);
    x_pos = pixelmap_button->gx_widget_size.gx_rectangle_left;
    y_pos = pixelmap_button->gx_widget_size.gx_rectangle_top;

    if (pixelmap_button -> gx_widget_style & GX_STYLE_BUTTON_PUSHED)
    {
        x_pos += 2;
        y_pos += 2;
    }

    if (map)
    {
        gx_canvas_pixelmap_draw(x_pos, y_pos, map);
    }
}

/*****************************************************************************/
/* Define a function to format numeric prompt value.                         */
/*****************************************************************************/
VOID numeric_prompt_format_func(GX_NUMERIC_PROMPT *prompt, INT value)
{
    gx_utility_ltoa(value % 10, prompt->gx_numeric_prompt_buffer, GX_NUMERIC_PROMPT_BUFFER_SIZE);
}

/*****************************************************************************/
/* Define a function to fromat numeric pixelmap prompt value                 */
/*****************************************************************************/
VOID numeric_pixelmap_prompt_format_func(GX_NUMERIC_PIXELMAP_PROMPT *prompt, INT value)
{
    int length;
    gx_utility_ltoa(value / 100, prompt->gx_numeric_pixelmap_prompt_buffer, GX_NUMERIC_PROMPT_BUFFER_SIZE);
    
    length = string_length_get(prompt->gx_numeric_pixelmap_prompt_buffer, GX_NUMERIC_PROMPT_BUFFER_SIZE);
    prompt->gx_numeric_pixelmap_prompt_buffer[length++] = '.';
    gx_utility_ltoa(value % 100, prompt->gx_numeric_pixelmap_prompt_buffer + length, GX_NUMERIC_PROMPT_BUFFER_SIZE - length);
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
