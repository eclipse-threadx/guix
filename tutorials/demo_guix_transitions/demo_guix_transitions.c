/* This is a small demo of the high-performance GUIX graphics framework. */
/*
   This demonstration illustrates how to implement slide and fade transtions when
   moving from screen to screen.

   Actually, the work of doing most of the transitions can be done using just 
   the GUIX Studio screen flow diagram, which generates the logic to implement slide and fade screen
   transitions for you. However, for the purpose of a tutorial, we have implemented the slide-in
   slide-out animations of the settings window in the application logic, and change the slide
   direction with each button click.

   The fade-in/fade-out animation of the about window is handled by the Studio screen flow diagram.

   The application also implements a screen-drag animation for the three photo screens, which
   must be initialized in the application logic.
*/

#include <stdio.h>
#include "gx_api.h"

#include "demo_guix_transitions_resources.h"
#include "demo_guix_transitions_specifications.h"

/* Define the ThreadX demo thread control block and stack.  */

GX_WINDOW_ROOT    *root;
GX_ANIMATION      *drag_animation = GX_NULL;
GX_ANIMATION_INFO  drag_animation_info;

GX_ANIMATION      *transition_animation;  
GX_ANIMATION_INFO  transition_animation_info;

GX_WIDGET *drag_screen_list[] = {
    (GX_WIDGET *) &photo_1,
    (GX_WIDGET *) &photo_2,
    (GX_WIDGET *) &photo_3,
    GX_NULL
};

enum slide_directions {
    SLIDE_UP = 0,
    SLIDE_DOWN,
    SLIDE_RIGHT,
    SLIDE_LEFT,
    MAX_SLIDE_DIRECTIONS
};

enum animation_ids {
    ANIMATION_SETTINGS_SLIDE_IN = GX_NEXT_ANIMATION_ID,
    ANIMATION_SETTINGS_SLIDE_OUT
};

int slide_direction = SLIDE_DOWN;

/* Define prototypes.   */

VOID  start_guix(VOID);
extern UINT win32_graphics_driver_setup_565rgb(GX_DISPLAY *display);

int main(int argc, char ** argv)
{
  tx_kernel_enter();
  return(0);
}

VOID tx_application_define(void *first_unused_memory)
{
    start_guix();
}

VOID  start_guix(VOID)
{
GX_RECTANGLE size;

    /* Initialize GUIX.  */
    gx_system_initialize();

    gx_studio_display_configure(DISPLAY_1, win32_graphics_driver_setup_565rgb, 
                                LANGUAGE_ENGLISH, DISPLAY_1_DEFAULT_THEME, &root);

    /* create the main screen */
    gx_studio_named_widget_create("background_win", (GX_WIDGET *) root, GX_NULL);

    /* create the info screen */
    gx_studio_named_widget_create("about_win", GX_NULL, GX_NULL);

    /* create the settings screen */
    gx_studio_named_widget_create("settings_win", GX_NULL, GX_NULL);

    /* create the three photo windows, and make the first one visible */
    gx_studio_named_widget_create("photo_1", (GX_WIDGET *) &background_win.background_win_child_frame, GX_NULL);
    gx_studio_named_widget_create("photo_2", GX_NULL, GX_NULL);
    gx_studio_named_widget_create("photo_3", GX_NULL, GX_NULL);

    /* position the three photo windows inside the outer frame */

    gx_widget_client_get(&background_win.background_win_child_frame, -1, &size);
    gx_widget_resize(&photo_1, &size);
    gx_widget_resize(&photo_2, &size);
    gx_widget_resize(&photo_3, &size);

    /* make the root and all children visible */
    gx_widget_show(root);

    /* let GUIX thread run */
    gx_system_start();
}

/*
    This slide animation can easily be configured in the GUIX Studio screen flow diagram,
    however since we want to use this as a tutorial, we decided to change the
    animation direction with each push of the settings button. GUIX Studio can only
    define one animation type for each event, so it's a useful to program the slide
    animation in the application logic.

    There are MANY other ways to do this (changing the button ID is one example).

    The fade animation is performed completly with the Studio screen flow diagram.
*/

