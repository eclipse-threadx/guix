/* This is a small demo of the high-performance GUIX graphics framework. */

#include "demo_guix_industrial.h"

#define SEQUENCE_TIMER       10
#define SEQUENCE_TIMER_TICKS (40 / GX_SYSTEM_TIMER_MS)

#define LEFT_PROGRESS_BAR_INCREMENT  3

#define TASK_PROPERTY_ANIMATION_STEPS 10
#define TASK_ICON_ANIMATION_STEPS    8
#define TASK_TITLE_ANIMATION_STEPS   8
#define TASK_TITLE_ANIMATION_SHIFT   60
#define TASK_TITLE_ANIMATION_START_X 19

/* Define task IDs.  */
enum task_ids {
    TASK_ID_INSPECTING = 0,
    TASK_ID_ASSEMBLING,
    TASK_ID_WELDING,
    TASK_ID_PALLETIZING,
    TASK_COUNT
};

/* Define task information struture.  */
typedef struct TASK_INFO_STRUCT {
    INT task_id;
    GX_PIXELMAP_BUTTON* task_button;
    GX_RESOURCE_ID task_text_id;
    GX_RESOURCE_ID task_icon_id;
    GX_RESOURCE_ID progress_1_text_id;
    GX_RESOURCE_ID progress_2_text_id;
    GX_RESOURCE_ID progress_3_text_id;
    GX_RESOURCE_ID progress_4_text_id;
}TASK_INFO;

/* Define progress information structure.  */
typedef struct PROGRESS_INFO_STRUCT {
    INT current_val;
    GX_PROGRESS_BAR *progress_bar;
    GX_NUMERIC_PROMPT *value_prompt;
    GX_PROMPT *percent_flag_prompt;
    GX_ICON *complete_icon;
}PROGRESS_INFO;

/* Define property information structure.  */
typedef struct PROPERTY_INFO_STRUCT {
    INT        values[TASK_COUNT];
    GX_WIDGET *widget;
}PROPERTY_INFO;

static int timer_minute;
static int timer_second;
static int timer_tick;

static int current_task_property_animation_step = TASK_PROPERTY_ANIMATION_STEPS;
static int current_task_title_animation_step = TASK_TITLE_ANIMATION_STEPS;
static int current_task_icon_animation_step = TASK_ICON_ANIMATION_STEPS;
static int current_task_id = TASK_ID_INSPECTING;

static int task_title_blend_alpha = 255;
extern USHORT toggle_animation_flag;

PROGRESS_INFO left_progress_info_list[] = {
    {
        0,
        &sequence_window.sequence_window_progress_1,
        &sequence_window.sequence_window_progress_1_value,
        &sequence_window.sequence_window_progress_1_percent_flag,
        &sequence_window.sequence_window_complete_icon_1
    },
    {
        0,
        &sequence_window.sequence_window_progress_2,
        &sequence_window.sequence_window_progress_2_value,
        &sequence_window.sequence_window_progress_2_percent_flag,
        &sequence_window.sequence_window_complete_icon_2
    },
    {
        0,
        &sequence_window.sequence_window_progress_3,
        &sequence_window.sequence_window_progress_3_value,
        &sequence_window.sequence_window_progress_3_percent_flag,
        &sequence_window.sequence_window_complete_icon_3
    },
    {
        0,
        &sequence_window.sequence_window_progress_4,
        &sequence_window.sequence_window_progress_4_value,
        &sequence_window.sequence_window_progress_4_percent_flag,
        &sequence_window.sequence_window_complete_icon_4
    }
};

