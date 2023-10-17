
#include "demo_guix_washing_machine.h"

/* Define progress bar animation timer ID.  */
#define ID_TIMER_PROGRESS_BAR 1

/* Define progress bar animation properties.  */
#define PROGRESS_BAR_ANIMATION_TOTAL_STEPS    9
#define PROGRESS_BAR_ANIMATION_END_VAL       -270

/* Define variables.  */
static INT progress_bar_animation_step = 10;
static INT selected_button_id = ID_BTN_ECO;

/* Define MODE button control block.  */
typedef struct MODE_BTN_INFO_STRUCT {
    TEMPLATE_MODE_BTN_CONTROL_BLOCK *button;
    TEMPLATE_MODE_BTN_SMALL_CONTROL_BLOCK* button_small;
    GX_RESOURCE_ID text_id;
    GX_RESOURCE_ID map_id;
    GX_RESOURCE_ID map_id_small;
    INT minutes;
}MODE_BTN_INFO;

/* Define mode button properties.  */
MODE_BTN_INFO mode_button_info_list[] = {
    {&mode_select_window.mode_select_window_btn_quick, &mode_select_window.mode_select_window_btn_quick_small, GX_STRING_ID_QUICK, GX_PIXELMAP_ID_CYCLE_QUICK, GX_PIXELMAP_ID_CYCLE_QUICK_SMALL, 30},
    {&mode_select_window.mode_select_window_btn_eco, &mode_select_window.mode_select_window_btn_eco_small, GX_STRING_ID_ECO, GX_PIXELMAP_ID_CYCLE_ECO, GX_PIXELMAP_ID_CYCLE_ECO_SMALL, 60},
    {&mode_select_window.mode_select_window_btn_deep, &mode_select_window.mode_select_window_btn_deep_small, GX_STRING_ID_DEEP, GX_PIXELMAP_ID_CYCLE_DEEP, GX_PIXELMAP_ID_CYCLE_DEEP_SMALL, 90},
    {GX_NULL, GX_NULL, 0, 0, 0, 0}
};

/******************************************************************************************/
/* Initialize mode buttons in mode select window.                                         */
/******************************************************************************************/
VOID mode_select_window_init()
{
    MODE_BTN_INFO *entry = mode_button_info_list;

    /* Loop through mode buttons.  */
    while (entry->button)
    {

        /* Set big mode button background map.  */
        gx_pixelmap_button_pixelmap_set((GX_PIXELMAP_BUTTON*)entry->button, entry->map_id, entry->map_id, entry->map_id);

        /* Set big mode button title text.  */
        gx_prompt_text_id_set(&entry->button->template_mode_btn_mode_label, entry->text_id);

        /* Set total time in big button.  */
        gx_numeric_prompt_value_set(&entry->button->template_mode_btn_total_time, entry->minutes);

        /* Set title text in big button.  */
        gx_prompt_text_id_set(&entry->button_small->template_mode_btn_small_mode_label, entry->text_id);

        /* Set total time in small button.  */
        gx_numeric_prompt_value_set(&entry->button_small->template_mode_btn_small_total_time, entry->minutes);

        /* Set small button background map.  */
        gx_pixelmap_button_pixelmap_set((GX_PIXELMAP_BUTTON*)entry->button_small, entry->map_id_small, entry->map_id_small, entry->map_id_small);

        if (selected_button_id != entry->button->gx_widget_id)
        {
            /* Hide big button.  */
            gx_widget_hide(entry->button);

            /* Hide progress bar in big button.  */
            gx_widget_hide(&entry->button->template_mode_btn_mode_progress_bar);
        }
        else
        {
            /* Hide small button.  */
            gx_widget_hide(entry->button_small);

            /* Set remain time in big button.  */
            gx_numeric_prompt_value_set(&entry->button->template_mode_btn_minute, entry->minutes);
            gx_numeric_prompt_value_set(&entry->button->template_mode_btn_second, 0);
        }

        entry++;
    }
}

/******************************************************************************************/
/* Get mode button information by button id.                                              */
/******************************************************************************************/
MODE_BTN_INFO *get_mode_button_info(INT button_id)
{
    MODE_BTN_INFO* entry = mode_button_info_list;

    while (entry->button)
    {
        if ((button_id == entry->button->gx_widget_id) ||
            (button_id == entry->button_small->gx_widget_id))
        {
            return entry;
        }

        entry++;
    }

    return GX_NULL;
}

