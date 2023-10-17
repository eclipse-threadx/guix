/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"

#include "medical_resources.h"
#include "medical_specifications.h"

GX_WINDOW_ROOT    *root;

/* Define prototypes.   */


VOID  start_guix(VOID);
extern UINT win32_graphics_driver_setup_565rgb(GX_DISPLAY *display);
VOID patient_list_children_create(PATIENTS_SCREEN_CONTROL_BLOCK *win);
UINT string_length_get(GX_CONST GX_CHAR* input_string, UINT max_string_length);

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
    /* Initialize GUIX.  */
    gx_system_initialize();

    gx_studio_display_configure(MAIN_DISPLAY, win32_graphics_driver_setup_565rgb, 
                                LANGUAGE_ENGLISH, MAIN_DISPLAY_THEME_1, &root);

    /* create the patients screen */
    gx_studio_named_widget_create("patients_screen", (GX_WIDGET *) root, GX_NULL);
    patient_list_children_create(&patients_screen);

    /* create the vitals screen */
    gx_studio_named_widget_create("vitals_screen", GX_NULL, GX_NULL);

    /* create the meds screen */
    gx_studio_named_widget_create("meds_screen", GX_NULL, GX_NULL);

    /* Show the root window to make it and patients screen visible.  */
    gx_widget_show(root);

    /* start GUIX thread */
    gx_system_start();
}

void ToggleScreen(GX_WIDGET *new_win, GX_WIDGET *old_win)
{
    if (!new_win->gx_widget_parent)
    {
        gx_widget_attach(root, new_win);
    }
    else
    {
        gx_widget_show(new_win);
    }
    gx_widget_hide(old_win);
}

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

