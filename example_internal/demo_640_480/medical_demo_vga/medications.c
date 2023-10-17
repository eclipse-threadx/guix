/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"

#include "medical_resources.h"
#include "medical_specifications.h"

extern MEDS_SCREEN_CONTROL_BLOCK meds_screen;
extern VITALS_SCREEN_CONTROL_BLOCK vitals_screen;
extern PATIENTS_SCREEN_CONTROL_BLOCK patients_screen;
void ToggleScreen(GX_WINDOW *new_win, GX_WINDOW *old_win);
extern UINT string_length_get(GX_CONST GX_CHAR* input_string, UINT max_string_length);

extern char current_patient_name[128];
GX_CHAR value_strings[4][8];

#define MAX_VAL_LEN 4

VOID update_prompt(GX_PROMPT *prompt, GX_CHAR *buffer, LONG value)
{
    GX_STRING string;

    gx_utility_ltoa(value, buffer, MAX_VAL_LEN);
    string.gx_string_ptr = buffer;
    string.gx_string_length = string_length_get(buffer, MAX_VAL_LEN);
    gx_prompt_text_set_ext(prompt, &string);
}

UINT meds_screen_event_handler(GX_WINDOW *window, GX_EVENT *myevent)
{
GX_BUTTON   *meds_button;
GX_PROMPT   *prompt;

    switch(myevent ->gx_event_type)
    {
    case GX_EVENT_SHOW:
        gx_window_event_process(window, myevent);
        gx_widget_find(window, ID_MEDS_BUTTON, 0, (GX_WIDGET **) &meds_button);
        meds_button->gx_button_select_handler((GX_WIDGET *) meds_button);
        break;

    case GX_SIGNAL(ID_SLIDER1, GX_EVENT_SLIDER_VALUE):
        gx_widget_find(window, ID_VAL1_DISPLAY, 0, (GX_WIDGET **) &prompt);
        update_prompt(prompt, value_strings[0], 
            myevent -> gx_event_payload.gx_event_longdata);
        break;

    case GX_SIGNAL(ID_SLIDER2, GX_EVENT_SLIDER_VALUE):
        gx_widget_find(window, ID_VAL2_DISPLAY, 0, (GX_WIDGET **) &prompt);
        update_prompt(prompt, value_strings[1], 
            myevent -> gx_event_payload.gx_event_longdata);
        break;

    case GX_SIGNAL(ID_SLIDER3, GX_EVENT_SLIDER_VALUE):
        gx_widget_find(window, ID_VAL3_DISPLAY, 0, (GX_WIDGET **) &prompt);
        update_prompt(prompt, value_strings[2], 
            myevent -> gx_event_payload.gx_event_longdata);
         break;

    case GX_SIGNAL(ID_SLIDER4, GX_EVENT_SLIDER_VALUE):
        gx_widget_find(window, ID_VAL4_DISPLAY, 0, (GX_WIDGET **) &prompt);
        update_prompt(prompt, value_strings[3], 
            myevent -> gx_event_payload.gx_event_longdata);
        break;

    case GX_SIGNAL(ID_VITALS_BUTTON, GX_EVENT_RADIO_SELECT):
        ToggleScreen((GX_WINDOW *) &vitals_screen, window);
        break;

    case GX_SIGNAL(ID_PATIENTS_BUTTON, GX_EVENT_RADIO_SELECT):
        ToggleScreen((GX_WINDOW *) &patients_screen, window);
        break;
    default:
        return gx_window_event_process(window, myevent);
    }
    return 0;
}


