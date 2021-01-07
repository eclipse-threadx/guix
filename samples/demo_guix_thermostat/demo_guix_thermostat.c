/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"
#include "guix_thermostat_resources.h"
#include "guix_thermostat_specifications.h"

#define            SCRATCHPAD_PIXELS (MAIN_DISPLAY_X_RESOLUTION * MAIN_DISPLAY_Y_RESOLUTION)
GX_COLOR           scratchpad[SCRATCHPAD_PIXELS];
TX_BYTE_POOL       memory_pool;


GX_WINDOW_ROOT    *root;
GX_CHAR           plus_text[] = "+";
GX_CHAR           minus_text[] = "-";
INT               i_temperature = 79;

/* Define prototypes.   */

VOID  start_guix(VOID);
UINT  win32_graphics_driver_setup_565rgb(GX_DISPLAY *display);

int main(int argc, char ** argv)
{
    tx_kernel_enter();
    return(0);
}

/******************************************************************************************/
VOID *memory_allocate(ULONG size)
{
    VOID *memptr;

    if (tx_byte_allocate(&memory_pool, &memptr, size, TX_NO_WAIT) == TX_SUCCESS)
    {
        return memptr;
    }
    return NULL;
}

/******************************************************************************************/
void memory_free(VOID *mem)
{
    tx_byte_release(mem);
}


/******************************************************************************************/
VOID tx_application_define(void *first_unused_memory)
{
    /* create byte pool for needle rotation to use */
    tx_byte_pool_create(&memory_pool, "scratchpad", scratchpad,
                        SCRATCHPAD_PIXELS * sizeof(GX_COLOR));

    start_guix();

    /* install our memory allocator and de-allocator */
    gx_system_memory_allocator_set(memory_allocate, memory_free);
}

/******************************************************************************************/
VOID  start_guix(VOID)
{
    /* Initialize GUIX.  */
    gx_system_initialize();

    gx_studio_display_configure(MAIN_DISPLAY, win32_graphics_driver_setup_565rgb, LANGUAGE_ENGLISH, MAIN_DISPLAY_DEFAULT_THEME, &root);

    /* create the thermometer screen */
    gx_studio_named_widget_create("thermometer_screen", (GX_WIDGET *) root, NULL);

    /* Show the root window to make it and patients screen visible.  */
    gx_widget_show(root);

    /* let GUIX thread run */
    gx_system_start();
}

/******************************************************************************************/
VOID custom_pixelmap_button_draw(GX_PIXELMAP_BUTTON *button)
{
INT      x_offset = 0;
INT      y_offset = 0;
GX_STRING text;

    gx_pixelmap_button_draw(button);


    switch (button->gx_widget_id)
    {
    case ID_PLUS_BTN:
        text.gx_string_ptr = plus_text;
        text.gx_string_length = sizeof(plus_text) - 1;
        break;

    case ID_MINUS_BTN:
        text.gx_string_ptr = minus_text;
        text.gx_string_length = sizeof(minus_text) - 1;
        break;

    }

    if (button->gx_widget_style & GX_STYLE_BUTTON_PUSHED)
    {
        x_offset++;
        y_offset++;
    }

    gx_widget_text_draw_ext((GX_WIDGET *)button, GX_COLOR_ID_TEXT_INPUT_FILL,
                        GX_FONT_ID_BOLD, &text,
                        x_offset, y_offset);
}

/******************************************************************************************/
void set_temperature_value()
{
GX_PROMPT *prompt;
static GX_CHAR str_value[10];
GX_STRING string;

    prompt = &thermometer_screen.thermometer_screen_value_prompt;

    gx_utility_ltoa(i_temperature, str_value, 10);
    string.gx_string_ptr = str_value;
    string.gx_string_length = sizeof(str_value) - 1;
    gx_prompt_text_set_ext(prompt, &string);
}

/******************************************************************************************/
UINT thermo_screen_event_handler(GX_WINDOW *widget, GX_EVENT *event_ptr)
{
GX_CIRCULAR_GAUGE *gauge;
INT                angle;

    gauge = &thermometer_screen.thermometer_screen_gauge;

    switch(event_ptr->gx_event_type)
    {
    case GX_SIGNAL(ID_PLUS_BTN, GX_EVENT_CLICKED):
        gx_circular_gauge_angle_get(gauge, &angle);
        if (angle < 90)
        {
            gx_circular_gauge_angle_set(gauge, angle + 5);
            i_temperature++;
            set_temperature_value();
        }
        break;

    case GX_SIGNAL(ID_MINUS_BTN, GX_EVENT_CLICKED):
        gx_circular_gauge_angle_get(gauge, &angle);

        if (angle > -185)
        {
            gx_circular_gauge_angle_set(gauge, angle - 5);
            i_temperature--;
            set_temperature_value();
        }
        break;

    default:
        return gx_window_event_process(widget, event_ptr);
    }
    return 0;
}


