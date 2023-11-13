/* This is a small demo of the high-performance GUIX graphics framework. */

#include "demo_guix_industrial.h"

#define SEQUENCE_TIMER       10
#define SEQUENCE_TIMER_TICKS 1
#define MODE_WINDOW_ANIMATION_STEPS 10
#define POSITION_ANIMATION_STEPS    10
#define LEFT_PROGRESS_BAR_INCREMENT  3
#define RIGHT_PROGRESS_BAR_INCREMENT 4

#define APP_EVENT_TX_TIMER GX_FIRST_APP_EVENT

#define MODE_TITLE_ANIMATION_STEPS   8
#define MODE_TITLE_ANIMATION_SHIFT   60
#define MODE_TITLE_ANIMATION_START_X 19

enum mode_ids {
    MODE_ID_INSPECTING = 0,
    MODE_ID_ASSEMBLING,
    MODE_ID_WELDING,
    MODE_ID_PALLETIZING
};


static GX_CONST GX_CHAR string_surface[] = "Surface";
static GX_CONST GX_CHAR string_components[] = "Components";
static GX_CONST GX_CHAR string_tracing_pattern[]= "Tracing Pattern";
static GX_CONST GX_CHAR string_circuit_test[] = "Circuit Test";
static GX_CONST GX_CHAR string_inspecting[] = "Inspecting";
static GX_CONST GX_CHAR string_initialize_keys[] = "Initialize Keys";
static GX_CONST GX_CHAR string_rotate_axis[] = "Rotate Axis";
static GX_CONST GX_CHAR string_drive_press[] = "Drive Press";
static GX_CONST GX_CHAR string_force_join[] = "Force Join";
static GX_CONST GX_CHAR string_assembling[] = "Assembling";
static GX_CONST GX_CHAR string_current_check[] = "Current Check";
static GX_CONST GX_CHAR string_target_clamps[] = "Target Clamps";
static GX_CONST GX_CHAR string_brazing_flux[] = "Brazing & Flux";
static GX_CONST GX_CHAR string_ground_flush[] = "Ground Flush";
static GX_CONST GX_CHAR string_welding[] = "Welding";
static GX_CONST GX_CHAR string_level_surface[] = "Level Surface";
static GX_CONST GX_CHAR string_arm_auxiliary[] = "Arm Auxiliary";
static GX_CONST GX_CHAR string_pallet_friction[] = "Pallet Friction";
static GX_CONST GX_CHAR string_unitset[] = "UnitSet";
static GX_CONST GX_CHAR string_palletizing[] = "Palletizing";

static int timer_minute;
static int timer_second;
static int timer_tick;

static int SX_values[] = { 75, 70, 79, 68 };
static int SY_values[] = { 72, 69, 74, 63 };
static int RX_values[] = { -1050, -1050, 9290, -1050 };
static int RY_values[] = { 13650, 13650, 3870, 13650 };
static int UP_values[] = { 112, 423, 1822, 205 };
static int DN_values[] = { 8, 217, 1436, 129 };

static int speed_progress_values[] = { 65, 61, 68, 61 };
static int rotation_progress_values[] = { 81, 76, 26, 76 };

static int X_values[] = { 814, 756, 903, 816, 792 };
static int Y_values[] = { 734, 680, 588, 734, 839};
static int Z_values[] = { 387, 360, 373, 388, 408 };

static int current_speed_progress_value = 0;
static int current_rotation_progress_value = 0;
static int current_force_progress_value = 0;

static int current_progress_1_value = 0;
static int current_progress_2_value = 0;
static int current_progress_3_value = 0;
static int current_progress_4_value = 0;
static int current_mode_window_animation_step = MODE_WINDOW_ANIMATION_STEPS;
static int current_mode_title_animation_step = MODE_WINDOW_ANIMATION_STEPS;
static int current_mode = MODE_ID_INSPECTING;
static int current_position_value_index = 0;
static int current_position_animation_step = 0;

static int mode_title_blend_alpha = 255;
extern USHORT toggle_animation_flag;

VOID sequence_window_init();
VOID mode_set(int new_mode);
VOID left_progress_bars_update();
VOID bottom_progress_bar_update();
VOID right_mode_window_update();
VOID position_value_update();
VOID mode_title_position_udpate();
UINT string_length_get(GX_CONST GX_CHAR* input_string, UINT max_string_length);