TASK_INFO task_info_list[] =
{
    {
        TASK_ID_INSPECTING,
        &main_screen.main_screen_button_inspecting,
        GX_STRING_ID_INSPECTING,
        GX_PIXELMAP_ID_CIRCLE_ICON_INSPECTING,
        GX_STRING_ID_SURFACE, 
        GX_STRING_ID_COMPONENTS, 
        GX_STRING_ID_TRACING_PATTERN, 
        GX_STRING_ID_CIRCUIT_TEST
    },
    {
        TASK_ID_ASSEMBLING,
        &main_screen.main_screen_button_inspecting,
        GX_STRING_ID_ASSEMBLING,
        GX_PIXELMAP_ID_CIRCLE_ICON_ASSEMBLING,
        GX_STRING_ID_ROTATE_AXIS,
        GX_STRING_ID_DRIVE_PRESS,
        GX_STRING_ID_FORCE_JOIN,
        GX_STRING_ID_ASSEMBLING
    },
    {
        TASK_ID_WELDING,
        &main_screen.main_screen_button_welding,
        GX_STRING_ID_WELDING,
        GX_PIXELMAP_ID_CIRCLE_ICON_WELDING,
        GX_STRING_ID_CURRENT_CHECK,
        GX_STRING_ID_TARGET_CLAMPS,
        GX_STRING_ID_BRAZING_FLUX,
        GX_STRING_ID_GROUND_FLUSH
    },
    {
        TASK_ID_PALLETIZING,
        &main_screen.main_screen_button_palletizing,
        GX_STRING_ID_PALLETIZING,
        GX_PIXELMAP_ID_CIRCLE_ICON_PALLET,
        GX_STRING_ID_LEVEL_SURFACE,
        GX_STRING_ID_ARM_AUXILIARY,
        GX_STRING_ID_PALLET_FRICTION,
        GX_STRING_ID_UNITSET
    }
};

PROPERTY_INFO property_prompt_value_list[] = {
    {{ 75, 70, 79, 68 }, (GX_WIDGET*)&sequence_window.sequence_window_prompt_SX },
    {{ 72, 69, 74, 63 }, (GX_WIDGET*)&sequence_window.sequence_window_prompt_SY },
    {{ -1050, -1050, 9290, -1050 }, (GX_WIDGET*)&sequence_window.sequence_window_prompt_RX },
    {{ 13650, 13650, 3870, 13650 }, (GX_WIDGET*)&sequence_window.sequence_window_prompt_RY },
    {{ 112, 423, 1822, 205 }, (GX_WIDGET*)&sequence_window.sequence_window_prompt_UP },
    {{ 8, 217, 1436, 129 }, (GX_WIDGET*)&sequence_window.sequence_window_prompt_DN },
    {{ 814, 756, 903, 816 }, (GX_WIDGET*)&sequence_window.sequence_window_prompt_X },
    {{ 734, 680, 588, 734}, (GX_WIDGET*)&sequence_window.sequence_window_prompt_Y },
    {{ 387, 360, 373, 388 }, (GX_WIDGET*)&sequence_window.sequence_window_prompt_Z }
};

PROPERTY_INFO property_radial_progress_value_list[] ={
    {{ -220, -270, -200, -250 }, (GX_WIDGET*)&sequence_window.sequence_window_radial_progress_speed },
    {{ -160, -170, -150, -160 }, (GX_WIDGET*)&sequence_window.sequence_window_radial_progress_rotation },
    {{ -90, -40, -50, -80 },(GX_WIDGET*)&sequence_window.sequence_window_radial_progress_force }
};

VOID sequence_window_init();
VOID task_set(int new_mode);
TASK_INFO* task_info_get(INT task_id);
VOID task_time_update();
VOID task_progress_information_init();
VOID task_progress_information_update();
VOID task_properties_init();
VOID task_properties_update();
VOID task_title_animation_init();
VOID task_title_animation_update();
VOID task_icon_animation_update();
UINT string_length_get(GX_CONST GX_CHAR* input_string, UINT max_string_length);

/******************************************************************************************/
/* Retrieve tasl information according to task id.                                        */
/******************************************************************************************/
TASK_INFO *task_info_get(INT task_id)
{
    if (task_id < sizeof(task_info_list) / sizeof(TASK_INFO))
    {
        return &task_info_list[task_id];
    }

    return GX_NULL;
}

/******************************************************************************************/
/* Override event processing of "sequence_window" to handle signals from my child widgets.*/
/******************************************************************************************/
UINT sequence_window_event_process(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_TIMER:
        if (event_ptr->gx_event_payload.gx_event_timer_id == SEQUENCE_TIMER)
        {
            sequence_animation_update();
        }
        break;

    case GX_SIGNAL(ID_INSPECTING, GX_EVENT_CLICKED):
        task_set(TASK_ID_INSPECTING);
        break;

    case GX_SIGNAL(ID_ASSEMBLING, GX_EVENT_CLICKED):
        task_set(TASK_ID_ASSEMBLING);
        break;

    case GX_SIGNAL(ID_WELDING, GX_EVENT_CLICKED):
        task_set(TASK_ID_WELDING);
        break;

    case GX_SIGNAL(ID_PALLETIZING, GX_EVENT_CLICKED):
        task_set(TASK_ID_PALLETIZING);
        break;

    default:
        return gx_window_event_process(window, event_ptr);
    }

    return 0;
}

