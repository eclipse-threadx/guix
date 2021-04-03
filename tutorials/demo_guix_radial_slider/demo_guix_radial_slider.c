/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"
#include "demo_guix_radial_slider_resources.h"
#include "demo_guix_radial_slider_specifications.h"

/* Define variables.  */
GX_WINDOW_ROOT    *root;

/* Define radial slider anchor angle list. */
GX_VALUE radial_slider_anchor_list[] = { -63, -34, -10, 13, 38, 67, 112, 142, 168, 191, 214, 242 };
INT radial_slider_animation_step = 0;

/* Define prototypes.   */
VOID start_guix(VOID);
extern UINT win32_graphics_driver_setup_24xrgb(GX_DISPLAY *display);

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
    start_guix();
}

/******************************************************************************************/
/* Initiate and run GUIX.                                                                 */
/******************************************************************************************/
VOID start_guix(VOID)
{
    /* Initialize GUIX.  */
    gx_system_initialize();

    /* Configure display. */
    gx_studio_display_configure(DISPLAY_1, win32_graphics_driver_setup_24xrgb,
                                LANGUAGE_ENGLISH, DISPLAY_1_THEME_1, &root);

    /* create the main screen */
    gx_studio_named_widget_create("main_screen", (GX_WIDGET *) root, GX_NULL);

    /* Show the root window to make it and patients screen visible.  */
    gx_widget_show(root);

    /* let GUIX run */
    gx_system_start();
}

/******************************************************************************************/
/* Application defined callback function that will be called after each radial slider     */
/* animation step.                                                                        */
/******************************************************************************************/
VOID radial_slider_animation_callback(GX_RADIAL_SLIDER *slider)
{
    radial_slider_animation_step++;
    gx_numeric_prompt_value_set(&main_screen.main_screen_animation_step_prompt, radial_slider_animation_step);
}

/******************************************************************************************/
/* Override the default event processing of "main_screen" to handle signals from my child */
/* widgets.                                                                               */
/******************************************************************************************/
UINT main_screen_event_handler(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
    case GX_SIGNAL(ID_ANIMATED, GX_EVENT_TOGGLE_ON):
        gx_radial_slider_animation_set(&main_screen.main_screen_radial_slider, 15, 2, GX_ANIMATION_CIRC_EASE_IN_OUT, GX_NULL);
        break;

    case GX_SIGNAL(ID_ANIMATED, GX_EVENT_TOGGLE_OFF):
        gx_radial_slider_animation_set(&main_screen.main_screen_radial_slider, 0, 0, 0, GX_NULL);
        break;

    case GX_SIGNAL(ID_SET_ANCHOR_LIST, GX_EVENT_TOGGLE_ON):
        gx_radial_slider_anchor_angles_set(&main_screen.main_screen_radial_slider, radial_slider_anchor_list, sizeof(radial_slider_anchor_list) / sizeof(GX_VALUE));
        break;

    case GX_SIGNAL(ID_SET_ANCHOR_LIST, GX_EVENT_TOGGLE_OFF):
        gx_radial_slider_anchor_angles_set(&main_screen.main_screen_radial_slider, GX_NULL, 0);
        break;

    case GX_SIGNAL(ID_RADIAL_SLIDER, GX_EVENT_SLIDER_VALUE):
        gx_numeric_prompt_value_set(&main_screen.main_screen_radial_slider_value, event_ptr->gx_event_payload.gx_event_intdata[0]);
        break;

    case GX_SIGNAL(ID_RADIAL_SLIDER, GX_EVENT_ANIMATION_START):
        radial_slider_animation_step = 0;
        break;

    case GX_SIGNAL(ID_RADIAL_SLIDER, GX_EVENT_ANIMATION_COMPLETE):
        break;

    default:
        return gx_window_event_process(window, event_ptr);
    }

    return 0;
}