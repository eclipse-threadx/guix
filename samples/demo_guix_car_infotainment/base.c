/* This is a small demo of the high-performance GUIX graphics framework. */

#include "demo_guix_car_infotainment.h"

#if (MAIN_DISPLAY_X_RESOLUTION == 640)
#define SCREEN_WEATHER_WIN_OFF_SHIFT 84
#endif

static GX_RESOURCE_ID main_weather_icon_ids[] = { GX_PIXELMAP_ID_WEATHER_ICON_PARTLY_CLOUDY_OFF, GX_PIXELMAP_ID_WEATHER_ICON_PARTLY_CLOUDY };
static GX_RESOURCE_ID screen_weather_icon_ids[] = { GX_PIXELMAP_ID_WEATHER_ICON_PARTLY_CLOUDY_SMALL_OFF, GX_PIXELMAP_ID_WEATHER_ICON_PARTLY_CLOUDY_SMALL };
static GX_RESOURCE_ID logo_ids[] = { GX_PIXELMAP_ID_MICROSOFT_LOGO_OFF, GX_PIXELMAP_ID_MICROSOFT_AZURE_LOGO };
extern INT bottom_button_alpha;

/******************************************************************************************/
/* Deselect buttons in main screen.                                                       */
/******************************************************************************************/
VOID screen_btn_deselect()
{
    GX_WIDGET *child = main_screen.main_screen_btn_window.gx_widget_first_child;

    while (child)
    {
        if ((child->gx_widget_type == GX_TYPE_PIXELMAP_BUTTON) &&
            (child->gx_widget_style & GX_STYLE_BUTTON_PUSHED))
        {
            ((GX_BUTTON *)child)->gx_button_deselect_handler(child, GX_FALSE);

            return;
        }

        child = child->gx_widget_next;
       
    }
}

/******************************************************************************************/
/* Enable/Disable child widgets.                                                          */
/******************************************************************************************/
VOID enable_disable_widget(GX_WIDGET *parent, GX_BOOL enable)
{
    GX_WIDGET* child = parent->gx_widget_first_child;

    if (enable)
    {
        gx_widget_style_add(parent, GX_STYLE_ENABLED);
    }
    else
    {
        gx_widget_style_remove(parent, GX_STYLE_ENABLED);
    }

    while (child)
    {
        enable_disable_widget(child, enable);

        child = child->gx_widget_next;
    }
}

/******************************************************************************************/
/* Show/Hide screen contents.                                                             */
/******************************************************************************************/
VOID show_hide_screen_contents(GX_WINDOW* parent, GX_BOOL show)
{
    GX_WIDGET *child = parent->gx_widget_first_child;
    INT shift;

    while (child)
    {
        switch (child->gx_widget_id)
        {
        case ID_MAIN_WEATHER_WIN:
            enable_disable_widget(child, show);
            gx_icon_pixelmap_set(&main_screen.main_screen_weather_icon, main_weather_icon_ids[show], main_weather_icon_ids[show]);
            break;

        case ID_SCREEN_WEATHER_WIN:
            enable_disable_widget(child, show);
            gx_icon_pixelmap_set(&((SCREEN_BASE_CONTROL_BLOCK *)parent)->screen_base_weather_icon, screen_weather_icon_ids[show], screen_weather_icon_ids[show]);
 
            if (show)
            {
                shift = -SCREEN_WEATHER_WIN_OFF_SHIFT;
            }
            else
            {
                shift = SCREEN_WEATHER_WIN_OFF_SHIFT;
            }
            gx_widget_shift(child, shift, 0, GX_TRUE);
            break;

        case ID_BTN_WIN:
            enable_disable_widget(child, show);

            if (show)
            {
                bottom_button_alpha = 255;
            }
            else
            {
                bottom_button_alpha = 50;
            }
            break;

        case ID_LOGO:
            gx_icon_pixelmap_set((GX_ICON*)child, logo_ids[show] , logo_ids[show]);
            break;

        case ID_ON_OFF_BTN:
            break;

        default:
            if (show)
            {
                gx_widget_show(child);
            }
            else
            {
                gx_widget_hide(child);
            }
            break;
        }

        child = child->gx_widget_next;
    }
}

/******************************************************************************************/
/* Override the default event processing of "base" to handle signals from my child        */
/* widgets.                                                                               */
/******************************************************************************************/
UINT base_event_process(GX_WINDOW* window, GX_EVENT* event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
        case GX_SIGNAL(ID_VEHICLE_BTN, GX_EVENT_RADIO_SELECT):
            ToggleScreen((GX_WINDOW*)&vehicle_screen);
            break;

        case GX_SIGNAL(ID_CLIMATE_BTN, GX_EVENT_RADIO_SELECT):
            ToggleScreen((GX_WINDOW*)&climate_screen);
            break;

        case GX_SIGNAL(ID_PHONE_BTN, GX_EVENT_RADIO_SELECT):
            ToggleScreen((GX_WINDOW*)&phone_screen);
            break;

        case GX_SIGNAL(ID_AUDIO_BTN, GX_EVENT_RADIO_SELECT):
            ToggleScreen((GX_WINDOW*)&audio_screen);
            break;

        case GX_SIGNAL(ID_VIDEO_BTN, GX_EVENT_RADIO_SELECT):
            ToggleScreen((GX_WINDOW*)&video_screen);
            break;

        case GX_SIGNAL(ID_ON_OFF_BTN, GX_EVENT_CLICKED):
            if (main_screen.main_screen_on_off_text.gx_prompt_text_id == GX_STRING_ID_OFF)
            {
                gx_window_wallpaper_set(window, GX_PIXELMAP_ID_BG_OFF, GX_PIXELMAP_ID_BG_OFF);
                show_hide_screen_contents(window, GX_FALSE);
                gx_prompt_text_id_set(&main_screen.main_screen_on_off_text, GX_STRING_ID_ON);
            }
            else
            {
                gx_window_wallpaper_set(window, GX_PIXELMAP_ID_BG_IMAGE, GX_PIXELMAP_ID_BG_IMAGE);
                show_hide_screen_contents(window, GX_TRUE);
                gx_prompt_text_id_set(&main_screen.main_screen_on_off_text, GX_STRING_ID_OFF);
            }
            break;

        default:
            return gx_window_event_process(window, event_ptr);
    }

    return 0;
}

/******************************************************************************************/
/* Override the default event processing of "screen_base" to handle signals from my       */
/* child widgets.                                                                         */
/******************************************************************************************/
UINT screen_base_event_process(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
    case GX_SIGNAL(ID_HOME_BTN, GX_EVENT_CLICKED):
        ToggleScreen((GX_WINDOW *)&main_screen);
        screen_btn_deselect();
        break;

    default:
        return base_event_process(window, event_ptr);
    }

    return 0;
}