VOID InitSlideTransition(int animation_id)
{
GX_VALUE offset;
GX_WIDGET *frame;

    frame = (GX_WIDGET *) &background_win.background_win_child_frame;

    // allocate animation control block from system pool
    gx_system_animation_get(&transition_animation);

    // fully opaque
    transition_animation_info.gx_animation_end_alpha = 255;
    transition_animation_info.gx_animation_start_alpha = 255;

    // end position inside child frame:
    transition_animation_info.gx_animation_end_position.gx_point_x = frame->gx_widget_size.gx_rectangle_left + 1;
    transition_animation_info.gx_animation_end_position.gx_point_y = frame->gx_widget_size.gx_rectangle_top + 1;

    // calculate start position:
    transition_animation_info.gx_animation_start_position = transition_animation_info.gx_animation_end_position;

    switch(slide_direction)
    {
    case SLIDE_DOWN:
        gx_widget_height_get(frame, &offset);
        if (animation_id == ANIMATION_SETTINGS_SLIDE_IN)
        {
            transition_animation_info.gx_animation_start_position.gx_point_y -= offset;
        }
        else
        {
            transition_animation_info.gx_animation_end_position.gx_point_y += offset;
        }
        break;

    case SLIDE_UP:
        gx_widget_height_get(frame, &offset);
        if (animation_id == ANIMATION_SETTINGS_SLIDE_IN)
        {
            transition_animation_info.gx_animation_start_position.gx_point_y += offset;
        }
        else
        {
            transition_animation_info.gx_animation_end_position.gx_point_y -= offset;
        }
        break;

    case SLIDE_LEFT:
        gx_widget_width_get(frame, &offset);
        if (animation_id == ANIMATION_SETTINGS_SLIDE_IN)
        {
            transition_animation_info.gx_animation_start_position.gx_point_x += offset;
        }
        else
        {
            transition_animation_info.gx_animation_end_position.gx_point_x -= offset;
        }
        break;

    case SLIDE_RIGHT:
        gx_widget_width_get(frame, &offset);
        if (animation_id == ANIMATION_SETTINGS_SLIDE_IN)
        {
            transition_animation_info.gx_animation_start_position.gx_point_x -= offset;
        }
        else
        {
            transition_animation_info.gx_animation_end_position.gx_point_x += offset;
        }
        break;
    }

    transition_animation_info.gx_animation_frame_interval = 1;
    transition_animation_info.gx_animation_steps = 10;
    transition_animation_info.gx_animation_parent = frame;
    transition_animation_info.gx_animation_target = (GX_WIDGET *) &settings_win;
    transition_animation_info.gx_animation_style = GX_ANIMATION_TRANSLATE | GX_ANIMATION_SINE_EASE_IN;

    // if we are sliding out, detach the settings window when the animation is complete:

    if (animation_id == ANIMATION_SETTINGS_SLIDE_OUT)
    {
        transition_animation_info.gx_animation_style |= GX_ANIMATION_DETACH;

        /* increment the slide direction */
        slide_direction++;

        if (slide_direction == MAX_SLIDE_DIRECTIONS)
        {
            slide_direction = 0;
        }
    }
    transition_animation_info.gx_animation_id = animation_id;
    gx_animation_start(transition_animation, &transition_animation_info);

}

/* Event handler for the background window. Initializes the photo-drag 
   animation, and catches the "Settings" button push.
*/
UINT BackgroundWinEventProcess(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    switch(event_ptr->gx_event_type)
    {
    case GX_EVENT_SHOW:
        gx_window_event_process(window, event_ptr);

        /* grab an animation control block for the screen drag animation */

        if (!drag_animation)
        {
            gx_system_animation_get(&drag_animation);
            drag_animation ->gx_animation_landing_speed = 10;

            memset(&drag_animation_info, 0, sizeof(GX_ANIMATION_INFO));
            drag_animation_info.gx_animation_parent = (GX_WIDGET *) &background_win.background_win_child_frame;
            drag_animation_info.gx_animation_style = GX_ANIMATION_SCREEN_DRAG | GX_ANIMATION_WRAP | GX_ANIMATION_HORIZONTAL;
            drag_animation_info.gx_animation_frame_interval = 1;
            drag_animation_info.gx_animation_slide_screen_list = drag_screen_list;
            gx_animation_drag_enable(drag_animation, (GX_WIDGET *) &background_win.background_win_child_frame, &drag_animation_info);
        }
        break;

    case GX_SIGNAL(IDB_SETTINGS, GX_EVENT_CLICKED):
        if (!(settings_win.gx_widget_status & GX_STATUS_VISIBLE))
        {
            gx_widget_style_remove(&background_win.background_win_info_button, GX_STYLE_ENABLED);
            InitSlideTransition(ANIMATION_SETTINGS_SLIDE_IN);
        }
        break;

    case GX_EVENT_ANIMATION_COMPLETE:
        switch(event_ptr->gx_event_sender)
        {
        case ANIMATION_SETTINGS_SLIDE_OUT:
            gx_widget_style_add(&background_win.background_win_info_button, GX_STYLE_ENABLED);
            break;
        }
        break;

    default:
        return gx_window_event_process(window, event_ptr);;
    }
    return 0;
}


/* Since we didn't implement the screen slide in Studio (because we want to change
   slide directions), we need to catch the CLOSE button here and initiate the
   "slide out" animation
*/
UINT settings_win_event_process(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    switch(event_ptr->gx_event_type)
    {
    case GX_SIGNAL(IDB_CLOSE, GX_EVENT_CLICKED):
        InitSlideTransition(ANIMATION_SETTINGS_SLIDE_OUT);
        break;

    default:
        return gx_window_event_process(window, event_ptr);
    }
    return GX_SUCCESS;
}