/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"

#include "guix_medical_resources.h"
#include "guix_medical_specifications.h"

#include "demo_guix_medical.h"

extern UINT string_length_get(GX_CONST GX_CHAR* input_string, UINT max_string_length);

#define NUM_SLIDERS 4
extern char current_patient_name[128];
GX_CHAR value_strings[NUM_SLIDERS][8];
#define MAX_VAL_LEN 4
#define NUM_ANIMATION_STEPS 10

INT med_levels[NUM_SLIDERS] = {75, 60, 53, 42};
GX_BOOL animation_running = GX_FALSE;

GX_SLIDER *med_sliders[NUM_SLIDERS] = {
    (GX_SLIDER *) &meds_screen.meds_screen_med1_level,
    (GX_SLIDER *) &meds_screen.meds_screen_med2_level,
    (GX_SLIDER *) &meds_screen.meds_screen_med3_level,
    (GX_SLIDER *) &meds_screen.meds_screen_med4_level
};

INT animation_step;

VOID update_prompt(GX_PROMPT *prompt, GX_CHAR *buffer, INT value)
{
GX_STRING string;

    gx_utility_ltoa(value, buffer, MAX_VAL_LEN);

    string.gx_string_ptr = buffer;
    string.gx_string_length = string_length_get(buffer, MAX_VAL_LEN);

    gx_prompt_text_set_ext(prompt, &string);
}

UINT meds_screen_event_handler(GX_WINDOW *window, GX_EVENT *myevent)
{
GX_PROMPT *prompt;
GX_SLIDER *slider;
INT loop;
INT value;
GX_STRING string;

    switch(myevent ->gx_event_type)
    {
    case GX_EVENT_SHOW:
        GetPatientName(&string);
        gx_prompt_text_set_ext(&meds_screen.meds_screen_patient_name, &string);
        GetPatientAdmitDate(&string);
        gx_prompt_text_set_ext(&meds_screen.meds_screen_admit_date, &string);
        animation_running = GX_TRUE;

        // reset all sliders to initial value 0
        for (loop = 0; loop < NUM_SLIDERS; loop++)
        {
            slider = med_sliders[loop];
            gx_slider_value_set(slider, &slider->gx_slider_info, 0);
        }
        med_screen_base_event_handler(window, myevent);
        animation_step = 0;
        gx_system_timer_start(window, 1, 1, 1);
        break;

    case GX_SIGNAL(ID_SLIDER1, GX_EVENT_SLIDER_VALUE):
        gx_widget_find(window, ID_VAL1_DISPLAY, 0, (GX_WIDGET **) &prompt);
        update_prompt(prompt, value_strings[0], myevent->gx_event_payload.gx_event_longdata);
        
        if (!animation_running)
        {
            med_levels[0] = myevent->gx_event_payload.gx_event_longdata;
        }
        break;

    case GX_SIGNAL(ID_SLIDER2, GX_EVENT_SLIDER_VALUE):
        gx_widget_find(window, ID_VAL2_DISPLAY, 0, (GX_WIDGET **) &prompt);
        update_prompt(prompt, value_strings[1], myevent->gx_event_payload.gx_event_longdata);
        if (!animation_running)
        {
            med_levels[1] = myevent->gx_event_payload.gx_event_longdata;
        }
        break;

    case GX_SIGNAL(ID_SLIDER3, GX_EVENT_SLIDER_VALUE):
        gx_widget_find(window, ID_VAL3_DISPLAY, 0, (GX_WIDGET **) &prompt);
        update_prompt(prompt, value_strings[2], myevent->gx_event_payload.gx_event_longdata);

        if (!animation_running)
        {
            med_levels[2] = myevent->gx_event_payload.gx_event_longdata;
        }
        break;

    case GX_SIGNAL(ID_SLIDER4, GX_EVENT_SLIDER_VALUE):
        gx_widget_find(window, ID_VAL4_DISPLAY, 0, (GX_WIDGET **) &prompt);
        update_prompt(prompt, value_strings[3], myevent->gx_event_payload.gx_event_longdata);

        if (!animation_running)
        {
            med_levels[3] = myevent->gx_event_payload.gx_event_longdata;
        }
        break;

    case GX_EVENT_TIMER:
        animation_step++;
        if (animation_step == NUM_ANIMATION_STEPS)
        {
            gx_system_timer_stop(window, 1);

            for (loop = 0; loop < NUM_SLIDERS; loop++)
            {
                slider = med_sliders[loop];
                gx_slider_value_set(slider, &slider->gx_slider_info, med_levels[loop]);
            }
            animation_running = GX_FALSE;
        }
        else
        {
            for (loop = 0; loop < NUM_SLIDERS; loop++)
            {
                slider = med_sliders[loop];
                value = (med_levels[loop] + slider->gx_slider_info.gx_slider_info_current_val) / 2;
                gx_slider_value_set(slider, &slider->gx_slider_info, value);
            }
        }
        break;
    
    default:
        return med_screen_base_event_handler(window, myevent);;
    }
    return 0;
}


VOID med_level_draw(GX_PROMPT *prompt)
{
GX_VALUE twidth;
GX_FONT *font;
GX_PIXELMAP *map;
GX_STRING text;

    gx_prompt_draw(prompt);

    // Draw the % symbol next to the text:
    gx_context_font_get(prompt->gx_prompt_font_id, &font);
    gx_context_pixelmap_get(GX_PIXELMAP_ID_PERCENT, &map);
    gx_prompt_text_get_ext(prompt, &text);
    gx_system_string_width_get_ext(font, &text, &twidth);
    gx_canvas_pixelmap_draw(prompt->gx_widget_size.gx_rectangle_left + twidth,
                            prompt->gx_widget_size.gx_rectangle_top, map);
}