/******************************************************************************************/
/* Override event processing of "sequence_window" to handle signals from my child widgets.*/
/******************************************************************************************/
UINT sequence_window_event_process(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_SHOW:
        gx_window_event_process(window, event_ptr);
        sequence_window_init();
        break;
       
    case GX_EVENT_HIDE:
        gx_window_event_process(window, event_ptr);
        gx_system_timer_stop(window, SEQUENCE_TIMER);
        break;

    case GX_EVENT_TIMER:
        if (event_ptr->gx_event_payload.gx_event_timer_id == SEQUENCE_TIMER)
        {
            sequence_animation_update();
        }
        break;

    case GX_SIGNAL(ID_REFRESH, GX_EVENT_CLICKED):
        mode_set(MODE_ID_INSPECTING);
        sequence_animation_start();
        break;

    case GX_SIGNAL(ID_INSPECTING, GX_EVENT_CLICKED):
        mode_set(MODE_ID_INSPECTING);
        break;
    case GX_SIGNAL(ID_ASSEMBLING, GX_EVENT_CLICKED):
        mode_set(MODE_ID_ASSEMBLING);
        break;
    case GX_SIGNAL(ID_WELDING, GX_EVENT_CLICKED):
        mode_set(MODE_ID_WELDING);
        break;
    case GX_SIGNAL(ID_PALLETIZING, GX_EVENT_CLICKED):
        mode_set(MODE_ID_PALLETIZING);
        break;

    default:
        return gx_window_event_process(window, event_ptr);
    }

    return 0;
}

/******************************************************************************************/
/* Override drawing of "mode_icon" to draw the icon with specified blending alpha.        */
/******************************************************************************************/
VOID mode_icon_draw(GX_ICON *icon)
{
    GX_BRUSH *brush;

    if ((toggle_animation_flag == ANIMATION_NONE))
    {
        gx_context_brush_get(&brush);
        brush->gx_brush_alpha = mode_title_blend_alpha;
    }

    gx_icon_draw(icon);
}

/******************************************************************************************/
/* Override drawing of "mode_title" to draw the prompt with specified blending alpha.     */
/******************************************************************************************/
VOID mode_title_draw(GX_PROMPT *prompt)
{
    GX_BRUSH *brush;

    if ((toggle_animation_flag == ANIMATION_NONE))
    {
        gx_context_brush_get(&brush);
        brush->gx_brush_alpha = mode_title_blend_alpha;
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

    /* Init mode values. */
    gx_numeric_prompt_value_set(&sequence_window.sequence_window_prompt_SX, SX_values[0]);
    gx_numeric_prompt_value_set(&sequence_window.sequence_window_prompt_SY, SY_values[0]);
    gx_numeric_prompt_value_set(&sequence_window.sequence_window_prompt_RX, RX_values[0]);
    gx_numeric_prompt_value_set(&sequence_window.sequence_window_prompt_RY, RY_values[0]);
    gx_numeric_prompt_value_set(&sequence_window.sequence_window_prompt_UP, UP_values[0]);
    gx_numeric_prompt_value_set(&sequence_window.sequence_window_prompt_DN, DN_values[0]);

    /* Init bottom progress bar values. */
    gx_progress_bar_value_set(&sequence_window.sequence_window_bottom_progress, 0);
    gx_numeric_prompt_value_set(&sequence_window.sequence_window_bottom_progress_value, 0);

    /* Init coordinate values. */
    gx_numeric_prompt_value_set(&sequence_window.sequence_window_prompt_X, X_values[0]);
    gx_numeric_prompt_value_set(&sequence_window.sequence_window_prompt_Y, Y_values[0]);
    gx_numeric_prompt_value_set(&sequence_window.sequence_window_prompt_Z, Z_values[0]);

    mode_set(MODE_ID_INSPECTING);
}

/******************************************************************************************/
/* Start sequence animation.                                                              */
/******************************************************************************************/
VOID sequence_animation_start()
{
    /* Create a sequence timer. */
    gx_system_timer_start(&sequence_window, SEQUENCE_TIMER, SEQUENCE_TIMER_TICKS, SEQUENCE_TIMER_TICKS);

}

/******************************************************************************************/
/* Process one step for sequence animation.                                               */
/******************************************************************************************/
VOID sequence_animation_update()
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

    mode_title_position_udpate();
    right_mode_window_update();
    left_progress_bars_update();
    bottom_progress_bar_update();
    position_value_update();
}

