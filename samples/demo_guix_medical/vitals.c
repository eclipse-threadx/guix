/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"

#include "guix_medical_resources.h"
#include "guix_medical_specifications.h"
#include "demo_guix_medical.h"

/* Define timer ids.  */
#define EKG_WAVEFORM_TIMER    2
#define PULSE_WAVEFORM_TIMER  3
#define HEART_RATE_TIMER      4
#define SPO2_TIMER            5
#define VALUE_ANIMATION_TIMER 6

/* Define constants.  */
#define CHART_SCROLL                 4
#define VALUE_ANIOMATION_TOTAL_STEPS 10

#define CARDIO_DOT_WIDTH  9
#define CARDIO_DOT_HEIGHT 10

/* Define a macro to calculate the current value according to the animation step.  */
#define GET_NEW_VALUE(info) (info.start_value + (info.end_value - info.current_value) * value_animation_step / VALUE_ANIOMATION_TOTAL_STEPS)

/* Define a waveform information structure for waveform drawing.  */
typedef struct WAVE_INFO_STRUCT
{
    GX_COLOR *capture_memory;
    INT capture_memory_size;
    INT capture_xpos;
    INT capture_ypos;
    INT *value_array;
    INT  total_values;
    INT  index;
    INT  retrace_xpos;
    GX_RESOURCE_ID color_id;
}WAVE_INFO;

/* Define a value information structure for value animation.  */
typedef struct VALUE_INFO_STRUCT
{
    INT start_value;
    INT end_value;
    INT current_value;
}VALUE_INFO;

/* Define ekg waveform values.  */
INT ekg_values[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 4, 5, 4, 2, 0, 0, 0, 0, -3, 30, -7, 0, 0, 0, 0, 5, 8, 10, 8, 5
};

/* Define pulse waveform values.  */
INT pulse_values[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 4, 5, 4, 2, 0, 0, 0, 0, -3, 30, -7, 0, 0, 0, 0, 5, 8, 10, 8, 5
};

/* Define heart rate values.  */
INT heart_rate_values[] = {
    88, 87, 86, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 94, 93, 92, 90, 89
};

/* Define spo2 values.  */
INT spo2_values[] = {
    94, 95, 96, 97, 96, 95, 94, 93
};

/* Define waveform capture memory.  */
GX_COLOR ekg_capture_memory[CARDIO_DOT_WIDTH * CARDIO_DOT_HEIGHT];
GX_COLOR pulse_capture_memory[CARDIO_DOT_WIDTH * CARDIO_DOT_HEIGHT];

/* Define and initialize ekg waveform information instance.  */
WAVE_INFO ekg_wave_info = {
    ekg_capture_memory,
    CARDIO_DOT_WIDTH* CARDIO_DOT_HEIGHT * sizeof(GX_COLOR),
    0,
    0,
    ekg_values,
    (INT)(sizeof(ekg_values) / sizeof(INT)),
    0,
    0,
    GX_COLOR_ID_GREEN
};

/* Define and initialize pulse waveform information instance.  */
WAVE_INFO pulse_wave_info = {
    pulse_capture_memory,
    CARDIO_DOT_WIDTH* CARDIO_DOT_HEIGHT * sizeof(GX_COLOR),
    0,
    0,
    pulse_values,
    (INT)(sizeof(pulse_values) / sizeof(INT)),
    0,
    0,
    GX_COLOR_ID_BLUE
};

INT heart_rate_index = 0;
INT spo2_index = 0;
INT value_animation_step = 0;
VALUE_INFO insulin = { 48, 71, 48 };
VALUE_INFO heart_rate = { 0, 88, 0 };
VALUE_INFO spo2 = { 0, 94, 0 };
VALUE_INFO medtype1 = { 0, 55, 0 };
VALUE_INFO medtype2 = { 0, 25, 0 };
VALUE_INFO medtype3 = { 0, 45, 0 };

/* Define prototypes.  */
extern VOID GetPatientName(GX_STRING* string);
extern VOID GetPatientDOB(GX_STRING* string);
extern VOID GetPatientAge(INT* age);

/******************************************************************************************/
/* Capture canvas under cardio dot.                                                       */
/******************************************************************************************/
static VOID capture_memory(GX_CANVAS *canvas, WAVE_INFO *info)
{
GX_COLOR *get;
GX_COLOR *put;
INT       copy_width;
INT       y;

    get = canvas->gx_canvas_memory;
    get += info->capture_ypos * canvas->gx_canvas_x_resolution;
    get += info->capture_xpos;

    put = info->capture_memory;
    copy_width = CARDIO_DOT_WIDTH * sizeof(GX_COLOR);

    for (y = 0; y < CARDIO_DOT_HEIGHT; y++)
    {
        memcpy(put, get, copy_width);

        put += CARDIO_DOT_WIDTH;
        get += canvas->gx_canvas_x_resolution;
    }
}