/******************************************************************************************/
/* Override drawing of "task_icon" to draw the icon with specified blending alpha.        */
/******************************************************************************************/
VOID task_icon_draw(GX_ICON *icon)
{
    GX_BRUSH *brush;
    GX_PIXELMAP *map;
    GX_RECTANGLE *size;
    INT angle;
    TASK_INFO *tinfo;

    if ((toggle_animation_flag == ANIMATION_NONE))
    {
        gx_context_brush_get(&brush);
        brush->gx_brush_alpha = task_title_blend_alpha;
    }

    /* Call default icon draw.  */
    gx_icon_draw(icon);

    tinfo = task_info_get(current_task_id);

    if (!tinfo)
    {
        return;
    }

    /* Retrieve task icon pixelmap.  */
    gx_context_pixelmap_get(tinfo->task_icon_id, &map);
    size = &icon->gx_widget_size;

    if (current_task_icon_animation_step < TASK_ICON_ANIMATION_STEPS)
    {
        if (map)
        {
            angle = -90 * (current_task_icon_animation_step - TASK_ICON_ANIMATION_STEPS) / TASK_ICON_ANIMATION_STEPS;

            /* Rotate task icon.  */
            gx_canvas_pixelmap_rotate(size->gx_rectangle_left, size->gx_rectangle_top, map, angle, -1, -1);
        }
    }
    else
    {
        /* Draw task icon.  */
        gx_canvas_pixelmap_draw(size->gx_rectangle_left, size->gx_rectangle_top, map);
    }
}

/******************************************************************************************/
/* Override drawing of "task_title" to draw the prompt with specified blending alpha.     */
/******************************************************************************************/
VOID task_title_draw(GX_PROMPT *prompt)
{
    GX_BRUSH *brush;

    if ((toggle_animation_flag == ANIMATION_NONE))
    {
        gx_context_brush_get(&brush);
        brush->gx_brush_alpha = task_title_blend_alpha;
    }

    gx_prompt_draw(prompt);
}

/******************************************************************************************/
/* Initiate widget values in sequence window.                                             */
/******************************************************************************************/
VOID sequence_window_init()
{
    timer_minute = 0;
    timer_second = 0;
    timer_tick = 0;

    /* Init timer values. */
    gx_numeric_prompt_value_set(&sequence_window.sequence_window_timer_minute, timer_minute);
    gx_numeric_prompt_value_set(&sequence_window.sequence_window_timer_second, timer_second);
    gx_numeric_prompt_value_set(&sequence_window.sequence_window_timer_tick, timer_tick);

    gx_sprite_current_frame_set(&sequence_window.sequence_window_sprite, 0);

    task_set(TASK_ID_INSPECTING);
}

/******************************************************************************************/
/* Start sequence animation.                                                              */
/******************************************************************************************/
VOID sequence_animation_start()
{
    /* Create a sequence timer. */
    gx_system_timer_start(&sequence_window, SEQUENCE_TIMER, SEQUENCE_TIMER_TICKS, SEQUENCE_TIMER_TICKS);
    gx_sprite_start(&sequence_window.sequence_window_sprite, 0);
}

/******************************************************************************************/
/* Process one step for sequence animation.                                               */
/******************************************************************************************/
VOID sequence_animation_update()
{
    task_time_update();
    task_title_animation_update();
    task_icon_animation_update();
    task_properties_update();
    task_progress_information_update();
}

/******************************************************************************************/
/* Stop sequence animation.                                                               */
/******************************************************************************************/
VOID sequence_animation_stop()
{
    gx_system_timer_stop(&sequence_window, SEQUENCE_TIMER);
    gx_sprite_stop(&sequence_window.sequence_window_sprite);
}