/******************************************************************************************/
/* Stop sequence animation.                                                               */
/******************************************************************************************/
VOID sequence_animation_stop()
{
    gx_system_timer_stop(&sequence_window, SEQUENCE_TIMER);
    screen_toggle_animation_start((GX_WINDOW *)&complete_window, (GX_WINDOW *)&sequence_window);
}

/******************************************************************************************/
/* Set title text.                                                                        */
/******************************************************************************************/
VOID title_text_set(GX_STRING *progress_1_title, GX_STRING *progress_2_title, GX_STRING *progress_3_title,
                    GX_STRING *progress_4_title, GX_STRING *mode_title)
{
    gx_prompt_text_set_ext(&sequence_window.sequence_window_progress_1_title, progress_1_title);
    gx_prompt_text_set_ext(&sequence_window.sequence_window_progress_2_title, progress_2_title);
    gx_prompt_text_set_ext(&sequence_window.sequence_window_progress_3_title, progress_3_title);
    gx_prompt_text_set_ext(&sequence_window.sequence_window_progress_4_title, progress_4_title);
    gx_prompt_text_set_ext(&sequence_window.sequence_window_mode_title, mode_title);
}

/******************************************************************************************/
/* Set sequence mode.                                                                     */
/******************************************************************************************/
VOID mode_set(int new_mode)
{
    GX_RECTANGLE *size;
    INT shift; 
    GX_STRING progress_1_title;
    GX_STRING progress_2_title;
    GX_STRING progress_3_title;
    GX_STRING progress_4_title;
    GX_STRING mode_title;

    if (new_mode > MODE_ID_PALLETIZING)
    {
        sequence_animation_stop();
        return;
    }

    current_mode = new_mode;
    current_progress_1_value = 0;
    current_progress_2_value = 0;
    current_progress_3_value = 0;
    current_progress_4_value = 0;

    current_speed_progress_value = 0;
    current_rotation_progress_value = 0;
    current_force_progress_value = 0;

    switch (new_mode)
    {
    case MODE_ID_INSPECTING:
        progress_1_title.gx_string_ptr = string_surface;
        progress_1_title.gx_string_length = sizeof(string_surface) - 1;
        progress_2_title.gx_string_ptr = string_components;
        progress_2_title.gx_string_length = sizeof(string_components) - 1;
        progress_3_title.gx_string_ptr = string_tracing_pattern;
        progress_3_title.gx_string_length = sizeof(string_tracing_pattern) - 1;
        progress_4_title.gx_string_ptr = string_circuit_test;
        progress_4_title.gx_string_length = sizeof(string_circuit_test) - 1;
        mode_title.gx_string_ptr = string_inspecting;
        mode_title.gx_string_length = sizeof(string_inspecting) - 1;

        button_indicator_attach(&main_screen.main_screen_button_inspecting);
        break;

    case MODE_ID_ASSEMBLING:
        progress_1_title.gx_string_ptr =  string_initialize_keys;
        progress_1_title.gx_string_length = sizeof(string_initialize_keys) - 1;
        progress_2_title.gx_string_ptr = string_rotate_axis;
        progress_2_title.gx_string_length = sizeof(string_rotate_axis) - 1;
        progress_3_title.gx_string_ptr =  string_drive_press;
        progress_3_title.gx_string_length = sizeof(string_drive_press) - 1;
        progress_4_title.gx_string_ptr = string_force_join;
        progress_4_title.gx_string_length = sizeof(string_force_join) - 1;
        mode_title.gx_string_ptr = string_assembling;
        mode_title.gx_string_length = sizeof(string_assembling);

        button_indicator_attach(&main_screen.main_screen_button_assembling);
        break;

    case MODE_ID_WELDING:
        progress_1_title.gx_string_ptr = string_current_check;
        progress_1_title.gx_string_length = sizeof(string_current_check) - 1;
        progress_2_title.gx_string_ptr = string_target_clamps;
        progress_2_title.gx_string_length = sizeof(string_target_clamps) - 1;
        progress_3_title.gx_string_ptr = string_brazing_flux;
        progress_3_title.gx_string_length = sizeof(string_brazing_flux) - 1;
        progress_4_title.gx_string_ptr = string_ground_flush;
        progress_4_title.gx_string_length = sizeof(string_ground_flush) - 1;
        mode_title.gx_string_ptr = string_welding;
        mode_title.gx_string_length = sizeof(string_welding) - 1;

        button_indicator_attach(&main_screen.main_screen_button_welding);
        break;

    case MODE_ID_PALLETIZING:
        progress_1_title.gx_string_ptr = string_level_surface;
        progress_1_title.gx_string_length = sizeof(string_level_surface) - 1;
        progress_2_title.gx_string_ptr = string_arm_auxiliary;
        progress_2_title.gx_string_length = sizeof(string_arm_auxiliary) - 1;
        progress_3_title.gx_string_ptr = string_pallet_friction;
        progress_3_title.gx_string_length = sizeof(string_pallet_friction) - 1;
        progress_4_title.gx_string_ptr = string_unitset;
        progress_4_title.gx_string_length = sizeof(string_unitset) - 1;
        mode_title.gx_string_ptr = string_palletizing;
        mode_title.gx_string_length = sizeof(string_palletizing) - 1;

        button_indicator_attach(&main_screen.main_screen_button_palletizing);
        break;
    }

    /* Set title text. */
    title_text_set(&progress_1_title, &progress_2_title, &progress_3_title, &progress_4_title, &mode_title);

    /* Hide progress complete icon. */
    gx_widget_hide((GX_WIDGET *)&sequence_window.sequence_window_complete_icon_1);
    gx_widget_hide((GX_WIDGET *)&sequence_window.sequence_window_complete_icon_2);
    gx_widget_hide((GX_WIDGET *)&sequence_window.sequence_window_complete_icon_3);
    gx_widget_hide((GX_WIDGET *)&sequence_window.sequence_window_complete_icon_4);

    gx_widget_show(&sequence_window.sequence_window_progress_1_value);
    gx_widget_show(&sequence_window.sequence_window_progress_2_value);
    gx_widget_show(&sequence_window.sequence_window_progress_3_value);
    gx_widget_show(&sequence_window.sequence_window_progress_4_value);

    /* Init left progress values. */
    gx_numeric_prompt_value_set(&sequence_window.sequence_window_progress_1_value, 0);
    gx_numeric_prompt_value_set(&sequence_window.sequence_window_progress_2_value, 0);
    gx_numeric_prompt_value_set(&sequence_window.sequence_window_progress_3_value, 0);
    gx_numeric_prompt_value_set(&sequence_window.sequence_window_progress_4_value, 0);

    gx_progress_bar_value_set(&sequence_window.sequence_window_progress_1, 0);
    gx_progress_bar_value_set(&sequence_window.sequence_window_progress_2, 0);
    gx_progress_bar_value_set(&sequence_window.sequence_window_progress_3, 0);
    gx_progress_bar_value_set(&sequence_window.sequence_window_progress_4, 0);

    /* Init right radial progress bar values. */
    gx_radial_progress_bar_value_set(&sequence_window.sequence_window_radial_progress_speed, 0);
    gx_radial_progress_bar_value_set(&sequence_window.sequence_window_radial_progress_rotation, 0);
    gx_radial_progress_bar_value_set(&sequence_window.sequence_window_radial_progress_force, 0);

    if (new_mode != MODE_ID_INSPECTING)
    {
        current_mode_window_animation_step = 0;
        current_mode_title_animation_step = 0;
        mode_title_blend_alpha = 255;

        size = &sequence_window.sequence_window_mode_title.gx_widget_size;

        if (size->gx_rectangle_left != MODE_TITLE_ANIMATION_START_X)
        {
            shift = MODE_TITLE_ANIMATION_START_X - size->gx_rectangle_left;
            gx_widget_shift((GX_WIDGET *)&sequence_window.sequence_window_mode_title, shift, 0, GX_TRUE);
            gx_widget_shift((GX_WIDGET *)&sequence_window.sequence_window_mode_icon, shift, 0, GX_TRUE);
        }
    }
}

