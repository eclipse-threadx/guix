/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include <string.h>
#include "gx_api.h"
#include "screen_drag_animation_with_block_move_resources.h"
#include "screen_drag_animation_with_block_move_specifications.h"


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
    /* Create screen drag animations.  */
    memset(&h_screen_drag_animation, 0, sizeof(GX_ANIMATION));
    memset(&v_screen_drag_animation, 0, sizeof(GX_ANIMATION));
    gx_animation_create(&h_screen_drag_animation);
    gx_animation_create(&v_screen_drag_animation);

    enable_screen_drag_animation(&h_screen_drag_animation, h_screen_list, &main_screen.main_screen_h_menu_window,
        GX_ANIMATION_SCREEN_DRAG | GX_ANIMATION_HORIZONTAL | GX_ANIMATION_WRAP | GX_ANIMATION_BLOCK_MOVE, ANIMATION_ID_H_SCREEN_DRAG);

    enable_screen_drag_animation(&v_screen_drag_animation, v_screen_list, &main_screen.main_screen_v_menu_window,
        GX_ANIMATION_SCREEN_DRAG | GX_ANIMATION_VERTICAL | GX_ANIMATION_WRAP | GX_ANIMATION_BLOCK_MOVE, ANIMATION_ID_V_SCREEN_DRAG);
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