/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include <string.h>
#include "gx_api.h"
#include "screen_drag_animation_with_canvas_resources.h"
#include "screen_drag_animation_with_canvas_specifications.h"


#define ANIMATION_ID_H_SCREEN_DRAG 1
#define ANIMATION_ID_V_SCREEN_DRAG 2
#define H_ANIMATION_CANVAS_WIDTH 279 * 2
#define H_ANIMATION_CANVAS_HEIGHT 53
#define V_ANIMATION_CANVAS_WIDTH 78
#define V_ANIMATION_CANVAS_HEIGHT 248 * 2

/* Define the ThreadX demo thread control block and stack.  */
GX_WINDOW_ROOT    *root;

/* Define prototypes.   */
extern UINT win32_graphics_driver_setup_24xrgb(GX_DISPLAY *display);
VOID create_screen_drag_animations();

GX_ANIMATION h_screen_drag_animation;
GX_ANIMATION v_screen_drag_animation;
INT    h_animation_count = 0;
INT    v_animation_count = 0;

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

GX_WINDOW_ROOT     h_animation_root;
GX_WINDOW_ROOT     v_animation_root;
GX_COLOR           composite_canvas_memory[DISPLAY_1_X_RESOLUTION * DISPLAY_1_Y_RESOLUTION];
GX_COLOR           h_animation_canvas_memory[H_ANIMATION_CANVAS_WIDTH * H_ANIMATION_CANVAS_HEIGHT];
GX_COLOR           v_animation_canvas_memory[V_ANIMATION_CANVAS_WIDTH * V_ANIMATION_CANVAS_HEIGHT];
GX_CANVAS          composite_canvas;
GX_CANVAS          h_animation_canvas;
GX_CANVAS          v_animation_canvas;

int main(int argc, char ** argv)
{
    tx_kernel_enter();
    return(0);
}

VOID tx_application_define(void *first_unused_memory)
{

    /* Initialize GUIX.  */
    gx_system_initialize();

    /* Configure display.  */
    gx_studio_display_configure(DISPLAY_1, win32_graphics_driver_setup_24xrgb,
                                LANGUAGE_ENGLISH, DISPLAY_1_THEME_1, &root);

    /* Create the main screen.  */
    gx_studio_named_widget_create("main_screen", (GX_WIDGET *) root, GX_NULL);

    /* Create menu windows.  */
    gx_studio_named_widget_create("h_menu_window_2", GX_NULL, GX_NULL);
    gx_studio_named_widget_create("h_menu_window_3", GX_NULL, GX_NULL);
    gx_studio_named_widget_create("v_menu_window_2", GX_NULL, GX_NULL);
    gx_studio_named_widget_create("v_menu_window_3", GX_NULL, GX_NULL);

    create_screen_drag_animations();

    /* Show the root window to make it and patients screen visible.  */
    gx_widget_show(root);

    /* start the GUIX thread */
    gx_system_start();
}

static VOID enable_screen_drag_animation(GX_ANIMATION* animation, GX_WIDGET** screen_list, GX_WIDGET* parent, USHORT style, USHORT id)
{
    GX_ANIMATION_INFO info;

    memset(&info, 0, sizeof(GX_ANIMATION_INFO));
    info.gx_animation_parent = parent;
    info.gx_animation_style = style;
    info.gx_animation_id = id;
    info.gx_animation_frame_interval = 1;
    info.gx_animation_steps = 20;
    info.gx_animation_slide_screen_list = screen_list;
    info.gx_animation_start_alpha = 255;
    info.gx_animation_end_alpha = 255;
    gx_animation_drag_enable(animation, parent, &info);
}

VOID create_screen_drag_animations()
{
    GX_RECTANGLE size;

    /* Create the composite canvas used during animations */
    gx_canvas_create(&composite_canvas, GX_NULL,
        root->gx_window_root_canvas->gx_canvas_display,
        GX_CANVAS_COMPOSITE,
        root->gx_widget_size.gx_rectangle_right - root->gx_widget_size.gx_rectangle_left + 1,
        root->gx_widget_size.gx_rectangle_bottom - root->gx_widget_size.gx_rectangle_top + 1,
        composite_canvas_memory, sizeof(composite_canvas_memory));
    gx_canvas_alpha_set(&composite_canvas, 255);

    /* Create animation canvas. */
    gx_canvas_create(&h_animation_canvas, GX_NULL,
        root->gx_window_root_canvas->gx_canvas_display,
        GX_CANVAS_MANAGED_VISIBLE,
        H_ANIMATION_CANVAS_WIDTH, H_ANIMATION_CANVAS_HEIGHT,
        h_animation_canvas_memory, sizeof(h_animation_canvas_memory));
    gx_canvas_hide(&h_animation_canvas);

    gx_canvas_create(&v_animation_canvas, GX_NULL,
        root->gx_window_root_canvas->gx_canvas_display,
        GX_CANVAS_MANAGED_VISIBLE,
        V_ANIMATION_CANVAS_WIDTH, V_ANIMATION_CANVAS_HEIGHT,
        v_animation_canvas_memory, sizeof(v_animation_canvas_memory));
    gx_canvas_hide(&v_animation_canvas);

    /* create the root window used for screen slide animation */
    gx_utility_rectangle_define(&size, 0, 0, H_ANIMATION_CANVAS_WIDTH - 1, H_ANIMATION_CANVAS_HEIGHT - 1);
    gx_window_root_create(&h_animation_root, "animation root", &h_animation_canvas, GX_STYLE_BORDER_NONE, GX_NULL, &size);
    gx_widget_fill_color_set(&h_animation_root, GX_COLOR_ID_WINDOW_FILL, GX_COLOR_ID_WINDOW_FILL, GX_COLOR_ID_WINDOW_FILL);
    gx_utility_rectangle_define(&size, 0, 0, V_ANIMATION_CANVAS_WIDTH - 1, V_ANIMATION_CANVAS_HEIGHT - 1);
    gx_window_root_create(&v_animation_root, "animation root", &v_animation_canvas, GX_STYLE_BORDER_NONE, GX_NULL, &size);
    gx_widget_fill_color_set(&v_animation_root, GX_COLOR_ID_WINDOW_FILL, GX_COLOR_ID_WINDOW_FILL, GX_COLOR_ID_WINDOW_FILL);

    /* Create screen drag animations.  */
    memset(&h_screen_drag_animation, 0, sizeof(GX_ANIMATION));
    memset(&v_screen_drag_animation, 0, sizeof(GX_ANIMATION));
    gx_animation_create(&h_screen_drag_animation);
    gx_animation_create(&v_screen_drag_animation);

    gx_animation_canvas_define(&h_screen_drag_animation, &h_animation_canvas);
    gx_animation_canvas_define(&v_screen_drag_animation, &v_animation_canvas);

    enable_screen_drag_animation(&h_screen_drag_animation, h_screen_list, &main_screen.main_screen_h_menu_window,
        GX_ANIMATION_SCREEN_DRAG | GX_ANIMATION_HORIZONTAL | GX_ANIMATION_WRAP, ANIMATION_ID_H_SCREEN_DRAG);

    enable_screen_drag_animation(&v_screen_drag_animation, v_screen_list, &main_screen.main_screen_v_menu_window,
        GX_ANIMATION_SCREEN_DRAG | GX_ANIMATION_VERTICAL | GX_ANIMATION_WRAP, ANIMATION_ID_V_SCREEN_DRAG);
}

UINT h_menu_window_event_handler(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
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