/******************************************************************************************/
/* Set sequence mode.                                                                     */
/******************************************************************************************/
VOID task_set(int new_task_id)
{
    TASK_INFO *tinfo;

    if (new_task_id > TASK_ID_PALLETIZING)
    {
        screen_toggle_animation_start((GX_WINDOW*)&complete_window, (GX_WINDOW*)&sequence_window);
        return;
    }

    current_task_id = new_task_id;

    tinfo = task_info_get(new_task_id);

    if (!tinfo)
    {
        return;
    }

    button_indicator_attach(tinfo->task_button);

    /* Set title text. */
    gx_prompt_text_id_set(&sequence_window.sequence_window_progress_1_title, tinfo->progress_1_text_id);
    gx_prompt_text_id_set(&sequence_window.sequence_window_progress_2_title, tinfo->progress_2_text_id);
    gx_prompt_text_id_set(&sequence_window.sequence_window_progress_3_title, tinfo->progress_3_text_id);
    gx_prompt_text_id_set(&sequence_window.sequence_window_progress_4_title, tinfo->progress_4_text_id);
    gx_prompt_text_id_set(&sequence_window.sequence_window_task_title, tinfo->task_text_id);

    task_progress_information_init();
    task_properties_init();
    task_title_animation_init();
}

/******************************************************************************************/
/* Update task time.                                                                      */
/******************************************************************************************/
VOID task_time_update()
{
    int ms;

    if (timer_tick < GX_TICKS_SECOND)
    {
        timer_tick += SEQUENCE_TIMER_TICKS;

        ms = timer_tick * GX_SYSTEM_TIMER_MS / 10;

        gx_numeric_prompt_value_set(&sequence_window.sequence_window_timer_tick, ms);
    }
    else if (timer_second < 59)
    {
        timer_tick = 0;
        timer_second++;

        gx_numeric_prompt_value_set(&sequence_window.sequence_window_timer_tick, timer_tick);
        gx_numeric_prompt_value_set(&sequence_window.sequence_window_timer_second, timer_second);
    }
    else if (timer_minute < 59)
    {
        timer_second = 0;
        timer_minute++;

        gx_numeric_prompt_value_set(&sequence_window.sequence_window_timer_second, timer_tick);
        gx_numeric_prompt_value_set(&sequence_window.sequence_window_timer_minute, timer_second);
    }
    else
    {
        timer_minute = 0;

        gx_numeric_prompt_value_set(&sequence_window.sequence_window_timer_minute, timer_second);
    }
}

/******************************************************************************************/
/* Initialize progress information in left and bottom.                                    */
/******************************************************************************************/
VOID task_progress_information_init()
{
    PROGRESS_INFO *pinfo;
    INT stage;

    for (stage = 0; stage < sizeof(left_progress_info_list) / sizeof(PROGRESS_INFO); stage++)
    {
        pinfo = &left_progress_info_list[stage];

        pinfo->current_val = 0;

        /* Init progress bar value. */
        gx_progress_bar_value_set(pinfo->progress_bar, 0);

        /* Init progress value prompt value. */
        gx_numeric_prompt_value_set(pinfo->value_prompt, 0);

        /* Show progress value prompt.  */
        gx_widget_show(pinfo->value_prompt);

        /* Show percentage flag. */
        gx_widget_show(pinfo->percent_flag_prompt);

        /* Hide progress complete icon. */
        gx_widget_hide(pinfo->complete_icon);
    }

    /* Init bottom progress bar values. */
    gx_progress_bar_value_set(&sequence_window.sequence_window_bottom_progress, 0);
    gx_numeric_prompt_value_set(&sequence_window.sequence_window_bottom_progress_value, 0);
}

