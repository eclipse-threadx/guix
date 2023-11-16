
#include "gx_api.h"

GX_WINDOW_ROOT    *root;

#include "overlap_specifications.h"
#include "overlap_resources.h"

VOID start_guix(VOID);

extern UINT win32_graphics_driver_setup_565rgb(GX_DISPLAY *display);

/*******************************************************************************/
int main(int argc, char ** argv)
{
  tx_kernel_enter();
  return(0);
}

/*******************************************************************************/
VOID tx_application_define(void *first_unused_memory)
{
    start_guix();
}


/*******************************************************************************/
VOID  start_guix(VOID)
{

    /* Initialize GUIX.  */
    gx_system_initialize();

    gx_studio_display_configure(DISPLAY_1, win32_graphics_driver_setup_565rgb, 
		LANGUAGE_ENGLISH, DISPLAY_1_THEME_1, &root);

    /* create the bench1 screen */
    gx_studio_named_widget_create("w_main", (GX_WIDGET *) root, GX_NULL);

    /* create the gauge screen */
    gx_studio_named_widget_create("w_popup", GX_NULL, GX_NULL);

    /* Show the root window to make it and patients screen visible.  */
    gx_widget_show(root);

    /* start GUIX thread */
    gx_system_start();
}

int counterval = 0;
char counter_string[20];

UINT w_main_event(GX_WINDOW * p_self, GX_EVENT * p_event)
{
    GX_STRING string;

    switch (p_event->gx_event_type)
    {
    case GX_EVENT_SHOW:
        gx_system_timer_start(p_self, 1, 5, 5);
        break;

    case GX_EVENT_TIMER:
        counterval++;
        gx_utility_ltoa(counterval, counter_string, 20);
        string.gx_string_ptr = counter_string;
        string.gx_string_length = strnlen(counter_string, sizeof(counter_string));
        gx_prompt_text_set_ext(&w_main.w_main_counter, &string);
        //gx_prompt_text_set(&w_main.w_main_counter2, counter_string);
        break;

    case GX_SIGNAL(ID_BTN_1, GX_EVENT_CLICKED):
#if (0) /* This doesn't work */
        gx_widget_attach(p_self->gx_widget_parent, &w_popup);
        gx_window_execute(&w_popup, GX_NULL);
#else /* This does work */
        gx_widget_attach(p_self, &w_popup);
#endif
        break;
    }
    return gx_window_event_process(p_self, p_event);
}

UINT popup_events(GX_WINDOW * p_self, GX_EVENT * p_event)
{
    if (p_event->gx_event_type == GX_SIGNAL(IDB_CLOSE, GX_EVENT_CLICKED))
    {
        gx_widget_detach(p_self);
        return (IDB_CLOSE);
    }
    return gx_window_event_process(p_self, p_event);
}