/******************************************************************************************/
/* Restore canvas under cardio dot.                                                       */
/******************************************************************************************/
static VOID restore_memory(GX_CANVAS *canvas, WAVE_INFO *info)
{
GX_PIXELMAP pixelmap;

    memset(&pixelmap, 0, sizeof(GX_PIXELMAP));
    pixelmap.gx_pixelmap_data = (GX_UBYTE *)info->capture_memory;
    pixelmap.gx_pixelmap_data_size = info->capture_memory_size;
    pixelmap.gx_pixelmap_width = CARDIO_DOT_WIDTH;
    pixelmap.gx_pixelmap_height = CARDIO_DOT_HEIGHT;
    pixelmap.gx_pixelmap_format = GX_COLOR_FORMAT_24XRGB;

    gx_canvas_pixelmap_draw(info->capture_xpos, info->capture_ypos, &pixelmap);
}

/******************************************************************************************/
/* Update waveform.                                                                       */
/******************************************************************************************/
static VOID update_waveform(GX_WINDOW *win, WAVE_INFO *info)
{
GX_CANVAS    *canvas;
GX_PIXELMAP  *map;
GX_RECTANGLE *chart_area;
INT           xpos;
INT           ycenter;
INT           line_start_offset;
INT           line_end_offset;

    chart_area = &win->gx_window_client;

    /* pick up the canvas pointer.  */
    gx_widget_canvas_get(win, &canvas);

    /* Initiate drawing on this canvas.  */
    gx_canvas_drawing_initiate(canvas, win, chart_area);        

    /* Erase the rectangle ahead of the line.  */
    xpos = chart_area->gx_rectangle_left + info->retrace_xpos + 10;
    gx_context_pixelmap_get(GX_PIXELMAP_ID_CHART_ERASE, &map);
    gx_context_fill_color_set(GX_COLOR_ID_CANVAS);
    gx_canvas_pixelmap_draw(xpos, chart_area->gx_rectangle_top, map);

    if (xpos + map->gx_pixelmap_width > chart_area->gx_rectangle_right)
    {
        gx_canvas_pixelmap_draw(chart_area->gx_rectangle_left + xpos - chart_area->gx_rectangle_right,
                                chart_area->gx_rectangle_top, map);
    }

    xpos = chart_area->gx_rectangle_left +info->retrace_xpos;

    /* Define brush for line draw.  */
    gx_context_brush_define(info->color_id, info->color_id, GX_BRUSH_SOLID_FILL | GX_BRUSH_ROUND | GX_BRUSH_ALIAS);
    gx_context_brush_width_set(2);

    ycenter = (chart_area->gx_rectangle_top + chart_area->gx_rectangle_bottom) >> 1;
    line_start_offset = info->value_array[info->index++];
    if (info->index >= info->total_values)
    {
        info->index = 0;
    }
    line_end_offset = info->value_array[info->index];;

    /* Restore canvas under previous cardio dot.  */
    restore_memory(canvas, info);

    gx_canvas_line_draw(xpos, ycenter - line_start_offset, xpos + CHART_SCROLL, ycenter - line_end_offset);

    /* Draw cardio dot.  */
    gx_context_fill_color_set(info->color_id);
    info->capture_xpos = xpos + CHART_SCROLL - (CARDIO_DOT_WIDTH / 2);
    info->capture_ypos = ycenter - line_end_offset - (CARDIO_DOT_HEIGHT / 2);

    /* Capture canvas under new cardio dot. */
    capture_memory(canvas, info);

    /* Draw cardio dot.  */
    gx_context_pixelmap_get(GX_PIXELMAP_ID_CARDIO_DOT, &map);
    gx_canvas_pixelmap_draw(info->capture_xpos, info->capture_ypos, map);

    /* Indicate that drawing on this canvas is complete.  */
    gx_canvas_drawing_complete(canvas, GX_TRUE);

    info->retrace_xpos += CHART_SCROLL;

    if (chart_area->gx_rectangle_left + info->retrace_xpos + CHART_SCROLL > chart_area->gx_rectangle_right)
    {
        info->retrace_xpos = 0;
    }
}

/******************************************************************************************/
/* Update heart rate.                                                                     */
/******************************************************************************************/
static VOID update_heart_rate()
{
    gx_numeric_prompt_value_set(&vitals_screen.vitals_screen_hr_value, heart_rate_values[heart_rate_index++]);

    if (heart_rate_index > (sizeof(heart_rate_values) / sizeof(INT)))
    {
        heart_rate_index = 0;
    }
}

/******************************************************************************************/
/* Update spo2 value.                                                                     */
/******************************************************************************************/
static VOID update_spo2()
{
    gx_numeric_prompt_value_set(&vitals_screen.vitals_screen_spo2_value, spo2_values[spo2_index++]);

    if (spo2_index > (sizeof(spo2_values) / sizeof(INT)))
    {
        spo2_index = 0;
    }
}