/******************************************************************************************/
/* Select button by button id.                                                            */
/******************************************************************************************/
VOID button_select(INT button_id)
{
    MODE_BTN_INFO* info = get_mode_button_info(selected_button_id);

    if (info)
    {
        /* Detach the big button of the selected mode.  */
        gx_widget_detach(info->button);

        /* Attach the small button of the selected mode.  */
        gx_widget_attach(&mode_select_window, info->button_small); 
    }

    /* Get button information of the new selected mode.  */
    info = get_mode_button_info(button_id);

    if (info)
    {
        /* Detach the small button of the new selected mode.  */
        gx_widget_detach(info->button_small);

        /* Attach the big button of the new selected mode.  */
        gx_widget_attach(&mode_select_window, info->button);

        /* Set selected button id.  */
        selected_button_id = info->button->gx_widget_id;
    }
}

/******************************************************************************************/
/* Set cycle mode.                                                                        */
/******************************************************************************************/
VOID set_cycle_mode()
{
    MODE_BTN_INFO* entry = mode_button_info_list;

    while (entry->button)
    {
        if (selected_button_id == entry->button->gx_widget_id)
        {
            gx_prompt_text_id_set(&main_screen.main_screen_wash_cycle_mode, entry->text_id);
            gx_widget_detach(&mode_select_window);
            gx_widget_attach(entry->button, &entry->button->template_mode_btn_mode_progress_bar);
            gx_widget_attach(&main_screen, &wash_cycle_window);
            remain_time_set(entry->minutes * 60);
        }
        else
        {
            gx_widget_detach(&entry->button->template_mode_btn_mode_progress_bar);
        }

        entry++;
    }
}

/******************************************************************************************/
/* Update radial progress bar animation.                                                  */
/******************************************************************************************/
VOID update_radial_progress_bar()
{
    MODE_BTN_INFO* info = get_mode_button_info(selected_button_id);
    INT value;

    progress_bar_animation_step--;

    value = PROGRESS_BAR_ANIMATION_END_VAL *
            (PROGRESS_BAR_ANIMATION_TOTAL_STEPS - progress_bar_animation_step) /
            PROGRESS_BAR_ANIMATION_TOTAL_STEPS;

    gx_radial_progress_bar_value_set(&info->button->template_mode_btn_mode_progress_bar, value);

    if(progress_bar_animation_step == 0)
    {
        gx_system_timer_stop(&mode_select_window, ID_TIMER_PROGRESS_BAR);
    }
}

/******************************************************************************************/
/* Update remain time.                                                                    */
/******************************************************************************************/
VOID mode_select_window_remain_time_update(INT remain_seconds)
{
    MODE_BTN_INFO *info;

    if (remain_seconds)
    {
        info = get_mode_button_info(selected_button_id);
        gx_numeric_prompt_value_set(&info->button->template_mode_btn_minute, remain_seconds / 60);
        gx_numeric_prompt_value_set(&info->button->template_mode_btn_second, remain_seconds % 60);
    }
}

/******************************************************************************************/
/* Override the default event processing of "wash_cycle_select_window" to handle signals  */
/* from my child widgets.                                                                 */
/******************************************************************************************/
UINT mode_select_window_event_process(GX_WINDOW* window, GX_EVENT* event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
    case GX_SIGNAL(ID_BTN_QUICK, GX_EVENT_CLICKED):
    case GX_SIGNAL(ID_BTN_QUICK_SMALL, GX_EVENT_CLICKED):
    case GX_SIGNAL(ID_BTN_ECO, GX_EVENT_CLICKED):
    case GX_SIGNAL(ID_BTN_ECO_SMALL, GX_EVENT_CLICKED):
    case GX_SIGNAL(ID_BTN_DEEP, GX_EVENT_CLICKED):
    case GX_SIGNAL(ID_BTN_DEEP_SMALL, GX_EVENT_CLICKED):
        if (selected_button_id == event_ptr->gx_event_sender)
        {
            set_cycle_mode();
        }
        else
        {
            button_select(event_ptr->gx_event_sender);
        }
        break;

    case GX_EVENT_SHOW:
        gx_window_event_process(window, event_ptr);
        gx_widget_detach(&main_screen.main_screen_btn_play);
        gx_system_timer_start(window, ID_TIMER_PROGRESS_BAR, 20 / GX_SYSTEM_TIMER_MS, 20 / GX_SYSTEM_TIMER_MS);
        progress_bar_animation_step = 10;
        break;

    case GX_EVENT_HIDE:
        gx_window_event_process(window, event_ptr);

        gx_widget_attach(&main_screen, &main_screen.main_screen_btn_play);

        gx_system_timer_stop(window, 0);
        break;

    case GX_EVENT_TIMER:
        if (event_ptr->gx_event_payload.gx_event_timer_id == ID_TIMER_PROGRESS_BAR)
        {
            update_radial_progress_bar();
        }
        break;

    default:
        return gx_window_event_process(window, event_ptr);
    }

    return 0;
}