/******************************************************************************************/
/* Update progress bar values in left.                                                    */
/******************************************************************************************/
VOID left_progress_bars_update()
{
    if (current_progress_1_value < 100)
    {
        current_progress_1_value += LEFT_PROGRESS_BAR_INCREMENT;
        gx_progress_bar_value_set(&sequence_window.sequence_window_progress_1, current_progress_1_value);
        gx_numeric_prompt_value_set(&sequence_window.sequence_window_progress_1_value, current_progress_1_value);

        if (current_progress_1_value >= 100)
        {
            gx_widget_hide(&sequence_window.sequence_window_progress_1_value);
            gx_widget_show(&sequence_window.sequence_window_complete_icon_1);
        }
    }

    if (current_progress_1_value >= 25)
    {
        if (current_progress_2_value < 100)
        {
            current_progress_2_value += LEFT_PROGRESS_BAR_INCREMENT;
            gx_progress_bar_value_set(&sequence_window.sequence_window_progress_2, current_progress_2_value);
            gx_numeric_prompt_value_set(&sequence_window.sequence_window_progress_2_value, current_progress_2_value);

            if (current_progress_2_value >= 100)
            {
                gx_widget_hide(&sequence_window.sequence_window_progress_2_value);
                gx_widget_show(&sequence_window.sequence_window_complete_icon_2);
            }
        }
    }
    if (current_progress_2_value >= 25)
    {
        if (current_progress_3_value < 100)
        {
            current_progress_3_value += LEFT_PROGRESS_BAR_INCREMENT;
            gx_progress_bar_value_set(&sequence_window.sequence_window_progress_3, current_progress_3_value);
            gx_numeric_prompt_value_set(&sequence_window.sequence_window_progress_3_value, current_progress_3_value);

            if (current_progress_3_value >= 100)
            {
                gx_widget_hide(&sequence_window.sequence_window_progress_3_value);
                gx_widget_show(&sequence_window.sequence_window_complete_icon_3);
            }
        }
    }
    if (current_progress_3_value >= 25)
    {
        if (current_progress_4_value < 100)
        {
            current_progress_4_value += LEFT_PROGRESS_BAR_INCREMENT;
            gx_progress_bar_value_set(&sequence_window.sequence_window_progress_4, current_progress_4_value);
            gx_numeric_prompt_value_set(&sequence_window.sequence_window_progress_4_value, current_progress_4_value);

            if (current_progress_4_value >= 100)
            {
                gx_widget_hide(&sequence_window.sequence_window_progress_4_value);
                gx_widget_show(&sequence_window.sequence_window_complete_icon_4);
            }
        }
    }

    if (current_progress_4_value >= 100)
    {
        mode_set(current_mode + 1);
    }
}