/******************************************************************************************/
/* Start value animation.                                                                 */
/******************************************************************************************/
static VOID start_value_animation()
{
    gx_system_timer_start((GX_WIDGET*)&vitals_screen, VALUE_ANIMATION_TIMER, 200 / GX_SYSTEM_TIMER_MS, 40 / GX_SYSTEM_TIMER_MS);

    gx_numeric_prompt_value_set(&vitals_screen.vitals_screen_insulin_value, insulin.start_value);
    gx_numeric_prompt_value_set(&vitals_screen.vitals_screen_hr_value, heart_rate.start_value);
    gx_numeric_prompt_value_set(&vitals_screen.vitals_screen_spo2_value, spo2.start_value);
    gx_numeric_prompt_value_set(&vitals_screen.vitals_screen_medtype1_value, medtype1.start_value);
    gx_numeric_prompt_value_set(&vitals_screen.vitals_screen_medtype2_value, medtype2.start_value);
    gx_numeric_prompt_value_set(&vitals_screen.vitals_screen_medtype3_value, medtype3.start_value);
    gx_slider_value_set((GX_SLIDER *)&vitals_screen.vitals_screen_medtype1_slider, &vitals_screen.vitals_screen_medtype1_slider.gx_slider_info, medtype1.start_value);
    gx_slider_value_set((GX_SLIDER*)&vitals_screen.vitals_screen_medtype2_slider, &vitals_screen.vitals_screen_medtype2_slider.gx_slider_info, medtype2.start_value);
    gx_slider_value_set((GX_SLIDER*)&vitals_screen.vitals_screen_medtype3_slider, &vitals_screen.vitals_screen_medtype3_slider.gx_slider_info, medtype3.start_value);

    value_animation_step = 0;
}

/******************************************************************************************/
/* Update value animation.                                                                */
/******************************************************************************************/
static VOID update_value_animation()
{
INT new_value;

    value_animation_step++;

    new_value = GET_NEW_VALUE(insulin);
    gx_numeric_prompt_value_set(&vitals_screen.vitals_screen_insulin_value, new_value);

    new_value = GET_NEW_VALUE(heart_rate);
    gx_numeric_prompt_value_set(&vitals_screen.vitals_screen_hr_value, new_value);

    new_value = GET_NEW_VALUE(spo2);
    gx_numeric_prompt_value_set(&vitals_screen.vitals_screen_spo2_value, new_value);

    new_value = GET_NEW_VALUE(medtype1);
    gx_numeric_prompt_value_set(&vitals_screen.vitals_screen_medtype1_value, new_value);
    gx_slider_value_set((GX_SLIDER*)&vitals_screen.vitals_screen_medtype1_slider, &vitals_screen.vitals_screen_medtype1_slider.gx_slider_info, new_value);

    new_value = GET_NEW_VALUE(medtype2);
    gx_numeric_prompt_value_set(&vitals_screen.vitals_screen_medtype2_value, medtype2.start_value);
    gx_slider_value_set((GX_SLIDER*)&vitals_screen.vitals_screen_medtype2_slider, &vitals_screen.vitals_screen_medtype2_slider.gx_slider_info, new_value);

    new_value = GET_NEW_VALUE(medtype3);
    gx_numeric_prompt_value_set(&vitals_screen.vitals_screen_medtype3_value, medtype3.start_value);
    gx_slider_value_set((GX_SLIDER*)&vitals_screen.vitals_screen_medtype3_slider, &vitals_screen.vitals_screen_medtype3_slider.gx_slider_info, new_value);

    if (value_animation_step == VALUE_ANIOMATION_TOTAL_STEPS)
    {
        gx_system_timer_stop((GX_WIDGET*)&vitals_screen, VALUE_ANIMATION_TIMER);
    }
}

/******************************************************************************************/
/* Start ekg waveform drawing.                                                            */
/******************************************************************************************/
static VOID start_ekg_waveform()
{
    ekg_wave_info.index = 0;
    ekg_wave_info.retrace_xpos = 0;
    memset(ekg_wave_info.capture_memory, 0, ekg_wave_info.capture_memory_size);

    gx_system_timer_start((GX_WIDGET*)&vitals_screen, EKG_WAVEFORM_TIMER, 80 / GX_SYSTEM_TIMER_MS, 80 / GX_SYSTEM_TIMER_MS);
    gx_system_timer_start((GX_WIDGET*)&vitals_screen, HEART_RATE_TIMER, GX_TICKS_SECOND * 2, GX_TICKS_SECOND * 2);
}