/******************************************************************************************/
/* Update progress information in left and bottom.                                        */
/******************************************************************************************/
VOID task_progress_information_update()
{
    INT progress_val = 0;
    PROGRESS_INFO *pinfo;
    TASK_INFO *tinfo = task_info_get(current_task_id);
    INT stage;
    INT total_stage = sizeof(left_progress_info_list) / sizeof(PROGRESS_INFO);

    if (!tinfo)
    {
        return;
    }

    for (stage = 0; stage < total_stage; stage++)
    {
        /* Update progress information when stage is 0 or previous stage is 25% completed.  */
        if (stage == 0 || (left_progress_info_list[stage - 1].current_val >= 25))
        {
            pinfo = &left_progress_info_list[stage];

            if (pinfo->current_val < 100)
            {
                pinfo->current_val += LEFT_PROGRESS_BAR_INCREMENT;

                /* Update progress bar value.  */
                gx_progress_bar_value_set(pinfo->progress_bar, pinfo->current_val);
                gx_numeric_prompt_value_set(pinfo->value_prompt, pinfo->current_val);

                if (pinfo->current_val >= 100)
                {
                    gx_widget_hide(pinfo->value_prompt);
                    gx_widget_hide(pinfo->percent_flag_prompt);
                    gx_widget_show(pinfo->complete_icon);
                }
            }
        }
    }

    progress_val = (tinfo->task_id * 100 / TASK_COUNT);

    if (left_progress_info_list[total_stage - 1].current_val)
    {
        progress_val += (75 + left_progress_info_list[total_stage - 1].current_val) * 25 / 175;
    }
    else
    {
        progress_val += (left_progress_info_list[0].current_val) * 25 / 175;
    }

    /* Start next task when last stage completed. */
    if (left_progress_info_list[total_stage - 1].current_val >= 100)
    {
        task_set(current_task_id + 1);
    }

    /* Update bottom progress information.  */
    gx_progress_bar_value_set(&sequence_window.sequence_window_bottom_progress, progress_val);
    gx_numeric_prompt_value_set(&sequence_window.sequence_window_bottom_progress_value, progress_val);
}

/******************************************************************************************/
/* Initialize task title animation.                                                       */
/******************************************************************************************/
VOID task_title_animation_init()
{
    GX_RECTANGLE *size;
    INT shift;

    current_task_title_animation_step = 0;
    task_title_blend_alpha = 255;

    size = &sequence_window.sequence_window_task_title.gx_widget_size;

    if (size->gx_rectangle_left != TASK_TITLE_ANIMATION_START_X)
    {
        shift = TASK_TITLE_ANIMATION_START_X - size->gx_rectangle_left;
        gx_widget_shift((GX_WIDGET*)&sequence_window.sequence_window_task_title, shift, 0, GX_TRUE);
        gx_widget_shift((GX_WIDGET*)&sequence_window.sequence_window_task_icon, shift, 0, GX_TRUE);
    }
}

/******************************************************************************************/
/* Udpate task title animation.                                                           */
/******************************************************************************************/
VOID task_title_animation_update()
{
    INT shift;

    if (current_task_title_animation_step < TASK_TITLE_ANIMATION_STEPS)
    {
        current_task_title_animation_step++;
        if (current_task_title_animation_step <= TASK_TITLE_ANIMATION_STEPS / 2)
        {
            shift = TASK_TITLE_ANIMATION_SHIFT;
            task_title_blend_alpha -= 50;
        }
        else
        {
            shift = -TASK_TITLE_ANIMATION_SHIFT;
            task_title_blend_alpha += 50;
        }

        gx_widget_shift((GX_WIDGET *)&sequence_window.sequence_window_task_title, shift, 0, GX_TRUE);
        gx_widget_shift((GX_WIDGET *)&sequence_window.sequence_window_task_icon, shift, 0, GX_TRUE);

        if (current_task_title_animation_step == TASK_TITLE_ANIMATION_STEPS)
        {
            /* Start mode icon animation.  */
            current_task_icon_animation_step = 0;
        }
    }
}

/******************************************************************************************/
/* Update mode icon animation.                                                            */
/******************************************************************************************/
VOID task_icon_animation_update()
{
    if (current_task_icon_animation_step < TASK_ICON_ANIMATION_STEPS)
    {
        current_task_icon_animation_step++;

        gx_system_dirty_mark(&sequence_window.sequence_window_task_icon);
    }
}