/******************************************************************************************/
/* Udpate progress bar values in bottom.                                                  */
/******************************************************************************************/
VOID bottom_progress_bar_update()
{
    int value;

    switch (current_mode)
    {
    case MODE_ID_INSPECTING:
        value = 0;
        break;

    case MODE_ID_ASSEMBLING:
        value = 25;
        break;

    case MODE_ID_WELDING:
        value = 50;
        break;

    case MODE_ID_PALLETIZING:
        value = 75;
        break;
    }

    if (current_progress_4_value)
    {
        value += (75 + current_progress_4_value) * 25 / 175;
    }
    else
    {
        value += (current_progress_1_value)* 25 / 175;
    }

    gx_progress_bar_value_set(&sequence_window.sequence_window_bottom_progress, value);
    gx_numeric_prompt_value_set(&sequence_window.sequence_window_bottom_progress_value, value);
}

/******************************************************************************************/
/* Udpate position of mode title/icon widgets in left.                                    */
/******************************************************************************************/
VOID mode_title_position_udpate()
{
    INT shift;

    if (current_mode_title_animation_step < MODE_TITLE_ANIMATION_STEPS)
    {
        current_mode_title_animation_step++;
        if (current_mode_title_animation_step <= MODE_TITLE_ANIMATION_STEPS / 2)
        {
            shift = MODE_TITLE_ANIMATION_SHIFT;
            mode_title_blend_alpha -= 50;
        }
        else
        {
            shift = -MODE_TITLE_ANIMATION_SHIFT;
            mode_title_blend_alpha += 50;
        }

        gx_widget_shift((GX_WIDGET *)&sequence_window.sequence_window_mode_title, shift, 0, GX_TRUE);
        gx_widget_shift((GX_WIDGET *)&sequence_window.sequence_window_mode_icon, shift, 0, GX_TRUE);
    }
}