/******************************************************************************************/
/* Start pulse waveform drawing.                                                          */
/******************************************************************************************/
static VOID start_pulse_waveform()
{
    pulse_wave_info.index = 0;
    pulse_wave_info.retrace_xpos = 0;
    memset(pulse_wave_info.capture_memory, 0, pulse_wave_info.capture_memory_size);

    gx_system_timer_start((GX_WIDGET *)&vitals_screen, PULSE_WAVEFORM_TIMER, 80 / GX_SYSTEM_TIMER_MS, 80 / GX_SYSTEM_TIMER_MS);
    gx_system_timer_start((GX_WIDGET*)&vitals_screen, SPO2_TIMER, GX_TICKS_SECOND, GX_TICKS_SECOND);
}

/******************************************************************************************/
/* Update patient information.                                                            */
/******************************************************************************************/
static VOID update_patient_information()
{
GX_STRING string;
INT       age;

    /* Get patient's name.  */
    GetPatientName(&string);

    /* Set patient's name.  */
    gx_prompt_text_set_ext(&vitals_screen.vitals_screen_patient_name, &string);

    /* Get patient's date of birth.  */
    GetPatientDOB(&string);

    /* Set patient's date of birth.  */
    gx_prompt_text_set_ext(&vitals_screen.vitals_screen_patient_dob, &string);

    /* Get patient's age.  */
    GetPatientAge(&age);

    /* Set patient's age.  */
    gx_numeric_prompt_value_set(&vitals_screen.vitals_screen_patient_age, age);
}

/******************************************************************************************/
/* Override the default event processing of "vitals_screen" to handle signals from my     */
/* child widgets.                                                                         */
/******************************************************************************************/
UINT vitals_screen_event_process(GX_WINDOW *window, GX_EVENT *myevent)
{
UINT status = GX_SUCCESS;

    switch(myevent->gx_event_type)
    {
    case GX_EVENT_SHOW:

        /* Call the event process of the template on which the vitals screen is based.  */
        template_event_handler(window, myevent);

        /* Update patient's information.  */
        update_patient_information();

        /* Start value animation.  */
        start_value_animation();
        break;

    case GX_EVENT_HIDE:

        /* Call the event process of the template on which the vitals screen is based.  */
        template_event_handler(window, myevent);

        /* Stop all timers that belongs to the window. */
        gx_system_timer_stop((GX_WIDGET*)window, 0);
        break;

    case GX_SIGNAL(ID_MEDTYPE1_SLIDER, GX_EVENT_SLIDER_VALUE):
        gx_numeric_prompt_value_set(&vitals_screen.vitals_screen_medtype1_value, myevent->gx_event_payload.gx_event_longdata);
        break;
        
    case GX_SIGNAL(ID_MEDTYPE2_SLIDER, GX_EVENT_SLIDER_VALUE):
        gx_numeric_prompt_value_set(&vitals_screen.vitals_screen_medtype2_value, myevent->gx_event_payload.gx_event_longdata);
        break;

    case GX_SIGNAL(ID_MEDTYPE3_SLIDER, GX_EVENT_SLIDER_VALUE):
        gx_numeric_prompt_value_set(&vitals_screen.vitals_screen_medtype3_value, myevent->gx_event_payload.gx_event_longdata);
        break;

    case GX_EVENT_TIMER:
        switch(myevent ->gx_event_payload.gx_event_timer_id)
        {
        case EKG_WAVEFORM_TIMER:
            update_waveform(&vitals_screen.vitals_screen_ekg_waveform_win, &ekg_wave_info);
            break;

        case PULSE_WAVEFORM_TIMER:
            update_waveform(&vitals_screen.vitals_screen_pulse_waveform_win, &pulse_wave_info);
            break;

        case HEART_RATE_TIMER:
            update_heart_rate();
            break;

        case SPO2_TIMER:
            update_spo2();
            break;

        case VALUE_ANIMATION_TIMER:
            update_value_animation();
            break;
        }
        break;

    case GX_EVENT_ANIMATION_COMPLETE:
        if (myevent->gx_event_sender == ID_EKG_WIN_SLIDE_IN)
        {
            start_ekg_waveform();
        }
        else if (myevent->gx_event_sender = ID_PULSE_WIN_SLIDE_IN)
        {
            start_pulse_waveform();
        }
        break;

    default:
        status = template_event_handler(window, myevent);
    }
    return status;
}

/******************************************************************************************/
/* Callback function to format insulin value.                                             */
/******************************************************************************************/
VOID insulin_value_format(GX_NUMERIC_PROMPT *prompt, INT value)
{
    prompt->gx_numeric_prompt_buffer[0] = '0';
    prompt->gx_numeric_prompt_buffer[1] = '.';
    gx_utility_ltoa(value, prompt->gx_numeric_prompt_buffer + 2, GX_NUMERIC_PROMPT_BUFFER_SIZE - 1);
}