/******************************************************************************************/
/* Initialize task perperty values.                                                       */
/******************************************************************************************/
VOID task_properties_init()
{
    INT index;
    PROPERTY_INFO *pinfo;

    for (index = 0; index < sizeof(property_prompt_value_list) / sizeof(PROPERTY_INFO); index++)
    {
        pinfo = &property_prompt_value_list[index];

        gx_numeric_prompt_value_set((GX_NUMERIC_PROMPT *)pinfo->widget, pinfo->values[0]);
    }

    for (index = 0; index < sizeof(property_radial_progress_value_list) / sizeof(PROPERTY_INFO); index++)
    {
        pinfo = &property_radial_progress_value_list[index];

        gx_radial_progress_bar_value_set((GX_RADIAL_PROGRESS_BAR *)pinfo->widget, 0);
    }

    current_task_property_animation_step = 0;
}

/******************************************************************************************/
/* Update task property values.                                                           */
/******************************************************************************************/
VOID task_properties_update()
{
    INT start_val;
    INT target_val;
    INT value;
    INT index;
    PROPERTY_INFO *pinfo;

    if (current_task_property_animation_step < TASK_PROPERTY_ANIMATION_STEPS)
    {
        current_task_property_animation_step++;

        for (index = 0; index < sizeof(property_prompt_value_list) / sizeof(PROPERTY_INFO); index++)
        {
            pinfo = &property_prompt_value_list[index];

            target_val = pinfo->values[current_task_id];

            if (current_task_id == 0)
            {
                start_val = pinfo->values[TASK_COUNT - 1];
            }
            else
            {
                start_val = pinfo->values[current_task_id - 1];
            }

            value = start_val + (target_val - start_val) * current_task_property_animation_step / TASK_PROPERTY_ANIMATION_STEPS;

            gx_numeric_prompt_value_set((GX_NUMERIC_PROMPT*)pinfo->widget, value);
        }

        for (index = 0; index < sizeof(property_radial_progress_value_list) / sizeof(PROPERTY_INFO); index++)
        {
            pinfo = &property_radial_progress_value_list[index];

            target_val = pinfo->values[current_task_id];


            value = target_val * current_task_property_animation_step / TASK_PROPERTY_ANIMATION_STEPS;

            gx_radial_progress_bar_value_set((GX_RADIAL_PROGRESS_BAR*)pinfo->widget, value);
        }
    }
}

/******************************************************************************************/
/* Define a value format function for timer prompts.                                      */
/******************************************************************************************/
VOID timer_value_format(GX_NUMERIC_PROMPT *prompt, INT value)
{
    gx_utility_ltoa(value, prompt->gx_numeric_prompt_buffer, GX_NUMERIC_PROMPT_BUFFER_SIZE);

    if (value < 10)
    {
        prompt->gx_numeric_prompt_buffer[1] = prompt->gx_numeric_prompt_buffer[0];
        prompt->gx_numeric_prompt_buffer[0] = '0';
        prompt->gx_numeric_prompt_buffer[2] = '\0';
    }
}

/******************************************************************************************/
/* Define a value format function for value prompts in mode windows.                      */
/******************************************************************************************/
VOID mode_value_format(GX_NUMERIC_PROMPT *prompt, INT value)
{
    INT index;

    gx_utility_ltoa(value / 100, prompt->gx_numeric_prompt_buffer, GX_NUMERIC_PROMPT_BUFFER_SIZE);
    index = string_length_get(prompt->gx_numeric_prompt_buffer, GX_NUMERIC_PROMPT_BUFFER_SIZE - 1);
    prompt->gx_numeric_prompt_buffer[index++] = '.';
    
    if (value < 0)
    {
        value *= -1;
    }

    gx_utility_ltoa(value % 100, prompt->gx_numeric_prompt_buffer + index, GX_NUMERIC_PROMPT_BUFFER_SIZE - index);
}

/******************************************************************************************/
/* Define a value format function for position value prompts.                             */
/******************************************************************************************/
VOID position_value_format(GX_NUMERIC_PROMPT *prompt, INT value)
{
    INT index;

    gx_utility_ltoa(value / 10, prompt->gx_numeric_prompt_buffer, GX_NUMERIC_PROMPT_BUFFER_SIZE);
    index = string_length_get(prompt->gx_numeric_prompt_buffer, GX_NUMERIC_PROMPT_BUFFER_SIZE);
    prompt->gx_numeric_prompt_buffer[index++] = '.';
    gx_utility_ltoa(value % 10, prompt->gx_numeric_prompt_buffer + index, GX_NUMERIC_PROMPT_BUFFER_SIZE - index);
}