/******************************************************************************************/
/* Update values of mode windows in right.                                                */
/******************************************************************************************/
VOID right_mode_window_update()
{
    INT value;

    if (current_mode_window_animation_step < MODE_WINDOW_ANIMATION_STEPS)
    {
        current_mode_window_animation_step++;
        value = SX_values[current_mode - 1] + (SX_values[current_mode] - SX_values[current_mode - 1]) * current_mode_window_animation_step / MODE_WINDOW_ANIMATION_STEPS;
        gx_numeric_prompt_value_set(&sequence_window.sequence_window_prompt_SX, value);

        value = SY_values[current_mode - 1] + (SY_values[current_mode] - SY_values[current_mode - 1]) * current_mode_window_animation_step / MODE_WINDOW_ANIMATION_STEPS;
        gx_numeric_prompt_value_set(&sequence_window.sequence_window_prompt_SY, value);

        value = RX_values[current_mode - 1] + (RX_values[current_mode] - RX_values[current_mode - 1]) * current_mode_window_animation_step / MODE_WINDOW_ANIMATION_STEPS;
        gx_numeric_prompt_value_set(&sequence_window.sequence_window_prompt_RX, value);

        value = RY_values[current_mode - 1] + (RY_values[current_mode] - RY_values[current_mode - 1]) * current_mode_window_animation_step / MODE_WINDOW_ANIMATION_STEPS;
        gx_numeric_prompt_value_set(&sequence_window.sequence_window_prompt_RY, value);

        value = UP_values[current_mode - 1] + (UP_values[current_mode] - UP_values[current_mode - 1]) * current_mode_window_animation_step / MODE_WINDOW_ANIMATION_STEPS;
        gx_numeric_prompt_value_set(&sequence_window.sequence_window_prompt_UP, value);

        value = DN_values[current_mode - 1] + (DN_values[current_mode] - DN_values[current_mode - 1]) * current_mode_window_animation_step / MODE_WINDOW_ANIMATION_STEPS;
        gx_numeric_prompt_value_set(&sequence_window.sequence_window_prompt_DN, value);
    }

    if (current_speed_progress_value < speed_progress_values[current_mode])
    {
        current_speed_progress_value += RIGHT_PROGRESS_BAR_INCREMENT;
        gx_radial_progress_bar_value_set(&sequence_window.sequence_window_radial_progress_speed, -current_speed_progress_value * 360 / 100);
    }

    if (current_rotation_progress_value < rotation_progress_values[current_mode])
    {
        current_rotation_progress_value += RIGHT_PROGRESS_BAR_INCREMENT;
        gx_radial_progress_bar_value_set(&sequence_window.sequence_window_radial_progress_rotation, -current_rotation_progress_value * 360 / 100);
    }

    if (current_force_progress_value < speed_progress_values[current_mode])
    {
        current_force_progress_value += RIGHT_PROGRESS_BAR_INCREMENT;
        gx_radial_progress_bar_value_set(&sequence_window.sequence_window_radial_progress_force, -current_force_progress_value * 360 / 100);
    }
}

/******************************************************************************************/
/* Update position values in top.                                                         */
/******************************************************************************************/
VOID position_value_update()
{
    INT value;
    INT target_x_value;
    INT target_y_value;
    INT target_z_value;

    if (current_position_animation_step < POSITION_ANIMATION_STEPS)
    {
        current_position_animation_step++;
        if (current_position_value_index + 1 == sizeof(X_values) / sizeof(INT))
        {
            target_x_value = X_values[0];
            target_y_value = Y_values[0];
            target_z_value = Z_values[0];
        }
        else
        {
            target_x_value = X_values[current_position_value_index + 1];
            target_y_value = Y_values[current_position_value_index + 1];
            target_z_value = Z_values[current_position_value_index + 1];
        }

        value = X_values[current_position_value_index] + (target_x_value - X_values[current_position_value_index]) * current_position_animation_step / POSITION_ANIMATION_STEPS;
        gx_numeric_prompt_value_set(&sequence_window.sequence_window_prompt_X, value);

        value = Y_values[current_position_value_index] + (target_y_value - Y_values[current_position_value_index]) * current_position_animation_step / POSITION_ANIMATION_STEPS;
        gx_numeric_prompt_value_set(&sequence_window.sequence_window_prompt_Y, value);

        value = Z_values[current_position_value_index] + (target_z_value - Z_values[current_position_value_index]) * current_position_animation_step / POSITION_ANIMATION_STEPS;
        gx_numeric_prompt_value_set(&sequence_window.sequence_window_prompt_Z, value);
    }
    else
    {
        current_position_animation_step = 0;
        current_position_value_index++;
        if (current_position_value_index >= (int)(sizeof(X_values) / sizeof(INT)))
        {
            current_position_value_index = 0;
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
/* Define a value format function for progress bar value prompts.                         */
/******************************************************************************************/
VOID progress_value_format(GX_NUMERIC_PROMPT *prompt, INT value)
{
    gx_utility_ltoa(value, prompt->gx_numeric_prompt_buffer, GX_NUMERIC_PROMPT_BUFFER_SIZE);
    memcpy(prompt->gx_numeric_prompt_buffer, "%", 2);
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
