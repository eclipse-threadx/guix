/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"

#include "guix_medical_resources.h"
#include "guix_medical_specifications.h"

GX_WINDOW_ROOT    *root;

/* Define prototypes.   */
VOID  start_guix(VOID);
extern UINT win32_graphics_driver_setup_24xrgb(GX_DISPLAY *display);
extern VOID patient_list_children_create(PATIENTS_SCREEN_CONTROL_BLOCK* win);
extern VOID hide_active_bar();

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

    /* Configure display.  */
    gx_studio_display_configure(MAIN_DISPLAY, win32_graphics_driver_setup_24xrgb, 
                                LANGUAGE_ENGLISH, MAIN_DISPLAY_DEFAULT_THEME, &root);

    /* Create the vitals screen.  */
    gx_studio_named_widget_create("vitals_screen", (GX_WIDGET*)root, GX_NULL);

    /* Create the schedule screen.  */
    gx_studio_named_widget_create("schedule_screen", GX_NULL, GX_NULL);
    hide_active_bar();

    /* Create the patients screen.  */
    gx_studio_named_widget_create("patients_screen", GX_NULL, GX_NULL);
    patient_list_children_create(&patients_screen);

    /* Show the root window to make it and patients screen visible.  */
    gx_widget_show(root);

    /* Let GUIX run.  */
    gx_system_start();
}

/******************************************************************************************/
/* Toggle Screen.                                                                         */
/******************************************************************************************/
VOID ToggleScreen(GX_WIDGET *new_win, GX_WIDGET *old_win)
{
    gx_widget_hide(old_win);

    if (!new_win->gx_widget_parent)
    {
        gx_widget_attach(root, new_win);
    }
    else
    {
        gx_widget_show(new_win);
    }
}

/******************************************************************************************/
/* Override the default event processing of "template" to handle signals from my child    */
/* widgets.                                                                               */
/******************************************************************************************/
UINT template_event_handler(GX_WINDOW* window, GX_EVENT* event_ptr)
{
UINT                    status = GX_SUCCESS;
TEMPLATE_CONTROL_BLOCK *template = (TEMPLATE_CONTROL_BLOCK *)window;

    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_SHOW:
        gx_window_event_process(window, event_ptr);

        /* Set navigation buttons as unselected.  */
        gx_widget_style_remove(&template->template_vitals_button, GX_STYLE_BUTTON_PUSHED);
        gx_widget_style_remove(&template->template_schedule_button, GX_STYLE_BUTTON_PUSHED);
        gx_widget_style_remove(&template->template_patients_button, GX_STYLE_BUTTON_PUSHED);

        /* Selected navigation button according to the active screen.  */
        switch (window->gx_widget_id)
        {
        case ID_VITALS_SCREEN:
            gx_widget_style_add(&template->template_vitals_button, GX_STYLE_BUTTON_PUSHED);
            break;

        case ID_SCHEDULE_SCREEN:
            gx_widget_style_add(&template->template_schedule_button, GX_STYLE_BUTTON_PUSHED);
            break;

        case ID_PATIENTS_SCREEN:
            gx_widget_style_add(&template->template_patients_button, GX_STYLE_BUTTON_PUSHED);
            break;
        }
        break;

    case GX_SIGNAL(ID_BTN_VITALS, GX_EVENT_RADIO_SELECT):
    case GX_SIGNAL(ID_PATIENT_LIST, GX_EVENT_LIST_SELECT):

        /* Toggle to vitals screen.  */
        ToggleScreen((GX_WIDGET *) &vitals_screen, (GX_WIDGET*)window);
        break;

    case GX_SIGNAL(ID_BTN_SCHEDULE, GX_EVENT_RADIO_SELECT):

        /* Toggle to schedule screen.  */
        ToggleScreen((GX_WIDGET*) &schedule_screen, (GX_WIDGET*)window);
        break;

    case GX_SIGNAL(ID_BTN_PATIENTS, GX_EVENT_RADIO_SELECT):

        /* Toggle to patients screen.  */
        ToggleScreen((GX_WIDGET*) &patients_screen, (GX_WIDGET*)window);
        break;

    case GX_EVENT_HIDE:
        if (window->gx_widget_id == ID_VITALS_SCREEN)
        {
            gx_animation_delete(GX_NULL, (GX_WIDGET *)window);
        }
        else if (window->gx_widget_id == ID_SCHEDULE_SCREEN)
        {
            gx_animation_delete(GX_NULL, (GX_WIDGET *)&schedule_screen.schedule_screen_schedule_win);
        }

        gx_window_event_process(window, event_ptr);
        break;

    default:

        /* Call default window event process.  */
        status = gx_window_event_process(window, event_ptr);
        break;
    }

    return status;
}


